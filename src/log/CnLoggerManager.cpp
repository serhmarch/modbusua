#include "CnLoggerManager.h"

#include <CnStd_stdio.h>

CnLoggerManager::CnLoggerManager()
{
}

void CnLoggerManager::logMessage(Cn::LogFlag category, const CnChar *format, ...)
{
    va_list args;
    va_start(args, format);
    vlogMessage(category, format, args);
    va_end(args);
}

void CnLoggerManager::vlogMessage(Cn::LogFlag category, const CnChar *format, std::va_list args)
{
    CnChar buffer[CN_LOGMESSAGE_MAXLEN];
    CnStd::vsnprintf(buffer, CN_LOGMESSAGE_MAXLEN, format, args);

    CnLogger::Message message;
    message.time = CnDateTime::currentLocalDateTime();
    message.category = category;
    message.text = buffer;
    m_cs.lock();
    CnList<CnLoggerPtr> loggers = m_loggers;
    m_cs.unlock();
    for (CnLoggerPtr &logger : loggers)
        logger->logMessage(m_name, message);
}
