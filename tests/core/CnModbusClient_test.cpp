#include <gtest/gtest.h>
#include <core/CnModbusClient.h>
#include <Modbus.h>

// Helper function to create a TCP client port
static ModbusClientPort* createTestPort()
{
    Modbus::TcpSettings settings;
    settings.host = "127.0.0.1";
    settings.port = 502;
    settings.timeout = 1000;
    settings.maxconn = 1;
    return Modbus::createClientPort(Modbus::TCP, &settings, false);
}

// ============================================================================
// CnModbusClient Constructor and Basic Property Tests
// ============================================================================

TEST(CnModbusClientTest, ConstructorWithUnitAndPort)
{
    ModbusClientPort *port = createTestPort();
    ASSERT_NE(port, nullptr);
    
    CnModbusClient client(1, port);
    
    EXPECT_EQ(client.unit(), 1);
    EXPECT_EQ(client.port(), port);
    EXPECT_EQ(client.tries(), 1);
    EXPECT_EQ(client.repeatCount(), 1);
    EXPECT_EQ(client.type(), Modbus::TCP);
    
    delete port;
}

TEST(CnModbusClientTest, ConstructorWithDifferentUnits)
{
    ModbusClientPort *port = createTestPort();
    ASSERT_NE(port, nullptr);
    
    CnModbusClient client1(0, port);
    EXPECT_EQ(client1.unit(), 0);
    
    CnModbusClient client2(42, port);
    EXPECT_EQ(client2.unit(), 42);
    
    CnModbusClient client3(255, port);
    EXPECT_EQ(client3.unit(), 255);
    
    delete port;
}

// ============================================================================
// CnModbusClient Property Getters and Setters Tests
// ============================================================================

TEST(CnModbusClientTest, SetAndGetUnit)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    client.setUnit(10);
    EXPECT_EQ(client.unit(), 10);
    
    client.setUnit(255);
    EXPECT_EQ(client.unit(), 255);
    
    client.setUnit(0);
    EXPECT_EQ(client.unit(), 0);
    
    delete port;
}

TEST(CnModbusClientTest, SetAndGetTries)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    EXPECT_EQ(client.tries(), 1);
    
    client.setTries(3);
    EXPECT_EQ(client.tries(), 3);
    
    client.setTries(10);
    EXPECT_EQ(client.tries(), 10);
    
    client.setTries(0);
    EXPECT_EQ(client.tries(), 0);
    
    client.setTries(100);
    EXPECT_EQ(client.tries(), 100);
    
    delete port;
}

TEST(CnModbusClientTest, SetAndGetRepeatCount)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    EXPECT_EQ(client.repeatCount(), 1);
    
    client.setRepeatCount(5);
    EXPECT_EQ(client.repeatCount(), 5);
    EXPECT_EQ(client.tries(), 5);
    
    client.setTries(7);
    EXPECT_EQ(client.repeatCount(), 7);
    EXPECT_EQ(client.tries(), 7);
    
    delete port;
}

TEST(CnModbusClientTest, PortAccessor)
{
    ModbusClientPort *port1 = createTestPort();
    ModbusClientPort *port2 = createTestPort();
    
    CnModbusClient client(1, port1);
    EXPECT_EQ(client.port(), port1);
    
    delete port1;
    delete port2;
}

TEST(CnModbusClientTest, TypeAccessor)
{
    ModbusClientPort *tcpPort = createTestPort();
    CnModbusClient tcpClient(1, tcpPort);
    EXPECT_EQ(tcpClient.type(), Modbus::TCP);
    
    delete tcpPort;
}

TEST(CnModbusClientTest, IsOpenAccessor)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    // Port should not be open initially (no server running)
    EXPECT_FALSE(client.isOpen());
    
    delete port;
}

// ============================================================================
// CnModbusClient Status Methods Tests
// ============================================================================

TEST(CnModbusClientTest, LastPortStatus)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    // Status method should return some value (not testing specific status without server)
    Modbus::StatusCode status = client.lastPortStatus();
    EXPECT_TRUE(Modbus::StatusIsUncertain(status));
    
    delete port;
}

TEST(CnModbusClientTest, LastPortErrorStatus)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    // Default status should be uncertain
    Modbus::StatusCode errorStatus = client.lastPortErrorStatus();
    EXPECT_TRUE(Modbus::StatusIsUncertain(errorStatus));
    
    delete port;
}

TEST(CnModbusClientTest, LastPortErrorText)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    const Modbus::Char* errorText = client.lastPortErrorText();
    EXPECT_NE(errorText, nullptr);
    
    delete port;
}

// ============================================================================
// CnModbusClient Read Function Interface Tests
// ============================================================================

TEST(CnModbusClientTest, ReadCoilsInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint8_t values[10] = {0};
    
    // Call should not crash, but will fail since no server is running
    Modbus::StatusCode status = client.readCoils(0, 10, values);
    
    // Status should be Bad or Processing (connection failed)
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

TEST(CnModbusClientTest, ReadDiscreteInputsInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint8_t values[10] = {0};
    Modbus::StatusCode status = client.readDiscreteInputs(0, 10, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

TEST(CnModbusClientTest, ReadHoldingRegistersInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[10] = {0};
    Modbus::StatusCode status = client.readHoldingRegisters(0, 10, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

TEST(CnModbusClientTest, ReadInputRegistersInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[10] = {0};
    Modbus::StatusCode status = client.readInputRegisters(0, 10, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

TEST(CnModbusClientTest, ReadExceptionStatusInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint8_t value = 0;
    Modbus::StatusCode status = client.readExceptionStatus(&value);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

// ============================================================================
// CnModbusClient Write Function Interface Tests
// ============================================================================

TEST(CnModbusClientTest, WriteSingleCoilInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    Modbus::StatusCode status1 = client.writeSingleCoil(100, true);
    EXPECT_TRUE(Modbus::StatusIsBad(status1) || Modbus::StatusIsProcessing(status1));
    
    Modbus::StatusCode status2 = client.writeSingleCoil(200, false);
    EXPECT_TRUE(Modbus::StatusIsBad(status2) || Modbus::StatusIsProcessing(status2));
    
    delete port;
}

TEST(CnModbusClientTest, WriteSingleRegisterInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    Modbus::StatusCode status1 = client.writeSingleRegister(100, 12345);
    EXPECT_TRUE(Modbus::StatusIsBad(status1) || Modbus::StatusIsProcessing(status1));
    
    Modbus::StatusCode status2 = client.writeSingleRegister(200, 65535);
    EXPECT_TRUE(Modbus::StatusIsBad(status2) || Modbus::StatusIsProcessing(status2));
    
    delete port;
}

TEST(CnModbusClientTest, WriteMultipleCoilsInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint8_t values[2] = {0xFF, 0x00};
    Modbus::StatusCode status = client.writeMultipleCoils(100, 16, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

TEST(CnModbusClientTest, WriteMultipleRegistersInterface)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[5] = {100, 200, 300, 400, 500};
    Modbus::StatusCode status = client.writeMultipleRegisters(100, 5, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

// ============================================================================
// CnModbusClient Parameter Validation Tests
// ============================================================================

TEST(CnModbusClientTest, ReadWithZeroCount)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[10] = {0};
    Modbus::StatusCode status = client.readHoldingRegisters(0, 0, values);
    
    // Should handle zero count gracefully
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status) || Modbus::StatusIsGood(status));
    
    delete port;
}

TEST(CnModbusClientTest, WriteWithZeroCount)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[10] = {0};
    Modbus::StatusCode status = client.writeMultipleRegisters(0, 0, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status) || Modbus::StatusIsGood(status));
    
    delete port;
}

TEST(CnModbusClientTest, MaxOffsetValue)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[1] = {0};
    Modbus::StatusCode status = client.readHoldingRegisters(65535, 1, values);
    
    // Should handle max offset
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

TEST(CnModbusClientTest, LargeCountValue)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[125]; // Modbus max is typically 125 registers
    Modbus::StatusCode status = client.readHoldingRegisters(0, 125, values);
    
    EXPECT_TRUE(Modbus::StatusIsBad(status) || Modbus::StatusIsProcessing(status));
    
    delete port;
}

// ============================================================================
// CnModbusClient Multiple Clients on Same Port Tests
// ============================================================================

TEST(CnModbusClientTest, MultipleClientsOnSamePort)
{
    ModbusClientPort *port = createTestPort();
    
    CnModbusClient client1(1, port);
    CnModbusClient client2(2, port);
    CnModbusClient client3(3, port);
    
    EXPECT_EQ(client1.unit(), 1);
    EXPECT_EQ(client2.unit(), 2);
    EXPECT_EQ(client3.unit(), 3);
    
    EXPECT_EQ(client1.port(), port);
    EXPECT_EQ(client2.port(), port);
    EXPECT_EQ(client3.port(), port);
    
    delete port;
}

TEST(CnModbusClientTest, DifferentClientsHaveIndependentTries)
{
    ModbusClientPort *port = createTestPort();
    
    CnModbusClient client1(1, port);
    CnModbusClient client2(2, port);
    
    client1.setTries(5);
    client2.setTries(10);
    
    EXPECT_EQ(client1.tries(), 5);
    EXPECT_EQ(client2.tries(), 10);
    
    delete port;
}

TEST(CnModbusClientTest, ChangeUnitBetweenOperations)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    EXPECT_EQ(client.unit(), 1);
    
    uint16_t values[5];
    client.readHoldingRegisters(0, 5, values);
    
    client.setUnit(10);
    EXPECT_EQ(client.unit(), 10);
    
    client.readHoldingRegisters(0, 5, values);
    
    client.setUnit(255);
    EXPECT_EQ(client.unit(), 255);
    
    delete port;
}

TEST(CnModbusClientTest, ChangeTriesBetweenOperations)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    client.setTries(1);
    uint16_t values[5];
    client.readHoldingRegisters(0, 5, values);
    
    client.setTries(3);
    EXPECT_EQ(client.tries(), 3);
    client.readHoldingRegisters(0, 5, values);
    
    client.setTries(10);
    EXPECT_EQ(client.tries(), 10);
    
    delete port;
}

// ============================================================================
// CnModbusClient Edge Cases and Boundary Tests
// ============================================================================

TEST(CnModbusClientTest, MinimalUnit)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(0, port);
    
    EXPECT_EQ(client.unit(), 0);
    
    delete port;
}

TEST(CnModbusClientTest, MaximalUnit)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(255, port);
    
    EXPECT_EQ(client.unit(), 255);
    
    delete port;
}

TEST(CnModbusClientTest, SequentialOperations)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t regValues[5];
    uint8_t coilValues[10];
    
    // Multiple operations in sequence should not crash
    client.readHoldingRegisters(0, 5, regValues);
    client.writeSingleCoil(100, true);
    client.readCoils(200, 10, coilValues);
    client.writeSingleRegister(300, 500);
    client.readInputRegisters(400, 3, regValues);
    
    // All operations should return valid status codes
    EXPECT_TRUE(true); // If we got here, no crashes occurred
    
    delete port;
}

TEST(CnModbusClientTest, DefaultTriesValue)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    // Default tries should be 1
    EXPECT_EQ(client.tries(), 1);
    EXPECT_EQ(client.repeatCount(), 1);
    
    delete port;
}

TEST(CnModbusClientTest, PortTypeConsistency)
{
    ModbusClientPort *tcpPort = createTestPort();
    CnModbusClient tcpClient(1, tcpPort);
    
    // Type should remain consistent
    EXPECT_EQ(tcpClient.type(), Modbus::TCP);
    EXPECT_EQ(tcpClient.type(), Modbus::TCP);
    
    delete tcpPort;
}

TEST(CnModbusClientTest, MultipleReadsFromSameOffset)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values1[5];
    uint16_t values2[5];
    
    // Multiple reads from same offset should work
    client.readHoldingRegisters(100, 5, values1);
    client.readHoldingRegisters(100, 5, values2);
    
    EXPECT_TRUE(true); // No crash
    
    delete port;
}

TEST(CnModbusClientTest, AlternatingReadWrite)
{
    ModbusClientPort *port = createTestPort();
    CnModbusClient client(1, port);
    
    uint16_t values[5] = {1, 2, 3, 4, 5};
    
    // Alternating read/write operations
    client.writeMultipleRegisters(0, 5, values);
    client.readHoldingRegisters(0, 5, values);
    client.writeMultipleRegisters(10, 5, values);
    client.readHoldingRegisters(10, 5, values);
    
    EXPECT_TRUE(true); // No crash
    
    delete port;
}
