/*!
 * \file   CnPort.h
 * \brief  Describes the structure storing all runtime port parameters
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNPORT_H
#define CNPORT_H

#include <CnGlobal.h>
#include <CnObject.h>
#include <log/CnLoggerManager.h>
#include <cfg/CnCfgPort.h>

#include "CnPortConfig.h"

#include <device/CnDevice.h>

class CnPortConfig;
class CnPortStat;
class CnPortRunnable;
class CnDeviceRunnable;
class CnDevice;

/*! \brief Describes the structure storing all runtime port parameters.

    \details This class contains all configuration, settings, statistics,
    and commands related to the operation of this port. It also exposes
    logging functions for this object.
 */

class CnPort : public CnObject
{
public:
    class Event : public CnEvent
    {
    public:
        enum Type
        {
            SetConfig   ,
            SetCfgPort  ,
            AddDevice   ,
            RemoveDevice
        };
    };

    class EventSetConfig : public Event 
    {
    public: 
        EventSetConfig(const Cn::Config &v) : config(v) {  }
        int type() const override { return SetConfig; }
        Cn::Config config;
    };

    class EventSetCfgPort : public Event 
    {
    public: 
        EventSetCfgPort(const CnCfgPortPtr &v) : cfg(v) {  }
        int type() const override { return SetCfgPort; }
        CnCfgPortPtr cfg;
    };

    class EventAddDevice : public Event
    {
    public:
        EventAddDevice(CnDevice *dev) : device(dev) {}
        int type() const override { return AddDevice; }
        CnDevicePtr device;
    };

    class EventRemoveDevice : public Event
    {
    public:
        EventRemoveDevice(CnDevice *dev) : device(dev) {}
        int type() const override { return RemoveDevice; }
        CnDevicePtr device;
    };

public:
    /// \details Constructs the port object.
    explicit CnPort();

    /// \details Destructs the port object.
    ~CnPort();

public: // signals
    void signalAddDevice(CnDevice *dev);
    void signalRemoveDevice(CnDevice *dev);

public:
    bool event(CnEvent *event) override;

public:
    /// \details Virtual function called before object processing starts.
    virtual void initialize();

    /// \details Virtual function called before object destruction begins.
    virtual void finalize();

public: // Config
    inline CnString CfgName            () const { return m_cfg->CfgName            (); } ///< Configuration. Name of this port
    inline bool     CfgEnable          () const { return m_cfg->CfgEnable          (); } ///< Configuration. Availability for runtime execution sequence (1-on, 0-off)
    inline CnString CfgType            () const { return m_cfg->CfgType            (); } ///< Configuration. Modbus protocol type
    inline CnString CfgHost            () const { return m_cfg->CfgHost            (); } ///< Configuration. IP address or DNS name of the remote device
    inline uint16_t CfgPort            () const { return m_cfg->CfgPort            (); } ///< Configuration. TCP port number (standard Modbus TCP port 502)
    inline uint32_t CfgTimeout         () const { return m_cfg->CfgTimeout         (); } ///< Configuration. Connection timeout (milliseconds)
    inline CnString CfgSerialPortName  () const { return m_cfg->CfgSerialPortName  (); } ///< Configuration. Serial port name
    inline int32_t  CfgBaudRate        () const { return m_cfg->CfgBaudRate        (); } ///< Configuration. Serial port baud rate
    inline int8_t   CfgDataBits        () const { return m_cfg->CfgDataBits        (); } ///< Configuration. Serial port data bits
    inline CnString CfgParity          () const { return m_cfg->CfgParity          (); } ///< Configuration. Serial port parity
    inline CnString CfgStopBits        () const { return m_cfg->CfgStopBits        (); } ///< Configuration. Serial port stop bits
    inline CnString CfgFlowControl     () const { return m_cfg->CfgFlowControl     (); } ///< Configuration. Serial port flow control
    inline uint32_t CfgTimeoutFirstByte() const { return m_cfg->CfgTimeoutFirstByte(); } ///< Configuration. Timeout waiting for the first byte (ms)
    inline uint32_t CfgTimeoutInterByte() const { return m_cfg->CfgTimeoutInterByte(); } ///< Configuration. Inter-byte timeout (ms)

public:
    /// \details Returns the port display name.
    inline CnString name() const { return m_cfg->CfgName(); }

    /// \details Returns the configuration change counter value.
    inline uint32_t configChangeCounter() const { CnCriticalSectionLocker _(&m_vars.cs); return m_vars.configChangeCounter; }

    /// \details Returns the associative configuration map.
    Cn::Config config() const;

    /// \details Sets the configuration.
    void setConfig(const Cn::Config& config);

    /// \details Sets the port configuration object.
    void setCfgPort(const CnCfgPortPtr& cfg);

public:
    CnPortConfig *cfg () const { return m_cfg ; }
    CnPortStat   *stat() const { return m_stat; }    

public: // Settings
    /// \details Returns the setting 'Availability of this port for runtime execution sequence (1-on, 0-off)'.
    inline bool SettingEnable() const { CnCriticalSectionLocker _(&m_set.cs); return m_set.Enable; }

    /// \details Sets the setting 'Availability of this port for runtime execution sequence (1-on, 0-off)'.
    inline void setSettingEnable(bool v) { CnCriticalSectionLocker _(&m_set.cs); m_set.Enable = v; }

public: // Commands
    /// \details Executes command 'Clear statistical information'.
    void CmdStatClear();

public: // Statistics
    /// \details Sets the duration of the last working cycle.
    void setStatCycleTime(uint32_t time);

    /// \details Sets statistics of the last executed Modbus request.
    void setStatStatus(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err = CnString());

public:
    /// \details Returns `true` if this port is enabled for runtime execution sequence, otherwise `false`.
    inline bool isEnable() const { { return CfgEnable() && SettingEnable(); } }

public: // inner command interface
    CnList<CnDevice*> devices() const;
    void addDevice(CnDevice* device);
    void removeDevice(CnDevice* device);

public: // Logging
    CnLoggerManager *logger() const { return m_logger; }

protected: // Config
    void setConfigInner(const Cn::Config& config);
    void setCfgPortInner(const CnCfgPortPtr& cfg);
    virtual void addDeviceInner(CnDevice* device);
    virtual void removeDeviceInner(CnDevice* device);

protected: // 'Config'-struct
    CnPortConfig         *m_cfg ;
    CnPortStat           *m_stat;

protected: // 'Settings'-struct
    struct Setting
    {
        mutable CnCriticalSection cs;
        Setting()
        {
            Enable = true;
        }
        bool Enable;
    } m_set;

protected: // Common Variables
    typedef CnList<CnDevice*> Devices_t;
    struct
    {
        mutable CnCriticalSection cs;
        uint32_t configChangeCounter;
        Devices_t devices;
    } m_vars;

protected: // Logger
    CnLoggerManager *m_logger;
};

typedef CnSharedPointer<CnPort> CnPortPtr;

#endif // CNPORT_H