#ifndef CNFILE_P_WIN_H
#define CNFILE_P_WIN_H

#include "../CnFile_p.h"
#include "CnWindows.h"

#define CN_WIN_INVALID_HANDLE INVALID_HANDLE_VALUE

class CnFilePrivateWin : public CnFilePrivate
{
public:
    CnFilePrivateWin()
        : CnFilePrivate()
        , hFile(CN_WIN_INVALID_HANDLE)
    {
    }

public:
    inline bool isValid() const { return hFile != CN_WIN_INVALID_HANDLE; }
    inline bool isOpen() const { return hFile != CN_WIN_INVALID_HANDLE; }

public:
    HANDLE hFile;
};

inline CnFilePrivateWin* d_cast(CnFilePrivate* p) { return static_cast<CnFilePrivateWin*>(p); }

#endif // CNFILE_P_WIN_H