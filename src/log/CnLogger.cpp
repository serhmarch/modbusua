#include "CnLogger.h"

#include <CnLog.h>
#include <app/CnApp.h>

CnLogger::Strings::Strings() : 
    tokenSource          (CnSTR("%src")),
    tokenTime            (CnSTR("%time")), 
    tokenCategory        (CnSTR("%cat")), 
    tokenText            (CnSTR("%text")),
    cfg_defaultFormat    (CnSTR("default.format")),
    cfg_defaultTimeformat(CnSTR("default.timeformat")),
    cfg_format           (CnSTR("format")),
    cfg_timeformat       (CnSTR("timeformat"))
{
}

const CnLogger::Strings &CnLogger::Strings::instance()
{
    static Strings s;
    return s;
}

CnLogger::Defaults::Defaults() : 
    format    (CnSTR("[%time] %src. %text")),
    timeformat(CnSTR("%Y-%M-%D %h:%m:%s.%f"))
{
}

const CnLogger::Defaults &CnLogger::Defaults::instance()
{
    static Defaults s;
    return s;
}

CnList<CnLogger::Token> CnLogger::parse(const CnString &str)
{
    const Strings &s = Strings::instance();
    size_t pos = 0, len = str.length();
    Token token;
    CnList<Token> res;
    while (pos < len) 
    {
        CnChar c = str[pos];
        if (c == CnCHR('%'))
        {
            if ((pos + s.tokenSource.size()) <= len && str.substr(pos, s.tokenSource.size()) == s.tokenSource) 
            {
                if (token.type == Token::Literal) 
                    res.push_back(token);
                token = Token(Token::Source);
                res.push_back(token);
                pos += s.tokenSource.size();
                continue;
            } 
            else if ((pos + s.tokenTime.size()) <= len && str.substr(pos, s.tokenTime.size()) == s.tokenTime) 
            {
                if (token.type == Token::Literal) 
                    res.push_back(token);
                token = Token(Token::Time);
                res.push_back(token);
                pos += s.tokenTime.size();
                continue;
            } 
            else if ((pos + s.tokenCategory.size()) <= len && str.substr(pos, s.tokenCategory.size()) == s.tokenCategory) 
            {
                if (token.type == Token::Literal) 
                    res.push_back(token);
                token = Token(Token::Category);
                res.push_back(token);
                pos += s.tokenCategory.size();
                continue;
            } 
            else if ((pos + s.tokenText.size()) <= len && str.substr(pos, s.tokenText.size()) == s.tokenText) 
            {
                if (token.type == Token::Literal) 
                    res.push_back(token);
                token = Token(Token::Text);
                res.push_back(token);
                pos += s.tokenText.size();
                continue;
            }
        }
        if (token.type == Token::Literal) 
            token.str += c;
        else 
        {
            token = Token(Token::Literal);
            token.str = c;
        }
        pos += 1;
    }
    if (token.type == Token::Literal) 
        res.push_back(token);
    return res;
}

CnString CnLogger::toString(const Token &token,  const CnString &source, const CnString &timeformat, const Message &msg)
{
    switch (token.type) 
    {
    case Token::Time: 
        return msg.time.toString(timeformat);
    case Token::Category: 
        return Cn::toString(msg.category);
    case Token::Text: 
        return msg.text;
    case Token::Source:
        return source;
    case Token::Literal: 
        return token.str;
    default: 
        break;
    }
    return CnString();
}


CnLogger::CnLogger()
{
    const Defaults &d = Defaults::instance();
    m_isNeedResetConfig = true;
    m_defaultFormat     = d.format    ;
    m_defaultTimeformat = d.timeformat;
    m_timeformat        = m_defaultTimeformat;
}

CnLogger::~CnLogger()
{
}

void CnLogger::setFormat(const CnString &format)
{
    CnCriticalSectionLocker _(&m_cs);
    m_tokens = parse(format);
}

CnString CnLogger::toString(const CnString &source, const Message &msg) const
{
    CnCriticalSectionLocker _(&m_cs);
    CnString res;
    for (auto it = m_tokens.begin(); it != m_tokens.end(); ++it) 
        res += toString(*it, source, m_timeformat, msg);
    return res;
}

void CnLogger::setConfig(const Cn::Config &config)
{
    if (m_isNeedResetConfig)
    {
        CnCriticalSectionLocker _(&m_cs);
        setConfigInner(config);
        m_isNeedResetConfig = false;
    }
}

void CnLogger::resetConfig()
{
    CnCriticalSectionLocker _(&m_cs);
    m_isNeedResetConfig = true;
}

void CnLogger::setConfigInner(const Cn::Config &config)
{
    const Strings &s = Strings::instance();

    Cn::Config::const_iterator it;
    Cn::Config::const_iterator end = config.end();

    it = config.find(s.cfg_defaultFormat); 
    if (it != end) 
        m_defaultFormat = it->second.toString();

    it = config.find(s.cfg_defaultTimeformat); 
    if (it != end) 
        m_defaultTimeformat = it->second.toString();

    it = config.find(s.cfg_timeformat); 
    if (it != end) 
        m_timeformat = it->second.toString();
    else
        m_timeformat = m_defaultTimeformat;

    it = config.find(s.cfg_format);
    if (it != config.end())
        m_tokens = parse(it->second.toString());
    else
        m_tokens = parse(m_defaultFormat);

}
