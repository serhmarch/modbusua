/*!
 * \file   CnDevice.h
 * \brief  Describes the structure that stores all parameters of the device/PLC for the program execution stage
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNDEVICE_H
#define CNDEVICE_H

#include <CnGlobal.h>
#include <CnObject.h>
#include <log/CnLoggerManager.h>
#include <cfg/CnCfgDevice.h>

#include "CnDeviceConfig.h"
#include "CnDeviceStat.h"

class CnPort;

class CnDeviceItemList;
class CnDevicePokeMessageList;
class CnDevicePollMessageList;
class CnDeviceConfig;
class CnDeviceStat;

class CnDeviceBaseItem;
class CnDeviceModbusItem;
class CnDeviceMessageBase;

class CnDevice : public CnObject
{
public:
    class Event : public CnEvent
    {
    public:
        enum Type
        {
            SetConfig    ,
            SetCfgDevice ,
            AddPokeItem  ,
            AddPollItem  ,
            ClearMessages
        };
    };

    class EventSetConfig : public Event 
    {
    public: 
        EventSetConfig(const Cn::Config &v) : config(v) {  }
        int type() const override { return SetConfig; }
        Cn::Config config;
    };

    class EventSetCfgDevice : public Event 
    {
    public: 
        EventSetCfgDevice(const CnCfgDevicePtr &v) : cfg(v) {  }
        int type() const override { return SetCfgDevice; }
        CnCfgDevicePtr cfg;
    };

    class EventAddPokeItem : public Event 
    {
    public: 
        EventAddPokeItem(CnDeviceModbusItem *item, const CnVariant &v) : origin(item), value(v) {  }
        int type() const override { return AddPokeItem; }
        CnDeviceModbusItem *origin;
        CnVariant value;
    };

    class EventAddPollItem : public Event
    {
    public: 
        EventAddPollItem(CnDeviceModbusItem *item) : origin(item){}
        int type() const override { return AddPollItem; } 
        CnDeviceModbusItem *origin;
    };

    class EventClearMessages: public Event { public: int type() const override { return ClearMessages; } };
    
public:
    /// \details Class constructor.
    explicit CnDevice(CnDeviceItemList           *items       = nullptr,
                      CnDevicePokeMessageList    *poke        = nullptr,
                      CnDevicePollMessageList    *poll        = nullptr,
                      CnDeviceConfig             *cfg         = nullptr,
                      CnDeviceStat               *stat        = nullptr);

    /// \details Class destructor.
    ~CnDevice();

public:
    bool event(CnEvent *event) override;

public:
    /// \details Virtual function called before object processing begins.
    virtual void initialize();

    /// \details Virtual function called before object deletion begins.
    virtual void finalize();

public:
    inline CnString CfgName                     () const { return m_cfg->CfgName                     (); }
    inline uint16_t CfgMaxReadCoils             () const { return m_cfg->CfgMaxReadCoils             (); }
    inline uint16_t CfgMaxWriteMultipleCoils    () const { return m_cfg->CfgMaxWriteMultipleCoils    (); }
    inline uint16_t CfgMaxReadDiscreteInputs    () const { return m_cfg->CfgMaxReadDiscreteInputs    (); }
    inline uint16_t CfgMaxReadInputRegisters    () const { return m_cfg->CfgMaxReadInputRegisters    (); }
    inline uint16_t CfgMaxReadHoldingRegisters  () const { return m_cfg->CfgMaxReadHoldingRegisters  (); }
    inline uint16_t CfgMaxWriteMultipleRegisters() const { return m_cfg->CfgMaxWriteMultipleRegisters(); }
    inline uint32_t CfgDefaultPeriod            () const { return m_cfg->CfgDefaultPeriod            (); }
    inline uint32_t CfgRequestTimeout           () const { return m_cfg->CfgRequestTimeout           (); }

public:
    /// \details
    inline CnString name() const { return m_cfg->CfgName(); }

    /// \details Returns a pointer to the `CnPort` object to which this device/PLC belongs.
    inline CnPort *parentPort() const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.port; }

    /// \details Sets the pointer to the `CnPort` object to which this device/PLC belongs.
    inline void setParentPort(CnPort *port) { CnCriticalSectionLocker _(&m_vars.cs); m_vars.port = port; }

    /// \details Returns `true` if the device/PLC is activated (there are 'Item Reference' objects related to it), `false` otherwise.
    inline bool isActive() const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.isActive; }

    /// \details Activates/deactivates the device/PLC depending on the 'active' parameter, i.e., includes or excludes it from the processing cycle.
    inline void setActive(bool active = true) { CnCriticalSectionLocker _(&m_vars.cs); m_vars.isActive = active; }

public:
    CnDeviceConfig *cfg () const { return m_cfg ; }
    CnDeviceStat   *stat() const { return m_stat; }    

public:
    /// \details Returns the configuration change counter value.
    inline uint32_t configChangeCounter() const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.configChangeCounter; }

    /// \details Returns the associative configuration array.
    Cn::Config config() const;

    /// \details Sets the configuration.
    void setConfig(const Cn::Config& config);

    /// \details Sets the configuration.
    void setCfgDevice(const CnCfgDevicePtr &cfg);

public: // Commands
    void CmdStatClear           (); ///< \details Command. Reset statistical information

public: // Settings
    inline bool SettingEnableDevice   () const { CnCriticalSectionLocker _(&m_set.cs); return m_set.EnableDevice  ; } ///< \details Setting. Availability of this device/PLC for the execution cycle (1-on, 0-off)

    void setSettingEnableDevice   (bool v); ///< \details Set setting. Availability of this device/PLC for the execution cycle (1-on, 0-off)

public: // Data

    inline bool isEnablePort() const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.isEnablePort; }
    void setEnablePort(bool v);

    inline bool isConnectionAlive           () const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.flag.ConnectionAlive ; } ///< \details Flag. There is a connection with the PLC (corresponds to bit 2 Stat.State)
    inline bool isIncluded                  () const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.flag.Included        ; } ///< \details Flag. This PLC is included in the execution sequence. It can be excluded if a communication error occurs and the restore timeout has not yet expired (corresponds to bit 10 Stat.State)

    void setConnectionAlive (bool v); ///< \details Set flag. There is a connection with the PLC (corresponds to bit 2 Stat.State)
    void setIncluded        (bool v); ///< \details Set flag. This PLC is included in the execution process. It can be excluded if a communication error occurs and the restore timeout has not yet expired (corresponds to bit 10 Stat.State)

    /// \details
    inline bool isDataAvailable() const {return isConnectionAlive() && isEnableDevice(); }

    ///< \details Flag. Communication is allowed (corresponds to bit 0 Stat.State)
    bool isEnableDevice() const;

    /// \details
    void renewStat(int innerState);
 
public: // Poke Message interface
    /// \details Returns `true` if there is a message in the queue, `false` otherwise.
    bool hasPokeMessage();

    /// \details
    void addPokeItem(CnDeviceModbusItem *origin, const CnVariant &v);

    /// \details Removes a Modbus write message from the queue and sets its pointer in `m`.
    /// \return Returns `true` if there is a message in the queue, `false` otherwise.
    bool popPokeMessage(CnDeviceMessageBase **m);

    /// \details
    void endPokeMessage(CnDeviceMessageBase *m);

public: // Poll Message interface
    /// \details
    void addPollItem(CnDeviceModbusItem *item);

    /// \details Removes a Modbus read message from the queue and sets its pointer in `m`.
    /// \return Returns `true` if there is a message in the queue, `false` otherwise.
    bool popPollMessage(CnDeviceMessageBase **m);

    /// \details
    void endPollMessage(CnDeviceMessageBase *m);

public:
    /// \details
    void clearMessages();

public:
    /// \details Sets the static data for the last Modbus direct data write message.
    /// \param[in]  status  Result status
    /// \param[in]  err     Error text (if any)
    void setPokeResult(Modbus::StatusCode status, const CnString& err = CnString());

    /// \details Sets the static data for the last Modbus direct data read message.
    /// \param[in]  status  Result status
    /// \param[in]  err     Error text (if any)
    void setPollResult(Modbus::StatusCode status, const CnString& err = CnString());

protected:
    void setMessageResult(Modbus::StatusCode status);

protected:
    void setCfgDeviceInner(const CnCfgDevicePtr &cfg);
    bool compareModbusItem(const CnCfgDeviceItem *cfg, const CnDeviceModbusItem *item);
    virtual void renewModbusItem(const CnCfgDeviceItem *cfg, CnDeviceModbusItem *item);
    virtual void renewBaseItem(const CnCfgDeviceItem *cfg, CnDeviceBaseItem *item);
    virtual CnDeviceBaseItem* createItem(const CnCfgDeviceItem *cfg);

protected:
    void setConfigInner(const Cn::Config& config);
    void addPokeItemInner(CnDeviceModbusItem *origin, const CnVariant &v);
    void addPollItemInner(CnDeviceModbusItem *origin);
    void clearMessagesInner();
    void clearDataInner();

public: // Logging
    CnLoggerManager *logger() const { return m_logger; }

protected:
    CnDeviceItemList           *m_items           ;
    CnDevicePokeMessageList    *m_poke            ;
    CnDevicePollMessageList    *m_poll            ;
    CnDeviceConfig             *m_cfg             ;
    CnDeviceStat               *m_stat            ;

protected: // 'Settings' struct
    struct
    {
        mutable CnCriticalSection cs;
        bool EnableDevice;
    } m_set;

protected:
    struct
    {
        mutable CnCriticalSection cs;

        Cn::StatusCode status;
        CnTimestamp timestamp;
        CnTimestamp initTimestamp   ;
    } m_data;

    struct
    {
        mutable CnCriticalSection cs;

        CnPort *port;
        uint32_t configChangeCounter;
        bool isActive;
        bool isEnablePort;
        struct
        {
            bool ConnectionAlive;
            bool ClockAlive;
            bool Initialized;
            bool ProcessInit;
            bool ProcessTimeSynch;
            bool CommunicationAllowed;
            bool Included;
        } flag;
    } m_vars;

protected: // Logger
    CnLoggerManager *m_logger;
};

typedef CnSharedPointer<CnDevice> CnDevicePtr;

#endif // CNDEVICE_H