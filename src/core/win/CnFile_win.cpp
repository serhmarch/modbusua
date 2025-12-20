#include <CnFile.h>
#include "CnFile_p_win.h"

#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#include <io.h>
#include <cstdio>
#include <cstring>

#include <CnDir.h>

CnFilePrivate* createCnFilePrivate() { return new CnFilePrivateWin(); }

bool CnFile::exists() const
{
    if (d_cast(d)->isOpen())
        return true;
    d->info.refresh();
    return d->info.exists();
}

bool CnFile::isOpen() const
{
    return d_cast(d)->isOpen();
}

bool CnFile::atEnd() const
{
    if (!this->isOpen())
        return true;
    LARGE_INTEGER current, zero;
    zero.QuadPart = 0;
    if (!::SetFilePointerEx(d_cast(d)->hFile, zero, &current, FILE_CURRENT))
        return true;
    return (current.QuadPart >= this->size());
}

bool CnFile::setPermissions(int permissions)
{
    // Windows has limited permission control compared to Unix
    DWORD attrs = ::GetFileAttributesA(d->info.filePath().chars());
    if (attrs == INVALID_FILE_ATTRIBUTES)
        return false;
    
    // Handle read-only attribute based on write permission
    if (permissions & Cn::Permission_WriteUser)
        attrs &= ~FILE_ATTRIBUTE_READONLY;
    else
        attrs |= FILE_ATTRIBUTE_READONLY;
    
    BOOL ret = ::SetFileAttributesA(d->info.filePath().chars(), attrs);
    if (ret)
    {
        d->info.refresh();
        return true;
    }
    return false;
}

int64_t CnFile::pos() const
{
    if (d_cast(d)->isOpen())
    {
        LARGE_INTEGER current, zero;
        zero.QuadPart = 0;
        if (::SetFilePointerEx(d_cast(d)->hFile, zero, &current, FILE_CURRENT))
            return static_cast<int64_t>(current.QuadPart);
    }
    return -1;
}

bool CnFile::open(int mode)
{
    DWORD dwDesiredAccess = 0;
    DWORD dwCreationDisposition = 0;
    
    // Set access rights
    if (mode & Cn::Open_ReadWrite)
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    else if (mode & Cn::Open_WriteOnly)
        dwDesiredAccess = GENERIC_WRITE;
    else
        dwDesiredAccess = GENERIC_READ;

    // Set creation disposition
    if (mode & Cn::Open_NewOnly)
        dwCreationDisposition = CREATE_NEW;
    else if (mode & Cn::Open_ExistingOnly)
        dwCreationDisposition = OPEN_EXISTING;
    else if (mode & Cn::Open_Truncate)
        dwCreationDisposition = CREATE_ALWAYS;
    else if (mode & Cn::Open_WriteOnly)
        dwCreationDisposition = OPEN_ALWAYS;
    else
        dwCreationDisposition = OPEN_EXISTING;

    HANDLE hFile = ::CreateFileA(d->info.filePath().chars(),
                                dwDesiredAccess,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                nullptr,
                                dwCreationDisposition,
                                FILE_ATTRIBUTE_NORMAL,
                                nullptr);
    
    d_cast(d)->hFile = hFile;
    if (hFile == INVALID_HANDLE_VALUE)
    {
        d->lastError = OpenError;
        DWORD error = ::GetLastError();
        char buffer[256];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, buffer, sizeof(buffer), nullptr);
        d->errorString = CnString::fromChars(buffer);
        return false;
    }
    
    // Handle append mode by seeking to end
    if (mode & Cn::Open_Append)
    {
        ::SetFilePointer(hFile, 0, nullptr, FILE_END);
    }
    
    d->info.refresh();
    return true;
}

int CnFile::get()
{
    uint8_t ch; // Note: use uint8_t to avoid sign extension issues
    DWORD bytesRead;
    if (!::ReadFile(d_cast(d)->hFile, &ch, 1, &bytesRead, nullptr) || bytesRead == 0)
        return EOF;
    return ch;
}

int64_t CnFile::read(char *data, int64_t maxSize)
{
    DWORD bytesRead;
    if (!::ReadFile(d_cast(d)->hFile, data, static_cast<DWORD>(maxSize), &bytesRead, nullptr))
    {
        d->lastError = ReadError;
        DWORD error = ::GetLastError();
        char buffer[256];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, buffer, sizeof(buffer), nullptr);
        d->errorString = CnString::fromChars(buffer);
        return -1;
    }
    return static_cast<int64_t>(bytesRead);
}

int64_t CnFile::readLine(char *data, int64_t maxSize)
{
    DWORD bytesReadTotal = 0;
    DWORD bytesRead;

    if (!::ReadFile(d_cast(d)->hFile, data, static_cast<DWORD>(maxSize-1), &bytesRead, nullptr))
        return -1; // error
    
    if (bytesRead == 0)
    {
        data[0] = '\0';
        return 0; // EOF
    }
    
    bytesReadTotal = bytesRead;
    for (DWORD i = 0; i < bytesRead; ++i)
    {
        if (data[i] == '\n')
        {
            bytesReadTotal = i + 1;
            LONG unread = bytesRead - bytesReadTotal;
            // Move file pointer back
            LARGE_INTEGER moveBy;
            moveBy.QuadPart = -unread;
            ::SetFilePointerEx(d_cast(d)->hFile, moveBy, nullptr, FILE_CURRENT);
            break;
        }
    }

    data[bytesReadTotal] = '\0';
    return static_cast<int64_t>(bytesReadTotal);
}

int64_t CnFile::peek(char *data, int64_t maxSize)
{
    LARGE_INTEGER currentPos, zero;
    zero.QuadPart = 0;
    if (!::SetFilePointerEx(d_cast(d)->hFile, zero, &currentPos, FILE_CURRENT))
        return -1;
    
    int64_t n = read(data, maxSize);
    
    // Restore original position
    ::SetFilePointerEx(d_cast(d)->hFile, currentPos, nullptr, FILE_BEGIN);
    return n;
}

int64_t CnFile::write(const char *data, int64_t maxSize)
{
    if (maxSize < 0)
        maxSize = strlen(data);
    DWORD bytesWritten;
    if (!::WriteFile(d_cast(d)->hFile, data, static_cast<DWORD>(maxSize), &bytesWritten, nullptr))
    {
        d->lastError = WriteError;
        DWORD error = ::GetLastError();
        char buffer[256];
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, buffer, sizeof(buffer), nullptr);
        d->errorString = CnString::fromChars(buffer);
        return -1;
    }
    return static_cast<int64_t>(bytesWritten);
}

bool CnFile::seek(int64_t pos)
{
    LARGE_INTEGER newPos;
    newPos.QuadPart = pos;
    return ::SetFilePointerEx(d_cast(d)->hFile, newPos, nullptr, FILE_BEGIN) != 0;
}

void CnFile::close()
{
    if (d_cast(d)->isOpen())
    {
        ::CloseHandle(d_cast(d)->hFile);
        d_cast(d)->hFile = CN_WIN_INVALID_HANDLE;
    }
}

bool CnFile::flush()
{
    if (!d_cast(d)->isOpen())
        return false;
    return ::FlushFileBuffers(d_cast(d)->hFile) != 0;
}

bool CnFile::resize(int64_t size)
{
    // Save current position
    LARGE_INTEGER currentPos, zero;
    zero.QuadPart = 0;
    if (!::SetFilePointerEx(d_cast(d)->hFile, zero, &currentPos, FILE_CURRENT))
        return false;
    
    // Set file size
    LARGE_INTEGER newSize;
    newSize.QuadPart = size;
    if (!::SetFilePointerEx(d_cast(d)->hFile, newSize, nullptr, FILE_BEGIN))
        return false;
    
    if (!::SetEndOfFile(d_cast(d)->hFile))
        return false;
    
    // Restore position if it's still valid
    if (currentPos.QuadPart <= size)
        ::SetFilePointerEx(d_cast(d)->hFile, currentPos, nullptr, FILE_BEGIN);
    
    return true;
}

bool CnFile::remove()
{
    int r = ::_unlink(d->info.filePath().c_str());
    return (r == 0);
}

bool CnFile::rename(const CnString &newName)
{
    int r = ::rename(d->info.filePath().c_str(), newName.c_str());
    return (r == 0);
}

bool CnFile::copy(const CnString &newName)
{
    return ::CopyFileA(d->info.filePath().chars(), newName.chars(), FALSE) != 0;
}

bool CnFile::link(const CnString &linkName)
{
    if (linkName.isEmpty())
        return false;

    // Build destination .lnk path: if relative, make it relative to current file's directory
    CnString dest = CnFileInfo::normalizePathSeparators(linkName);
    CnFileInfo destInfo(dest);
    if (!destInfo.isAbsolute())
        dest = d->info.dirPath() + CN_PATH_SEP + dest;

    // Ensure .lnk extension
    if (!dest.ends_with(CnSTR(".lnk")))
        dest += CnSTR(".lnk");

    // Resolve target absolute file path
    CnString target = d->info.absoluteFilePath();

    // Create Windows Shell Link (.lnk) via COM
    HRESULT hrInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    bool needUninit = SUCCEEDED(hrInit);
    IShellLinkA* psl = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (void**)&psl);
    if (FAILED(hr) || !psl)
    {
        if (needUninit) CoUninitialize();
        return false;
    }

    psl->SetPath(target.chars());

    IPersistFile* ppf = nullptr;
    hr = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
    if (FAILED(hr) || !ppf)
    {
        psl->Release();
        if (needUninit) CoUninitialize();
        return false;
    }

    wchar_t wpath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, dest.chars(), -1, wpath, MAX_PATH);
    hr = ppf->Save(wpath, TRUE);
    ppf->Release();
    psl->Release();
    if (needUninit) CoUninitialize();

    return SUCCEEDED(hr);
}
