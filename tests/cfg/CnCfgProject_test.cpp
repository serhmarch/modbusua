/*!
 * \file   CnCfgProject_test.cpp
 * \brief  Unit tests for CnCfgProject class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */
#include "gtest/gtest.h"
#include <cfg/CnCfgProject.h>
#include <cfg/CnCfgPort.h>
#include <cfg/CnCfgDevice.h>

class CnCfgProjectTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        project = new CnCfgProject();
    }

    void TearDown() override
    {
        delete project;
    }

    CnCfgProject* project;
};

// Test default constructor
TEST_F(CnCfgProjectTest, DefaultConstructor)
{
    EXPECT_EQ(project->name(), "");
    EXPECT_TRUE(project->ports().empty());
    EXPECT_TRUE(project->devices().empty());
    EXPECT_TRUE(project->groupConfig().empty());
}

// Test name setter and getter (inherited from CnCfgBase)
TEST_F(CnCfgProjectTest, SetAndGetName)
{
    project->setName("TestProject");
    EXPECT_EQ(project->name(), "TestProject");
}

// Test adding single port
TEST_F(CnCfgProjectTest, AddSinglePort)
{
    CnCfgPort* port = new CnCfgPort();
    port->setName("Port1");

    project->portAdd(port);

    EXPECT_EQ(project->ports().size(), 1);
    EXPECT_NE(project->port("Port1"), nullptr);
    EXPECT_EQ(project->port("Port1")->name(), "Port1");
}

// Test adding multiple ports
TEST_F(CnCfgProjectTest, AddMultiplePorts)
{
    CnCfgPort* port1 = new CnCfgPort();
    port1->setName("Port1");

    CnCfgPort* port2 = new CnCfgPort();
    port2->setName("Port2");

    CnCfgPort* port3 = new CnCfgPort();
    port3->setName("Port3");

    project->portAdd(port1);
    project->portAdd(port2);
    project->portAdd(port3);

    EXPECT_EQ(project->ports().size(), 3);
    EXPECT_NE(project->port("Port1"), nullptr);
    EXPECT_NE(project->port("Port2"), nullptr);
    EXPECT_NE(project->port("Port3"), nullptr);
}

// Test getting non-existent port
TEST_F(CnCfgProjectTest, GetNonExistentPort)
{
    EXPECT_EQ(project->port("NonExistent"), nullptr);
}

// Test adding single device
TEST_F(CnCfgProjectTest, AddSingleDevice)
{
    CnCfgDevice* device = new CnCfgDevice();
    device->setName("Device1");

    project->deviceAdd(device);

    EXPECT_EQ(project->devices().size(), 1);
    EXPECT_NE(project->device("Device1"), nullptr);
    EXPECT_EQ(project->device("Device1")->name(), "Device1");
}

// Test adding multiple devices
TEST_F(CnCfgProjectTest, AddMultipleDevices)
{
    CnCfgDevice* device1 = new CnCfgDevice();
    device1->setName("Device1");

    CnCfgDevice* device2 = new CnCfgDevice();
    device2->setName("Device2");

    CnCfgDevice* device3 = new CnCfgDevice();
    device3->setName("Device3");

    project->deviceAdd(device1);
    project->deviceAdd(device2);
    project->deviceAdd(device3);

    EXPECT_EQ(project->devices().size(), 3);
    EXPECT_NE(project->device("Device1"), nullptr);
    EXPECT_NE(project->device("Device2"), nullptr);
    EXPECT_NE(project->device("Device3"), nullptr);
}

// Test getting non-existent device
TEST_F(CnCfgProjectTest, GetNonExistentDevice)
{
    EXPECT_EQ(project->device("NonExistent"), nullptr);
}

// Test port retrieval by name
TEST_F(CnCfgProjectTest, PortRetrievalByName)
{
    CnCfgPort* port1 = new CnCfgPort();
    port1->setName("COM1");

    CnCfgPort* port2 = new CnCfgPort();
    port2->setName("COM2");

    project->portAdd(port1);
    project->portAdd(port2);

    CnCfgPortPtr retrieved1 = project->port("COM1");
    CnCfgPortPtr retrieved2 = project->port("COM2");

    EXPECT_NE(retrieved1, nullptr);
    EXPECT_NE(retrieved2, nullptr);
    EXPECT_EQ(retrieved1->name(), "COM1");
    EXPECT_EQ(retrieved2->name(), "COM2");
}

// Test device retrieval by name
TEST_F(CnCfgProjectTest, DeviceRetrievalByName)
{
    CnCfgDevice* device1 = new CnCfgDevice();
    device1->setName("PLC1");

    CnCfgDevice* device2 = new CnCfgDevice();
    device2->setName("PLC2");

    project->deviceAdd(device1);
    project->deviceAdd(device2);

    CnCfgDevicePtr retrieved1 = project->device("PLC1");
    CnCfgDevicePtr retrieved2 = project->device("PLC2");

    EXPECT_NE(retrieved1, nullptr);
    EXPECT_NE(retrieved2, nullptr);
    EXPECT_EQ(retrieved1->name(), "PLC1");
    EXPECT_EQ(retrieved2->name(), "PLC2");
}

// Test adding ports with configured settings
TEST_F(CnCfgProjectTest, AddPortWithConfiguration)
{
    CnCfgPort* port = new CnCfgPort();
    port->setName("TCPPort");

    Cn::Config cfg;
    cfg[CnSTR("Type")] = CnSTR("TCP");
    cfg[CnSTR("Host")] = CnSTR("127.0.0.1");
    cfg[CnSTR("Port")] = CnVariant(502);
    port->setConfig(cfg);

    project->portAdd(port);

    CnCfgPortPtr retrieved = project->port("TCPPort");
    EXPECT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->config().at(CnSTR("Type")).toString(), "TCP");
    EXPECT_EQ(retrieved->config().at(CnSTR("Host")).toString(), "127.0.0.1");
}

// Test adding devices with configured settings
TEST_F(CnCfgProjectTest, AddDeviceWithConfiguration)
{
    CnCfgDevice* device = new CnCfgDevice();
    device->setName("Controller1");
    device->setPortName("COM1");

    Cn::Config cfg;
    cfg[CnSTR("Unit")] = CnVariant(1);
    cfg[CnSTR("Enable")] = CnVariant(true);
    device->setConfig(cfg);

    project->deviceAdd(device);

    CnCfgDevicePtr retrieved = project->device("Controller1");
    EXPECT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->portName(), "COM1");
    EXPECT_EQ(retrieved->config().at(CnSTR("Unit")).toInt(), 1);
}

// Test setGroupConfig
TEST_F(CnCfgProjectTest, SetGroupConfig)
{
    Cn::GroupConfig groups;
    
    Cn::Config group1;
    group1[CnSTR("Param1")] = CnSTR("Value1");
    groups[CnSTR("Group1")] = group1;

    Cn::Config group2;
    group2[CnSTR("Param2")] = CnVariant(123);
    groups[CnSTR("Group2")] = group2;

    project->setGroupConfig(groups);

    const Cn::GroupConfig& result = project->groupConfig();
    EXPECT_EQ(result.size(), 2);
    EXPECT_NE(result.find(CnSTR("Group1")), result.end());
    EXPECT_NE(result.find(CnSTR("Group2")), result.end());
}

// Test configForGroup
TEST_F(CnCfgProjectTest, ConfigForGroup)
{
    Cn::GroupConfig groups;
    
    Cn::Config group1;
    group1[CnSTR("Setting1")] = CnSTR("Value1");
    group1[CnSTR("Setting2")] = CnVariant(42);
    groups[CnSTR("TestGroup")] = group1;

    project->setGroupConfig(groups);

    Cn::Config retrievedConfig = project->configForGroup("TestGroup");
    EXPECT_EQ(retrievedConfig.at(CnSTR("Setting1")).toString(), "Value1");
    EXPECT_EQ(retrievedConfig.at(CnSTR("Setting2")).toInt(), 42);
}

// Test configForGroup with non-existent group
TEST_F(CnCfgProjectTest, ConfigForGroupNonExistent)
{
    Cn::Config cfg = project->configForGroup("NonExistent");
    EXPECT_TRUE(cfg.empty());
}

// Test configPtrForGroup
TEST_F(CnCfgProjectTest, ConfigPtrForGroup)
{
    Cn::GroupConfig groups;
    
    Cn::Config group1;
    group1[CnSTR("Value")] = CnVariant(999);
    groups[CnSTR("TestGroup")] = group1;

    project->setGroupConfig(groups);

    const Cn::Config* ptr = project->configPtrForGroup("TestGroup");
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->at(CnSTR("Value")).toInt(), 999);
}

// Test configPtrForGroup with non-existent group
TEST_F(CnCfgProjectTest, ConfigPtrForGroupNonExistent)
{
    const Cn::Config* ptr = project->configPtrForGroup("NonExistent");
    EXPECT_EQ(ptr, nullptr);
}

// Test swapGroupConfig
TEST_F(CnCfgProjectTest, SwapGroupConfig)
{
    Cn::GroupConfig groups1;
    Cn::Config cfg1;
    cfg1[CnSTR("Param1")] = CnSTR("Original");
    groups1[CnSTR("Group1")] = cfg1;

    project->setGroupConfig(groups1);
    EXPECT_EQ(project->groupConfig().size(), 1);

    Cn::GroupConfig groups2;
    Cn::Config cfg2;
    cfg2[CnSTR("Param2")] = CnSTR("Swapped");
    groups2[CnSTR("Group2")] = cfg2;

    project->swapGroupConfig(groups2);

    // After swap, project should have Group2 and groups2 should have Group1
    EXPECT_EQ(project->groupConfig().size(), 1);
    EXPECT_NE(project->groupConfig().find(CnSTR("Group2")), project->groupConfig().end());
    EXPECT_EQ(groups2.size(), 1);
    EXPECT_NE(groups2.find(CnSTR("Group1")), groups2.end());
}

// Test groupConfig const and non-const accessors
TEST_F(CnCfgProjectTest, GroupConfigAccessors)
{
    Cn::GroupConfig groups;
    Cn::Config cfg;
    cfg[CnSTR("Test")] = CnVariant(123);
    groups[CnSTR("TestGroup")] = cfg;

    project->setGroupConfig(groups);

    // Non-const accessor
    Cn::GroupConfig& nonConstGroups = project->groupConfig();
    EXPECT_EQ(nonConstGroups.size(), 1);

    // Const accessor
    const CnCfgProject* constProject = project;
    const Cn::GroupConfig& constGroups = constProject->groupConfig();
    EXPECT_EQ(constGroups.size(), 1);
}

// Test complex project setup
TEST_F(CnCfgProjectTest, ComplexProjectSetup)
{
    // Set project name
    project->setName("IndustrialProject");

    // Add ports
    CnCfgPort* tcpPort = new CnCfgPort();
    tcpPort->setName("TCPPort1");
    Cn::Config tcpCfg;
    tcpCfg[CnSTR("Type")] = CnSTR("TCP");
    tcpCfg[CnSTR("Host")] = CnSTR("192.168.1.100");
    tcpCfg[CnSTR("Port")] = CnVariant(502);
    tcpPort->setConfig(tcpCfg);
    project->portAdd(tcpPort);

    CnCfgPort* serialPort = new CnCfgPort();
    serialPort->setName("SerialPort1");
    Cn::Config serialCfg;
    serialCfg[CnSTR("Type")] = CnSTR("RTU");
    serialCfg[CnSTR("Device")] = CnSTR("COM1");
    serialCfg[CnSTR("BaudRate")] = CnVariant(9600);
    serialPort->setConfig(serialCfg);
    project->portAdd(serialPort);

    // Add devices
    CnCfgDevice* device1 = new CnCfgDevice();
    device1->setName("PLC_Main");
    device1->setPortName("TCPPort1");
    project->deviceAdd(device1);

    CnCfgDevice* device2 = new CnCfgDevice();
    device2->setName("PLC_Secondary");
    device2->setPortName("SerialPort1");
    project->deviceAdd(device2);

    // Add groups
    Cn::GroupConfig groups;
    Cn::Config systemGroup;
    systemGroup[CnSTR("LogLevel")] = CnSTR("Debug");
    systemGroup[CnSTR("MaxConnections")] = CnVariant(10);
    groups[CnSTR("System")] = systemGroup;

    Cn::Config dbGroup;
    dbGroup[CnSTR("ConnectionString")] = CnSTR("localhost");
    dbGroup[CnSTR("Port")] = CnVariant(5432);
    groups[CnSTR("Database")] = dbGroup;

    project->setGroupConfig(groups);

    // Verify
    EXPECT_EQ(project->name(), "IndustrialProject");
    EXPECT_EQ(project->ports().size(), 2);
    EXPECT_EQ(project->devices().size(), 2);
    EXPECT_EQ(project->groupConfig().size(), 2);

    EXPECT_NE(project->port("TCPPort1"), nullptr);
    EXPECT_NE(project->port("SerialPort1"), nullptr);
    EXPECT_NE(project->device("PLC_Main"), nullptr);
    EXPECT_NE(project->device("PLC_Secondary"), nullptr);

    const Cn::Config* sysConfig = project->configPtrForGroup("System");
    EXPECT_NE(sysConfig, nullptr);
    EXPECT_EQ(sysConfig->at(CnSTR("LogLevel")).toString(), "Debug");
}

// Test ports list iteration
TEST_F(CnCfgProjectTest, PortsListIteration)
{
    CnCfgPort* port1 = new CnCfgPort();
    port1->setName("Port1");
    CnCfgPort* port2 = new CnCfgPort();
    port2->setName("Port2");
    CnCfgPort* port3 = new CnCfgPort();
    port3->setName("Port3");

    project->portAdd(port1);
    project->portAdd(port2);
    project->portAdd(port3);

    const CnList<CnCfgPortPtr>& ports = project->ports();
    EXPECT_EQ(ports.size(), 3);

    size_t count = 0;
    for (const auto& port : ports)
    {
        EXPECT_NE(port, nullptr);
        EXPECT_FALSE(port->name().empty());
        ++count;
    }
    EXPECT_EQ(count, 3);
}

// Test devices list iteration
TEST_F(CnCfgProjectTest, DevicesListIteration)
{
    CnCfgDevice* device1 = new CnCfgDevice();
    device1->setName("Device1");
    CnCfgDevice* device2 = new CnCfgDevice();
    device2->setName("Device2");
    CnCfgDevice* device3 = new CnCfgDevice();
    device3->setName("Device3");

    project->deviceAdd(device1);
    project->deviceAdd(device2);
    project->deviceAdd(device3);

    const CnList<CnCfgDevicePtr>& devices = project->devices();
    EXPECT_EQ(devices.size(), 3);

    size_t count = 0;
    for (const auto& device : devices)
    {
        EXPECT_NE(device, nullptr);
        EXPECT_FALSE(device->name().empty());
        ++count;
    }
    EXPECT_EQ(count, 3);
}

// Test empty project state
TEST_F(CnCfgProjectTest, EmptyProjectState)
{
    EXPECT_TRUE(project->ports().empty());
    EXPECT_TRUE(project->devices().empty());
    EXPECT_TRUE(project->groupConfig().empty());
    EXPECT_EQ(project->port("AnyName"), nullptr);
    EXPECT_EQ(project->device("AnyName"), nullptr);
    EXPECT_EQ(project->configPtrForGroup("AnyGroup"), nullptr);
}

// Test setConfig (inherited from CnCfgBase)
TEST_F(CnCfgProjectTest, SetConfig)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("MyProject");
    cfg[CnSTR("Version")] = CnSTR("1.0");
    cfg[CnSTR("Description")] = CnSTR("Test project");

    project->setConfig(cfg);

    EXPECT_EQ(project->name(), "MyProject");
    const Cn::Config& resultCfg = project->config();
    EXPECT_EQ(resultCfg.at(CnSTR("Version")).toString(), "1.0");
    EXPECT_EQ(resultCfg.at(CnSTR("Description")).toString(), "Test project");
}

// Test shared pointer type
TEST_F(CnCfgProjectTest, SharedPointerType)
{
    CnCfgProjectPtr ptr(new CnCfgProject());
    ptr->setName("SharedProject");

    EXPECT_EQ(ptr->name(), "SharedProject");
}

// Test adding ports and devices with same name (hash collision)
TEST_F(CnCfgProjectTest, DuplicateNames)
{
    CnCfgPort* port1 = new CnCfgPort();
    port1->setName("DuplicateName");
    project->portAdd(port1);

    CnCfgPort* port2 = new CnCfgPort();
    port2->setName("DuplicateName");
    project->portAdd(port2);

    // List should have 2 ports
    EXPECT_EQ(project->ports().size(), 2);
    
    // Hash will return the last added one
    CnCfgPortPtr retrieved = project->port("DuplicateName");
    EXPECT_NE(retrieved, nullptr);
}

// Test multiple groups with different data types
TEST_F(CnCfgProjectTest, MultipleGroupsVariousTypes)
{
    Cn::GroupConfig groups;

    Cn::Config intGroup;
    intGroup[CnSTR("Value1")] = CnVariant(100);
    intGroup[CnSTR("Value2")] = CnVariant(200);
    groups[CnSTR("IntegerGroup")] = intGroup;

    Cn::Config stringGroup;
    stringGroup[CnSTR("Str1")] = CnSTR("Hello");
    stringGroup[CnSTR("Str2")] = CnSTR("World");
    groups[CnSTR("StringGroup")] = stringGroup;

    Cn::Config mixedGroup;
    mixedGroup[CnSTR("Int")] = CnVariant(42);
    mixedGroup[CnSTR("String")] = CnSTR("Mixed");
    mixedGroup[CnSTR("Bool")] = CnVariant(true);
    mixedGroup[CnSTR("Double")] = CnVariant(3.14);
    groups[CnSTR("MixedGroup")] = mixedGroup;

    project->setGroupConfig(groups);

    EXPECT_EQ(project->groupConfig().size(), 3);

    Cn::Config intCfg = project->configForGroup("IntegerGroup");
    EXPECT_EQ(intCfg.at(CnSTR("Value1")).toInt(), 100);

    Cn::Config strCfg = project->configForGroup("StringGroup");
    EXPECT_EQ(strCfg.at(CnSTR("Str1")).toString(), "Hello");

    Cn::Config mixedCfg = project->configForGroup("MixedGroup");
    EXPECT_EQ(mixedCfg.at(CnSTR("Int")).toInt(), 42);
    EXPECT_EQ(mixedCfg.at(CnSTR("Bool")).toBool(), true);
    EXPECT_DOUBLE_EQ(mixedCfg.at(CnSTR("Double")).toDouble(), 3.14);
}

// Test ports with various configurations
TEST_F(CnCfgProjectTest, VariousPortConfigurations)
{
    // TCP Port
    CnCfgPort* tcpPort = new CnCfgPort();
    tcpPort->setName("TCP_Port");
    Cn::Config tcpCfg;
    tcpCfg[CnSTR("Type")] = CnSTR("TCP");
    tcpCfg[CnSTR("Host")] = CnSTR("192.168.1.10");
    tcpCfg[CnSTR("Port")] = CnVariant(502);
    tcpCfg[CnSTR("Timeout")] = CnVariant(3000);
    tcpPort->setConfig(tcpCfg);
    project->portAdd(tcpPort);

    // RTU Port
    CnCfgPort* rtuPort = new CnCfgPort();
    rtuPort->setName("RTU_Port");
    Cn::Config rtuCfg;
    rtuCfg[CnSTR("Type")] = CnSTR("RTU");
    rtuCfg[CnSTR("Device")] = CnSTR("COM1");
    rtuCfg[CnSTR("BaudRate")] = CnVariant(19200);
    rtuCfg[CnSTR("DataBits")] = CnVariant(8);
    rtuCfg[CnSTR("Parity")] = CnSTR("None");
    rtuPort->setConfig(rtuCfg);
    project->portAdd(rtuPort);

    // ASCII Port
    CnCfgPort* asciiPort = new CnCfgPort();
    asciiPort->setName("ASCII_Port");
    Cn::Config asciiCfg;
    asciiCfg[CnSTR("Type")] = CnSTR("ASCII");
    asciiCfg[CnSTR("Device")] = CnSTR("/dev/ttyUSB0");
    asciiCfg[CnSTR("BaudRate")] = CnVariant(9600);
    asciiPort->setConfig(asciiCfg);
    project->portAdd(asciiPort);

    EXPECT_EQ(project->ports().size(), 3);
    EXPECT_NE(project->port("TCP_Port"), nullptr);
    EXPECT_NE(project->port("RTU_Port"), nullptr);
    EXPECT_NE(project->port("ASCII_Port"), nullptr);
}

// Test devices with various port assignments
TEST_F(CnCfgProjectTest, DevicesWithVariousPortAssignments)
{
    CnCfgDevice* device1 = new CnCfgDevice();
    device1->setName("Device_TCP");
    device1->setPortName("TCP_Port");
    project->deviceAdd(device1);

    CnCfgDevice* device2 = new CnCfgDevice();
    device2->setName("Device_RTU");
    device2->setPortName("RTU_Port");
    project->deviceAdd(device2);

    CnCfgDevice* device3 = new CnCfgDevice();
    device3->setName("Device_NoPort");
    // No port assigned
    project->deviceAdd(device3);

    EXPECT_EQ(project->devices().size(), 3);
    EXPECT_EQ(project->device("Device_TCP")->portName(), "TCP_Port");
    EXPECT_EQ(project->device("Device_RTU")->portName(), "RTU_Port");
    EXPECT_EQ(project->device("Device_NoPort")->portName(), "");
}

// Test empty group config operations
TEST_F(CnCfgProjectTest, EmptyGroupConfigOperations)
{
    EXPECT_TRUE(project->groupConfig().empty());

    Cn::Config cfg = project->configForGroup("NonExistent");
    EXPECT_TRUE(cfg.empty());

    const Cn::Config* ptr = project->configPtrForGroup("NonExistent");
    EXPECT_EQ(ptr, nullptr);
}

// Test group config modification
TEST_F(CnCfgProjectTest, GroupConfigModification)
{
    Cn::GroupConfig groups;
    Cn::Config cfg1;
    cfg1[CnSTR("Initial")] = CnSTR("Value");
    groups[CnSTR("Group1")] = cfg1;

    project->setGroupConfig(groups);
    EXPECT_EQ(project->configForGroup("Group1").at(CnSTR("Initial")).toString(), "Value");

    // Modify via non-const accessor
    Cn::GroupConfig& groupRef = project->groupConfig();
    Cn::Config cfg2;
    cfg2[CnSTR("Modified")] = CnSTR("NewValue");
    groupRef[CnSTR("Group2")] = cfg2;

    EXPECT_EQ(project->groupConfig().size(), 2);
    EXPECT_EQ(project->configForGroup("Group2").at(CnSTR("Modified")).toString(), "NewValue");
}

// Test large number of ports
TEST_F(CnCfgProjectTest, LargeNumberOfPorts)
{
    const int portCount = 50;
    for (int i = 0; i < portCount; ++i)
    {
        CnCfgPort* port = new CnCfgPort();
        CnString name = CnSTR("Port") + Cn::toString(i);
        port->setName(name);
        project->portAdd(port);
    }

    EXPECT_EQ(project->ports().size(), portCount);

    // Test random access
    EXPECT_NE(project->port("Port0"), nullptr);
    EXPECT_NE(project->port("Port25"), nullptr);
    EXPECT_NE(project->port("Port49"), nullptr);
}

// Test large number of devices
TEST_F(CnCfgProjectTest, LargeNumberOfDevices)
{
    const int deviceCount = 50;
    for (int i = 0; i < deviceCount; ++i)
    {
        CnCfgDevice* device = new CnCfgDevice();
        CnString name = CnSTR("Device") + Cn::toString(i);
        device->setName(name);
        project->deviceAdd(device);
    }

    EXPECT_EQ(project->devices().size(), deviceCount);

    // Test random access
    EXPECT_NE(project->device("Device0"), nullptr);
    EXPECT_NE(project->device("Device25"), nullptr);
    EXPECT_NE(project->device("Device49"), nullptr);
}

// Test project with name and config
TEST_F(CnCfgProjectTest, ProjectNameAndConfig)
{
    project->setName("CompleteProject");

    Cn::Config cfg;
    cfg[CnSTR("Author")] = CnSTR("TestUser");
    cfg[CnSTR("Version")] = CnSTR("2.0");
    cfg[CnSTR("Created")] = CnSTR("2024-11-24");
    project->setConfig(cfg);

    EXPECT_EQ(project->name(), "CompleteProject");
    EXPECT_EQ(project->config().at(CnSTR("Author")).toString(), "TestUser");
    EXPECT_EQ(project->config().at(CnSTR("Version")).toString(), "2.0");
    EXPECT_EQ(project->config().at(CnSTR("Created")).toString(), "2024-11-24");
}

// Test destructor cleanup
TEST_F(CnCfgProjectTest, DestructorCleanup)
{
    CnCfgProject* tempProject = new CnCfgProject();

    CnCfgPort* port = new CnCfgPort();
    port->setName("TempPort");
    tempProject->portAdd(port);

    CnCfgDevice* device = new CnCfgDevice();
    device->setName("TempDevice");
    tempProject->deviceAdd(device);

    // Destructor should clean up properly
    delete tempProject;
    // No crash = success
}
