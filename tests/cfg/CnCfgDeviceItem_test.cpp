/*!
 * \file   CnCfgDeviceItem_test.cpp
 * \brief  Unit tests for CnCfgDeviceItem class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */
#include "gtest/gtest.h"
#include <cfg/CnCfgDeviceItem.h>
#include <device/items/CnDeviceBaseItem.h>

class CnCfgDeviceItemTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        item = new CnCfgDeviceItem(
            CnDeviceBaseItem::Item_Config,
            Cn::Suffix_Default
        );
    }

    void TearDown() override
    {
        delete item;
    }

    CnCfgDeviceItem* item;
};

// Test constructor with type and dataSuffix
TEST_F(CnCfgDeviceItemTest, Constructor)
{
    CnCfgDeviceItem* testItem = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_Command,
        Cn::Suffix_Int16
    );

    EXPECT_EQ(testItem->type(), CnDeviceBaseItem::Item_Command);
    EXPECT_EQ(testItem->dataSuffix(), Cn::Suffix_Int16);
    EXPECT_EQ(testItem->name(), "");
    EXPECT_EQ(testItem->itemReference(), "");
    EXPECT_EQ(testItem->memoryType(), Modbus::Memory_Unknown);
    EXPECT_EQ(testItem->offset(), 0);
    EXPECT_EQ(testItem->count(), 0);
    EXPECT_EQ(testItem->period(), CN_MODBUS_PERIOD_DEFAULT_VALUE);
    EXPECT_EQ(testItem->messageId(), "");

    delete testItem;
}

// Test name getter/setter
TEST_F(CnCfgDeviceItemTest, NameGetterSetter)
{
    item->setName("TestItem");
    EXPECT_EQ(item->name(), "TestItem");

    item->setName("AnotherItem");
    EXPECT_EQ(item->name(), "AnotherItem");
}

// Test itemReference getter/setter
TEST_F(CnCfgDeviceItemTest, ItemReferenceGetterSetter)
{
    item->setItemReference("Device.Item1");
    EXPECT_EQ(item->itemReference(), "Device.Item1");

    item->setItemReference("Device.SubItem.Value");
    EXPECT_EQ(item->itemReference(), "Device.SubItem.Value");
}

// Test type getter/setter
TEST_F(CnCfgDeviceItemTest, TypeGetterSetter)
{
    item->setType(CnDeviceBaseItem::Item_Setting);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Setting);

    item->setType(CnDeviceBaseItem::Item_Statistic);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Statistic);
}

// Test dataSuffix getter/setter
TEST_F(CnCfgDeviceItemTest, DataSuffixGetterSetter)
{
    item->setDataSuffix(Cn::Suffix_Bool);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Bool);

    item->setDataSuffix(Cn::Suffix_Float);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Float);
}

// Test memoryType getter/setter
TEST_F(CnCfgDeviceItemTest, MemoryTypeGetterSetter)
{
    item->setMemoryType(Modbus::Memory_Coils);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_Coils);

    item->setMemoryType(Modbus::Memory_HoldingRegisters);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);
}

// Test offset getter/setter
TEST_F(CnCfgDeviceItemTest, OffsetGetterSetter)
{
    item->setOffset(100);
    EXPECT_EQ(item->offset(), 100);

    item->setOffset(65535);
    EXPECT_EQ(item->offset(), 65535);

    item->setOffset(0);
    EXPECT_EQ(item->offset(), 0);
}

// Test count getter/setter
TEST_F(CnCfgDeviceItemTest, CountGetterSetter)
{
    item->setCount(10);
    EXPECT_EQ(item->count(), 10);

    item->setCount(127);
    EXPECT_EQ(item->count(), 127);

    item->setCount(1);
    EXPECT_EQ(item->count(), 1);
}

// Test period getter/setter
TEST_F(CnCfgDeviceItemTest, PeriodGetterSetter)
{
    item->setPeriod(1000);
    EXPECT_EQ(item->period(), 1000);

    item->setPeriod(CN_MODBUS_PERIOD_DEFAULT_VALUE);
    EXPECT_EQ(item->period(), CN_MODBUS_PERIOD_DEFAULT_VALUE);

    item->setPeriod(CN_MODBUS_PERIOD_DISABLE_VALUE);
    EXPECT_EQ(item->period(), CN_MODBUS_PERIOD_DISABLE_VALUE);
}

// Test messageId getter/setter
TEST_F(CnCfgDeviceItemTest, MessageIdGetterSetter)
{
    item->setMessageId("MSG001");
    EXPECT_EQ(item->messageId(), "MSG001");

    item->setMessageId("Command.Start");
    EXPECT_EQ(item->messageId(), "Command.Start");
}

// Test all ItemType values
TEST_F(CnCfgDeviceItemTest, AllItemTypes)
{
    item->setType(CnDeviceBaseItem::Item_None);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_None);

    item->setType(CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);

    item->setType(CnDeviceBaseItem::Item_Command);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Command);

    item->setType(CnDeviceBaseItem::Item_Setting);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Setting);

    item->setType(CnDeviceBaseItem::Item_Statistic);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Statistic);

    item->setType(CnDeviceBaseItem::Item_Config);
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Config);

}

// Test all DataSuffix values
TEST_F(CnCfgDeviceItemTest, AllDataSuffixValues)
{
    item->setDataSuffix(Cn::Suffix_Default);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Default);

    item->setDataSuffix(Cn::Suffix_Bool);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Bool);

    item->setDataSuffix(Cn::Suffix_Int16);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int16);

    item->setDataSuffix(Cn::Suffix_UInt16);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt16);

    item->setDataSuffix(Cn::Suffix_Int32);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int32);

    item->setDataSuffix(Cn::Suffix_UInt32);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt32);

    item->setDataSuffix(Cn::Suffix_Int64);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int64);

    item->setDataSuffix(Cn::Suffix_UInt64);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt64);

    item->setDataSuffix(Cn::Suffix_Float);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Float);

    item->setDataSuffix(Cn::Suffix_Double);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Double);

    item->setDataSuffix(Cn::Suffix_SwappedInt32);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedInt32);

    item->setDataSuffix(Cn::Suffix_SwappedUInt32);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedUInt32);

    item->setDataSuffix(Cn::Suffix_SwappedInt64);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedInt64);

    item->setDataSuffix(Cn::Suffix_SwappedUInt64);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedUInt64);

    item->setDataSuffix(Cn::Suffix_SwappedFloat);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedFloat);

    item->setDataSuffix(Cn::Suffix_SwappedDouble);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedDouble);

    item->setDataSuffix(Cn::Suffix_ByteArray);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_ByteArray);
}

// Test all Modbus MemoryType values
TEST_F(CnCfgDeviceItemTest, AllModbusMemoryTypes)
{
    item->setMemoryType(Modbus::Memory_Unknown);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_Unknown);

    item->setMemoryType(Modbus::Memory_Coils);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_Coils);

    item->setMemoryType(Modbus::Memory_0x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_0x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_Coils);

    item->setMemoryType(Modbus::Memory_DiscreteInputs);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_DiscreteInputs);

    item->setMemoryType(Modbus::Memory_1x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_1x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_DiscreteInputs);

    item->setMemoryType(Modbus::Memory_InputRegisters);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_InputRegisters);

    item->setMemoryType(Modbus::Memory_3x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_3x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_InputRegisters);

    item->setMemoryType(Modbus::Memory_HoldingRegisters);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);

    item->setMemoryType(Modbus::Memory_4x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_4x);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);
}

// Test complete configuration for Config item
TEST_F(CnCfgDeviceItemTest, CompleteConfigItemConfiguration)
{
    item->setName("ConfigItem1");
    item->setItemReference("Device.Config.Param1");
    item->setType(CnDeviceBaseItem::Item_Config);
    item->setDataSuffix(Cn::Suffix_Int16);
    item->setMemoryType(Modbus::Memory_HoldingRegisters);
    item->setOffset(100);
    item->setCount(1);
    item->setPeriod(1000);

    EXPECT_EQ(item->name(), "ConfigItem1");
    EXPECT_EQ(item->itemReference(), "Device.Config.Param1");
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Config);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int16);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);
    EXPECT_EQ(item->offset(), 100);
    EXPECT_EQ(item->count(), 1);
    EXPECT_EQ(item->period(), 1000);
}

// Test complete configuration for Command item
TEST_F(CnCfgDeviceItemTest, CompleteCommandItemConfiguration)
{
    item->setName("StartCommand");
    item->setItemReference("Device.Commands.Start");
    item->setType(CnDeviceBaseItem::Item_Command);
    item->setDataSuffix(Cn::Suffix_Bool);
    item->setMemoryType(Modbus::Memory_Coils);
    item->setOffset(50);
    item->setCount(1);
    item->setMessageId("CMD_START");

    EXPECT_EQ(item->name(), "StartCommand");
    EXPECT_EQ(item->itemReference(), "Device.Commands.Start");
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Command);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Bool);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_Coils);
    EXPECT_EQ(item->offset(), 50);
    EXPECT_EQ(item->count(), 1);
    EXPECT_EQ(item->messageId(), "CMD_START");
}

// Test complete configuration for Setting item
TEST_F(CnCfgDeviceItemTest, CompleteSettingItemConfiguration)
{
    item->setName("TemperatureSetpoint");
    item->setItemReference("Device.Settings.Temp");
    item->setType(CnDeviceBaseItem::Item_Setting);
    item->setDataSuffix(Cn::Suffix_Float);
    item->setMemoryType(Modbus::Memory_HoldingRegisters);
    item->setOffset(200);
    item->setCount(2);
    item->setPeriod(500);

    EXPECT_EQ(item->name(), "TemperatureSetpoint");
    EXPECT_EQ(item->itemReference(), "Device.Settings.Temp");
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Setting);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Float);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);
    EXPECT_EQ(item->offset(), 200);
    EXPECT_EQ(item->count(), 2);
    EXPECT_EQ(item->period(), 500);
}

// Test complete configuration for Statistic item
TEST_F(CnCfgDeviceItemTest, CompleteStatisticItemConfiguration)
{
    item->setName("RunningHours");
    item->setItemReference("Device.Stats.Hours");
    item->setType(CnDeviceBaseItem::Item_Statistic);
    item->setDataSuffix(Cn::Suffix_UInt32);
    item->setMemoryType(Modbus::Memory_InputRegisters);
    item->setOffset(300);
    item->setCount(2);
    item->setPeriod(60000);

    EXPECT_EQ(item->name(), "RunningHours");
    EXPECT_EQ(item->itemReference(), "Device.Stats.Hours");
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Statistic);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt32);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_InputRegisters);
    EXPECT_EQ(item->offset(), 300);
    EXPECT_EQ(item->count(), 2);
    EXPECT_EQ(item->period(), 60000);
}

// Test period with various values
TEST_F(CnCfgDeviceItemTest, PeriodVariousValues)
{
    // Default period
    item->setPeriod(CN_MODBUS_PERIOD_DEFAULT_VALUE);
    EXPECT_EQ(item->period(), CN_MODBUS_PERIOD_DEFAULT_VALUE);
    EXPECT_EQ(item->period(), -1);

    // Disabled period
    item->setPeriod(CN_MODBUS_PERIOD_DISABLE_VALUE);
    EXPECT_EQ(item->period(), CN_MODBUS_PERIOD_DISABLE_VALUE);
    EXPECT_EQ(item->period(), 0);

    // Normal periods
    item->setPeriod(100);
    EXPECT_EQ(item->period(), 100);

    item->setPeriod(1000);
    EXPECT_EQ(item->period(), 1000);

    item->setPeriod(60000);
    EXPECT_EQ(item->period(), 60000);
}

// Test offset boundary values
TEST_F(CnCfgDeviceItemTest, OffsetBoundaryValues)
{
    item->setOffset(0);
    EXPECT_EQ(item->offset(), 0);

    item->setOffset(65535);
    EXPECT_EQ(item->offset(), 65535);

    item->setOffset(32768);
    EXPECT_EQ(item->offset(), 32768);
}

// Test count boundary values
TEST_F(CnCfgDeviceItemTest, CountBoundaryValues)
{
    item->setCount(0);
    EXPECT_EQ(item->count(), 0);

    item->setCount(1);
    EXPECT_EQ(item->count(), 1);

    item->setCount(127);
    EXPECT_EQ(item->count(), 127);

    item->setCount(255);
    EXPECT_EQ(item->count(), 255);
}

// Test empty name and itemReference
TEST_F(CnCfgDeviceItemTest, EmptyStrings)
{
    item->setName("");
    EXPECT_EQ(item->name(), "");

    item->setItemReference("");
    EXPECT_EQ(item->itemReference(), "");

    item->setMessageId("");
    EXPECT_EQ(item->messageId(), "");
}

// Test name with special characters
TEST_F(CnCfgDeviceItemTest, NameWithSpecialCharacters)
{
    item->setName("Item_123");
    EXPECT_EQ(item->name(), "Item_123");

    item->setName("Item.Sub.Value");
    EXPECT_EQ(item->name(), "Item.Sub.Value");

    item->setName("Item-With-Dashes");
    EXPECT_EQ(item->name(), "Item-With-Dashes");
}

// Test itemReference with hierarchical structure
TEST_F(CnCfgDeviceItemTest, ItemReferenceHierarchical)
{
    item->setItemReference("Device1.SubDevice.Parameter.Value");
    EXPECT_EQ(item->itemReference(), "Device1.SubDevice.Parameter.Value");

    item->setItemReference("System.Controller.Module.Sensor.Temperature");
    EXPECT_EQ(item->itemReference(), "System.Controller.Module.Sensor.Temperature");
}

// Test multiple modifications
TEST_F(CnCfgDeviceItemTest, MultipleModifications)
{
    // Initial setup
    item->setName("Item1");
    item->setOffset(100);
    EXPECT_EQ(item->name(), "Item1");
    EXPECT_EQ(item->offset(), 100);

    // Modify
    item->setName("Item2");
    item->setOffset(200);
    EXPECT_EQ(item->name(), "Item2");
    EXPECT_EQ(item->offset(), 200);

    // Modify again
    item->setName("Item3");
    item->setOffset(300);
    EXPECT_EQ(item->name(), "Item3");
    EXPECT_EQ(item->offset(), 300);
}

// Test Modbus item configuration
TEST_F(CnCfgDeviceItemTest, ModbusItemConfiguration)
{
    item->setName("ModbusRegister");
    item->setItemReference("Device.Modbus.Register");
    item->setType(CnDeviceBaseItem::Item_Modbus);
    item->setDataSuffix(Cn::Suffix_Int32);
    item->setMemoryType(Modbus::Memory_HoldingRegisters);
    item->setOffset(1000);
    item->setCount(2);
    item->setPeriod(250);

    EXPECT_EQ(item->name(), "ModbusRegister");
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int32);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);
    EXPECT_EQ(item->offset(), 1000);
    EXPECT_EQ(item->count(), 2);
    EXPECT_EQ(item->period(), 250);
}

// Test swapped data types
TEST_F(CnCfgDeviceItemTest, SwappedDataTypes)
{
    item->setDataSuffix(Cn::Suffix_SwappedInt32);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedInt32);

    item->setDataSuffix(Cn::Suffix_SwappedFloat);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedFloat);

    item->setDataSuffix(Cn::Suffix_SwappedDouble);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedDouble);
}

// Test byte array data type
TEST_F(CnCfgDeviceItemTest, ByteArrayDataType)
{
    item->setDataSuffix(Cn::Suffix_ByteArray);
    item->setCount(20);

    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_ByteArray);
    EXPECT_EQ(item->count(), 20);
}

// Test constructor with different types
TEST_F(CnCfgDeviceItemTest, ConstructorVariations)
{
    CnCfgDeviceItem* item1 = new CnCfgDeviceItem(
        CnDeviceBaseItem::Item_None,
        Cn::Suffix_Default
    );
    EXPECT_EQ(item1->type(), CnDeviceBaseItem::Item_None);
    EXPECT_EQ(item1->dataSuffix(), Cn::Suffix_Default);
    delete item1;
}
