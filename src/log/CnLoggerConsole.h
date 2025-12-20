/*!\
 * \file   CnLoggerConsole.h
 * \brief  Console logger implementation.
 * 
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNLOGGERCONSOLE_H
#define CNLOGGERCONSOLE_H

#include "CnLogger.h"

/*! \brief Logger that writes formatted messages to console output.
    \details Uses base formatting to print lines to the console.
*/
class CnLoggerConsole : public CnLogger
{
public:
    /// \details Constructs the console logger.
    CnLoggerConsole();

public:
    /// \details Formats and writes a message to the console.
    void logMessage(const CnString &source, const Message &msg) override;
};
#endif // CNLOGGERCONSOLE_H
