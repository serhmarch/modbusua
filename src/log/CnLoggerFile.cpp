#include "CnLoggerFile.h"

#include <CnStd_sstream.h>

#include <CnUtils.h>
#include <CnFileInfo.h>
#include <CnDir.h>
#include <app/CnApp.h>

CnLoggerFile::Strings::Strings() : CnLogger::Strings(),
    cfg_filePath    (CnSTR("filePath")),
    cfg_fileMaxCount(CnSTR("fileMaxCount")),
    cfg_fileMaxSize (CnSTR("fileMaxSize"))
{
}

const CnLoggerFile::Strings &CnLoggerFile::Strings::instance()
{
    static Strings s;
    return s;
}

size_t CnLoggerFile::toMaxSize(const CnVariant &v, bool *ok)
{
    if (v.type() == CnVariant::vString)
    {
        CnString s = Cn::toUpper(Cn::trim(v.toString()));
        size_t factor = 1;
        if (s.ends_with(CnSTR("KB")))
        {
            factor = 1024;
            s = Cn::trim(s.substr(0, s.size() - 2));
        }
        else if (s.ends_with(CnSTR("MB")))
        {
            factor = 1024 * 1024;
            s = Cn::trim(s.substr(0, s.size() - 2));
        }
        else if (s.ends_with(CnSTR("GB")))
        {
            factor = 1024 * 1024 * 1024;
            s = Cn::trim(s.substr(0, s.size() - 2));
        }
        bool okInner = false;
        size_t value = Cn::toUInt(s, &okInner);
        if (okInner)
        {
            if (ok)
                *ok = true;
            return value * factor;
        }
    }
    return v.toUInt(ok);
}

CnLoggerFile::CnLoggerFile() : CnLogger()
{
    m_isKeepContent = true;
    m_currentFileIndex = 0;
    m_currentFileSize  = 0;
    m_maxSize  = CNLOGFILE_MAXSIZE ;
    m_maxFiles = CNLOGFILE_MAXFILES;
}

CnLoggerFile::~CnLoggerFile()
{
    m_currentFile.close();
}

void CnLoggerFile::logMessage(const CnString &source, const Message &msg)
{
    CnCriticalSectionLocker _(&m_cs);
    if (!m_currentFile.isOpen()) 
    {
        openLogFile();
    }

    if ((m_maxFiles > 1) && (m_currentFileSize > m_maxSize))
        rotateToNextFile();

    CnString s = toString(source, msg) + CnCHR('\n');
    m_currentFile << s;
    m_currentFile.flush();
    m_currentFileSize += s.size();

}

void CnLoggerFile::setConfigInner(const Cn::Config &config)
{
    const Strings &s = Strings::instance();
    Cn::Config::const_iterator it;
    Cn::Config::const_iterator end = config.end();

    bool ok;

    if (m_currentFile.isOpen())
        m_currentFile.close();
    m_isKeepContent = true;

    CnLogger::setConfigInner(config);

    it = config.find(s.cfg_filePath); 
    if (it != end) 
        m_filename = it->second.toString();
    else
        m_filename = CnString();

    ok = false;
    it = config.find(s.cfg_fileMaxCount); 
    if (it != end) 
        m_maxFiles = it->second.toInt(&ok);
    if (!ok)
        m_maxFiles = CNLOGFILE_MAXFILES;
    
    ok = false;
    it = config.find(s.cfg_fileMaxSize); 
    if (it != end) 
        m_maxSize = it->second.toInt(&ok);
    if (!ok)
        m_maxSize = CNLOGFILE_MAXSIZE;
    
    CnDir mainLogDir = CnApp::logDir();
    CnString abs = mainLogDir.absoluteFilePath(m_filename);
    CnFileInfo fi(abs);
    CnString absDir = fi.absolutePath();
    CnString relDirPath = mainLogDir.relativeFilePath(absDir);
    mainLogDir.mkpath(relDirPath);
    m_logDir = absDir;
    m_baseFilename = fi.completeBaseName();
    m_suffix = fi.suffix();
    rotateIfNeeded();
}

void CnLoggerFile::openLogFile()
{
    m_currentFile.close();
    m_currentFileSize = 0;
    CnString filePath = getLogFilePath(m_currentFileIndex);
    m_currentFile.setFileName(filePath);
    if (m_isKeepContent)
    {
        m_currentFile.open(Cn::Open_WriteOnly | Cn::Open_Append);
        if (m_currentFile.isOpen())
            m_isKeepContent = false;
    }
    else
    {
        m_currentFile.open(Cn::Open_WriteOnly | Cn::Open_Truncate);
    }
    if (m_currentFile.isOpen())
        m_currentFileSize = m_currentFile.size();
    //else
    //    std::cerr << CnSTR("Failed to open log file");
    
}

void CnLoggerFile::rotateToNextFile()
{
    if (m_currentFileIndex >= (m_maxFiles-1))
        m_currentFileIndex = 0;
    else    
        ++m_currentFileIndex;
    openLogFile();
}

void CnLoggerFile::rotateIfNeeded() 
{
    int i;
    for (i = 0; i < m_maxFiles; ++i) 
    {
        CnString filePath = getLogFilePath(i);

        CnFileInfo fi(filePath);
        // TODO: check oldest file
        if (!fi.exists() || static_cast<size_t>(fi.size()) < m_maxSize) 
        {
            m_currentFileIndex = i;
            break;
        }
    }
    if (i == m_maxFiles) 
        m_currentFileIndex = 0;
    openLogFile();
}

CnString CnLoggerFile::getLogFilePath(int index) const
{
    if (m_maxFiles > 1)
    {
        CnStd::stringstream ss;
        ss << m_logDir.data() << CnDir::separator() << m_baseFilename.data() << CnCHR('_');
        ss << std::hex << std::uppercase << std::setw(3) << std::setfill(CnCHR('0')) << index;
        ss << CnCHR('.') << m_suffix.data();
        return CnString(ss.str().data());
    }
    else
        return m_filename;
}
