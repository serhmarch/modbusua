@echo off
setlocal enabledelayedexpansion

:: @CN_APP_NAME@ Service Management Script
:: This script manages @CN_APP_NAME@ Windows Service installation/uninstallation

set "SCRIPT_NAME=%~n0"
set "CN_APP_NAME=@CN_APP_NAME@"
set "DEFAULT_EXE_FILE=%CN_APP_NAME%.exe"
set "DEFAULT_SERVICE_NAME=%CN_APP_NAME%"
set "SERVICE_DESCRIPTION=@CN_APP_DESC@"

:: Initialize variables with defaults
set "EXE_FILE=%DEFAULT_EXE_FILE%"
set "SERVICE_NAME=%DEFAULT_SERVICE_NAME%"
set "ACTION=install"
set "CN_APP_EXTRA="
set "RAW_ARGS=%*"

if "%~1"=="--help" (
    goto :show_help
) else if "%~1"=="-h" (
    goto :show_help
) else if "%~1"=="-?" (
    goto :show_help
) 

:: Check for administrator privileges
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ERROR: Administrator privileges required!
    echo Please run this script as Administrator.
    echo.
::    pause
    exit /b 1
)

:: Parse command line arguments
:parse_args
if "%~1"=="" goto :validate_args

if "%~1"=="--install" (
    set "ACTION=install"
    :: Optional service name immediately after action (short for -n NAME)
    if not "%~2"=="" if not "%~2:~0,1%"=="-" if not "%~2"=="--" (
        set "SERVICE_NAME=%~2"
        shift
        shift
        goto :parse_args
    )
    shift
    goto :parse_args
) else if "%~1"=="-i" (
    set "ACTION=install"
    if not "%~2"=="" if not "%~2:~0,1%"=="-" if not "%~2"=="--" (
        set "SERVICE_NAME=%~2"
        shift
        shift
        goto :parse_args
    )
    shift
    goto :parse_args
) else if "%~1"=="--uninstall" (
    set "ACTION=uninstall"
    if not "%~2"=="" if not "%~2:~0,1%"=="-" if not "%~2"=="--" (
        set "SERVICE_NAME=%~2"
        shift
        shift
        goto :parse_args
    )
    shift
    goto :parse_args
) else if "%~1"=="-u" (
    set "ACTION=uninstall"
    if not "%~2"=="" if not "%~2:~0,1%"=="-" if not "%~2"=="--" (
        set "SERVICE_NAME=%~2"
        shift
        shift
        goto :parse_args
    )
    shift
    goto :parse_args
) else if "%~1"=="--exe" (
    if "%~2"=="" (
        echo ERROR: --exe option requires a filename
        goto :show_help
    )
    set "EXE_FILE=%~2"
    shift
    shift
    goto :parse_args
) else if "%~1"=="-e" (
    if "%~2"=="" (
        echo ERROR: -e option requires a filename
        goto :show_help
    )
    set "EXE_FILE=%~2"
    shift
    shift
    goto :parse_args
) else if "%~1"=="--name" (
    if "%~2"=="" (
        echo ERROR: --name option requires a service name
        goto :show_help
    )
    set "SERVICE_NAME=%~2"
    shift
    shift
    goto :parse_args
) else if "%~1"=="-n" (
    if "%~2"=="" (
        echo ERROR: -n option requires a service name
        goto :show_help
    )
    set "SERVICE_NAME=%~2"
    shift
    shift
    goto :parse_args
) else if "%~1"=="--" (
    rem Capture all remaining args after the delimiter preserving quotes
    set "CN_APP_EXTRA=%RAW_ARGS:*-- =%"
    goto :validate_args
) else (
    echo ERROR: Unknown argument '%~1'
    echo.
    goto :show_help
)


:validate_args
if "%ACTION%"=="" goto :show_help
if "%ACTION%"=="install" goto :install_service
if "%ACTION%"=="uninstall" goto :uninstall_service

:show_help
echo.
echo %CN_APP_NAME% Service Management Script
echo =================================
echo.
echo Usage: %SCRIPT_NAME% [ACTION] [OPTIONS] [-- %CN_APP_NAME% OPTIONS]
echo.
echo Actions:
echo   --install [NAME], -i [NAME]     Install %CN_APP_NAME% as Windows service (start on demand). NAME - optional, short for -n NAME
echo   --uninstall [NAME], -u [NAME]   Uninstall %CN_APP_NAME% service (stop if running). NAME - optional, short for -n NAME
echo   --help, -?                      Show this help message
echo.
echo Options:
echo   --exe FILE, -e FILE       Executable filename (default: %DEFAULT_EXE_FILE%)
echo   --name NAME, -n NAME      Service name (default: %DEFAULT_SERVICE_NAME%)
echo.
echo %CN_APP_NAME% options:
echo   -- delimiter before %CN_APP_NAME% options
echo Example:
echo   %SCRIPT_NAME% -i %CN_APP_NAME%2 -- -f %CN_APP_NAME%2.conf --logdir "C:\Program Files\%CN_APP_NAME%\log2"
echo To see all %CN_APP_NAME% options, run:
echo   %CN_APP_NAME% --help
echo.
echo Description:
echo   This script manages the %CN_APP_NAME% Gateway Windows service installation
echo   and uninstallation. Administrator privileges are required.
echo.
echo Examples:
echo   %SCRIPT_NAME% --install                              Install with defaults
echo   %SCRIPT_NAME% -i -e %DEFAULT_EXE_FILE% -n my%CN_APP_NAME%       Install custom service
echo   %SCRIPT_NAME% -i my%CN_APP_NAME%-- -f conf\my%CN_APP_NAME%.conf Install custom service with app options
echo   %SCRIPT_NAME% --uninstall --name my%CN_APP_NAME%           Remove custom service
echo   %SCRIPT_NAME% -u my%CN_APP_NAME%                           Remove custom service (short form)
echo   %SCRIPT_NAME% --help                                 Show this help
echo.
echo Default Configuration:
echo   Executable  : %DEFAULT_EXE_FILE%
echo   Service Name: %DEFAULT_SERVICE_NAME%
echo   Display Name: %SERVICE_DESCRIPTION%
echo.
goto :end

:install_service
echo Installing %CN_APP_NAME% service...
echo Configuration:
echo   Executable: %EXE_FILE%
echo   Service Name: %SERVICE_NAME%
echo   Display Name: %SERVICE_NAME%
echo.

:: Check if executable exists next to this script (handles spaces)
if not exist "%SCRIPT_DIR%%EXE_FILE%" (
    echo ERROR: Executable '%EXE_FILE%' not found in current directory!
    echo Please ensure the executable file is present before installing the service.
    echo Current directory: %CD%
    echo.
    goto :error_end
)

:: Get full path to executable using script directory
for %%F in ("%SCRIPT_DIR%%EXE_FILE%") do set "EXE_FULL_PATH=%%~fF"

:: Check if service already exists
sc query "%SERVICE_NAME%" >nul 2>&1
if %errorLevel% equ 0 (
    echo WARNING: Service '%SERVICE_NAME%' already exists!
    echo Use --uninstall first to remove the existing service.
    echo.
    goto :error_end
)

:: Build application parameters (default: -s <service-name>) and append extras if provided
set "CN_APP_PARAMS=-s %SERVICE_NAME%"
if defined CN_APP_EXTRA set "CN_APP_PARAMS=%CN_APP_PARAMS% %CN_APP_EXTRA%"
:: Escape embedded quotes for inclusion inside overall quoted binPath value
set "CN_APP_PARAMS_ESC=%CN_APP_PARAMS:"=\"%"

:: Install the service
echo Creating service '%SERVICE_NAME%'...
sc create "%SERVICE_NAME%" ^
    binPath= "\"%EXE_FULL_PATH%\" %CN_APP_PARAMS_ESC%" ^
    DisplayName= "%SERVICE_NAME%" ^
    start= demand ^
    type= own

if %errorLevel% neq 0 (
    echo ERROR: Failed to create service!
    goto :error_end
)

:: Set service description
sc description "%SERVICE_NAME%" "%SERVICE_DESCRIPTION%" >nul 2>&1

:: Set service failure actions (restart on failure)
sc failure "%SERVICE_NAME%" reset= 86400 actions= restart/60000/restart/60000/restart/60000 >nul 2>&1

echo.
echo SUCCESS: Service '%SERVICE_NAME%' installed successfully!
echo.
echo Service Details:
echo   Name: %SERVICE_NAME%
echo   Display Name: %SERVICE_NAME%
echo   Executable: %EXE_FULL_PATH%
echo   Start Type: Manual (On Demand)
echo.
echo Management Commands:
echo   Start:  sc start %SERVICE_NAME%
echo   Stop:   sc stop %SERVICE_NAME%
echo   Query:  sc query %SERVICE_NAME%
echo   GUI:    services.msc
echo.
goto :end

:uninstall_service
echo Uninstalling %CN_APP_NAME% service...
echo Service Name: %SERVICE_NAME%
echo.

:: Check if service exists
sc query "%SERVICE_NAME%" >nul 2>&1
if %errorLevel% neq 0 (
    echo WARNING: Service '%SERVICE_NAME%' does not exist!
    echo Nothing to uninstall.
    echo.
    goto :end
)

:: Check service status
for /f "tokens=4" %%i in ('sc query "%SERVICE_NAME%" ^| findstr "STATE"') do set "SERVICE_STATE=%%i"

:: Stop service if running
if /i "%SERVICE_STATE%"=="RUNNING" (
    echo Service is currently running. Stopping service...
    sc stop "%SERVICE_NAME%" >nul 2>&1
    
    :: Wait for service to stop (max 30 seconds)
    set /a "timeout_counter=0"
    :wait_stop
    timeout /t 1 /nobreak >nul 2>&1
    set /a "timeout_counter+=1"
    
    for /f "tokens=4" %%i in ('sc query "%SERVICE_NAME%" ^| findstr "STATE"') do set "CURRENT_STATE=%%i"
    if /i "!CURRENT_STATE!"=="STOPPED" goto :service_stopped
    if !timeout_counter! geq 30 (
        echo WARNING: Service did not stop within 30 seconds.
        echo Proceeding with uninstallation anyway...
        goto :service_stopped
    )
    goto :wait_stop
    
    :service_stopped
    echo Service stopped successfully.
    echo.
) else if /i "%SERVICE_STATE%"=="STOPPED" (
    echo Service is already stopped.
    echo.
) else (
    echo Service state: %SERVICE_STATE%
    echo.
)

:: Delete the service
echo Removing service '%SERVICE_NAME%'...
sc delete "%SERVICE_NAME%" >nul 2>&1

if %errorLevel% neq 0 (
    echo ERROR: Failed to delete service!
    echo The service may still be stopping. Please try again in a few moments.
    goto :error_end
)

echo.
echo SUCCESS: Service '%SERVICE_NAME%' uninstalled successfully!
echo.
goto :end

:error_end
echo.
echo Script completed with errors.
::pause
exit /b 1

:end
echo Script completed successfully.
::pause
exit /b 0
