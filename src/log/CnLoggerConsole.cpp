#include "CnLoggerConsole.h"

#include <app/CnApp.h>

CnLoggerConsole::CnLoggerConsole() : CnLogger()
{
}

void CnLoggerConsole::logMessage(const CnString &source, const Message &msg)
{
    CnString message = toString(source, msg);
    CnApp::logConsole(msg.category, message);
}
