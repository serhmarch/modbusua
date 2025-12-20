#ifndef CNFILEINFO_P_WIN_H
#define CNFILEINFO_P_WIN_H

#include "../CnFileInfo_p.h"
#include "CnWindows.h"

#include <sys/stat.h>

class CnFileInfoPrivateWin : public CnFileInfoPrivate
{
public:
    inline static bool isAbsolutePath(const CnString &fileName)
    {
        const CnChar* path = fileName.data();
        CnChar c0 = path[0];
        return (((c0 >= 'a' && c0 <= 'z') || (c0 >= 'A' && c0 <= 'Z')) && path[1] == ':');    
    }

public:
    CnFileInfoPrivateWin() : CnFileInfoPrivate()
    {
        memset(&this->fileStat, 0, sizeof(this->fileStat));
        memset(&this->findData, 0, sizeof(this->findData));
    }

    CnFileInfoPrivateWin(const CnFileInfoPrivateWin &other) : CnFileInfoPrivate(other)
    {
        memset(&this->fileStat, 0, sizeof(this->fileStat));
        memset(&this->findData, 0, sizeof(this->findData));
    };
    
public:
    inline bool exists() const { return (fileStat.st_mode != 0); }
    inline bool isAbsolute() const { return isAbsolutePath(filePath); }
    inline bool isDir() const { return (fileStat.st_mode & _S_IFDIR) != 0; }
    inline bool isExecutable() const 
    { 
        return (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
               (fileName.ends_with(CnSTR(".exe")) || fileName.ends_with(CnSTR(".bat")) || 
                fileName.ends_with(CnSTR(".cmd")) || fileName.ends_with(CnSTR(".com")));
    }
    inline bool isFile() const { return (fileStat.st_mode & _S_IFREG) != 0; }
    inline bool isHidden() const { return (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0; }
    inline bool isSystem() const { return (findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0; }
    inline bool isReadable() const { return (fileStat.st_mode & _S_IREAD) != 0; }
    inline bool isRelative() const { return !isAbsolute(); }
    inline bool isRoot() const 
    { 
        const CnChar* path = filePath.data();
        return (path[0] != CnCHR('\0') && path[1] == CnCHR(':') && 
                (path[2] == CnCHR('\0') || (path[2] == CN_PATH_SEP && path[3] == CnCHR('\0'))));
    }
    inline bool isWritable() const { return (fileStat.st_mode & _S_IWRITE) != 0; }

public:
    inline int permissions() const 
    { 
        int perms = 0;
        if (fileStat.st_mode & _S_IREAD)  perms |= Cn::Permission_ReadUser | Cn::Permission_ReadGroup | Cn::Permission_ReadOther;
        if (fileStat.st_mode & _S_IWRITE) perms |= Cn::Permission_WriteUser | Cn::Permission_WriteGroup | Cn::Permission_WriteOther;
        if (isExecutable()) perms |= Cn::Permission_ExecUser | Cn::Permission_ExecGroup | Cn::Permission_ExecOther;
        return perms;
    }
    inline int64_t size() const { return fileStat.st_size; }
    inline int ownerId() const { return 0; } // Windows doesn't have Unix-style UIDs
    inline int groupId() const { return 0; } // Windows doesn't have Unix-style GIDs

public:
    inline CnTimestamp accessTime() const { return Cn::toTimestamp(fileStat.st_atime); }
    inline CnTimestamp modifiedTime() const { return Cn::toTimestamp(fileStat.st_mtime); }
    inline CnTimestamp statusTime() const { return Cn::toTimestamp(fileStat.st_ctime); }

public:
    struct _stat fileStat;
    WIN32_FIND_DATA findData;
};

inline CnFileInfoPrivateWin* d_cast(CnFileInfoPrivate* p) { return static_cast<CnFileInfoPrivateWin*>(p); }

#endif // CNFILEINFO_P_WIN_H