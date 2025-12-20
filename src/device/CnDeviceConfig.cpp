#include "CnDeviceConfig.h"

#include <app/CnAppConfig.h>
#include <log/CnLoggerFile.h>

CnDeviceConfig::Strings::Strings() :
    Name                     (CnSTR("Name")),
    EnableDevice             (CnSTR("EnableDevice")),
    ModbusUnit               (Cn::Strings::instance().Unit),
    RepeatCount              (Cn::Strings::instance().RepeatCount),
    RestoreTimeout           (CnSTR("RestoreTimeout")),
    MaxReadCoils             (CnSTR("MaxReadCoils")),
    MaxWriteMultipleCoils    (CnSTR("MaxWriteMultipleCoils")),
    MaxReadDiscreteInputs    (CnSTR("MaxReadDiscreteInputs")),
    MaxReadInputRegisters    (CnSTR("MaxReadInputRegisters")),
    MaxReadHoldingRegisters  (CnSTR("MaxReadHoldingRegisters")),
    MaxWriteMultipleRegisters(CnSTR("MaxWriteMultipleRegisters")),
    DefaultPeriod            (CnSTR("DefaultPeriod")),
    RequestTimeout           (CnSTR("RequestTimeout")),
    LogFlags                 (CnAppConfig::Strings::instance().LogFlags         ),
    LogOutput                (CnAppConfig::Strings::instance().LogOutput        ),
    LogFileFormat            (CnAppConfig::Strings::instance().LogFileFormat    ),
    LogFileTimeformat        (CnAppConfig::Strings::instance().LogFileTimeformat),
    LogFilePath              (CnAppConfig::Strings::instance().LogFilePath      ),
    LogFileMaxCount          (CnAppConfig::Strings::instance().LogFileMaxCount  ),
    LogFileMaxSize           (CnAppConfig::Strings::instance().LogFileMaxSize   )
{
}

const CnDeviceConfig::Strings &CnDeviceConfig::Strings::instance()
{
    static const Strings s;
    return s;
}

CnDeviceConfig::Defaults::Defaults() :
    Name                     (CnSTR("PLC")),
    EnableDevice             (true),
    ModbusUnit               (Modbus::VALID_MODBUS_ADDRESS_BEGIN),
    RepeatCount              (Cn::Defaults::instance().RepeatCount),
    RestoreTimeout           (10000),
    MaxReadCoils             (2000),
    MaxWriteMultipleCoils    (2000),
    MaxReadDiscreteInputs    (2000),
    MaxReadInputRegisters    (120),
    MaxReadHoldingRegisters  (120),
    MaxWriteMultipleRegisters(120),
    DefaultPeriod            (1000),
    RequestTimeout           (10000),
    LogFlags                 (CnAppConfig::Defaults::instance().LogFlags         ),
    LogOutput                (CnAppConfig::Defaults::instance().LogOutput        ),
    LogFileFormat            (CnAppConfig::Defaults::instance().LogFileFormat    ),
    LogFileTimeformat        (CnAppConfig::Defaults::instance().LogFileTimeformat),
    LogFilePath              (CnAppConfig::Defaults::instance().LogFilePath      ),
    LogFileMaxCount          (CnAppConfig::Defaults::instance().LogFileMaxCount  ),
    LogFileMaxSize           (CnAppConfig::Defaults::instance().LogFileMaxSize   )
{
}

const CnDeviceConfig::Defaults &CnDeviceConfig::Defaults::instance()
{
    static const Defaults d;
    return d;
}

CnString CnDeviceConfig::getCfgName                     (const Cn::Config &config, bool *ok) { CN_GET_CFGMAP_VALUE(Name                     , toString) }
bool     CnDeviceConfig::getCfgEnableDevice             (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(EnableDevice             , toBool  ) }
uint8_t  CnDeviceConfig::getCfgModbusUnit               (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(ModbusUnit               , toUInt8 ) }
uint32_t CnDeviceConfig::getCfgRepeatCount              (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(RepeatCount              , toUInt32 ) }
uint32_t CnDeviceConfig::getCfgRestoreTimeout           (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(RestoreTimeout           , toUInt32 ) }
uint16_t CnDeviceConfig::getCfgMaxReadCoils             (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(MaxReadCoils             , toUInt16) }
uint16_t CnDeviceConfig::getCfgMaxWriteMultipleCoils    (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(MaxWriteMultipleCoils    , toUInt16) }
uint16_t CnDeviceConfig::getCfgMaxReadDiscreteInputs    (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(MaxReadDiscreteInputs    , toUInt16) }
uint16_t CnDeviceConfig::getCfgMaxReadInputRegisters    (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(MaxReadInputRegisters    , toUInt16) }
uint16_t CnDeviceConfig::getCfgMaxReadHoldingRegisters  (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(MaxReadHoldingRegisters  , toUInt16) }
uint16_t CnDeviceConfig::getCfgMaxWriteMultipleRegisters(const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(MaxWriteMultipleRegisters, toUInt16) }
uint32_t CnDeviceConfig::getCfgDefaultPeriod            (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(DefaultPeriod            , toUInt32 ) }
uint32_t CnDeviceConfig::getCfgRequestTimeout           (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(RequestTimeout           , toUInt32 ) }
CnString CnDeviceConfig::getCfgLogFlags                 (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFlags                 , toString) }
CnString CnDeviceConfig::getCfgLogOutput                (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogOutput                , toString) }
CnString CnDeviceConfig::getCfgLogFileFormat            (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileFormat            , toString) }
CnString CnDeviceConfig::getCfgLogFileTimeformat        (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileTimeformat        , toString) }
CnString CnDeviceConfig::getCfgLogFilePath              (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFilePath              , toString) }
int      CnDeviceConfig::getCfgLogFileMaxCount          (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileMaxCount          , toInt   ) }
int      CnDeviceConfig::getCfgLogFileMaxSize           (const Cn::Config& config, bool *ok)
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

CnDeviceConfig::CnDeviceConfig()
{
    const Defaults &d = Defaults::instance();

    Name                              = toInnerName                              (d.Name                     );
    EnableDevice                      = toInnerEnableDevice                      (d.EnableDevice             );
    ModbusUnit                        = toInnerModbusUnit                        (d.ModbusUnit               );
    RepeatCount                       = toInnerRepeatCount                       (d.RepeatCount              );
    RestoreTimeoutMillisec            = toInnerRestoreTimeoutMillisec            (d.RestoreTimeout           );
    MaxReadCoils                      = toInnerMaxReadCoils                      (d.MaxReadCoils             );
    MaxWriteMultipleCoils             = toInnerMaxWriteMultipleCoils             (d.MaxWriteMultipleCoils    );
    MaxReadDiscreteInputs             = toInnerMaxReadDiscreteInputs             (d.MaxReadDiscreteInputs    );
    MaxReadInputRegisters             = toInnerMaxReadInputRegisters             (d.MaxReadInputRegisters    );
    MaxReadHoldingRegisters           = toInnerMaxReadHoldingRegisters           (d.MaxReadHoldingRegisters  );
    MaxWriteMultipleRegisters         = toInnerMaxWriteMultipleRegisters         (d.MaxWriteMultipleRegisters);
    DefaultPeriodMillisec             = toInnerDefaultPeriodMillisec             (d.DefaultPeriod            );
    RequestTimeoutMillisec            = toInnerRequestTimeoutMillisec            (d.RequestTimeout           );
    LogFlags                          = toInnerLogFlags                          (d.LogFlags                 );
    LogOutput                         = toInnerLogOutput                         (d.LogOutput                );
    LogFileFormat                     = toInnerLogFileFormat                     (d.LogFileFormat            );
    LogFileTimeformat                 = toInnerLogFileTimeformat                 (d.LogFileTimeformat        );
    LogFilePath                       = toInnerLogFilePath                       (d.LogFilePath              );
    LogFileMaxCount                   = toInnerLogFileMaxCount                   (d.LogFileMaxCount          );
    LogFileMaxSize                    = toInnerLogFileMaxSize                    (d.LogFileMaxSize           );
}

Cn::Config CnDeviceConfig::config() const
{
    const Strings& sKeys = Strings::instance();
    Cn::Config config;

    CnCriticalSectionLocker _(&cs);
    config[sKeys.Name                     ] = toCfgName                     (Name                             );
    config[sKeys.EnableDevice             ] = toCfgEnableDevice             (EnableDevice                     );
    config[sKeys.ModbusUnit               ] = toCfgModbusUnit               (ModbusUnit                       );
    config[sKeys.RepeatCount              ] = toCfgRepeatCount              (RepeatCount                      );
    config[sKeys.RestoreTimeout           ] = toCfgRestoreTimeout           (RestoreTimeoutMillisec           );
    config[sKeys.MaxReadCoils             ] = toCfgMaxReadCoils             (MaxReadCoils                     );
    config[sKeys.MaxWriteMultipleCoils    ] = toCfgMaxWriteMultipleCoils    (MaxWriteMultipleCoils            );
    config[sKeys.MaxReadDiscreteInputs    ] = toCfgMaxReadDiscreteInputs    (MaxReadDiscreteInputs            );
    config[sKeys.MaxReadInputRegisters    ] = toCfgMaxReadInputRegisters    (MaxReadInputRegisters            );
    config[sKeys.MaxReadHoldingRegisters  ] = toCfgMaxReadHoldingRegisters  (MaxReadHoldingRegisters          );
    config[sKeys.MaxWriteMultipleRegisters] = toCfgMaxWriteMultipleRegisters(MaxWriteMultipleRegisters        );
    config[sKeys.DefaultPeriod            ] = toCfgDefaultPeriod            (DefaultPeriodMillisec            );
    config[sKeys.RequestTimeout           ] = toCfgRequestTimeout           (RequestTimeoutMillisec           );
    config[sKeys.LogFlags                 ] = toCfgLogFlags                 (LogFlags                         );
    config[sKeys.LogOutput                ] = toCfgLogOutput                (LogOutput                        );
    config[sKeys.LogFileFormat            ] = toCfgLogFileFormat            (LogFileFormat                    );
    config[sKeys.LogFileTimeformat        ] = toCfgLogFileTimeformat        (LogFileTimeformat                );
    config[sKeys.LogFilePath              ] = toCfgLogFilePath              (LogFilePath                      );
    config[sKeys.LogFileMaxCount          ] = toCfgLogFileMaxCount          (LogFileMaxCount                  );
    config[sKeys.LogFileMaxSize           ] = toCfgLogFileMaxSize           (LogFileMaxSize                   );
    
    return config; 
}

void CnDeviceConfig::setConfig(const Cn::Config &config)
{
    const Strings& sKeys = Strings::instance();
    bool ok;
    CnCriticalSectionLocker _(&cs);
    { auto v = getInnerName                             (config, &ok); if (ok) Name                              = v; }
    { auto v = getInnerEnableDevice                     (config, &ok); if (ok) EnableDevice                      = v; }
    { auto v = getInnerModbusUnit                       (config, &ok); if (ok) ModbusUnit                        = v; }
    { auto v = getInnerRepeatCount                      (config, &ok); if (ok) RepeatCount                       = v; }
    { auto v = getInnerRestoreTimeoutMillisec           (config, &ok); if (ok) RestoreTimeoutMillisec            = v; }
    { auto v = getInnerMaxReadCoils                     (config, &ok); if (ok) MaxReadCoils                      = v; }
    { auto v = getInnerMaxWriteMultipleCoils            (config, &ok); if (ok) MaxWriteMultipleCoils             = v; }
    { auto v = getInnerMaxReadDiscreteInputs            (config, &ok); if (ok) MaxReadDiscreteInputs             = v; }
    { auto v = getInnerMaxReadInputRegisters            (config, &ok); if (ok) MaxReadInputRegisters             = v; }
    { auto v = getInnerMaxReadHoldingRegisters          (config, &ok); if (ok) MaxReadHoldingRegisters           = v; }
    { auto v = getInnerMaxWriteMultipleRegisters        (config, &ok); if (ok) MaxWriteMultipleRegisters         = v; }
    { auto v = getInnerDefaultPeriodMillisec            (config, &ok); if (ok) DefaultPeriodMillisec             = v; }
    { auto v = getInnerRequestTimeoutMillisec           (config, &ok); if (ok) RequestTimeoutMillisec            = v; }
    { auto v = getInnerLogFlags                         (config, &ok); if (ok) LogFlags                          = v; }
    { auto v = getInnerLogOutput                        (config, &ok); if (ok) LogOutput                         = v; }
    { auto v = getInnerLogFileFormat                    (config, &ok); if (ok) LogFileFormat                     = v; }
    { auto v = getInnerLogFileTimeformat                (config, &ok); if (ok) LogFileTimeformat                 = v; }
    { auto v = getInnerLogFilePath                      (config, &ok); if (ok) LogFilePath                       = v; }
    { auto v = getInnerLogFileMaxCount                  (config, &ok); if (ok) LogFileMaxCount                   = v; }
    { auto v = getInnerLogFileMaxSize                   (config, &ok); if (ok) LogFileMaxSize                    = v; }
}
