/*!
 * \file   CnCfgProject.h
 * \brief  
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNCFGPROJECT_H
#define CNCFGPROJECT_H

#include "CnCfgBase.h"
#include "CnCfgPort.h"
#include "CnCfgDevice.h"

/*! \brief Root configuration object for the `modbusua` project.

    \details `CnCfgProject` aggregates ports (`CnCfgPort`), devices
    (`CnCfgDevice`), and grouped settings (`Cn::GroupConfig`). It
    provides lookup by name and add operations for ports and devices,
    and exposes helpers to retrieve configuration subsets by group.
    Typical instances are produced by `CnCfgBuilder`.

    \sa `CnCfgBuilder`, `CnCfgPort`, `CnCfgDevice`
*/
class CnCfgProject : public CnCfgBase
{
public:
    /// \details Constructs an empty project configuration.
    CnCfgProject();

    /// \details Destroys the project configuration.
    ~CnCfgProject();

public: // ports
    /// \details Returns the list of configured ports.
    inline const CnList<CnCfgPortPtr> &ports() const { return m_ports; }

    /// \details Finds and returns a port by its name, or null pointer.
    CnCfgPortPtr port(const CnString &name) const;

    /// \details Adds a port to the project and indexes it by name.
    void portAdd(CnCfgPort* port);

public: // devices
    /// \details Returns the list of configured devices.
    inline const CnList<CnCfgDevicePtr> &devices() const { return m_devices; }

    /// \details Finds and returns a device by its name, or null pointer.
    CnCfgDevicePtr device(const CnString &name) const;

    /// \details Adds a device to the project and indexes it by name.
    void deviceAdd(CnCfgDevice* dev);

public: // groups
    /// \details Returns the grouped configuration dictionary (const).
    inline const Cn::GroupConfig& groupConfig() const { return m_groups; }

    /// \details Returns the grouped configuration dictionary (mutable).
    inline Cn::GroupConfig& groupConfig() { return m_groups; }

    /// \details Sets the entire grouped configuration dictionary.
    inline void setGroupConfig(const Cn::GroupConfig& groupConfig) { m_groups = groupConfig; }

    /// \details Swaps grouped configuration with the provided argument.
    inline void swapGroupConfig(Cn::GroupConfig& groupConfig) { m_groups.swap(groupConfig); }

    /// \details Returns a merged configuration subset for `groupName`.
    Cn::Config configForGroup(const CnString &groupName) const;
    
    /// \details Returns a pointer to the configuration subset if present.
    const Cn::Config* configPtrForGroup(const CnString &groupName) const;
    
private:
    typedef CnList<CnCfgPortPtr> Ports_t;
    typedef CnHash<CnString, CnCfgPortPtr> HashPorts_t;
    Ports_t m_ports;
    HashPorts_t m_hashPorts;

    typedef CnList<CnCfgDevicePtr> Devices_t;
    typedef CnHash<CnString, CnCfgDevicePtr> HashDevices_t;
    Devices_t m_devices;
    HashDevices_t m_hashDevices;

    Cn::GroupConfig m_groups;

};

typedef CnSharedPointer<CnCfgProject> CnCfgProjectPtr;

#endif // CNCFGPROJECT_H
