#include "gtest/gtest.h"

#include <log/CnLoggerManager.h>
#include <log/CnLogger.h>

// ============================================================================
// Test Logger Implementation for CnLoggerManager Testing
// ============================================================================

class MockLogger : public CnLogger
{
public:
    MockLogger() : m_logCount(0), m_lastSource(), m_lastMessage() {}

    void logMessage(const CnString &source, const Message &msg) override
    {
        m_logCount++;
        m_lastSource = source;
        m_lastMessage = msg;
        m_allMessages.push_back(msg);
    }

    // Test accessors
    int logCount() const { return m_logCount; }
    const CnString& lastSource() const { return m_lastSource; }
    const Message& lastMessage() const { return m_lastMessage; }
    const CnList<Message>& allMessages() const { return m_allMessages; }
    void clear() { m_logCount = 0; m_lastSource.clear(); m_allMessages.clear(); }

private:
    int m_logCount;
    CnString m_lastSource;
    Message m_lastMessage;
    CnList<Message> m_allMessages;
};

// ============================================================================
// CnLoggerManager Tests - Constructor and Basic Properties
// ============================================================================

TEST(CnLoggerManagerTest, ConstructorDefaults)
{
    CnLoggerManager manager;
    
    EXPECT_TRUE(manager.name().isEmpty());
    EXPECT_EQ(manager.logFlags(), Cn::LogFlags(0));
    EXPECT_TRUE(manager.loggers().empty());
}

TEST(CnLoggerManagerTest, SetAndGetName)
{
    CnLoggerManager manager;
    
    manager.setName(CnSTR("TestManager"));
    EXPECT_EQ(manager.name(), CnSTR("TestManager"));
}

TEST(CnLoggerManagerTest, SetNameMultipleTimes)
{
    CnLoggerManager manager;
    
    manager.setName(CnSTR("First"));
    EXPECT_EQ(manager.name(), CnSTR("First"));
    
    manager.setName(CnSTR("Second"));
    EXPECT_EQ(manager.name(), CnSTR("Second"));
}

TEST(CnLoggerManagerTest, SetEmptyName)
{
    CnLoggerManager manager;
    manager.setName(CnSTR("NotEmpty"));
    
    manager.setName(CnSTR(""));
    EXPECT_TRUE(manager.name().isEmpty());
}

// ============================================================================
// CnLoggerManager Tests - Log Flags
// ============================================================================

TEST(CnLoggerManagerTest, SetAndGetLogFlags)
{
    CnLoggerManager manager;
    
    manager.setLogFlags(Cn::Log_Error | Cn::Log_Warning);
    EXPECT_EQ(manager.logFlags(), Cn::Log_Error | Cn::Log_Warning);
}

TEST(CnLoggerManagerTest, SetAllLogFlags)
{
    CnLoggerManager manager;
    
    Cn::LogFlags allFlags = Cn::Log_Error | Cn::Log_Warning | Cn::Log_Info | 
                            Cn::Log_Trace | Cn::Log_TraceDetails;
    manager.setLogFlags(allFlags);
    
    EXPECT_EQ(manager.logFlags(), allFlags);
}

TEST(CnLoggerManagerTest, SetSingleLogFlag)
{
    CnLoggerManager manager;
    
    manager.setLogFlags(Cn::Log_Error);
    EXPECT_EQ(manager.logFlags(), Cn::Log_Error);
    EXPECT_TRUE(manager.logFlags() & Cn::Log_Error);
}

TEST(CnLoggerManagerTest, SetZeroLogFlags)
{
    CnLoggerManager manager;
    manager.setLogFlags(Cn::Log_Error);
    
    manager.setLogFlags(Cn::LogFlags(0));
    EXPECT_EQ(manager.logFlags(), Cn::LogFlags(0));
}

// ============================================================================
// CnLoggerManager Tests - Loggers Management
// ============================================================================

TEST(CnLoggerManagerTest, SetAndGetLoggers)
{
    CnLoggerManager manager;
    
    CnLoggerPtr logger1 = new MockLogger();
    CnLoggerPtr logger2 = new MockLogger();
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger1);
    loggers.push_back(logger2);
    
    manager.setLoggers(loggers);
    
    CnList<CnLoggerPtr> retrieved = manager.loggers();
    EXPECT_EQ(retrieved.size(), 2);
}

TEST(CnLoggerManagerTest, SetEmptyLoggersList)
{
    CnLoggerManager manager;
    
    CnLoggerPtr logger = new MockLogger();
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    
    // Set empty list
    manager.setLoggers(CnList<CnLoggerPtr>());
    EXPECT_TRUE(manager.loggers().empty());
}

TEST(CnLoggerManagerTest, SetSingleLogger)
{
    CnLoggerManager manager;
    
    CnLoggerPtr logger = new MockLogger();
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    
    manager.setLoggers(loggers);
    
    EXPECT_EQ(manager.loggers().size(), 1);
}

TEST(CnLoggerManagerTest, ReplaceLoggers)
{
    CnLoggerManager manager;
    
    CnLoggerPtr logger1 = new MockLogger();
    CnList<CnLoggerPtr> loggers1;
    loggers1.push_back(logger1);
    manager.setLoggers(loggers1);
    
    CnLoggerPtr logger2 = new MockLogger();
    CnList<CnLoggerPtr> loggers2;
    loggers2.push_back(logger2);
    manager.setLoggers(loggers2);
    
    CnList<CnLoggerPtr> retrieved = manager.loggers();
    EXPECT_EQ(retrieved.size(), 1);
}

// ============================================================================
// CnLoggerManager Tests - logMessage with va_list
// ============================================================================

TEST(CnLoggerManagerTest, LogMessageBasic)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Error);
    manager.setName(CnSTR("TestApp"));
    
    manager.logMessage(Cn::Log_Error, CnSTR("Test message"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_EQ(mockLogger->lastSource(), CnSTR("TestApp"));
    EXPECT_EQ(mockLogger->lastMessage().text, CnSTR("Test message"));
    EXPECT_EQ(mockLogger->lastMessage().category, Cn::Log_Error);
}

TEST(CnLoggerManagerTest, LogMessageWithFormatting)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    manager.setName(CnSTR("App"));
    
    manager.logMessage(Cn::Log_Info, CnSTR("Value: %d, Name: %s"), 42, "test");
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_TRUE(mockLogger->lastMessage().text.find(CnSTR("42")) != CnString::npos);
    EXPECT_TRUE(mockLogger->lastMessage().text.find(CnSTR("test")) != CnString::npos);
}

TEST(CnLoggerManagerTest, LogMessageMultipleLoggers)
{
    CnLoggerManager manager;
    MockLogger *mockLogger1 = new MockLogger();
    MockLogger *mockLogger2 = new MockLogger();
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(CnLoggerPtr(mockLogger1));
    loggers.push_back(CnLoggerPtr(mockLogger2));
    
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Warning);
    manager.setName(CnSTR("MultiApp"));
    
    manager.logMessage(Cn::Log_Warning, CnSTR("Warning message"));
    
    EXPECT_EQ(mockLogger1->logCount(), 1);
    EXPECT_EQ(mockLogger2->logCount(), 1);
    EXPECT_EQ(mockLogger1->lastMessage().text, CnSTR("Warning message"));
    EXPECT_EQ(mockLogger2->lastMessage().text, CnSTR("Warning message"));
}

TEST(CnLoggerManagerTest, LogMessageNoLoggers)
{
    CnLoggerManager manager;
    manager.setLogFlags(Cn::Log_Error);
    
    // Should not crash with no loggers
    manager.logMessage(Cn::Log_Error, CnSTR("No logger message"));
    
    EXPECT_TRUE(manager.loggers().empty());
}

TEST(CnLoggerManagerTest, LogMessageWithTimestamp)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    CnDateTime beforeLog = CnDateTime::currentLocalDateTime();
    manager.logMessage(Cn::Log_Info, CnSTR("Timestamped"));
    CnDateTime afterLog = CnDateTime::currentLocalDateTime();
    
    // Message timestamp should be between before and after
    CnDateTime msgTime = mockLogger->lastMessage().time;
    EXPECT_TRUE(msgTime >= beforeLog || msgTime <= afterLog);
}

// ============================================================================
// CnLoggerManager Tests - Convenience Logging Methods
// ============================================================================

TEST(CnLoggerManagerTest, LogErrorMethod)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Error);
    
    manager.logError(CnSTR("Error message"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_EQ(mockLogger->lastMessage().category, Cn::Log_Error);
    EXPECT_EQ(mockLogger->lastMessage().text, CnSTR("Error message"));
}

TEST(CnLoggerManagerTest, LogWarningMethod)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Warning);
    
    manager.logWarning(CnSTR("Warning message"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_EQ(mockLogger->lastMessage().category, Cn::Log_Warning);
}

TEST(CnLoggerManagerTest, LogInfoMethod)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("Info message"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_EQ(mockLogger->lastMessage().category, Cn::Log_Info);
}

TEST(CnLoggerManagerTest, LogTraceMethod)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Trace);
    
    manager.logTrace(CnSTR("Trace message"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_EQ(mockLogger->lastMessage().category, Cn::Log_Trace);
}

TEST(CnLoggerManagerTest, LogTraceDetailsMethod)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_TraceDetails);
    
    manager.logTraceDetails(CnSTR("Detailed trace"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_EQ(mockLogger->lastMessage().category, Cn::Log_TraceDetails);
}

// ============================================================================
// CnLoggerManager Tests - Log Flag Filtering
// ============================================================================

TEST(CnLoggerManagerTest, LogErrorFilteredByFlags)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Warning); // Only warnings enabled
    
    manager.logError(CnSTR("Should not log"));
    
    EXPECT_EQ(mockLogger->logCount(), 0);
}

TEST(CnLoggerManagerTest, LogInfoFilteredByFlags)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Error); // Only errors enabled
    
    manager.logInfo(CnSTR("Should not log"));
    
    EXPECT_EQ(mockLogger->logCount(), 0);
}

TEST(CnLoggerManagerTest, MultipleLogLevels)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Error | Cn::Log_Warning | Cn::Log_Info);
    
    manager.logError(CnSTR("Error"));
    manager.logWarning(CnSTR("Warning"));
    manager.logInfo(CnSTR("Info"));
    manager.logTrace(CnSTR("Trace")); // Should be filtered
    
    EXPECT_EQ(mockLogger->logCount(), 3);
}

TEST(CnLoggerManagerTest, NoFlagsSetFiltersAll)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::LogFlags(0)); // No flags
    
    manager.logError(CnSTR("Error"));
    manager.logWarning(CnSTR("Warning"));
    manager.logInfo(CnSTR("Info"));
    
    EXPECT_EQ(mockLogger->logCount(), 0);
}

// ============================================================================
// CnLoggerManager Tests - Format String Tests
// ============================================================================

TEST(CnLoggerManagerTest, FormatWithInteger)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("Count: %d"), 42);
    
    EXPECT_TRUE(mockLogger->lastMessage().text.find(CnSTR("42")) != CnString::npos);
}

TEST(CnLoggerManagerTest, FormatWithMultipleArgs)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("X: %d, Y: %d, Z: %d"), 10, 20, 30);
    
    CnString text = mockLogger->lastMessage().text;
    EXPECT_TRUE(text.find(CnSTR("10")) != CnString::npos);
    EXPECT_TRUE(text.find(CnSTR("20")) != CnString::npos);
    EXPECT_TRUE(text.find(CnSTR("30")) != CnString::npos);
}

TEST(CnLoggerManagerTest, FormatWithString)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("Name: %s"), "TestName");
    
    EXPECT_TRUE(mockLogger->lastMessage().text.find(CnSTR("TestName")) != CnString::npos);
}

TEST(CnLoggerManagerTest, FormatWithFloat)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("Pi: %.2f"), 3.14159);
    
    EXPECT_TRUE(mockLogger->lastMessage().text.find(CnSTR("3.14")) != CnString::npos);
}

TEST(CnLoggerManagerTest, LongFormattedMessage)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    // Create a long message
    CnString format = CnSTR("Long: ");
    for (int i = 0; i < 100; i++)
        format += CnSTR("%d ");
    
    manager.logInfo(format.c_str(), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                    41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
                    51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                    61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                    71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                    81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
                    91, 92, 93, 94, 95, 96, 97, 98, 99, 100);
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_FALSE(mockLogger->lastMessage().text.isEmpty());
}

// ============================================================================
// CnLoggerManager Tests - Sequential Logging
// ============================================================================

TEST(CnLoggerManagerTest, SequentialMessages)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("First"));
    manager.logInfo(CnSTR("Second"));
    manager.logInfo(CnSTR("Third"));
    
    EXPECT_EQ(mockLogger->logCount(), 3);
    EXPECT_EQ(mockLogger->allMessages().size(), 3);
    auto it = mockLogger->allMessages().begin();
    EXPECT_EQ(it->text, CnSTR("First"));
    ++it;
    EXPECT_EQ(it->text, CnSTR("Second"));
    ++it;
    EXPECT_EQ(it->text, CnSTR("Third"));
}

TEST(CnLoggerManagerTest, MixedCategories)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Error | Cn::Log_Warning | Cn::Log_Info);
    
    manager.logError(CnSTR("Error 1"));
    manager.logWarning(CnSTR("Warning 1"));
    manager.logInfo(CnSTR("Info 1"));
    
    EXPECT_EQ(mockLogger->logCount(), 3);
    auto it = mockLogger->allMessages().begin();
    EXPECT_EQ(it->category, Cn::Log_Error);
    ++it;
    EXPECT_EQ(it->category, Cn::Log_Warning);
    ++it;
    EXPECT_EQ(it->category, Cn::Log_Info);
}

// ============================================================================
// CnLoggerManager Tests - Edge Cases
// ============================================================================

TEST(CnLoggerManagerTest, EmptyMessage)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR(""));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
    EXPECT_TRUE(mockLogger->lastMessage().text.isEmpty());
}

TEST(CnLoggerManagerTest, SpecialCharactersInMessage)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("Special: \n\t\r"));
    
    EXPECT_EQ(mockLogger->logCount(), 1);
}

TEST(CnLoggerManagerTest, ChangeLoggersAfterLogging)
{
    CnLoggerManager manager;
    MockLogger *mockLogger1 = new MockLogger();
    MockLogger *mockLogger2 = new MockLogger();
    
    CnList<CnLoggerPtr> loggers1;
    loggers1.push_back(CnLoggerPtr(mockLogger1));
    manager.setLoggers(loggers1);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.logInfo(CnSTR("To logger 1"));
    EXPECT_EQ(mockLogger1->logCount(), 1);
    
    CnList<CnLoggerPtr> loggers2;
    loggers2.push_back(CnLoggerPtr(mockLogger2));
    manager.setLoggers(loggers2);
    
    manager.logInfo(CnSTR("To logger 2"));
    EXPECT_EQ(mockLogger1->logCount(), 1); // No additional logs
    EXPECT_EQ(mockLogger2->logCount(), 1);
}

TEST(CnLoggerManagerTest, ChangeFlagsAfterLogging)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    
    manager.setLogFlags(Cn::Log_Error);
    manager.logError(CnSTR("Error message"));
    EXPECT_EQ(mockLogger->logCount(), 1);
    
    manager.setLogFlags(Cn::Log_Warning);
    manager.logError(CnSTR("Should not log"));
    EXPECT_EQ(mockLogger->logCount(), 1); // Still 1
    
    manager.logWarning(CnSTR("Warning message"));
    EXPECT_EQ(mockLogger->logCount(), 2);
}

TEST(CnLoggerManagerTest, ChangeNameAfterLogging)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    
    manager.setName(CnSTR("FirstName"));
    manager.logInfo(CnSTR("Message 1"));
    EXPECT_EQ(mockLogger->lastSource(), CnSTR("FirstName"));
    
    manager.setName(CnSTR("SecondName"));
    manager.logInfo(CnSTR("Message 2"));
    EXPECT_EQ(mockLogger->lastSource(), CnSTR("SecondName"));
}

TEST(CnLoggerManagerTest, UnicodeInManagerName)
{
    CnLoggerManager manager;
    MockLogger *mockLogger = new MockLogger();
    CnLoggerPtr logger(mockLogger);
    
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(logger);
    manager.setLoggers(loggers);
    manager.setLogFlags(Cn::Log_Info);
    manager.setName(CnSTR("Тест Manager 测试"));
    
    manager.logInfo(CnSTR("Test"));
    
    EXPECT_EQ(mockLogger->lastSource(), CnSTR("Тест Manager 测试"));
}

// ============================================================================
// CnLoggerManager Tests - Thread Safety (basic check)
// ============================================================================

TEST(CnLoggerManagerTest, ConcurrentPropertyAccess)
{
    CnLoggerManager manager;
    
    // Set properties
    manager.setName(CnSTR("TestManager"));
    manager.setLogFlags(Cn::Log_Error | Cn::Log_Warning);
    
    MockLogger *mockLogger = new MockLogger();
    CnList<CnLoggerPtr> loggers;
    loggers.push_back(CnLoggerPtr(mockLogger));
    manager.setLoggers(loggers);
    
    // Get properties (simulating concurrent access)
    CnString name = manager.name();
    Cn::LogFlags flags = manager.logFlags();
    CnList<CnLoggerPtr> retrievedLoggers = manager.loggers();
    
    EXPECT_EQ(name, CnSTR("TestManager"));
    EXPECT_EQ(flags, Cn::Log_Error | Cn::Log_Warning);
    EXPECT_EQ(retrievedLoggers.size(), 1);
}
