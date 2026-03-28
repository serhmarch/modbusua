#include "CnPort.h"

#include "CnPortStat.h"

#include <CnThread.h>
#include <device/CnDevice.h>

CnPort::CnPort()
{
    m_logger = new CnLoggerManager();

    m_cfg = new CnPortConfig();
    m_stat = new CnPortStat();

    m_vars.configChangeCounter = 0;
    m_logger->logCtorDtor(CnSTR("CnPort::CnPort()"));
}

CnPort::~CnPort()
{
    delete m_cfg ;
    delete m_stat;    
    m_logger->logCtorDtor(CnSTR("CnPort::~CnPort()"));
    delete m_logger;
}

void CnPort::signalAddDevice(CnDevice *dev)
{
    emitSignal(__func__, &CnPort::signalAddDevice, dev);
}

void CnPort::signalRemoveDevice(CnDevice *dev)
{
    emitSignal(__func__, &CnPort::signalRemoveDevice, dev);
}

bool CnPort::event(CnEvent *event)
{
    switch (event->type())
    {
    case Event::SetConfig:
        this->setConfigInner(static_cast<EventSetConfig*>(event)->config);
        break;
    case Event::SetCfgPort:
        this->setCfgPortInner(static_cast<EventSetCfgPort*>(event)->cfg);
        break;
    case Event::AddDevice:
        this->addDeviceInner(static_cast<EventAddDevice*>(event)->device);
        break;
    case Event::RemoveDevice:
        this->removeDeviceInner(static_cast<EventRemoveDevice*>(event)->device);
        break;
    }
    return false;
}

void CnPort::initialize()
{
    //setThreadId(Cn::currentThreadId());
}

void CnPort::finalize()
{
    // Note: Base implementation do nothing
}

Cn::Config CnPort::config() const
{
    return m_cfg->config();
}

void CnPort::setConfig(const Cn::Config& config)
{
    if (thread() == CnThread::currentThread())
        setConfigInner(config);
    else
        Cn::postEvent(this, new EventSetConfig(config));
}

void CnPort::setCfgPort(const CnCfgPortPtr &cfg)
{
    if (thread() == CnThread::currentThread())
        setCfgPortInner(cfg);
    else
        Cn::postEvent(this, new EventSetCfgPort(cfg));
}

void CnPort::setSettingEnable(bool v)
{
    m_set.cs.lock();
    m_set.Enable = v;
    m_set.cs.unlock();
    bool enable = this->isEnable();
    m_stat->setStatStateEnablePort(enable);
}

void CnPort::CmdStatClear()
{
    m_stat->clear();
}

void CnPort::setStatCycleTime(uint32_t time)
{
    m_stat->setStatCycleTime(time);
}

void CnPort::setStatStatus(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err)
{
    m_stat->setStatStatus(status, timestamp, err);
}

CnList<CnDevice *> CnPort::devices() const
{
    CnCriticalSectionLocker _(&m_vars.cs);
    return m_vars.devices;
}

void CnPort::addDevice(CnDevice *device)
{
    if (thread() == CnThread::currentThread())
        addDeviceInner(device);
    else
        Cn::postEvent(this, new EventAddDevice(device));
}

void CnPort::removeDevice(CnDevice* device)
{
    if (thread() == CnThread::currentThread())
        removeDeviceInner(device);
    else
        Cn::postEvent(this, new EventRemoveDevice(device));
}

void CnPort::setConfigInner(const Cn::Config &config)
{ 
    m_cfg->setConfig(config);
    bool enable = this->isEnable();
    m_stat->setStatStateEnablePort(enable);

    m_logger->setName(m_cfg->CfgName());

    m_vars.cs.lock();
    m_vars.configChangeCounter++;
    m_vars.cs.unlock();

    m_logger->logTrace(CnSTR("CONFIG changed"));
}

void CnPort::setCfgPortInner(const CnCfgPortPtr& cfg)
{
    this->setConfigInner(cfg->config());
}

void CnPort::addDeviceInner(CnDevice *device)
{
    device->IncrementRef();
    device->setParentPort(this);
    device->moveToThread(this->thread());
    m_vars.cs.lock();
    m_vars.devices.push_back(device);
    m_vars.cs.unlock();
    signalAddDevice(device);
}

void CnPort::removeDeviceInner(CnDevice* device)
{
    signalAddDevice(device);
    m_vars.cs.lock();
    m_vars.devices.remove(device);
    m_vars.cs.unlock();
    device->DecrementRef();
}

