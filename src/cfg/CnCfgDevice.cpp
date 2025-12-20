#include "CnCfgDevice.h"

void CnCfgDevice::setConfig(const Cn::Config &cfg)
{
    CnCfgBase::setConfig(cfg);
    auto it = m_config.find(CnSTR("PortName"));
    if (it != m_config.end())
    {
        setPortName(it->second.toString());
        m_config.erase(it);
    }
}
