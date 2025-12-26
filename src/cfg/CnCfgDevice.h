/*!
 * \file   CnCfgDevice.h
 * \brief  
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNCFGDEVICE_H
#define CNCFGDEVICE_H

#include "CnCfgBase.h"
#include "CnCfgDeviceItem.h"

class CnCfgPort;
class CnCfgDeviceItem;

/*! \brief Device/PLC configuration object in the project.

    \details `CnCfgDevice` stores the name of its communication port,
    an optional source file reference, and a collection of device
    items (`CnCfgDeviceItem`). It exposes efficient lookup by item
    name via an internal hash table and provides accessors to the
    item list.

    \sa `CnCfgDeviceItem`, `CnCfgPort`
*/
class CnCfgDevice : public CnCfgBase
{
public:
    using CnCfgBase::CnCfgBase;

    ~CnCfgDevice();

public:
    /// \details Returns the associated communication port name.
    inline const CnString &portName() const { return m_portName; }
    
    /// \details Sets the associated communication port name.
    inline void setPortName(const CnString &name) { m_portName = name; }

public:
    /// \details Applies the device-specific configuration values.
    void setConfig(const Cn::Config &cfg) override;

    /// \details Returns the list of configured device items.
    inline const CnList<CnCfgDeviceItem*> &items() const { return m_items; }

    /// \details Returns `true` if an item with name `s` exists.
    inline bool hasItem(const CnString &s) const { return m_hash.find(s) != m_hash.end(); }
   
    /// \details Adds an item to the device and index it by name.
    inline void addItem(CnCfgDeviceItem* item) { m_items.push_back(item); m_hash.emplace(item->name(), item); }

private:
    CnString m_portName;
    CnString m_file;
    CnList<CnCfgDeviceItem*> m_items;
    CnHash<CnString, CnCfgDeviceItem*> m_hash;
};

typedef CnSharedPointer<CnCfgDevice> CnCfgDevicePtr;

#endif // CNCFGDEVICE_H
