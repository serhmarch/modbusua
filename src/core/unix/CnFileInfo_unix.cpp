
#include <CnFileInfo.h>
#include "CnFileInfo_p_unix.h"

#include <unistd.h>     // for realpath, readlink
#include <limits.h>     // for PATH_MAX
#include <sys/stat.h>   // for stat, lstat
#include <pwd.h>        // for getpwuid
#include <grp.h>        // for getgrgid
#include <libgen.h>     // for dirname, basename
#include <cstring>

#include <CnDir.h>

static CnFileInfoPrivateUnix nullCnFileInfoPrivateUnix;

CnFileInfoPrivate* getNullCnFileInfoPrivate() { return &nullCnFileInfoPrivateUnix; }

CnFileInfoPrivate* createCnFileInfoPrivate() { return new CnFileInfoPrivateUnix(); }

CnFileInfoPrivate* createCnFileInfoPrivate(const CnFileInfoPrivate* other) { return new CnFileInfoPrivateUnix(*static_cast<const CnFileInfoPrivateUnix*>(other)); }

const size_t CnFileInfo::RootPathBaseLength = 1; // "/" or "\\"

CnString CnFileInfo::rootPath()
{
    // return main drive root, e.g., "C:/"
    return CnString(CnSTR("/"));
}

// Normalizes root paths for Unix: '///', '\\\\', '/\\'
// becomes '/'
CnString CnFileInfo::normalizeRootPath(const CnString &path)
{
    if (path.length() == 0)
        return path;
    CnString newPath = path;
    const CnChar* s = newPath.data();

    CnChar c0 = s[0];
    bool isRoot = c0 == '/' || c0 == '\\';
    if (isRoot)
    {
        if (c0 == CN_PATH_SEP && s[1] != '\\' && s[1] != '/')
            return newPath;
        if (c0 != CN_PATH_SEP)
            newPath[0] = CN_PATH_SEP;
        size_t i = 1;
        while (newPath[i] == '\\' || newPath[i] == '/')
            ++i;
        if (i > 1)
            newPath = newPath.erase(1, i - 1);
    }
    return newPath;
}

bool CnFileInfo::exists(const CnString &fileName)
{
    return ::access(fileName.chars(), F_OK) == 0;
}

bool CnFileInfo::isAbsolutePath(const CnString &fileName)
{
    return CnFileInfoPrivateUnix::isAbsolutePath(fileName);
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
        return d->dirName;
    return CnDir::cwd() + CN_PATH_SEP + d->dirName;
}

CnString CnFileInfo::canonicalFilePath() const
{
	if (d_cast(d)->exists())
    {
        char buf[PATH_MAX];
        buf[0] = '\0';
        const char *src = d->filePath.chars();
        ::realpath(src, buf);
        if (buf[0])
            return CnString::fromChars(buf);
    }
    return CnDir::cleanPath(CnDir::cwd() + CN_PATH_SEP + d->filePath);
}

CnString CnFileInfo::canonicalPath() const
{
    if (CnFileInfo::exists(d->dirName))
    {
        char buf[PATH_MAX];
        if (realpath(d->dirName.chars(), buf))
            return CnString::fromChars(buf);
    }
    return CnDir::cleanPath(CnDir::cwd() + CN_PATH_SEP + d->dirName);
}

CnString CnFileInfo::symLinkTarget() const
{
	char buf[PATH_MAX];
	ssize_t len = readlink(d->filePath.chars(), buf, sizeof(buf) - 1);
	if (len > 0)
	{
		buf[len] = '\0';
		return CnString::fromChars(buf);
	}
	return CnString();
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
	// Not applicable on Unix, always false
	return false;
}

bool CnFileInfo::isSymLink() const
{
    struct stat st;
    if (lstat(d->filePath.chars(), &st) == 0)
        return S_ISLNK(st.st_mode);
    return false;
}

bool CnFileInfo::isSymbolicLink() const
{
    return isSymLink();
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
	struct passwd *pw = getpwuid(d_cast(d)->ownerId());
    if (pw)
	    return CnString::fromChars(pw->pw_name);
    return CnString();
}

int CnFileInfo::groupId() const
{
	return d_cast(d)->groupId();
}

CnString CnFileInfo::group() const
{
	struct group *gr = getgrgid(d_cast(d)->groupId());
    if (gr)
	    return CnString::fromChars(gr->gr_name);
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
    std::string base = d->filePath.toStdString();
    std::string dir = base;
    d->fileName = ::basename((char*)base.c_str());
    d->dirName  = ::dirname((char*)dir.c_str());
    this->refresh();
}

void CnFileInfo::refresh()
{
    CNFILEINFO_DETACH
    d_cast(d)->fileStat.st_mode = 0;
    ::stat(d->filePath.chars(), &d_cast(d)->fileStat);
}

