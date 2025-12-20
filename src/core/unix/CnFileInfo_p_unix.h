#ifndef CNFILEINFO_P_UNIX_H
#define CNFILEINFO_P_UNIX_H

#include "../CnFileInfo_p.h"
#include "CnUnix.h"

#include <sys/stat.h>

class CnFileInfoPrivateUnix : public CnFileInfoPrivate
{
public:
    inline static bool isAbsolutePath(const CnString &fileName) { return fileName.data()[0] == CnCHR('/') || fileName.data()[0] == CnCHR('\\'); }
    inline static bool isRelativePath(const CnString &fileName) { return fileName.data()[0] != CnCHR('/') && fileName.data()[0] != CnCHR('\\'); }

public:
    CnFileInfoPrivateUnix() : CnFileInfoPrivate()
    {
        memset(&this->fileStat, 0, sizeof(this->fileStat));
    }

    CnFileInfoPrivateUnix(const CnFileInfoPrivateUnix &other) : CnFileInfoPrivate(other)
    {
        memset(&this->fileStat, 0, sizeof(this->fileStat));
    };
    
public:
    inline bool exists() const { return (fileStat.st_mode != 0); }
    inline bool isAbsolute() const { return isAbsolutePath(filePath); }
    inline bool isDir() const { return S_ISDIR(fileStat.st_mode); }
    inline bool isExecutable() const { return (fileStat.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH)); }
    inline bool isFile() const { return S_ISREG(fileStat.st_mode); }
    inline bool isHidden() const { return (fileName.data()[0] == CnCHR('.')); }
    inline bool isSystem() const { return S_ISCHR(fileStat.st_mode) || S_ISBLK(fileStat.st_mode) || S_ISFIFO(fileStat.st_mode) || S_ISSOCK(fileStat.st_mode); }
    inline bool isReadable() const { return (fileStat.st_mode & (S_IRUSR|S_IRGRP|S_IROTH)); }
    inline bool isRelative() const { return isRelativePath(filePath); }
    inline bool isRoot() const { return (filePath == CnCHR("/")); }
    inline bool isWritable() const { return (fileStat.st_mode & (S_IWUSR|S_IWGRP|S_IWOTH)); }

public:
    inline int permissions() const { return static_cast<int>(fileStat.st_mode & 0x0FFF); }
    inline int64_t size() const { return fileStat.st_size; }
    inline int ownerId() const { return fileStat.st_uid; }
    inline int groupId() const { return fileStat.st_gid; }

public:
    inline CnTimestamp accessTime() const { return Cn::toTimestamp(fileStat.st_atim); }
    inline CnTimestamp modifiedTime() const { return Cn::toTimestamp(fileStat.st_mtim); }
    inline CnTimestamp statusTime() const { return Cn::toTimestamp(fileStat.st_ctim); }

public:
    struct stat fileStat;
};

inline CnFileInfoPrivateUnix* d_cast(CnFileInfoPrivate* p) { return static_cast<CnFileInfoPrivateUnix*>(p); }

#endif // CNFILEINFO_P_UNIX_H