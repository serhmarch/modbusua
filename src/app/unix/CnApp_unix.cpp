#include <app/CnApp.h>

#include <unistd.h>
#include <cstdlib>
#include <csignal>

#include <app/CnAppConfig.h>

CnString CnApp::applicationFilePath()
{
    char path[PATH_MAX];

#if defined(__linux__)
    // Linux: Use /proc/self/exe
    ssize_t size = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (size > 0)
    {
        return CnString::fromChars(path, size);
    }
#elif defined(__APPLE__)
    // macOS: Use _NSGetExecutablePath
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        return CnString::fromChars(path, size);
    }
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    // BSD: Use sysctl to get the executable path
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
    size_t size = sizeof(path);
    if (sysctl(mib, 4, path, &size, nullptr, 0) == 0)
    {
        return CnString::fromChars(path, size);
    }
#endif

    return CnString();
}


CnString CnApp::applicationDirPath()
{
    char path[PATH_MAX];

#if defined(__linux__)
    // Linux: Use /proc/self/exe
    ssize_t size = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (size > 0)
    {
        char* lastSep = strrchr(path, '/');
        if (lastSep != nullptr)
        {
            *lastSep = '\0';  // Terminate the string at the last '/'
            return CnString::fromChars(path, lastSep - path);
        }
    }
#elif defined(__APPLE__)
    // macOS: Use _NSGetExecutablePath
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        char* lastSep = strrchr(path, '/');
        if (lastSep != nullptr)
        {
            *lastSep = '\0';  // Terminate the string at the last '/'
            return CnString::fromChars(path, lastSep - path);
        }
    }
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    // BSD: Use sysctl to get the executable path
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
    size_t size = sizeof(path);
    if (sysctl(mib, 4, path, &size, nullptr, 0) == 0)
    {
        char* lastSep = strrchr(path, '/');
        if (lastSep != nullptr)
        {
            *lastSep = '\0';  // Terminate the string at the last '/'
            return CnString::fromChars(path, lastSep - path);
        }
    }
#endif

    return CnString();
}


Cn::StatusCode CnApp::serviceStart()
{
    CnString serviceName = getServiceName();

    // Determine if prog was started by systemd
    // When starting service, systemd set INVOCATION_ID env variable
    if (std::getenv("INVOCATION_ID") == nullptr)
    {
        // Console-log is disabled for service
        // Create it only for non-service (console) mode
        auto r = std::signal(SIGHUP, CnApp::signal_reload);
        if (r == SIG_ERR)
            CN_LOG_Warning(CnSTR("Can't set signal handler for SIGHUP. Error: %s"), Cn::getLastErrorText().data());
        createLoggerConsole();
        CN_LOG_Info(CnSTR("Can't start '%s' as service '%s'. Switch into console mode"), m_name.data(), serviceName.data()  );
        return Cn::Status_BadNoService;
    }

    createLoggerService();
    // int daemon(int nochdir, int noclose):
    // If `nochdir` is zero, daemon() changes the process's current working
    // directory to the root directory ("/"); otherwise, the current
    // working directory is left unchanged.
    // If `noclose` is zero, daemon() redirects standard input, standard
    // output, and standard error to /dev/null; otherwise, no changes are
    // made to these file descriptors.
    if (daemon(1, 0)) // Detach from terminal, on error return -1
    {
        CN_LOG_Error(CnSTR("Daemon mode for '%s' failed. Error: %s"), serviceName.data(), Cn::getLastErrorText().data());
        return Cn::Status_Bad;
    }

    auto r = std::signal(SIGHUP, CnApp::signal_reload);
    if (r == SIG_ERR)
        CN_LOG_Warning(CnSTR("Can't set signal handler for SIGHUP. Error: %s"), Cn::getLastErrorText().data());

    CN_LOG_Info(CnSTR("Start '%s' as service '%s'"), m_name.data(), serviceName.data());
    int res = this->run();
    if (res)
        return Cn::Status_Bad;
    return Cn::Status_Good;
}

void CnApp::initSpec()
{
    m_defaultLogDirPath = CnSTR("/var/log/") + m_name;
    m_logDir.setPath(m_defaultLogDirPath);
    m_confDir.setPath(CnSTR("/etc/") + m_name);
}

void CnApp::finalSpec()
{
}

