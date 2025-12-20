/*!
 * \file   CnCfgDeviceItemParser_test.cpp
 * \brief  Unit tests for CnCfgDeviceItemParser class
 *
 * \author GitHub Copilot
 * \date   November 2024
 */
#include "gtest/gtest.h"
#include <cfg/CnCfgDeviceItemParser.h>
#include <cfg/CnCfgDeviceItem.h>
#include <device/items/CnDeviceBaseItem.h>

class CnCfgDeviceItemParserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        parser = new CnCfgDeviceItemParser();
    }

    void TearDown() override
    {
        delete parser;
    }

    CnCfgDeviceItemParser* parser;
};

// ========== SubSystemStatus Tests ==========

// ========== Modbus Item Tests ==========

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_Coil)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("000123"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_Coils);
    EXPECT_EQ(item->offset(), 122);  // 000123 - 1
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_DiscreteInput)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("100456"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_DiscreteInputs);
    EXPECT_EQ(item->offset(), 455);  // 100456 - 1
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_InputRegister)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("300001"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_InputRegisters);
    EXPECT_EQ(item->offset(), 0);  // 300001 - 1
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_HoldingRegister)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400123"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->memoryType(), Modbus::Memory_HoldingRegisters);
    EXPECT_EQ(item->offset(), 122);  // 400123 - 1
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithSuffixInt16)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 S"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int16);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithSuffixUInt16)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 R"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt16);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithSuffixInt32)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 I"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int32);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithSuffixFloat)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 F"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Float);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithSuffixDouble)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 LF"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Double);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithMessageId)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001, msgid=MSG1"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->messageId(), "MSG1");
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithNumericMessageId)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001, msgid=123"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->messageId(), "123");
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithPeriod)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001, 1000"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->period(), 1000);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_WithMessageIdAndPeriod)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001, msgid=MSG1, 2000"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->messageId(), "MSG1");
    EXPECT_EQ(item->period(), 2000);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_Array)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001-400010 B"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->type(), CnDeviceBaseItem::Item_Modbus);
    EXPECT_EQ(item->offset(), 0);  // 400001 - 1
    EXPECT_EQ(item->count(), 10);  // 400010 - 400001 + 1
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_ByteArray);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_ArrayWithMessageId)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001-400005 B, msgid=ARRAY1"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_FALSE(parser->hasError());
    EXPECT_EQ(item->count(), 5);
    EXPECT_EQ(item->messageId(), "ARRAY1");
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_ArrayTypeMismatch)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001-300010 B"));
    
    EXPECT_EQ(item, nullptr);
    EXPECT_TRUE(parser->hasError());
}

TEST_F(CnCfgDeviceItemParserTest, ParseModbusItem_ArrayInvalidRange)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400010-400005 B"));
    
    EXPECT_EQ(item, nullptr);
    EXPECT_TRUE(parser->hasError());
}

// ========== Error Handling Tests ==========

TEST_F(CnCfgDeviceItemParserTest, ParseError_EmptyString)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR(""));
    
    EXPECT_EQ(item, nullptr);
    EXPECT_TRUE(parser->hasError());
}

TEST_F(CnCfgDeviceItemParserTest, ParseError_UnknownReference)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("UNKNOWN"));
    
    EXPECT_EQ(item, nullptr);
    EXPECT_TRUE(parser->hasError());
}

TEST_F(CnCfgDeviceItemParserTest, ParseError_InvalidSuffix)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 INVALID"));
    
    EXPECT_EQ(item, nullptr);
    EXPECT_TRUE(parser->hasError());
}

TEST_F(CnCfgDeviceItemParserTest, ParseError_MissingMessageIdValue)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001, msgid="));
    
    EXPECT_EQ(item, nullptr);
    EXPECT_TRUE(parser->hasError());
}

// ========== Parser State Tests ==========

TEST_F(CnCfgDeviceItemParserTest, ParserState_HasErrorAndClear)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("INVALID"));
    delete item;
    
    EXPECT_TRUE(parser->hasError());
    EXPECT_NE(parser->errorString()[0], '\0');
    
    parser->clearError();
    
    EXPECT_FALSE(parser->hasError());
}

TEST_F(CnCfgDeviceItemParserTest, ParserState_MultipleParses)
{
    CnCfgDeviceItem* item2 = parser->parse(CnSTR("400001"));
    ASSERT_NE(item2, nullptr);
    delete item2;
    
    CnCfgDeviceItem* item3 = parser->parse(CnSTR("INVALID"));
    EXPECT_EQ(item3, nullptr);
    EXPECT_TRUE(parser->hasError());
    delete item3;
}

// ========== All Data Suffix Tests ==========

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_Int64)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 LL"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_Int64);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_UInt32)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 U"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt32);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_UInt64)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 UL"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_UInt64);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_SwappedInt32)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 IS"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedInt32);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_SwappedUInt32)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 US"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedUInt32);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_SwappedInt64)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 LLS"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedInt64);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_SwappedUInt64)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 ULS"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedUInt64);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_SwappedFloat)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 FS"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedFloat);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, ParseSuffix_SwappedDouble)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001 LFS"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->dataSuffix(), Cn::Suffix_SwappedDouble);
    
    delete item;
}

// ========== Edge Cases ==========

TEST_F(CnCfgDeviceItemParserTest, EdgeCase_MinModbusAddress)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("000001"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->offset(), 0);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, EdgeCase_MaxModbusAddress)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("465536"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->offset(), 65535);
    
    delete item;
}

TEST_F(CnCfgDeviceItemParserTest, EdgeCase_ComplexItemReference)
{
    CnCfgDeviceItem* item = parser->parse(CnSTR("400001-400010 B, msgid=TEST, 5000"));
    
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->itemReference(), "400001-400010 B, msgid=TEST, 5000");
    
    delete item;
}
