#ifndef CHFILE_P_UNIX_H
#define CHFILE_P_UNIX_H

#include "../CnFile_p.h"
#include "CnUnix.h"

#define CN_UNIX_INVALID_FD -1

class CnFilePrivateUnix : public CnFilePrivate
{
public:
    CnFilePrivateUnix()
        : CnFilePrivate()
        , fd(CN_UNIX_INVALID_FD)
    {
    }

public:
    inline bool isValid() const { return fd != CN_UNIX_INVALID_FD; }
    inline bool isOpen() const { return fd != CN_UNIX_INVALID_FD; }

public:
    int fd;
};

inline CnFilePrivateUnix* d_cast(CnFilePrivate* p) { return static_cast<CnFilePrivateUnix*>(p); }

#endif // CHFILE_P_UNIX_H