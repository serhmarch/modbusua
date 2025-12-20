#ifndef CNPORTCONFIG_H
#define CNPORTCONFIG_H

#include <CnGlobal.h>

class CnPort;

class CnPortConfig
{
public:
    /*! \brief Defines constant configuration key names.
        \details Keys used in the associative configuration map.
    */
    struct Strings
    {
        const CnString Name             ; ///< Config key. Name of this port
        const CnString Enable           ; ///< Config key. Availability of this port for runtime execution sequence (1-on, 0-off)
        const CnString Type             ; ///< Config key. Modbus protocol type
        const CnString Host             ; ///< Config key. IP address or DNS name of the remote device
        const CnString Port             ; ///< Config key. TCP port number of the remote device (standard Modbus TCP port 502)
        const CnString Timeout          ; ///< Config key. Connection timeout (milliseconds)
        const CnString SerialPortName   ; ///< Config key for the serial port name
        const CnString BaudRate         ; ///< Config key for the serial port's baud rate
        const CnString DataBits         ; ///< Config key for the serial port's data bits
        const CnString Parity           ; ///< Config key for the serial port's parity
        const CnString StopBits         ; ///< Config key for the serial port's stop bits
        const CnString FlowControl      ; ///< Config key for the serial port's flow control
        const CnString TimeoutFirstByte ; ///< Config key for the serial port's timeout waiting first byte of packet
        const CnString TimeoutInterByte ; ///< Config key for the serial port's timeout waiting next byte of packet
        const CnString LogFlags         ; ///< Config key. Logging flags
        const CnString LogOutput        ; ///< Config key. Logging output target
        const CnString LogFileFormat    ; ///< Config key. Log file message format
        const CnString LogFileTimeformat; ///< Config key. Log file time format
        const CnString LogFilePath      ; ///< Config key. Log file path
        const CnString LogFileMaxCount  ; ///< Config key. Max number of rotated log files
        const CnString LogFileMaxSize   ; ///< Config key. Max size of a single log file

        /// \details Constructs the key names container.
        Strings();

        /// \details Returns a reference to the global key names singleton.
        static const Strings& instance();
    };

    /*! \brief Defines constant default configuration values.
        \details Provides defaults for port connection and logging.
     */
    struct Defaults
    {
        const CnString Name             ; ///< Default config. Name of this port
        const bool     Enable           ; ///< Default config. Availability for runtime execution sequence (1-on, 0-off)
        const CnString Type             ; ///< Default config. Modbus protocol type
        const CnString Host             ; ///< Default config. IP address or DNS name of the remote device
        const uint16_t Port             ; ///< Default config. TCP port number (standard Modbus TCP port 502)
        const uint32_t Timeout          ; ///< Default config. Connection timeout (milliseconds)
        const CnString SerialPortName   ; ///< Default value for the serial port name
        const int32_t  BaudRate         ; ///< Default value for the serial port's baud rate
        const int8_t   DataBits         ; ///< Default value for the serial port's data bits
        const CnString Parity           ; ///< Default value for the serial port's parity
        const CnString StopBits         ; ///< Default value for the serial port's stop bits
        const CnString FlowControl      ; ///< Default value for the serial port's flow control
        const uint32_t TimeoutFirstByte ; ///< Default value for the serial port's timeout waiting first byte of packet
        const uint32_t TimeoutInterByte ; ///< Default value for the serial port's timeout waiting next byte of packet
        const CnString LogFlags         ; ///< Default config. Logging flags
        const CnString LogOutput        ; ///< Default config. Logging output target
        const CnString LogFileFormat    ; ///< Default config. Log file message format
        const CnString LogFileTimeformat; ///< Default config. Log file time format
        const CnString LogFilePath      ; ///< Default config. Log file path
        const int      LogFileMaxCount  ; ///< Default config. Max number of rotated log files
        const int      LogFileMaxSize   ; ///< Default config. Max size of a single log file

        /// \details Constructs the defaults container.
        Defaults();
        /// \details Returns a reference to the global defaults singleton.
        static const Defaults& instance();
    };

public: // convertion function from inner values to config values
    inline static CnString toCfgName             (const CnString       &v) { return v; }
    inline static bool     toCfgEnable           (bool                  v) { return v; }
    inline static CnString toCfgType             (Modbus::ProtocolType  v) { return Cn::toString(v); }
    inline static CnString toCfgHost             (const CnString       &v) { return v; }
    inline static uint16_t toCfgPort             (uint16_t              v) { return v; }
    inline static uint32_t toCfgTimeout          (uint32_t              v) { return v; }
    inline static CnString toCfgSerialPortName   (const CnString       &v) { return v; }
    inline static int32_t  toCfgBaudRate         (int32_t               v) { return v; }
    inline static int8_t   toCfgDataBits         (int8_t                v) { return v; }
    inline static CnString toCfgParity           (Modbus::Parity        v) { return Cn::toString(v); }
    inline static CnString toCfgStopBits         (Modbus::StopBits      v) { return Cn::toString(v); }
    inline static CnString toCfgFlowControl      (Modbus::FlowControl   v) { return Cn::toString(v); }
    inline static uint32_t toCfgTimeoutFirstByte (uint32_t              v) { return v; }
    inline static uint32_t toCfgTimeoutInterByte (uint32_t              v) { return v; }
    inline static CnString toCfgLogFlags         (Cn::LogFlags          v) { return Cn::toString(v); }
    inline static CnString toCfgLogOutput        (const CnString       &v) { return v; }
    inline static CnString toCfgLogFileFormat    (const CnString       &v) { return v; }
    inline static CnString toCfgLogFileTimeformat(const CnString       &v) { return v; }
    inline static CnString toCfgLogFilePath      (const CnString       &v) { return v; }
    inline static int      toCfgLogFileMaxCount  (int                   v) { return v; }
    inline static int      toCfgLogFileMaxSize   (int                   v) { return v; }

public: // convertion function from config values to inner values
    inline static CnString             toInnerName                    (const CnString &v) { return v; }
    inline static bool                 toInnerEnable                  (bool            v) { return v; }
    inline static Modbus::ProtocolType toInnerTypeEnum                (const CnString &v) { return Cn::toProtocolType(v); }
    inline static CnString             toInnerHost                    (const CnString &v) { return v; }
    inline static uint16_t             toInnerPort                    (uint16_t        v) { return v; }
    inline static uint32_t             toInnerTimeoutMillisec         (uint32_t        v) { return v; }
    inline static CnString             toInnerSerialPortName          (const CnString &v) { return v; }
    inline static int32_t              toInnerBaudRate                (int32_t         v) { return v; }
    inline static int8_t               toInnerDataBits                (int8_t          v) { return v; }
    inline static Modbus::Parity       toInnerParityEnum              (const CnString &v) { return Cn::toParity     (v); }
    inline static Modbus::StopBits     toInnerStopBitsEnum            (const CnString &v) { return Cn::toStopBits   (v); }
    inline static Modbus::FlowControl  toInnerFlowControlEnum         (const CnString &v) { return Cn::toFlowControl(v); }
    inline static uint32_t             toInnerTimeoutFirstByteMillisec(uint32_t        v) { return v; }
    inline static uint32_t             toInnerTimeoutInterByteMillisec(uint32_t        v) { return v; }
    inline static Cn::LogFlags         toInnerLogFlags                (const CnString &v) { return Cn::toLogFlags(v); }
    inline static CnString             toInnerLogOutput               (const CnString &v) { return v; }
    inline static CnString             toInnerLogFileFormat           (const CnString &v) { return v; }
    inline static CnString             toInnerLogFileTimeformat       (const CnString &v) { return v; }
    inline static CnString             toInnerLogFilePath             (const CnString &v) { return v; }
    inline static int                  toInnerLogFileMaxCount         (int             v) { return v; }
    inline static int                  toInnerLogFileMaxSize          (int             v) { return v; }

public: // static Config getters
    static CnString getCfgName             (const Cn::Config& config, bool *ok = nullptr);
    static bool     getCfgEnable           (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgType             (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgHost             (const Cn::Config& config, bool *ok = nullptr);
    static uint16_t getCfgPort             (const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgTimeout          (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgSerialPortName   (const Cn::Config& config, bool *ok = nullptr);
    static int32_t  getCfgBaudRate         (const Cn::Config& config, bool *ok = nullptr);
    static int8_t   getCfgDataBits         (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgParity           (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgStopBits         (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgFlowControl      (const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgTimeoutFirstByte (const Cn::Config& config, bool *ok = nullptr);
    static uint32_t getCfgTimeoutInterByte (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFlags         (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogOutput        (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFileFormat    (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFileTimeformat(const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFilePath      (const Cn::Config& config, bool *ok = nullptr);
    static int      getCfgLogFileMaxCount  (const Cn::Config& config, bool *ok = nullptr);
    static int      getCfgLogFileMaxSize   (const Cn::Config& config, bool *ok = nullptr);

public: // static Inner getters
    inline static CnString             getInnerName                    (const Cn::Config& config, bool *ok = nullptr) { return toInnerName                    (getCfgName             (config, ok)); }
    inline static bool                 getInnerEnable                  (const Cn::Config& config, bool *ok = nullptr) { return toInnerEnable                  (getCfgEnable           (config, ok)); }
    inline static Modbus::ProtocolType getInnerTypeEnum                (const Cn::Config& config, bool *ok = nullptr) { return toInnerTypeEnum                (getCfgType             (config, ok)); }
    inline static CnString             getInnerHost                    (const Cn::Config& config, bool *ok = nullptr) { return toInnerHost                    (getCfgHost             (config, ok)); }
    inline static uint16_t             getInnerPort                    (const Cn::Config& config, bool *ok = nullptr) { return toInnerPort                    (getCfgPort             (config, ok)); }
    inline static uint32_t             getInnerTimeoutMillisec         (const Cn::Config& config, bool *ok = nullptr) { return toInnerTimeoutMillisec         (getCfgTimeout          (config, ok)); }
    inline static CnString             getInnerSerialPortName          (const Cn::Config& config, bool *ok = nullptr) { return toInnerSerialPortName          (getCfgSerialPortName   (config, ok)); }
    inline static int32_t              getInnerBaudRate                (const Cn::Config& config, bool *ok = nullptr) { return toInnerBaudRate                (getCfgBaudRate         (config, ok)); }
    inline static int8_t               getInnerDataBits                (const Cn::Config& config, bool *ok = nullptr) { return toInnerDataBits                (getCfgDataBits         (config, ok)); }
    inline static Modbus::Parity       getInnerParityEnum              (const Cn::Config& config, bool *ok = nullptr) { return toInnerParityEnum              (getCfgParity           (config, ok)); }
    inline static Modbus::StopBits     getInnerStopBitsEnum            (const Cn::Config& config, bool *ok = nullptr) { return toInnerStopBitsEnum            (getCfgStopBits         (config, ok)); }
    inline static Modbus::FlowControl  getInnerFlowControlEnum         (const Cn::Config& config, bool *ok = nullptr) { return toInnerFlowControlEnum         (getCfgFlowControl      (config, ok)); }
    inline static uint32_t             getInnerTimeoutFirstByteMillisec(const Cn::Config& config, bool *ok = nullptr) { return toInnerTimeoutFirstByteMillisec(getCfgTimeoutFirstByte (config, ok)); }
    inline static uint32_t             getInnerTimeoutInterByteMillisec(const Cn::Config& config, bool *ok = nullptr) { return toInnerTimeoutInterByteMillisec(getCfgTimeoutInterByte (config, ok)); }
    inline static Cn::LogFlags      getInnerLogFlags                   (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFlags                (getCfgLogFlags         (config, ok)); }
    inline static CnString             getInnerLogOutput               (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogOutput               (getCfgLogOutput        (config, ok)); }
    inline static CnString             getInnerLogFileFormat           (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileFormat           (getCfgLogFileFormat    (config, ok)); }
    inline static CnString             getInnerLogFileTimeformat       (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileTimeformat       (getCfgLogFileTimeformat(config, ok)); }
    inline static CnString             getInnerLogFilePath             (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFilePath             (getCfgLogFilePath      (config, ok)); }
    inline static int                  getInnerLogFileMaxCount         (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileMaxCount         (getCfgLogFileMaxCount  (config, ok)); }
    inline static int                  getInnerLogFileMaxSize          (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileMaxSize          (getCfgLogFileMaxSize   (config, ok)); }

public:
    CnPortConfig();

public:
    Cn::Config config() const;
    void setConfig(const Cn::Config& config);

public: // Config
    inline CnString CfgName             () const { CnCriticalSectionLocker _(&cs); return toCfgName             (Name                    ); } ///< Configuration. Name of this port
    inline bool     CfgEnable           () const { CnCriticalSectionLocker _(&cs); return toCfgEnable           (Enable                  ); } ///< Configuration. Availability for runtime execution sequence (1-on, 0-off)
    inline CnString CfgType             () const { CnCriticalSectionLocker _(&cs); return toCfgType             (TypeEnum                ); } ///< Configuration. Modbus protocol type
    inline CnString CfgHost             () const { CnCriticalSectionLocker _(&cs); return toCfgHost             (Host                    ); } ///< Configuration. IP address or DNS name of the remote device
    inline uint16_t CfgPort             () const { CnCriticalSectionLocker _(&cs); return toCfgPort             (Port                    ); } ///< Configuration. TCP port number (standard Modbus TCP port 502)
    inline uint32_t CfgTimeout          () const { CnCriticalSectionLocker _(&cs); return toCfgTimeout          (TimeoutMillisec         ); } ///< Configuration. Connection timeout (milliseconds)
    inline CnString CfgSerialPortName   () const { CnCriticalSectionLocker _(&cs); return toCfgSerialPortName   (SerialPortName          ); } ///< Configuration. Serial port name
    inline int32_t  CfgBaudRate         () const { CnCriticalSectionLocker _(&cs); return toCfgBaudRate         (BaudRate                ); } ///< Configuration. Serial port baud rate
    inline int8_t   CfgDataBits         () const { CnCriticalSectionLocker _(&cs); return toCfgDataBits         (DataBits                ); } ///< Configuration. Serial port data bits
    inline CnString CfgParity           () const { CnCriticalSectionLocker _(&cs); return toCfgParity           (ParityEnum              ); } ///< Configuration. Serial port parity
    inline CnString CfgStopBits         () const { CnCriticalSectionLocker _(&cs); return toCfgStopBits         (StopBitsEnum            ); } ///< Configuration. Serial port stop bits
    inline CnString CfgFlowControl      () const { CnCriticalSectionLocker _(&cs); return toCfgFlowControl      (FlowControlEnum         ); } ///< Configuration. Serial port flow control
    inline uint32_t CfgTimeoutFirstByte () const { CnCriticalSectionLocker _(&cs); return toCfgTimeoutFirstByte (TimeoutFirstByteMillisec); } ///< Configuration. Timeout waiting for the first byte (ms)
    inline uint32_t CfgTimeoutInterByte () const { CnCriticalSectionLocker _(&cs); return toCfgTimeoutInterByte (TimeoutInterByteMillisec); } ///< Configuration. Inter-byte timeout (ms)
    inline CnString CfgLogFlags         () const { CnCriticalSectionLocker _(&cs); return toCfgLogFlags         (LogFlags                ); } ///< Configuration. Logging flags
    inline CnString CfgLogOutput        () const { CnCriticalSectionLocker _(&cs); return toCfgLogOutput        (LogOutput               ); } ///< Configuration. Logging output target
    inline CnString CfgLogFileFormat    () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileFormat    (LogFileFormat           ); } ///< Configuration. Log file message format
    inline CnString CfgLogFileTimeformat() const { CnCriticalSectionLocker _(&cs); return toCfgLogFileTimeformat(LogFileTimeformat       ); } ///< Configuration. Log file time format
    inline CnString CfgLogFilePath      () const { CnCriticalSectionLocker _(&cs); return toCfgLogFilePath      (LogFilePath             ); } ///< Configuration. Log file path
    inline int      CfgLogFileMaxCount  () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileMaxCount  (LogFileMaxCount         ); } ///< Configuration. Max number of rotated log files
    inline int      CfgLogFileMaxSize   () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileMaxSize   (LogFileMaxSize          ); } ///< Configuration. Max size of a single log file

public:
    inline void setCfgName             (const CnString &v) { setInnerName                    (toInnerName                    (v)); }
    inline void setCfgEnable           (bool            v) { setInnerEnable                  (toInnerEnable                  (v)); }
    inline void setCfgType             (const CnString &v) { setInnerTypeEnum                (toInnerTypeEnum                (v)); }
    inline void setCfgHost             (const CnString &v) { setInnerHost                    (toInnerHost                    (v)); }
    inline void setCfgPort             (uint16_t        v) { setInnerPort                    (toInnerPort                    (v)); }
    inline void setCfgTimeout          (uint32_t        v) { setInnerTimeoutMillisec         (toInnerTimeoutMillisec         (v)); }
    inline void setCfgSerialPortName   (const CnString &v) { setInnerSerialPortName          (toInnerSerialPortName          (v)); }
    inline void setCfgBaudRate         (int32_t         v) { setInnerBaudRate                (toInnerBaudRate                (v)); }
    inline void setCfgDataBits         (int8_t          v) { setInnerDataBits                (toInnerDataBits                (v)); }
    inline void setCfgParity           (const CnString &v) { setInnerParityEnum              (toInnerParityEnum              (v)); }
    inline void setCfgStopBits         (const CnString &v) { setInnerStopBitsEnum            (toInnerStopBitsEnum            (v)); }
    inline void setCfgFlowControl      (const CnString &v) { setInnerFlowControlEnum         (toInnerFlowControlEnum         (v)); }
    inline void setCfgTimeoutFirstByte (uint32_t        v) { setInnerTimeoutFirstByteMillisec(toInnerTimeoutFirstByteMillisec(v)); }
    inline void setCfgTimeoutInterByte (uint32_t        v) { setInnerTimeoutInterByteMillisec(toInnerTimeoutInterByteMillisec(v)); }
    inline void setCfgLogFlags         (const CnString& v) { setInnerLogFlags                (toInnerLogFlags                (v)); } 
    inline void setCfgLogOutput        (const CnString& v) { setInnerLogOutput               (toInnerLogOutput               (v)); } 
    inline void setCfgLogFileFormat    (const CnString& v) { setInnerLogFileFormat           (toInnerLogFileFormat           (v)); } 
    inline void setCfgLogFileTimeformat(const CnString& v) { setInnerLogFileTimeformat       (toInnerLogFileTimeformat       (v)); } 
    inline void setCfgLogFilePath      (const CnString& v) { setInnerLogFilePath             (toInnerLogFilePath             (v)); } 
    inline void setCfgLogFileMaxCount  (int             v) { setInnerLogFileMaxCount         (toInnerLogFileMaxCount         (v)); } 
    inline void setCfgLogFileMaxSize   (int             v) { setInnerLogFileMaxSize          (toInnerLogFileMaxSize          (v)); } 

public:
    inline CnString             innerName                    () const { CnCriticalSectionLocker _(&cs); return Name                    ; } 
    inline bool                 innerEnable                  () const { CnCriticalSectionLocker _(&cs); return Enable                  ; } 
    inline Modbus::ProtocolType innerTypeEnum                () const { CnCriticalSectionLocker _(&cs); return TypeEnum                ; } 
    inline CnString             innerHost                    () const { CnCriticalSectionLocker _(&cs); return Host                    ; } 
    inline uint16_t             innerPort                    () const { CnCriticalSectionLocker _(&cs); return Port                    ; } 
    inline uint32_t             innerTimeoutMillisec         () const { CnCriticalSectionLocker _(&cs); return TimeoutMillisec         ; } 
    inline CnString             innerSerialPortName          () const { CnCriticalSectionLocker _(&cs); return SerialPortName          ; } 
    inline int32_t              innerBaudRate                () const { CnCriticalSectionLocker _(&cs); return BaudRate                ; } 
    inline int8_t               innerDataBits                () const { CnCriticalSectionLocker _(&cs); return DataBits                ; } 
    inline Modbus::Parity       innerParityEnum              () const { CnCriticalSectionLocker _(&cs); return ParityEnum              ; } 
    inline Modbus::StopBits     innerStopBitsEnum            () const { CnCriticalSectionLocker _(&cs); return StopBitsEnum            ; } 
    inline Modbus::FlowControl  innerFlowControlEnum         () const { CnCriticalSectionLocker _(&cs); return FlowControlEnum         ; } 
    inline uint32_t             innerTimeoutFirstByteMillisec() const { CnCriticalSectionLocker _(&cs); return TimeoutFirstByteMillisec; } 
    inline uint32_t             innerTimeoutInterByteMillisec() const { CnCriticalSectionLocker _(&cs); return TimeoutInterByteMillisec; } 
    inline Cn::LogFlags      innerLogFlags                   () const { CnCriticalSectionLocker _(&cs); return LogFlags                ; }
    inline CnString             innerLogOutput               () const { CnCriticalSectionLocker _(&cs); return LogOutput               ; }
    inline CnString             innerLogFileFormat           () const { CnCriticalSectionLocker _(&cs); return LogFileFormat           ; }
    inline CnString             innerLogFileTimeformat       () const { CnCriticalSectionLocker _(&cs); return LogFileTimeformat       ; }
    inline CnString             innerLogFilePath             () const { CnCriticalSectionLocker _(&cs); return LogFilePath             ; }
    inline int                  innerLogFileMaxCount         () const { CnCriticalSectionLocker _(&cs); return LogFileMaxCount         ; }
    inline int                  innerLogFileMaxSize          () const { CnCriticalSectionLocker _(&cs); return LogFileMaxSize          ; }

public:
    inline void setInnerName                    (const CnString       &v) { CnCriticalSectionLocker _(&cs); Name                     = v; }
    inline void setInnerEnable                  (bool                  v) { CnCriticalSectionLocker _(&cs); Enable                   = v; }
    inline void setInnerTypeEnum                (Modbus::ProtocolType  v) { CnCriticalSectionLocker _(&cs); TypeEnum                 = v; }
    inline void setInnerHost                    (const CnString       &v) { CnCriticalSectionLocker _(&cs); Host                     = v; }
    inline void setInnerPort                    (uint16_t              v) { CnCriticalSectionLocker _(&cs); Port                     = v; }
    inline void setInnerTimeoutMillisec         (uint32_t              v) { CnCriticalSectionLocker _(&cs); TimeoutMillisec          = v; }
    inline void setInnerSerialPortName          (const CnString       &v) { CnCriticalSectionLocker _(&cs); SerialPortName           = v; }
    inline void setInnerBaudRate                (int32_t               v) { CnCriticalSectionLocker _(&cs); BaudRate                 = v; }
    inline void setInnerDataBits                (int8_t                v) { CnCriticalSectionLocker _(&cs); DataBits                 = v; }
    inline void setInnerParityEnum              (Modbus::Parity        v) { CnCriticalSectionLocker _(&cs); ParityEnum               = v; }
    inline void setInnerStopBitsEnum            (Modbus::StopBits      v) { CnCriticalSectionLocker _(&cs); StopBitsEnum             = v; }
    inline void setInnerFlowControlEnum         (Modbus::FlowControl   v) { CnCriticalSectionLocker _(&cs); FlowControlEnum          = v; }
    inline void setInnerTimeoutFirstByteMillisec(uint32_t              v) { CnCriticalSectionLocker _(&cs); TimeoutFirstByteMillisec = v; }
    inline void setInnerTimeoutInterByteMillisec(uint32_t              v) { CnCriticalSectionLocker _(&cs); TimeoutInterByteMillisec = v; }
    inline void setInnerLogFlags                (Cn::LogFlags          v) { CnCriticalSectionLocker _(&cs); LogFlags                 = v; } 
    inline void setInnerLogOutput               (const CnString       &v) { CnCriticalSectionLocker _(&cs); LogOutput                = v; } 
    inline void setInnerLogFileFormat           (const CnString       &v) { CnCriticalSectionLocker _(&cs); LogFileFormat            = v; } 
    inline void setInnerLogFileTimeformat       (const CnString       &v) { CnCriticalSectionLocker _(&cs); LogFileTimeformat        = v; } 
    inline void setInnerLogFilePath             (const CnString       &v) { CnCriticalSectionLocker _(&cs); LogFilePath              = v; } 
    inline void setInnerLogFileMaxCount         (int                   v) { CnCriticalSectionLocker _(&cs); LogFileMaxCount          = v; } 
    inline void setInnerLogFileMaxSize          (int                   v) { CnCriticalSectionLocker _(&cs); LogFileMaxSize           = v; } 

private:
    mutable CnCriticalSection cs;
    
private: // Inner Config
    CnString             Name                    ;
    bool                 Enable                  ;
    Modbus::ProtocolType TypeEnum                ;
    CnString             Host                    ;
    uint16_t             Port                    ;
    uint32_t             TimeoutMillisec         ;
    CnString             SerialPortName          ;
    int32_t              BaudRate                ;
    int8_t               DataBits                ;
    Modbus::Parity       ParityEnum              ;
    Modbus::StopBits     StopBitsEnum            ;
    Modbus::FlowControl  FlowControlEnum         ;
    uint32_t             TimeoutFirstByteMillisec;
    uint32_t             TimeoutInterByteMillisec;
    Cn::LogFlags         LogFlags                ;
    CnString             LogOutput               ;
    CnString             LogFileFormat           ;
    CnString             LogFileTimeformat       ;
    CnString             LogFilePath             ;
    int                  LogFileMaxCount         ;
    int                  LogFileMaxSize          ;
};

#endif // CNPORTCONFIG_H