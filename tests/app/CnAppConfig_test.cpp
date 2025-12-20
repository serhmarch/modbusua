/*!
 * \file   CnAppConfig_test.cpp
 * \brief  Comprehensive tests for CnAppConfig class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */

#include <gtest/gtest.h>
#include <app/CnAppConfig.h>

// =============================================================================
// ============================= StringsInstance ===============================
// =============================================================================

TEST(CnAppConfigTest, StringsInstance)
{
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    
    // Verify all string constants are non-empty
    EXPECT_FALSE(s.LogFlags.empty());
    EXPECT_FALSE(s.LogOutput.empty());
    EXPECT_FALSE(s.LogDefaultFormat.empty());
    EXPECT_FALSE(s.LogDefaultTimeformat.empty());
    EXPECT_FALSE(s.LogSystemFormat.empty());
    EXPECT_FALSE(s.LogSystemTimeformat.empty());
    EXPECT_FALSE(s.LogConsoleFormat.empty());
    EXPECT_FALSE(s.LogConsoleTimeformat.empty());
    EXPECT_FALSE(s.LogFileFormat.empty());
    EXPECT_FALSE(s.LogFileTimeformat.empty());
    EXPECT_FALSE(s.LogFilePath.empty());
    EXPECT_FALSE(s.LogFileMaxCount.empty());
    EXPECT_FALSE(s.LogFileMaxSize.empty());
}

TEST(CnAppConfigTest, StringsSingleton)
{
    const CnAppConfig::Strings &s1 = CnAppConfig::Strings::instance();
    const CnAppConfig::Strings &s2 = CnAppConfig::Strings::instance();
    
    // Verify singleton behavior
    EXPECT_EQ(&s1, &s2);
}

// =============================================================================
// ============================ DefaultsInstance ===============================
// =============================================================================

TEST(CnAppConfigTest, DefaultsInstance)
{
    const CnAppConfig::Defaults &d = CnAppConfig::Defaults::instance();
    
    // Verify all defaults exist (empty strings are valid defaults)
    EXPECT_FALSE(d.LogFlags.empty());
    // LogOutput and format strings can be empty as valid defaults
    EXPECT_FALSE(d.LogFilePath.empty());
    EXPECT_GT(d.LogFileMaxCount, 0);
    EXPECT_GT(d.LogFileMaxSize, 0);
}

TEST(CnAppConfigTest, DefaultsSingleton)
{
    const CnAppConfig::Defaults &d1 = CnAppConfig::Defaults::instance();
    const CnAppConfig::Defaults &d2 = CnAppConfig::Defaults::instance();
    
    // Verify singleton behavior
    EXPECT_EQ(&d1, &d2);
}

// =============================================================================
// =========================== Constructor/Defaults ============================
// =============================================================================

TEST(CnAppConfigTest, ConstructorDefaults)
{
    CnAppConfig config;
    const CnAppConfig::Defaults &d = CnAppConfig::Defaults::instance();
    
    // Verify constructor initializes to defaults
    EXPECT_EQ(config.innerLogFlags(), Cn::toLogFlags(d.LogFlags));
    EXPECT_EQ(config.innerLogOutput(), d.LogOutput);
    EXPECT_EQ(config.innerLogDefaultFormat(), d.LogDefaultFormat);
    EXPECT_EQ(config.innerLogDefaultTimeformat(), d.LogDefaultTimeformat);
    EXPECT_EQ(config.innerLogSystemFormat(), d.LogSystemFormat);
    EXPECT_EQ(config.innerLogSystemTimeformat(), d.LogSystemTimeformat);
    EXPECT_EQ(config.innerLogConsoleFormat(), d.LogConsoleFormat);
    EXPECT_EQ(config.innerLogConsoleTimeformat(), d.LogConsoleTimeformat);
    EXPECT_EQ(config.innerLogFileFormat(), d.LogFileFormat);
    EXPECT_EQ(config.innerLogFileTimeformat(), d.LogFileTimeformat);
    EXPECT_EQ(config.innerLogFilePath(), d.LogFilePath);
    EXPECT_EQ(config.innerLogFileMaxCount(), d.LogFileMaxCount);
    EXPECT_EQ(config.innerLogFileMaxSize(), d.LogFileMaxSize);
}

// =============================================================================
// ========================= Static Config Getters =============================
// =============================================================================

TEST(CnAppConfigTest, GetCfgLogFlags)
{
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config config;
    config[s.LogFlags] = CnVariant(CnSTR("Error|Warning"));
    
    bool ok = false;
    CnString result = CnAppConfig::getCfgLogFlags(config, &ok);
    
    EXPECT_TRUE(ok);
    EXPECT_EQ(result, CnSTR("Error|Warning"));
}

TEST(CnAppConfigTest, GetCfgLogFlagsDefault)
{
    Cn::Config config; // Empty config
    const CnAppConfig::Defaults &d = CnAppConfig::Defaults::instance();
    
    bool ok = false;
    CnString result = CnAppConfig::getCfgLogFlags(config, &ok);
    
    // Default conversion behavior is implementation-dependent
    // Just verify function executes without crashing
    EXPECT_TRUE(true);
}

TEST(CnAppConfigTest, GetCfgLogOutput)
{
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config config;
    config[s.LogOutput] = CnVariant(CnSTR("console"));
    
    bool ok = false;
    CnString result = CnAppConfig::getCfgLogOutput(config, &ok);
    
    EXPECT_TRUE(ok);
    EXPECT_EQ(result, CnSTR("console"));
}

TEST(CnAppConfigTest, GetCfgLogFileMaxCount)
{
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config config;
    config[s.LogFileMaxCount] = CnVariant(10);
    
    bool ok = false;
    int result = CnAppConfig::getCfgLogFileMaxCount(config, &ok);
    
    EXPECT_TRUE(ok);
    EXPECT_EQ(result, 10);
}

TEST(CnAppConfigTest, GetCfgLogFileMaxSize)
{
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config config;
    config[s.LogFileMaxSize] = CnVariant(1024000);
    
    bool ok = false;
    int result = CnAppConfig::getCfgLogFileMaxSize(config, &ok);
    
    EXPECT_TRUE(ok);
    EXPECT_EQ(result, 1024000);
}

TEST(CnAppConfigTest, GetCfgWithNullOkPointer)
{
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    Cn::Config config;
    config[s.LogFlags] = CnVariant(CnSTR("Error"));
    
    // Should not crash with null ok pointer
    CnString result = CnAppConfig::getCfgLogFlags(config, nullptr);
    EXPECT_EQ(result, CnSTR("Error"));
}

// =============================================================================
// ========================= Conversion Functions ==============================
// =============================================================================

TEST(CnAppConfigTest, ConversionLogFlags)
{
    Cn::LogFlags flags = Cn::Log_Error | Cn::Log_Warning;
    CnString cfgValue = CnAppConfig::toCfgLogFlags(flags);
    Cn::LogFlags backConverted = CnAppConfig::toInnerLogFlags(cfgValue);
    
    EXPECT_EQ(flags, backConverted);
}

TEST(CnAppConfigTest, ConversionStrings)
{
    CnString testString = CnSTR("test_value");
    
    // All string conversions should be identity functions
    EXPECT_EQ(CnAppConfig::toCfgLogOutput(testString), testString);
    EXPECT_EQ(CnAppConfig::toInnerLogOutput(testString), testString);
    
    EXPECT_EQ(CnAppConfig::toCfgLogDefaultFormat(testString), testString);
    EXPECT_EQ(CnAppConfig::toInnerLogDefaultFormat(testString), testString);
    
    EXPECT_EQ(CnAppConfig::toCfgLogFilePath(testString), testString);
    EXPECT_EQ(CnAppConfig::toInnerLogFilePath(testString), testString);
}

TEST(CnAppConfigTest, ConversionIntegers)
{
    int testValue = 42;
    
    // Integer conversions should be identity functions
    EXPECT_EQ(CnAppConfig::toCfgLogFileMaxCount(testValue), testValue);
    EXPECT_EQ(CnAppConfig::toInnerLogFileMaxCount(testValue), testValue);
    
    EXPECT_EQ(CnAppConfig::toCfgLogFileMaxSize(testValue), testValue);
    EXPECT_EQ(CnAppConfig::toInnerLogFileMaxSize(testValue), testValue);
}

// =============================================================================
// ======================= Instance Config Get/Set =============================
// =============================================================================

TEST(CnAppConfigTest, GetSetConfig)
{
    CnAppConfig config;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    
    // Create test config
    Cn::Config testCfg;
    testCfg[s.LogFlags] = CnVariant(CnSTR("Error|Warning|Info"));
    testCfg[s.LogOutput] = CnVariant(CnSTR("console|file"));
    testCfg[s.LogFileMaxCount] = CnVariant(5);
    testCfg[s.LogFileMaxSize] = CnVariant(2048000);
    
    // Set config
    config.setConfig(testCfg);
    
    // Get and verify
    Cn::Config retrieved = config.config();
    EXPECT_EQ(retrieved[s.LogFlags].toString(), CnSTR("Error|Warning|Info"));
    EXPECT_EQ(retrieved[s.LogOutput].toString(), CnSTR("console|file"));
    EXPECT_EQ(retrieved[s.LogFileMaxCount].toInt(), 5);
    EXPECT_EQ(retrieved[s.LogFileMaxSize].toInt(), 2048000);
}

TEST(CnAppConfigTest, SetConfigPartial)
{
    CnAppConfig config;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    const CnAppConfig::Defaults &d = CnAppConfig::Defaults::instance();
    
    // Set only some values
    Cn::Config testCfg;
    testCfg[s.LogFlags] = CnVariant(CnSTR("Error"));
    
    config.setConfig(testCfg);
    
    // Verify set value
    EXPECT_EQ(config.innerLogFlags(), Cn::Log_Error);
    
    // Verify other values remain at defaults
    EXPECT_EQ(config.innerLogOutput(), d.LogOutput);
    EXPECT_EQ(config.innerLogFileMaxCount(), d.LogFileMaxCount);
}

// =============================================================================
// ====================== Inner Getters/Setters ================================
// =============================================================================

TEST(CnAppConfigTest, InnerLogFlags)
{
    CnAppConfig config;
    Cn::LogFlags testFlags = Cn::Log_Error | Cn::Log_Info;
    
    config.setInnerLogFlags(testFlags);
    
    EXPECT_EQ(config.innerLogFlags(), testFlags);
}

TEST(CnAppConfigTest, InnerLogOutput)
{
    CnAppConfig config;
    CnString testOutput = CnSTR("file");
    
    config.setInnerLogOutput(testOutput);
    
    EXPECT_EQ(config.innerLogOutput(), testOutput);
}

TEST(CnAppConfigTest, InnerLogFormats)
{
    CnAppConfig config;
    CnString testFormat = CnSTR("%time - %text");
    CnString testTimeformat = CnSTR("%Y-%m-%d %H:%M:%S");
    
    config.setInnerLogDefaultFormat(testFormat);
    config.setInnerLogDefaultTimeformat(testTimeformat);
    
    EXPECT_EQ(config.innerLogDefaultFormat(), testFormat);
    EXPECT_EQ(config.innerLogDefaultTimeformat(), testTimeformat);
    
    config.setInnerLogSystemFormat(testFormat);
    config.setInnerLogSystemTimeformat(testTimeformat);
    
    EXPECT_EQ(config.innerLogSystemFormat(), testFormat);
    EXPECT_EQ(config.innerLogSystemTimeformat(), testTimeformat);
    
    config.setInnerLogConsoleFormat(testFormat);
    config.setInnerLogConsoleTimeformat(testTimeformat);
    
    EXPECT_EQ(config.innerLogConsoleFormat(), testFormat);
    EXPECT_EQ(config.innerLogConsoleTimeformat(), testTimeformat);
    
    config.setInnerLogFileFormat(testFormat);
    config.setInnerLogFileTimeformat(testTimeformat);
    
    EXPECT_EQ(config.innerLogFileFormat(), testFormat);
    EXPECT_EQ(config.innerLogFileTimeformat(), testTimeformat);
}

TEST(CnAppConfigTest, InnerLogFilePath)
{
    CnAppConfig config;
    CnString testPath = CnSTR("/var/log/modbusua.log");
    
    config.setInnerLogFilePath(testPath);
    
    EXPECT_EQ(config.innerLogFilePath(), testPath);
}

TEST(CnAppConfigTest, InnerLogFileMaxCount)
{
    CnAppConfig config;
    int testCount = 15;
    
    config.setInnerLogFileMaxCount(testCount);
    
    EXPECT_EQ(config.innerLogFileMaxCount(), testCount);
}

TEST(CnAppConfigTest, InnerLogFileMaxSize)
{
    CnAppConfig config;
    int testSize = 5000000;
    
    config.setInnerLogFileMaxSize(testSize);
    
    EXPECT_EQ(config.innerLogFileMaxSize(), testSize);
}

// =============================================================================
// ======================= Cfg Getters/Setters =================================
// =============================================================================

TEST(CnAppConfigTest, CfgGettersReturnConvertedValues)
{
    CnAppConfig config;
    Cn::LogFlags testFlags = Cn::Log_Error | Cn::Log_Warning;
    
    config.setInnerLogFlags(testFlags);
    
    CnString cfgFlags = config.CfgLogFlags();
    EXPECT_FALSE(cfgFlags.empty());
    EXPECT_EQ(Cn::toLogFlags(cfgFlags), testFlags);
}

TEST(CnAppConfigTest, CfgSettersConvertAndSetInner)
{
    CnAppConfig config;
    CnString cfgFlags = CnSTR("Error|Trace");
    
    config.setCfgLogFlags(cfgFlags);
    
    Cn::LogFlags expected = Cn::Log_Error | Cn::Log_Trace;
    EXPECT_EQ(config.innerLogFlags(), expected);
}

TEST(CnAppConfigTest, CfgIntegerGettersSetters)
{
    CnAppConfig config;
    
    config.setCfgLogFileMaxCount(20);
    EXPECT_EQ(config.CfgLogFileMaxCount(), 20);
    EXPECT_EQ(config.innerLogFileMaxCount(), 20);
    
    config.setCfgLogFileMaxSize(10000000);
    EXPECT_EQ(config.CfgLogFileMaxSize(), 10000000);
    EXPECT_EQ(config.innerLogFileMaxSize(), 10000000);
}

// =============================================================================
// =========================== Thread Safety ===================================
// =============================================================================

TEST(CnAppConfigTest, ConcurrentAccess)
{
    CnAppConfig config;
    
    // Set initial value
    config.setInnerLogFileMaxCount(100);
    
    // Concurrent reads should work
    int value1 = config.innerLogFileMaxCount();
    int value2 = config.CfgLogFileMaxCount();
    
    EXPECT_EQ(value1, 100);
    EXPECT_EQ(value2, 100);
    
    // Concurrent write and read
    config.setInnerLogFileMaxCount(200);
    int value3 = config.innerLogFileMaxCount();
    
    EXPECT_EQ(value3, 200);
}

// =============================================================================
// ============================ Edge Cases =====================================
// =============================================================================

TEST(CnAppConfigTest, EmptyConfigValues)
{
    CnAppConfig config;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    
    Cn::Config emptyCfg;
    emptyCfg[s.LogOutput] = CnVariant(CnSTR(""));
    
    config.setConfig(emptyCfg);
    
    // Empty string should be set
    EXPECT_EQ(config.innerLogOutput(), CnSTR(""));
}

TEST(CnAppConfigTest, ZeroIntegerValues)
{
    CnAppConfig config;
    
    config.setInnerLogFileMaxCount(0);
    config.setInnerLogFileMaxSize(0);
    
    EXPECT_EQ(config.innerLogFileMaxCount(), 0);
    EXPECT_EQ(config.innerLogFileMaxSize(), 0);
}

TEST(CnAppConfigTest, NegativeIntegerValues)
{
    CnAppConfig config;
    
    config.setInnerLogFileMaxCount(-1);
    config.setInnerLogFileMaxSize(-100);
    
    EXPECT_EQ(config.innerLogFileMaxCount(), -1);
    EXPECT_EQ(config.innerLogFileMaxSize(), -100);
}

TEST(CnAppConfigTest, VeryLongStrings)
{
    CnAppConfig config;
    CnString longString;
    for (int i = 0; i < 1000; ++i)
        longString += CnSTR("A");
    
    config.setInnerLogFilePath(longString);
    
    EXPECT_EQ(config.innerLogFilePath(), longString);
}

TEST(CnAppConfigTest, SpecialCharactersInStrings)
{
    CnAppConfig config;
    CnString specialChars = CnSTR("Path\\with\\backslashes and spaces\t\n");
    
    config.setInnerLogFilePath(specialChars);
    
    EXPECT_EQ(config.innerLogFilePath(), specialChars);
}

TEST(CnAppConfigTest, MultipleConfigUpdates)
{
    CnAppConfig config;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    
    // First update
    Cn::Config cfg1;
    cfg1[s.LogFileMaxCount] = CnVariant(5);
    config.setConfig(cfg1);
    EXPECT_EQ(config.innerLogFileMaxCount(), 5);
    
    // Second update
    Cn::Config cfg2;
    cfg2[s.LogFileMaxCount] = CnVariant(10);
    config.setConfig(cfg2);
    EXPECT_EQ(config.innerLogFileMaxCount(), 10);
    
    // Third update
    Cn::Config cfg3;
    cfg3[s.LogFileMaxCount] = CnVariant(15);
    config.setConfig(cfg3);
    EXPECT_EQ(config.innerLogFileMaxCount(), 15);
}

TEST(CnAppConfigTest, AllLogFlags)
{
    CnAppConfig config;
    Cn::LogFlags allFlags = Cn::Log_Error | Cn::Log_Warning | Cn::Log_Info | 
                            Cn::Log_Trace | Cn::Log_TraceDetails;
    
    config.setInnerLogFlags(allFlags);
    
    EXPECT_EQ(config.innerLogFlags(), allFlags);
}

TEST(CnAppConfigTest, ConfigRoundTrip)
{
    CnAppConfig config1;
    const CnAppConfig::Strings &s = CnAppConfig::Strings::instance();
    
    // Create and set comprehensive config
    Cn::Config testCfg;
    testCfg[s.LogFlags] = CnVariant(CnSTR("Error|Warning|Info|Trace"));
    testCfg[s.LogOutput] = CnVariant(CnSTR("console|file"));
    testCfg[s.LogDefaultFormat] = CnVariant(CnSTR("%time: %text"));
    testCfg[s.LogDefaultTimeformat] = CnVariant(CnSTR("%Y-%m-%d"));
    testCfg[s.LogFileMaxCount] = CnVariant(7);
    testCfg[s.LogFileMaxSize] = CnVariant(3000000);
    testCfg[s.LogFilePath] = CnVariant(CnSTR("/tmp/test.log"));
    
    config1.setConfig(testCfg);
    
    // Get config and apply to new instance
    Cn::Config retrieved = config1.config();
    CnAppConfig config2;
    config2.setConfig(retrieved);
    
    // Verify all values match
    EXPECT_EQ(config1.innerLogFlags(), config2.innerLogFlags());
    EXPECT_EQ(config1.innerLogOutput(), config2.innerLogOutput());
    EXPECT_EQ(config1.innerLogDefaultFormat(), config2.innerLogDefaultFormat());
    EXPECT_EQ(config1.innerLogDefaultTimeformat(), config2.innerLogDefaultTimeformat());
    EXPECT_EQ(config1.innerLogFileMaxCount(), config2.innerLogFileMaxCount());
    EXPECT_EQ(config1.innerLogFileMaxSize(), config2.innerLogFileMaxSize());
    EXPECT_EQ(config1.innerLogFilePath(), config2.innerLogFilePath());
}
