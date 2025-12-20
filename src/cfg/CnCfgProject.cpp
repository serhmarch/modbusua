#include "CnCfgProject.h"

#include "CnCfgPort.h"
#include "CnCfgDevice.h"

CnCfgProject::CnCfgProject() : CnCfgBase()
{
}

CnCfgProject::~CnCfgProject()
{
}

CnCfgPortPtr CnCfgProject::port(const CnString &name) const
{
    auto it = m_hashPorts.find(name);
    if (it != m_hashPorts.end())
        return it->second;
    return nullptr;
}

void CnCfgProject::portAdd(CnCfgPort* port)
{ 
    m_ports.push_back(port);
    m_hashPorts[port->name()] = port;
}

CnCfgDevicePtr CnCfgProject::device(const CnString &name) const
{
    auto it = m_hashDevices.find(name);
    if (it != m_hashDevices.end())
        return it->second;
    return nullptr;
}

void CnCfgProject::deviceAdd(CnCfgDevice* dev)
{ 
    m_devices.push_back(dev);
    m_hashDevices[dev->name()] = dev;
}

Cn::Config CnCfgProject::configForGroup(const CnString &groupName) const
{
    auto it = m_groups.find(groupName);
    if (it != m_groups.end()) 
        return it->second;
    return Cn::Config();
}

const Cn::Config *CnCfgProject::configPtrForGroup(const CnString &groupName) const
{
    auto it = m_groups.find(groupName);
    if (it != m_groups.end()) 
        return &it->second;
    return nullptr;
}
