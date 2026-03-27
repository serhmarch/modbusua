#include "CnApp.h"

#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cwchar>
#include <cassert>
#include <sstream>
#include <csignal>

#include <CnStd_iostream.h>

#include <log/CnLoggerManager.h>
#include <log/CnLoggerSystem.h>
#include <log/CnLoggerConsole.h>
#include <log/CnLoggerFile.h>

#include <port/CnPort.h>
#include <port/CnPortThread.h>
#include <device/CnDevice.h>

#include <cfg/CnCfgProject.h>
#include <cfg/CnCfgPort.h>
#include <cfg/CnCfgDevice.h>
#include <cfg/CnCfgBuilder.h>

#include <device/items/CnDeviceBaseItem.h>

#include <CnItemCsvReader.h>
#include <CnLog.h>
#include <CnEventLoop.h>

#include "CnAppConfig.h"

namespace Cn {

Cn::LogFlags logFlags()
{
    return CnApp::global()->logger()->logFlags();
}

void logMessage(LogFlag category, const CnChar *format, ...)
{
    va_list args;
    va_start(args, format);
    CnApp::global()->logger()->vlogMessage(category, format, args);
    va_end(args);
}

} // namespace Cn

CnApp::Strings::Strings() :
    system (CnSTR("system")),
    console(CnSTR("console")),
    file   (CnSTR("file"))
{
}

const CnApp::Strings &CnApp::Strings::instance()
{
    static const Strings s;
    return s;
}


CnApp *CnApp::s_global = nullptr;

void CnApp::signal_handler(int /*signal*/)
{
    CnApp::cmdStop();
}

void CnApp::signal_reload(int /*signal*/)
{
    CnApp::cmdReloadConfig();
}

CnString CnApp::absolutePath(const CnString &path)
{
    // TODO: canonical path
    return global()->m_applicationDir.absoluteFilePath(path);
}

void CnApp::sendEvent(CnObject *object, CnEvent *event)
{
    object->event(event);
    delete event;
}

void CnApp::postEvent(CnObject *object, CnEvent *event)
{
    //object->pushEvent(event);
    object->thread()->postEvent(object, event);
}

CnApp::CnApp()
{
    assert(s_global == nullptr);
    s_global = this;

	m_ctrlRun = true;
	m_cmdReload = false;

	m_name = CnSTR(CN_APP_NAME);
    m_description = CnSTR(CN_APP_DESC);
    m_defaultFileConf = CnSTR(CN_APP_CONF);
    m_applicationDir.setPath(applicationDirPath());

    initSpec();

    m_cfg = new CnAppConfig();
    m_project.cfg = nullptr;

    m_loggers.system = new CnLoggerSystem();
    m_loggers.system->setFormat(CnAppConfig::Defaults::instance().LogDefaultFormat);
    m_loggers.system->setTimeformat(CnAppConfig::Defaults::instance().LogDefaultTimeformat);
    m_loggers.manager = new CnLoggerManager();
}

CnApp::~CnApp()
{
    finalSpec();
    delete m_loggers.manager;
    delete m_cfg;
    s_global = nullptr;
}

bool CnApp::event(CnEvent *event)
{
    switch (event->type())
    {
    case Event::LogConsole:
    {
        EventLogConsole *c = static_cast<EventLogConsole*>(event);
        logConsoleInner(c->category, c->message);
        break;
    }
    case Event::PortAdd:
    {
        EventPortAdd *c = static_cast<EventPortAdd*>(event);
        CnPortThread *thread = static_cast<CnPortThread*>(c->port->thread());
        thread->start();
        break;
    }
    case Event::PortRemove:
    {
        EventPortRemove *c = static_cast<EventPortRemove*>(event);
        CnPortThread *thread = static_cast<CnPortThread*>(c->port->thread());
        if ((m_threadsToStop.find(c->port) == m_threadsToStop.end()))
        {
            thread->stop();
            m_threadsToStop[c->port] = thread;
        }
        break;
    }
    }
    return false;
}

Cn::Config CnApp::config() const
{
    return m_cfg->config();
}

void CnApp::setConfig(const Cn::Config &config)
{
    CnCriticalSectionLocker _(&m_cs);
    setInnerConfig(config);
}

void CnApp::logConsole(Cn::LogFlag category, const CnString &message)
{
    if (global()->thread() == CnThread::currentThread())
        global()->logConsoleInner(category, message);
    else
    {
        Cn::postEvent(global(), new EventLogConsole(category, message));
    }
}

void CnApp::commandReloadConfig()
{
    m_cmdReload = true;
}

int CnApp::exec(int argc, char *argv[])
{
    m_loggers.manager->setName(m_name);
    parseArgs(argc, argv);
    beginToStart();
    Cn::StatusCode status = serviceStart();
    if (status == Cn::Status_BadNoService)
    {
        return run();
    }
    if (Cn::StatusIsBad(status))
        return 1;
    return 0;
}

int CnApp::run()
{
    auto r1 = std::signal(SIGINT , CnApp::signal_handler);
    if (r1 == SIG_ERR)
        CN_LOG_Warning(CnSTR("Can't set signal handler for SIGINT. Error: %s"), Cn::getLastErrorText().data());
    auto r2 = std::signal(SIGTERM, CnApp::signal_handler);
    if (r2 == SIG_ERR)
        CN_LOG_Warning(CnSTR("Can't set signal handler for SIGTERM. Error: %s"), Cn::getLastErrorText().data());

    m_ctrlRun = true;
    CnStd::cout << m_name << CnSTR(" starting ...") << std::endl;

    CnEventLoop loop;
    CnCfgProject *cfg = loadConfig();
    if (!cfg)
    {
        return 1;
    }

    initialize(cfg);
    while (m_ctrlRun)
    {
        loop.processEvents();
        processStopThreads();
		if (m_cmdReload)
		{
			reloadConfig();
			m_cmdReload = false;
		}
        Cn::msleep(10);
    }

    beginToFinalize();
    for (auto it = m_project.ports.begin(); it != m_project.ports.end(); ++it)
    {
        CnPort *port = *it;
        m_threadsToStop[port] = static_cast<CnPortThread*>(port->thread());
    }
    while (m_threadsToStop.size())
    {
        processStopThreads();
        Cn::msleep(1);
    }
    finalize();
    CnStd::cout << m_name << CnSTR(" stopped") << std::endl;
    return 0;
}

void CnApp::beginToStart()
{
    auto sLogDir = getLogDir();
    if (CnFileInfo::isAbsolutePath(sLogDir))
    {
        if (m_logDir.path() != sLogDir)
            m_logDir.setPath(sLogDir);
        if (!m_logDir.exists())
        {
            while (!m_logDir.exists() && m_logDir.cdUp())
                ;
            if (m_logDir.exists())
            {
                auto relPath = m_logDir.relativeFilePath(sLogDir);
                m_logDir.mkpath(relPath);
                m_logDir.cd(relPath);
            }
        }
    }
    else
    {
        m_logDir.mkpath(sLogDir);
        m_logDir.cd(sLogDir);
    }
}

void CnApp::parseArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        char *opt = argv[i];
        if (!std::strcmp(opt, "--version") || !std::strcmp(opt, "-v"))
        {
            printVersion();
            exit(0);
        }
        if (!std::strcmp(opt, "--help") || !std::strcmp(opt, "-h") || !std::strcmp(opt, "-?"))
        {
            printHelp();
            exit(0);
        }
        if (!std::strcmp(opt, "--service-name") || !std::strcmp(opt, "-s"))
        {
            if (++i >= argc)
            {
                printHelp();
                std::exit(1);
            }
            m_options.serviceName = Cn::toString(argv[i]);
            continue;
        }
        if (!std::strcmp(opt, "--logdir"))
        {
            if (++i >= argc)
            {
                printHelp();
                std::exit(1);
            }
            m_options.logdir = Cn::toString(argv[i]);
            continue;
        }
        if (!std::strcmp(opt, "--file") || !std::strcmp(opt, "-f"))
        {
            if (++i >= argc)
            {
                printHelp();
                std::exit(1);
            }
            m_options.file = Cn::toString(argv[i]);
            continue;
        }
        if (!parseArg(argc, argv, i))
        {
            printHelp();
            std::exit(1);
        }        
    }
}

bool CnApp::parseArg(int argc, char *argv[], int &i)
{
    return false;
}

void CnApp::printVersion()
{
    CnStd::cout << m_name << CnSTR("   version: ") << CnSTR(CN_VERSION_STR) << std::endl;
    CnStd::cout << CnSTR("ModbusLib version: ") << CnSTR(MODBUSLIB_VERSION_STR) << std::endl;
}

void CnApp::printHelp()
{
    CnStd::cout << CnSTR("usage: ") << m_name << CnSTR(" [options]") << CnSTR("\n");
    CnStd::cout << CnSTR("options:\n")
                   CnSTR("  -v, --version             show version\n")
                   CnSTR("  -h, -?, --help            show this help\n")
                   CnSTR("  -s, --service-name <name> service name, default: ") << m_name.data() << CnSTR("\n") <<
                   CnSTR("      --logdir <dir>        directory for log files, default: ") << m_defaultLogDirPath << CnSTR("\n") <<
                   CnSTR("  -f, --file <file>         configuration file name, default: ") << m_defaultFileConf << CnSTR("\n");
}

void CnApp::processStopThreads()
{
    for (auto it = m_threadsToStop.begin(); it != m_threadsToStop.end(); )
    {
        CnPortThread *thread = it->second;
        thread->stop();
        if (thread->isRunning())
            ++it;
        else
        {
            it = m_threadsToStop.erase(it);
            delete thread;
        }
    }
}

void CnApp::initialize(CnCfgProject *project)
{
    ProjectDiff diff;
    diff.config = project->config();
    diff.groupConfig = project->groupConfig();
    diff.toAdd.ports = project->ports();
    diff.toAdd.devices = project->devices();
    CnCriticalSectionLocker _(&m_cs);
    m_project.cfg = project;
    this->applyProjectDiff(diff);
}

void CnApp::beginToFinalize()
{
}

void CnApp::finalize()
{
    while (m_project.devices.size())
        this->deviceRemove(m_project.devices.front());
    m_project.devices.clear();
    m_project.hashDevices.clear();

    while (m_project.ports.size())
        this->portRemove(m_project.ports.front());
    m_project.ports.clear();
    m_project.hashPorts.clear();
    
    //for (CnLogger *logger : m_loggers.list)
    //    delete logger;
    m_loggers.cache.clear();
}

CnString CnApp::getServiceName() const
{
    if (m_options.serviceName.size())
        return m_options.serviceName;
    return m_name;
}

CnString CnApp::getLogDir() const
{
    if (m_options.logdir.size())
        return m_options.logdir;
    return m_defaultLogDirPath;
}

CnCfgProject *CnApp::loadConfig()
{
    CnCfgBuilder builder;
    CnString *fileptr;
    if (m_options.file.size())
        fileptr = &m_options.file;
    else
        fileptr = &m_defaultFileConf;
    CN_LOG_Info(CnSTR("Loading config file: %s"), fileptr->data());   
    CnCfgProject *cfg = builder.load(*fileptr);
    if (!cfg)
        CN_LOG_Error(CnSTR("Can't load config file '%s': %s"), fileptr->data(), builder.errorString().data());

    return cfg;
}

void CnApp::reloadConfig()
{
    CnCfgProject *cfg = loadConfig();
    if (!cfg)
        return;
    CnCriticalSectionLocker _(&m_cs);
    ProjectDiff diff = diffProject(cfg);
    CnCfgProject *old = m_project.cfg;
    m_project.cfg = cfg;
    this->applyProjectDiff(diff);
    delete old;
}

CnApp::ProjectDiff CnApp::diffProject(CnCfgProject *newProject)
{
    ProjectDiff diff;
    diff.config = newProject->config();
    diff.groupConfig = newProject->groupConfig();
    for (const CnCfgPortPtr& newPort : newProject->ports())
    {
        CnCfgPortPtr oldPort = m_project.cfg->port(newPort->name());
        if (oldPort)
        {
            diff.toUpdate.ports.push_back(newPort);
        }
        else
            diff.toAdd.ports.push_back(newPort);
    }
    for (const CnCfgPortPtr& oldPort : m_project.cfg->ports())
    {
        CnCfgPortPtr newPort = newProject->port(oldPort->name());
        if (!newPort)
            diff.toRemove.ports.push_back(oldPort);
    }

    for (const CnCfgDevicePtr& newDevice : newProject->devices())
    {
        CnCfgDevicePtr oldDevice = m_project.cfg->device(newDevice->name());
        // Note: if device with old name exists and it relates to same port, then update it, otherwise add new device
        if (oldDevice && (newDevice->portName() == oldDevice->portName()))
        {
            diff.toUpdate.devices.push_back(newDevice);
        }
        else
            diff.toAdd.devices.push_back(newDevice);
    }
    for (const CnCfgDevicePtr& oldDevice : m_project.cfg->devices())
    {
        CnCfgDevicePtr newDevice = newProject->device(oldDevice->name());
        // Note: if device with new name don't exists or it relates to another port, then remove old device
        if (!newDevice || (newDevice->portName() != oldDevice->portName()))
            diff.toRemove.devices.push_back(oldDevice);
    }

    return diff;
}

void CnApp::applyProjectDiff(const ProjectDiff &diff)
{
    if (m_loggers.console)
        m_loggers.console->resetConfig();
    m_loggers.oldCache = m_loggers.cache;
    for (auto it : m_loggers.oldCache)
        it.second->resetConfig();
    m_loggers.cache.clear();

    this->setInnerConfig(diff.config);
    this->setGroupConfig(diff.groupConfig);    

    for (const CnCfgPortPtr &cfg : diff.toRemove.ports)
    {
        CnPort *port = this->innerCnPort(cfg->name());
        if (port)
        {
            for (CnDevice *device : port->devices())
            {
                port->removeDevice(device);
                this->deviceRemove(device);
            }
            Cn::postEvent(this, new EventPortRemove(port));
            this->portRemove(port);
        }
    }

    for (const CnCfgDevicePtr &cfg : diff.toRemove.devices)
    {
        CnDevice *device = this->innerCnDevice(cfg->name());
        if (device)
        {
            CnPort *port = device->parentPort();
            port->removeDevice(device);
            this->deviceRemove(device);
        }
    }

    for (const CnCfgPortPtr &cfg : diff.toAdd.ports)
    {
        CnPort *port = createPort(cfg);
        if (port)
        {
            fillPort(port, cfg);
            this->portAdd(port);
            Cn::postEvent(this, new EventPortAdd(port));
        }
    }

    for (const CnCfgDevicePtr &cfg : diff.toAdd.devices)
    {
        CnPort *port = this->innerCnPort(cfg->portName());
        if (port)
        {
            CnDevice *device = createDevice(cfg);
            if (device)
            {
                fillDevice(device, cfg);
                this->deviceAdd(device);
                port->addDevice(device);
            }
        }
    }

    for (const CnCfgPortPtr &cfg : diff.toUpdate.ports)
    {
        CnPort *port = this->innerCnPort(cfg->name());
        if (port)
            fillPort(port, cfg);
    }

    for (const CnCfgDevicePtr &cfg : diff.toUpdate.devices)
    {
        CnDevice *device = this->innerCnDevice(cfg->name());
        if (device)
            fillDevice(device, cfg);
    }

    m_loggers.oldCache.clear();
}

void CnApp::setGroupConfig(const Cn::GroupConfig &groupConfig)
{
}

void CnApp::portAdd(CnPort *port)
{
    port->IncrementRef();
    CnPortThread *thread = new CnPortThread(port);
    m_project.ports.push_back(port);
    m_project.hashPorts[port->CfgName()] = port;
}

void CnApp::portRemove(CnPort *port)
{
    m_project.ports.remove(port);
    m_project.hashPorts.erase(port->CfgName());
    port->DecrementRef();
}

void CnApp::deviceAdd(CnDevice *device)
{
    device->IncrementRef();
    m_project.devices.push_back(device);
    m_project.hashDevices[device->name()] = device;
}

void CnApp::deviceRemove(CnDevice *device)
{
    m_project.devices.remove(device);
    m_project.hashDevices.erase(device->name());
    device->DecrementRef();
}

CnPort *CnApp::innerCnPort(const CnString &name) const
{
    auto it = m_project.hashPorts.find(name);
    if (it != m_project.hashPorts.end())
        return it->second;
    return nullptr;
}

void CnApp::fillPort(CnPort *port, const CnCfgPortPtr& cfg)
{
    const Cn::Config &config = cfg->config();
    port->logger()->setName(cfg->name());
    fillLogger(port->logger(), config);
    port->setCfgPort(cfg);
}

CnDevice *CnApp::innerCnDevice(const CnString &name) const
{
    auto it = m_project.hashDevices.find(name);
    if (it != m_project.hashDevices.end())
        return it->second;
    return nullptr;
}

void CnApp::fillDevice(CnDevice *device, const CnCfgDevicePtr &cfg)
{
    const Cn::Config &config = cfg->config();
    device->logger()->setName(cfg->name());
    fillLogger(device->logger(), config);
    device->setCfgDevice(cfg);
}

Cn::LogFlags CnApp::logFlags(const Cn::Config &config) const
{
    Cn::LogFlags res;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config::const_iterator it;
    it = config.find(s.LogFlags); if (it != config.end()) res = Cn::toLogFlags(it->second.toString()); else res = m_cfg->innerLogFlags(); 
    if (res == 0)
        res = m_cfg->innerLogFlags();
    return res;
}

CnStringList CnApp::logOutputs(const Cn::Config &config) const
{
    CnStringList res;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config::const_iterator it;
    it = config.find(s.LogOutput); 
    if (it != config.end())
        res = Cn::toStringList(it->second.toString());
    return res;
}

Cn::Config CnApp::loggerFileParams(const Cn::Config &config) const
{
    Cn::Config res;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    const CnLoggerFile::Strings &sLog = CnLoggerFile::Strings::instance();

    Cn::Config::const_iterator it;
    it = config.find(s.LogFileFormat    ); if (it != config.end()) res[sLog.cfg_format      ] = (it->second);
    it = config.find(s.LogFileTimeformat); if (it != config.end()) res[sLog.cfg_timeformat  ] = (it->second);
  //it = config.find(s.LogFilePath      ); if (it != config.end()) res[sLog.cfg_filePath    ] = (it->second); 
    it = config.find(s.LogFileMaxCount  ); if (it != config.end()) res[sLog.cfg_fileMaxCount] = (it->second);
    it = config.find(s.LogFileMaxSize   ); if (it != config.end()) res[sLog.cfg_fileMaxSize ] = (it->second);

    it = config.find(s.LogFilePath);
    if (it != config.end())
        res[sLog.cfg_filePath] = logDir().absoluteFilePath(it->second.toString());
    else
        res[sLog.cfg_filePath] = logDir().absoluteFilePath(m_cfg->innerLogFilePath());

    res[sLog.cfg_defaultFormat] = innerLogFileFormat();
    res[sLog.cfg_defaultTimeformat] = innerLogFileTimeformat();
    return res;
}

CnLoggerPtr CnApp::getOrCreateLogger(const CnString &type, const Cn::Config &params)
{
    const Strings &s = Strings::instance();
    CnLoggerPtr logger;
    if (type == s.system)
    {
        logger = m_loggers.system;
    }
    else if (type == s.console)
    {
        logger = m_loggers.console;
    }
    else if (type == s.file)
    {
        const CnLoggerFile::Strings &sLog = CnLoggerFile::Strings::instance();
        auto itname = params.find(sLog.cfg_filePath);
        if (itname == params.end())
            return nullptr;
        CnString file = itname ->second.toString();
        auto it = m_loggers.cache.find(file);
        if (it != m_loggers.cache.end())
            logger = it->second;
        else
        {
            auto itOld = m_loggers.oldCache.find(file);
            if (itOld != m_loggers.oldCache.end())
            {
                logger = itOld->second;
                m_loggers.oldCache.erase(itOld);
            }
            else
            {
                logger = new CnLoggerFile();
            }
            m_loggers.cache[file] = logger;
        }
        logger->setConfig(params);
    }
    return logger;
}

void CnApp::fillLogger(CnLoggerManager *logger, const Cn::Config &config)
{
    logger->setLogFlags(logFlags(config));
    Cn::Config params = loggerFileParams(config);
    CnList<CnLoggerPtr> loggers;
    CnStringList outputs = logOutputs(config);
    if (outputs.empty())
        outputs = innerLogOutputs();
    for (const CnString &output : outputs)
    {
        CnLoggerPtr logger = getOrCreateLogger(output, params);
        if (logger)
            loggers.push_back(logger);
    }
    logger->setLoggers(loggers);

    //if (loggers.empty())
    //{
    //    logger->setLoggers(m_loggers.manager->loggers());
    //}
    //else
    //{
    //    logger->setLoggers(loggers);
    //}
}

CnStringList CnApp::innerLogOutputs() const
{
    return Cn::toStringList(m_cfg->innerLogOutput());
}

CnString CnApp::innerLogSystemFormat() const
{ 
    CnString res = m_cfg->innerLogSystemFormat();
    if (res.empty()) 
        return m_cfg->innerLogDefaultFormat();
    return res; 
}

CnString CnApp::innerLogSystemTimeformat() const
{ 
    CnString res = m_cfg->innerLogSystemTimeformat();
    if (res.empty()) 
        return m_cfg->innerLogDefaultTimeformat();
    return res; 
}

CnString CnApp::innerLogConsoleFormat() const
{ 
    CnString res = m_cfg->innerLogConsoleFormat();
    if (res.empty()) 
        return m_cfg->innerLogDefaultFormat();
    return res; 
}

CnString CnApp::innerLogConsoleTimeformat() const
{ 
    CnString res = m_cfg->innerLogConsoleTimeformat();
    if (res.empty()) 
        return m_cfg->innerLogDefaultTimeformat();
    return res; 
}

CnString CnApp::innerLogFileFormat() const
{
    CnString res = m_cfg->innerLogFileFormat();
    if (res.empty()) 
        return m_cfg->innerLogDefaultFormat();
    return res; 
}

CnString CnApp::innerLogFileTimeformat() const
{
    CnString res = m_cfg->innerLogConsoleFormat();
    if (res.empty()) 
        return m_cfg->innerLogDefaultTimeformat();
    return res; 
}

void CnApp::setInnerConfig(const Cn::Config &config)
{
    const CnAppConfig::Strings& sKeys = CnAppConfig::Strings::instance();
    bool ok;
    { auto v = CnAppConfig::getInnerLogFlags            (config, &ok); if (ok) this ->setInnerLogFlags            (v); }
  //{ auto v = CnAppConfig::getInnerLogOutput           (config, &ok); if (ok) m_cfg->setInnerLogOutput           (v); }
    { auto v = CnAppConfig::getInnerLogDefaultFormat    (config, &ok); if (ok) m_cfg->setInnerLogDefaultFormat    (v); }
    { auto v = CnAppConfig::getInnerLogDefaultTimeformat(config, &ok); if (ok) m_cfg->setInnerLogDefaultTimeformat(v); }
    { auto v = CnAppConfig::getInnerLogSystemFormat     (config, &ok); if (ok) m_cfg->setInnerLogSystemFormat     (v); }
    { auto v = CnAppConfig::getInnerLogSystemTimeformat (config, &ok); if (ok) m_cfg->setInnerLogSystemTimeformat (v); }
    { auto v = CnAppConfig::getInnerLogConsoleFormat    (config, &ok); if (ok) m_cfg->setInnerLogConsoleFormat    (v); }
    { auto v = CnAppConfig::getInnerLogConsoleTimeformat(config, &ok); if (ok) m_cfg->setInnerLogConsoleTimeformat(v); }
    { auto v = CnAppConfig::getInnerLogFileFormat       (config, &ok); if (ok) m_cfg->setInnerLogFileFormat       (v); }
    { auto v = CnAppConfig::getInnerLogFileTimeformat   (config, &ok); if (ok) m_cfg->setInnerLogFileTimeformat   (v); }
    { auto v = CnAppConfig::getInnerLogFilePath         (config, &ok); if (ok) m_cfg->setInnerLogFilePath         (v); }
    { auto v = CnAppConfig::getInnerLogFileMaxCount     (config, &ok); if (ok) m_cfg->setInnerLogFileMaxCount     (v); }
    { auto v = CnAppConfig::getInnerLogFileMaxSize      (config, &ok); if (ok) m_cfg->setInnerLogFileMaxSize      (v); }

    const CnLogger::Strings& sLog = CnLogger::Strings::instance();

    if (m_loggers.system)
    {
        Cn::Config params;
        params[sLog.cfg_format    ] = innerLogSystemFormat    ();
        params[sLog.cfg_timeformat] = innerLogSystemTimeformat();
        m_loggers.system->setConfig(params);
    }

    if (m_loggers.console)
    {
        Cn::Config params;
        params[sLog.cfg_format    ] = innerLogConsoleFormat    ();
        params[sLog.cfg_timeformat] = innerLogConsoleTimeformat();
        m_loggers.console->setConfig(params);
    }

    {
        auto v = CnAppConfig::getInnerLogOutput(config, &ok); 
        if (ok) 
            this->setInnerLogOutput(v); 
    }
}

void CnApp::setInnerLogFlags(const CnString &sflags)
{
    Cn::LogFlags flags = Cn::toLogFlags(sflags);
    setInnerLogFlags(flags);
}

void CnApp::setInnerLogFlags(const Cn::LogFlags &flags)
{
    m_loggers.manager->setLogFlags(flags);
    m_cfg->setInnerLogFlags(flags);
}

void CnApp::setInnerLogOutput(const CnString &logOutput)
{
    this->setInnerLogOutputs(Cn::toStringList(logOutput));
}

void CnApp::setInnerLogOutputs(const CnStringList &outputs)
{
    //m_loggers.list.clear();
    const CnLoggerFile::Strings &sLog = CnLoggerFile::Strings::instance();
    Cn::Config params;
    params[sLog.cfg_format      ] = innerLogFileFormat();
    params[sLog.cfg_timeformat  ] = innerLogFileTimeformat();
    params[sLog.cfg_filePath    ] = logDir().absoluteFilePath(m_cfg->CfgLogFilePath());
    params[sLog.cfg_fileMaxCount] = m_cfg->CfgLogFileMaxCount();
    params[sLog.cfg_fileMaxSize ] = m_cfg->CfgLogFileMaxSize();
    CnList<CnLoggerPtr> loggers;
    CnStringList strlsLoggers;
            
    for (const CnString &output : outputs)
    {
        CnLoggerPtr logger = getOrCreateLogger(output, params);
        if (logger)
        {
            //m_loggers.list.push_back(logger);
            strlsLoggers.push_back(output);
            loggers.push_back(logger);
        }
    }
    //if (loggers.empty())
    //    loggers.push_back(m_loggers.console);
    m_loggers.manager->setLoggers(loggers);
    m_cfg->setInnerLogOutput(Cn::toString(strlsLoggers));

}

void CnApp::createLoggerService()
{
    const Strings &s = Strings::instance();
    setInnerLogOutputs({s.system});
}

void CnApp::createLoggerConsole()
{
    m_loggers.console = new CnLoggerConsole();
    m_loggers.console->setFormat(CnAppConfig::Defaults::instance().LogDefaultFormat);
    m_loggers.console->setTimeformat(CnAppConfig::Defaults::instance().LogDefaultTimeformat);

    const Strings &s = Strings::instance();
    setInnerLogFlags(Cn::LogFlags(Cn::Log_All)); 
    setInnerLogOutputs({s.console});
}

void CnApp::logConsoleInner(Cn::LogFlag category, const CnString &message)
{
    Cn::Color color = Cn::toColor(category);
    if (color == Cn::Color_Default)
        CnStd::cout << message << std::endl;
    else
    {
        Cn::setConsoleColor(color);
        CnStd::cout << message << std::endl;
        Cn::setConsoleColor(Cn::Color_Default);
    }
}
