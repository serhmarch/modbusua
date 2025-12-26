#include <CnDir.h>
#include "CnDir_p.h"

CnString CnDir::cleanPath(const CnString &path)
{
    CnString newPath = path;
    size_t begin = 0;
    bool isAbsolute = CnFileInfo::isAbsolutePath(path);
    if (isAbsolute)
    {
        newPath = CnFileInfo::normalizeRootPath(newPath);
        begin = CnFileInfo::RootPathBaseLength;
    }
    const CnChar *origin = newPath.data();
    size_t originLen = newPath.length(), resolvedLen = 0;
    CnChar resolvedPath[CN_PATH_MAX];
    memset(resolvedPath, 0, sizeof(resolvedPath));
    CnChar prev = CnCHR('\0');
    if (begin)
    {
        for (size_t i = 0; i < begin; ++i)
            resolvedPath[resolvedLen++] = origin[i];
        prev = origin[begin - 1];
    }
    for (size_t i = begin; i < originLen; ++i)
    {
        switch (origin[i])
        {
        case CnCHR('\0'):
            return CnString(resolvedPath, resolvedLen);
        case CnCHR('/'):
        case CnCHR('\\'):
        {
            //CnChar next = origin[i+1]; // Note: if i+1 == originLen, next == '\0', so it's safe
            while ((origin[i+1] == CnCHR('/')) || (origin[i+1] == CnCHR('\\')))
                ++i;
            resolvedPath[resolvedLen++] = CN_PATH_SEP;
        }
            break;
        case CnCHR('.'):
        {
            switch (prev)
            {
            case CnCHR('/'):
            case CnCHR('\\'):
            case CnCHR('\0'):
            {
                CnChar next = origin[i+1]; // Note: if i+1 == originLen, next == '\0', so it's safe
                switch (next)
                {
                case CnCHR('.'):
                {
                    CnChar next2 = origin[i+2]; // Note: if i+2 == originLen, next2 == '\0', so it's safe
                    switch (next2)
                    {
                    case CnCHR('/'):
                    case CnCHR('\\'):
                    case CnCHR('\0'):
                    {
                        if (resolvedLen < 2)
                            return newPath;
                        size_t n = CnString::find_last_of_chars(resolvedPath, CnSTR("/"), resolvedLen-2, 1);
                        if (n == CnString::npos)
                        {
                            resolvedLen = begin;
                            i += 2;
                        }
                        else
                        {
                            resolvedLen = n;
                            ++i;
                        }
                        //i += 2;
                    }
                        break;
                    default:
                        resolvedPath[resolvedLen++] = origin[i];
                        break;
                    }
                }
                    break;
                case CnCHR('/'):
                case CnCHR('\\'):
                case CnCHR('\0'):
                    ++i;
                    break;
                default:
                    resolvedPath[resolvedLen++] = origin[i];
                    break;
                }
            }
                break;
            default:
                resolvedPath[resolvedLen++] = origin[i];
                break;
            }
        }
            break;
        default:
            resolvedPath[resolvedLen++] = origin[i];
            break;
        }
        prev = origin[i];
    }
    while (resolvedLen > 1 && resolvedPath[resolvedLen - 1] == CN_PATH_SEP)
    {
        if (isAbsolute && resolvedLen <= CnFileInfo::RootPathBaseLength)
            break; // Do not remove trailing slash of root path
        --resolvedLen;
        break;
    }
    return CnString(resolvedPath, resolvedLen);
}

static bool is_equal_ignore_case(CnChar a, CnChar b)
{
    return std::tolower(a) == std::tolower(b);
}

static bool is_equal_count_case(CnChar a, CnChar b)
{
    return a == b;
}

bool CnDir::matchNameFilters(const CnString &fname, const CnStringList &nameFilters, bool caseSensitive)
{
    bool matchesFilter = nameFilters.empty();
    typedef bool (*CharEqualFunc)(CnChar, CnChar);
    CharEqualFunc charEqual = caseSensitive ? is_equal_count_case : is_equal_ignore_case;

    for (const auto& filter : nameFilters)
    {
        // Simple wildcard: '*' matches any, '?' matches one char
        // For now, only support '*' at start/end or exact match
        if (filter == CnSTR("*"))
        {
            matchesFilter = true;
            break;
        }
        if (fname.size() == 1 && filter == CnSTR("?"))
        {
            matchesFilter = true;
            break;
        }

        CnString pattern = filter;
        size_t pi = 0, fi = 0;
        bool match = true;
        while (pi < pattern.size() && fi < fname.size())
        {
            if (pattern[pi] == CnCHR('?'))
            {
                // '?' matches any single character
                ++pi;
                ++fi;
            }
            else if (charEqual(pattern[pi], fname[fi]))
            {
                ++pi;
                ++fi;
            }
            else
            {
                match = false;
                break;
            }
        }
        if (match && pi == pattern.size() && fi == fname.size())
        {
            matchesFilter = true;
            break;
        }
        if (filter.find(CnSTR("*")) != CnString::npos)
        {
            // Only support '*suffix' or 'prefix*' or '*substring*'
            if (filter.starts_with(CnSTR("*")) && filter.ends_with(CnSTR("*")))
            {
                CnString sub = filter.substr(1, filter.size()-2);
                if (fname.constains(sub)) { matchesFilter = true; break; }
            }
            else if (filter.starts_with(CnSTR("*")))
            {
                CnString suffix = filter.substr(1);
                if (fname.ends_with(suffix)) { matchesFilter = true; break; }
            }
            else if (filter.ends_with(CnSTR("*")))
            {
                CnString prefix = filter.substr(0, filter.size()-1);
                if (fname.starts_with(prefix)) { matchesFilter = true; break; }
            }
            else
            {
                // 'pre*post' pattern
                size_t starPos = filter.find(CnCHR('*'));
                CnString prefix = filter.substr(0, starPos);
                CnString suffix = filter.substr(starPos + 1);
                if (fname.size() >= prefix.size() + suffix.size() &&
                    fname.starts_with(prefix) && fname.ends_with(suffix))
                {
                    matchesFilter = true;
                    break;
                }
            }
        }
        else
        {
            if (fname == filter) { matchesFilter = true; break; }
        }
    }
    return matchesFilter;
}

CnDir::CnDir(const CnString &path, const CnStringList &nameFilters, int filters, int sort) :
    d(createCnDirPrivate())
{
    d->info.setFile(path);
    d->nameFilters = nameFilters;
    d->filter = filters;
    d->sort = sort;
}

CnDir::CnDir(const CnString &path) :
    d(createCnDirPrivate())
{
    d->info.setFile(path);
}

CnDir::CnDir(const CnDir &other)
{
    if (this != &other)
    {
        other.d->incRef();
        d = other.d;
    }
}

CnDir::CnDir(CnDir &&other)
{
    auto* dd = getNullCnDirPrivate();
    dd->incRef();
    d = other.d;
    other.d = dd;
}

CnDir::CnDir() : 
    d(getNullCnDirPrivate())
{
    d->incRef();
}

CnDir::~CnDir()
{
    d->decRef();
}

CnDir &CnDir::operator=(const CnDir &other)
{
    if (this != &other)
    {
        other.d->incRef();
        d->decRef();
        d = other.d;
    }
    return *this;
}

CnDir &CnDir::operator=(CnDir &&other)
{
    if (this != &other)
    {
        auto* dd = getNullCnDirPrivate();
        dd->incRef();
        d->decRef();
        d = other.d;
        other.d = dd;
    }
    return *this;
}

bool CnDir::operator==(const CnDir &other) const
{
    return d->info == other.d->info;
}

bool CnDir::operator!=(const CnDir &other) const
{
    return d->info != other.d->info;
}

bool CnDir::isNull() const
{
    return d == getNullCnDirPrivate();
}

bool CnDir::exists() const
{
    return d->info.exists();
}

bool CnDir::exists(const CnString &path) const
{
    if (CnFileInfo::isAbsolutePath(path))
        return CnFileInfo::exists(path);
    return CnFileInfo::exists(this->filePath(path));
}

CnString CnDir::path() const
{
    return d->info.filePath();
}

void CnDir::setPath(const CnString &path)
{
    CNDIR_DETACH
    d->info.setFile(path);
}

CnString CnDir::absoluteFilePath(const CnString &fileName) const
{
    CnString path = d->info.absoluteFilePath();
    if (path.ends_with(CN_PATH_SEP))
        return path + CnFileInfo::normalizePathSeparators(fileName);
    return path + CN_PATH_SEP + CnFileInfo::normalizePathSeparators(fileName);
}

CnString CnDir::relativeFilePath(const CnString &fileName) const
{
    CnFileInfo fi(fileName);
    if (fi.isRelative())
        return fileName;
    CnString absDirPath = CnDir::cleanPath(this->absolutePath());
    CnString absFilePath = CnDir::cleanPath(fi.absoluteFilePath());
    if (isRoot())
        return absFilePath.substr(absDirPath.length());
    int matchSepIndex = -1;
    const CnChar *fdata = absFilePath.data();
    const CnChar *ddata = absDirPath.data();
    int minLen = static_cast<int>(absDirPath.length() < absFilePath.length() ? absDirPath.length() : absFilePath.length()) + 1; // Note: `+1` to include the trailing '\0'
    for (int i = 0; i < minLen; ++i)
    {
        CnChar fch = fdata[i];
        CnChar dch = ddata[i];
        switch (fch)
        {
        case CnCHR('\0'):
        case CN_PATH_SEP:
            switch (dch)
            {
            case CnCHR('\0'):
            case CN_PATH_SEP:
                matchSepIndex = i;
                continue;
            }
            break;
        }
        if (fch != dch)
            break;
    }
    if (matchSepIndex < 0)
        return CnString();
    CnString relPath;
    minLen = static_cast<int>(absDirPath.length()) + 1; // +1 to include the trailing '\0'
    for (int i = matchSepIndex + 1; i < minLen; ++i)
    {
        CnChar dch = ddata[i];
        switch (dch)
        {
        case CnCHR('\0'):
        case CN_PATH_SEP:
            relPath += CnSTR(".." _CN_STR_SEP);
            break;
        }
    }
    relPath += absFilePath.substr(matchSepIndex + 1);
    return relPath;
}

CnString CnDir::absolutePath() const
{
    return d->info.absoluteFilePath();
}

CnString CnDir::canonicalPath() const
{
    return d->info.canonicalFilePath();
}

CnString CnDir::dirName() const
{
    return d->info.fileName();
}

CnString CnDir::filePath(const CnString &fileName) const
{
    CnString path = d->info.filePath();
    if (path.ends_with(CN_PATH_SEP))
        return path + CnFileInfo::normalizePathSeparators(fileName);
    return path + CN_PATH_SEP + CnFileInfo::normalizePathSeparators(fileName);
}

bool CnDir::isAbsolute() const
{
    return d->info.isAbsolute();
}

bool CnDir::isEmpty(int filters) const
{
    return innerEntryInfoList(d->nameFilters, filters, d->sort).empty();
}

bool CnDir::isReadable() const
{
    return d->info.isReadable();
}

bool CnDir::isRelative() const
{
    return d->info.isRelative();
}

bool CnDir::isRoot() const
{
    return d->info.isRoot();
}

int CnDir::filter() const
{
    return d->filter;
}

void CnDir::setFilter(int filters)
{
    d->filter = filters;
}

CnStringList CnDir::nameFilters() const
{
    return d->nameFilters;
}

void CnDir::setNameFilters(const CnStringList &nameFilters)
{
    CNDIR_DETACH
    d->nameFilters = nameFilters;
}

int CnDir::sorting() const
{
    return d->sort;
}

void CnDir::setSorting(int sort)
{
    CNDIR_DETACH
    d->sort = sort;
}

size_t CnDir::count() const
{
    CnFileInfoList fileInfoList = entryInfoList(d->nameFilters, d->filter, d->sort);
    return fileInfoList.size();
}

CnFileInfoList CnDir::entryInfoList(const CnStringList &nameFilters, int filters, int sort) const
{
    CnStringList nf = nameFilters;
    if (nf.empty())
        nf = d->nameFilters;
    filters = (filters == NoFilter) ? d->filter : filters;
    sort = (sort == NoSort) ? d->sort : sort;
    return innerEntryInfoList(nf, filters, sort);
}

CnFileInfoList CnDir::entryInfoList(int filters, int sort) const
{
    CnStringList nf = d->nameFilters;
    filters = (filters == NoFilter) ? d->filter : filters;
    sort = (sort == NoSort) ? d->sort : sort;
    return innerEntryInfoList(nf, filters, sort);
}

CnStringList CnDir::entryList(const CnStringList &nameFilters, int filters, int sort) const
{
    CnFileInfoList fileInfoList = entryInfoList(nameFilters, filters, sort);
    CnStringList result;
    for (const auto &info : fileInfoList)
        result.push_back(info.fileName());
    return result;
}

CnStringList CnDir::entryList(int filters, int sort) const
{
    CnFileInfoList fileInfoList = entryInfoList(filters, sort);
    CnStringList result;
    for (const auto &info : fileInfoList)
        result.push_back(info.fileName());
    return result;
}

bool CnDir::makeAbsolute()
{
    if (d->info.isNull())
        return false;
    if (d->info.isAbsolute())
        return true;
    CnString absPath = d->info.canonicalFilePath();
    if (absPath.isEmpty())
        return false;
    CNDIR_DETACH
    d->info.setFile(absPath);
    return true;
}

bool CnDir::cd(const CnString &dirName)
{
    if (d->info.isNull())
        return false;
    CnFileInfo newInfo(cleanPath(d->info.filePath() + CN_PATH_SEP + dirName));
    if (!newInfo.exists() || !newInfo.isDir())
        return false;
    CNDIR_DETACH
    d->info.swap(newInfo);
    return true;
}

bool CnDir::cdUp()
{
    if (d->info.isNull())
        return false;
    CnFileInfo newInfo(cleanPath(d->info.filePath() + CnSTR(_CN_STR_SEP"..")));
    if (!newInfo.exists() || !newInfo.isDir())
        return false;
    CNDIR_DETACH
    d->info.swap(newInfo);
    return true;
}

void CnDir::refresh()
{
    CNDIR_DETACH
    d->info.refresh();
}

void CnDir::swap(CnDir &other)
{
    auto *tmp = d;
    d = other.d;
    other.d = tmp;
}
