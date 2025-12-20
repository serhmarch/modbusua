/*!
 * \file   CnModbus.h
 * \brief  File for Converter-Modbus conventions.
 *
 * \author serhmarch
 * \date   January 2025
 */
#ifndef CNMODBUS_H
#define CNMODBUS_H

#include <Modbus.h>

#include "CnGlobal.h"

#define CN_MODBUS_PERIOD_DEFAULT_VALUE (-1)
#define CN_MODBUS_PERIOD_DISABLE_VALUE 0

#define CN_MODBUS_DEVICE_PERIOD(period, defaultPeriod) ((period) > CN_MODBUS_PERIOD_DEFAULT_VALUE ? (period) : (defaultPeriod))

namespace Cn {

/*! \brief Sets constant key values for the map of settings.
 */
class Strings
{
public:
    const CnString Unit            ; ///< Config key for the unit number of remote device
    const CnString Type            ; ///< Config key for the type of Modbus protocol
    const CnString Tries           ; ///< Config key for the number of tries a Modbus request is repeated if it fails
    const CnString RepeatCount     ; ///< Config key same as `tries`
    const CnString Host            ; ///< Config key for the IP address or DNS name of the remote device
    const CnString Port            ; ///< Config key for the TCP port number of the remote device
    const CnString Timeout         ; ///< Config key for connection timeout (milliseconds)
    const CnString SerialPortName  ; ///< Config key for the serial port name
    const CnString BaudRate        ; ///< Config key for the serial port's baud rate
    const CnString DataBits        ; ///< Config key for the serial port's data bits
    const CnString Parity          ; ///< Config key for the serial port's parity
    const CnString StopBits        ; ///< Config key for the serial port's stop bits
    const CnString FlowControl     ; ///< Config key for the serial port's flow control
    const CnString TimeoutFirstByte; ///< Config key for the serial port's timeout waiting first byte of packet
    const CnString TimeoutInterByte; ///< Config key for the serial port's timeout waiting next byte of packet

    const CnString ProtocolTCP     ;
    const CnString ProtocolRTU     ;
    const CnString ProtocolASC     ;

    const CnString NoParity        ;
    const CnString EvenParity      ;
    const CnString OddParity       ;
    const CnString SpaceParity     ;
    const CnString MarkParity      ;

    const CnString OneStop         ;
    const CnString OneAndHalfStop  ;
    const CnString TwoStop         ;

    const CnString NoFlowControl   ;
    const CnString HardwareControl ;
    const CnString SoftwareControl ;

    /// \details Constructor ot the class.
    Strings();

    /// \details Returns a reference to the global `Modbus::Strings` object.
    static const Strings &instance();
};

/*! \brief Holds the default values of the settings.
*/
class Defaults
{
public:
    const uint8_t               Unit            ; ///< Default value for the unit number of remote device
    const Modbus::ProtocolType  Type            ; ///< Default value for the type of Modbus protocol
    const uint32_t              Tries           ; ///< Default value for number of tries a Modbus request is repeated if it fails
    const uint32_t              RepeatCount     ; ///< Default value same as `tries` (for backward compatibility)
    const CnString              Host            ; ///< Default value for the IP address or DNS name of the remote device
    const uint16_t              Port            ; ///< Default value for the TCP port number of the remote device
    const uint32_t              Timeout         ; ///< Default value for connection timeout (milliseconds)
    const CnString              SerialPortName  ; ///< Default value for the serial port name
    const int32_t               BaudRate        ; ///< Default value for the serial port's baud rate
    const int8_t                DataBits        ; ///< Default value for the serial port's data bits
    const Modbus::Parity        Parity          ; ///< Default value for the serial port's parity
    const Modbus::StopBits      StopBits        ; ///< Default value for the serial port's stop bits
    const Modbus::FlowControl   FlowControl     ; ///< Default value for the serial port's flow control
    const uint32_t              TimeoutFirstByte; ///< Default value for the serial port's timeout waiting first byte of packet
    const uint32_t              TimeoutInterByte; ///< Default value for the serial port's timeout waiting next byte of packet

    /// \details Constructor ot the class.
    Defaults();

    /// \details Returns a reference to the global `Modbus::Defaults` object.
    static const Defaults &instance();
};

/// \details Get settings value for the unit number of remote device.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
uint8_t getCfgUnit(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the type of Modbus protocol.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
Modbus::ProtocolType getCfgType(const Config &s, bool *ok = nullptr);

/// \details Get settings value for number of tries a Modbus request is repeated if it fails.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
uint32_t getCfgTries(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the IP address or DNS name of the remote device.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
CnString getCfgHost(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the TCP port of the remote device.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
uint16_t getCfgPort(const Config &s, bool *ok = nullptr);

/// \details Get settings value for connection timeout (milliseconds).
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
uint32_t getCfgTimeout(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port name.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
CnString getCfgSerialPortName(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's baud rate.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
int32_t getCfgBaudRate(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's data bits.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
int8_t getCfgDataBits(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's parity.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
Modbus::Parity getCfgParity(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's stop bits.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
Modbus::StopBits getCfgStopBits(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's flow control.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
Modbus::FlowControl getCfgFlowControl(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's timeout waiting first byte of packet.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
uint32_t getCfgTimeoutFirstByte(const Config &s, bool *ok = nullptr);

/// \details Get settings value for the serial port's timeout waiting next byte of packet.
/// If value can't be retrieved that default value is returned and *ok = false (if provided).
uint32_t getCfgTimeoutInterByte(const Config &s, bool *ok = nullptr);

/// \details Set settings value for the unit number of remote device.
void setCfgUnit(Config &s, uint8_t v);

/// \details Set settings value the type of Modbus protocol.
void setCfgType(Config &s, Modbus::ProtocolType v);

/// \details Set settings value for number of tries a Modbus request is repeated if it fails.
void setCfgTries(Config &s, uint32_t);

/// \details Set settings value for the IP address or DNS name of the remote device.
void setCfgHost(Config &s, const CnString &v);

/// \details Set settings value for the TCP port number of the remote device.
void setCfgPort(Config &s, uint16_t v);

/// \details Set settings value for connection timeout (milliseconds).
void setCfgTimeout(Config &s, uint32_t v);

/// \details Set settings value for the serial port name.
void setCfgSerialPortName(Config &s, const CnString&v);

/// \details Set settings value for the serial port's baud rate.
void setCfgBaudRate(Config &s, int32_t v);

/// \details Set settings value for the serial port's data bits.
void setCfgDataBits(Config &s, int8_t v);

/// \details Set settings value for the serial port's parity.
void setCfgParity(Config &s, Modbus::Parity v);

/// \details Set settings value for the serial port's stop bits.
void setCfgStopBits(Config &s, Modbus::StopBits v);

/// \details Set settings value for the serial port's flow control.
void setCfgFlowControl(Config &s, Modbus::FlowControl v);

/// \details Set settings value for the serial port's timeout waiting first byte of packet.
void setCfgTimeoutFirstByte(Config &s, uint32_t v);

/// \details Set settings value for the serial port's timeout waiting next byte of packet.
void setCfgTimeoutInterByte(Config &s, uint32_t v);

/*! \brief Class for convinient manipulation with Modbus Data Address.
*/
using Address = Modbus::Address;

/// \details Convert CnString repr to Modbus::Address
inline Address addressFromString(const CnString &s) { return Address(toInt(s)); }

/// \details Converts int representation to `ProtocolType` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::ProtocolType toProtocolType(int v, bool *ok = nullptr);

/// \details Converts string representation to `ProtocolType` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::ProtocolType toProtocolType(const CnString &s, bool *ok = nullptr);

/// \details Converts CnVariant value to `ProtocolType` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::ProtocolType toProtocolType(const CnVariant &v, bool *ok = nullptr);

/// \details Converts string representation to `BaudRate` value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
int32_t toBaudRate(const CnString &s, bool *ok = nullptr);

/// \details Converts CnVariant value to `DataBits` value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
int32_t toBaudRate(const CnVariant &v, bool *ok = nullptr);

/// \details Converts string representation to `DataBits` value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
int8_t toDataBits(const CnString &s, bool *ok = nullptr);

/// \details Converts CnVariant value to `DataBits` value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
int8_t toDataBits(const CnVariant &v, bool *ok = nullptr);

/// \details Converts int representation to `Parity` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::Parity toParity(int v, bool *ok = nullptr);

/// \details Converts string representation to `Parity` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::Parity toParity(const CnString &s, bool *ok = nullptr);

/// \details Converts CnVariant value to `Parity` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::Parity toParity(const CnVariant &v, bool *ok = nullptr);

/// \details Converts int representation to `StopBits` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::StopBits toStopBits(int v, bool *ok = nullptr);

/// \details Converts string representation to `StopBits` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::StopBits toStopBits(const CnString &s, bool *ok = nullptr);

/// \details Converts CnVariant value to `StopBits` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::StopBits toStopBits(const CnVariant &v, bool *ok = nullptr);

/// \details Converts int representation to `FlowControl` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::FlowControl toFlowControl(int v, bool *ok = nullptr);

/// \details Converts string representation to `FlowControl` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::FlowControl toFlowControl(const CnString &s, bool *ok = nullptr);

/// \details Converts CnVariant value to `FlowControl` enum value.
/// If ok is not nullptr, failure is reported by setting *ok to false, and success by setting *ok to true.
Modbus::FlowControl toFlowControl(const CnVariant &v, bool *ok = nullptr);

/// \details Returns string representation of `ProtocolType` enum value
CnString toString(Modbus::ProtocolType v);

/// \details Returns string representation of `Parity` enum value
CnString toString(Modbus::Parity v);

/// \details Returns string representation of `StopBits` enum value
CnString toString(Modbus::StopBits v);

/// \details Returns string representation of `FlowControl` enum value
CnString toString(Modbus::FlowControl v);

/// \details Same as `Modbus::createPort(ProtocolType type, const void *settings, bool blocking)`
/// but `ProtocolType type` and `const void *settings` are defined by `Cn::Config` key-value map.
ModbusPort *createPort(const Config &settings, bool blocking = false);

/// \details
void fillPort(ModbusPort *port, const Config &settings);

/// \details Same as `Modbus::createClientPort(ProtocolType type, const void *settings, bool blocking)`
/// but `ProtocolType type` and `const void *settings` are defined by `Cn::Config` key-value map.
ModbusClientPort *createClientPort(const Config &settings, bool blocking = false);

/// \details
void fillClientPort(ModbusClientPort *port, const Config &settings);

} // namespace Modbus

#endif // CNMODBUS_H
