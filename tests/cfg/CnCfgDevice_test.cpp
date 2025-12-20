/*!
 * \file   CnCfgDevice_test.cpp
 * \brief  Unit tests for CnCfgDevice class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */
#include "gtest/gtest.h"
#include <cfg/CnCfgDevice.h>
#include <cfg/CnCfgDeviceItem.h>
#include <device/items/CnDeviceBaseItem.h>

class CnCfgDeviceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        device = new CnCfgDevice();
    }

    void TearDown() override
    {
        delete device;
    }

    CnCfgDevice* device;
};

// Test default constructor and basic functionality
TEST_F(CnCfgDeviceTest, DefaultConstructor)
{
    EXPECT_EQ(device->name(), "");
    EXPECT_EQ(device->portName(), "");
    EXPECT_TRUE(device->items().empty());
}

// Test name setter and getter (inherited from CnCfgBase)
TEST_F(CnCfgDeviceTest, SetAndGetName)
{
    device->setName("TestDevice");
    EXPECT_EQ(device->name(), "TestDevice");
}

// Test port name setter and getter
TEST_F(CnCfgDeviceTest, SetAndGetPortName)
{
    device->setPortName("COM1");
    EXPECT_EQ(device->portName(), "COM1");
}

// Test setConfig with PortName
TEST_F(CnCfgDeviceTest, SetConfigWithPortName)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Device1");
    cfg[CnSTR("PortName")] = CnSTR("COM3");

    device->setConfig(cfg);

    EXPECT_EQ(device->name(), "Device1");
    EXPECT_EQ(device->portName(), "COM3");
}

// Test setConfig with Name only
TEST_F(CnCfgDeviceTest, SetConfigWithNameOnly)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Device2");

    device->setConfig(cfg);

    EXPECT_EQ(device->name(), "Device2");
    EXPECT_EQ(device->portName(), "");
}

// Test setConfig with empty config
TEST_F(CnCfgDeviceTest, SetConfigEmpty)
{
    Cn::Config cfg;

    device->setConfig(cfg);

    EXPECT_EQ(device->name(), "");
    EXPECT_EQ(device->portName(), "");
}

// Test setConfig removes PortName from config map
TEST_F(CnCfgDeviceTest, SetConfigRemovesPortName)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Device3");
    cfg[CnSTR("PortName")] = CnSTR("COM5");
    cfg[CnSTR("OtherParam")] = CnSTR("Value");

    device->setConfig(cfg);

    // PortName should be removed from config after processing
    const Cn::Config& resultConfig = device->config();
    EXPECT_EQ(resultConfig.find(CnSTR("PortName")), resultConfig.end());
    EXPECT_NE(resultConfig.find(CnSTR("OtherParam")), resultConfig.end());
}

// Test adding single item
TEST_F(CnCfgDeviceTest, AddSingleItem)
{
    CnCfgDeviceItem* item = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Config,
        Cn::Suffix_Default
    );
    item->setName("Item1");

    device->addItem(item);

    EXPECT_EQ(device->items().size(), 1);
    EXPECT_TRUE(device->hasItem("Item1"));
}

// Test adding multiple items
TEST_F(CnCfgDeviceTest, AddMultipleItems)
{
    CnCfgDeviceItem* item1 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Config,
        Cn::Suffix_Default
    );
    item1->setName("Item1");

    CnCfgDeviceItem* item2 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Command,
        Cn::Suffix_Bool
    );
    item2->setName("Item2");

    CnCfgDeviceItem* item3 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Setting,
        Cn::Suffix_Int16
    );
    item3->setName("Item3");

    device->addItem(item1);
    device->addItem(item2);
    device->addItem(item3);

    EXPECT_EQ(device->items().size(), 3);
    EXPECT_TRUE(device->hasItem("Item1"));
    EXPECT_TRUE(device->hasItem("Item2"));
    EXPECT_TRUE(device->hasItem("Item3"));
}

// Test hasItem with non-existent item
TEST_F(CnCfgDeviceTest, HasItemNonExistent)
{
    EXPECT_FALSE(device->hasItem("NonExistent"));

    CnCfgDeviceItem* item = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Config,
        Cn::Suffix_Default
    );
    item->setName("ExistingItem");
    device->addItem(item);

    EXPECT_FALSE(device->hasItem("NonExistent"));
    EXPECT_TRUE(device->hasItem("ExistingItem"));
}

// Test items list order
TEST_F(CnCfgDeviceTest, ItemsListOrder)
{
    CnCfgDeviceItem* item1 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Config,
        Cn::Suffix_Default
    );
    item1->setName("First");

    CnCfgDeviceItem* item2 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Command,
        Cn::Suffix_Bool
    );
    item2->setName("Second");

    CnCfgDeviceItem* item3 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Setting,
        Cn::Suffix_Int16
    );
    item3->setName("Third");

    device->addItem(item1);
    device->addItem(item2);
    device->addItem(item3);

    const CnList<CnCfgDeviceItem*>& items = device->items();
    auto it = items.begin();
    EXPECT_EQ((*it)->name(), "First");
    ++it;
    EXPECT_EQ((*it)->name(), "Second");
    ++it;
    EXPECT_EQ((*it)->name(), "Third");
}

// Test config getter
TEST_F(CnCfgDeviceTest, GetConfig)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Device4");
    cfg[CnSTR("PortName")] = CnSTR("COM7");
    cfg[CnSTR("CustomParam")] = CnVariant(123);

    device->setConfig(cfg);

    const Cn::Config& resultConfig = device->config();
    EXPECT_NE(resultConfig.find(CnSTR("CustomParam")), resultConfig.end());
    EXPECT_EQ(resultConfig.at(CnSTR("CustomParam")).toInt(), 123);
}

// Test with different data types in config
TEST_F(CnCfgDeviceTest, SetConfigWithVariousDataTypes)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Device5");
    cfg[CnSTR("PortName")] = CnSTR("COM9");
    cfg[CnSTR("IntValue")] = CnVariant(42);
    cfg[CnSTR("DoubleValue")] = CnVariant(3.14);
    cfg[CnSTR("BoolValue")] = CnVariant(true);

    device->setConfig(cfg);

    const Cn::Config& resultConfig = device->config();
    EXPECT_EQ(resultConfig.at(CnSTR("IntValue")).toInt(), 42);
    EXPECT_DOUBLE_EQ(resultConfig.at(CnSTR("DoubleValue")).toDouble(), 3.14);
    EXPECT_EQ(resultConfig.at(CnSTR("BoolValue")).toBool(), true);
}

// Test pointer type (CnCfgDevicePtr)
TEST_F(CnCfgDeviceTest, SharedPointerType)
{
    CnCfgDevicePtr ptr(new CnCfgDevice());
    ptr->setName("SharedDevice");
    ptr->setPortName("COM11");

    EXPECT_EQ(ptr->name(), "SharedDevice");
    EXPECT_EQ(ptr->portName(), "COM11");
}

// Test item retrieval by name through hash
TEST_F(CnCfgDeviceTest, ItemRetrievalByName)
{
    CnCfgDeviceItem* item1 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Config,
        Cn::Suffix_Default
    );
    item1->setName("ConfigItem");

    CnCfgDeviceItem* item2 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Statistic,
        Cn::Suffix_UInt16
    );
    item2->setName("StatusItem");

    device->addItem(item1);
    device->addItem(item2);

    EXPECT_TRUE(device->hasItem("ConfigItem"));
    EXPECT_TRUE(device->hasItem("StatusItem"));
    EXPECT_FALSE(device->hasItem("NonExistentItem"));
}

// Test empty items list
TEST_F(CnCfgDeviceTest, EmptyItemsList)
{
    EXPECT_TRUE(device->items().empty());
    EXPECT_EQ(device->items().size(), 0);
}

// Test config preservation after setConfig
TEST_F(CnCfgDeviceTest, ConfigPreservation)
{
    Cn::Config cfg;
    cfg[CnSTR("Name")] = CnSTR("Device6");
    cfg[CnSTR("PortName")] = CnSTR("COM13");
    cfg[CnSTR("Param1")] = CnVariant(100);
    cfg[CnSTR("Param2")] = CnSTR("TestValue");

    device->setConfig(cfg);

    // Verify that non-special parameters are preserved
    const Cn::Config& resultConfig = device->config();
    EXPECT_EQ(resultConfig.at(CnSTR("Param1")).toInt(), 100);
    EXPECT_EQ(resultConfig.at(CnSTR("Param2")).toString(), "TestValue");
}

// Test setConfig overrides previous configuration
TEST_F(CnCfgDeviceTest, SetConfigOverride)
{
    Cn::Config cfg1;
    cfg1[CnSTR("Name")] = CnSTR("FirstDevice");
    cfg1[CnSTR("PortName")] = CnSTR("COM1");

    device->setConfig(cfg1);
    EXPECT_EQ(device->name(), "FirstDevice");
    EXPECT_EQ(device->portName(), "COM1");

    Cn::Config cfg2;
    cfg2[CnSTR("Name")] = CnSTR("SecondDevice");
    cfg2[CnSTR("PortName")] = CnSTR("COM2");

    device->setConfig(cfg2);
    EXPECT_EQ(device->name(), "SecondDevice");
    EXPECT_EQ(device->portName(), "COM2");
}

// Test with complex item configurations
TEST_F(CnCfgDeviceTest, ComplexItemConfiguration)
{
    CnCfgDeviceItem* item = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Modbus,
        Cn::Suffix_UInt32
    );
    item->setName("ComplexItem");
    item->setItemReference("Device.Subsystem.Item");
    item->setMemoryType(Modbus::Memory_4x);
    item->setOffset(100);
    item->setCount(10);
    item->setPeriod(1000);
    item->setMessageId("MSG001");

    device->addItem(item);

    EXPECT_TRUE(device->hasItem("ComplexItem"));
    const CnList<CnCfgDeviceItem*>& items = device->items();
    EXPECT_EQ(items.size(), 1);
    
    CnCfgDeviceItem* retrievedItem = items.front();
    EXPECT_EQ(retrievedItem->name(), "ComplexItem");
    EXPECT_EQ(retrievedItem->itemReference(), "Device.Subsystem.Item");
    EXPECT_EQ(retrievedItem->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(retrievedItem->dataSuffix(), Cn::Suffix_UInt32);
}
