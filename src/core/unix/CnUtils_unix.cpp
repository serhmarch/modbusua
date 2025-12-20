#include <CnUtils.h>

#include <iostream>

#include <limits.h>
#include <unistd.h>

#include <CnLog.h>

#include "CnUnix.h"

namespace Cn {

CnChar PathSep = CnCHR('/');

void setConsoleColor(Color color)
{
    switch(color)
    {
    case Color_Black  : std::cout << "\033[30m"; break;
    case Color_Red    : std::cout << "\033[31m"; break;
    case Color_Green  : std::cout << "\033[32m"; break;
    case Color_Yellow : std::cout << "\033[33m"; break;
    case Color_Blue   : std::cout << "\033[34m"; break;
    case Color_Magenta: std::cout << "\033[35m"; break;
    case Color_Cyan   : std::cout << "\033[36m"; break; 
    case Color_White  : std::cout << "\033[37m"; break;
    case Color_Default:
    default:            std::cout << "\033[0m" ; break;
    }
}

} // namespace Cn
