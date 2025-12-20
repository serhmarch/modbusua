#include "gtest/gtest.h"
#include <device/CnDeviceConfig.h>

TEST(CnDeviceConfigTest, SetAndGetConfig)
{
    CnDeviceConfig config;
    config.setCfgName("TestDevice");
    config.setCfgEnableDevice(true);

    EXPECT_EQ(config.CfgName(), "TestDevice");
    EXPECT_EQ(config.CfgEnableDevice(), true);
}

TEST(CnDeviceConfigTest, SetAndGetAllConfigValues)
{
    CnDeviceConfig config;
    config.setCfgName("Device1");
    config.setCfgEnableDevice(true);
    config.setCfgModbusUnit(1);
    config.setCfgRestoreTimeout(5000);

    EXPECT_EQ(config.CfgName(), "Device1");
    EXPECT_EQ(config.CfgEnableDevice(), true);
    EXPECT_EQ(config.CfgModbusUnit(), 1);
    EXPECT_EQ(config.CfgRestoreTimeout(), 5000);
}

TEST(CnDeviceConfigTest, ConversionFunctions)
{
    EXPECT_EQ(CnDeviceConfig::toCfgName("DeviceName"), "DeviceName");
    EXPECT_EQ(CnDeviceConfig::toCfgEnableDevice(true), true);
}

TEST(CnDeviceConfigTest, ConversionFunctionsForAll)
{
    EXPECT_EQ(CnDeviceConfig::toCfgName("TestName"), "TestName");
    EXPECT_EQ(CnDeviceConfig::toCfgEnableDevice(false), false);
    EXPECT_EQ(CnDeviceConfig::toCfgModbusUnit(2), 2);
    EXPECT_EQ(CnDeviceConfig::toCfgRestoreTimeout(3000), 3000);
}

TEST(CnDeviceConfigTest, GetInnerValues)
{
    CnDeviceConfig config;
    config.setCfgName("InnerTest");
    config.setCfgEnableDevice(true);

    EXPECT_EQ(config.innerName(), "InnerTest");
    EXPECT_EQ(config.innerEnableDevice(), true);
}

TEST(CnDeviceConfigTest, InnerValuesAfterSet)
{
    CnDeviceConfig config;
    config.setCfgName("InnerDevice");
    config.setCfgEnableDevice(false);
    config.setCfgModbusUnit(3);
    config.setCfgRestoreTimeout(1000);

    EXPECT_EQ(config.innerName(), "InnerDevice");
    EXPECT_EQ(config.innerEnableDevice(), false);
    EXPECT_EQ(config.innerModbusUnit(), 3);
    EXPECT_EQ(config.innerRestoreTimeoutMillisec(), 1000);
}

TEST(CnDeviceConfigTest, ConfigMapIntegration)
{
    Cn::Config cfg;
    const CnDeviceConfig::Strings &s = CnDeviceConfig::Strings::instance();
    cfg[s.Name] = "MapDevice";
    cfg[s.EnableDevice] = true;
    cfg[s.LogFileMaxSize ] = CnSTR("2 MB");


    CnDeviceConfig config;
    config.setConfig(cfg);

    EXPECT_EQ(config.CfgName(), "MapDevice");
    EXPECT_EQ(config.CfgEnableDevice(), true);
    EXPECT_EQ(config.CfgLogFileMaxSize(), 2 * 1024 * 1024);
}