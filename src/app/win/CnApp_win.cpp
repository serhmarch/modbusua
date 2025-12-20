#include <app/CnApp.h>

const CnChar *os_log_file_dir = CnSTR("./log/");

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle = NULL;

// GatewayReportServiceStatus: Reports the service status to the SCM
void GatewayReportServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    ServiceStatus.dwCurrentState = dwCurrentState;
    ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
    ServiceStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
    {
        ServiceStatus.dwControlsAccepted = 0;
    }
    else
    {
        ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    SetServiceStatus(hServiceStatusHandle, &ServiceStatus);
}

// GatewayServiceControlHandler: Handles control requests from the SCM
void WINAPI GatewayServiceControlHandler(DWORD dwCtrlCode)
{
    switch (dwCtrlCode)
    {
    case SERVICE_CONTROL_STOP:
        GatewayReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        CnApp::cmdStop();
        break;
    default:
        GatewayReportServiceStatus(ServiceStatus.dwCurrentState, NO_ERROR, 0);
        break;
    }
}

// GatewayServiceMain: Main service function
void WINAPI GatewayServiceMain(DWORD /*argc*/, LPTSTR* /*argv*/)
{
    CnApp *sys = CnApp::global();
    sys->moveToThread(CnThread::currentThread());
    
    hServiceStatusHandle = RegisterServiceCtrlHandler(sys->name().data(), GatewayServiceControlHandler);

    if (hServiceStatusHandle == NULL)
    {
        return;
    }

    // Initialize service status
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwServiceSpecificExitCode = 0;

    //GatewayReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    // Report running status
    GatewayReportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);

    // Run the service worker thread
    CnApp::global()->run();

    // Cleanup
    GatewayReportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

CnString CnApp::applicationFilePath()
{
    char path[MAX_PATH];
    DWORD len = ::GetModuleFileNameA(nullptr, path, MAX_PATH);
    if (len > 0)
    {
        return CnString::fromChars(path, len);
    }
    return CnString();
}

CnString CnApp::applicationDirPath()
{
    char path[MAX_PATH];
    DWORD len = ::GetModuleFileNameA(nullptr, path, MAX_PATH);
    if (len > 0)
    {
        // Find the last backslash or forward slash to get directory
        char* lastSep = strrchr(path, '\\');
        if (lastSep)
        {
            *lastSep = '\0';  // Terminate at directory
            return CnString::fromChars(path, lastSep - path);
        }
        return CnString::fromChars(path);
    }
    return CnString();
}

Cn::StatusCode CnApp::serviceStart()
{
    CnString serviceName = m_options.service.name;

    SERVICE_TABLE_ENTRY ServiceTable[] = {
        { const_cast<LPTSTR>(serviceName.data()), (LPSERVICE_MAIN_FUNCTION)GatewayServiceMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcher(ServiceTable))
    {
        if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
        {
            // Note: This error is returned if the program is being run as a console application rather than as a service.
            createLoggerConsole();        
            CN_LOG_Info(CnSTR("Can't start '%s' as service. Switch into console mode"), m_name.data());
            return Cn::Status_BadNoService;
        }
        createLoggerService();
        CN_LOG_Error(CnSTR("Error starting service control dispatcher: %d"), GetLastError());
        return Cn::Status_Bad;
    }    
    createLoggerService();        
    return Cn::Status_Good;
}

void CnApp::initSpec()
{
    m_confDir = this->applicationDir().path() + CnSTR("/conf");

    m_logDir = this->applicationDir().path() + CnSTR("/log");
    m_logDir.mkpath(CnString());
}

void CnApp::finalSpec()
{
}

