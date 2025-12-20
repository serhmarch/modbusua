#include "CnPortRunnable.h"

#include <algorithm>

#include <ModbusTcpPort.h>
#include <CnModbusClient.h>

#include "CnPort.h"
#include <device/CnDevice.h>
#include <device/CnDeviceRunnable.h>

CnPortRunnable::CnPortRunnable(CnPort *port)
{
    m_logger = port->logger();
    m_clientPort = nullptr;
    
    m_port = port;
    m_port->IncrementRef();
    m_logger->logCtorDtor(CnSTR("CnPortRunnable::CnPortRunnable()"));

    for (CnDevice* dev : m_port->devices())
        addDevice(dev);
    m_port->connect(&CnPort::signalAddDevice   , this, &CnPortRunnable::addDevice   );
    m_port->connect(&CnPort::signalRemoveDevice, this, &CnPortRunnable::removeDevice);

    m_tmLastRequest = 0;
    m_configChangeCounter = m_port->configChangeCounter();
    synchConfig(m_port->config());
    m_port->initialize();
}

CnPortRunnable::~CnPortRunnable()
{
    m_port->finalize();
    m_logger->logCtorDtor(CnSTR("CnPortRunnable::~CnPortRunnable(): destructor begin"));
    for (Sequence_t::const_iterator it = m_sequence.begin(); it != m_sequence.end(); ++it)
        delete (*it);
    m_clientPort->close();
    delete m_clientPort;
    m_port->disconnect(this);
    m_logger->logCtorDtor(CnSTR("CnPortRunnable::~CnPortRunnable()"));
    m_port->DecrementRef();
}

void CnPortRunnable::run()
{
    CnTimestamp tmbegin = CnDateTime::currentTimestampPrecise();
    checkSynchConfig();
    processToDelete();
    uint32_t enableDevicesCount = 0;
    bool portEnabled = m_port->isEnable();
    CnDeviceRunnable *dPoke = nullptr;
    for (auto it = m_sequence.begin(); it != m_sequence.end(); it++)
    {
        CnDeviceRunnable *d = *it;
        d->device()->setEnablePort(portEnabled);
        if (portEnabled)
        {
            // Note: (m_clientPort->currentClient() == nullptr) means
            // ModbusClientPort is free and will be captured by next `CnDeviceRunnable::run()` call.
            if ((m_clientPort->currentClient() == nullptr) && m_pokeQueue.size())
            {
                dPoke = m_pokeQueue.front();
                m_pokeQueue.pop_front();
                dPoke->run(); 
            }
            if ((dPoke == nullptr) || (dPoke != d))
                d->run();
            enableDevicesCount += d->isEnable();
            if (d->hasPokeMessage())
            {
                if (std::find(m_pokeQueue.begin(), m_pokeQueue.end(), d) == m_pokeQueue.end())
                    m_pokeQueue.push_back(d);
            }
        }
    }
    if (enableDevicesCount == 0)
        m_clientPort->close();
    //------------------------------------------
    // Statistics
    Modbus::Timestamp tm = m_clientPort->lastStatusTimestamp();
    if (m_tmLastRequest != tm)
    {
        Modbus::StatusCode s = m_clientPort->lastStatus();
        if (Modbus::StatusIsBad(s))
            m_port->setStatStatus(s, CnTimestamp::fromUtcMSecSinceEpoch(tm), Cn::toString(m_clientPort->lastErrorText()));
        else
            m_port->setStatStatus(s, CnTimestamp::fromUtcMSecSinceEpoch(tm));
        m_tmLastRequest = tm;
    }
    CnTimestamp tmend = CnDateTime::currentTimestampPrecise();
    uint32_t ctm = static_cast<uint32_t>(tmend.microseconds() - tmbegin.microseconds());
    m_port->setStatCycleTime(ctm);
}

void CnPortRunnable::processToDelete()
{
    for (auto it = m_devicesToDelete.begin(); it != m_devicesToDelete.end(); )
    {
        CnDeviceRunnable *c = *it;
        if (c->modbusClient() != m_clientPort->currentClient())
        {
            it = m_devicesToDelete.erase(it);
            m_sequence.remove(c);
            m_pokeQueue.remove(c);
            delete c;
        }
        else
            ++it;
    }
}

void CnPortRunnable::addDevice(CnDevice* device)
{
    m_logger->logTrace(CnSTR("CnPortRunnable::CnPortCommand::AddDevice"));
    CnDeviceRunnable* c = new CnDeviceRunnable(device, m_clientPort);
    m_sequence.push_back(c);
}

void CnPortRunnable::removeDevice(CnDevice* device)
{
    m_logger->logTrace(CnSTR("CnPortRunnable::CnPortCommand::RemoveDevice"));
    Sequence_t::const_iterator it = findRunnableIterator(device);
    if (it != m_sequence.end())
    {
        CnDeviceRunnable* c = *it;
        if (c->modbusClient() != m_clientPort->currentClient())
        {
            m_sequence.erase(it);
            m_pokeQueue.remove(c);
            delete c;
        }
        else
        {
            // Note: Need to wait while device is processing and then to delete it
            m_devicesToDelete.insert(c);
        }
    }
}

CnPortRunnable::Sequence_t::const_iterator CnPortRunnable::findRunnableIterator(CnDevice* device) const
{
    for (Sequence_t::const_iterator i = m_sequence.begin(); i != m_sequence.end(); ++i)
    {
        if ((*i)->device() == device)
            return i;
    }
    return m_sequence.end();
}

void CnPortRunnable::checkSynchConfig()
{
    uint32_t configChangeCounter = m_port->configChangeCounter();
    if (m_configChangeCounter != configChangeCounter)
    {
        synchConfig(m_port->config());
        m_configChangeCounter = configChangeCounter;
    }
}

void CnPortRunnable::synchConfig(const Cn::Config &cfg)
{
    if (m_clientPort)
    {
        Cn::fillClientPort(m_clientPort, cfg);
        bool ok;
        Modbus::ProtocolType type = Cn::getCfgType(cfg, &ok);
        if (ok && (type != m_clientPort->type()))
        {
            ModbusPort *port = Cn::createPort(cfg);
            m_clientPort->setPort(port);
        }
    }
    else
    {
        m_clientPort = Cn::createClientPort(cfg);
        m_clientPort->connect(&ModbusClientPort::signalTx, this, &CnPortRunnable::slotTx);
        m_clientPort->connect(&ModbusClientPort::signalRx, this, &CnPortRunnable::slotRx);
    }
}

void CnPortRunnable::slotTx(const Modbus::Char *, const uint8_t* buff, uint16_t size)
{
    m_logger->logSend(CnSTR("Tx: %s"), Cn::bytesToString(buff, size).data());
}

void CnPortRunnable::slotRx(const Modbus::Char *, const uint8_t* buff, uint16_t size)
{
    m_logger->logReceive(CnSTR("Rx: %s"), Cn::bytesToString(buff, size).data());
}