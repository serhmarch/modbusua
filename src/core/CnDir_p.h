#ifndef CNDIR_P_H
#define CNDIR_P_H

#include <CnString.h>
#include <CnFileInfo.h>
#include <CnDir.h>

class CnDirPrivate
{
public:
    CnDirPrivate()
        : ref(1)
    {
        filter = CnDir::NoFilter;
        sort = CnDir::NoSort;
    }

    CnDirPrivate(const CnDirPrivate &other) 
    {
        ref = 1;
        info = other.info;
        filter = other.filter;
        nameFilters = other.nameFilters;
        sort = other.sort;
    }

    virtual ~CnDirPrivate()
    {
    }

public:
    inline void incRef() { ++ref; }
    inline void decRef() { if (--ref < 1) delete this; }

public:
    CnRefCount_t ref;
    CnFileInfo info;
    CnStringList nameFilters;
    int filter;
    int sort;
};

CnDirPrivate* getNullCnDirPrivate();
CnDirPrivate* createCnDirPrivate();
CnDirPrivate* createCnDirPrivate(const CnDirPrivate* other);

#define CNDIR_DETACH \
    if (d->ref > 1) { \
        CnDirPrivate* dd = createCnDirPrivate(d); \
        d->decRef(); \
        d = dd; \
    }

#endif // CNDIR_P_H
