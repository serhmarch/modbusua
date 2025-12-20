#include "CnDeviceRunnable.h"

#include "CnDevice.h"
#include "CnDeviceConfig.h"
#include "messages/CnDeviceMessageBase.h"

#include <ModbusPort.h>
#include <ModbusClientPort.h>
#include <CnModbusClient.h>

using namespace std;

CnDeviceRunnable::CnDeviceRunnable(CnDevice *device, ModbusClientPort *port)
{
    const CnDeviceConfig::Defaults &d = CnDeviceConfig::Defaults::instance();

    m_device = device;
    m_device->IncrementRef();
    m_logger = m_device->logger();

    m_plc = new CnModbusClient(d.ModbusUnit, port);

	m_state = STATE_MAIN;
    m_isEnable = true;
    m_pokeMessage = nullptr;
    m_pollMessage = nullptr;

    m_configChangeCounter = device->configChangeCounter();
    synchConfig(m_device->config());
    m_device->initialize();
    m_logger->logTrace(CnSTR("CnDeviceRunnable::CnDeviceRunnable(): constructor"));
}

CnDeviceRunnable::~CnDeviceRunnable()
{
    m_logger->logTrace(CnSTR("CnDeviceRunnable::~CnDeviceRunnable(): destructor"));
    // TODO: call (maybe) 'setComplete'-method on every message with appropriate error
    //if (m_pokeMessage)
    //    m_pokeMessage->DecrementRef();
    //if (m_pollMessage)
    //    m_pollMessage->DecrementRef();
    //m_device->clearMessages();
    m_device->finalize();
    delete m_plc;
    m_device->DecrementRef();
}

bool CnDeviceRunnable::isEnable() const 
{ 
    return m_device->isIncluded() && ((m_state != STATE_MAIN) || isEnablePrivate());
}

Cn::StatusCode CnDeviceRunnable::run()
{
    Cn::StatusCode r = runPrivate();
    if (!this->isEnablePrivate())// || (Cn::StatusIsBad(r) && !m_device->isConnectionAlive()))
        m_device->clearMessages(); // TODO:
    m_device->renewStat(m_state);
    return r;
}

bool CnDeviceRunnable::hasPokeMessage() const
{
    return m_device->hasPokeMessage();
}

Cn::StatusCode CnDeviceRunnable::runPrivate()
{
    bool fRepeat;
    Cn::StatusCode r = Cn::Status_Good;
    do
    {
        fRepeat = false;
        switch (m_state)
        {
        case STATE_MAIN:
        {
            checkSynchConfig();
            if (this->isEnablePrivate())
            {
                if (m_plc->port()->currentClient() != nullptr)
                    break;
                if (m_device->popPokeMessage(&m_pokeMessage))
                {
                    m_state = STATE_POKE_BEGIN;
                    fRepeat = true;
                    break;
                }
                else if (m_device->popPollMessage(&m_pollMessage))
                {
                    m_state = STATE_POLL_BEGIN;
                    fRepeat = true;
                }
            }
        }
            break;
        case STATE_POLL_BEGIN:
            beginPollMessage();
            m_logger->logTraceDetails(CnSTR("CnDeviceRunnable::STATE_POLL_BEGIN: %s"), m_pollMessage->fineName());
            m_state = STATE_POLL;
            // no need break
        case STATE_POLL:
            r = execPollMessage();
            if (Cn::StatusIsProcessing(r))
                return r;
            if (Cn::StatusIsBad(r))
            {
                m_state = STATE_RESTORE_TIMEOUT_BEGIN;
                fRepeat = true;
            }
            else
                m_state = STATE_MAIN;
            break;
        case STATE_POKE_BEGIN:
            beginPokeMessage();
            m_logger->logTraceDetails(CnSTR("CnDeviceRunnable::STATE_POKE_BEGIN: %s"), m_pokeMessage->fineName());
            m_state = STATE_POKE;
            // no need break
        case STATE_POKE:
            r = execPokeMessage();
            if (Cn::StatusIsProcessing(r))
                return r;
            if (Cn::StatusIsBad(r))
            {
                m_state = STATE_RESTORE_TIMEOUT_BEGIN;
                fRepeat = true;
            }
            else
                m_state = STATE_MAIN;
            break;
        case STATE_RESTORE_TIMEOUT_BEGIN:
            m_timerRestoreBegin = Cn::GetTimer();
            m_state = STATE_RESTORE_TIMEOUT;
            m_device->setIncluded(false);
            // Note: Check 'RestoreTimeout' in next cycle. At least one cycle to restore is needed
            break;
        case STATE_RESTORE_TIMEOUT:
            if ((Cn::GetTimer() - m_timerRestoreBegin) >= static_cast<Cn::Timer_t>(m_config.RestoreTimeout))
            {
                m_device->setIncluded(true);
                m_state = STATE_MAIN;
                fRepeat = true;
                break;
            }
            checkSynchConfig();
            break;
        default:
            m_state = STATE_MAIN;
            fRepeat = true;
            break;
        }
    }
    while (fRepeat);
    return r;
}

bool CnDeviceRunnable::isEnablePrivate() const
{
    return m_device->isEnableDevice() && m_device->isActive();
}

void CnDeviceRunnable::beginPokeMessage()
{
    m_pokeMessage->beginProcess();
    m_timestampMessage = CnDateTime::currentTimestamp();
}

Cn::StatusCode CnDeviceRunnable::execPokeMessage()
{
    // TODO: to speed up execution create struct with 'CnDeviceMessageBase' params to cache it data
    //       and fill it at 'beginPokeMessage'-function and use it there
    Modbus::StatusCode mbres;
    int func = m_pokeMessage->ModbusFunction();
    switch (func)
    {
    case MBF_WRITE_SINGLE_COIL:
        mbres = m_plc->writeSingleCoil(m_pokeMessage->execOffset(), *reinterpret_cast<const bool*>(m_pokeMessage->innerBuffer()));
        break;
    case MBF_WRITE_SINGLE_REGISTER:
        mbres = m_plc->writeSingleRegister(m_pokeMessage->execOffset(), *reinterpret_cast<const uint16_t*>(m_pokeMessage->innerBuffer()));
        break;
    case MBF_WRITE_MULTIPLE_COILS:
        mbres = m_plc->writeMultipleCoils(m_pokeMessage->execOffset(), m_pokeMessage->execCount(), m_pokeMessage->innerBuffer());
        break;
    case MBF_WRITE_MULTIPLE_REGISTERS:
        mbres = m_plc->writeMultipleRegisters(m_pokeMessage->execOffset(), m_pokeMessage->execCount(), reinterpret_cast<const uint16_t*>(m_pokeMessage->innerBuffer()));
        break;
    default:
    {
        CnString err = CnSTR("Error while execPokeMessage: Unsupported Modbus-function type with code ") + Cn::toString(func);
        m_logger->logWarning(CnSTR("CnDeviceRunnable::execPokeMessage(): %s"), err.data());
        m_device->setPokeResult(Modbus::Status_Bad, err);
        endPokeMessage(Modbus::Status_Bad);
        return Cn::Status_Bad;
    }
    }
    if (Modbus::StatusIsProcessing(mbres))
        return Cn::Status_Processing;
    if (Modbus::StatusIsGood(mbres))
    {
        m_device->setPokeResult(mbres);
        endPokeMessage(mbres);
        return Cn::Status_Good;
    }
    else
    {
        CnString err = Cn::toString(m_plc->lastPortErrorText());
        m_device->setPokeResult(mbres, err);
        endPokeMessage(mbres);
        m_logger->logWarning(CnSTR("CnDeviceRunnable::STATE_POKE. status = 0x%08x: %s"), mbres, Cn::toString(m_plc->lastPortErrorText()).data());
        return Cn::Status_Bad;
    }
}

void CnDeviceRunnable::endPokeMessage(Modbus::StatusCode status)
{
    CnTimestamp timestamp = CnDateTime::currentTimestamp();
    CnTimestamp timestampBegin = m_pokeMessage->timestampBegin();
    m_logger->logTrace(CnSTR("ModbusPokeMessage(%s) complete (status=0x%08x): response: %llu ms, bufferized: %s, %llu ms"), m_pokeMessage->fineName(), status,
                        timestamp.milliseconds() - m_timestampMessage.milliseconds(),
                        CnDateTime(timestampBegin).toLocal().toString(CnSTR("%Y.%M.%D %h:%m:%s.%f")).data(),
                        timestamp.milliseconds() - timestampBegin.milliseconds());
    m_pokeMessage->setComplete(status, timestamp);
    m_device->endPokeMessage(m_pokeMessage);
    m_pokeMessage = nullptr;
}

void CnDeviceRunnable::beginPollMessage()
{
    m_pollMessage->beginProcess();
    m_timestampMessage = CnDateTime::currentTimestamp();
}

Cn::StatusCode CnDeviceRunnable::execPollMessage()
{
    // TODO: to speed up execution create struct with 'CnDeviceMessageBase' params to cache it data
    //       and fill it at 'beginPollMessage'-function and use it there
    Modbus::StatusCode mbres;
    int func = m_pollMessage->ModbusFunction();
    switch (func)
    {
    case MBF_READ_COILS:
        mbres = m_plc->readCoils(m_pollMessage->execOffset(), m_pollMessage->execCount(), m_pollMessage->innerBuffer());
        break;
    case MBF_READ_DISCRETE_INPUTS:
        mbres = m_plc->readDiscreteInputs(m_pollMessage->execOffset(), m_pollMessage->execCount(), m_pollMessage->innerBuffer());
        break;
    case MBF_READ_INPUT_REGISTERS:
        mbres = m_plc->readInputRegisters(m_pollMessage->execOffset(), m_pollMessage->execCount(), reinterpret_cast<uint16_t*>(m_pollMessage->innerBuffer()));
        break;
    case MBF_READ_HOLDING_REGISTERS:
        mbres = m_plc->readHoldingRegisters(m_pollMessage->execOffset(), m_pollMessage->execCount(), reinterpret_cast<uint16_t*>(m_pollMessage->innerBuffer()));
        break;
    default:
    {
        CnString err = CnSTR("Error while execPollMessage: Unsupported Modbus-function type with code ") + Cn::toString(func);
        m_logger->logWarning(CnSTR("CnDeviceRunnable::execPollMessage(): %s"), err.data());
        m_device->setPollResult(Modbus::Status_Bad, err);
        endPollMessage(Modbus::Status_Bad);
        return Cn::Status_Bad;
    }
    }
    if (Modbus::StatusIsProcessing(mbres))
        return Cn::Status_Processing;
    if (Modbus::StatusIsGood(mbres))
    {
        m_device->setPollResult(mbres);
        endPollMessage(mbres);
        return Cn::Status_Good;
    }
    else
    {
        CnString err = Cn::toString(m_plc->lastPortErrorText());
        m_device->setPollResult(mbres, err);
        endPollMessage(mbres);
        m_logger->logWarning(CnSTR("CnDeviceRunnable::STATE_POLL. status = 0x%08x: %s"), mbres, Cn::toString(m_plc->lastPortErrorText()).data());
        return Cn::Status_Bad;
    }
}

void CnDeviceRunnable::endPollMessage(Modbus::StatusCode status)
{
    CnTimestamp timestamp = CnDateTime::currentTimestamp();
    CnTimestamp timestampBegin = m_pollMessage->timestampBegin();   
    m_logger->logTrace(CnSTR("ModbusPollMessage(%s) complete (status=0x%08x): response: %llu ms, bufferized: %s, %llu ms"), m_pollMessage->fineName(), status,
                        timestamp.milliseconds() - m_timestampMessage.milliseconds(),
                        CnDateTime(timestampBegin).toLocal().toString(CnSTR("%Y.%M.%D %h:%m:%s.%f")).data(),
                        timestamp.milliseconds() - timestampBegin.milliseconds());
    m_pollMessage->setComplete(status, timestamp);
    m_device->endPollMessage(m_pollMessage);
    m_pollMessage = nullptr;
}

void CnDeviceRunnable::checkSynchConfig()
{
    uint32_t configChangeCounter = m_device->configChangeCounter();
    if (m_configChangeCounter != configChangeCounter)
    {
        synchConfig(m_device->config());
        m_configChangeCounter = configChangeCounter;
    }
}

void CnDeviceRunnable::synchConfig(const Cn::Config &config)
{
    bool ok;
    { auto v = CnDeviceConfig::getInnerName                              (config, &ok); if (ok) m_config.Name                      = v; } 
    { auto v = CnDeviceConfig::getInnerRestoreTimeoutMillisec            (config, &ok); if (ok) m_config.RestoreTimeout            = v; }
    { auto v = CnDeviceConfig::getInnerMaxReadHoldingRegisters           (config, &ok); if (ok) m_config.MaxReadHoldingRegisters   = v; }
    { auto v = CnDeviceConfig::getInnerMaxWriteMultipleRegisters         (config, &ok); if (ok) m_config.MaxWriteMultipleRegisters = v; }
    //m_logger->logTrace(CnSTR("SET CONFIG: INIT_TIMEOUT = %u"), m_config.InitTimeout);
}

