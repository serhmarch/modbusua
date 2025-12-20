#include "../CnLoggerSystem.h"

#include <syslog.h>

namespace Cn {
    
int toSyslogPriority(LogFlag logFlag)
{
    switch (logFlag)
    {
    Log_Error   : return LOG_EMERG  ;
    Log_Warning : return LOG_WARNING;
    Log_Info    : return LOG_INFO   ;
    default     : return LOG_DEBUG  ;
    }
}

} // namespace Cn

CnLoggerSystem::CnLoggerSystem()
{
    openlog(CN_APP_NAME, LOG_PID, LOG_DAEMON);
}

CnLoggerSystem::~CnLoggerSystem()
{
	closelog();
}

void CnLoggerSystem::logMessage(const CnString &source, const Message &msg)
{
    std::string s = Cn::toStdString(toString(source, msg));
    syslog(Cn::toSyslogPriority(msg.category), "%s", s.data());
}
