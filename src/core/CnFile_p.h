#ifndef CNFILE_P_H
#define CNFILE_P_H

#include <CnString.h>
#include <CnFileInfo.h>
#include <CnFile.h>

class CnFilePrivate
{
public:
    CnFilePrivate()
    {
        lastError = CnFile::NoError;
    }

    virtual ~CnFilePrivate()
    {
    }

public:
    CnFileInfo info;
    CnFile::Error lastError;
    CnString errorString;
};

CnFilePrivate* createCnFilePrivate();

#endif // CNFILE_P_H