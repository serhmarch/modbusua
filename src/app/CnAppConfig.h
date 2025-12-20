#ifndef CNSYSTEMCONFIG_H
#define CNSYSTEMCONFIG_H

#include <CnGlobal.h>

class CnApp;

class CnAppConfig
{
public:
    /*! \brief Defines constant configuration key names.
        \details Keys are used in the associative configuration map.
    */
    struct Strings
    {
        const CnString LogFlags            ;     
        const CnString LogOutput           ;     
        const CnString LogDefaultFormat    ;
        const CnString LogDefaultTimeformat;
        const CnString LogSystemFormat     ;
        const CnString LogSystemTimeformat ;
        const CnString LogConsoleFormat    ;
        const CnString LogConsoleTimeformat;
        const CnString LogFileFormat       ;     
        const CnString LogFileTimeformat   ;     
        const CnString LogFilePath         ;     
        const CnString LogFileMaxCount     ;     
        const CnString LogFileMaxSize      ;

        /// \details Constructs the key name container.
        Strings();

        /// \details Returns a reference to the global key names singleton.
        static const Strings& instance();
    };

    /*! \brief Defines constant default configuration values.
        \details Provides defaults for logging flags, formats and file settings.
     */
    struct Defaults
    {
        const CnString LogFlags            ;
        const CnString LogOutput           ;
        const CnString LogDefaultFormat    ;
        const CnString LogDefaultTimeformat;
        const CnString LogSystemFormat     ;
        const CnString LogSystemTimeformat ;
        const CnString LogConsoleFormat    ;
        const CnString LogConsoleTimeformat;
        const CnString LogFileFormat       ;
        const CnString LogFileTimeformat   ;
        const CnString LogFilePath         ;
        const int      LogFileMaxCount     ;
        const int      LogFileMaxSize      ;

        /// \details Constructs the defaults container.
        Defaults();

        /// \details Returns a reference to the global defaults singleton.
        static const Defaults& instance();
    };

public: // convertion function from inner values to config values
    inline static CnString toCfgLogFlags            (Cn::LogFlags    v) { return Cn::toString(v); }
    inline static CnString toCfgLogOutput           (const CnString &v) { return v; }
    inline static CnString toCfgLogDefaultFormat    (const CnString &v) { return v; }
    inline static CnString toCfgLogDefaultTimeformat(const CnString &v) { return v; }
    inline static CnString toCfgLogSystemFormat     (const CnString &v) { return v; }
    inline static CnString toCfgLogSystemTimeformat (const CnString &v) { return v; }
    inline static CnString toCfgLogConsoleFormat    (const CnString &v) { return v; }
    inline static CnString toCfgLogConsoleTimeformat(const CnString &v) { return v; }
    inline static CnString toCfgLogFileFormat       (const CnString &v) { return v; }
    inline static CnString toCfgLogFileTimeformat   (const CnString &v) { return v; }
    inline static CnString toCfgLogFilePath         (const CnString &v) { return v; }
    inline static int      toCfgLogFileMaxCount     (int             v) { return v; }
    inline static int      toCfgLogFileMaxSize      (int             v) { return v; }

public: // convertion function from config values to inner values
    inline static Cn::LogFlags toInnerLogFlags            (const CnString &v) { return Cn::toLogFlags(v); }
    inline static CnString     toInnerLogOutput           (const CnString &v) { return v; }
    inline static CnString     toInnerLogDefaultFormat    (const CnString &v) { return v; }
    inline static CnString     toInnerLogDefaultTimeformat(const CnString &v) { return v; }
    inline static CnString     toInnerLogSystemFormat     (const CnString &v) { return v; }
    inline static CnString     toInnerLogSystemTimeformat (const CnString &v) { return v; }
    inline static CnString     toInnerLogConsoleFormat    (const CnString &v) { return v; }
    inline static CnString     toInnerLogConsoleTimeformat(const CnString &v) { return v; }
    inline static CnString     toInnerLogFileFormat       (const CnString &v) { return v; }
    inline static CnString     toInnerLogFileTimeformat   (const CnString &v) { return v; }
    inline static CnString     toInnerLogFilePath         (const CnString &v) { return v; }
    inline static int          toInnerLogFileMaxCount     (int             v) { return v; }
    inline static int          toInnerLogFileMaxSize      (int             v) { return v; }

public: // static Config getters
    static CnString getCfgLogFlags            (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogOutput           (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogDefaultFormat    (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogDefaultTimeformat(const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogSystemFormat     (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogSystemTimeformat (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogConsoleFormat    (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogConsoleTimeformat(const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFileFormat       (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFileTimeformat   (const Cn::Config& config, bool *ok = nullptr);
    static CnString getCfgLogFilePath         (const Cn::Config& config, bool *ok = nullptr);
    static int      getCfgLogFileMaxCount     (const Cn::Config& config, bool *ok = nullptr);
    static int      getCfgLogFileMaxSize      (const Cn::Config& config, bool *ok = nullptr);

public: // static Inner getters
    inline static Cn::LogFlags getInnerLogFlags            (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFlags            (getCfgLogFlags            (config, ok)); }
    inline static CnString     getInnerLogOutput           (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogOutput           (getCfgLogOutput           (config, ok)); }
    inline static CnString     getInnerLogDefaultFormat    (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogDefaultFormat    (getCfgLogDefaultFormat    (config, ok)); }
    inline static CnString     getInnerLogDefaultTimeformat(const Cn::Config& config, bool *ok = nullptr) { return toInnerLogDefaultTimeformat(getCfgLogDefaultTimeformat(config, ok)); }
    inline static CnString     getInnerLogSystemFormat     (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogSystemFormat     (getCfgLogSystemFormat     (config, ok)); }
    inline static CnString     getInnerLogSystemTimeformat (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogSystemTimeformat (getCfgLogSystemTimeformat (config, ok)); }
    inline static CnString     getInnerLogConsoleFormat    (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogConsoleFormat    (getCfgLogConsoleFormat    (config, ok)); }
    inline static CnString     getInnerLogConsoleTimeformat(const Cn::Config& config, bool *ok = nullptr) { return toInnerLogConsoleTimeformat(getCfgLogConsoleTimeformat(config, ok)); }
    inline static CnString     getInnerLogFileFormat       (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileFormat       (getCfgLogFileFormat       (config, ok)); }
    inline static CnString     getInnerLogFileTimeformat   (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileTimeformat   (getCfgLogFileTimeformat   (config, ok)); }
    inline static CnString     getInnerLogFilePath         (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFilePath         (getCfgLogFilePath         (config, ok)); }
    inline static int          getInnerLogFileMaxCount     (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileMaxCount     (getCfgLogFileMaxCount     (config, ok)); }
    inline static int          getInnerLogFileMaxSize      (const Cn::Config& config, bool *ok = nullptr) { return toInnerLogFileMaxSize      (getCfgLogFileMaxSize      (config, ok)); }

public:
    CnAppConfig();

public:
    Cn::Config config() const;
    void setConfig(const Cn::Config& config);

public: // Config
    inline CnString CfgLogFlags            () const { CnCriticalSectionLocker _(&cs); return toCfgLogFlags            (LogFlags            ); }
    inline CnString CfgLogOutput           () const { CnCriticalSectionLocker _(&cs); return toCfgLogOutput           (LogOutput           ); }
    inline CnString CfgLogDefaultFormat    () const { CnCriticalSectionLocker _(&cs); return toCfgLogDefaultFormat    (LogDefaultFormat    ); }
    inline CnString CfgLogDefaultTimeformat() const { CnCriticalSectionLocker _(&cs); return toCfgLogDefaultTimeformat(LogDefaultTimeformat); }
    inline CnString CfgLogSystemFormat     () const { CnCriticalSectionLocker _(&cs); return toCfgLogSystemFormat     (LogSystemFormat     ); }
    inline CnString CfgLogSystemTimeformat () const { CnCriticalSectionLocker _(&cs); return toCfgLogSystemTimeformat (LogSystemTimeformat ); }
    inline CnString CfgLogConsoleFormat    () const { CnCriticalSectionLocker _(&cs); return toCfgLogConsoleFormat    (LogConsoleFormat    ); }
    inline CnString CfgLogConsoleTimeformat() const { CnCriticalSectionLocker _(&cs); return toCfgLogConsoleTimeformat(LogConsoleTimeformat); }
    inline CnString CfgLogFileFormat       () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileFormat       (LogFileFormat       ); }
    inline CnString CfgLogFileTimeformat   () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileTimeformat   (LogFileTimeformat   ); }
    inline CnString CfgLogFilePath         () const { CnCriticalSectionLocker _(&cs); return toCfgLogFilePath         (LogFilePath         ); }
    inline int      CfgLogFileMaxCount     () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileMaxCount     (LogFileMaxCount     ); }
    inline int      CfgLogFileMaxSize      () const { CnCriticalSectionLocker _(&cs); return toCfgLogFileMaxSize      (LogFileMaxSize      ); }

public:
    inline void setCfgLogFlags            (const CnString& v) { setInnerLogFlags            (toInnerLogFlags            (v)); } 
    inline void setCfgLogOutput           (const CnString& v) { setInnerLogOutput           (toInnerLogOutput           (v)); } 
    inline void setCfgLogDefaultFormat    (const CnString& v) { setInnerLogDefaultFormat    (toInnerLogDefaultFormat    (v)); } 
    inline void setCfgLogDefaultTimeformat(const CnString& v) { setInnerLogDefaultTimeformat(toInnerLogDefaultTimeformat(v)); } 
    inline void setCfgLogSystemFormat     (const CnString& v) { setInnerLogSystemFormat     (toInnerLogSystemFormat     (v)); } 
    inline void setCfgLogSystemTimeformat (const CnString& v) { setInnerLogSystemTimeformat (toInnerLogSystemTimeformat (v)); } 
    inline void setCfgLogConsoleFormat    (const CnString& v) { setInnerLogConsoleFormat    (toInnerLogConsoleFormat    (v)); } 
    inline void setCfgLogConsoleTimeformat(const CnString& v) { setInnerLogConsoleTimeformat(toInnerLogConsoleTimeformat(v)); } 
    inline void setCfgLogFileFormat       (const CnString& v) { setInnerLogFileFormat       (toInnerLogFileFormat       (v)); } 
    inline void setCfgLogFileTimeformat   (const CnString& v) { setInnerLogFileTimeformat   (toInnerLogFileTimeformat   (v)); } 
    inline void setCfgLogFilePath         (const CnString& v) { setInnerLogFilePath         (toInnerLogFilePath         (v)); } 
    inline void setCfgLogFileMaxCount     (int             v) { setInnerLogFileMaxCount     (toInnerLogFileMaxCount     (v)); } 
    inline void setCfgLogFileMaxSize      (int             v) { setInnerLogFileMaxSize      (toInnerLogFileMaxSize      (v)); } 

public:
    inline Cn::LogFlags innerLogFlags            () const { CnCriticalSectionLocker _(&cs); return LogFlags            ; }
    inline CnString     innerLogOutput           () const { CnCriticalSectionLocker _(&cs); return LogOutput           ; }
    inline CnString     innerLogDefaultFormat    () const { CnCriticalSectionLocker _(&cs); return LogDefaultFormat    ; }
    inline CnString     innerLogDefaultTimeformat() const { CnCriticalSectionLocker _(&cs); return LogDefaultTimeformat; }
    inline CnString     innerLogSystemFormat     () const { CnCriticalSectionLocker _(&cs); return LogSystemFormat     ; }
    inline CnString     innerLogSystemTimeformat () const { CnCriticalSectionLocker _(&cs); return LogSystemTimeformat ; }
    inline CnString     innerLogConsoleFormat    () const { CnCriticalSectionLocker _(&cs); return LogConsoleFormat    ; }
    inline CnString     innerLogConsoleTimeformat() const { CnCriticalSectionLocker _(&cs); return LogConsoleTimeformat; }
    inline CnString     innerLogFileFormat       () const { CnCriticalSectionLocker _(&cs); return LogFileFormat       ; }
    inline CnString     innerLogFileTimeformat   () const { CnCriticalSectionLocker _(&cs); return LogFileTimeformat   ; }
    inline CnString     innerLogFilePath         () const { CnCriticalSectionLocker _(&cs); return LogFilePath         ; }
    inline int          innerLogFileMaxCount     () const { CnCriticalSectionLocker _(&cs); return LogFileMaxCount     ; }
    inline int          innerLogFileMaxSize      () const { CnCriticalSectionLocker _(&cs); return LogFileMaxSize      ; }

public:
    inline void setInnerLogFlags            (Cn::LogFlags    v) { CnCriticalSectionLocker _(&cs); LogFlags             = v; } 
    inline void setInnerLogOutput           (const CnString& v) { CnCriticalSectionLocker _(&cs); LogOutput            = v; } 
    inline void setInnerLogDefaultFormat    (const CnString& v) { CnCriticalSectionLocker _(&cs); LogDefaultFormat     = v; } 
    inline void setInnerLogDefaultTimeformat(const CnString& v) { CnCriticalSectionLocker _(&cs); LogDefaultTimeformat = v; } 
    inline void setInnerLogSystemFormat     (const CnString& v) { CnCriticalSectionLocker _(&cs); LogSystemFormat      = v; } 
    inline void setInnerLogSystemTimeformat (const CnString& v) { CnCriticalSectionLocker _(&cs); LogSystemTimeformat  = v; } 
    inline void setInnerLogConsoleFormat    (const CnString& v) { CnCriticalSectionLocker _(&cs); LogConsoleFormat     = v; } 
    inline void setInnerLogConsoleTimeformat(const CnString& v) { CnCriticalSectionLocker _(&cs); LogConsoleTimeformat = v; } 
    inline void setInnerLogFileFormat       (const CnString& v) { CnCriticalSectionLocker _(&cs); LogFileFormat        = v; } 
    inline void setInnerLogFileTimeformat   (const CnString& v) { CnCriticalSectionLocker _(&cs); LogFileTimeformat    = v; } 
    inline void setInnerLogFilePath         (const CnString& v) { CnCriticalSectionLocker _(&cs); LogFilePath          = v; } 
    inline void setInnerLogFileMaxCount     (int             v) { CnCriticalSectionLocker _(&cs); LogFileMaxCount      = v; } 
    inline void setInnerLogFileMaxSize      (int             v) { CnCriticalSectionLocker _(&cs); LogFileMaxSize       = v; } 

private:
    mutable CnCriticalSection cs;
    
private: // Inner Config
    Cn::LogFlags LogFlags            ;
    CnString     LogOutput           ;
    CnString     LogDefaultFormat    ;
    CnString     LogDefaultTimeformat;
    CnString     LogSystemFormat     ;
    CnString     LogSystemTimeformat ;
    CnString     LogConsoleFormat    ;
    CnString     LogConsoleTimeformat;
    CnString     LogFileFormat       ;
    CnString     LogFileTimeformat   ;
    CnString     LogFilePath         ;
    int          LogFileMaxCount     ;
    int          LogFileMaxSize      ;
};

#endif // CNSYSTEMCONFIG_H