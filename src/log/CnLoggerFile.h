/*!\
 * \file   CnLoggerFile.h
 * \brief  File-based logger implementation with rotation.
 * 
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNLOGGERFILE_H
#define CNLOGGERFILE_H

#include <CnFile.h>

#include "CnLogger.h"

#define CNLOGFILE_MAXSIZE  (1024 * 1024) // 1 MB
#define CNLOGFILE_MAXFILES 10

/*! \brief Logger that writes messages to rotating log files.
    \details Supports file size limits and file count rotation.
*/
class CnLoggerFile : public CnLogger
{
public:
    /// \details Converts a variant to max size in bytes; sets `ok`.
    static size_t toMaxSize(const CnVariant &v, bool *ok);

public:
    /*! \brief Configuration key names specific to file logger.
    */
    struct Strings : public CnLogger::Strings
    {
        /// \brief Config key: log file path.
        CnString cfg_filePath    ;
        /// \brief Config key: max number of rotated files.
        CnString cfg_fileMaxCount;
        /// \brief Config key: max size of a single file.
        CnString cfg_fileMaxSize ;

        /// \details Constructs the key names container.
        Strings();
        /// \details Returns the singleton with file logger keys.
        static const Strings& instance();
    };

public:
    /// \details Constructs the file logger.
    CnLoggerFile();
    /// \details Destructor closes files if needed.
    ~CnLoggerFile();

public:
    /// \details Formats and appends a message to the current log file.
    void logMessage(const CnString &source, const Message &msg) override;

private:
    void setConfigInner(const Cn::Config &config) override;
    void openLogFile();
    void rotateToNextFile();
    void rotateIfNeeded();
    CnString getLogFilePath(int index) const;

private:
    CnFile m_currentFile;
    bool m_isKeepContent;
    int m_maxFiles;
    size_t m_maxSize;
    size_t m_currentFileSize;
    int m_currentFileIndex;
    CnString m_filename;
    CnString m_logDir;
    CnString m_baseFilename;
    CnString m_suffix;
};

#endif // CNLOGGERFILE_H