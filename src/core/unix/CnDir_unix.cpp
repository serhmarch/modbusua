#include <CnDir.h>
#include "CnDir_p_unix.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>

#define CNDIR_UNI_SEP CN_PATH_SEP

inline CnDirPrivateUnix* d_cast(CnDirPrivate* p) { return static_cast<CnDirPrivateUnix*>(p); }

static CnDirPrivateUnix nullCnDirPrivateUnix;

CnDirPrivate* getNullCnDirPrivate() { return &nullCnDirPrivateUnix; }

CnDirPrivate* createCnDirPrivate() { return new CnDirPrivateUnix(); }

CnDirPrivate* createCnDirPrivate(const CnDirPrivate* other) { return new CnDirPrivateUnix(*static_cast<const CnDirPrivateUnix*>(other)); }

static Cn::Permissions defaultDirPermissions = static_cast<Cn::Permissions>(
                                               Cn::Permission_ReadUser |Cn::Permission_WriteUser|Cn::Permission_ExecUser|
                                               Cn::Permission_ReadGroup|Cn::Permission_ExecGroup|
                                               Cn::Permission_ReadOther|Cn::Permission_ExecOther
                                               );

const CnChar CnDir::PathSep = CNDIR_UNI_SEP;

CnString CnDir::cwd()
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
        return CnString::fromChars(buffer);
    return CnString();
}

CnChar CnDir::separator()
{
    return CNDIR_UNI_SEP;
}

bool CnDir::mkdir(const CnString &dirName, int mode) const
{
    if (!d->info.isDir())
        return false;
    if (mode == Cn::Permission_Empty)
        mode = defaultDirPermissions;
    int r = ::mkdir((d->info.filePath() + CN_STR_SEP + dirName).chars(), Cn::to_mode_t(mode));
    return r == 0;
}

bool CnDir::mkpath(const CnString &dirPath, int mode) const
{
    // Note: * `::access(path.chars(), F_OK) == 0` mean the path exist
    //       * `::access(path.chars(), F_OK) != 0` mean the path doesn't exist
    if (!d->info.isDir())
        return false;
    if (dirPath.isEmpty())
        return false;
    if (mode == Cn::Permission_Empty)
        mode = defaultDirPermissions;
    CnString path = this->filePath(dirPath);
    if (path.back() == CN_PATH_SEP)
        path.pop_back();
    size_t pos = path.length()-dirPath.length()-1;
    while ((pos = path.find(CN_PATH_SEP, pos + 1)) != CnString::npos)
    {
        CnString sub = path.substr(0, pos);
        if (sub.isEmpty())
            return false;
        if (::mkdir(sub.chars(), Cn::to_mode_t(mode)) != 0 && errno != EEXIST)
            return false;
    }
    if (::mkdir(path.chars(), Cn::to_mode_t(mode)) != 0 && errno != EEXIST)
        return false;
    return ::access(path.chars(), F_OK) == 0;
}

bool CnDir::remove(const CnString &fileName)
{
    if (!d->info.isDir())
        return false;
    int r = ::remove((d->info.filePath() + CN_STR_SEP + fileName).chars());
    return r == 0;
}

bool CnDir::removeRecursively()
{
    if (!d->info.isDir())
        return false;
    // If already removed, return true
    if (::access(d->info.filePath().chars(), F_OK) != 0)
        return true;
    bool ok = true;
    DIR* dir = opendir(d->info.filePath().chars());
    if (!dir)
        return false;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        CnString fullPath = d->info.filePath() + CN_STR_SEP + entry->d_name;
        struct stat st;
        if (lstat(fullPath.chars(), &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                CnDir subdir(fullPath);
                if (!subdir.removeRecursively())
                    ok = false;
            }
            else
            {
                // Note: The unlink() system call removes a link (deletes a filename) and,
                // if that is the last link to the file, also removes the file itself
                // The unlink() system call doesn’t dereference symbolic links.
                // If pathname is a symbolic link, the link itself is removed, rather than the name to which it points.
                if (::unlink(fullPath.chars()) != 0)
                    ok = false;
            }
        }
        else
        {
            ok = false;
        }
    }
    closedir(dir);
    if (::rmdir(d->info.filePath().chars()) != 0)
        ok = false;
    this->refresh();
    return ok;
}

bool CnDir::rename(const CnString &oldName, const CnString &newName)
{
    if (!d->info.isDir())
        return false;
    int r = ::rename((d->info.filePath() + CN_STR_SEP + oldName).chars(), (d->info.filePath() + CN_STR_SEP + newName).chars());
    return r == 0;
}

bool CnDir::rmdir(const CnString &dirName) const
{
    if (!d->info.isDir())
        return false;
    int r = ::rmdir((d->info.filePath() + CN_STR_SEP + dirName).chars());
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
    if (pos < newLen -1)
        path = path.substr(0, pos + 1);
    bool res = false;
    while (pos > len)
    {
        if (::rmdir(path.chars()) != 0)
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
    DIR* dir = opendir(d->info.filePath().chars());
    if (dir == NULL)
        return CnFileInfoList();
    CnFileInfoList result;
    bool caseSensitive = (filters & CnDir::CaseSensitive) != 0;
    struct dirent* dp;
    while ((dp = readdir(dir)) != NULL)
    {
        if ((filters & CnDir::NoDot) && (strcmp(dp->d_name, ".") == 0)) 
            continue;
        if ((filters & CnDir::NoDotDot) && (strcmp(dp->d_name, "..") == 0)) 
            continue;

        // nameFilters support
        if (!matchNameFilters(CnString::fromChars(dp->d_name), nameFilters, caseSensitive))
            continue;

        CnFileInfo info(d->info.filePath() + CN_STR_SEP + CnString(dp->d_name));
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
    closedir(dir);

    // Sort result according to int
    if (sort != 0 && !result.empty())
    {
        result.sort([sort](const CnFileInfo& a, const CnFileInfo& b) {
            // Example: sort by name, size, time, etc. You can expand this as needed
            if (sort & CnDir::Name) {
                int cmp = a.fileName().compare(b.fileName());
                if (cmp != 0) return (sort & CnDir::Reversed) ? cmp > 0 : cmp < 0;
            }
            if (sort & CnDir::Size) {
                if (a.size() != b.size()) return (sort & CnDir::Reversed) ? a.size() > b.size() : a.size() < b.size();
            }
            if (sort & CnDir::Time) {
                if (a.modifiedTime() != b.modifiedTime()) return (sort & CnDir::Reversed) ? a.modifiedTime() > b.modifiedTime() : a.modifiedTime() < b.modifiedTime();
            }
            // Add more sort flags as needed
            return false;
        });
    }
    return result;
}