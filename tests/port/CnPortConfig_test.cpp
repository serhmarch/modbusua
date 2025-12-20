#include "gtest/gtest.h"
#include <port/CnPortConfig.h>

TEST(CnPortConfigTest, SetAndGetConfig)
{
    CnPortConfig config;
    config.setCfgName(CnSTR("TestPort"));
    config.setCfgEnable(true);
    config.setCfgTimeout(5000);

    EXPECT_EQ(config.CfgName(), CnSTR("TestPort"));
    EXPECT_EQ(config.CfgEnable(), true);
    EXPECT_EQ(config.CfgTimeout(), 5000);
}

TEST(CnPortConfigTest, SetAndGetAllConfigValues)
{
    CnPortConfig config;
    config.setCfgName(CnSTR("Port1"));
    config.setCfgEnable(true);
    config.setCfgType(CnSTR("TCP"));
    config.setCfgHost(CnSTR("192.168.1.100"));
    config.setCfgPort(502);
    config.setCfgTimeout(3000);
    config.setCfgSerialPortName(CnSTR("COM1"));
    config.setCfgBaudRate(9600);
    config.setCfgDataBits(8);
    config.setCfgParity(CnSTR("None"));
    config.setCfgStopBits(CnSTR("One"));
    config.setCfgFlowControl(CnSTR("None"));
    config.setCfgTimeoutFirstByte(1000);
    config.setCfgTimeoutInterByte(100);

    EXPECT_EQ(config.CfgName(), CnSTR("Port1"));
    EXPECT_EQ(config.CfgEnable(), true);
    EXPECT_EQ(config.CfgType(), CnSTR("TCP"));
    EXPECT_EQ(config.CfgHost(), CnSTR("192.168.1.100"));
    EXPECT_EQ(config.CfgPort(), 502);
    EXPECT_EQ(config.CfgTimeout(), 3000);
    EXPECT_EQ(config.CfgSerialPortName(), CnSTR("COM1"));
    EXPECT_EQ(config.CfgBaudRate(), 9600);
    EXPECT_EQ(config.CfgDataBits(), 8);
    EXPECT_EQ(config.CfgParity(), CnSTR("No"));
    EXPECT_EQ(config.CfgStopBits(), CnSTR("1"));
    EXPECT_EQ(config.CfgFlowControl(), CnSTR("No"));
    EXPECT_EQ(config.CfgTimeoutFirstByte(), 1000);
    EXPECT_EQ(config.CfgTimeoutInterByte(), 100);
}

TEST(CnPortConfigTest, ConversionFunctions)
{
    EXPECT_EQ(CnPortConfig::toCfgName(CnSTR("PortName")), CnSTR("PortName"));
    EXPECT_EQ(CnPortConfig::toCfgEnable(true), true);
    EXPECT_EQ(CnPortConfig::toCfgHost(CnSTR("localhost")), CnSTR("localhost"));
    EXPECT_EQ(CnPortConfig::toCfgPort(502), 502);
    EXPECT_EQ(CnPortConfig::toCfgTimeout(5000), 5000);
}

TEST(CnPortConfigTest, ConversionFunctionsForAll)
{
    EXPECT_EQ(CnPortConfig::toCfgName(CnSTR("TestName")), CnSTR("TestName"));
    EXPECT_EQ(CnPortConfig::toCfgEnable(false), false);
    EXPECT_EQ(CnPortConfig::toCfgType(Modbus::ProtocolType::TCP), CnSTR("TCP"));
    EXPECT_EQ(CnPortConfig::toCfgHost(CnSTR("127.0.0.1")), CnSTR("127.0.0.1"));
    EXPECT_EQ(CnPortConfig::toCfgPort(503), 503);
    EXPECT_EQ(CnPortConfig::toCfgTimeout(10000), 10000);
    EXPECT_EQ(CnPortConfig::toCfgSerialPortName(CnSTR("COM2")), CnSTR("COM2"));
    EXPECT_EQ(CnPortConfig::toCfgBaudRate(19200), 19200);
    EXPECT_EQ(CnPortConfig::toCfgDataBits(7), 7);
    EXPECT_EQ(CnPortConfig::toCfgParity(Modbus::EvenParity), CnSTR("Even"));
    EXPECT_EQ(CnPortConfig::toCfgStopBits(Modbus::TwoStop), CnSTR("2"));
    EXPECT_EQ(CnPortConfig::toCfgFlowControl(Modbus::HardwareControl), CnSTR("Hard"));
    EXPECT_EQ(CnPortConfig::toCfgTimeoutFirstByte(2000), 2000);
    EXPECT_EQ(CnPortConfig::toCfgTimeoutInterByte(200), 200);
}

TEST(CnPortConfigTest, GetInnerValues)
{
    CnPortConfig config;
    config.setCfgName(CnSTR("InnerTest"));
    config.setCfgEnable(true);
    config.setCfgTimeout(3000);

    EXPECT_EQ(config.innerName(), CnSTR("InnerTest"));
    EXPECT_EQ(config.innerEnable(), true);
    EXPECT_EQ(config.innerTimeoutMillisec(), 3000);
}

TEST(CnPortConfigTest, InnerValuesAfterSet)
{
    CnPortConfig config;
    config.setCfgName(CnSTR("InnerPort"));
    config.setCfgEnable(false);
    config.setCfgType(CnSTR("RTU"));
    config.setCfgHost(CnSTR("192.168.0.1"));
    config.setCfgPort(503);
    config.setCfgTimeout(2000);
    config.setCfgSerialPortName(CnSTR("COM3"));
    config.setCfgBaudRate(115200);
    config.setCfgDataBits(8);
    config.setCfgParity(CnSTR("Odd"));
    config.setCfgStopBits(CnSTR("1"));
    config.setCfgFlowControl(CnSTR("Soft"));
    config.setCfgTimeoutFirstByte(500);
    config.setCfgTimeoutInterByte(50);

    EXPECT_EQ(config.innerName(), CnSTR("InnerPort"));
    EXPECT_EQ(config.innerEnable(), false);
    EXPECT_EQ(config.innerTypeEnum(), Modbus::ProtocolType::RTU);
    EXPECT_EQ(config.innerHost(), CnSTR("192.168.0.1"));
    EXPECT_EQ(config.innerPort(), 503);
    EXPECT_EQ(config.innerTimeoutMillisec(), 2000);
    EXPECT_EQ(config.innerSerialPortName(), CnSTR("COM3"));
    EXPECT_EQ(config.innerBaudRate(), 115200);
    EXPECT_EQ(config.innerDataBits(), 8);
    EXPECT_EQ(config.innerParityEnum(), Modbus::OddParity);
    EXPECT_EQ(config.innerStopBitsEnum(), Modbus::OneStop);
    EXPECT_EQ(config.innerFlowControlEnum(), Modbus::SoftwareControl);
    EXPECT_EQ(config.innerTimeoutFirstByteMillisec(), 500);
    EXPECT_EQ(config.innerTimeoutInterByteMillisec(), 50);
}

TEST(CnPortConfigTest, ConfigMapIntegration)
{
    Cn::Config cfg;
    const CnPortConfig::Strings &s = CnPortConfig::Strings::instance();
    cfg[s.Name] = CnSTR("MapPort");
    cfg[s.Enable] = true;
    cfg[s.Type] = CnSTR("TCP");
    cfg[s.Host] = CnSTR("10.0.0.1");
    cfg[s.Port] = 502;
    cfg[s.Timeout] = 4000;
    cfg[s.LogFileMaxSize] = CnSTR("5 MB");

    CnPortConfig config;
    config.setConfig(cfg);

    EXPECT_EQ(config.CfgName(), CnSTR("MapPort"));
    EXPECT_EQ(config.CfgEnable(), true);
    EXPECT_EQ(config.CfgType(), CnSTR("TCP"));
    EXPECT_EQ(config.CfgHost(), CnSTR("10.0.0.1"));
    EXPECT_EQ(config.CfgPort(), 502);
    EXPECT_EQ(config.CfgTimeout(), 4000);
    EXPECT_EQ(config.CfgLogFileMaxSize(), 5 * 1024 * 1024);
}
