#ifndef CNLOGGERMANAGER_H
#define CNLOGGERMANAGER_H

#include <cstdarg>
#include "CnLogger.h"

/*! \brief Aggregates loggers and routes messages by category.
    \details Holds a collection of `CnLogger` instances and provides
    printf-style APIs to emit messages respecting enabled `logFlags`.
*/
class CnLoggerManager
{
public:
    /// \details Constructs an empty manager with no loggers.
    CnLoggerManager();

public:
    /// \details Returns the manager display name.
    inline CnString name() const { CnCriticalSectionLocker _(&m_cs); return m_name; }
    /// \details Sets the manager display name.
    inline void setName(const CnString &name) { CnCriticalSectionLocker _(&m_cs); m_name = name; }

    /// \details Returns enabled logging flags.
    inline Cn::LogFlags logFlags() const { CnCriticalSectionLocker _(&m_cs); return m_logFlags; }
    /// \details Sets enabled logging flags.
    inline void setLogFlags(Cn::LogFlags logFlags) { CnCriticalSectionLocker _(&m_cs); m_logFlags = logFlags; }

    /// \details Returns the list of registered loggers.
    inline CnList<CnLoggerPtr> loggers() const { CnCriticalSectionLocker _(&m_cs); return m_loggers; }
    /// \details Replaces the list of registered loggers.
    inline void setLoggers(CnList<CnLoggerPtr> loggers) { CnCriticalSectionLocker _(&m_cs); m_loggers = loggers; }

public:
    /// \details Logs a formatted message with a specific category.
    void logMessage(Cn::LogFlag category, const CnChar *format, ...);
    /// \details Logs a formatted message using a `va_list`.
    void vlogMessage(Cn::LogFlag category, const CnChar *format, std::va_list args);

public:
    /// \name Logging helpers
    /// Group `void log<Category>(const CnChar* format, ...)` adds a
    /// formatted message into the log where `<Category>` denotes the
    /// message category. Signature matches C `printf` formatting.
    ///@{ 
    template<typename... Args>
    /// \details Logs an error if `Log_Error` is enabled.
    inline void logError(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Error) this->logMessage(Cn::Log_Error, format, args...);
    }

    template<typename... Args>
    /// \details Logs a warning if `Log_Warning` is enabled.
    inline void logWarning(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Warning) this->logMessage(Cn::Log_Warning, format, args...);
    }

    template<typename... Args>
    /// \details Logs an info message if `Log_Info` is enabled.
    inline void logInfo(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Info) this->logMessage(Cn::Log_Info, format, args...);
    }

    template<typename... Args>
    /// \details Logs a trace message if `Log_Trace` is enabled.
    inline void logTrace(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Trace) this->logMessage(Cn::Log_Trace, format, args...);
    }

    template<typename... Args>
    /// \details Logs detailed trace if `Log_TraceDetails` is enabled.
    inline void logTraceDetails(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_TraceDetails) this->logMessage(Cn::Log_TraceDetails, format, args...);
    }

    template<typename... Args>
    /// \details Logs ctor/dtor events if `Log_CtorDtor` is enabled.
    inline void logCtorDtor(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_CtorDtor) this->logMessage(Cn::Log_CtorDtor, format, args...);
    }

    template<typename... Args>
    /// \details Logs entry/exit if `Log_EntryExit` is enabled.
    inline void logEntryExit(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_EntryExit) this->logMessage(Cn::Log_EntryExit, format, args...);
    }

    template<typename... Args>
    /// \details Logs thread start/stop if `Log_ThreadStartStop` is enabled.
    inline void logThreadStartStop(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_ThreadStartStop) this->logMessage(Cn::Log_ThreadStartStop, format, args...);
    }

    template<typename... Args>
    /// \details Logs connection events if `Log_Connection` is enabled.
    inline void logConnection(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Connection) this->logMessage(Cn::Log_Connection, format, args...);
    }

    template<typename... Args>
    /// \details Logs item events if `Log_Item` is enabled.
    inline void logItem(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Item) this->logMessage(Cn::Log_Item, format, args...);
    }

    template<typename... Args>
    /// \details Logs generic message if `Log_Message` is enabled.
    inline void logMessage(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Message) this->logMessage(Cn::Log_Message, format, args...);
    }

    template<typename... Args>
    /// \details Logs protocol details if `Log_Protocol` is enabled.
    inline void logProtocol(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Protocol) this->logMessage(Cn::Log_Protocol, format, args...);
    }

    template<typename... Args>
    /// \details Logs received data if `Log_Receive` is enabled.
    inline void logReceive(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Receive) this->logMessage(Cn::Log_Receive, format, args...);
    }

    template<typename... Args>
    /// \details Logs sent data if `Log_Send` is enabled.
    inline void logSend(const CnChar* format, Args... args)
    {
        if (this->logFlags() & Cn::Log_Send) this->logMessage(Cn::Log_Send, format, args...);
    }
    ///@}
        
private:
    mutable CnCriticalSection m_cs;
    CnString m_name;
    Cn::LogFlags m_logFlags;
    CnList<CnLoggerPtr> m_loggers;
};

#endif // CNLOGGERMANAGER_H