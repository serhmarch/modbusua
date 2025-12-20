#include <gtest/gtest.h>
#include <core/CnModbus.h>
#include <core/CnUtils.h>

// ============================================================================
// Cn::Strings Tests
// ============================================================================

TEST(CnModbusStringsTest, Instance)
{
    const Cn::Strings &s1 = Cn::Strings::instance();
    const Cn::Strings &s2 = Cn::Strings::instance();
    
    EXPECT_EQ(&s1, &s2); // Singleton
}

TEST(CnModbusStringsTest, ConfigKeys)
{
    const Cn::Strings &s = Cn::Strings::instance();
    
    EXPECT_EQ(s.Unit, CnSTR("ModbusUnit"));
    EXPECT_EQ(s.Type, CnSTR("Type"));
    EXPECT_EQ(s.Tries, CnSTR("Tries"));
    EXPECT_EQ(s.RepeatCount, CnSTR("RepeatCount"));
    EXPECT_EQ(s.Host, CnSTR("Host"));
    EXPECT_EQ(s.Port, CnSTR("Port"));
    EXPECT_EQ(s.Timeout, CnSTR("Timeout"));
}

TEST(CnModbusStringsTest, SerialPortKeys)
{
    const Cn::Strings &s = Cn::Strings::instance();
    
    EXPECT_EQ(s.SerialPortName, CnSTR("SerialPortName"));
    EXPECT_EQ(s.BaudRate, CnSTR("BaudRate"));
    EXPECT_EQ(s.DataBits, CnSTR("DataBits"));
    EXPECT_EQ(s.Parity, CnSTR("Parity"));
    EXPECT_EQ(s.StopBits, CnSTR("StopBits"));
    EXPECT_EQ(s.FlowControl, CnSTR("FlowControl"));
    EXPECT_EQ(s.TimeoutFirstByte, CnSTR("TimeoutFirstByte"));
    EXPECT_EQ(s.TimeoutInterByte, CnSTR("TimeoutInterByte"));
}

TEST(CnModbusStringsTest, ProtocolTypeStrings)
{
    const Cn::Strings &s = Cn::Strings::instance();
    
    EXPECT_EQ(s.ProtocolTCP, CnSTR("TCP"));
    EXPECT_EQ(s.ProtocolRTU, CnSTR("RTU"));
    EXPECT_EQ(s.ProtocolASC, CnSTR("ASC"));
}

TEST(CnModbusStringsTest, ParityStrings)
{
    const Cn::Strings &s = Cn::Strings::instance();
    
    EXPECT_EQ(s.NoParity, CnSTR("No"));
    EXPECT_EQ(s.EvenParity, CnSTR("Even"));
    EXPECT_EQ(s.OddParity, CnSTR("Odd"));
    EXPECT_EQ(s.SpaceParity, CnSTR("Space"));
    EXPECT_EQ(s.MarkParity, CnSTR("Mark"));
}

TEST(CnModbusStringsTest, StopBitsStrings)
{
    const Cn::Strings &s = Cn::Strings::instance();
    
    EXPECT_EQ(s.OneStop, CnSTR("1"));
    EXPECT_EQ(s.OneAndHalfStop, CnSTR("1.5"));
    EXPECT_EQ(s.TwoStop, CnSTR("2"));
}

TEST(CnModbusStringsTest, FlowControlStrings)
{
    const Cn::Strings &s = Cn::Strings::instance();
    
    EXPECT_EQ(s.NoFlowControl, CnSTR("No"));
    EXPECT_EQ(s.HardwareControl, CnSTR("Hard"));
    EXPECT_EQ(s.SoftwareControl, CnSTR("Soft"));
}

// ============================================================================
// Cn::Defaults Tests
// ============================================================================

TEST(CnModbusDefaultsTest, Instance)
{
    const Cn::Defaults &d1 = Cn::Defaults::instance();
    const Cn::Defaults &d2 = Cn::Defaults::instance();
    
    EXPECT_EQ(&d1, &d2); // Singleton
}

TEST(CnModbusDefaultsTest, DefaultValues)
{
    const Cn::Defaults &d = Cn::Defaults::instance();
    
    EXPECT_EQ(d.Unit, 1);
    EXPECT_EQ(d.Type, Modbus::TCP);
    EXPECT_EQ(d.Tries, 1);
    EXPECT_EQ(d.RepeatCount, d.Tries);
    EXPECT_EQ(d.Port, 502); // Standard Modbus TCP port
}

// ============================================================================
// toProtocolType Tests
// ============================================================================

TEST(CnModbusConversionTest, ToProtocolTypeFromInt)
{
    bool ok;
    
    EXPECT_EQ(Cn::toProtocolType(Modbus::TCP, &ok), Modbus::TCP);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toProtocolType(Modbus::RTU, &ok), Modbus::RTU);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toProtocolType(Modbus::ASC, &ok), Modbus::ASC);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToProtocolTypeFromIntInvalid)
{
    bool ok;
    
    Cn::toProtocolType(999, &ok);
    EXPECT_FALSE(ok);
    
    Cn::toProtocolType(-1, &ok);
    EXPECT_FALSE(ok);
}

TEST(CnModbusConversionTest, ToProtocolTypeFromString)
{
    bool ok;
    
    CnString tcpStr = CnSTR("TCP");
    EXPECT_EQ(Cn::toProtocolType(tcpStr, &ok), Modbus::TCP);
    EXPECT_TRUE(ok);
    
    CnString rtuStr = CnSTR("RTU");
    EXPECT_EQ(Cn::toProtocolType(rtuStr, &ok), Modbus::RTU);
    EXPECT_TRUE(ok);
    
    CnString ascStr = CnSTR("ASC");
    EXPECT_EQ(Cn::toProtocolType(ascStr, &ok), Modbus::ASC);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToProtocolTypeFromStringInvalid)
{
    bool ok;
    
    CnString invalidStr = CnSTR("INVALID");
    Cn::toProtocolType(invalidStr, &ok);
    EXPECT_FALSE(ok);
    
    CnString emptyStr = CnSTR("");
    Cn::toProtocolType(emptyStr, &ok);
    EXPECT_FALSE(ok);
}

TEST(CnModbusConversionTest, ToProtocolTypeFromVariant)
{
    bool ok;
    
    EXPECT_EQ(Cn::toProtocolType(CnVariant(CnSTR("TCP")), &ok), Modbus::TCP);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toProtocolType(CnVariant(Modbus::RTU), &ok), Modbus::RTU);
    EXPECT_TRUE(ok);
}

// ============================================================================
// toBaudRate Tests
// ============================================================================

TEST(CnModbusConversionTest, ToBaudRateFromString)
{
    bool ok;
    
    CnString str1 = CnSTR("9600");
    EXPECT_EQ(Cn::toBaudRate(str1, &ok), 9600);
    EXPECT_TRUE(ok);
    
    CnString str2 = CnSTR("115200");
    EXPECT_EQ(Cn::toBaudRate(str2, &ok), 115200);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToBaudRateFromVariant)
{
    bool ok;
    
    EXPECT_EQ(Cn::toBaudRate(CnVariant(19200), &ok), 19200);
    EXPECT_TRUE(ok);
}

// ============================================================================
// toDataBits Tests
// ============================================================================

TEST(CnModbusConversionTest, ToDataBitsFromString)
{
    bool ok;
    
    CnString str8 = CnSTR("8");
    EXPECT_EQ(Cn::toDataBits(str8, &ok), 8);
    EXPECT_TRUE(ok);
    
    CnString str7 = CnSTR("7");
    EXPECT_EQ(Cn::toDataBits(str7, &ok), 7);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToDataBitsFromStringNamed)
{
    bool ok;
    
    CnString str8 = CnSTR("Data8");
    EXPECT_EQ(Cn::toDataBits(str8, &ok), 8);
    EXPECT_TRUE(ok);
    
    CnString str7 = CnSTR("Data7");
    EXPECT_EQ(Cn::toDataBits(str7, &ok), 7);
    EXPECT_TRUE(ok);
    
    CnString str6 = CnSTR("Data6");
    EXPECT_EQ(Cn::toDataBits(str6, &ok), 6);
    EXPECT_TRUE(ok);
    
    CnString str5 = CnSTR("Data5");
    EXPECT_EQ(Cn::toDataBits(str5, &ok), 5);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToDataBitsFromVariant)
{
    bool ok;
    
    EXPECT_EQ(Cn::toDataBits(CnVariant(8), &ok), 8);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toDataBits(CnVariant(CnSTR("Data7")), &ok), 7);
    EXPECT_TRUE(ok);
}

// ============================================================================
// toParity Tests
// ============================================================================

TEST(CnModbusConversionTest, ToParityFromInt)
{
    bool ok;
    
    EXPECT_EQ(Cn::toParity(Modbus::NoParity, &ok), Modbus::NoParity);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toParity(Modbus::EvenParity, &ok), Modbus::EvenParity);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toParity(Modbus::OddParity, &ok), Modbus::OddParity);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toParity(Modbus::SpaceParity, &ok), Modbus::SpaceParity);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toParity(Modbus::MarkParity, &ok), Modbus::MarkParity);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToParityFromString)
{
    bool ok;
    
    CnString noStr = CnSTR("No");
    EXPECT_EQ(Cn::toParity(noStr, &ok), Modbus::NoParity);
    EXPECT_TRUE(ok);
    
    CnString evenStr = CnSTR("Even");
    EXPECT_EQ(Cn::toParity(evenStr, &ok), Modbus::EvenParity);
    EXPECT_TRUE(ok);
    
    CnString oddStr = CnSTR("Odd");
    EXPECT_EQ(Cn::toParity(oddStr, &ok), Modbus::OddParity);
    EXPECT_TRUE(ok);
    
    CnString spaceStr = CnSTR("Space");
    EXPECT_EQ(Cn::toParity(spaceStr, &ok), Modbus::SpaceParity);
    EXPECT_TRUE(ok);
    
    CnString markStr = CnSTR("Mark");
    EXPECT_EQ(Cn::toParity(markStr, &ok), Modbus::MarkParity);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToParityFromVariant)
{
    bool ok;
    
    EXPECT_EQ(Cn::toParity(CnVariant(CnSTR("Even")), &ok), Modbus::EvenParity);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toParity(CnVariant(Modbus::OddParity), &ok), Modbus::OddParity);
    EXPECT_TRUE(ok);
}

// ============================================================================
// toStopBits Tests
// ============================================================================

TEST(CnModbusConversionTest, ToStopBitsFromInt)
{
    bool ok;
    
    EXPECT_EQ(Cn::toStopBits(Modbus::OneStop, &ok), Modbus::OneStop);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toStopBits(Modbus::OneAndHalfStop, &ok), Modbus::OneAndHalfStop);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toStopBits(Modbus::TwoStop, &ok), Modbus::TwoStop);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToStopBitsFromString)
{
    bool ok;
    
    CnString str1 = CnSTR("1");
    EXPECT_EQ(Cn::toStopBits(str1, &ok), Modbus::OneStop);
    EXPECT_TRUE(ok);
    
    CnString str15 = CnSTR("1.5");
    EXPECT_EQ(Cn::toStopBits(str15, &ok), Modbus::OneAndHalfStop);
    EXPECT_TRUE(ok);
    
    CnString str2 = CnSTR("2");
    EXPECT_EQ(Cn::toStopBits(str2, &ok), Modbus::TwoStop);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToStopBitsFromVariant)
{
    bool ok;
    
    EXPECT_EQ(Cn::toStopBits(CnVariant(CnSTR("2")), &ok), Modbus::TwoStop);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toStopBits(CnVariant(Modbus::OneStop), &ok), Modbus::OneStop);
    EXPECT_TRUE(ok);
}

// ============================================================================
// toFlowControl Tests
// ============================================================================

TEST(CnModbusConversionTest, ToFlowControlFromInt)
{
    bool ok;
    
    EXPECT_EQ(Cn::toFlowControl(Modbus::NoFlowControl, &ok), Modbus::NoFlowControl);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toFlowControl(Modbus::HardwareControl, &ok), Modbus::HardwareControl);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toFlowControl(Modbus::SoftwareControl, &ok), Modbus::SoftwareControl);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToFlowControlFromString)
{
    bool ok;
    
    CnString noStr = CnSTR("No");
    EXPECT_EQ(Cn::toFlowControl(noStr, &ok), Modbus::NoFlowControl);
    EXPECT_TRUE(ok);
    
    CnString hardStr = CnSTR("Hard");
    EXPECT_EQ(Cn::toFlowControl(hardStr, &ok), Modbus::HardwareControl);
    EXPECT_TRUE(ok);
    
    CnString softStr = CnSTR("Soft");
    EXPECT_EQ(Cn::toFlowControl(softStr, &ok), Modbus::SoftwareControl);
    EXPECT_TRUE(ok);
}

TEST(CnModbusConversionTest, ToFlowControlFromVariant)
{
    bool ok;
    
    EXPECT_EQ(Cn::toFlowControl(CnVariant(CnSTR("Hard")), &ok), Modbus::HardwareControl);
    EXPECT_TRUE(ok);
    
    EXPECT_EQ(Cn::toFlowControl(CnVariant(Modbus::SoftwareControl), &ok), Modbus::SoftwareControl);
    EXPECT_TRUE(ok);
}

// ============================================================================
// toString Tests
// ============================================================================

TEST(CnModbusConversionTest, ToStringProtocolType)
{
    EXPECT_EQ(Cn::toString(Modbus::TCP), CnSTR("TCP"));
    EXPECT_EQ(Cn::toString(Modbus::RTU), CnSTR("RTU"));
    EXPECT_EQ(Cn::toString(Modbus::ASC), CnSTR("ASC"));
}

TEST(CnModbusConversionTest, ToStringParity)
{
    EXPECT_EQ(Cn::toString(Modbus::NoParity), CnSTR("No"));
    EXPECT_EQ(Cn::toString(Modbus::EvenParity), CnSTR("Even"));
    EXPECT_EQ(Cn::toString(Modbus::OddParity), CnSTR("Odd"));
    EXPECT_EQ(Cn::toString(Modbus::SpaceParity), CnSTR("Space"));
    EXPECT_EQ(Cn::toString(Modbus::MarkParity), CnSTR("Mark"));
}

TEST(CnModbusConversionTest, ToStringStopBits)
{
    EXPECT_EQ(Cn::toString(Modbus::OneStop), CnSTR("1"));
    EXPECT_EQ(Cn::toString(Modbus::OneAndHalfStop), CnSTR("1.5"));
    EXPECT_EQ(Cn::toString(Modbus::TwoStop), CnSTR("2"));
}

TEST(CnModbusConversionTest, ToStringFlowControl)
{
    EXPECT_EQ(Cn::toString(Modbus::NoFlowControl), CnSTR("No"));
    EXPECT_EQ(Cn::toString(Modbus::HardwareControl), CnSTR("Hard"));
    EXPECT_EQ(Cn::toString(Modbus::SoftwareControl), CnSTR("Soft"));
}

// ============================================================================
// getCfg/setCfg Tests
// ============================================================================

TEST(CnModbusConfigTest, GetSetUnit)
{
    Cn::Config cfg;
    
    Cn::setCfgUnit(cfg, 5);
    EXPECT_EQ(Cn::getCfgUnit(cfg), 5);
}

TEST(CnModbusConfigTest, GetUnitDefault)
{
    Cn::Config cfg;
    bool ok;
    
    uint8_t value = Cn::getCfgUnit(cfg, &ok);
    EXPECT_FALSE(ok);
    EXPECT_EQ(value, Cn::Defaults::instance().Unit);
}

TEST(CnModbusConfigTest, GetSetType)
{
    Cn::Config cfg;
    
    Cn::setCfgType(cfg, Modbus::RTU);
    EXPECT_EQ(Cn::getCfgType(cfg), Modbus::RTU);
}

TEST(CnModbusConfigTest, GetSetTries)
{
    Cn::Config cfg;
    
    Cn::setCfgTries(cfg, 3);
    EXPECT_EQ(Cn::getCfgTries(cfg), 3);
}

TEST(CnModbusConfigTest, GetSetHost)
{
    Cn::Config cfg;
    
    Cn::setCfgHost(cfg, CnSTR("192.168.1.100"));
    EXPECT_EQ(Cn::getCfgHost(cfg), CnSTR("192.168.1.100"));
}

TEST(CnModbusConfigTest, GetSetPort)
{
    Cn::Config cfg;
    
    Cn::setCfgPort(cfg, 5020);
    EXPECT_EQ(Cn::getCfgPort(cfg), 5020);
}

TEST(CnModbusConfigTest, GetSetTimeout)
{
    Cn::Config cfg;
    
    Cn::setCfgTimeout(cfg, 5000);
    EXPECT_EQ(Cn::getCfgTimeout(cfg), 5000);
}

TEST(CnModbusConfigTest, GetSetSerialPortName)
{
    Cn::Config cfg;
    
    Cn::setCfgSerialPortName(cfg, CnSTR("COM3"));
    EXPECT_EQ(Cn::getCfgSerialPortName(cfg), CnSTR("COM3"));
}

TEST(CnModbusConfigTest, GetSetBaudRate)
{
    Cn::Config cfg;
    
    Cn::setCfgBaudRate(cfg, 19200);
    EXPECT_EQ(Cn::getCfgBaudRate(cfg), 19200);
}

TEST(CnModbusConfigTest, GetSetDataBits)
{
    Cn::Config cfg;
    
    Cn::setCfgDataBits(cfg, 7);
    EXPECT_EQ(Cn::getCfgDataBits(cfg), 7);
}

TEST(CnModbusConfigTest, GetSetParity)
{
    Cn::Config cfg;
    
    Cn::setCfgParity(cfg, Modbus::EvenParity);
    EXPECT_EQ(Cn::getCfgParity(cfg), Modbus::EvenParity);
}

TEST(CnModbusConfigTest, GetSetStopBits)
{
    Cn::Config cfg;
    
    Cn::setCfgStopBits(cfg, Modbus::TwoStop);
    EXPECT_EQ(Cn::getCfgStopBits(cfg), Modbus::TwoStop);
}

TEST(CnModbusConfigTest, GetSetFlowControl)
{
    Cn::Config cfg;
    
    Cn::setCfgFlowControl(cfg, Modbus::HardwareControl);
    EXPECT_EQ(Cn::getCfgFlowControl(cfg), Modbus::HardwareControl);
}

TEST(CnModbusConfigTest, GetSetTimeoutFirstByte)
{
    Cn::Config cfg;
    
    Cn::setCfgTimeoutFirstByte(cfg, 100);
    EXPECT_EQ(Cn::getCfgTimeoutFirstByte(cfg), 100);
}

TEST(CnModbusConfigTest, GetSetTimeoutInterByte)
{
    Cn::Config cfg;
    
    Cn::setCfgTimeoutInterByte(cfg, 50);
    EXPECT_EQ(Cn::getCfgTimeoutInterByte(cfg), 50);
}

TEST(CnModbusConfigTest, CompleteConfiguration)
{
    Cn::Config cfg;
    
    // TCP configuration
    Cn::setCfgUnit(cfg, 10);
    Cn::setCfgType(cfg, Modbus::TCP);
    Cn::setCfgHost(cfg, CnSTR("192.168.0.1"));
    Cn::setCfgPort(cfg, 502);
    Cn::setCfgTimeout(cfg, 3000);
    Cn::setCfgTries(cfg, 5);
    
    EXPECT_EQ(Cn::getCfgUnit(cfg), 10);
    EXPECT_EQ(Cn::getCfgType(cfg), Modbus::TCP);
    EXPECT_EQ(Cn::getCfgHost(cfg), CnSTR("192.168.0.1"));
    EXPECT_EQ(Cn::getCfgPort(cfg), 502);
    EXPECT_EQ(Cn::getCfgTimeout(cfg), 3000);
    EXPECT_EQ(Cn::getCfgTries(cfg), 5);
}

TEST(CnModbusConfigTest, SerialConfiguration)
{
    Cn::Config cfg;
    
    Cn::setCfgType(cfg, Modbus::RTU);
    Cn::setCfgSerialPortName(cfg, CnSTR("COM1"));
    Cn::setCfgBaudRate(cfg, 115200);
    Cn::setCfgDataBits(cfg, 8);
    Cn::setCfgParity(cfg, Modbus::NoParity);
    Cn::setCfgStopBits(cfg, Modbus::OneStop);
    Cn::setCfgFlowControl(cfg, Modbus::NoFlowControl);
    Cn::setCfgTimeoutFirstByte(cfg, 200);
    Cn::setCfgTimeoutInterByte(cfg, 100);
    
    EXPECT_EQ(Cn::getCfgType(cfg), Modbus::RTU);
    EXPECT_EQ(Cn::getCfgSerialPortName(cfg), CnSTR("COM1"));
    EXPECT_EQ(Cn::getCfgBaudRate(cfg), 115200);
    EXPECT_EQ(Cn::getCfgDataBits(cfg), 8);
    EXPECT_EQ(Cn::getCfgParity(cfg), Modbus::NoParity);
    EXPECT_EQ(Cn::getCfgStopBits(cfg), Modbus::OneStop);
    EXPECT_EQ(Cn::getCfgFlowControl(cfg), Modbus::NoFlowControl);
    EXPECT_EQ(Cn::getCfgTimeoutFirstByte(cfg), 200);
    EXPECT_EQ(Cn::getCfgTimeoutInterByte(cfg), 100);
}

TEST(CnModbusConfigTest, TriesBackwardCompatibility)
{
    Cn::Config cfg;
    
    // Set using old RepeatCount key
    cfg[Cn::Strings::instance().RepeatCount] = 7;
    
    // Should be retrieved as Tries
    EXPECT_EQ(Cn::getCfgTries(cfg), 7);
}

TEST(CnModbusConfigTest, TriesPriority)
{
    Cn::Config cfg;
    
    // Set both Tries and RepeatCount
    Cn::setCfgTries(cfg, 3);
    cfg[Cn::Strings::instance().RepeatCount] = 5;
    
    // Tries should have priority
    EXPECT_EQ(Cn::getCfgTries(cfg), 3);
}

// ============================================================================
// Address Tests
// ============================================================================

TEST(CnModbusAddressTest, AddressFromString)
{
    Cn::Address addr = Cn::addressFromString(CnSTR("400001"));
    EXPECT_EQ(addr.toInt(), 400001);
}

TEST(CnModbusAddressTest, AddressFromStringZero)
{
    CnString zeroStr = CnSTR("0");
    Modbus::Address addr = Cn::addressFromString(zeroStr);
    EXPECT_FALSE(addr.isValid());
}

TEST(CnModbusAddressTest, AddressFromStringLarge)
{
    Cn::Address addr = Cn::addressFromString(CnSTR("465536"));
    EXPECT_EQ(addr.toInt(), 465536);
}

// ============================================================================
// Conversion Edge Cases Tests
// ============================================================================

TEST(CnModbusConversionTest, ToProtocolTypeWithoutOkParam)
{
    EXPECT_EQ(Cn::toProtocolType(Modbus::TCP), Modbus::TCP);
    
    CnString rtuStr = CnSTR("RTU");
    EXPECT_EQ(Cn::toProtocolType(rtuStr), Modbus::RTU);
}

TEST(CnModbusConversionTest, ToBaudRateWithoutOkParam)
{
    CnString str9600 = CnSTR("9600");
    EXPECT_EQ(Cn::toBaudRate(str9600), 9600);
}

TEST(CnModbusConversionTest, ToDataBitsWithoutOkParam)
{
    CnString str8 = CnSTR("8");
    EXPECT_EQ(Cn::toDataBits(str8), 8);
}

TEST(CnModbusConversionTest, ToParityWithoutOkParam)
{
    CnString evenStr = CnSTR("Even");
    EXPECT_EQ(Cn::toParity(evenStr), Modbus::EvenParity);
}

TEST(CnModbusConversionTest, ToStopBitsWithoutOkParam)
{
    CnString str2 = CnSTR("2");
    EXPECT_EQ(Cn::toStopBits(str2), Modbus::TwoStop);
}

TEST(CnModbusConversionTest, ToFlowControlWithoutOkParam)
{
    CnString hardStr = CnSTR("Hard");
    EXPECT_EQ(Cn::toFlowControl(hardStr), Modbus::HardwareControl);
}

TEST(CnModbusConversionTest, GetCfgWithoutOkParam)
{
    Cn::Config cfg;
    Cn::setCfgUnit(cfg, 8);
    
    EXPECT_EQ(Cn::getCfgUnit(cfg), 8);
    EXPECT_EQ(Cn::getCfgType(cfg), Cn::Defaults::instance().Type);
}

// ============================================================================
// Boundary Value Tests
// ============================================================================

TEST(CnModbusConfigTest, UnitBoundaryValues)
{
    Cn::Config cfg;
    
    Cn::setCfgUnit(cfg, 0);
    EXPECT_EQ(Cn::getCfgUnit(cfg), 0);
    
    Cn::setCfgUnit(cfg, 255);
    EXPECT_EQ(Cn::getCfgUnit(cfg), 255);
}

TEST(CnModbusConfigTest, PortBoundaryValues)
{
    Cn::Config cfg;
    
    Cn::setCfgPort(cfg, 1);
    EXPECT_EQ(Cn::getCfgPort(cfg), 1);
    
    Cn::setCfgPort(cfg, 65535);
    EXPECT_EQ(Cn::getCfgPort(cfg), 65535);
}

TEST(CnModbusConfigTest, TimeoutZero)
{
    Cn::Config cfg;
    
    Cn::setCfgTimeout(cfg, 0);
    EXPECT_EQ(Cn::getCfgTimeout(cfg), 0);
}

TEST(CnModbusConfigTest, TriesZero)
{
    Cn::Config cfg;
    
    Cn::setCfgTries(cfg, 0);
    EXPECT_EQ(Cn::getCfgTries(cfg), 0);
}

TEST(CnModbusConfigTest, BaudRateCommonValues)
{
    Cn::Config cfg;
    
    int32_t baudRates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
    
    for (int32_t rate : baudRates)
    {
        Cn::setCfgBaudRate(cfg, rate);
        EXPECT_EQ(Cn::getCfgBaudRate(cfg), rate);
    }
}

TEST(CnModbusConfigTest, DataBitsAllValues)
{
    Cn::Config cfg;
    
    int8_t dataBits[] = {5, 6, 7, 8};
    
    for (int8_t bits : dataBits)
    {
        Cn::setCfgDataBits(cfg, bits);
        EXPECT_EQ(Cn::getCfgDataBits(cfg), bits);
    }
}

// ============================================================================
// String Conversion Round-trip Tests
// ============================================================================

TEST(CnModbusConversionTest, ProtocolTypeRoundTrip)
{
    Modbus::ProtocolType types[] = {Modbus::TCP, Modbus::RTU, Modbus::ASC};
    
    for (Modbus::ProtocolType type : types)
    {
        CnString str = Cn::toString(type);
        bool ok;
        Modbus::ProtocolType converted = Cn::toProtocolType(str, &ok);
        EXPECT_TRUE(ok);
        EXPECT_EQ(converted, type);
    }
}

TEST(CnModbusConversionTest, ParityRoundTrip)
{
    Modbus::Parity parities[] = {
        Modbus::NoParity, 
        Modbus::EvenParity, 
        Modbus::OddParity, 
        Modbus::SpaceParity, 
        Modbus::MarkParity
    };
    
    for (Modbus::Parity parity : parities)
    {
        CnString str = Cn::toString(parity);
        bool ok;
        Modbus::Parity converted = Cn::toParity(str, &ok);
        EXPECT_TRUE(ok);
        EXPECT_EQ(converted, parity);
    }
}

TEST(CnModbusConversionTest, StopBitsRoundTrip)
{
    Modbus::StopBits stopBits[] = {
        Modbus::OneStop, 
        Modbus::OneAndHalfStop, 
        Modbus::TwoStop
    };
    
    for (Modbus::StopBits bits : stopBits)
    {
        CnString str = Cn::toString(bits);
        bool ok;
        Modbus::StopBits converted = Cn::toStopBits(str, &ok);
        EXPECT_TRUE(ok);
        EXPECT_EQ(converted, bits);
    }
}

TEST(CnModbusConversionTest, FlowControlRoundTrip)
{
    Modbus::FlowControl controls[] = {
        Modbus::NoFlowControl, 
        Modbus::HardwareControl, 
        Modbus::SoftwareControl
    };
    
    for (Modbus::FlowControl control : controls)
    {
        CnString str = Cn::toString(control);
        bool ok;
        Modbus::FlowControl converted = Cn::toFlowControl(str, &ok);
        EXPECT_TRUE(ok);
        EXPECT_EQ(converted, control);
    }
}

// ============================================================================
// Empty/Invalid Configuration Tests
// ============================================================================

TEST(CnModbusConfigTest, EmptyConfig)
{
    Cn::Config cfg;
    bool ok;
    
    // All getCfg functions should return defaults and ok=false
    Cn::getCfgUnit(cfg, &ok);
    EXPECT_FALSE(ok);
    
    Cn::getCfgType(cfg, &ok);
    EXPECT_FALSE(ok);
    
    Cn::getCfgTries(cfg, &ok);
    EXPECT_FALSE(ok);
    
    Cn::getCfgHost(cfg, &ok);
    EXPECT_FALSE(ok);
    
    Cn::getCfgPort(cfg, &ok);
    EXPECT_FALSE(ok);
}

TEST(CnModbusConfigTest, InvalidTypeInConfig)
{
    Cn::Config cfg;
    cfg[Cn::Strings::instance().Type] = CnSTR("INVALID");
    
    bool ok;
    Cn::getCfgType(cfg, &ok);
    EXPECT_FALSE(ok);
}

TEST(CnModbusConfigTest, InvalidParityInConfig)
{
    Cn::Config cfg;
    cfg[Cn::Strings::instance().Parity] = CnSTR("INVALID");
    
    bool ok;
    Cn::getCfgParity(cfg, &ok);
    EXPECT_FALSE(ok);
}

// ============================================================================
// Multiple Configuration Sets Tests
// ============================================================================

TEST(CnModbusConfigTest, OverwriteValues)
{
    Cn::Config cfg;
    
    Cn::setCfgUnit(cfg, 1);
    EXPECT_EQ(Cn::getCfgUnit(cfg), 1);
    
    Cn::setCfgUnit(cfg, 5);
    EXPECT_EQ(Cn::getCfgUnit(cfg), 5);
    
    Cn::setCfgUnit(cfg, 10);
    EXPECT_EQ(Cn::getCfgUnit(cfg), 10);
}

TEST(CnModbusConfigTest, MixedProtocolSettings)
{
    Cn::Config cfg;
    
    // Set TCP settings
    Cn::setCfgType(cfg, Modbus::TCP);
    Cn::setCfgHost(cfg, CnSTR("10.0.0.1"));
    Cn::setCfgPort(cfg, 502);
    
    // Also set serial settings (should coexist)
    Cn::setCfgSerialPortName(cfg, CnSTR("COM1"));
    Cn::setCfgBaudRate(cfg, 9600);
    
    // Both should be retrievable
    EXPECT_EQ(Cn::getCfgType(cfg), Modbus::TCP);
    EXPECT_EQ(Cn::getCfgHost(cfg), CnSTR("10.0.0.1"));
    EXPECT_EQ(Cn::getCfgSerialPortName(cfg), CnSTR("COM1"));
}

// ============================================================================
// Special String Values Tests
// ============================================================================

TEST(CnModbusConfigTest, EmptyHostString)
{
    Cn::Config cfg;
    
    Cn::setCfgHost(cfg, CnSTR(""));
    EXPECT_EQ(Cn::getCfgHost(cfg), CnSTR(""));
}

TEST(CnModbusConfigTest, EmptySerialPortName)
{
    Cn::Config cfg;
    
    Cn::setCfgSerialPortName(cfg, CnSTR(""));
    EXPECT_EQ(Cn::getCfgSerialPortName(cfg), CnSTR(""));
}

TEST(CnModbusConfigTest, SpecialCharactersInHost)
{
    Cn::Config cfg;
    
    Cn::setCfgHost(cfg, CnSTR("192.168.1.1:502"));
    EXPECT_EQ(Cn::getCfgHost(cfg), CnSTR("192.168.1.1:502"));
}

TEST(CnModbusConfigTest, WindowsSerialPort)
{
    Cn::Config cfg;
    
    Cn::setCfgSerialPortName(cfg, CnSTR("COM10"));
    EXPECT_EQ(Cn::getCfgSerialPortName(cfg), CnSTR("COM10"));
}

TEST(CnModbusConfigTest, LinuxSerialPort)
{
    Cn::Config cfg;
    
    Cn::setCfgSerialPortName(cfg, CnSTR("/dev/ttyUSB0"));
    EXPECT_EQ(Cn::getCfgSerialPortName(cfg), CnSTR("/dev/ttyUSB0"));
}
