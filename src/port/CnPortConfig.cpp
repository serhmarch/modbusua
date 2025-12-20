#include "CnPortConfig.h"

#include <app/CnAppConfig.h>
#include <log/CnLoggerFile.h>

CnPortConfig::Strings::Strings() :
    Name             (CnSTR("Name"           )),
    Enable           (CnSTR("Enable"         )),
    Type             (Cn::Strings::instance().Type            ),
    Host             (Cn::Strings::instance().Host            ),
    Port             (Cn::Strings::instance().Port            ),
    Timeout          (Cn::Strings::instance().Timeout         ),
    SerialPortName   (Cn::Strings::instance().SerialPortName  ),
    BaudRate         (Cn::Strings::instance().BaudRate        ),
    DataBits         (Cn::Strings::instance().DataBits        ),
    Parity           (Cn::Strings::instance().Parity          ),
    StopBits         (Cn::Strings::instance().StopBits        ),
    FlowControl      (Cn::Strings::instance().FlowControl     ),
    TimeoutFirstByte (Cn::Strings::instance().TimeoutFirstByte),
    TimeoutInterByte (Cn::Strings::instance().TimeoutInterByte),
    LogFlags         (CnAppConfig::Strings::instance().LogFlags         ),
    LogOutput        (CnAppConfig::Strings::instance().LogOutput        ),
    LogFileFormat    (CnAppConfig::Strings::instance().LogFileFormat    ),
    LogFileTimeformat(CnAppConfig::Strings::instance().LogFileTimeformat),
    LogFilePath      (CnAppConfig::Strings::instance().LogFilePath      ),
    LogFileMaxCount  (CnAppConfig::Strings::instance().LogFileMaxCount  ),
    LogFileMaxSize   (CnAppConfig::Strings::instance().LogFileMaxSize   )
{
}

const CnPortConfig::Strings &CnPortConfig::Strings::instance()
{
    static const Strings s;
    return s;
}

CnPortConfig::Defaults::Defaults() :
    Name             (CnSTR("PORT")),
    Enable           (true),
    Type             (Cn::toString(Cn::Defaults::instance().Type)),
    Host             (Cn::Defaults::instance().Host            ),
    Port             (Cn::Defaults::instance().Port            ),
    Timeout          (Cn::Defaults::instance().Timeout         ),
    SerialPortName   (Cn::Defaults::instance().SerialPortName  ),
    BaudRate         (Cn::Defaults::instance().BaudRate        ),
    DataBits         (Cn::Defaults::instance().DataBits        ),
    Parity           (Cn::toString(Cn::Defaults::instance().Parity     )),
    StopBits         (Cn::toString(Cn::Defaults::instance().StopBits   )),
    FlowControl      (Cn::toString(Cn::Defaults::instance().FlowControl)),
    TimeoutFirstByte (Cn::Defaults::instance().TimeoutFirstByte),
    TimeoutInterByte (Cn::Defaults::instance().TimeoutInterByte),
    LogFlags         (CnAppConfig::Defaults::instance().LogFlags         ),
    LogOutput        (CnAppConfig::Defaults::instance().LogOutput        ),
    LogFileFormat    (CnAppConfig::Defaults::instance().LogFileFormat    ),
    LogFileTimeformat(CnAppConfig::Defaults::instance().LogFileTimeformat),
    LogFilePath      (CnAppConfig::Defaults::instance().LogFilePath      ),
    LogFileMaxCount  (CnAppConfig::Defaults::instance().LogFileMaxCount  ),
    LogFileMaxSize   (CnAppConfig::Defaults::instance().LogFileMaxSize   )

{
}

const CnPortConfig::Defaults &CnPortConfig::Defaults::instance()
{
    static const Defaults d;
    return d;
}

CnString CnPortConfig::getCfgName             (const Cn::Config &config, bool *ok) { CN_GET_CFGMAP_VALUE(Name             , toString) }
bool     CnPortConfig::getCfgEnable           (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(Enable           , toBool  ) }
CnString CnPortConfig::getCfgType             (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(Type             , toString) }
CnString CnPortConfig::getCfgHost             (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(Host             , toString) }
uint16_t CnPortConfig::getCfgPort             (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(Port             , toUInt16) }
uint32_t CnPortConfig::getCfgTimeout          (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(Timeout          , toUInt32 ) }
CnString CnPortConfig::getCfgSerialPortName   (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(SerialPortName   , toString) }
int32_t  CnPortConfig::getCfgBaudRate         (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(BaudRate         , toUInt16) }
int8_t   CnPortConfig::getCfgDataBits         (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(DataBits         , toInt8  ) }
CnString CnPortConfig::getCfgParity           (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(Parity           , toString) }
CnString CnPortConfig::getCfgStopBits         (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(StopBits         , toString) }
CnString CnPortConfig::getCfgFlowControl      (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(FlowControl      , toString) }
uint32_t CnPortConfig::getCfgTimeoutFirstByte (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(TimeoutFirstByte , toUInt32 ) }
uint32_t CnPortConfig::getCfgTimeoutInterByte (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(TimeoutInterByte , toUInt32 ) }
CnString CnPortConfig::getCfgLogFlags         (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFlags         , toString) }
CnString CnPortConfig::getCfgLogOutput        (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogOutput        , toString) }
CnString CnPortConfig::getCfgLogFileFormat    (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileFormat    , toString) }
CnString CnPortConfig::getCfgLogFileTimeformat(const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileTimeformat, toString) }
CnString CnPortConfig::getCfgLogFilePath      (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFilePath      , toString) }
int      CnPortConfig::getCfgLogFileMaxCount  (const Cn::Config& config, bool *ok) { CN_GET_CFGMAP_VALUE(LogFileMaxCount  , toInt   ) }
int      CnPortConfig::getCfgLogFileMaxSize   (const Cn::Config& config, bool *ok)
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

CnPortConfig::CnPortConfig()
{
    const Defaults &d = Defaults::instance();

    Name                     = toInnerName                    (d.Name             );
    Enable                   = toInnerEnable                  (d.Enable           );
    TypeEnum                 = toInnerTypeEnum                (d.Type             );
    Host                     = toInnerHost                    (d.Host             );
    Port                     = toInnerPort                    (d.Port             );
    TimeoutMillisec          = toInnerTimeoutMillisec         (d.Timeout          );
    SerialPortName           = toInnerSerialPortName          (d.SerialPortName   );
    BaudRate                 = toInnerBaudRate                (d.BaudRate         );
    DataBits                 = toInnerDataBits                (d.DataBits         );
    ParityEnum               = toInnerParityEnum              (d.Parity           );
    StopBitsEnum             = toInnerStopBitsEnum            (d.StopBits         );
    FlowControlEnum          = toInnerFlowControlEnum         (d.FlowControl      );
    TimeoutFirstByteMillisec = toInnerTimeoutFirstByteMillisec(d.TimeoutFirstByte );
    TimeoutInterByteMillisec = toInnerTimeoutInterByteMillisec(d.TimeoutInterByte );
    LogFlags                 = toInnerLogFlags                (d.LogFlags         );
    LogOutput                = toInnerLogOutput               (d.LogOutput        );
    LogFileFormat            = toInnerLogFileFormat           (d.LogFileFormat    );
    LogFileTimeformat        = toInnerLogFileTimeformat       (d.LogFileTimeformat);
    LogFilePath              = toInnerLogFilePath             (d.LogFilePath      );
    LogFileMaxCount          = toInnerLogFileMaxCount         (d.LogFileMaxCount  );
    LogFileMaxSize           = toInnerLogFileMaxSize          (d.LogFileMaxSize   );
}

Cn::Config CnPortConfig::config() const
{
    const Strings& sKeys = Strings::instance();
    Cn::Config config;

    CnCriticalSectionLocker _(&cs);
    config[sKeys.Name             ] = toCfgName             (Name                    );
    config[sKeys.Enable           ] = toCfgEnable           (Enable                  );
    config[sKeys.Type             ] = toCfgType             (TypeEnum                );
    config[sKeys.Host             ] = toCfgHost             (Host                    );
    config[sKeys.Port             ] = toCfgPort             (Port                    );
    config[sKeys.Timeout          ] = toCfgTimeout          (TimeoutMillisec         );
    config[sKeys.SerialPortName   ] = toCfgSerialPortName   (SerialPortName          );
    config[sKeys.BaudRate         ] = toCfgBaudRate         (BaudRate                );
    config[sKeys.DataBits         ] = toCfgDataBits         (DataBits                );
    config[sKeys.Parity           ] = toCfgParity           (ParityEnum              );
    config[sKeys.StopBits         ] = toCfgStopBits         (StopBitsEnum            );
    config[sKeys.FlowControl      ] = toCfgFlowControl      (FlowControlEnum         );
    config[sKeys.TimeoutFirstByte ] = toCfgTimeoutFirstByte (TimeoutFirstByteMillisec);
    config[sKeys.TimeoutInterByte ] = toCfgTimeoutInterByte (TimeoutInterByteMillisec);
    config[sKeys.LogFlags         ] = toCfgLogFlags         (LogFlags                );
    config[sKeys.LogOutput        ] = toCfgLogOutput        (LogOutput               );
    config[sKeys.LogFileFormat    ] = toCfgLogFileFormat    (LogFileFormat           );
    config[sKeys.LogFileTimeformat] = toCfgLogFileTimeformat(LogFileTimeformat       );
    config[sKeys.LogFilePath      ] = toCfgLogFilePath      (LogFilePath             );
    config[sKeys.LogFileMaxCount  ] = toCfgLogFileMaxCount  (LogFileMaxCount         );
    config[sKeys.LogFileMaxSize   ] = toCfgLogFileMaxSize   (LogFileMaxSize          );
    
    return config; 
}

void CnPortConfig::setConfig(const Cn::Config &config)
{
    const Strings& sKeys = Strings::instance();
    bool ok;
    CnCriticalSectionLocker _(&cs);
    { auto v = getInnerName                    (config, &ok); if (ok) Name                      = v; }
    { auto v = getInnerEnable                  (config, &ok); if (ok) Enable                    = v; }
    { auto v = getInnerTypeEnum                (config, &ok); if (ok) TypeEnum                  = v; }
    { auto v = getInnerHost                    (config, &ok); if (ok) Host                      = v; }
    { auto v = getInnerPort                    (config, &ok); if (ok) Port                      = v; }
    { auto v = getInnerTimeoutMillisec         (config, &ok); if (ok) TimeoutMillisec           = v; }
    { auto v = getInnerSerialPortName          (config, &ok); if (ok) SerialPortName            = v; }
    { auto v = getInnerBaudRate                (config, &ok); if (ok) BaudRate                  = v; }
    { auto v = getInnerDataBits                (config, &ok); if (ok) DataBits                  = v; }
    { auto v = getInnerParityEnum              (config, &ok); if (ok) ParityEnum                = v; }
    { auto v = getInnerStopBitsEnum            (config, &ok); if (ok) StopBitsEnum              = v; }
    { auto v = getInnerFlowControlEnum         (config, &ok); if (ok) FlowControlEnum           = v; }
    { auto v = getInnerTimeoutFirstByteMillisec(config, &ok); if (ok) TimeoutFirstByteMillisec  = v; }
    { auto v = getInnerTimeoutInterByteMillisec(config, &ok); if (ok) TimeoutInterByteMillisec  = v; }
    { auto v = getInnerLogFlags                (config, &ok); if (ok) LogFlags                  = v; }
    { auto v = getInnerLogOutput               (config, &ok); if (ok) LogOutput                 = v; }
    { auto v = getInnerLogFileFormat           (config, &ok); if (ok) LogFileFormat             = v; }
    { auto v = getInnerLogFileTimeformat       (config, &ok); if (ok) LogFileTimeformat         = v; }
    { auto v = getInnerLogFilePath             (config, &ok); if (ok) LogFilePath               = v; }
    { auto v = getInnerLogFileMaxCount         (config, &ok); if (ok) LogFileMaxCount           = v; }
    { auto v = getInnerLogFileMaxSize          (config, &ok); if (ok) LogFileMaxSize            = v; }
}
