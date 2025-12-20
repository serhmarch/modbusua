/*!
 * \file   CnLog.h
 * \brief  
 * 
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNLOGGER_H
#define CNLOGGER_H

#include <CnGlobal.h>
#include <CnObject.h>

/*! \brief Base class for all loggers.
    \details Provides message formatting utilities and the abstract
    delivery interface `logMessage()` used by derived loggers.
*/
class CnLogger : public CnObject
{
public:
    /*! \brief Configuration key names for logger settings.
        \details Keys for format and time format in configuration maps.
    */
    struct Strings
    {
        /// \brief Token placeholder for message source.
        CnString tokenSource          ;
        /// \brief Token placeholder for message time.
        CnString tokenTime            ;
        /// \brief Token placeholder for message category.
        CnString tokenCategory        ;
        /// \brief Token placeholder for message text.
        CnString tokenText            ;
        /// \brief Config key: default message format.
        CnString cfg_defaultFormat    ;
        /// \brief Config key: default time format.
        CnString cfg_defaultTimeformat;
        /// \brief Config key: active message format.
        CnString cfg_format           ;
        /// \brief Config key: active time format.
        CnString cfg_timeformat       ;
        /// \details Constructs the key names container.
        Strings();
        /// \details Returns the singleton instance with key names.
        static const Strings& instance();
   };

   /*! \brief Default logger configuration values.
       \details Provides default format and time format.
   */
   struct Defaults
   {
       /// \brief Default message format string.
       CnString format    ;
       /// \brief Default time format string.
       CnString timeformat;
       /// \details Constructs the defaults container.
       Defaults();
       /// \details Returns the singleton with default values.
       static const Defaults& instance();
   };

   /*! \brief A parsed formatting token.
       \details Literal text or a placeholder (source, time, category, text).
   */
   struct Token
    {
        /// \brief Token kind.
        enum Type { Null, Literal, Source, Time, Category, Text };
        /// \details Constructs a token of the given type.
        Token(Type type = Null) : type(type) {}
        /// \brief Token type.
        Type type;
        /// \brief Literal text when `type==Literal`.
        CnString str;
    };

    /*! \brief A structured log message.
        \details Timestamp, category and text payload to be formatted.
    */
    struct Message
    {
        /// \brief Message timestamp.
        CnDateTime time;
        /// \brief Message category.
        Cn::LogFlag category;
        /// \brief Message text.
        CnString text;
    };

public:
    /// \details Parses a format string into a list of tokens.
    static CnList<Token> parse(const CnString &str);
    /// \details Converts a token to string using `source`, `timeformat` and `msg`.
    static CnString toString(const Token &token, const CnString &source, const CnString &timeformat, const Message &msg);

public:
    /// \details Constructs the base logger.
    CnLogger();
    /// \details Virtual destructor.
    virtual ~CnLogger();

public:
    /// \details Returns the current formatting tokens.
    inline CnList<Token> tokens() const { CnCriticalSectionLocker _(&m_cs); return m_tokens; }
    /// \details Replaces the current formatting tokens.
    inline void setTokens(const CnList<Token> &tokens) { CnCriticalSectionLocker _(&m_cs); m_tokens = tokens; }
    /// \details Parses and sets the message format string.
    void setFormat(const CnString &format);
    /// \details Returns the current time format string.
    inline CnString timeformat() const { CnCriticalSectionLocker _(&m_cs); return m_timeformat; }
    /// \details Sets the time format string.
    inline void setTimeformat(const CnString &timeformat) { CnCriticalSectionLocker _(&m_cs); m_timeformat = timeformat; }
    /// \details Formats a message using current tokens and formats.
    CnString toString(const CnString &source, const Message &msg) const;
    /// \details Applies configuration values to this logger.
    void setConfig(const Cn::Config &config);
    /// \details Resets configuration to defaults.
    void resetConfig();

public:
    /// \details Delivers a formatted message to the concrete sink.
    virtual void logMessage(const CnString &source, const Message &msg) = 0;

protected:
    virtual void setConfigInner(const Cn::Config &config);

protected:
    mutable CnCriticalSection m_cs;
    CnString m_defaultFormat    ;
    CnString m_defaultTimeformat;
    CnList<Token> m_tokens;
    CnString m_timeformat;
    bool m_isNeedResetConfig;
};

typedef CnSharedPointer<CnLogger> CnLoggerPtr;

#endif // CNLOGGER_H