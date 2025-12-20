#include "CnModbusClient.h"

CnModbusClient::CnModbusClient(uint8_t unit, ModbusClientPort *port) :
    ModbusObject(),
    m_port(port),
    m_unit(unit),
    m_tries(1),
    m_innerTries(0)
{
}

#define CN_MODBUS_CLIENT_CALL(func, ...) \
    Modbus::StatusCode status = m_port->func(this, m_unit, __VA_ARGS__); \
    if (Modbus::StatusIsProcessing(status)) \
        return status; \
    if (Modbus::StatusIsBad(status)) \
    { \
        ++m_innerTries; \
        if (m_innerTries < m_tries) \
            return Modbus::Status_Processing; \
    } \
    m_innerTries = 0; \
    return status;

Modbus::StatusCode CnModbusClient::readCoils(uint16_t offset, uint16_t count, void *values)
{
    CN_MODBUS_CLIENT_CALL(readCoils, offset, count, values);
}

Modbus::StatusCode CnModbusClient::readDiscreteInputs(uint16_t offset, uint16_t count, void *values)
{
    CN_MODBUS_CLIENT_CALL(readDiscreteInputs, offset, count, values);
}

Modbus::StatusCode CnModbusClient::readHoldingRegisters(uint16_t offset, uint16_t count, uint16_t *values)
{
    CN_MODBUS_CLIENT_CALL(readHoldingRegisters, offset, count, values);
}

Modbus::StatusCode CnModbusClient::readInputRegisters(uint16_t offset, uint16_t count, uint16_t *values)
{
    CN_MODBUS_CLIENT_CALL(readInputRegisters, offset, count, values);
}

Modbus::StatusCode CnModbusClient::writeSingleCoil(uint16_t offset, bool value)
{
    CN_MODBUS_CLIENT_CALL(writeSingleCoil, offset, value);
}

Modbus::StatusCode CnModbusClient::writeSingleRegister(uint16_t offset, uint16_t value)
{
    CN_MODBUS_CLIENT_CALL(writeSingleRegister, offset, value);
}

Modbus::StatusCode CnModbusClient::readExceptionStatus(uint8_t *value)
{
    CN_MODBUS_CLIENT_CALL(readExceptionStatus, value);
}

Modbus::StatusCode CnModbusClient::writeMultipleCoils(uint16_t offset, uint16_t count, const void *values)
{
    CN_MODBUS_CLIENT_CALL(writeMultipleCoils, offset, count, values);
}

Modbus::StatusCode CnModbusClient::writeMultipleRegisters(uint16_t offset, uint16_t count, const uint16_t *values)
{
    CN_MODBUS_CLIENT_CALL(writeMultipleRegisters, offset, count, values);
}
