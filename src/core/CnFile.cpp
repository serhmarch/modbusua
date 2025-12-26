#include <CnFile.h>
#include "CnFile_p.h"

CnFile::CnFile()
    : d(createCnFilePrivate())
{
}

CnFile::CnFile(const CnString &fileName)
    : d(createCnFilePrivate())
{
    this->setFileName(fileName);
}

CnFile::~CnFile()
{
    this->close();
    delete d;
}

CnFile& CnFile::operator<<(const CnString &str)
{
    this->write(str.chars());
    return *this;
}

CnFile& CnFile::operator<<(CnChar ch)
{
    this->write(&ch, 1);
    return *this;
}

bool CnFile::isReadable() const
{
    return d->info.isReadable();
}

bool CnFile::isWritable() const
{
    return d->info.isWritable();
}

bool CnFile::isExecutable() const
{
    return d->info.isExecutable();
}

CnString CnFile::fileName() const
{
    return d->info.fileName();
}

void CnFile::setFileName(const CnString &fileName)
{
    d->info.setFile(fileName);
}

int CnFile::permissions() const
{
    return d->info.permissions();
}

int64_t CnFile::size() const
{
    d->info.refresh();
    return d->info.size();
}

CnString CnFile::symLinkTarget() const
{
    return d->info.symLinkTarget();
}

CnFileInfo CnFile::info() const
{
    d->info.refresh();
    return d->info;
}

CnFile::Error CnFile::lastError() const
{
    return d->lastError;
}

CnString CnFile::errorString() const
{
    return d->errorString;
}

void CnFile::clearError()
{
    d->lastError = NoError;
    d->errorString.clear();
}

bool CnFile::getline(CnString &line)
{
    const int bufsz = 4096;
    char buf[bufsz];
    int64_t n = readLine(buf, bufsz);
    if (n <= 0)
        return false;
    line = CnString::fromChars(buf, n);
    return true;
}