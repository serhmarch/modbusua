#include "CnModbus.h"

#include <iomanip>

#include <CnStd_sstream.h>
#include <CnUtils.h>

#include "ModbusTcpPort.h"
#include "ModbusSerialPort.h"
#include "ModbusClientPort.h"
#include "ModbusServerPort.h"
#include "ModbusServerResource.h"
#include "ModbusTcpServer.h"

namespace Cn {

Strings::Strings() :
    Unit            (CnSTR("ModbusUnit")),
    Type            (CnSTR("Type")),
    Tries           (CnSTR("Tries")),
    RepeatCount     (CnSTR("RepeatCount")),
    Host            (CnSTR("Host")),
    Port            (CnSTR("Port")),
    Timeout         (CnSTR("Timeout")),
    SerialPortName  (CnSTR("SerialPortName")),
    BaudRate        (CnSTR("BaudRate")),
    DataBits        (CnSTR("DataBits")),
    Parity          (CnSTR("Parity")),
    StopBits        (CnSTR("StopBits")),
    FlowControl     (CnSTR("FlowControl")),
    TimeoutFirstByte(CnSTR("TimeoutFirstByte")),
    TimeoutInterByte(CnSTR("TimeoutInterByte")),

    ProtocolTCP     (CnSTR("TCP")),
    ProtocolRTU     (CnSTR("RTU")),
    ProtocolASC     (CnSTR("ASC")),

    NoParity        (CnSTR("No")),
    EvenParity      (CnSTR("Even")),
    OddParity       (CnSTR("Odd")),
    SpaceParity     (CnSTR("Space")),
    MarkParity      (CnSTR("Mark")),

    OneStop         (CnSTR("1")),
    OneAndHalfStop  (CnSTR("1.5")),
    TwoStop         (CnSTR("2")),

    NoFlowControl   (CnSTR("No")),
    HardwareControl (CnSTR("Hard")),
    SoftwareControl (CnSTR("Soft"))
{
}

const Strings &Strings::instance()
{
    static Strings s;
    return s;
}

Defaults::Defaults() :
    Unit            (1),
    Type            (Modbus::TCP),
    Tries           (1), // TODO: initialize by constant from ModbusClientPort
    RepeatCount     (Tries), 
    Host            (toString(ModbusTcpPort::Defaults::instance().host)),
    Port            (ModbusTcpPort   ::Defaults::instance().port            ),
    Timeout         (ModbusTcpPort   ::Defaults::instance().timeout         ),
    SerialPortName  (toString(ModbusSerialPort::Defaults::instance().portName)),
    BaudRate        (ModbusSerialPort::Defaults::instance().baudRate        ),
    DataBits        (ModbusSerialPort::Defaults::instance().dataBits        ),
    Parity          (ModbusSerialPort::Defaults::instance().parity          ),
    StopBits        (ModbusSerialPort::Defaults::instance().stopBits        ),
    FlowControl     (ModbusSerialPort::Defaults::instance().flowControl     ),
    TimeoutFirstByte(ModbusSerialPort::Defaults::instance().timeoutFirstByte),
    TimeoutInterByte(ModbusSerialPort::Defaults::instance().timeoutInterByte)
{
}

const Defaults &Defaults::instance()
{
    static Defaults s;
    return s;
}

#define MB_GET_SETTING_MACRO(type, name, assign)                                    \
    type v;                                                                         \
    bool okInner = false;                                                           \
    Config::const_iterator it = s.find(Strings::instance().name);                   \
    if (it != s.end())                                                              \
    {                                                                               \
        CnVariant var = it->second;                                                 \
        assign;                                                                     \
    }                                                                               \
    if (ok)                                                                         \
        *ok = okInner;                                                              \
    if (okInner)                                                                    \
        return v;                                                                   \
    return Defaults::instance().name;


uint8_t getCfgUnit(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(uint8_t, Unit, v = static_cast<uint8_t>(var.toUInt(&okInner)))
}

Modbus::ProtocolType getCfgType(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(Modbus::ProtocolType, Type, v = toProtocolType(var, &okInner))
}

static uint32_t __getCfgTries(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(uint32_t, Tries, v = static_cast<uint32_t>(var.toUInt(&okInner)))
}

static uint32_t __getCfgRepeatCount(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(uint32_t, RepeatCount, v = static_cast<uint32_t>(var.toUInt(&okInner)))
}

uint32_t getCfgTries(const Config &s, bool *ok)
{
    bool okInner;
    uint32_t v = __getCfgTries(s, &okInner);
    if (okInner)
    {
        if (ok)
            *ok = true;
        return v;
    }
    return __getCfgRepeatCount(s, ok);
}

CnString getCfgHost(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(CnString, Host, v = var.toString(); okInner = true)
}

uint16_t getCfgPort(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(uint16_t, Port, v = static_cast<uint16_t>(var.toUInt(&okInner)))
}

uint32_t getCfgTimeout(const Config &s, bool *ok)
{
   MB_GET_SETTING_MACRO(uint32_t, Timeout, v = static_cast<uint32_t>(var.toUInt(&okInner)))
}

CnString getCfgSerialPortName(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(CnString, SerialPortName, v = var.toString(); okInner = true)
}

int32_t getCfgBaudRate(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(int32_t, BaudRate, v = static_cast<int32_t>(var.toInt(&okInner)))
}

int8_t getCfgDataBits(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(int8_t, DataBits, v = static_cast<int8_t>(var.toInt(&okInner)))
}

Modbus::Parity getCfgParity(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(Modbus::Parity, Parity, v = toParity(var, &okInner))
}

Modbus::StopBits getCfgStopBits(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(Modbus::StopBits, StopBits, v = toStopBits(var, &okInner))
}

Modbus::FlowControl getCfgFlowControl(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(Modbus::FlowControl, FlowControl, v = toFlowControl(var, &okInner))
}

uint32_t getCfgTimeoutFirstByte(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(uint32_t, TimeoutFirstByte, v = static_cast<uint32_t>(var.toUInt(&okInner)))
}

uint32_t getCfgTimeoutInterByte(const Config &s, bool *ok)
{
    MB_GET_SETTING_MACRO(uint32_t, TimeoutInterByte, v = static_cast<uint32_t>(var.toUInt(&okInner)))
}

void setCfgUnit(Config &s, uint8_t v)
{
    s[Strings::instance().Unit] = v;
}

void setCfgType(Config &s, Modbus::ProtocolType v)
{
    s[Strings::instance().Type] = toString(v);
}

void setCfgTries(Config &s, uint32_t v)
{
    s[Strings::instance().Tries] = v;
}

void setCfgHost(Config &s, const CnString &v)
{
    s[Strings::instance().Host] = v;
}

void setCfgPort(Config &s, uint16_t v)
{
    s[Strings::instance().Port] = v;
}

void setCfgTimeout(Config &s, uint32_t v)
{
    s[Strings::instance().Timeout] = v;
}

void setCfgSerialPortName(Config &s, const CnString& v)
{
    s[Strings::instance().SerialPortName] = v;
}

void setCfgBaudRate(Config &s, int32_t v)
{
    s[Strings::instance().BaudRate] = v;
}

void setCfgDataBits(Config &s, int8_t v)
{
    s[Strings::instance().DataBits] = v;
}

void setCfgParity(Config &s, Modbus::Parity v)
{
    s[Strings::instance().Parity] = toString(v);
}

void setCfgStopBits(Config &s, Modbus::StopBits v)
{
    s[Strings::instance().StopBits] = toString(v);
}

void setCfgFlowControl(Config &s, Modbus::FlowControl v)
{
    s[Strings::instance().FlowControl] = toString(v);
}

void setCfgTimeoutFirstByte(Config &s, uint32_t v)
{
    s[Strings::instance().TimeoutFirstByte] = v;
}

void setCfgTimeoutInterByte(Config &s, uint32_t v)
{
    s[Strings::instance().TimeoutInterByte] = v;
}


Modbus::ProtocolType toProtocolType(int v, bool *ok)
{
    Modbus::ProtocolType res = Defaults::instance().Type;
    bool okInner = true; 
    switch (v)
    { 
    case Modbus::TCP:
    case Modbus::RTU:
    case Modbus::ASC:
        res = static_cast<Modbus::ProtocolType>(v);
        break;
    default:
        okInner = false;
        break;
    }
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::ProtocolType toProtocolType(const CnString &v, bool *ok)
{
    const Strings &s = Strings::instance();
    bool okInner = true; 
    Modbus::ProtocolType res = Defaults::instance().Type;
    if (v == s.ProtocolTCP) 
        res = Modbus::TCP;
    else if (v == s.ProtocolRTU) 
        res = Modbus::RTU;
    else if (v == s.ProtocolASC) 
        res = Modbus::ASC;
    else
        okInner = false;
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::ProtocolType toProtocolType(const CnVariant &v, bool *ok)
{
    if (v.type() == CnVariant::vString)
        return toProtocolType(v.toString(), ok);
    return toProtocolType(v.toInt(), ok);
}

int32_t toBaudRate(const CnString &s, bool *ok)
{
    bool okInner;
    int32_t r = static_cast<int32_t>(toInt(s, &okInner));
    if (ok)
        *ok = okInner;
    if (okInner)
        return r;
    return Defaults::instance().BaudRate;
}

int32_t toBaudRate(const CnVariant &v, bool *ok)
{
    bool okInner;
    int32_t r = static_cast<int32_t>(v.toInt(&okInner));
    if (ok)
        *ok = okInner;
    if (okInner)
        return r;
    return Defaults::instance().BaudRate;
}

int8_t toDataBits(const CnString &s, bool *ok)
{
    bool okInner;
    int8_t r = static_cast<int32_t>(toInt(s, &okInner));
    if (!okInner)
    {
        okInner = true;
        if      (s == CnSTR("Data8"))
            r = 8;
        else if (s == CnSTR("Data7"))
            r = 7;
        else if (s == CnSTR("Data6"))
            r = 6;
        else if (s == CnSTR("Data5"))
            r = 5;
        else
        {
            r = Defaults::instance().DataBits;
            okInner = false;
        }
    }
    if (ok)
        *ok = okInner;
    return r;
}

int8_t toDataBits(const CnVariant &v, bool *ok)
{
    bool okInner;
    int8_t r = static_cast<int8_t>(v.toInt(&okInner));
    if (!okInner)
        r = toDataBits(v.toString(), &okInner);
    if (ok)
        *ok = okInner;
    return r;
}

Modbus::Parity toParity(int v, bool *ok)
{
    Modbus::Parity res = Defaults::instance().Parity;
    bool okInner = true; 
    switch (v)
    { 
    case Modbus::NoParity   :
    case Modbus::EvenParity :
    case Modbus::OddParity  :
    case Modbus::SpaceParity:
    case Modbus::MarkParity :
        res = static_cast<Modbus::Parity>(v);
        break;
    default:
        okInner = false;
        break;
    }
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::Parity toParity(const CnString &v, bool *ok)
{
    const Strings &s = Strings::instance();
    Modbus::Parity res = Defaults::instance().Parity;
    bool okInner = true;
    if (v == s.NoParity)
        res = Modbus::NoParity;
    else if (v == s.EvenParity)
        res = Modbus::EvenParity;
    else if (v == s.OddParity)
        res = Modbus::OddParity;
    else if (v == s.SpaceParity)
        res = Modbus::SpaceParity;
    else if (v == s.MarkParity)
        res = Modbus::MarkParity;
    else
        okInner = false;
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::Parity toParity(const CnVariant &v, bool *ok)
{
    if (v.type() == CnVariant::vString)
        return toParity(v.toString(), ok);
    return toParity(v.toInt(), ok);
}

Modbus::StopBits toStopBits(int v, bool *ok)
{
    Modbus::StopBits res = Defaults::instance().StopBits;
    bool okInner = true; 
    switch (v)
    { 
    case Modbus::OneStop:
    case Modbus::OneAndHalfStop:
    case Modbus::TwoStop:
        res = static_cast<Modbus::StopBits>(v);
        break;
    default:
        okInner = false;
        break;
    }
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::StopBits toStopBits(const CnString &v, bool *ok)
{
    const Strings &s = Strings::instance();
    Modbus::StopBits res;
    bool okInner = true; 
    if (v == s.OneStop)
        res = Modbus::OneStop;
    else if (v == s.OneAndHalfStop)
        res = Modbus::OneAndHalfStop;
    else if (v == s.TwoStop)
        res = Modbus::TwoStop;
    else
    {
        okInner = false;
        res = Defaults::instance().StopBits;
    }
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::StopBits toStopBits(const CnVariant &v, bool *ok)
{
    if (v.type() == CnVariant::vString)
        return toStopBits(v.toString(), ok);
    return toStopBits(v.toInt(), ok);
}

Modbus::FlowControl toFlowControl(int v, bool *ok)
{
    Modbus::FlowControl res = Defaults::instance().FlowControl;
    bool okInner = true; 
    switch (v)
    { 
    case Modbus::NoFlowControl:
    case Modbus::HardwareControl:
    case Modbus::SoftwareControl:
        res = static_cast<Modbus::FlowControl>(v);
        break;
    default:
        okInner = false;
        break;
    }
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::FlowControl toFlowControl(const CnString &v, bool *ok)
{
    const Strings &s = Strings::instance();
    Modbus::FlowControl res = Defaults::instance().FlowControl;
    bool okInner = true; 
    if (v == s.NoFlowControl)
        res = Modbus::NoFlowControl;
    else if (v == s.HardwareControl)
        res = Modbus::HardwareControl;
    else if (v == s.SoftwareControl)
        res = Modbus::SoftwareControl;
    else
        okInner = false;
    if (ok)
        *ok = okInner;
    return res;
}

Modbus::FlowControl toFlowControl(const CnVariant &v, bool *ok)
{
    if (v.type() == CnVariant::vString)
        return toFlowControl(v.toString(), ok);
    return toFlowControl(v.toInt(), ok);
}

CnString toString(Modbus::ProtocolType v)
{
    const Strings &s = Strings::instance();
    switch (v)
    {
    case Modbus::TCP: return s.ProtocolTCP;
    case Modbus::RTU: return s.ProtocolRTU;
    case Modbus::ASC: return s.ProtocolASC;
    default:
        return CnString();
    }
}

CnString toString(Modbus::Parity v)
{
    const Strings &s = Strings::instance();
    switch (v)
    {
    case Modbus::NoParity   : return s.NoParity   ;
    case Modbus::EvenParity : return s.EvenParity ;
    case Modbus::OddParity  : return s.OddParity  ;
    case Modbus::SpaceParity: return s.SpaceParity;
    case Modbus::MarkParity : return s.MarkParity ;
    default:
        return CnString();
    }
}

CnString toString(Modbus::StopBits v)
{
    const Strings &s = Strings::instance();
    switch (v)
    {
    case Modbus::OneStop       : return s.OneStop       ;
    case Modbus::OneAndHalfStop: return s.OneAndHalfStop;
    case Modbus::TwoStop       : return s.TwoStop       ;
    default:
        return CnString();
    }
}

CnString toString(Modbus::FlowControl v)
{
    const Strings &s = Strings::instance();
    switch (v)
    {
    case Modbus::NoFlowControl  : return s.NoFlowControl  ;
    case Modbus::HardwareControl: return s.HardwareControl;
    case Modbus::SoftwareControl: return s.SoftwareControl;
    default:
        return CnString();
    }
}

ModbusPort *createPort(const Config &settings, bool blocking)
{
    const Strings &s = Strings::instance();
    const Config::const_iterator it = settings.find(s.Type);
    if (it != settings.end())
    {
        bool ok;
        Modbus::ProtocolType type = toProtocolType(it->second, &ok);
        if (ok)
        {
            switch (type)
            {
            case Modbus::TCP:
            {
                std::string host = Cn::toStdString(getCfgHost(settings));
                Modbus::TcpSettings tc;
                tc.host = host.data();
                tc.port = getCfgPort(settings);
                tc.timeout = getCfgTimeout(settings);
                return Modbus::createPort(type, &tc, blocking);
            }
                break;
            case Modbus::RTU:
            case Modbus::ASC:
            {
                std::string portName = Cn::toStdString(getCfgSerialPortName(settings));
                Modbus::SerialSettings sl;
                sl.portName = portName.data();
                sl.baudRate = getCfgBaudRate(settings);
                sl.dataBits = getCfgDataBits(settings);
                sl.parity   = getCfgParity(settings);
                sl.stopBits = getCfgStopBits(settings);
                sl.flowControl = getCfgFlowControl(settings);
                sl.timeoutFirstByte = getCfgTimeoutFirstByte(settings);
                sl.timeoutInterByte = getCfgTimeoutInterByte(settings);
                return Modbus::createPort(type, &sl, blocking);
            }
                break;
            default:
                return nullptr;
            }
        }
    }
    return nullptr;
}

void fillPort(ModbusPort *port, const Config &cfg)
{
    Modbus::ProtocolType type = port->type();
    switch (type)
    {
    case Modbus::TCP:
    {
        bool ok;
        ModbusTcpPort *p = static_cast<ModbusTcpPort*>(port);
        std::string host = Cn::toStdString(getCfgHost(cfg, &ok)); if (ok) p->setHost(host.data());
        auto tcpport = getCfgPort   (cfg, &ok); if (ok) p->setPort   (tcpport);
        auto timeout = getCfgTimeout(cfg, &ok); if (ok) p->setTimeout(timeout);
    }
        break;
    case Modbus::RTU:
    case Modbus::ASC:
    {
        bool ok;
        ModbusSerialPort *p = static_cast<ModbusSerialPort*>(port);
        auto portName           = Cn::toStdString(getCfgSerialPortName(cfg, &ok)); if (ok) p->setPortName(portName.data());
        auto BaudRate           = getCfgBaudRate        (cfg, &ok); if (ok) p->setBaudRate        (BaudRate        );
        auto DataBits           = getCfgDataBits        (cfg, &ok); if (ok) p->setDataBits        (DataBits        );
        auto Parity             = getCfgParity          (cfg, &ok); if (ok) p->setParity          (Parity          );
        auto StopBits           = getCfgStopBits        (cfg, &ok); if (ok) p->setStopBits        (StopBits        );
        auto FlowControl        = getCfgFlowControl     (cfg, &ok); if (ok) p->setFlowControl     (FlowControl     );
        auto TimeoutFirstByte   = getCfgTimeoutFirstByte(cfg, &ok); if (ok) p->setTimeoutFirstByte(TimeoutFirstByte);
        auto TimeoutInterByte   = getCfgTimeoutInterByte(cfg, &ok); if (ok) p->setTimeoutInterByte(TimeoutInterByte);
    }
        break;
    default:
        return;
    }
}

ModbusClientPort *createClientPort(const Config &settings, bool blocking)
{
    ModbusPort *port = createPort(settings, blocking);
    if (port)
    {
        ModbusClientPort *cp = new ModbusClientPort(port);
        cp->setTries(getCfgTries(settings));
        return cp;
    }
    return nullptr;
}

void fillClientPort(ModbusClientPort *port, const Config &cfg)
{
    bool ok;
    auto Tries = getCfgTries(cfg, &ok); if (ok) port->setTries(Tries);
    fillPort(port->port(), cfg);
}

} // namespace Cn
