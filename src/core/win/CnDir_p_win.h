#ifndef CHDIR_P_WIN_H
#define CHDIR_P_WIN_H

#include "../CnDir_p.h"
#include "CnWindows.h"

#define CN_UNIX_INVALID_FD -1

class CnDirPrivateWin : public CnDirPrivate
{
public:
    CnDirPrivateWin() : CnDirPrivate()
    {
    }

    CnDirPrivateWin(const CnDirPrivateWin &other) : CnDirPrivate(other)
    {
    };
};

#endif // CHDIR_P_WIN_H