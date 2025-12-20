/*!
 * \file   CnCfgBuilder_test.cpp
 * \brief  Unit tests for CnCfgBuilder class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */
#include <gtest/gtest.h>
#include <cfg/CnCfgBuilder.h>
#include <cfg/CnCfgProject.h>
#include <cfg/CnCfgPort.h>
#include <cfg/CnCfgDevice.h>
#include <cfg/CnCfgDeviceItem.h>
#include <app/CnApp.h>
#include <port/CnPort.h>
#include <device/CnDevice.h>
#include <CnFileInfo.h>
#include <CnDir.h>
#include <fstream>

class CnCfgBuilderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        builder = new CnCfgBuilder();
        // Get absolute path to current directory
        testDir = CnDir::currentPath();
    }

    void TearDown() override
    {
        delete builder;
        
        // Clean up test files using absolute paths
        RemoveTestFile("test_project.conf");
        RemoveTestFile("test_include.conf");
        RemoveTestFile("test_items.csv");
        RemoveTestFile("test_port.conf");
        RemoveTestFile("test_device.conf");
        RemoveTestFile("test_empty.conf");
        RemoveTestFile("test_invalid.conf");
        RemoveTestFile("test_multiports.conf");
        RemoveTestFile("test_multidevices.conf");
    }

    CnString GetTestFilePath(const CnString& fileName) const
    {
        return testDir.absoluteFilePath(fileName);
    }

    void RemoveTestFile(const CnString& fileName)
    {
        CnString fullPath = GetTestFilePath(fileName);
        std::remove(fullPath.c_str());
    }

    CnDir testDir;
    CnCfgBuilder* builder;

    void CreateBasicProjectFile()
    {
        CnString path = GetTestFilePath("test_project.conf");
        std::ofstream file(path.c_str());
        file << "[System]\n";
        file << "Name=TestProject\n";
        file << "Log.flags=Error|Warning\n";
        file.close();
    }

    void CreateProjectWithPortFile()
    {
        CnString path = GetTestFilePath("test_port.conf");
        std::ofstream file(path.c_str());
        file << "[System]\n";
        file << "Name=ProjectWithPort\n";
        file << "\n";
        file << "[Port]\n";
        file << "Name=PORT1\n";
        file << "Enable=1\n";
        file << "Type=TCP\n";
        file << "Host=127.0.0.1\n";
        file << "Port=502\n";
        file.close();
    }

    void CreateProjectWithDeviceFile()
    {
        CnString path = GetTestFilePath("test_device.conf");
        std::ofstream file(path.c_str());
        file << "[System]\n";
        file << "Name=ProjectWithDevice\n";
        file << "\n";
        file << "[Port]\n";
        file << "Name=PORT1\n";
        file << "\n";
        file << "[Device]\n";
        file << "Name=DEVICE1\n";
        file << "PortName=PORT1\n";
        file << "EnableDevice=1\n";
        file << "ModbusUnit=1\n";
        file.close();
    }

    void CreateProjectWithIncludeFile()
    {
        CnString mainPath = GetTestFilePath("test_project.conf");
        std::ofstream mainFile(mainPath.c_str());
        mainFile << "[System]\n";
        mainFile << "Name=MainProject\n";
        // Use absolute path for include
        CnString includePath = GetTestFilePath("test_include.conf");
        mainFile << "include=" << includePath.c_str() << "\n";
        mainFile.close();

        std::ofstream includeFile(includePath.c_str());
        includeFile << "[Port]\n";
        includeFile << "Name=IncludedPort\n";
        includeFile << "Type=TCP\n";
        includeFile.close();
    }

    void CreateProjectWithDeviceItemsFile()
    {
        CnString path = GetTestFilePath("test_device.conf");
        std::ofstream confFile(path.c_str());
        confFile << "[System]\n";
        confFile << "Name=DeviceWithItems\n";
        confFile << "\n";
        confFile << "[Port]\n";
        confFile << "Name=PORT1\n";
        confFile << "\n";
        confFile << "[Device]\n";
        confFile << "Name=DEV1\n";
        confFile << "PortName=PORT1\n";
        confFile << "i:Item1=400001 S\n";
        confFile << "i:Item2=400002 U\n";
        confFile.close();
    }

    void CreateProjectWithItemFileReference()
    {
        CnString confPath = GetTestFilePath("test_device.conf");
        CnString csvPath = GetTestFilePath("test_items.csv");
        
        std::ofstream confFile(confPath.c_str());
        confFile << "[System]\n";
        confFile << "Name=DeviceWithItemFile\n";
        confFile << "\n";
        confFile << "[Port]\n";
        confFile << "Name=PORT1\n";
        confFile << "\n";
        confFile << "[Device]\n";
        confFile << "Name=DEV1\n";
        confFile << "PortName=PORT1\n";
        // Use absolute path for itemfile
        confFile << "itemfile=" << csvPath.c_str() << "\n";
        confFile.close();

        std::ofstream csvFile(csvPath.c_str());
        csvFile << "Item1;400001 S\n";
        csvFile << "Item2;400002 U\n";
        csvFile << "Item3;400003 F\n";
        csvFile.close();
    }

    void CreateEmptyProjectFile()
    {
        CnString path = GetTestFilePath("test_empty.conf");
        std::ofstream file(path.c_str());
        file.close();
    }

    void CreateInvalidProjectFile()
    {
        CnString path = GetTestFilePath("test_invalid.conf");
        std::ofstream file(path.c_str());
        file << "[System\n"; // Missing closing bracket
        file << "Name=Invalid\n";
        file.close();
    }

    void CreateProjectWithMultiplePortsFile()
    {
        CnString path = GetTestFilePath("test_multiports.conf");
        std::ofstream file(path.c_str());
        file << "[System]\n";
        file << "Name=MultiPortProject\n";
        file << "\n";
        file << "[Port1]\n";
        file << "Name=PORT1\n";
        file << "Type=TCP\n";
        file << "\n";
        file << "[Port.Second]\n";
        file << "Name=PORT2\n";
        file << "Type=RTU\n";
        file << "\n";
        file << "[PORT_Third]\n";
        file << "Name=PORT3\n";
        file << "Type=ASCII\n";
        file.close();
    }

    void CreateProjectWithMultipleDevicesFile()
    {
        CnString path = GetTestFilePath("test_multidevices.conf");
        std::ofstream file(path.c_str());
        file << "[System]\n";
        file << "Name=MultiDeviceProject\n";
        file << "\n";
        file << "[Port]\n";
        file << "Name=PORT1\n";
        file << "\n";
        file << "[Device1]\n";
        file << "Name=DEV1\n";
        file << "PortName=PORT1\n";
        file << "\n";
        file << "[Device.Second]\n";
        file << "Name=DEV2\n";
        file << "PortName=PORT1\n";
        file << "\n";
        file << "[DEVICE_Third]\n";
        file << "Name=DEV3\n";
        file << "PortName=PORT1\n";
        file.close();
    }

    void CreateProjectWithCustomGroupsFile()
    {
        CnString path = GetTestFilePath("test_project.conf");
        std::ofstream file(path.c_str());
        file << "[System]\n";
        file << "Name=CustomGroupProject\n";
        file << "\n";
        file << "[OPCUA]\n";
        file << "Port=4840\n";
        file << "UncertainAs=Good\n";
        file << "\n";
        file << "[CustomGroup]\n";
        file << "Param1=Value1\n";
        file << "Param2=Value2\n";
        file.close();
    }
};

// Test default constructor
TEST_F(CnCfgBuilderTest, DefaultConstructor)
{
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(builder->errorString(), "");
}

// Test loading basic project file
TEST_F(CnCfgBuilderTest, LoadBasicProject)
{
    CreateBasicProjectFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_project.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->name(), "TestProject");
    
    delete project;
}

// Test loading project with port
TEST_F(CnCfgBuilderTest, LoadProjectWithPort)
{
    CreateProjectWithPortFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_port.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->ports().size(), 1);
    
    CnCfgPortPtr port = project->port("PORT1");
    ASSERT_NE(port, nullptr);
    EXPECT_EQ(port->name(), "PORT1");
    
    delete project;
}

// Test loading project with device
TEST_F(CnCfgBuilderTest, LoadProjectWithDevice)
{
    CreateProjectWithDeviceFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_device.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->devices().size(), 1);
    
    CnCfgDevicePtr device = project->device("DEVICE1");
    ASSERT_NE(device, nullptr);
    EXPECT_EQ(device->name(), "DEVICE1");
    EXPECT_EQ(device->portName(), "PORT1");
    
    delete project;
}

// Test loading project with include file
TEST_F(CnCfgBuilderTest, LoadProjectWithInclude)
{
    CreateProjectWithIncludeFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_project.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->name(), "MainProject");
    EXPECT_EQ(project->ports().size(), 1);
    
    CnCfgPortPtr port = project->port("IncludedPort");
    ASSERT_NE(port, nullptr);
    
    delete project;
}

// Test loading project with device items
TEST_F(CnCfgBuilderTest, LoadProjectWithDeviceItems)
{
    CreateProjectWithDeviceItemsFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_device.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgDevicePtr device = project->device("DEV1");
    ASSERT_NE(device, nullptr);
    EXPECT_EQ(device->items().size(), 2);
    EXPECT_TRUE(device->hasItem("Item1"));
    EXPECT_TRUE(device->hasItem("Item2"));
    
    delete project;
}

// Test loading project with item file reference
TEST_F(CnCfgBuilderTest, LoadProjectWithItemFileReference)
{
    CreateProjectWithItemFileReference();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_device.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgDevicePtr device = project->device("DEV1");
    ASSERT_NE(device, nullptr);
    EXPECT_EQ(device->items().size(), 3);
    EXPECT_TRUE(device->hasItem("Item1"));
    EXPECT_TRUE(device->hasItem("Item2"));
    EXPECT_TRUE(device->hasItem("Item3"));
    
    delete project;
}

// Test loading non-existent file
// Note: CnCfgBuilder creates an empty project when file doesn't exist
TEST_F(CnCfgBuilderTest, LoadNonExistentFile)
{
    CnString path = GetTestFilePath("nonexistent_file_xyz123.conf");
    CnCfgProject* project = builder->load(path);
    
    // CnCfgBuilder creates empty project for non-existent files
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->ports().size(), 0);
    EXPECT_EQ(project->devices().size(), 0);
    
    delete project;
}

// Test loading empty project file
TEST_F(CnCfgBuilderTest, LoadEmptyProject)
{
    CreateEmptyProjectFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_empty.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->ports().size(), 0);
    EXPECT_EQ(project->devices().size(), 0);
    
    delete project;
}

// Test loading invalid project file
TEST_F(CnCfgBuilderTest, LoadInvalidProject)
{
    CreateInvalidProjectFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_invalid.conf"));
    
    EXPECT_EQ(project, nullptr);
    EXPECT_TRUE(builder->hasError());
}

// Test error string clearing
TEST_F(CnCfgBuilderTest, ClearError)
{
    CreateInvalidProjectFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_invalid.conf"));
    EXPECT_EQ(project, nullptr);
    EXPECT_TRUE(builder->hasError());
    
    builder->clearError();
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(builder->errorString(), "");
}

// Test loading project with multiple ports
TEST_F(CnCfgBuilderTest, LoadProjectWithMultiplePorts)
{
    CreateProjectWithMultiplePortsFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_multiports.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->ports().size(), 3);
    
    EXPECT_NE(project->port("PORT1"), nullptr);
    EXPECT_NE(project->port("PORT2"), nullptr);
    EXPECT_NE(project->port("PORT3"), nullptr);
    
    delete project;
}

// Test loading project with multiple devices
TEST_F(CnCfgBuilderTest, LoadProjectWithMultipleDevices)
{
    CreateProjectWithMultipleDevicesFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_multidevices.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->devices().size(), 3);
    
    EXPECT_NE(project->device("DEV1"), nullptr);
    EXPECT_NE(project->device("DEV2"), nullptr);
    EXPECT_NE(project->device("DEV3"), nullptr);
    
    delete project;
}

// Test loading project with custom groups
TEST_F(CnCfgBuilderTest, LoadProjectWithCustomGroups)
{
    CreateProjectWithCustomGroupsFile();
    
    CnCfgProject* project = builder->load(GetTestFilePath("test_project.conf"));
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    const Cn::GroupConfig& groups = project->groupConfig();
    EXPECT_NE(groups.find("OPCUA"), groups.end());
    EXPECT_NE(groups.find("CUSTOMGROUP"), groups.end());
    
    const Cn::Config* opcuaConfig = project->configPtrForGroup("OPCUA");
    ASSERT_NE(opcuaConfig, nullptr);
    EXPECT_NE(opcuaConfig->find("Port"), opcuaConfig->end());
    
    delete project;
}

// Test Strings singleton
TEST_F(CnCfgBuilderTest, StringsSingleton)
{
    const CnCfgBuilder::Strings& s1 = CnCfgBuilder::Strings::instance();
    const CnCfgBuilder::Strings& s2 = CnCfgBuilder::Strings::instance();
    
    EXPECT_EQ(&s1, &s2); // Same instance
    EXPECT_EQ(s1.KeyInclude, "include");
    EXPECT_EQ(s1.ItemPrefix, "i:");
    EXPECT_EQ(s1.KeyItemFile, "itemfile");
}

// Test duplicate port names handling
TEST_F(CnCfgBuilderTest, DuplicatePortNames)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=DuplicatePortTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n"; // Duplicate
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    // Only first port should be added, duplicate should be ignored (with warning)
    EXPECT_EQ(project->ports().size(), 1);
    
    delete project;
}

// Test duplicate device names handling
TEST_F(CnCfgBuilderTest, DuplicateDeviceNames)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=DuplicateDeviceTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=DEV1\n";
    file << "PortName=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=DEV1\n"; // Duplicate
    file << "PortName=PORT1\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    // Only first device should be added, duplicate should be ignored (with warning)
    EXPECT_EQ(project->devices().size(), 1);
    
    delete project;
}

// Test invalid port name handling
TEST_F(CnCfgBuilderTest, InvalidPortName)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=InvalidPortNameTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=123Invalid\n"; // Name must start with letter or underscore
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    // Invalid port should be ignored (with warning)
    EXPECT_EQ(project->ports().size(), 0);
    
    delete project;
}

// Test invalid device name handling
TEST_F(CnCfgBuilderTest, InvalidDeviceName)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=InvalidDeviceNameTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=Dev@Invalid\n"; // Name contains invalid character
    file << "PortName=PORT1\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    // Invalid device should be ignored (with warning)
    EXPECT_EQ(project->devices().size(), 0);
    
    delete project;
}

// Test empty device name handling
TEST_F(CnCfgBuilderTest, EmptyDeviceName)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=EmptyDeviceNameTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=\n"; // Empty name
    file << "PortName=PORT1\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    // Device with empty name should be ignored (with warning)
    EXPECT_EQ(project->devices().size(), 0);
    
    delete project;
}

// Test duplicate item names in device
TEST_F(CnCfgBuilderTest, DuplicateItemNames)
{
    CnString path = GetTestFilePath("test_device.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=DuplicateItemTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=DEV1\n";
    file << "PortName=PORT1\n";
    file << "i:Item1=400001 S\n";
    file << "i:Item1=400002 U\n"; // Duplicate
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgDevicePtr device = project->device("DEV1");
    ASSERT_NE(device, nullptr);
    // Only first item should be added, duplicate should be ignored (with warning)
    EXPECT_EQ(device->items().size(), 1);
    
    delete project;
}

// Test invalid item name handling
TEST_F(CnCfgBuilderTest, InvalidItemName)
{
    CnString path = GetTestFilePath("test_device.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=InvalidItemTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=DEV1\n";
    file << "PortName=PORT1\n";
    file << "i:123Invalid=400001 S\n"; // Invalid item name
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgDevicePtr device = project->device("DEV1");
    ASSERT_NE(device, nullptr);
    // Invalid item should be ignored (with warning)
    EXPECT_EQ(device->items().size(), 0);
    
    delete project;
}

// Test missing item file handling
TEST_F(CnCfgBuilderTest, MissingItemFile)
{
    CnString path = GetTestFilePath("test_device.conf");
    CnString nonexistentPath = GetTestFilePath("nonexistent_itemfile_xyz123.csv");
    
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=MissingItemFileTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[Device]\n";
    file << "Name=DEV1\n";
    file << "PortName=PORT1\n";
    file << "itemfile=" << nonexistentPath.c_str() << "\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgDevicePtr device = project->device("DEV1");
    ASSERT_NE(device, nullptr);
    // Device should be created but without items (with warning about missing file)
    EXPECT_EQ(device->items().size(), 0);
    
    delete project;
}

// Test project section variations (PROJECT vs SYSTEM)
TEST_F(CnCfgBuilderTest, ProjectSectionVariations)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[PROJECT]\n"; // Using PROJECT instead of SYSTEM
    file << "Name=ProjectTest\n";
    file << "Param1=Value1\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->name(), "ProjectTest");
    
    delete project;
}

// Test port section name variations
TEST_F(CnCfgBuilderTest, PortSectionNameVariations)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=PortVariationsTest\n";
    file << "\n";
    file << "[PORT]\n"; // Uppercase
    file << "Name=PORT1\n";
    file << "\n";
    file << "[port]\n"; // Lowercase
    file << "Name=PORT2\n";
    file << "\n";
    file << "[Port_ABC]\n"; // With suffix
    file << "Name=PORT3\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->ports().size(), 3);
    
    delete project;
}

// Test device section name variations
TEST_F(CnCfgBuilderTest, DeviceSectionNameVariations)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=DeviceVariationsTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=PORT1\n";
    file << "\n";
    file << "[DEVICE]\n"; // Uppercase
    file << "Name=DEV1\n";
    file << "PortName=PORT1\n";
    file << "\n";
    file << "[device]\n"; // Lowercase
    file << "Name=DEV2\n";
    file << "PortName=PORT1\n";
    file << "\n";
    file << "[Device_XYZ]\n"; // With suffix
    file << "Name=DEV3\n";
    file << "PortName=PORT1\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->devices().size(), 3);
    
    delete project;
}

// Test valid name patterns
TEST_F(CnCfgBuilderTest, ValidNamePatterns)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=ValidNamesTest\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=_Port1\n"; // Starting with underscore
    file << "\n";
    file << "[Port]\n";
    file << "Name=Port_With_Underscores\n";
    file << "\n";
    file << "[Port]\n";
    file << "Name=Port.With.Dots\n"; // With dots
    file << "\n";
    file << "[Port]\n";
    file << "Name=Port123ABC\n"; // Mixed alphanumeric
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    EXPECT_EQ(project->ports().size(), 4);
    
    delete project;
}

// Test config data types preservation
TEST_F(CnCfgBuilderTest, ConfigDataTypesPreservation)
{
    CnString path = GetTestFilePath("test_project.conf");
    std::ofstream file(path.c_str());
    file << "[System]\n";
    file << "Name=DataTypesTest\n";
    file << "IntValue=42\n";
    file << "StringValue=TestString\n";
    file << "BoolValue=1\n";
    file.close();
    
    CnCfgProject* project = builder->load(path);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    const Cn::Config& cfg = project->config();
    EXPECT_NE(cfg.find("IntValue"), cfg.end());
    EXPECT_NE(cfg.find("StringValue"), cfg.end());
    EXPECT_NE(cfg.find("BoolValue"), cfg.end());
    
    delete project;
}

// Test sequential load operations
TEST_F(CnCfgBuilderTest, SequentialLoadOperations)
{
    CreateBasicProjectFile();
    CnString path = GetTestFilePath("test_project.conf");
    
    // First load
    CnCfgProject* project1 = builder->load(path);
    ASSERT_NE(project1, nullptr);
    EXPECT_FALSE(builder->hasError());
    delete project1;
    
    // Second load (should clear previous state)
    CnCfgProject* project2 = builder->load(path);
    ASSERT_NE(project2, nullptr);
    EXPECT_FALSE(builder->hasError());
    delete project2;
}

// Test include file include from device section
TEST_F(CnCfgBuilderTest, IncludeFromDeviceSection)
{
    CnString inclPath = GetTestFilePath("test_include.conf");
    std::ofstream includeFile(inclPath.c_str());
    includeFile << "ModbusUnit=5\n";
    includeFile << "RequestTimeout=3000\n";
    includeFile.close();
    
    CnString mainPath = GetTestFilePath("test_project.conf");
    std::ofstream mainFile(mainPath.c_str());
    mainFile << "[System]\n";
    mainFile << "Name=IncludeInDeviceTest\n";
    mainFile << "\n";
    mainFile << "[Port]\n";
    mainFile << "Name=PORT1\n";
    mainFile << "\n";
    mainFile << "[Device]\n";
    mainFile << "Name=DEV1\n";
    mainFile << "PortName=PORT1\n";
    mainFile << "include=test_include.conf\n";
    mainFile.close();
    
    CnCfgProject* project = builder->load(mainPath);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgDevicePtr device = project->device("DEV1");
    ASSERT_NE(device, nullptr);
    
    const Cn::Config& cfg = device->config();
    EXPECT_NE(cfg.find("ModbusUnit"), cfg.end());
    EXPECT_NE(cfg.find("RequestTimeout"), cfg.end());
    
    delete project;
}

// Test include file from port section
TEST_F(CnCfgBuilderTest, IncludeFromPortSection)
{
    CnString inclPath = GetTestFilePath("test_include.conf");
    std::ofstream includeFile(inclPath.c_str());
    includeFile << "Timeout=2000\n";
    includeFile << "RepeatCount=3\n";
    includeFile.close();
    
    CnString mainPath = GetTestFilePath("test_project.conf");
    std::ofstream mainFile(mainPath.c_str());
    mainFile << "[System]\n";
    mainFile << "Name=IncludeInPortTest\n";
    mainFile << "\n";
    mainFile << "[Port]\n";
    mainFile << "Name=PORT1\n";
    mainFile << "Type=TCP\n";
    mainFile << "include=test_include.conf\n";
    mainFile.close();
    
    CnCfgProject* project = builder->load(mainPath);
    
    ASSERT_NE(project, nullptr);
    EXPECT_FALSE(builder->hasError());
    
    CnCfgPortPtr port = project->port("PORT1");
    ASSERT_NE(port, nullptr);
    
    const Cn::Config& cfg = port->config();
    EXPECT_NE(cfg.find("Timeout"), cfg.end());
    EXPECT_NE(cfg.find("RepeatCount"), cfg.end());
    
    delete project;
}

