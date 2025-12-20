/*!
 * \file   CnDeviceRunnable.h
 * \brief  The `CnDeviceRunnable` class implements the algorithm for a single Modbus device/PLC
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNDEVICERUNNABLE_H
#define CNDEVICERUNNABLE_H

#include <CnGlobal.h>

class ModbusClientPort;
class CnModbusClient;

class CnLoggerManager;

class CnDevice;
class CnDeviceMessageBase;

/*! \brief The `CnDeviceRunnable` class implements the algorithm for a single Modbus device/PLC.

*/

class CnDeviceRunnable
{
public:
	enum State
	{
        STATE_MAIN                              ,
        STATE_POKE_BEGIN                        ,
        STATE_POKE                              ,
        STATE_POLL_BEGIN                        ,
        STATE_POLL                              ,
        STATE_RESTORE_TIMEOUT_BEGIN             ,
        STATE_RESTORE_TIMEOUT
    };

public:
    /// \details Constructor creates an object implementing the algorithm for a single Modbus device/PLC.
    /// \param[in]  device    Pointer to the structure storing all runtime parameters of the device/PLC
    /// \param[in]  port      Pointer to the Modbus interface for the port
    CnDeviceRunnable(CnDevice *device, ModbusClientPort *port);

    /// \details Class destructor.
    ~CnDeviceRunnable();

public:
    /// \details Returns a pointer to the structure that stores all runtime parameters of the device/PLC.
    inline CnDevice *device() const { return m_device; }

    /// \details Returns a pointer to the Modbus interface for the port.
    inline CnModbusClient *modbusClient() const { return m_plc; }

public:
    /// \details Returns `true` if this device/PLC is available for the execution sequence, `false` otherwise.
    bool isEnable() const;

    /// \details Returns the internal state of the communication algorithm.
    inline State state() const { return m_state; }

    /// \details Returns the internal state of the communication algorithm (same as `state()`).
    inline int innerState() const { return m_state; }

    /// \details Implements the algorithm for a single Modbus device/PLC.
    Cn::StatusCode run();

public:
    bool hasPokeMessage() const;

private:
    Cn::StatusCode runPrivate();
    bool isEnablePrivate() const;

private: // Config
    void checkSynchConfig();
    void synchConfig(const Cn::Config &config);

private: // Poke Message
    inline bool isPokeMessageFinished() const { return m_pokeMessage == nullptr; }
    void beginPokeMessage();
    Cn::StatusCode execPokeMessage();
    void endPokeMessage(Modbus::StatusCode status);

private: // Poll Message
    inline bool isPollMessageFinished() const { return m_pollMessage == nullptr; }
    void beginPollMessage();
    Cn::StatusCode execPollMessage();
    void endPollMessage(Modbus::StatusCode status);

private: // 'Settings' struct
    struct
    {
        CnString                    Name                     ;
        uint32_t                    RestoreTimeout           ;
        uint16_t                    MaxReadHoldingRegisters  ;
        uint16_t                    MaxWriteMultipleRegisters;
    } m_config;

private: // Logger
    CnLoggerManager *m_logger;

private:
    CnModbusClient *m_plc;

	CnDevice *m_device;

    State m_state;
    uint32_t m_configChangeCounter;
    Cn::Timer_t m_timerRestoreBegin;
    uint16_t m_buff[MB_MAX_REGISTERS];

private: //
    bool m_isEnable;
    bool m_isNeedExecPollMessage;

private: // Messages
    CnTimestamp m_timestampMessage;
    CnDeviceMessageBase *m_pokeMessage;
    CnDeviceMessageBase *m_pollMessage;
};

#endif // CNDEVICERUNNABLE_H