#include "gtest/gtest.h"

#include <log/CnLogger.h>

// ============================================================================
// Test Implementation of CnLogger (concrete class for testing)
// ============================================================================

class TestCnLogger : public CnLogger
{
public:
    TestCnLogger() : m_lastSource(), m_lastMessage() {}

    void logMessage(const CnString &source, const Message &msg) override
    {
        m_lastSource = source;
        m_lastMessage = msg;
    }

    // Accessors for testing
    const CnString& lastSource() const { return m_lastSource; }
    const Message& lastMessage() const { return m_lastMessage; }
    bool hasLoggedMessage() const { return !m_lastSource.isEmpty(); }
    void clear() { m_lastSource.clear(); m_lastMessage = Message(); }

private:
    CnString m_lastSource;
    Message m_lastMessage;
};

// ============================================================================
// CnLogger Tests - Strings and Defaults
// ============================================================================

TEST(CnLoggerTest, StringsInstance)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    
    EXPECT_EQ(s.tokenSource, CnSTR("%src"));
    EXPECT_EQ(s.tokenTime, CnSTR("%time"));
    EXPECT_EQ(s.tokenCategory, CnSTR("%cat"));
    EXPECT_EQ(s.tokenText, CnSTR("%text"));
    EXPECT_EQ(s.cfg_defaultFormat, CnSTR("default.format"));
    EXPECT_EQ(s.cfg_defaultTimeformat, CnSTR("default.timeformat"));
    EXPECT_EQ(s.cfg_format, CnSTR("format"));
    EXPECT_EQ(s.cfg_timeformat, CnSTR("timeformat"));
}

TEST(CnLoggerTest, DefaultsInstance)
{
    const CnLogger::Defaults &d = CnLogger::Defaults::instance();
    
    EXPECT_EQ(d.format, CnSTR("[%time] %src. %text"));
    EXPECT_EQ(d.timeformat, CnSTR("%Y-%M-%D %h:%m:%s.%f"));
}

// ============================================================================
// CnLogger Tests - Token Parsing
// ============================================================================

TEST(CnLoggerTest, ParseEmptyString)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR(""));
    EXPECT_TRUE(tokens.empty());
}

TEST(CnLoggerTest, ParseLiteralOnly)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("Simple text"));
    
    EXPECT_EQ(tokens.size(), 1);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    EXPECT_EQ(it->str, CnSTR("Simple text"));
}

TEST(CnLoggerTest, ParseSourceToken)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("%src"));
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.begin()->type, CnLogger::Token::Source);
}

TEST(CnLoggerTest, ParseTimeToken)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("%time"));
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.begin()->type, CnLogger::Token::Time);
}

TEST(CnLoggerTest, ParseCategoryToken)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("%cat"));
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.begin()->type, CnLogger::Token::Category);
}

TEST(CnLoggerTest, ParseTextToken)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("%text"));
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.begin()->type, CnLogger::Token::Text);
}

TEST(CnLoggerTest, ParseMixedTokens)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("[%time] %src: %text"));
    
    EXPECT_EQ(tokens.size(), 6);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    EXPECT_EQ(it->str, CnSTR("["));
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Time);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    EXPECT_EQ(it->str, CnSTR("] "));
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Source);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    EXPECT_EQ(it->str, CnSTR(": "));
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Text);
}

TEST(CnLoggerTest, ParseAllTokenTypes)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("%src %time %cat %text"));
    
    EXPECT_EQ(tokens.size(), 7);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Source);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Time);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Category);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Text);
}

TEST(CnLoggerTest, ParseInvalidToken)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("%invalid"));
    
    EXPECT_EQ(tokens.size(), 1);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    EXPECT_EQ(it->str, CnSTR("%invalid"));
}

TEST(CnLoggerTest, ParsePercentWithoutToken)
{
    CnList<CnLogger::Token> tokens = CnLogger::parse(CnSTR("Value: 50%"));
    
    EXPECT_EQ(tokens.size(), 1);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    EXPECT_EQ(it->str, CnSTR("Value: 50%"));
}

// ============================================================================
// CnLogger Tests - Token toString
// ============================================================================

TEST(CnLoggerTest, TokenToStringSource)
{
    CnLogger::Token token(CnLogger::Token::Source);
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Test message");
    
    CnString result = CnLogger::toString(token, CnSTR("TestSource"), CnSTR("%Y-%M-%D"), msg);
    EXPECT_EQ(result, CnSTR("TestSource"));
}

TEST(CnLoggerTest, TokenToStringText)
{
    CnLogger::Token token(CnLogger::Token::Text);
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Test message");
    
    CnString result = CnLogger::toString(token, CnSTR("Source"), CnSTR("%Y-%M-%D"), msg);
    EXPECT_EQ(result, CnSTR("Test message"));
}

TEST(CnLoggerTest, TokenToStringCategory)
{
    CnLogger::Token token(CnLogger::Token::Category);
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Error;
    msg.text = CnSTR("Test");
    
    CnString result = CnLogger::toString(token, CnSTR("Source"), CnSTR("%Y-%M-%D"), msg);
    // Result should be string representation of Log_Error
    EXPECT_FALSE(result.isEmpty());
}

TEST(CnLoggerTest, TokenToStringLiteral)
{
    CnLogger::Token token(CnLogger::Token::Literal);
    token.str = CnSTR("Literal text");
    CnLogger::Message msg;
    
    CnString result = CnLogger::toString(token, CnSTR("Source"), CnSTR("%Y-%M-%D"), msg);
    EXPECT_EQ(result, CnSTR("Literal text"));
}

TEST(CnLoggerTest, TokenToStringTime)
{
    CnLogger::Token token(CnLogger::Token::Time);
    CnLogger::Message msg;
    msg.time = CnDateTime(2024, 12, 25, 14, 30, 45);
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Test");
    
    CnString result = CnLogger::toString(token, CnSTR("Source"), CnSTR("%Y-%M-%D %h:%m:%s"), msg);
    EXPECT_FALSE(result.isEmpty());
    // Basic check that it contains expected components
    EXPECT_TRUE(result.find(CnSTR("2024")) != CnString::npos || result.find(CnSTR("12")) != CnString::npos || result.find(CnSTR("25")) != CnString::npos);
}

// ============================================================================
// CnLogger Tests - Constructor and Basic Methods
// ============================================================================

TEST(CnLoggerTest, ConstructorDefaults)
{
    TestCnLogger logger;
    
    // Logger should have default timeformat set
    EXPECT_EQ(logger.timeformat(), CnSTR("%Y-%M-%D %h:%m:%s.%f"));
}

TEST(CnLoggerTest, SetAndGetTimeformat)
{
    TestCnLogger logger;
    
    logger.setTimeformat(CnSTR("%Y-%M-%D"));
    EXPECT_EQ(logger.timeformat(), CnSTR("%Y-%M-%D"));
}

TEST(CnLoggerTest, SetFormat)
{
    TestCnLogger logger;
    
    logger.setFormat(CnSTR("%src: %text"));
    CnList<CnLogger::Token> tokens = logger.tokens();
    
    EXPECT_EQ(tokens.size(), 3);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Source);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Literal);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Text);
}

TEST(CnLoggerTest, SetTokens)
{
    TestCnLogger logger;
    
    CnList<CnLogger::Token> tokens;
    tokens.push_back(CnLogger::Token(CnLogger::Token::Source));
    tokens.push_back(CnLogger::Token(CnLogger::Token::Text));
    
    logger.setTokens(tokens);
    
    CnList<CnLogger::Token> retrieved = logger.tokens();
    EXPECT_EQ(retrieved.size(), 2);
    auto it = retrieved.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Source);
    ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Text);
}

// ============================================================================
// CnLogger Tests - Message Formatting
// ============================================================================

TEST(CnLoggerTest, ToStringWithSimpleFormat)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%src: %text"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Hello World");
    
    CnString result = logger.toString(CnSTR("TestApp"), msg);
    EXPECT_EQ(result, CnSTR("TestApp: Hello World"));
}

TEST(CnLoggerTest, ToStringWithComplexFormat)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("[%time] %src - %text"));
    logger.setTimeformat(CnSTR("%Y-%M-%D"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime(2024, 12, 25, 10, 30, 0);
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Test message");
    
    CnString result = logger.toString(CnSTR("App"), msg);
    EXPECT_FALSE(result.isEmpty());
    EXPECT_TRUE(result.find(CnSTR("App")) != CnString::npos);
    EXPECT_TRUE(result.find(CnSTR("Test message")) != CnString::npos);
}

TEST(CnLoggerTest, ToStringEmptySource)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%src: %text"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Message");
    
    CnString result = logger.toString(CnSTR(""), msg);
    EXPECT_EQ(result, CnSTR(": Message"));
}

TEST(CnLoggerTest, ToStringEmptyText)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%src: %text"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("");
    
    CnString result = logger.toString(CnSTR("App"), msg);
    EXPECT_EQ(result, CnSTR("App: "));
}

// ============================================================================
// CnLogger Tests - Config Methods
// ============================================================================

TEST(CnLoggerTest, SetConfigWithFormat)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    TestCnLogger logger;
    
    Cn::Config config;
    config[s.cfg_format] = CnVariant(CnSTR("%src - %text"));
    
    logger.setConfig(config);
    
    CnList<CnLogger::Token> tokens = logger.tokens();
    EXPECT_EQ(tokens.size(), 3);
    auto it = tokens.begin();
    EXPECT_EQ(it->type, CnLogger::Token::Source);
    ++it; ++it;
    EXPECT_EQ(it->type, CnLogger::Token::Text);
}

TEST(CnLoggerTest, SetConfigWithTimeformat)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    TestCnLogger logger;
    
    Cn::Config config;
    config[s.cfg_timeformat] = CnVariant(CnSTR("%Y/%M/%D"));
    
    logger.setConfig(config);
    
    EXPECT_EQ(logger.timeformat(), CnSTR("%Y/%M/%D"));
}

TEST(CnLoggerTest, SetConfigWithDefaultFormat)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    TestCnLogger logger;
    
    Cn::Config config;
    config[s.cfg_defaultFormat] = CnVariant(CnSTR("[%src] %text"));
    
    logger.setConfig(config);
    
    // Should use default format when cfg_format is not specified
    CnList<CnLogger::Token> tokens = logger.tokens();
    EXPECT_FALSE(tokens.empty());
}

TEST(CnLoggerTest, SetConfigWithDefaultTimeformat)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    TestCnLogger logger;
    
    Cn::Config config;
    config[s.cfg_defaultTimeformat] = CnVariant(CnSTR("%Y-%M-%D"));
    config[s.cfg_format] = CnVariant(CnSTR("%time"));
    
    logger.setConfig(config);
    
    // Timeformat should fall back to default when cfg_timeformat not specified
    EXPECT_EQ(logger.timeformat(), CnSTR("%Y-%M-%D"));
}

TEST(CnLoggerTest, SetConfigEmptyConfig)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%src: %text"));
    
    Cn::Config config;
    logger.setConfig(config);
    
    // Should use defaults when config is empty
    CnList<CnLogger::Token> tokens = logger.tokens();
    EXPECT_FALSE(tokens.empty());
}

TEST(CnLoggerTest, ResetConfig)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    TestCnLogger logger;
    
    // First set a config
    Cn::Config config1;
    config1[s.cfg_format] = CnVariant(CnSTR("%src"));
    logger.setConfig(config1);
    
    // Reset and set new config
    logger.resetConfig();
    
    Cn::Config config2;
    config2[s.cfg_format] = CnVariant(CnSTR("%text"));
    logger.setConfig(config2);
    
    CnList<CnLogger::Token> tokens = logger.tokens();
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens.begin()->type, CnLogger::Token::Text);
}

TEST(CnLoggerTest, SetConfigMultipleTimes)
{
    const CnLogger::Strings &s = CnLogger::Strings::instance();
    TestCnLogger logger;
    
    Cn::Config config1;
    config1[s.cfg_format] = CnVariant(CnSTR("%src"));
    logger.setConfig(config1);
    
    // Second call without reset should not change config
    Cn::Config config2;
    config2[s.cfg_format] = CnVariant(CnSTR("%text"));
    logger.setConfig(config2);
    
    CnList<CnLogger::Token> tokens = logger.tokens();
    EXPECT_EQ(tokens.begin()->type, CnLogger::Token::Source);
}

// ============================================================================
// CnLogger Tests - Abstract Interface
// ============================================================================

TEST(CnLoggerTest, LogMessageInterface)
{
    TestCnLogger logger;
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Test message");
    
    logger.logMessage(CnSTR("TestSource"), msg);
    
    EXPECT_TRUE(logger.hasLoggedMessage());
    EXPECT_EQ(logger.lastSource(), CnSTR("TestSource"));
    EXPECT_EQ(logger.lastMessage().text, CnSTR("Test message"));
    EXPECT_EQ(logger.lastMessage().category, Cn::Log_Info);
}

TEST(CnLoggerTest, LogMessageMultipleTimes)
{
    TestCnLogger logger;
    
    CnLogger::Message msg1;
    msg1.category = Cn::Log_Info;
    msg1.text = CnSTR("First");
    logger.logMessage(CnSTR("Source1"), msg1);
    
    EXPECT_EQ(logger.lastMessage().text, CnSTR("First"));
    
    CnLogger::Message msg2;
    msg2.category = Cn::Log_Error;
    msg2.text = CnSTR("Second");
    logger.logMessage(CnSTR("Source2"), msg2);
    
    EXPECT_EQ(logger.lastMessage().text, CnSTR("Second"));
    EXPECT_EQ(logger.lastSource(), CnSTR("Source2"));
}

// ============================================================================
// CnLogger Tests - Message Structure
// ============================================================================

TEST(CnLoggerTest, MessageAllCategories)
{
    TestCnLogger logger;
    
    Cn::LogFlag categories[] = {
        Cn::Log_Error,
        Cn::Log_Warning,
        Cn::Log_Info,
        Cn::Log_Trace,
        Cn::Log_TraceDetails
    };
    
    for (Cn::LogFlag category : categories)
    {
        CnLogger::Message msg;
        msg.time = CnDateTime::currentLocalDateTime();
        msg.category = category;
        msg.text = CnSTR("Test");
        
        logger.logMessage(CnSTR("Source"), msg);
        EXPECT_EQ(logger.lastMessage().category, category);
    }
}

TEST(CnLoggerTest, MessageWithTimestamp)
{
    TestCnLogger logger;
    
    CnDateTime specificTime(2024, 12, 25, 10, 30, 45);
    CnLogger::Message msg;
    msg.time = specificTime;
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Timed message");
    
    logger.logMessage(CnSTR("App"), msg);
    
    EXPECT_EQ(logger.lastMessage().time.year(), 2024);
    EXPECT_EQ(logger.lastMessage().time.month(), 12);
    EXPECT_EQ(logger.lastMessage().time.day(), 25);
}

// ============================================================================
// CnLogger Tests - Thread Safety (basic)
// ============================================================================

TEST(CnLoggerTest, CriticalSectionProtection)
{
    TestCnLogger logger;
    
    // Test that multiple operations work correctly
    // (actual thread safety would require multi-threaded test)
    logger.setFormat(CnSTR("%src"));
    CnString tf1 = logger.timeformat();
    
    logger.setTimeformat(CnSTR("%Y"));
    CnList<CnLogger::Token> tokens = logger.tokens();
    
    CnString tf2 = logger.timeformat();
    EXPECT_EQ(tf2, CnSTR("%Y"));
    EXPECT_FALSE(tokens.empty());
}

// ============================================================================
// CnLogger Tests - Edge Cases
// ============================================================================

TEST(CnLoggerTest, VeryLongMessage)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%text"));
    
    CnString longText;
    for (int i = 0; i < 1000; i++)
        longText += CnSTR("x");
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = longText;
    
    CnString result = logger.toString(CnSTR("Source"), msg);
    EXPECT_EQ(result.length(), 1000);
}

TEST(CnLoggerTest, SpecialCharactersInMessage)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%text"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Special: \n\t\r\"\'\\");
    
    CnString result = logger.toString(CnSTR("Source"), msg);
    EXPECT_TRUE(result.find(CnSTR("Special:")) != CnString::npos);
}

TEST(CnLoggerTest, UnicodeInMessage)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%src: %text"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Тест Unicode 测试");
    
    logger.logMessage(CnSTR("Джерело"), msg);
    
    EXPECT_EQ(logger.lastMessage().text, CnSTR("Тест Unicode 测试"));
    EXPECT_EQ(logger.lastSource(), CnSTR("Джерело"));
}

TEST(CnLoggerTest, FormatWithMultipleSpaces)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("%src    %text"));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Message");
    
    CnString result = logger.toString(CnSTR("Source"), msg);
    EXPECT_EQ(result, CnSTR("Source    Message"));
}

TEST(CnLoggerTest, FormatOnlyLiterals)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR("Static log prefix: "));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Any text");
    
    CnString result = logger.toString(CnSTR("Source"), msg);
    EXPECT_EQ(result, CnSTR("Static log prefix: "));
}

TEST(CnLoggerTest, EmptyFormatString)
{
    TestCnLogger logger;
    logger.setFormat(CnSTR(""));
    
    CnLogger::Message msg;
    msg.time = CnDateTime::currentLocalDateTime();
    msg.category = Cn::Log_Info;
    msg.text = CnSTR("Message");
    
    CnString result = logger.toString(CnSTR("Source"), msg);
    EXPECT_TRUE(result.isEmpty());
}
