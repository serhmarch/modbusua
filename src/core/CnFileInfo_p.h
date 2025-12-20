#ifndef CNFILEINFO_P_H
#define CNFILEINFO_P_H

#include <CnFileInfo.h>

class CnFileInfoPrivate
{
public:
    CnFileInfoPrivate()
        : ref(1)
    {
    }

    CnFileInfoPrivate(const CnFileInfoPrivate &other) 
    {
        ref = 1;
        filePath = other.filePath;
        fileName = other.fileName;
        dirName = other.dirName;
    }

    virtual ~CnFileInfoPrivate()
    {
    }

public:
    inline void incRef() { ++ref; }
    inline void decRef() { if (--ref < 1) delete this; }

public:
    CnRefCount_t ref;
    CnString filePath;
    CnString fileName;
    CnString dirName;
};

CnFileInfoPrivate* getNullCnFileInfoPrivate();
CnFileInfoPrivate* createCnFileInfoPrivate();
CnFileInfoPrivate* createCnFileInfoPrivate(const CnFileInfoPrivate* other);

#define CNFILEINFO_DETACH \
    if (d->ref > 1) { \
        CnFileInfoPrivate* dd = createCnFileInfoPrivate(d); \
        d->decRef(); \
        d = dd; \
    }

#endif // CNFILEINFO_P_H
