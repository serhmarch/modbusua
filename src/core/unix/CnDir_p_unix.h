#ifndef CHDIR_P_UNIX_H
#define CHDIR_P_UNIX_H

#include "../CnDir_p.h"
#include "CnUnix.h"

#define CN_UNIX_INVALID_FD -1

class CnDirPrivateUnix : public CnDirPrivate
{
public:
    CnDirPrivateUnix() : CnDirPrivate()
    {
    }

    CnDirPrivateUnix(const CnDirPrivateUnix &other) : CnDirPrivate(other)
    {
    };
};

#endif // CHDIR_P_UNIX_H