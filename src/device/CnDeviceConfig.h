#ifndef CNDEVICECONFIG_H
#define CNDEVICECONFIG_H

#include <CnGlobal.h>

class CnDevice;

class CnDeviceConfig
{
public:
    /*! \brief Sets constant key values for the configuration associative array.
    */
    struct Strings
    {
        const CnString Name                     ; ///< Configuration key. Name of this device/PLC
        const CnString EnableDevice             ; ///< Configuration key. Availability of this device/PLC for the execution sequence (1-on, 0-off)
        const CnString ModbusUnit               ; ///< Configuration key. Modbus address of this device/PLC
        const CnString RepeatCount              ; ///< Configuration key. Maximum number of Modbus request retries if unsuccessful
        const CnString RestoreTimeout           ; ///< Configuration key. Timeout for restoring connection after error (removal from execution sequence) (milliseconds)
        const CnString MaxReadCoils             ; ///< Configuration key. Maximum number of Coils to read in one request
        const CnString MaxWriteMultipleCoils    ; ///< Configuration key. Maximum number of Coils to write in one request
        const CnString MaxReadDiscreteInputs    ; ///< Configuration key. Maximum number of DiscreteInputs to read in one request
        const CnString MaxReadInputRegisters    ; ///< Configuration key. Maximum number of InputRegisters to read in one request
        const CnString MaxReadHoldingRegisters  ; ///< Configuration key. Maximum number of HoldingRegisters to read in one request
        const CnString MaxWriteMultipleRegisters; ///< Configuration key. Maximum number of HoldingRegisters to write in one request
        const CnString DefaultPeriod            ; ///< Configuration key. Default polling period for each Modbus Item
        const CnString RequestTimeout           ; ///< Configuration key. Request timeout for each Modbus Item, after which polling stops
        const CnString LogFlags                 ; ///< Configuration key.
        const CnString LogOutput                ; ///< Configuration key.
        const CnString LogFileFormat            ; ///< Configuration key.
        const CnString LogFileTimeformat        ; ///< Configuration key.
        const CnString LogFilePath              ; ///< Configuration key.
        const CnString LogFileMaxCount          ; ///< Configuration key.
        const CnString LogFileMaxSize           ; ///< Configuration key.

        /// \details Class constructor.
        Strings();

        /// \details Returns a reference to the global configuration key values object.
        static const Strings& instance();
    };

    /*! \brief Sets constant default values for settings.

        \details
     */
    struct Defaults
    {
        const CnString      Name                     ; ///< Default configuration. Name of this device/PLC
        const bool          EnableDevice             ; ///< Default configuration. Availability of this device/PLC for the execution sequence (1-on, 0-off)
        const uint8_t       ModbusUnit               ; ///< Default configuration. Modbus address of this device/PLC
        const uint32_t      RepeatCount              ; ///< Default configuration. Maximum number of Modbus request retries if unsuccessful
        const uint32_t      RestoreTimeout           ; ///< Default configuration. Timeout for restoring connection after error (removal from execution sequence) (milliseconds)
        const uint16_t      MaxReadCoils             ; ///< Default configuration. Maximum number of Coils to read in one request
        const uint16_t      MaxWriteMultipleCoils    ; ///< Default configuration. Maximum number of Coils to write in one request
        const uint16_t      MaxReadDiscreteInputs    ; ///< Default configuration. Maximum number of DiscreteInputs to read in one request
        const uint16_t      MaxReadInputRegisters    ; ///< Default configuration. Maximum number of InputRegisters to read in one request
        const uint16_t      MaxReadHoldingRegisters  ; ///< Default configuration. Maximum number of HoldingRegisters to read in one request
        const uint16_t      MaxWriteMultipleRegisters; ///< Default configuration. Maximum number of HoldingRegisters to write in one request
        const uint32_t      DefaultPeriod            ; ///< Default configuration. Default polling period for each Modbus Item
        const uint32_t      RequestTimeout           ; ///< Default configuration. Request timeout for each Modbus Item, after which polling stops
        const CnString      LogFlags                 ;
        const CnString      LogOutput                ;
        const CnString      LogFileFormat            ;
        const CnString      LogFileTimeformat        ;
        const CnString      LogFilePath              ;
        const int           LogFileMaxCount          ;
        const int           LogFileMaxSize           ;

        /// \details Class constructor.
        Defaults();

        /// \details Returns a reference to the global default configuration values object.
        static const Defaults& instance();
    };

public: // convertion function from inner values to config values
    inline static CnString toCfgName                     (const CnString &v) { return v; }
    inline static bool     toCfgEnableDevice             (bool            v) { return v; }
    inline static uint8_t  toCfgModbusUnit               (uint8_t         v) { return v; }
    inline static uint32_t toCfgRepeatCount              (uint32_t        v) { return v; }
    inline static uint32_t toCfgRestoreTimeout           (uint32_t        v) { return v; }
    inline static uint16_t toCfgMaxReadCoils             (uint16_t        v) { return v; }
    inline static uint16_t toCfgMaxWriteMultipleCoils    (uint16_t        v) { return v; }
    inline static uint16_t toCfgMaxReadDiscreteInputs    (uint16_t        v) { return v; }
    inline static uint16_t toCfgMaxReadInputRegisters    (uint16_t        v) { return v; }
    inline static uint16_t toCfgMaxReadHoldingRegisters  (uint16_t        v) { return v; }
    inline static uint16_t toCfgMaxWriteMultipleRegisters(uint16_t        v) { return v; }
    inline static uint32_t toCfgDefaultPeriod            (uint32_t        v) { return v; }
    inline static uint32_t toCfgRequestTimeout           (uint32_t        v) { return v; }
    inline static CnString toCfgLogFlags                 (Cn::LogFlags    v) { return Cn::toString(v); }
    inline static CnString toCfgLogOutput                (const CnString &v) { return v; }
    inline static CnString toCfgLogFileFormat            (const CnString &v) { return v; }
    inline static CnString toCfgLogFileTimeformat        (const CnString &v) { return v; }
    inline static CnString toCfgLogFilePath              (const CnString &v) { return v; }
    inline static int      toCfgLogFileMaxCount          (int             v) { return v; }
    inline static int      toCfgLogFileMaxSize           (int             v) { return v; }

public: // convertion function from config values to inner values
    inline static CnString     toInnerName                     (const CnString&      v) { return v; }
    inline static bool         toInnerEnableDevice             (bool                 v) { return v; }
    inline static uint8_t      toInnerModbusUnit               (uint8_t              v) { return v; }
    inline static uint32_t     toInnerRepeatCount              (uint32_t             v) { return v; }
    inline static uint32_t     toInnerRestoreTimeoutMillisec   (uint32_t             v) { return v; }
    inline static uint16_t     toInnerMaxReadCoils             (uint16_t             v) { return v; }
    inline static uint16_t     toInnerMaxWriteMultipleCoils    (uint16_t             v) { return v; }
    inline static uint16_t     toInnerMaxReadDiscreteInputs    (uint16_t             v) { return v; }
    inline static uint16_t     toInnerMaxReadInputRegisters    (uint16_t             v) { return v; }
    inline static uint16_t     toInnerMaxReadHoldingRegisters  (uint16_t             v) { return v; }
    inline static uint16_t     toInnerMaxWriteMultipleRegisters(uint16_t             v) { return v; }
    inline static uint32_t     toInnerDefaultPeriodMillisec    (uint32_t             v) { return v; }
    inline static uint32_t     toInnerRequestTimeoutMillisec   (uint32_t             v) { return v; }
    inline static Cn::LogFlags toInnerLogFlags                 (const CnString&      v) { return Cn::toLogFlags(v); }
    inline static CnString     toInnerLogOutput                (const CnString&      v) { return v; }
    inline static CnString     toInnerLogFileFormat            (const CnString&      v) { return v; }
    inline static CnString     toInnerLogFileTimeformat        (const CnString&      v) { return v; }
    inline static CnString     toInnerLogFilePath              (const CnString&      v) { return v; }
    inline static int          toInnerLogFileMaxCount          (int                  v) { return v; }
    inline static int          toInnerLogFileMaxSize           (int                  v) { return v; }

public: // static Config getters
    static CnString getCfgName                     (const Cn::Config& config, bool *ok = nullptr);
    static bool     getCfgEnableDevice             (const Cn::Config& config, bool *ok = nullptr);
    static uint8_t  getCfgModbusUnit               (const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgRepeatCount              (const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgRestoreTimeout           (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgMaxReadCoils             (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgMaxWriteMultipleCoils    (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgMaxReadDiscreteInputs    (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgMaxReadInputRegisters    (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgMaxReadHoldingRegisters  (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgMaxWriteMultipleRegisters(const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgDefaultPeriod            (const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgRequestTimeout           (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFlags                 (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogOutput                (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFileFormat            (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFileTimeformat        (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFilePath              (const Cn::Config& config, bool *ok = nullptr);
    static int      getCfgLogFileMaxCount          (const Cn::Config& config, bool *ok = nullptr);
    static int      getCfgLogFileMaxSize           (const Cn::Config& config, bool *ok = nullptr);

public: // static Inner getters
    inline static CnString     getInnerName                     (const Cn::Config& config, bool *ok = nullptr) { return toInnerName                             (getCfgName                     (config, ok)); }
    inline static bool         getInnerEnableDevice             (const Cn::Config& config, bool *ok = nullptr) { return toInnerEnableDevice                     (getCfgEnableDevice             (config, ok)); }
    inline static uint8_t      getInnerModbusUnit               (const Cn::Config& config, bool *ok = nullptr) { return toInnerModbusUnit                       (getCfgModbusUnit               (config, ok)); }
    inline static uint32_t     getInnerRepeatCount              (const Cn::Config& config, bool *ok = nullptr) { return toInnerRepeatCount                      (getCfgRepeatCount              (config, ok)); }
    inline static uint32_t     getInnerRestoreTimeoutMillisec   (const Cn::Config& config, bool *ok = nullptr) { return toInnerRestoreTimeoutMillisec           (getCfgRestoreTimeout           (config, ok)); }
    inline static uint16_t     getInnerMaxReadCoils             (const Cn::Config& config, bool *ok = nullptr) { return toInnerMaxReadCoils                     (getCfgMaxReadCoils             (config, ok)); }
    inline static uint16_t     getInnerMaxWriteMultipleCoils    (const Cn::Config& config, bool *ok = nullptr) { return toInnerMaxWriteMultipleCoils            (getCfgMaxWriteMultipleCoils    (config, ok)); }
    inline static uint16_t     getInnerMaxReadDiscreteInputs    (const Cn::Config& config, bool *ok = nullptr) { return toInnerMaxReadDiscreteInputs            (getCfgMaxReadDiscreteInputs    (config, ok)); }
    inline static uint16_t     getInnerMaxReadInputRegisters    (const Cn::Config& config, bool *ok = nullptr) { return toInnerMaxReadInputRegisters            (getCfgMaxReadInputRegisters    (config, ok)); }
    inline static uint16_t     getInnerMaxReadHoldingRegisters  (const Cn::Config& config, bool *ok = nullptr) { return toInnerMaxReadHoldingRegisters          (getCfgMaxReadHoldingRegisters  (config, ok)); }
    inline static uint16_t     getInnerMaxWriteMultipleRegisters(const Cn::Config& config, bool *ok = nullptr) { return toInnerMaxWriteMultipleRegisters        (getCfgMaxWriteMultipleRegisters(config, ok)); }
    inline static uint32_t     getInnerDefaultPeriodMillisec    (const Cn::Config& config, bool *ok = nullptr) { return toInnerDefaultPeriodMillisec            (getCfgDefaultPeriod            (config, ok)); }
    inline static uint32_t     getInnerRequestTimeoutMillisec   (const Cn::Config& config, bool *ok = nullptr) { return toInnerRequestTimeoutMillisec           (getCfgRequestTimeout           (config, ok)); }
    inline static Cn::LogFlags getInnerLogFlags                 (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFlags                         (getCfgLogFlags                 (config, ok)); }
    inline static CnString     getInnerLogOutput                (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogOutput                        (getCfgLogOutput                (config, ok)); }
    inline static CnString     getInnerLogFileFormat            (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileFormat                    (getCfgLogFileFormat            (config, ok)); }
    inline static CnString     getInnerLogFileTimeformat        (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileTimeformat                (getCfgLogFileTimeformat        (config, ok)); }
    inline static CnString     getInnerLogFilePath              (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFilePath                      (getCfgLogFilePath              (config, ok)); }
    inline static int          getInnerLogFileMaxCount          (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileMaxCount                  (getCfgLogFileMaxCount          (config, ok)); }
    inline static int          getInnerLogFileMaxSize           (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileMaxSize                   (getCfgLogFileMaxSize           (config, ok)); }

    public:
    CnDeviceConfig();

public:
    Cn::Config config() const;
    void setConfig(const Cn::Config& config);

public: // Config
    inline CnString         CfgName                     () const { CnCriticalSectionLocker _(&cs); return toCfgName                     (Name                             ); } ///< \details Конфігурація. Ім’я даного пристрою/ПЛК  
    inline bool             CfgEnableDevice             () const { CnCriticalSectionLocker _(&cs); return toCfgEnableDevice             (EnableDevice                     ); } ///< \details Конфігурація. Доступність даного пристрою/ПЛК для ланцюга виконання (1-вкл, 0-викл) 
    inline uint8_t          CfgModbusUnit               () const { CnCriticalSectionLocker _(&cs); return toCfgModbusUnit               (ModbusUnit                       ); } ///< \details Конфігурація. Modbus-адреса даного пристрою/ПЛК 
    inline uint32_t         CfgRepeatCount              () const { CnCriticalSectionLocker _(&cs); return toCfgRepeatCount              (RepeatCount                      ); } ///< \details Конфігурація. Максимальна кількість повторів Modbus-запиту при неуспішності його виконання
    inline uint32_t         CfgRestoreTimeout           () const { CnCriticalSectionLocker _(&cs); return toCfgRestoreTimeout           (RestoreTimeoutMillisec           ); } ///< \details Конфігурація. Тайм-аут відновлення зв’язку після помилки (виключення із ланцюга виконання) (мілісекунди) 
    inline uint16_t         CfgMaxReadCoils             () const { CnCriticalSectionLocker _(&cs); return toCfgMaxReadCoils             (MaxReadCoils                     ); } ///< \details Конфігурація. Максимальна кількість Coils для читання однією посилкою 
    inline uint16_t         CfgMaxWriteMultipleCoils    () const { CnCriticalSectionLocker _(&cs); return toCfgMaxWriteMultipleCoils    (MaxWriteMultipleCoils            ); } ///< \details Конфігурація. Максимальна кількість Coils для запису однією посилкою 
    inline uint16_t         CfgMaxReadDiscreteInputs    () const { CnCriticalSectionLocker _(&cs); return toCfgMaxReadDiscreteInputs    (MaxReadDiscreteInputs            ); } ///< \details Конфігурація. Максимальна кількість DiscreteInputs для читання однією посилкою 
    inline uint16_t         CfgMaxReadInputRegisters    () const { CnCriticalSectionLocker _(&cs); return toCfgMaxReadInputRegisters    (MaxReadInputRegisters            ); } ///< \details Конфігурація. Максимальна кількість InputRegisters для читання однією посилкою 
    inline uint16_t         CfgMaxReadHoldingRegisters  () const { CnCriticalSectionLocker _(&cs); return toCfgMaxReadHoldingRegisters  (MaxReadHoldingRegisters          ); } ///< \details Конфігурація. Максимальна кількість HoldingRegisters для читання однією посилкою 
    inline uint16_t         CfgMaxWriteMultipleRegisters() const { CnCriticalSectionLocker _(&cs); return toCfgMaxWriteMultipleRegisters(MaxWriteMultipleRegisters        ); } ///< \details Конфігурація. Максимальна кількість HoldingRegisters для запису однією посилкою 
    inline uint32_t         CfgDefaultPeriod            () const { CnCriticalSectionLocker _(&cs); return toCfgDefaultPeriod            (DefaultPeriodMillisec            ); } ///< \details Конфігурація. Період опитування за замовчуванням для кожного Modbus Item 
    inline uint32_t         CfgRequestTimeout           () const { CnCriticalSectionLocker _(&cs); return toCfgRequestTimeout           (RequestTimeoutMillisec           ); } ///< \details Конфігурація. Таймаут запиту для кожного Modbus Item, після сплинення якого даний  для кожного Modbus Item припиняє опитуватись 
    inline CnString         CfgLogFlags                 () const { CnCriticalSectionLocker _(&cs); return toCfgLogFlags                 (LogFlags                         ); } ///< \details Конфігурація. 
    inline CnString         CfgLogOutput                () const { CnCriticalSectionLocker _(&cs); return toCfgLogOutput                (LogOutput                        ); } ///< \details Конфігурація. 
    inline CnString         CfgLogFileFormat            () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileFormat            (LogFileFormat                    ); } ///< \details Конфігурація. 
    inline CnString         CfgLogFileTimeformat        () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileTimeformat        (LogFileTimeformat                ); } ///< \details Конфігурація. 
    inline CnString         CfgLogFilePath              () const { CnCriticalSectionLocker _(&cs); return toCfgLogFilePath              (LogFilePath                      ); } ///< \details Конфігурація. 
    inline int              CfgLogFileMaxCount          () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileMaxCount          (LogFileMaxCount                  ); } ///< \details Конфігурація. 
    inline int              CfgLogFileMaxSize           () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileMaxSize           (LogFileMaxSize                   ); } ///< \details Конфігурація. 

public:
    inline void setCfgName                     (const CnString&      v) { setInnerName                             (toInnerName                             (v)); }
    inline void setCfgEnableDevice             (bool                 v) { setInnerEnableDevice                     (toInnerEnableDevice                     (v)); }
    inline void setCfgModbusUnit               (uint8_t              v) { setInnerModbusUnit                       (toInnerModbusUnit                       (v)); }
    inline void setCfgRepeatCount              (uint32_t             v) { setInnerRepeatCount                      (toInnerRepeatCount                      (v)); }
    inline void setCfgRestoreTimeout           (uint32_t             v) { setInnerRestoreTimeoutMillisec           (toInnerRestoreTimeoutMillisec           (v)); }
    inline void setCfgMaxReadCoils             (uint16_t             v) { setInnerMaxReadCoils                     (toInnerMaxReadCoils                     (v)); }
    inline void setCfgMaxWriteMultipleCoils    (uint16_t             v) { setInnerMaxWriteMultipleCoils            (toInnerMaxWriteMultipleCoils            (v)); }
    inline void setCfgMaxReadDiscreteInputs    (uint16_t             v) { setInnerMaxReadDiscreteInputs            (toInnerMaxReadDiscreteInputs            (v)); }
    inline void setCfgMaxReadInputRegisters    (uint16_t             v) { setInnerMaxReadInputRegisters            (toInnerMaxReadInputRegisters            (v)); }
    inline void setCfgMaxReadHoldingRegisters  (uint16_t             v) { setInnerMaxReadHoldingRegisters          (toInnerMaxReadHoldingRegisters          (v)); }
    inline void setCfgMaxWriteMultipleRegisters(uint16_t             v) { setInnerMaxWriteMultipleRegisters        (toInnerMaxWriteMultipleRegisters        (v)); }
    inline void setCfgDefaultPeriod            (uint32_t             v) { setInnerDefaultPeriodMillisec            (toInnerDefaultPeriodMillisec            (v)); }
    inline void setCfgRequestTimeout           (uint32_t             v) { setInnerRequestTimeoutMillisec           (toInnerRequestTimeoutMillisec           (v)); }
    inline void setCfgLogFlags                 (const CnString&      v) { setInnerLogFlags                         (toInnerLogFlags                         (v)); } 
    inline void setCfgLogOutput                (const CnString&      v) { setInnerLogOutput                        (toInnerLogOutput                        (v)); } 
    inline void setCfgLogFileFormat            (const CnString&      v) { setInnerLogFileFormat                    (toInnerLogFileFormat                    (v)); } 
    inline void setCfgLogFileTimeformat        (const CnString&      v) { setInnerLogFileTimeformat                (toInnerLogFileTimeformat                (v)); } 
    inline void setCfgLogFilePath              (const CnString&      v) { setInnerLogFilePath                      (toInnerLogFilePath                      (v)); } 
    inline void setCfgLogFileMaxCount          (int                  v) { setInnerLogFileMaxCount                  (toInnerLogFileMaxCount                  (v)); } 
    inline void setCfgLogFileMaxSize           (int                  v) { setInnerLogFileMaxSize                   (toInnerLogFileMaxSize                   (v)); } 

public:
    inline CnString                     innerName                             () const { CnCriticalSectionLocker _(&cs); return Name                             ; } 
    inline bool                         innerEnableDevice                     () const { CnCriticalSectionLocker _(&cs); return EnableDevice                     ; } 
    inline uint8_t                      innerModbusUnit                       () const { CnCriticalSectionLocker _(&cs); return ModbusUnit                       ; } 
    inline uint32_t                     innerRepeatCount                      () const { CnCriticalSectionLocker _(&cs); return RepeatCount                      ; } 
    inline uint32_t                     innerRestoreTimeoutMillisec           () const { CnCriticalSectionLocker _(&cs); return RestoreTimeoutMillisec           ; } 
    inline uint16_t                     innerMaxReadCoils                     () const { CnCriticalSectionLocker _(&cs); return MaxReadCoils                     ; } 
    inline uint16_t                     innerMaxWriteMultipleCoils            () const { CnCriticalSectionLocker _(&cs); return MaxWriteMultipleCoils            ; } 
    inline uint16_t                     innerMaxReadDiscreteInputs            () const { CnCriticalSectionLocker _(&cs); return MaxReadDiscreteInputs            ; } 
    inline uint16_t                     innerMaxReadInputRegisters            () const { CnCriticalSectionLocker _(&cs); return MaxReadInputRegisters            ; } 
    inline uint16_t                     innerMaxReadHoldingRegisters          () const { CnCriticalSectionLocker _(&cs); return MaxReadHoldingRegisters          ; } 
    inline uint16_t                     innerMaxWriteMultipleRegisters        () const { CnCriticalSectionLocker _(&cs); return MaxWriteMultipleRegisters        ; } 
    inline uint32_t                     innerDefaultPeriodMillisec            () const { CnCriticalSectionLocker _(&cs); return DefaultPeriodMillisec            ; } 
    inline uint32_t                     innerRequestTimeoutMillisec           () const { CnCriticalSectionLocker _(&cs); return RequestTimeoutMillisec           ; } 
    inline Cn::LogFlags                 innerLogFlags                         () const { CnCriticalSectionLocker _(&cs); return LogFlags                         ; }
    inline CnString                     innerLogOutput                        () const { CnCriticalSectionLocker _(&cs); return LogOutput                        ; }
    inline CnString                     innerLogFileFormat                    () const { CnCriticalSectionLocker _(&cs); return LogFileFormat                    ; }
    inline CnString                     innerLogFileTimeformat                () const { CnCriticalSectionLocker _(&cs); return LogFileTimeformat                ; }
    inline CnString                     innerLogFilePath                      () const { CnCriticalSectionLocker _(&cs); return LogFilePath                      ; }
    inline int                          innerLogFileMaxCount                  () const { CnCriticalSectionLocker _(&cs); return LogFileMaxCount                  ; }
    inline int                          innerLogFileMaxSize                   () const { CnCriticalSectionLocker _(&cs); return LogFileMaxSize                   ; }

public:
    inline void setInnerName                     (const CnString& v) { CnCriticalSectionLocker _(&cs); Name                              = v; }
    inline void setInnerEnableDevice             (bool            v) { CnCriticalSectionLocker _(&cs); EnableDevice                      = v; }
    inline void setInnerModbusUnit               (uint8_t         v) { CnCriticalSectionLocker _(&cs); ModbusUnit                        = v; }
    inline void setInnerRepeatCount              (uint32_t        v) { CnCriticalSectionLocker _(&cs); RepeatCount                       = v; }
    inline void setInnerRestoreTimeoutMillisec   (uint32_t        v) { CnCriticalSectionLocker _(&cs); RestoreTimeoutMillisec            = v; }
    inline void setInnerMaxReadCoils             (uint16_t        v) { CnCriticalSectionLocker _(&cs); MaxReadCoils                      = v; }
    inline void setInnerMaxWriteMultipleCoils    (uint16_t        v) { CnCriticalSectionLocker _(&cs); MaxWriteMultipleCoils             = v; }
    inline void setInnerMaxReadDiscreteInputs    (uint16_t        v) { CnCriticalSectionLocker _(&cs); MaxReadDiscreteInputs             = v; }
    inline void setInnerMaxReadInputRegisters    (uint16_t        v) { CnCriticalSectionLocker _(&cs); MaxReadInputRegisters             = v; }
    inline void setInnerMaxReadHoldingRegisters  (uint16_t        v) { CnCriticalSectionLocker _(&cs); MaxReadHoldingRegisters           = v; }
    inline void setInnerMaxWriteMultipleRegisters(uint16_t        v) { CnCriticalSectionLocker _(&cs); MaxWriteMultipleRegisters         = v; }
    inline void setInnerDefaultPeriodMillisec    (uint32_t        v) { CnCriticalSectionLocker _(&cs); DefaultPeriodMillisec             = v; }
    inline void setInnerRequestTimeoutMillisec   (uint32_t        v) { CnCriticalSectionLocker _(&cs); RequestTimeoutMillisec            = v; }
    inline void setInnerLogFlags                 (Cn::LogFlags    v) { CnCriticalSectionLocker _(&cs); LogFlags                          = v; } 
    inline void setInnerLogOutput                (const CnString& v) { CnCriticalSectionLocker _(&cs); LogOutput                         = v; } 
    inline void setInnerLogFileFormat            (const CnString& v) { CnCriticalSectionLocker _(&cs); LogFileFormat                     = v; } 
    inline void setInnerLogFileTimeformat        (const CnString& v) { CnCriticalSectionLocker _(&cs); LogFileTimeformat                 = v; } 
    inline void setInnerLogFilePath              (const CnString& v) { CnCriticalSectionLocker _(&cs); LogFilePath                       = v; } 
    inline void setInnerLogFileMaxCount          (int             v) { CnCriticalSectionLocker _(&cs); LogFileMaxCount                   = v; } 
    inline void setInnerLogFileMaxSize           (int             v) { CnCriticalSectionLocker _(&cs); LogFileMaxSize                    = v; } 

private:
    friend class CnDevice;

    mutable CnCriticalSection cs;
    
private: // Inner Config
    CnString                     Name                             ;
    bool                         EnableDevice                     ;
    uint8_t                      ModbusUnit                       ;
    uint32_t                     RepeatCount                      ;
    uint32_t                     RestoreTimeoutMillisec           ;
    uint16_t                     MaxReadCoils                     ;
    uint16_t                     MaxWriteMultipleCoils            ;
    uint16_t                     MaxReadDiscreteInputs            ;
    uint16_t                     MaxReadInputRegisters            ;
    uint16_t                     MaxReadHoldingRegisters          ;
    uint16_t                     MaxWriteMultipleRegisters        ;
    uint32_t                     DefaultPeriodMillisec            ;
    uint32_t                     RequestTimeoutMillisec           ;
    Cn::LogFlags                 LogFlags                         ;
    CnString                     LogOutput                        ;
    CnString                     LogFileFormat                    ;
    CnString                     LogFileTimeformat                ;
    CnString                     LogFilePath                      ;
    int                          LogFileMaxCount                  ;
    int                          LogFileMaxSize                   ;
};

#endif // CNDEVICECONFIG_H