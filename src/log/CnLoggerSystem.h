/*!
 * \file   CnLoggerSystem.h
 * \brief  System-level logger integrating with OS facilities.
 * 
 * \author serhmarch
 * \date   August 2025
 */
#ifndef CNLOGGERSYSTEM_H
#define CNLOGGERSYSTEM_H

#include "CnLogger.h"

class CnLoggerSystemPrivate;

/*! \brief Logger that writes to system logging facilities.
    \details Uses platform-specific APIs to send logs to the OS event
    log or similar facilities.
*/
class CnLoggerSystem : public CnLogger
{
public:
    /// \details Constructs the system logger.
    CnLoggerSystem();
    
    /// \details Destructor.
    ~CnLoggerSystem();

public:
    /// \details Formats and sends a message to the system log.
    void logMessage(const CnString &source, const Message &msg) override;

private:
    //CnLoggerSystemPrivate *m_d;
};
#endif // CNLOGGERSYSTEM_H
