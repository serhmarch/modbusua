#include <CnDir.h>
#include "CnDir_p_win.h"

#include <windows.h>
#include <io.h>
#include <direct.h>
#include <algorithm>

#define CNDIR_WIN_SEP CnCHR('\\')

inline CnDirPrivateWin* d_cast(CnDirPrivate* p) { return static_cast<CnDirPrivateWin*>(p); }

static CnDirPrivateWin nullCnDirPrivateWin;

CnDirPrivate* getNullCnDirPrivate() { return &nullCnDirPrivateWin; }

CnDirPrivate* createCnDirPrivate() { return new CnDirPrivateWin(); }

CnDirPrivate* createCnDirPrivate(const CnDirPrivate* other) { return new CnDirPrivateWin(*static_cast<const CnDirPrivateWin*>(other)); }

static Cn::Permissions defaultDirPermissions = static_cast<Cn::Permissions>(
                                               Cn::Permission_ReadUser |Cn::Permission_WriteUser|Cn::Permission_ExecUser|
                                               Cn::Permission_ReadGroup|Cn::Permission_ExecGroup|
                                               Cn::Permission_ReadOther|Cn::Permission_ExecOther
                                               );

const CnChar CnDir::PathSep = CNDIR_WIN_SEP;

CnString CnDir::cwd()
{
    char buffer[MAX_PATH];
    if (::_getcwd(buffer, sizeof(buffer)) != nullptr)
        return CnFileInfo::normalizePathSeparators(CnString::fromChars(buffer));
    return CnString();
}

CnChar CnDir::separator()
{
    return CNDIR_WIN_SEP;
}

bool CnDir::mkdir(const CnString &dirName, int mode) const
{
    if (!d->info.isDir())
        return false;
    CnString path = CnFileInfo::normalizePathSeparators(dirName);
    // Windows doesn't use Unix-style permissions in _mkdir
    int r = ::_mkdir(filePath(path).chars());
    return r == 0;
}

bool CnDir::mkpath(const CnString &dirPath, int mode) const
{
    // Note: * `::_access(path.chars(), 0) == 0` mean the path exist
    //       * `::_access(path.chars(), 0) != 0` mean the path doesn't exist
    if (!d->info.isDir())
        return false;
    if (dirPath.isEmpty())
        return false;
    CnString path = this->filePath(dirPath);
    if (path.back() == CN_PATH_SEP)
        path.pop_back();
    size_t pos = path.length()-dirPath.length()-1;
    // Handle both forward and backward slashes
    while ((pos = path.find(CN_PATH_SEP, pos + 1)) != CnString::npos)
    {
        CnString sub = path.substr(0, pos);
        if (sub.isEmpty())
            return false;
        if (::_mkdir(sub.chars()) != 0 && errno != EEXIST)
            return false;
    }
    if (::_mkdir(path.chars()) != 0 && errno != EEXIST)
        return false;
    return ::_access(path.chars(), 0) == 0;
}

bool CnDir::remove(const CnString &fileName)
{
    if (!d->info.isDir())
        return false;
    CnString path = CnFileInfo::normalizePathSeparators(fileName);
    int r = ::remove(filePath(path).chars());
    return r == 0;
}

bool CnDir::removeRecursively()
{
    if (!d->info.isDir())
        return false;
    // If already removed, return true
    if (::_access(d->info.filePath().chars(), 0) != 0)
        return true;
    bool ok = true;
    
    // Use Windows FindFirstFile/FindNextFile instead of opendir/readdir
    CnString searchPath = d->info.filePath() + CnSTR(_CN_STR_SEP "*");
    WIN32_FIND_DATAA findData;
    HANDLE hFind = ::FindFirstFileA(searchPath.chars(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;
    do
    {
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;
        CnString fullPath = filePath(findData.cFileName);
        
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            CnDir subdir(fullPath);
            if (!subdir.removeRecursively())
                ok = false;
        }
        else
        {
            // Remove file attributes that might prevent deletion
            ::SetFileAttributesA(fullPath.chars(), FILE_ATTRIBUTE_NORMAL);
            if (::DeleteFileA(fullPath.chars()) == 0)
                ok = false;
        }
    }
    while (::FindNextFileA(hFind, &findData));
    
    ::FindClose(hFind);
    if (::_rmdir(d->info.filePath().chars()) != 0)
        ok = false;
    this->refresh();
    return ok;
}

bool CnDir::rename(const CnString &oldName, const CnString &newName)
{
    if (!d->info.isDir())
        return false;
    int r = ::rename(filePath(oldName).chars(), filePath(newName).chars());
    return r == 0;
}

bool CnDir::rmdir(const CnString &dirName) const
{
    if (!d->info.isDir())
        return false;
    int r = ::_rmdir(filePath(dirName).chars());
    return r == 0;
}

bool CnDir::rmpath(const CnString &dirPath) const
{
    if (!d->info.isDir())
        return false;
    if (dirPath.isEmpty())
        return false;
    CnString path = filePath(dirPath);
    size_t len = path.length()-dirPath.length();
    size_t newLen = path.length();
    size_t pos = newLen-1;
    while (pos >= 0 && (path.at(pos) == '\\' || path.at(pos) == '/'))
        --pos;
    if (pos < newLen-1)
        path = path.substr(0, pos + 1);
    bool res = false;
    while (pos > len)
    {
        if (::_rmdir(path.chars()) != 0)
            return false;
        res = true;
        pos = path.rfind('\\');
        if (pos == CnString::npos)
            pos = path.rfind('/');
        if (pos != CnString::npos)
            path = path.substr(0, pos);
    }
    return true;
}

CnFileInfoList CnDir::innerEntryInfoList(const CnStringList &nameFilters, int filters, int sort) const
{
    CnString searchPath = d->info.filePath() + CnSTR(CN_STR_SEP "*");
    WIN32_FIND_DATAA findData;
    HANDLE hFind = ::FindFirstFileA(searchPath.chars(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return CnFileInfoList();
    CnFileInfoList result;
    bool caseSensitive = (filters & CnDir::CaseSensitive) != 0;
    
    do
    {
        if ((filters & CnDir::NoDot) && (strcmp(findData.cFileName, ".") == 0)) 
            continue;
        if ((filters & CnDir::NoDotDot) && (strcmp(findData.cFileName, "..") == 0)) 
            continue;

        // nameFilters support
        if (!matchNameFilters(CnString::fromChars(findData.cFileName), nameFilters, caseSensitive))
            continue;

        CnFileInfo info(filePath(findData.cFileName));
        if (!info.exists())
            continue;
        if (filters == CnDir::NoFilter)
            result.push_back(info);
        else if ((filters & CnDir::AllDirs) && info.isDir())
            result.push_back(info);
        else if ((filters & CnDir::NoSymLinks) && info.isSymLink())
            continue;
        else if (filters & (CnDir::Readable | CnDir::Writable | CnDir::Executable | CnDir::Hidden | CnDir::System))
        {
            if ((filters & CnDir::Dirs) && info.isDir())
            {
                if ((filters & CnDir::Readable) && info.isReadable())
                    result.push_back(info);
                else if ((filters & CnDir::Writable) && info.isWritable())
                    result.push_back(info);
                else if ((filters & CnDir::Executable) && info.isExecutable())
                    result.push_back(info);
                else if ((filters & CnDir::Hidden) && info.isHidden())
                    result.push_back(info);
            }
            else if ((filters & CnDir::Files) && info.isFile())
            {
                if ((filters & CnDir::Readable) && info.isReadable())
                    result.push_back(info);
                else if ((filters & CnDir::Writable) && info.isWritable())
                    result.push_back(info);
                else if ((filters & CnDir::Executable) && info.isExecutable())
                    result.push_back(info);
                else if ((filters & CnDir::Hidden) && info.isHidden())
                    result.push_back(info);
                else if ((filters & CnDir::System) && info.isSystem())
                    result.push_back(info);
            }
        }
        else if ((filters & CnDir::Dirs) && info.isDir())
            result.push_back(info);
        else if ((filters & CnDir::Files) && info.isFile())
            result.push_back(info);
    }
    while (::FindNextFileA(hFind, &findData));
    
    ::FindClose(hFind);

    // Sort result according to int
    if (sort != 0 && !result.empty())
    {
        result.sort([sort](const CnFileInfo& a, const CnFileInfo& b) {
            // Example: sort by name, size, time, etc. You can expand this as needed
            if (sort & CnDir::Name)
            {
                int cmp = a.fileName().compare(b.fileName());
                if (cmp != 0) return (sort & CnDir::Reversed) ? cmp > 0 : cmp < 0;
            }
            if (sort & CnDir::Size)
            {
                if (a.size() != b.size()) return (sort & CnDir::Reversed) ? a.size() > b.size() : a.size() < b.size();
            }
            if (sort & CnDir::Time)
            {
                if (a.modifiedTime() != b.modifiedTime()) return (sort & CnDir::Reversed) ? a.modifiedTime() > b.modifiedTime() : a.modifiedTime() < b.modifiedTime();
            }
            // Add more sort flags as needed
            return false;
        });
    }
    return result;
}