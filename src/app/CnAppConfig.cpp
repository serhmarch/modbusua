#include "CnAppConfig.h"

#include <log/CnLoggerFile.h>

extern const CnChar *os_log_file_dir;

CnAppConfig::Strings::Strings() :
    LogFlags            (CnSTR("Log.flags")),
    LogOutput           (CnSTR("Log.output")),
    LogDefaultFormat    (CnSTR("Log.default.format")),
    LogDefaultTimeformat(CnSTR("Log.default.timeformat")),
    LogSystemFormat     (CnSTR("Log.system.format")),
    LogSystemTimeformat (CnSTR("Log.system.timeformat")),
    LogConsoleFormat    (CnSTR("Log.concole.format")),
    LogConsoleTimeformat(CnSTR("Log.console.timeformat")),
    LogFileFormat       (CnSTR("Log.file.format")),
    LogFileTimeformat   (CnSTR("Log.file.timeformat")),
    LogFilePath         (CnSTR("Log.file.path")),
    LogFileMaxCount     (CnSTR("Log.file.maxcount")),
    LogFileMaxSize      (CnSTR("Log.file.maxsize"))
{
}

const CnAppConfig::Strings &CnAppConfig::Strings::instance()
{
    static const Strings s;
    return s;
}

CnAppConfig::Defaults::Defaults() :
    LogFlags            (CnSTR("Error|Warning|Info|Trace|TraceDetails|CtorDtor|EntryExit|ThreadStartStop|Connection|Item|Message|Protocol|Receive|Send")),
    LogOutput           (CnString()),
    LogDefaultFormat    (CnLogger::Defaults::instance().format),
    LogDefaultTimeformat(CnLogger::Defaults::instance().timeformat),
    LogSystemFormat     (CnString()),
    LogSystemTimeformat (CnString()),
    LogConsoleFormat    (CnString()),
    LogConsoleTimeformat(CnString()),
    LogFileFormat       (CnString()),
    LogFileTimeformat   (CnString()),
    LogFilePath         (CnSTR(CN_APP_NAME".log")),
    LogFileMaxCount     (CNLOGFILE_MAXFILES),
    LogFileMaxSize      (CNLOGFILE_MAXSIZE)
{
}

const CnAppConfig::Defaults &CnAppConfig::Defaults::instance()
{
    static const Defaults d;
    return d;
}


CnString CnAppConfig::getCfgLogFlags            (const Cn::Config &config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFlags            , toString) }
CnString CnAppConfig::getCfgLogOutput           (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogOutput           , toString) }
CnString CnAppConfig::getCfgLogDefaultFormat    (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogDefaultFormat    , toString) }
CnString CnAppConfig::getCfgLogDefaultTimeformat(const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogDefaultTimeformat, toString) }
CnString CnAppConfig::getCfgLogSystemFormat     (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogSystemFormat     , toString) }
CnString CnAppConfig::getCfgLogSystemTimeformat (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogSystemTimeformat , toString) }
CnString CnAppConfig::getCfgLogConsoleFormat    (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogConsoleFormat    , toString) }
CnString CnAppConfig::getCfgLogConsoleTimeformat(const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogConsoleTimeformat, toString) }
CnString CnAppConfig::getCfgLogFileFormat       (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileFormat       , toString) }
CnString CnAppConfig::getCfgLogFileTimeformat   (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileTimeformat   , toString) }
CnString CnAppConfig::getCfgLogFilePath         (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFilePath         , toString) }
int      CnAppConfig::getCfgLogFileMaxCount     (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileMaxCount     , toInt   ) }
int      CnAppConfig::getCfgLogFileMaxSize      (const Cn::Config& config, bool *ok)
{ 
    auto it = config.find(Strings::instance().LogFileMaxSize);
    if (it == config.end())
    {
        if (ok)
            *ok = false;
        return 0;
    }
    return static_cast<int>(CnLoggerFile::toMaxSize(it->second, ok));
}

CnAppConfig::CnAppConfig()
{
    const Defaults &d = Defaults::instance();

    LogFlags             = toInnerLogFlags            (d.LogFlags            );
    LogOutput            = toInnerLogOutput           (d.LogOutput           );
    LogDefaultFormat     = toInnerLogDefaultFormat    (d.LogDefaultFormat    );
    LogDefaultTimeformat = toInnerLogDefaultTimeformat(d.LogDefaultTimeformat);
    LogSystemFormat      = toInnerLogConsoleFormat    (d.LogSystemFormat     );
    LogSystemTimeformat  = toInnerLogConsoleTimeformat(d.LogSystemTimeformat );
    LogConsoleFormat     = toInnerLogConsoleFormat    (d.LogConsoleFormat    );
    LogConsoleTimeformat = toInnerLogConsoleTimeformat(d.LogConsoleTimeformat);
    LogFileFormat        = toInnerLogFileFormat       (d.LogFileFormat       );
    LogFileTimeformat    = toInnerLogFileTimeformat   (d.LogFileTimeformat   );
    LogFilePath          = toInnerLogFilePath         (d.LogFilePath         );
    LogFileMaxCount      = toInnerLogFileMaxCount     (d.LogFileMaxCount     );
    LogFileMaxSize       = toInnerLogFileMaxSize      (d.LogFileMaxSize      );
}

Cn::Config CnAppConfig::config() const
{
    const Strings& sKeys = Strings::instance();
    Cn::Config config;

    CnCriticalSectionLocker _(&cs);
    config[sKeys.LogFlags            ] = toCfgLogFlags            (LogFlags            );
    config[sKeys.LogOutput           ] = toCfgLogOutput           (LogOutput           );
    config[sKeys.LogDefaultFormat    ] = toCfgLogDefaultFormat    (LogDefaultFormat    );
    config[sKeys.LogDefaultTimeformat] = toCfgLogDefaultTimeformat(LogDefaultTimeformat);
    config[sKeys.LogSystemFormat     ] = toCfgLogSystemFormat     (LogSystemFormat     );
    config[sKeys.LogSystemTimeformat ] = toCfgLogSystemTimeformat (LogSystemTimeformat );
    config[sKeys.LogConsoleFormat    ] = toCfgLogConsoleFormat    (LogConsoleFormat    );
    config[sKeys.LogConsoleTimeformat] = toCfgLogConsoleTimeformat(LogConsoleTimeformat);
    config[sKeys.LogFileFormat       ] = toCfgLogFileFormat       (LogFileFormat       );
    config[sKeys.LogFileTimeformat   ] = toCfgLogFileTimeformat   (LogFileTimeformat   );
    config[sKeys.LogFilePath         ] = toCfgLogFilePath         (LogFilePath         );
    config[sKeys.LogFileMaxCount     ] = toCfgLogFileMaxCount     (LogFileMaxCount     );
    config[sKeys.LogFileMaxSize      ] = toCfgLogFileMaxSize      (LogFileMaxSize      );
    
    return config; 
}

void CnAppConfig::setConfig(const Cn::Config &config)
{
    const Strings& sKeys = Strings::instance();
    bool ok;
    CnCriticalSectionLocker _(&cs);
    { auto v = getInnerLogFlags            (config, &ok); if (ok) LogFlags             = v; }
    { auto v = getInnerLogOutput           (config, &ok); if (ok) LogOutput            = v; }
    { auto v = getInnerLogDefaultFormat    (config, &ok); if (ok) LogDefaultFormat     = v; }
    { auto v = getInnerLogDefaultTimeformat(config, &ok); if (ok) LogDefaultTimeformat = v; }
    { auto v = getInnerLogSystemFormat     (config, &ok); if (ok) LogSystemFormat      = v; }
    { auto v = getInnerLogSystemTimeformat (config, &ok); if (ok) LogSystemTimeformat  = v; }
    { auto v = getInnerLogConsoleFormat    (config, &ok); if (ok) LogConsoleFormat     = v; }
    { auto v = getInnerLogConsoleTimeformat(config, &ok); if (ok) LogConsoleTimeformat = v; }
    { auto v = getInnerLogFileFormat       (config, &ok); if (ok) LogFileFormat        = v; }
    { auto v = getInnerLogFileTimeformat   (config, &ok); if (ok) LogFileTimeformat    = v; }
    { auto v = getInnerLogFilePath         (config, &ok); if (ok) LogFilePath          = v; }
    { auto v = getInnerLogFileMaxCount     (config, &ok); if (ok) LogFileMaxCount      = v; }
    { auto v = getInnerLogFileMaxSize      (config, &ok); if (ok) LogFileMaxSize       = v; }
}
