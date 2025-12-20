/*!
 * \file   CnCfgPort_test.cpp
 * \brief  Unit tests for CnCfgPort class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */
#include "gtest/gtest.h"
#include <cfg/CnCfgPort.h>

class CnCfgPortTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        port = new CnCfgPort();
    }

    void TearDown() override
    {
        delete port;
    }

    CnCfgPort* port;
};

// Test default constructor and basic functionality
TEST_F(CnCfgPortTest, DefaultConstructor)
{
    EXPECT_EQ(port->name(), "");
    const Cn::Config& cfg = port->config();
    EXPECT_TRUE(cfg.empty());
}

// Test name setter and getter (inherited from CnCfgBase)
TEST_F(CnCfgPortTest, SetAndGetName)
{
    port->setName("TestPort");
    EXPECT_EQ(port->name(), "TestPort");
}

// Test name with various valid characters
TEST_F(CnCfgPortTest, SetNameWithValidCharacters)
{
    port->setName("Port_123");
    EXPECT_EQ(port->name(), "Port_123");
    
    port->setName("Port.Main");
    EXPECT_EQ(port->name(), "Port.Main");
    
    port->setName("_UnderscoreStart");
    EXPECT_EQ(port->name(), "_UnderscoreStart");
}

// Test setConfig with Name
TEST_F(CnCfgPortTest, SetConfigWithName)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port1");

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "Port1");
}

// Test setConfig with Name and other parameters
TEST_F(CnCfgPortTest, SetConfigWithNameAndParameters)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port2");
    cfg[CnSTR("Type")] = CnSTR("TCP");
    cfg[CnSTR("Host")] = CnSTR("127.0.0.1");
    cfg[CnSTR("Port")] = CnVariant(502);

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "Port2");
    
    const Cn::Config& resultConfig = port->config();
    EXPECT_NE(resultConfig.find(CnSTR("Type")), resultConfig.end());
    EXPECT_EQ(resultConfig.at(CnSTR("Type")).toString(), "TCP");
    EXPECT_NE(resultConfig.find(CnSTR("Host")), resultConfig.end());
    EXPECT_EQ(resultConfig.at(CnSTR("Host")).toString(), "127.0.0.1");
    EXPECT_NE(resultConfig.find(CnSTR("Port")), resultConfig.end());
    EXPECT_EQ(resultConfig.at(CnSTR("Port")).toInt(), 502);
}

// Test setConfig with empty config
TEST_F(CnCfgPortTest, SetConfigEmpty)
{
    Cn::Config cfg;

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "");
    const Cn::Config& resultConfig = port->config();
    EXPECT_TRUE(resultConfig.empty());
}

// Test setConfig preserves all parameters
TEST_F(CnCfgPortTest, SetConfigPreservesParameters)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port3");
    cfg[CnSTR("Enable")] = CnVariant(true);
    cfg[CnSTR("Type")] = CnSTR("RTU");
    cfg[CnSTR("Device")] = CnSTR("/dev/ttyS0");
    cfg[CnSTR("BaudRate")] = CnVariant(9600);
    cfg[CnSTR("DataBits")] = CnVariant(8);
    cfg[CnSTR("Parity")] = CnSTR("None");
    cfg[CnSTR("StopBits")] = CnVariant(1);
    cfg[CnSTR("Timeout")] = CnVariant(1000);

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Enable")).toBool(), true);
    EXPECT_EQ(resultConfig.at(CnSTR("Type")).toString(), "RTU");
    EXPECT_EQ(resultConfig.at(CnSTR("Device")).toString(), "/dev/ttyS0");
    EXPECT_EQ(resultConfig.at(CnSTR("BaudRate")).toInt(), 9600);
    EXPECT_EQ(resultConfig.at(CnSTR("DataBits")).toInt(), 8);
    EXPECT_EQ(resultConfig.at(CnSTR("Parity")).toString(), "None");
    EXPECT_EQ(resultConfig.at(CnSTR("StopBits")).toInt(), 1);
    EXPECT_EQ(resultConfig.at(CnSTR("Timeout")).toInt(), 1000);
}

// Test config getter
TEST_F(CnCfgPortTest, GetConfig)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port4");
    cfg[CnSTR("CustomParam")] = CnVariant(123);

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_NE(resultConfig.find(CnSTR("CustomParam")), resultConfig.end());
    EXPECT_EQ(resultConfig.at(CnSTR("CustomParam")).toInt(), 123);
}

// Test with different data types in config
TEST_F(CnCfgPortTest, SetConfigWithVariousDataTypes)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port5");
    cfg[CnSTR("IntValue")] = CnVariant(42);
    cfg[CnSTR("DoubleValue")] = CnVariant(3.14);
    cfg[CnSTR("BoolValue")] = CnVariant(true);
    cfg[CnSTR("StringValue")] = CnSTR("TestString");

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("IntValue")).toInt(), 42);
    EXPECT_DOUBLE_EQ(resultConfig.at(CnSTR("DoubleValue")).toDouble(), 3.14);
    EXPECT_EQ(resultConfig.at(CnSTR("BoolValue")).toBool(), true);
    EXPECT_EQ(resultConfig.at(CnSTR("StringValue")).toString(), "TestString");
}

// Test pointer type (CnCfgPortPtr)
TEST_F(CnCfgPortTest, SharedPointerType)
{
    CnCfgPortPtr ptr(new CnCfgPort());
    ptr->setName("SharedPort");

    EXPECT_EQ(ptr->name(), "SharedPort");
}

// Test config preservation after setConfig
TEST_F(CnCfgPortTest, ConfigPreservation)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port6");
    cfg[CnSTR("Param1")] = CnVariant(100);
    cfg[CnSTR("Param2")] = CnSTR("TestValue");

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Param1")).toInt(), 100);
    EXPECT_EQ(resultConfig.at(CnSTR("Param2")).toString(), "TestValue");
}

// Test setConfig overrides previous configuration
TEST_F(CnCfgPortTest, SetConfigOverride)
{
    Cn::Config cfg1;
    cfg1[CnSTR("Name")] = CnSTR("FirstPort");
    cfg1[CnSTR("Type")] = CnSTR("TCP");

    port->setConfig(cfg1);
    EXPECT_EQ(port->name(), "FirstPort");

    Cn::Config cfg2;
    cfg2[CnSTR("Name")] = CnSTR("SecondPort");
    cfg2[CnSTR("Type")] = CnSTR("RTU");

    port->setConfig(cfg2);
    EXPECT_EQ(port->name(), "SecondPort");
    
    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Type")).toString(), "RTU");
}

// Test TCP port configuration
TEST_F(CnCfgPortTest, TCPPortConfiguration)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("TCPPort");
    cfg[CnSTR("Type")] = CnSTR("TCP");
    cfg[CnSTR("Host")] = CnSTR("192.168.1.100");
    cfg[CnSTR("Port")] = CnVariant(502);
    cfg[CnSTR("Timeout")] = CnVariant(3000);

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "TCPPort");
    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Type")).toString(), "TCP");
    EXPECT_EQ(resultConfig.at(CnSTR("Host")).toString(), "192.168.1.100");
    EXPECT_EQ(resultConfig.at(CnSTR("Port")).toInt(), 502);
    EXPECT_EQ(resultConfig.at(CnSTR("Timeout")).toInt(), 3000);
}

// Test RTU port configuration
TEST_F(CnCfgPortTest, RTUPortConfiguration)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("RTUPort");
    cfg[CnSTR("Type")] = CnSTR("RTU");
    cfg[CnSTR("Device")] = CnSTR("COM1");
    cfg[CnSTR("BaudRate")] = CnVariant(115200);
    cfg[CnSTR("DataBits")] = CnVariant(8);
    cfg[CnSTR("Parity")] = CnSTR("Even");
    cfg[CnSTR("StopBits")] = CnVariant(1);

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "RTUPort");
    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Type")).toString(), "RTU");
    EXPECT_EQ(resultConfig.at(CnSTR("Device")).toString(), "COM1");
    EXPECT_EQ(resultConfig.at(CnSTR("BaudRate")).toInt(), 115200);
    EXPECT_EQ(resultConfig.at(CnSTR("DataBits")).toInt(), 8);
    EXPECT_EQ(resultConfig.at(CnSTR("Parity")).toString(), "Even");
    EXPECT_EQ(resultConfig.at(CnSTR("StopBits")).toInt(), 1);
}

// Test ASCII port configuration
TEST_F(CnCfgPortTest, ASCIIPortConfiguration)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("ASCIIPort");
    cfg[CnSTR("Type")] = CnSTR("ASCII");
    cfg[CnSTR("Device")] = CnSTR("/dev/ttyUSB0");
    cfg[CnSTR("BaudRate")] = CnVariant(19200);

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "ASCIIPort");
    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Type")).toString(), "ASCII");
    EXPECT_EQ(resultConfig.at(CnSTR("Device")).toString(), "/dev/ttyUSB0");
    EXPECT_EQ(resultConfig.at(CnSTR("BaudRate")).toInt(), 19200);
}

// Test empty name handling
TEST_F(CnCfgPortTest, EmptyName)
{
    port->setName("");
    EXPECT_EQ(port->name(), "");
}

// Test name with spaces (unusual but valid for config)
TEST_F(CnCfgPortTest, SetConfigWithSpacesInName)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Port With Spaces");

    port->setConfig(cfg);

    EXPECT_EQ(port->name(), "Port With Spaces");
}

// Test multiple setConfig calls
TEST_F(CnCfgPortTest, MultipleSetConfigCalls)
{
    Cn::Config cfg1;
    cfg1[CnSTR("Name")] = CnSTR("Port1");
    cfg1[CnSTR("Param1")] = CnVariant(1);

    port->setConfig(cfg1);
    EXPECT_EQ(port->name(), "Port1");
    EXPECT_EQ(port->config().at(CnSTR("Param1")).toInt(), 1);

    Cn::Config cfg2;
    cfg2[CnSTR("Name")] = CnSTR("Port2");
    cfg2[CnSTR("Param2")] = CnVariant(2);

    port->setConfig(cfg2);
    EXPECT_EQ(port->name(), "Port2");
    
    const Cn::Config& resultConfig = port->config();
    // Param1 should not exist after second setConfig
    EXPECT_EQ(resultConfig.find(CnSTR("Param1")), resultConfig.end());
    EXPECT_NE(resultConfig.find(CnSTR("Param2")), resultConfig.end());
    EXPECT_EQ(resultConfig.at(CnSTR("Param2")).toInt(), 2);
}

// Test config with enable/disable flag
TEST_F(CnCfgPortTest, EnableDisableConfiguration)
{
    Cn::Config cfg1;
    cfg1[CnSTR("Name")] = CnSTR("EnabledPort");
    cfg1[CnSTR("Enable")] = CnVariant(true);

    port->setConfig(cfg1);
    EXPECT_EQ(port->config().at(CnSTR("Enable")).toBool(), true);

    Cn::Config cfg2;
    cfg2[CnSTR("Name")] = CnSTR("DisabledPort");
    cfg2[CnSTR("Enable")] = CnVariant(false);

    port->setConfig(cfg2);
    EXPECT_EQ(port->config().at(CnSTR("Enable")).toBool(), false);
}

// Test config with RepeatCount parameter
TEST_F(CnCfgPortTest, RepeatCountConfiguration)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("PortWithRetry");
    cfg[CnSTR("RepeatCount")] = CnVariant(3);

    port->setConfig(cfg);

    EXPECT_EQ(port->config().at(CnSTR("RepeatCount")).toInt(), 3);
}

// Test config with timeout values
TEST_F(CnCfgPortTest, TimeoutConfiguration)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("PortWithTimeout");
    cfg[CnSTR("Timeout")] = CnVariant(5000);
    cfg[CnSTR("ResponseTimeout")] = CnVariant(2000);

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Timeout")).toInt(), 5000);
    EXPECT_EQ(resultConfig.at(CnSTR("ResponseTimeout")).toInt(), 2000);
}

// Test config with special characters in string values
TEST_F(CnCfgPortTest, SpecialCharactersInConfig)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("SpecialPort");
    cfg[CnSTR("Description")] = CnSTR("Port with special chars: @#$%^&*()");

    port->setConfig(cfg);

    EXPECT_EQ(port->config().at(CnSTR("Description")).toString(), "Port with special chars: @#$%^&*()");
}

// Test large config with many parameters
TEST_F(CnCfgPortTest, LargeConfiguration)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("ComplexPort");
    
    for (int i = 0; i < 20; ++i)
    {
        CnString key = CnSTR("Param") + Cn::toString(i);
        cfg[key] = CnVariant(i * 10);
    }

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.size(), 21); // 20 params + Name
    
    for (int i = 0; i < 20; ++i)
    {
        CnString key = CnSTR("Param") + Cn::toString(i);
        EXPECT_EQ(resultConfig.at(key).toInt(), i * 10);
    }
}

// Test config with zero values
TEST_F(CnCfgPortTest, ZeroValues)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("ZeroPort");
    cfg[CnSTR("Port")] = CnVariant(0);
    cfg[CnSTR("Timeout")] = CnVariant(0);

    port->setConfig(cfg);

    const Cn::Config& resultConfig = port->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Port")).toInt(), 0);
    EXPECT_EQ(resultConfig.at(CnSTR("Timeout")).toInt(), 0);
}

// Test config with negative values
TEST_F(CnCfgPortTest, NegativeValues)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("NegativePort");
    cfg[CnSTR("Offset")] = CnVariant(-100);

    port->setConfig(cfg);

    EXPECT_EQ(port->config().at(CnSTR("Offset")).toInt(), -100);
}

// Test shared pointer reference counting
TEST_F(CnCfgPortTest, SharedPointerRefCounting)
{
    CnCfgPortPtr ptr1(new CnCfgPort());
    ptr1->setName("RefCountPort");
    
    CnCfgPortPtr ptr2 = ptr1;
    EXPECT_EQ(ptr2->name(), "RefCountPort");
    
    ptr1->setName("ModifiedName");
    EXPECT_EQ(ptr2->name(), "ModifiedName");
}

// Test config const correctness
TEST_F(CnCfgPortTest, ConfigConstCorrectness)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("ConstPort");
    cfg[CnSTR("Value")] = CnVariant(999);

    port->setConfig(cfg);

    const CnCfgPort* constPort = port;
    const Cn::Config& resultConfig = constPort->config();
    
    EXPECT_EQ(resultConfig.at(CnSTR("Value")).toInt(), 999);
    EXPECT_EQ(constPort->name(), "ConstPort");
}
