/*!
 * \file   CnLog.h
 * \brief  Defines logging categories and functions for the library.
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNLOG_H
#define CNLOG_H

#include <wchar.h>
#include <stdint.h>
#include <CnStd_sstream.h>

#include <CnTypes.h>

namespace Cn {

/// \brief Defines logging categories as bit flags.
/// \details Each category corresponds to a specific type of log message,
/// allowing fine-grained control over what gets logged.    
enum LogFlag
{
    Log_Error           = 0x0000000000000001, ///< Error messages
    Log_Warning         = 0x0000000000000002, ///< Warning messages
    Log_Info            = 0x0000000000000004, ///< Informational messages
    Log_Trace           = 0x0000000000000008, ///< Trace messages
    Log_TraceDetails    = 0x0000000000000010, ///< Detailed trace messages
    Log_CtorDtor        = 0x0000000000000020, ///< Constructor/Destructor messages
    Log_EntryExit       = 0x0000000000000040, ///< Entry/Exit messages
    Log_ThreadStartStop = 0x0000000000000080, ///< Thread start/stop messages
    Log_Connection      = 0x0000000000000100, ///< Connection messages
    Log_Item            = 0x0000000000000200, ///< Item messages
    Log_Message         = 0x0000000000000400, ///< General messages
    Log_Protocol        = 0x0000000000000800, ///< Protocol messages
    Log_Receive         = 0x0000000000001000, ///< Receive messages
    Log_Send            = 0x0000000000002000, ///< Send messages
    Log_All             = 0xFFFFFFFFFFFFFFFF  ///< All messages
};

namespace Log { // namespace Cn::Log

/// \brief Contains strings for log categories.
/// \details This struct provides human-readable names for each log category,
struct Strings
{
    const CnString Log_Error          ; ///< Error messages
    const CnString Log_Warning        ; ///< Warning messages
    const CnString Log_Info           ; ///< Informational messages
    const CnString Log_Trace          ; ///< Trace messages
    const CnString Log_TraceDetails   ; ///< Detailed trace messages
    const CnString Log_CtorDtor       ; ///< Constructor/Destructor messages
    const CnString Log_EntryExit      ; ///< Entry/Exit messages
    const CnString Log_ThreadStartStop; ///< Thread start/stop messages
    const CnString Log_Connection     ; ///< Connection messages
    const CnString Log_Item           ; ///< Item messages
    const CnString Log_Message        ; ///< General messages
    const CnString Log_Protocol       ; ///< Protocol messages
    const CnString Log_Receive        ; ///< Receive messages
    const CnString Log_Send           ; ///< Send messages
    const CnString Log_All            ; ///< All messages

    /// \details 
    Strings();

    /// \details 
    static const Strings& instance();
};

} // namespace Cn::Log

//typedef uint64_t LogFlags;

class LogFlags
{
public:
    using ValueType = uint64_t;

    constexpr LogFlags() : m_flags(0) {}              
    constexpr LogFlags(ValueType flags) : m_flags(flags) {}  
    constexpr LogFlags(int flags) : m_flags(static_cast<ValueType>(flags)) {}

    inline void set(ValueType flags) { m_flags |= flags; }
    inline void clear(ValueType flags) { m_flags &= ~flags; }
    inline void toggle(ValueType flags) { m_flags ^= flags; }
    inline bool has(ValueType flags) const { return (m_flags & flags) == flags; }
    inline void reset() { m_flags = 0; }

    constexpr operator ValueType() const { return m_flags; }

    //constexpr LogFlags operator|(LogFlags other) const { return LogFlags(m_flags | other.m_flags); }
    LogFlags& operator|=(LogFlags other) { m_flags |= other.m_flags; return *this; }

    //constexpr LogFlags operator&(LogFlags other) const { return LogFlags(m_flags & other.m_flags); }
    LogFlags& operator&=(LogFlags other) { m_flags &= other.m_flags; return *this; }
    
    //constexpr LogFlags operator&(ValueType other) const { return LogFlags(m_flags & other); }
    LogFlags& operator&=(ValueType other) { m_flags &= other; return *this; }

    //constexpr LogFlags operator^(LogFlags other) const { return LogFlags(m_flags ^ other.m_flags); }
    LogFlags& operator^=(LogFlags other) { m_flags ^= other.m_flags; return *this; }

    constexpr LogFlags operator~() const { return LogFlags(~m_flags); }

    //constexpr bool operator==(LogFlags other) const { return m_flags == other.m_flags; }
    //constexpr bool operator!=(LogFlags other) const { return m_flags != other.m_flags; }

    friend CnStd::ostream& operator<<(CnStd::ostream& os, const LogFlags& flags) { return os << flags.m_flags; }

private:
    ValueType m_flags;
};

/// \details
const CnChar *toString(LogFlag flag);

/// \details
LogFlag toLogFlag(const CnChar *sflag);

/// \details
inline LogFlag toLogFlag(const CnString &sflag) { return toLogFlag(sflag.data()); }

/// \details
CnString toString(LogFlags flags);

/// \details
LogFlags toLogFlags(const CnString &sflags);

/// \details
LogFlags logFlags();

/// \details
void logMessage(LogFlag category, const CnChar *format, ...);

} // namespace Cn

#define CN_LOGMESSAGE_MAXLEN 1024

#ifdef __GNUC__

#define __LOG_MESSAGE(category, _MESSAGE, ...) if (Cn::logFlags() & category) Cn::logMessage(category, _MESSAGE, ##__VA_ARGS__ ); 

#define CN_LOG_Error(_MESSAGE, ...)            __LOG_MESSAGE(Cn::Log_Error          , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Warning(_MESSAGE, ...)          __LOG_MESSAGE(Cn::Log_Warning        , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Info(_MESSAGE, ...)             __LOG_MESSAGE(Cn::Log_Info           , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Trace(_MESSAGE, ...)            __LOG_MESSAGE(Cn::Log_Trace          , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_TraceDetails(_MESSAGE, ...)     __LOG_MESSAGE(Cn::Log_TraceDetails   , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_CtorDtor(_MESSAGE, ...)         __LOG_MESSAGE(Cn::Log_CtorDtor       , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_EntryExit(_MESSAGE, ...)        __LOG_MESSAGE(Cn::Log_EntryExit      , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_ThreadStartStop(_MESSAGE, ...)  __LOG_MESSAGE(Cn::Log_ThreadStartStop, _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Connection(_MESSAGE, ...)       __LOG_MESSAGE(Cn::Log_Connection     , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Item(_MESSAGE, ...)             __LOG_MESSAGE(Cn::Log_Item           , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Message(_MESSAGE, ...)          __LOG_MESSAGE(Cn::Log_Message        , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Protocol(_MESSAGE, ...)         __LOG_MESSAGE(Cn::Log_Protocol       , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Receive(_MESSAGE, ...)          __LOG_MESSAGE(Cn::Log_Receive        , _MESSAGE , ##__VA_ARGS__ )
#define CN_LOG_Send(_MESSAGE, ...)             __LOG_MESSAGE(Cn::Log_Send           , _MESSAGE , ##__VA_ARGS__ )

#else // __GNUC__

#define __LOG_MESSAGE(category, _MESSAGE, ...) if (Cn::logFlags() & category) Cn::logMessage(category, _MESSAGE, __VA_ARGS__ ); 

#define CN_LOG_Error(_MESSAGE, ...)            __LOG_MESSAGE(Cn::Log_Error          , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Warning(_MESSAGE, ...)          __LOG_MESSAGE(Cn::Log_Warning        , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Info(_MESSAGE, ...)             __LOG_MESSAGE(Cn::Log_Info           , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Trace(_MESSAGE, ...)            __LOG_MESSAGE(Cn::Log_Trace          , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_TraceDetails(_MESSAGE, ...)     __LOG_MESSAGE(Cn::Log_TraceDetails   , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_CtorDtor(_MESSAGE, ...)         __LOG_MESSAGE(Cn::Log_CtorDtor       , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_EntryExit(_MESSAGE, ...)        __LOG_MESSAGE(Cn::Log_EntryExit      , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_ThreadStartStop(_MESSAGE, ...)  __LOG_MESSAGE(Cn::Log_ThreadStartStop, _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Connection(_MESSAGE, ...)       __LOG_MESSAGE(Cn::Log_Connection     , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Item(_MESSAGE, ...)             __LOG_MESSAGE(Cn::Log_Item           , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Message(_MESSAGE, ...)          __LOG_MESSAGE(Cn::Log_Message        , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Protocol(_MESSAGE, ...)         __LOG_MESSAGE(Cn::Log_Protocol       , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Receive(_MESSAGE, ...)          __LOG_MESSAGE(Cn::Log_Receive        , _MESSAGE , __VA_ARGS__ )
#define CN_LOG_Send(_MESSAGE, ...)             __LOG_MESSAGE(Cn::Log_Send           , _MESSAGE , __VA_ARGS__ )

#endif // __GNUC__

#endif // CNLOG_H
