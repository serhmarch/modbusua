#include "../CnLoggerSystem.h"

#include <windows.h>

namespace Cn {
    
WORD toEventLogType(LogFlag logFlag)
{
    switch (logFlag)
    {
    case Log_Error   : return EVENTLOG_ERROR_TYPE;
    case Log_Warning : return EVENTLOG_WARNING_TYPE;
    case Log_Info    : return EVENTLOG_INFORMATION_TYPE;
    default          : return EVENTLOG_INFORMATION_TYPE;
    }
}

} // namespace Cn

// Static handle for Windows Event Log
static HANDLE g_hEventLog = nullptr;

CnLoggerSystem::CnLoggerSystem()
{
    // Register event source with Windows Event Log
    if (!g_hEventLog)
    {
        g_hEventLog = ::RegisterEventSourceA(nullptr, CN_APP_NAME);
    }
}

CnLoggerSystem::~CnLoggerSystem()
{
    // Note: We don't deregister here as there might be multiple instances
    // The handle will be cleaned up when the process exits
}

void CnLoggerSystem::logMessage(const CnString &source, const Message &msg)
{
    if (!g_hEventLog)
        return;
        
    std::string s = Cn::toStdString(toString(source, msg));
    const char* strings[] = { s.c_str() };
    
    ::ReportEventA(g_hEventLog,
                   Cn::toEventLogType(msg.category),
                   0,                    // Category
                   0,                    // Event ID
                   nullptr,              // User SID
                   1,                    // Number of strings
                   0,                    // Raw data size
                   strings,              // String array
                   nullptr);             // Raw data
}
