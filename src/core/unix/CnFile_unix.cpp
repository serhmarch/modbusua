#include <CnFile.h>
#include "CnFile_p_unix.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <cstdio>
#include <cstring>
#include <errno.h>

#include <CnDir.h>

CnFilePrivate* createCnFilePrivate() { return new CnFilePrivateUnix(); }

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
    off_t current = ::lseek(d_cast(d)->fd, 0, SEEK_CUR);
    if (current == (off_t)-1)
        return true;
    return (current >= this->size());
}

bool CnFile::setPermissions(int permissions)
{
    int ret = ::chmod(d->info.filePath().c_str(), static_cast<mode_t>(permissions));
    if (ret == 0)
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
        off_t current = ::lseek(d_cast(d)->fd, 0, SEEK_CUR);
        if (current != (off_t)-1)
            return static_cast<int64_t>(current);
    }
    return -1;
}

bool CnFile::open(int mode)
{
    int flags = 0;
    if (mode & Cn::Open_ReadWrite     ) flags |= O_RDWR;
    else if (mode & Cn::Open_WriteOnly) flags |= O_WRONLY;
    else                                flags |= O_RDONLY;

    if (mode & Cn::Open_Append        ) flags |= O_APPEND;
    if (mode & Cn::Open_Truncate      ) flags |= O_TRUNC;
    if (mode & Cn::Open_NewOnly       ) flags |= O_CREAT | O_EXCL;
    if (mode & Cn::Open_ExistingOnly  ) flags |= 0;
    else if (mode & Cn::Open_WriteOnly) flags |= O_CREAT;

    int fd = ::open(d->info.filePath().c_str(), flags, static_cast<mode_t>(0666));
    d_cast(d)->fd = fd;
    if (fd == CN_UNIX_INVALID_FD)
    {
        d->lastError = OpenError;
        d->errorString = CnString::fromChars(strerror(errno));
        return false;
    }
    d->info.refresh();
    return true;
}

int CnFile::get()
{
    uint8_t ch; // Note: use uint8_t to avoid sign extension issues
    int r = ::read(d_cast(d)->fd, &ch, 1);
    if (r <= 0)
        return EOF;
    return ch;
}

int64_t CnFile::read(char *data, int64_t maxSize)
{
    ssize_t n = ::read(d_cast(d)->fd, data, maxSize);
    if (n < 0)
    {
        d->lastError = ReadError;
        d->errorString = CnString::fromChars(strerror(errno));
        return -1;
    }
    return n;
}

int64_t CnFile::readLine(char *data, int64_t maxSize)
{
    size_t bytesReadTotal = 0;

    while (1)
    {
        ssize_t n = ::read(d_cast(d)->fd, data, maxSize-1);
        if (n < 0)
        {
            if (errno == EINTR)
                continue; // retry on signal
            return -1; // real error
        }
        if (n == 0)
            break; // EOF
        
        bytesReadTotal = n;
        for (ssize_t i = 0; i < n ; ++i)
        {
            if (data[i] == '\n')
            {
                bytesReadTotal = i + 1;
                off_t unread = n - bytesReadTotal;
                ::lseek(d_cast(d)->fd, -unread, SEEK_CUR);
                break;;
            }
        }
        break;
    }

    data[bytesReadTotal] = '\0';
    return bytesReadTotal;
}

int64_t CnFile::peek(char *data, int64_t maxSize)
{
    off_t pos = ::lseek(d_cast(d)->fd, 0, SEEK_CUR);
    int64_t n = read(data, maxSize);
    ::lseek(d_cast(d)->fd, pos, SEEK_SET);
    return n;
}

int64_t CnFile::write(const char *data, int64_t maxSize)
{
    if (maxSize < 0)
        maxSize = strlen(data);
    ssize_t n = ::write(d_cast(d)->fd, data, maxSize);
    if (n < 0)
    {
        d->lastError = WriteError;
        d->errorString = CnString::fromChars(strerror(errno));
        return -1;
    }
    return n;
}

bool CnFile::seek(int64_t pos)
{
    off_t r = ::lseek(d_cast(d)->fd, pos, SEEK_SET);
    return (r != (off_t)-1);
}

void CnFile::close()
{
    if (d_cast(d)->isOpen())
    {
        ::close(d_cast(d)->fd);
        d_cast(d)->fd = CN_UNIX_INVALID_FD;
    }
}

bool CnFile::flush()
{
    if (!d_cast(d)->isOpen())
        return false;
    int r = ::fsync(d_cast(d)->fd);
    return (r == 0);
}

bool CnFile::resize(int64_t size)
{
    int r = ::ftruncate(d_cast(d)->fd, size);
    return (r == 0);
}

bool CnFile::remove()
{
    int r = ::unlink(d->info.filePath().c_str());
    return (r == 0);
}

bool CnFile::rename(const CnString &newName)
{
    int r = ::rename(d->info.filePath().c_str(), newName.c_str());
    return (r == 0);
}

bool CnFile::copy(const CnString &newName)
{
    int src = ::open(d->info.filePath().chars(), O_RDONLY);
    if (src == -1)
        return false;
    int dst = ::open(newName.chars(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dst == -1)
        return false;
    char buf[8192];
    ssize_t n;
    bool ok = true;
    while ((n = ::read(src, buf, sizeof(buf))) > 0)
    {
        if (::write(dst, buf, n) != n)
        {
            ok = false;
            break;
        }
    }
    ::close(src);
    ::close(dst);
    return ok;
}

bool CnFile::link(const CnString &linkName)
{
    CnFileInfo linkfi(linkName);
    if (linkfi.isAbsolute())
    {
        std::string filepath = d->info.absoluteFilePath().toStdString();
        std::string linkpath = linkName.toStdString();
        int r = ::symlink(filepath.data(), linkpath.data());
        return (r == 0);
    }
    CnDir fileDir(d->info.absolutePath());
    linkfi.setFile(fileDir.absoluteFilePath(linkName));
    CnDir linkDir(linkfi.dirPath());
    // linkpath is relative to current file directory
    std::string linkpath = linkName.toStdString();
    // filepath is relative to link directory
    std::string filepath = linkDir.relativeFilePath(d->info.absoluteFilePath()).toStdString();
    // dirname is relative path to the current file directory
    std::string dirname = fileDir.path().toStdString();
    int fd = ::open(dirname.data(), O_RDONLY | O_DIRECTORY);
    if (fd == -1)
        return false;
    int r = ::symlinkat(filepath.data(), fd, linkpath.data());
    ::close(fd);
    return (r == 0);
}
