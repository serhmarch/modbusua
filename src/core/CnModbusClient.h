/*!
 * \file   CnModbusClient.h
 * \brief  File for Converter-Modbus client wrapper.
 *
 * \author serhmarch
 * \date   August 2025
 */
#ifndef CNMODBUSCLIENT_H
#define CNMODBUSCLIENT_H

#include <ModbusClientPort.h>

/*! \brief The `CnModbusClient` class implements the interface of the client part of the Modbus converter.

    \details `CnModbusClient` contains a list of Modbus functions that are implemented by the Modbus client program.
    It implements functions for reading and writing different types of Modbus memory that are defined by the specification.
    The operations that return `Modbus::StatusCode` are asynchronous, that is, if the operation is not completed,
    it returns the intermediate status `Modbus::Status_Processing`,
    and then it must be called until it is successfully completed or returns an error status.

 */

class CnModbusClient : public ModbusObject
{
public:
    /// \details Class constructor.
    /// \param[in] unit The address of the remote Modbus device to which this client is bound.
    /// \param[in] port A pointer to the port object to which this client object belongs.
    CnModbusClient(uint8_t unit, ModbusClientPort *port);

public:
    /// \details Returns the pointer to the port object to which this client object belongs.
    inline ModbusClientPort *port() const { return m_port; }

    /// \details Returns type of Modbus protocol.
    inline Modbus::ProtocolType type() const { return m_port->type(); }

    /// \details Returns `true` if the port is open, `false` otherwise.
    inline bool isOpen() const { return m_port->isOpen(); }

    /// \details Returns the address of the remote Modbus device to which this client is bound.
    inline uint8_t unit() const { return m_unit; }

    /// \details Sets the address of the remote Modbus device to which this client is bound.
    inline void setUnit(uint8_t unit) { m_unit = unit; }

    /// \details Returns the number of tries a Modbus request is repeated if it fails.
    inline uint32_t tries() const { return m_tries; }

    /// \details Sets the number of tries a Modbus request is repeated if it fails.
    inline void setTries(uint32_t v) { m_tries = v; }

    /// \details Same as `tries()`. Used for backward compatibility.
    inline uint32_t repeatCount() const { return tries(); }

    /// \details Same as `setTries()`. Used for backward compatibility.
    inline void setRepeatCount(uint32_t v) { setTries(v); }

public:
    /// \details Function for read discrete outputs (coils, 0x bits).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  count   Count of coils (bits).
    /// \param[out] values  Pointer to the output buffer (bit array) where the read values are stored.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode readCoils(uint16_t offset, uint16_t count, void *values);

    /// \details Function for read digital inputs (1x bits).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  count   Count of inputs (bits).
    /// \param[out] values  Pointer to the output buffer (bit array) where the read values are stored.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode readDiscreteInputs(uint16_t offset, uint16_t count, void *values);

    /// \details Function for read holding (output) 16-bit registers (4x regs).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  count   Count of registers.
    /// \param[out] values  Pointer to the output buffer (bit array) where the read values are stored.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode readHoldingRegisters(uint16_t offset, uint16_t count, uint16_t *values);

    /// \details Function for read input 16-bit registers (3x regs).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  count   Count of registers.
    /// \param[out] values  Pointer to the output buffer (bit array) where the read values are stored.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode readInputRegisters(uint16_t offset, uint16_t count, uint16_t *values);

    /// \details Function for write one separate discrete output (0x coil).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  value   Boolean value to be set.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode writeSingleCoil(uint16_t offset, bool value);

    /// \details Function for write one separate 16-bit holding register (4x).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  value   16-bit unsigned integer value to be set.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode writeSingleRegister(uint16_t offset, uint16_t value);

    /// \details Function to read ExceptionStatus.
    /// \param[out] status  Pointer to the byte (bit array) where the exception status is stored.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode readExceptionStatus(uint8_t *value);

    /// \details Function for write coils (discrete outputs, 1-bit values) (0x data).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  count   Count of coils (bits).
    /// \param[in]  values  Pointer to the input buffer (bit array) which values must be written.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode writeMultipleCoils(uint16_t offset, uint16_t count, const void *values);

    /// \details Function for write holding (output) 16-bit registers (4x regs).
    /// \param[in]  offset  Starting offset (0-based).
    /// \param[in]  count   Count of registers.
    /// \param[in]  values  Pointer to the input buffer which values must be written.
    /// \return The result `Modbus::StatusCode` of the operation.
    Modbus::StatusCode writeMultipleRegisters(uint16_t offset, uint16_t count, const uint16_t *values);

public:
    /// \details Returns the last status of the port object that is used by this client.
    inline Modbus::StatusCode lastPortStatus() const { return m_port->lastStatus(); }

    /// \details Returns the last error status of the port object that is used by this client.
    inline Modbus::StatusCode lastPortErrorStatus() const { return m_port->lastErrorStatus(); }

    /// \details Returns the text description of the last error of the port object that is used by this client.
    inline const Modbus::Char *lastPortErrorText() const { return m_port->lastErrorText(); }

protected:
    ModbusClientPort *m_port;
    uint8_t m_unit;
    uint32_t m_tries;
    uint32_t m_innerTries;

};


#endif // CNMODBUSCLIENT_H