/*!
 * \file   CnApp.h
 * \brief  
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNSYSTEM_H
#define CNSYSTEM_H

#include <CnTypes.h>
#include <CnObject.h>
#include <CnThread.h>
#include <CnDir.h>
#include <cfg/CnCfgProject.h>
#include <log/CnLogger.h>

class CnEvent;
class CnLoggerManager;
class CnPort;
class CnDevice;
class CnPortThread;
class CnCfgProject;
class CnCfgPort;
class CnCfgDevice;
class CnUaServer;
class CnAppThread;
class CnAppConfig;

/*! \brief Core application class for the gateway.

    \details `CnApp` orchestrates configuration loading, logging,
    ports/devices lifecycle, event dispatching, and application
    control flow (startup/shutdown). It provides global helpers
    for paths, logging, and access to configured components, and
    defines overridable hooks for derived applications.

    \sa `CnCfgProject`, `CnPort`, `CnDevice`, `CnLoggerManager`
*/
class CnApp : public CnObject
{
private:
    static CnApp *s_global;

public:
    struct Strings
    {
        const CnString system ; ///< 
        const CnString console; ///< 
        const CnString file   ; ///< 

        /// \details 
        Strings();

        /// \details 
        static const Strings& instance();
    };

public:
    struct Changes
    {
        CnList<CnCfgPortPtr> ports;
        CnList<CnCfgDevicePtr> devices;
    };

    struct ProjectDiff
    {
        Changes toAdd;
        Changes toRemove;
        Changes toUpdate;
        Cn::Config config;
        Cn::GroupConfig groupConfig;
    };
    
public:
    /// \details Returns the global application instance pointer.
    static CnApp *global() { return s_global; }
    
    /// \details Handles termination signals (e.g., SIGINT/SIGTERM).
    static void signal_handler(int signal);
    
    /// \details Handles configuration reload signal (e.g., SIGHUP).
    static void signal_reload(int signal);
    
    /// \details Returns the absolute path to the application binary.
    static CnString applicationFilePath();
    
    /// \details Returns the absolute path to the application directory.
    static CnString applicationDirPath();
    
    /// \details Returns the application directory as `CnDir`.
    inline static CnDir applicationDir() { return global()->m_applicationDir; }
    
    /// \details Returns the configuration directory as `CnDir`.
    inline static CnDir confDir() { return global()->m_confDir; }
    
    /// \details Returns the log directory as `CnDir`.
    inline static CnDir logDir() { return global()->m_logDir; }
    
    /// \details Converts a possibly relative `path` to an absolute path.
    static CnString absolutePath(const CnString &path);
    
    /// \details Returns the default configuration file name.
    inline static CnString defaultFileConf() { return global()->m_defaultFileConf; }
    
    /// \details Requests graceful application stop.
    inline static void cmdStop() { global()->m_ctrlRun = false; }

    /// \details Executes the 'Reload Configuration' command.
    inline static void cmdReloadConfig() { global()->commandReloadConfig(); }
    
    /// \details Logs a message to the console logger with category `flag`.
    static void logConsole(Cn::LogFlag flag, const CnString &message);
    
    /// \details Sends an event synchronously to `object`.
    void sendEvent(CnObject *object, CnEvent *event);
    
    /// \details Posts an event asynchronously to `object`.
    void postEvent(CnObject *object, CnEvent *event);
    
public:
    /// \details Constructs the application object.
    CnApp();
    /// \details Destroys the application object, releasing resources.
    virtual ~CnApp();

private:
    void initSpec();
    void finalSpec();

public:
    /// \details Main event handler; returns `true` if event was processed.
    bool event(CnEvent *event) override;

public:
    /// \details Returns the current application configuration object.
    inline CnAppConfig *cfg() const { return m_cfg; }
    
    /// \details Returns the application name.
    inline const CnString &name() const { return m_name; }
    
    /// \details Returns the application description.
    inline const CnString &description() const { return m_description; }

public:
    /// \details Returns enabled log outputs (system/console/file).
    CnStringList logOutputs() const { CnCriticalSectionLocker _(&m_cs); return innerLogOutputs(); }
    
    /// \details Returns the system log message format string.
    CnString logSystemFormat() const { CnCriticalSectionLocker _(&m_cs); return innerLogSystemFormat(); }
    
    /// \details Returns the system log time format string.
    CnString logSystemTimeformat() const { CnCriticalSectionLocker _(&m_cs); return innerLogSystemTimeformat(); }
    
    /// \details Returns the console log message format string.
    CnString logConsoleFormat() const { CnCriticalSectionLocker _(&m_cs); return innerLogConsoleFormat(); }
    
    /// \details Returns the console log time format string.
    CnString logConsoleTimeformat() const { CnCriticalSectionLocker _(&m_cs); return innerLogConsoleTimeformat(); }
    
    /// \details Returns the file log message format string.
    CnString logFileFormat() const { CnCriticalSectionLocker _(&m_cs); return innerLogFileFormat(); }
    
    /// \details Returns the file log time format string.
    CnString logFileTimeformat() const { CnCriticalSectionLocker _(&m_cs); return innerLogFileTimeformat(); }

public: // config
    /// \details Returns the current configuration snapshot.
    Cn::Config config() const;
    
    /// \details Applies a new configuration snapshot.
    void setConfig(const Cn::Config &config);

public: // logging interface
    /// \details Returns the logger manager instance.
    inline CnLoggerManager *logger() const { return m_loggers.manager; }

public: // port/device interface
    /// \details Finds a port by name; thread-safe lookup.
    inline CnPort *cnPort(const CnString &name) const { CnCriticalSectionLocker _(&m_cs); return innerCnPort(name); }
    
    /// \details Finds a device by name; thread-safe lookup.
    inline CnDevice *cnDevice(const CnString &name) const { CnCriticalSectionLocker _(&m_cs); return innerCnDevice(name); }

public: // commands
    /// \details Reloads configuration and applies differences.
    void commandReloadConfig();

public:
    /// \details Parses arguments and starts the application loop.
    int exec(int argc, char *argv[]);

    /// \details Runs the application until stopped; non-arg variant.
    int run();

protected:
    void beginToStart();
    Cn::StatusCode serviceStart();

protected:
    void parseArgs(int argc, char *argv[]);
    virtual bool parseArg(int argc, char *argv[], int &i);
    virtual void printVersion();
    virtual void printHelp();
    void processStopThreads();

protected:
    virtual void initialize(CnCfgProject *project);
    virtual void beginToFinalize();
    virtual void finalize();

protected:
    CnString getServiceName() const;
    CnString getLogDir() const;

protected:
    CnCfgProject *loadConfig();
    void reloadConfig();
    ProjectDiff diffProject(CnCfgProject *newProject);
    void applyProjectDiff(const ProjectDiff &diff);
    virtual void setGroupConfig(const Cn::GroupConfig &groupConfig);

protected:
    void portAdd(CnPort *port);
    void portRemove(CnPort *port);

protected:
    void deviceAdd(CnDevice *device);
    void deviceRemove(CnDevice *device);

protected: // port interface
    CnPort *innerCnPort(const CnString &name) const;
    virtual CnPort *createPort(const CnCfgPort *cfg) = 0;
    virtual void fillPort(CnPort *port, const CnCfgPortPtr& cfg);

protected: // device interface
    CnDevice *innerCnDevice(const CnString &name) const;
    virtual CnDevice *createDevice(const CnCfgDevice *cfg) = 0;
    virtual void fillDevice(CnDevice *device, const CnCfgDevicePtr& cfg);

protected:
    Cn::LogFlags logFlags(const Cn::Config &config) const;
    CnStringList logOutputs(const Cn::Config &config) const;
    Cn::Config loggerFileParams(const Cn::Config &config) const;
    CnLoggerPtr getOrCreateLogger(const CnString &type, const Cn::Config &params);
    void fillLogger(CnLoggerManager *logger, const Cn::Config &config);
    
protected: // inner config
    CnStringList innerLogOutputs() const;
    CnString innerLogSystemFormat() const;
    CnString innerLogSystemTimeformat() const;
    CnString innerLogConsoleFormat() const;
    CnString innerLogConsoleTimeformat() const;
    CnString innerLogFileFormat() const;
    CnString innerLogFileTimeformat() const;
    
    void setInnerConfig(const Cn::Config &config);
    void setInnerLogFlags(const CnString &flags);
    void setInnerLogFlags(const Cn::LogFlags &flags);
    void setInnerLogOutput(const CnString& innerLogOutput);
    void setInnerLogOutputs(const CnStringList& outputs);

protected: // inner console output
    void createLoggerService();
    void createLoggerConsole();
    void logConsoleInner(Cn::LogFlag flag, const CnString &message);

protected:
    volatile bool m_ctrlRun;
    volatile bool m_cmdReload;

    mutable CnCriticalSection m_cs;
    CnString m_name;
    CnString m_description;
    CnDir m_applicationDir;
    CnDir m_confDir;
    CnDir m_logDir;
    CnAppConfig *m_cfg;

    struct 
    {
        CnString file  ;
        CnString logdir;
        CnString serviceName;
    } m_options;
    CnString m_defaultFileConf;
    CnString m_defaultLogDirPath;
    
    struct
    {
        CnLoggerPtr system;
        CnLoggerPtr console;
        CnHash<CnString, CnLoggerPtr> cache;
        CnHash<CnString, CnLoggerPtr> oldCache;
        CnLoggerManager *manager;
    } m_loggers;

    struct
    {
        CnCfgProject *cfg;
        CnList<CnPort*> ports;
        CnHash<CnString, CnPort*> hashPorts;
        CnList<CnDevice*> devices;
        CnHash<CnString, CnDevice*> hashDevices;
    } m_project;


    CnHash<CnPort*, CnPortThread*> m_threadsToStop;

protected:
    class Event : public CnEvent
    {
    public:
        enum Type
        {
            LogConsole  ,
            PortAdd     ,
            PortRemove  
        };
    };

    class EventLogConsole : public Event
    {
    public:
        EventLogConsole(Cn::LogFlag flag, const CnString &message) : category(flag), message(message) {}
        int type() const override { return LogConsole; }
        Cn::LogFlag category;
        CnString message;
    };

    struct EventPortAdd : public Event
    {
    public:
        EventPortAdd(CnPort *port) : port(port) {}
        int type() const override { return PortAdd; }
        CnPort *port;
    };

    struct EventPortRemove : public Event
    {
    public:
        EventPortRemove(CnPort *port) : port(port) {}
        int type() const override { return PortRemove; }
        CnPort *port;
    };
};

#endif // CNSYSTEM_H
