/*!
 * \file   CnDeviceMessageList_test.cpp
 * \brief  Unit tests for CnDeviceMessageList classes
 *
 * \details Tests CnDevicePollMessageList and CnDevicePokeMessageList
 */

#include <gtest/gtest.h>
#include <device/messages/CnDeviceMessageList.h>
#include <device/messages/CnDeviceMessage.h>
#include <device/CnDevice.h>
#include <device/items/CnDeviceModbusItem.h>

// Mock device for testing
class MockDevice : public CnDevice
{
public:
    MockDevice() : CnDevice(nullptr,
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr) {}
};

// =============================================================================
// CnDevicePollMessageList Constructor/Destructor Tests
// =============================================================================

TEST(CnDevicePollMessageListTest, Constructor)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    EXPECT_FALSE(list.hasMessage());
    EXPECT_EQ(list.current(), nullptr);
}

TEST(CnDevicePollMessageListTest, Destructor)
{
    MockDevice device;
    {
        CnDevicePollMessageList list(&device);
        // Should destroy cleanly
    }
    // Test passes if no crash
}

// =============================================================================
// CnDevicePokeMessageList Constructor/Destructor Tests
// =============================================================================

TEST(CnDevicePokeMessageListTest, Constructor)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    EXPECT_FALSE(list.hasMessage());
    EXPECT_EQ(list.current(), nullptr);
}

TEST(CnDevicePokeMessageListTest, Destructor)
{
    MockDevice device;
    {
        CnDevicePokeMessageList list(&device);
        // Should destroy cleanly
    }
    // Test passes if no crash
}

// =============================================================================
// Poll Message List - Add Item Tests
// =============================================================================

TEST(CnDevicePollMessageListTest, AddSingleItem)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    EXPECT_TRUE(list.hasMessage());
}

TEST(CnDevicePollMessageListTest, AddMultipleItems)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item1(&device, 0, 1000, "msg1");
    CnDeviceModbusItem4xUInt16 item2(&device, 10, 1000, "msg2");
    CnDeviceModbusItem4xUInt16 item3(&device, 20, 1000, "msg3");
    
    list.addItem(&item1);
    list.addItem(&item2);
    list.addItem(&item3);
    
    EXPECT_TRUE(list.hasMessage());
}

// =============================================================================
// Poke Message List - Add Item Tests
// =============================================================================

TEST(CnDevicePokeMessageListTest, AddSingleItem)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    EXPECT_TRUE(list.hasMessage());
}

TEST(CnDevicePokeMessageListTest, AddMultipleItems)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item1(&device, 0, 1000, "msg1");
    CnDeviceModbusItem4xUInt16 item2(&device, 10, 1000, "msg2");
    CnDeviceModbusItem4xUInt16 item3(&device, 20, 1000, "msg3");
    
    list.addItem(&item1);
    list.addItem(&item2);
    list.addItem(&item3);
    
    EXPECT_TRUE(list.hasMessage());
}

// =============================================================================
// Poll Message List - Pop Message Tests
// =============================================================================

TEST(CnDevicePollMessageListTest, PopMessage)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    CnDeviceMessageBase* msg = nullptr;
    bool result = list.popMessage(&msg);
    
    EXPECT_TRUE(result);
    EXPECT_NE(msg, nullptr);
    EXPECT_EQ(list.current(), msg);
}

TEST(CnDevicePollMessageListTest, PopMessageEmpty)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceMessageBase* msg = nullptr;
    bool result = list.popMessage(&msg);
    
    EXPECT_FALSE(result);
    EXPECT_EQ(msg, nullptr);
}

TEST(CnDevicePollMessageListTest, PopAndEndMessage)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 *item = new CnDeviceModbusItem4xUInt16(&device, 0, 1000, "msg1");
    list.addItem(item);
    
    CnDeviceMessageBase* msg = nullptr;
    list.popMessage(&msg);
    
    EXPECT_NE(msg, nullptr);
    
    list.endMessage(msg);
    
    // Current should be cleared after end
    EXPECT_EQ(list.current(), nullptr);

    // item should still exist. It's not belonging to the message list
    // so we need to delete it manually
    delete item;
}

// =============================================================================
// Poke Message List - Pop Message Tests
// =============================================================================

TEST(CnDevicePokeMessageListTest, PopMessage)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    CnDeviceMessageBase* msg = nullptr;
    bool result = list.popMessage(&msg);
    
    EXPECT_TRUE(result);
    EXPECT_NE(msg, nullptr);
    EXPECT_EQ(list.current(), msg);
}

TEST(CnDevicePokeMessageListTest, PopMessageEmpty)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceMessageBase* msg = nullptr;
    bool result = list.popMessage(&msg);
    
    EXPECT_FALSE(result);
    EXPECT_EQ(msg, nullptr);
}

TEST(CnDevicePokeMessageListTest, PopAndEndMessage)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 *item = new CnDeviceModbusItem4xUInt16(&device, 0, 1000, "msg1");
    list.addItem(item);
    
    CnDeviceMessageBase* msg = nullptr;
    list.popMessage(&msg);
    
    EXPECT_NE(msg, nullptr);
    
    list.endMessage(msg);
    
    EXPECT_EQ(list.current(), nullptr);
}

// =============================================================================
// Add Message Tests
// =============================================================================

TEST(CnDeviceMessageListTest, AddMessagePoll)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    CnDevicePollMessage* msg = Cn::createDeviceModbusPollMessage(&device, &item);
    
    list.addMessage(msg);
    
    EXPECT_TRUE(list.hasMessage());
}

TEST(CnDeviceMessageListTest, AddMessagePoke)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    CnDevicePokeMessage* msg = Cn::createDeviceModbusPokeMessage(&device, &item);
    
    list.addMessage(msg);
    
    EXPECT_TRUE(list.hasMessage());
}

// =============================================================================
// Clear Tests
// =============================================================================

TEST(CnDeviceMessageListTest, ClearPoll)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item1(&device, 0, 1000, "msg1");
    CnDeviceModbusItem4xUInt16 item2(&device, 10, 1000, "msg2");
    
    list.addItem(&item1);
    list.addItem(&item2);
    
    EXPECT_TRUE(list.hasMessage());
    
    list.clear();
    
    EXPECT_FALSE(list.hasMessage());
}

TEST(CnDeviceMessageListTest, ClearPoke)
{
    MockDevice device;
    CnDevicePokeMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 *item1 = new CnDeviceModbusItem4xUInt16(&device, 0, 1000, "msg1");
    CnDeviceModbusItem4xUInt16 *item2 = new CnDeviceModbusItem4xUInt16(&device, 10, 1000, "msg2");
    
    list.addItem(item1);
    list.addItem(item2);
    
    EXPECT_TRUE(list.hasMessage());
    
    list.clear();
    
    EXPECT_FALSE(list.hasMessage());
}

TEST(CnDeviceMessageListTest, ClearEmpty)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    // Clear when already empty
    list.clear();
    
    EXPECT_FALSE(list.hasMessage());
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST(CnDeviceMessageListEdgeCasesTest, MultiplePopSameMessage)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    CnDeviceMessageBase* msg1 = nullptr;
    list.popMessage(&msg1);
    
    // Try to pop again without ending first message
    CnDeviceMessageBase* msg2 = nullptr;
    bool result = list.popMessage(&msg2);
    
    // Behavior depends on implementation - may return false or same message
    // Just verify no crash
    (void)result;
}

TEST(CnDeviceMessageListEdgeCasesTest, EndMessageNull)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    // Try to end null message
    list.endMessage(nullptr);
    
    // Should not crash
}

TEST(CnDeviceMessageListEdgeCasesTest, HasMessageAfterClear)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    EXPECT_TRUE(list.hasMessage());
    
    list.clear();
    
    EXPECT_FALSE(list.hasMessage());
}

TEST(CnDeviceMessageListEdgeCasesTest, AddItemAfterClear)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item1(&device, 0, 1000, "msg1");
    list.addItem(&item1);
    list.clear();
    
    CnDeviceModbusItem4xUInt16 item2(&device, 10, 1000, "msg2");
    list.addItem(&item2);
    
    EXPECT_TRUE(list.hasMessage());
}

TEST(CnDeviceMessageListEdgeCasesTest, DifferentMemoryTypes)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem0xBool item0x(&device, 0, 1000, "msg1");
    CnDeviceModbusItem1xBool item1x(&device, 0, 1000, "msg2");
    CnDeviceModbusItem3xUInt16 item3x(&device, 0, 1000, "msg3");
    CnDeviceModbusItem4xUInt16 item4x(&device, 0, 1000, "msg4");
    
    list.addItem(&item0x);
    list.addItem(&item1x);
    list.addItem(&item3x);
    list.addItem(&item4x);
    
    EXPECT_TRUE(list.hasMessage());
}

TEST(CnDeviceMessageListEdgeCasesTest, ManyItems)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    // Add many items
    std::vector<CnDeviceModbusItem4xUInt16*> items;
    for (int i = 0; i < 100; ++i)
    {
        CnString msgId = CnString::fromStdString(std::to_string(i));
        items.push_back(new CnDeviceModbusItem4xUInt16(&device, i * 10, 1000, msgId));
        list.addItem(items.back());
    }
    
    EXPECT_TRUE(list.hasMessage());
    
    // Clean up
    for (auto item : items)
    {
        delete item;
    }
}

// =============================================================================
// Poll vs Poke Differences
// =============================================================================

TEST(CnDeviceMessageListComparisonTest, BothTypesWork)
{
    MockDevice device;
    
    CnDevicePollMessageList pollList(&device);
    CnDevicePokeMessageList pokeList(&device);
    
    CnDeviceModbusItem4xUInt16 item1(&device, 0, 1000, "msg1");
    CnDeviceModbusItem4xUInt16 item2(&device, 10, 1000, "msg2");
    
    pollList.addItem(&item1);
    pokeList.addItem(&item2);
    
    EXPECT_TRUE(pollList.hasMessage());
    EXPECT_TRUE(pokeList.hasMessage());
}

TEST(CnDeviceMessageListComparisonTest, IndependentLists)
{
    MockDevice device;
    
    CnDevicePollMessageList pollList(&device);
    CnDevicePokeMessageList pokeList(&device);
    
    CnDeviceModbusItem4xUInt16 item1(&device, 0, 1000, "msg1");
    
    pollList.addItem(&item1);
    
    // Poll list has message, poke list doesn't
    EXPECT_TRUE(pollList.hasMessage());
    EXPECT_FALSE(pokeList.hasMessage());
}

// =============================================================================
// Current Message Tests
// =============================================================================

TEST(CnDeviceMessageListCurrentTest, CurrentAfterPop)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    EXPECT_EQ(list.current(), nullptr);
    
    CnDeviceMessageBase* msg = nullptr;
    list.popMessage(&msg);
    
    EXPECT_EQ(list.current(), msg);
}

TEST(CnDeviceMessageListCurrentTest, CurrentAfterEnd)
{
    MockDevice device;
    CnDevicePollMessageList list(&device);
    
    CnDeviceModbusItem4xUInt16 item(&device, 0, 1000, "msg1");
    list.addItem(&item);
    
    CnDeviceMessageBase* msg = nullptr;
    list.popMessage(&msg);
    
    EXPECT_NE(list.current(), nullptr);
    
    list.endMessage(msg);
    
    EXPECT_EQ(list.current(), nullptr);
}
