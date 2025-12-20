#include <CnUtils.h>

//TODO: exclude <CnStd_filesystem.h>
#include <CnLog.h>

#include "CnWindows.h"

namespace Cn {

CnChar PathSep = CnCHR('\\');

CnString workingDir()
{
    CnChar path[MAX_PATH];
    DWORD length = GetCurrentDirectory(MAX_PATH, path);
    if (length == 0 || length > MAX_PATH)
        return Cn::toString(path);
    return CnString();
}

CnString applicationAbsolutePath()
{
    CnChar path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    return CnString(path);
}

void setConsoleColor(Color color)
{
    switch(color)
    {
    case Color_Black  : SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0); break;
    case Color_Red    : SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED); break;
    case Color_Green  : SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN); break;    
    case Color_Yellow : SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN); break; 
    case Color_Blue   : SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE); break;   
    case Color_Magenta: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_BLUE); break;
    case Color_Cyan   : SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_BLUE); break; 
    case Color_White  : 
    case Color_Default:
    default:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); break;
    }
}

} // namespace Cn
