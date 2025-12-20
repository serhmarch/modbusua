
#include "CnFileInfo.h"
#include "CnFileInfo_p.h"

#include <CnDir.h>

CnFileInfo::CnFileInfo()
{
    d = getNullCnFileInfoPrivate();
    d->incRef();
}

CnString CnFileInfo::normalizePathSeparators(const CnChar *path, size_t length)
{
    CnString normalizedStr(length, CnCHR('\0')); 
    CnChar *normalizedPath = const_cast<CnChar*>(normalizedStr.data());
    size_t normalizedLen = 0;
    for (size_t i = 0; i < length; ++i)
    {
        if (path[i] == CnCHR('\\'))
            normalizedPath[normalizedLen++] = CN_PATH_SEP;
        else
            normalizedPath[normalizedLen++] = path[i];
    }
    return normalizedStr;
}

CnFileInfo::CnFileInfo(const CnString &path)
    : d(createCnFileInfoPrivate())
{
    setFile(path);
}

CnFileInfo::CnFileInfo(const CnFileInfo &other)
	: d(other.d)
{
    d = other.d;
    d->incRef();
}

CnFileInfo::CnFileInfo(CnFileInfo &&other)
	: d(other.d)
{
    auto n = getNullCnFileInfoPrivate();
    n->incRef();
    d = other.d;
    other.d = n;
}

CnFileInfo::~CnFileInfo()
{
	d->decRef();
}

CnFileInfo &CnFileInfo::operator=(const CnFileInfo &fileinfo)
{
	if (this != &fileinfo)
	{
		fileinfo.d->incRef();
		d->decRef();
		d = fileinfo.d;
	}
	return *this;
}

CnFileInfo &CnFileInfo::operator=(CnFileInfo &&other)
{
	if (this != &other)
	{
        auto n = getNullCnFileInfoPrivate();
        n->incRef();
        d = other.d;
        other.d = n;
	}
	return *this;
}

bool CnFileInfo::operator==(const CnFileInfo &fileinfo) const
{
	return d->filePath == fileinfo.d->filePath;
}

bool CnFileInfo::operator!=(const CnFileInfo &fileinfo) const
{
	return d->filePath != fileinfo.d->filePath;
}

CnString CnFileInfo::filePath() const
{
    return d->filePath;
}

CnString CnFileInfo::fileName() const
{
    return d->fileName;
}

CnString CnFileInfo::dirPath() const
{
	return d->dirName;
}

CnString CnFileInfo::baseName() const
{
	size_t dot = d->fileName.find('.');
	if (dot == CnString::npos)
		return d->fileName;
	return d->fileName.substr(0, dot);
}

CnString CnFileInfo::completeBaseName() const
{
	size_t dot = d->fileName.rfind('.');
	if (dot == CnString::npos)
		return d->fileName;
	return d->fileName.substr(0, dot);
}

CnString CnFileInfo::suffix() const
{
	size_t dot = d->fileName.rfind('.');
	if (dot == CnString::npos)
		return CnString();
	return d->fileName.substr(dot + 1);
}

CnString CnFileInfo::completeSuffix() const
{
	size_t dot = d->fileName.find('.');
	if (dot == CnString::npos)
		return CnString();
	return d->fileName.substr(dot + 1);
}

bool CnFileInfo::isNull() const
{
    return d == getNullCnFileInfoPrivate();
}

void CnFileInfo::swap(CnFileInfo &other)
{
    auto *tmp = d;
    d = other.d;
    other.d = tmp;
}

