
#include <CnFileInfo.h>
#include "CnFileInfo_p_win.h"

#include <windows.h>    // for Windows API
#include <shlwapi.h>    // for PathIsDirectory, PathFileExists
#include <shlobj.h>     // for IShellLink
#include <objbase.h>    // for CoInitialize
#include <winioctl.h>   // for FSCTL_GET_REPARSE_POINT
#ifndef MAXIMUM_REPARSE_DATA_BUFFER_SIZE
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE (16 * 1024)
#endif
#ifndef REPARSE_DATA_BUFFER
typedef struct _REPARSE_DATA_BUFFER {
    ULONG  ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG  Flags; // for symlinks: 0x1 relative
            WCHAR  PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR  PathBuffer[1];
        } MountPointReparseBuffer;
        struct {
            UCHAR  DataBuffer[1];
        } GenericReparseBuffer;
    };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#endif
#include <io.h>         // for _access
#include <direct.h>     // for _getcwd
#include <sys/stat.h>   // for _stat
#include <cstring>

#include <CnDir.h>

//#pragma comment(lib, "shlwapi.lib")

static CnFileInfoPrivateWin nullCnFileInfoPrivateWin;

CnFileInfoPrivate* getNullCnFileInfoPrivate() { return &nullCnFileInfoPrivateWin; }

CnFileInfoPrivate* createCnFileInfoPrivate() { return new CnFileInfoPrivateWin(); }

CnFileInfoPrivate* createCnFileInfoPrivate(const CnFileInfoPrivate* other) { return new CnFileInfoPrivateWin(*static_cast<const CnFileInfoPrivateWin*>(other)); }

const size_t CnFileInfo::RootPathBaseLength = 3; // e.g., "C:/"

// Normalizes root paths for Windows: 'C:\', 'C:', 'c:', 'c:////'  
// becomes 'C:/'
CnString CnFileInfo::normalizeRootPath(const CnString &path)
{
    if (path.length() < 2)
        return path;
    CnString newPath = path;
    const CnChar* s = newPath.data();

    CnChar c0 = s[0];
    bool isAlpha = (c0 >= 'a' && c0 <= 'z') || (c0 >= 'A' && c0 <= 'Z');
    if (isAlpha && s[1] == ':')
    {
        if (c0 >= 'a' && c0 <= 'z') 
            newPath[0] = (c0 - 'a' + 'A');
        if (s[2] == CN_PATH_SEP && s[3] != '\\' && s[3] != '/')
            return newPath;
        if (s[2] != '\\' && s[2] != '/')
        {
            newPath.insert(2, 1, CN_PATH_SEP);
            return newPath;
        }
        size_t i = 2;
        while (newPath[i] == '\\' || newPath[i] == '/')
            ++i;
        if (i > 2)
            newPath = newPath.replace(2, i - 2, CN_PATH_SEP);
    }
    return newPath;
}

bool CnFileInfo::exists(const CnString &fileName)
{
    return ::_access(fileName.chars(), 0) == 0;
}

bool CnFileInfo::isAbsolutePath(const CnString &fileName)
{
    return CnFileInfoPrivateWin::isAbsolutePath(fileName);
}

CnString CnFileInfo::absoluteFilePath() const
{
    if (this->isNull())
        return CnString();
    if (d_cast(d)->isAbsolute())
        return d->filePath;
    return CnDir::cwd() + CN_PATH_SEP + d->filePath;
}

CnString CnFileInfo::absolutePath() const
{
    if (this->isNull())
        return CnString();
    if (d_cast(d)->isAbsolute())
        return d->filePath;
    return CnDir::cwd() + CN_PATH_SEP + d->dirName;
}

CnString CnFileInfo::canonicalFilePath() const
{
	char buf[MAX_PATH];
    buf[0] = '\0';
    size_t len = ::GetFullPathNameA(d->filePath.chars(), MAX_PATH, buf, nullptr);
    if (len)
		return normalizePathSeparators(buf, len);
	return CnString();
}

CnString CnFileInfo::canonicalPath() const
{
	char buf[MAX_PATH];
	size_t len = ::GetFullPathNameA(d->dirName.chars(), MAX_PATH, buf, nullptr);
    if (len)
		return normalizePathSeparators(buf, len);
	return CnString();
}

CnString CnFileInfo::symLinkTarget() const
{
    if (!isSymLink())
        return CnString();

    // Helper: strip Win32 path prefixes like "\\?\" and "\??\"
    auto stripPrefixes = [](const std::string &sIn) -> std::string {
        if (sIn.rfind("\\\\?\\UNC\\", 0) == 0) {
            return std::string("\\\\") + sIn.substr(8); // "\\?\\UNC\\" -> "\\"
        }
        if (sIn.rfind("\\\\?\\", 0) == 0) {
            return sIn.substr(4);
        }
        if (sIn.rfind("\\??\\", 0) == 0) {
            return sIn.substr(4);
        }
        return sIn;
    };

    // Case 1: Windows Shortcut (.lnk) via COM
    if (isShortcut())
    {
        CnString result;
        HRESULT hrInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        bool needUninit = SUCCEEDED(hrInit);
        IShellLinkA* psl = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (void**)&psl);
        if (SUCCEEDED(hr) && psl)
        {
            IPersistFile* ppf = nullptr;
            if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (void**)&ppf)) && ppf)
            {
                // Convert path to wide for IPersistFile::Load
                wchar_t wpath[MAX_PATH];
                MultiByteToWideChar(CP_ACP, 0, d->filePath.chars(), -1, wpath, MAX_PATH);
                if (SUCCEEDED(ppf->Load(wpath, STGM_READ)))
                {
                    char target[MAX_PATH] = {0};
                    WIN32_FIND_DATAA wfd{};
                    if (SUCCEEDED(psl->GetPath(target, MAX_PATH, &wfd, SLGP_RAWPATH)) && target[0] != '\0')
                    {
                        result = normalizePathSeparators(CnString::fromChars(target));
                    }
                    else if (SUCCEEDED(psl->GetPath(target, MAX_PATH, &wfd, SLGP_UNCPRIORITY)) && target[0] != '\0')
                    {
                        result = normalizePathSeparators(CnString::fromChars(target));
                    }
                }
                ppf->Release();
            }
            psl->Release();
        }
        if (needUninit)
            CoUninitialize();

        if (result.isEmpty())
            return CnString();

        // Resolve relative target against the link's directory
        if (!CnFileInfoPrivateWin::isAbsolutePath(result))
            return normalizePathSeparators(d->dirName + CN_PATH_SEP + result);
        return result;
    }

    // Case 2: True NTFS symbolic link or junction (reparse point)
    HANDLE h = ::CreateFileA(d->filePath.chars(), 0,
                             FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                             nullptr, OPEN_EXISTING,
                             FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
                             nullptr);
    if (h == INVALID_HANDLE_VALUE)
        return CnString();

    BYTE buffer[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
    DWORD bytes = 0;
    CnString out;
    if (::DeviceIoControl(h, FSCTL_GET_REPARSE_POINT, nullptr, 0, buffer, sizeof(buffer), &bytes, nullptr))
    {
        auto rdb = reinterpret_cast<REPARSE_DATA_BUFFER*>(buffer);
        std::wstring wTarget;

        if (rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK)
        {
            auto &rb = rdb->SymbolicLinkReparseBuffer;
            USHORT off = rb.PrintNameLength ? rb.PrintNameOffset : rb.SubstituteNameOffset;
            USHORT len = rb.PrintNameLength ? rb.PrintNameLength : rb.SubstituteNameLength;
            const WCHAR* p = reinterpret_cast<const WCHAR*>(rb.PathBuffer) + (off / sizeof(WCHAR));
            wTarget.assign(p, len / sizeof(WCHAR));

            // Convert to multibyte
            int n = ::WideCharToMultiByte(CP_ACP, 0, wTarget.c_str(), (int)wTarget.size(), nullptr, 0, nullptr, nullptr);
            std::string s(n, '\0');
            if (n > 0)
                ::WideCharToMultiByte(CP_ACP, 0, wTarget.c_str(), (int)wTarget.size(), s.data(), n, nullptr, nullptr);
            s = stripPrefixes(s);

            // Resolve relative symlink if needed (Flags bit 0x1)
            if (rb.Flags & 0x1)
            {
                out = normalizePathSeparators(d->dirName + CN_PATH_SEP + CnString::fromChars(s.c_str())) ;
            }
            else
            {
                out = normalizePathSeparators(CnString::fromChars(s.c_str()));
            }
        }
        else if (rdb->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT)
        {
            auto &rb = rdb->MountPointReparseBuffer;
            USHORT off = rb.PrintNameLength ? rb.PrintNameOffset : rb.SubstituteNameOffset;
            USHORT len = rb.PrintNameLength ? rb.PrintNameLength : rb.SubstituteNameLength;
            const WCHAR* p = reinterpret_cast<const WCHAR*>(rb.PathBuffer) + (off / sizeof(WCHAR));
            wTarget.assign(p, len / sizeof(WCHAR));

            int n = ::WideCharToMultiByte(CP_ACP, 0, wTarget.c_str(), (int)wTarget.size(), nullptr, 0, nullptr, nullptr);
            std::string s(n, '\0');
            if (n > 0)
                ::WideCharToMultiByte(CP_ACP, 0, wTarget.c_str(), (int)wTarget.size(), s.data(), n, nullptr, nullptr);
            s = stripPrefixes(s);
            out = normalizePathSeparators(CnString::fromChars(s.c_str()));
        }
    }
    ::CloseHandle(h);
    return out;
}

bool CnFileInfo::exists() const
{
	return d_cast(d)->exists();
}

bool CnFileInfo::isAbsolute() const
{
	return d_cast(d)->isAbsolute();
}

bool CnFileInfo::isDir() const
{
	return d_cast(d)->isDir();
}

bool CnFileInfo::isExecutable() const
{
	return d_cast(d)->isExecutable();
}

bool CnFileInfo::isFile() const
{
	return d_cast(d)->isFile();
}

bool CnFileInfo::isHidden() const
{
	return d_cast(d)->isHidden();
}

bool CnFileInfo::isSystem() const
{
	return d_cast(d)->isSystem();
}

bool CnFileInfo::isReadable() const
{
	return d_cast(d)->isReadable();
}

bool CnFileInfo::isRelative() const
{
	return d_cast(d)->isRelative();
}

bool CnFileInfo::isRoot() const
{
	return d_cast(d)->isRoot();
}

bool CnFileInfo::isShortcut() const
{
	// Check if file ends with .lnk extension
	return d->fileName.ends_with(CnSTR(".lnk"));
}

bool CnFileInfo::isSymLink() const
{
    if (this->isSymbolicLink())
        return true;
    return isShortcut();
}

bool CnFileInfo::isSymbolicLink() const
{
    DWORD attrs = ::GetFileAttributes(d->filePath.data());
    if (attrs == INVALID_FILE_ATTRIBUTES)
        return false;
    return (attrs & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
}

bool CnFileInfo::isWritable() const
{
	return d_cast(d)->isWritable();
}

int CnFileInfo::permissions() const
{
	return d_cast(d)->permissions();
}

int64_t CnFileInfo::size() const
{
	return d_cast(d)->size();
}

int CnFileInfo::ownerId() const
{
	return d_cast(d)->ownerId();
}

CnString CnFileInfo::owner() const
{
	// Windows doesn't have simple owner names like Unix
	// Return empty string - could be enhanced with Windows SID lookup
    return CnString();
}

int CnFileInfo::groupId() const
{
	return d_cast(d)->groupId();
}

CnString CnFileInfo::group() const
{
	// Windows doesn't have simple group names like Unix
	// Return empty string - could be enhanced with Windows group lookup
    return CnString();
}

CnTimestamp CnFileInfo::accessTime() const
{
	return d_cast(d)->accessTime();
}

CnTimestamp CnFileInfo::modifiedTime() const
{
	return d_cast(d)->modifiedTime();
}

CnTimestamp CnFileInfo::statusTime() const
{
	return d_cast(d)->statusTime();
}

void CnFileInfo::setFile(const CnString &file)
{
    if (d->filePath == file)
        return;
    CNFILEINFO_DETACH
    d->filePath = normalizePathSeparators(file);
    
    // Extract filename and directory using Windows path functions
    const char* fullPath = d->filePath.chars();
    // Find the rightmost separator
    const char* lastSlash = strrchr(fullPath, CN_PATH_SEP);
    
    if (lastSlash)
    {
        d->fileName = CnString::fromChars(lastSlash + 1);
        d->dirName = CnString::fromChars(fullPath, lastSlash - fullPath);
    }
    else
    {
        d->fileName = d->filePath;
        d->dirName = CnString::fromChars(".");
    }
    this->refresh();
}

void CnFileInfo::refresh()
{
    CNFILEINFO_DETACH
    d_cast(d)->fileStat.st_mode = 0;
    ::_stat(d->filePath.chars(), &d_cast(d)->fileStat);
    
    // Also populate WIN32_FIND_DATA for Windows-specific attributes
    HANDLE hFind = ::FindFirstFileA(d->filePath.chars(), &d_cast(d)->findData);
    if (hFind != INVALID_HANDLE_VALUE) 
        ::FindClose(hFind);
}

