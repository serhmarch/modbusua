#include "CnDevice.h"

#include <algorithm>
#include <CnStd_stdio.h>
#include <CnThread.h>

#include "CnDeviceConfig.h"
#include "CnDeviceStat.h"

#include "messages/CnDeviceMessageList.h"
#include "messages/CnDeviceMessage.h"

#include "items/CnDeviceItemList.h"
#include "items/CnDeviceModbusItem.h"

#include <cfg/CnCfgDeviceItem.h>
#include <cfg/CnCfgDevice.h>

CnDevice::CnDevice(CnDeviceItemList           *items      ,
                   CnDevicePokeMessageList    *poke       ,
                   CnDevicePollMessageList    *poll       ,
                   CnDeviceConfig             *cfg        ,
                   CnDeviceStat               *stat       ) : CnObject(),
    m_items           (items),
    m_poke            (poke ),
    m_poll            (poll ),
    m_cfg             (cfg  ),
    m_stat            (stat )
{
    m_logger = new CnLoggerManager();

    if (!m_items           ) m_items            = new CnDeviceItemList            (); 
    if (!m_poke            ) m_poke             = new CnDevicePokeMessageList     (this);
    if (!m_poll            ) m_poll             = new CnDevicePollMessageList     (this);
    if (!m_cfg             ) m_cfg              = new CnDeviceConfig              ();
    if (!m_stat            ) m_stat             = new CnDeviceStat                ();

    m_set.EnableDevice   = false;

    memset(&m_vars.flag, 0, sizeof(m_vars.flag));
    m_vars.isActive = true;
    m_vars.configChangeCounter = 0;
    m_vars.isEnablePort = true;
    setIncluded(true);

    setSettingEnableDevice  (true);

    m_logger->logCtorDtor(CnSTR("CnDevice::CnDevice()"));
}

CnDevice::~CnDevice()
{
    clearDataInner();

    delete m_poke       ;
    delete m_poll       ;
    delete m_items      ; // Note: delete items after deleting all messages

    delete m_cfg        ;
    delete m_stat       ;

    m_logger->logCtorDtor(CnSTR("CnDevice::~CnDevice()"));
    delete m_logger;
}

bool CnDevice::event(CnEvent *event)
{
    switch (event->type())
    {
    case Event::SetConfig:
        this->setConfigInner(static_cast<EventSetConfig*>(event)->config);
        break;
    case Event::SetCfgDevice:
        this->setCfgDeviceInner(static_cast<EventSetCfgDevice*>(event)->cfg);
        break;
    case Event::AddPokeItem:
    {
        EventAddPokeItem *e = static_cast<EventAddPokeItem*>(event);
        this->addPokeItemInner(e->origin, e->value);
    }
        break;
    case Event::AddPollItem:
    {
        EventAddPollItem *e = static_cast<EventAddPollItem*>(event);
        this->addPollItemInner(e->origin);
    }
        break;
    case Event::ClearMessages:
        this->clearMessagesInner();
        break;
    }
    return false;
}

void CnDevice::initialize()
{
}

void CnDevice::finalize()
{
    clearDataInner();
}

Cn::Config CnDevice::config() const
{
    return m_cfg->config();
}

void CnDevice::setConfig(const Cn::Config& config)
{
    if (thread() == CnThread::currentThread())
        setConfigInner(config);
    else
        Cn::postEvent(this, new EventSetConfig(config));
}

void CnDevice::setCfgDevice(const CnCfgDevicePtr &cfg)
{
    if (thread() == CnThread::currentThread())
        setCfgDeviceInner(cfg);
    else
        Cn::postEvent(this, new EventSetCfgDevice(cfg));
}

void CnDevice::CmdStatClear()
{
    m_stat->clear();
}

void CnDevice::setSettingEnableDevice(bool v)
{
    {
        CnCriticalSectionLocker _(&m_set.cs);
        m_set.EnableDevice = v;
    }
    bool isEnable = isEnableDevice();
    m_stat->setStatStateEnableDevice(isEnable);
    if (!isEnable)
        this->setConnectionAlive(false);
}

void CnDevice::setEnablePort(bool v)
{
    bool isEnable = true;
    {
        CnCriticalSectionLocker _(&m_vars.cs);
        if (m_vars.isEnablePort != v)
        {
            m_vars.isEnablePort = v;
            isEnable = isEnableDevice();
            m_stat->setStatStateEnableDevice(isEnable);
        }
    }
    if (!isEnable)
        this->setConnectionAlive(false);
}

void CnDevice::setConnectionAlive(bool connected)
{
    CnCriticalSectionLocker _(&m_vars.cs);
    if (m_vars.flag.ConnectionAlive  != connected)
    {
        if (connected)
        {
            m_logger->logConnection(CnSTR("Connection established"));
        }
        else
        {
            m_logger->logConnection(CnSTR("Connection lost"));
        }
        m_vars.flag.ConnectionAlive = connected;
        m_stat->setStatStateConnectionAlive(connected);
    }
}

void CnDevice::setIncluded(bool v)
{
    CnCriticalSectionLocker _(&m_vars.cs); 
    m_vars.flag.Included = v;
    m_stat->setStatStateIncluded(v);
}

bool CnDevice::isEnableDevice() const
{ 
    return m_cfg->innerEnableDevice() && SettingEnableDevice() && isEnablePort();
} 

void CnDevice::renewStat(int innerState)
{
    m_stat->setStatInnerState(innerState);
}

bool CnDevice::hasPokeMessage()
{
    return m_poke->hasMessage();
}

void CnDevice::addPokeItem(CnDeviceModbusItem *origin, const CnVariant &v)
{
    Cn::postEvent(this, new EventAddPokeItem(origin, v));
}

bool CnDevice::popPokeMessage(CnDeviceMessageBase **m) 
{ 
    return m_poke->popMessage(m);
}

void CnDevice::endPokeMessage(CnDeviceMessageBase *m)
{
    return m_poke->endMessage(m);
}

void CnDevice::addPollItem(CnDeviceModbusItem *item)
{
    Cn::postEvent(this, new EventAddPollItem(item));
}

bool CnDevice::popPollMessage(CnDeviceMessageBase **m)
{
    return m_poll->popMessage(m);
}

void CnDevice::endPollMessage(CnDeviceMessageBase *m)
{
    return m_poll->endMessage(m);
}

void CnDevice::clearMessages()
{
    if (thread() == CnThread::currentThread())
        clearMessagesInner();
    else
        Cn::postEvent(this, new EventClearMessages);
}

void CnDevice::setPokeResult(Modbus::StatusCode status, const CnString & err)
{
    setMessageResult(status);
    m_stat->setStatPokeResult(status, Cn::currentTimestamp(), err);
}

void CnDevice::setPollResult(Modbus::StatusCode status, const CnString & err)
{
    setMessageResult(status);
    m_stat->setStatPollResult(status, Cn::currentTimestamp(), err);
}

void CnDevice::setMessageResult(Modbus::StatusCode status)
{
    bool connected = Modbus::StatusIsGood(status);
    setConnectionAlive(connected);
}

void CnDevice::setCfgDeviceInner(const CnCfgDevicePtr &cfg)
{
    clearMessagesInner();
    setConfigInner(cfg->config());
    // TODO: diff items
    CnList<CnDeviceBaseItem*> toAdd;
    if (m_items->count())
    {
        auto oldhash = m_items->dict();
        CnList<CnDeviceBaseItem*> toDelete;
        CnList<CnCfgDeviceItem*> toAddCfgItems;
        for (auto cfgItem : cfg->items())
        {
            auto it = oldhash.find(cfgItem->name());
            if (it != oldhash.end())
            {
                CnDeviceBaseItem* oldItem = it->second;
                oldhash.erase(it);
                if (oldItem->itemType() == cfgItem->type())
                {
                    // Note: if `compare...` functione returns `false` it means
                    //       item must be recreated and old must be deleted.
                    switch (cfgItem->type())
                    {
                    case CnDeviceBaseItem::Item_Modbus:
                    {
                        CnDeviceModbusItem *old = static_cast<CnDeviceModbusItem*>(oldItem);
                        if (compareModbusItem(cfgItem, old))
                        {
                            renewModbusItem(cfgItem, old);
                            continue;
                        }
                    }
                        break;
                    default:
                        break;
                    }
                }
                m_logger->logTraceDetails(CnSTR("Reload config. Item '%s:%s' can not be renewed, so deleting and creating it from begining"), oldItem->name().data(), oldItem->description().data());
                toDelete.push_back(oldItem);
            }
            else
                m_logger->logTraceDetails(CnSTR("Reload config. Creating new item: '%s:%s'"), cfgItem->name().data(), cfgItem->itemReference().data());
            toAddCfgItems.push_back(cfgItem);
        }
        for (auto d = oldhash.begin(); d != oldhash.end(); ++d)
        {
            CnDeviceBaseItem* oldItem = d->second;
            toDelete.push_back(oldItem);
            m_logger->logTraceDetails(CnSTR("Reload config. Deleting item: '%s:%s'"), oldItem->name().data(), oldItem->description().data());
        }
        m_items->removeItems(toDelete);

        for (auto d = toAddCfgItems.begin(); d != toAddCfgItems.end(); ++d)
        {
            if (CnDeviceBaseItem *item = createItem(*d))
                toAdd.push_back(item);
        }
        m_items->addItems(toAdd);
    }
    else
    {
        for (auto cfgItem : cfg->items())
        {
            if (CnDeviceBaseItem *item = createItem(cfgItem))
                toAdd.push_back(item);
        }
        m_items->addItems(toAdd);
    }
}

bool CnDevice::compareModbusItem(const CnCfgDeviceItem *cfg, const CnDeviceModbusItem *item)
{
    if (item->memoryType() != cfg->memoryType())
        return false;
    Cn::DataType type = item->dataType();
    Cn::DataSuffix suffix = item->dataSuffix();   
    if (suffix != cfg->dataSuffix())
    {
        switch (item->memoryType())
        {
        case Modbus::Memory_3x:
        case Modbus::Memory_4x:
            switch (suffix)
            {
            case Cn::Suffix_Default:
            case Cn::Suffix_UInt16:
                switch (cfg->dataSuffix())
                {
                case Cn::Suffix_Default:
                case Cn::Suffix_UInt16:
                    break;
                default:
                    return false;
                }
                break;
            default:
                return false;
            }
            break;
        default:
            return false;
        }
    }
    else if ((suffix == Cn::Suffix_ByteArray) && (item->count() != cfg->count()))
        return false;
    return (item->offset() == cfg->offset());
}

void CnDevice::renewModbusItem(const CnCfgDeviceItem *cfg, CnDeviceModbusItem *item)
{
    renewBaseItem(cfg, item);
    int period = CN_MODBUS_DEVICE_PERIOD(cfg->period(), this->CfgDefaultPeriod());
    item->setPeriod(period);
}

void CnDevice::renewBaseItem(const CnCfgDeviceItem *cfg, CnDeviceBaseItem *item)
{
    item->setDescription(cfg->itemReference());
}

CnDeviceBaseItem *CnDevice::createItem(const CnCfgDeviceItem *cfg)
{
    if (m_items->hasItem(cfg->name()))
    {
        m_logger->logWarning(CnSTR("Item with name '%s' already exists"), cfg->name().data());
        return nullptr;
    }
    CnDeviceBaseItem *item;
    switch (cfg->type())
    {
    case CnDeviceBaseItem::Item_Modbus:
    {
        int period = CN_MODBUS_DEVICE_PERIOD(cfg->period(), this->CfgDefaultPeriod());
        item = createDeviceModbusItem(cfg->dataSuffix(), cfg->memoryType(), this, cfg->offset(), cfg->count(), period, cfg->messageId());
    }
        break;
    default:
        return nullptr;
    }
    item->setName(cfg->name());
    item->setDescription(cfg->itemReference());
    return item;
}

void CnDevice::setConfigInner(const Cn::Config &config)
{
    m_cfg->setConfig(config);
    m_logger->setName(m_cfg->CfgName());
    bool isEnable = isEnableDevice();
    m_stat->setStatStateEnableDevice(isEnable);
    if (!isEnable)
        this->setConnectionAlive(false);
    m_stat->setStatStateEnableDevice(isEnable);
    m_vars.cs.lock();
    m_vars.configChangeCounter++;
    m_vars.cs.unlock();
    m_logger->logTrace(CnSTR("CONFIG changed"));
}

void CnDevice::addPokeItemInner(CnDeviceModbusItem *origin, const CnVariant &v)
{
    if (!m_items->hasItem(origin))
        return;
    if (!this->isEnableDevice())
        return;
    CnDeviceModbusItem *item = Cn::createDeviceModbusItem(origin->dataSuffix(),
                                                          origin->memoryType(),
                                                          this,
                                                          origin->offset(),
                                                          origin->count(),
                                                          0);
    if (item)
    {
        item->update(v);
        m_logger->logTraceDetails(CnSTR("Write item '%s' (%s) with value '%s'"), origin->name().data(), origin->description().data(), v.toString().data());
        m_poke->addItem(item);
    }
}

void CnDevice::addPollItemInner(CnDeviceModbusItem *item)
{
    if (!m_items->hasItem(item))
        return;
    if (!this->isEnableDevice() || item->isReadDisabled())
        return;
    m_poll->addItem(item);
}

void CnDevice::clearMessagesInner()
{
    m_poke->clear();
    m_poll->clear();
}

void CnDevice::clearDataInner()
{
    clearMessagesInner();
    m_items->clear();
}
