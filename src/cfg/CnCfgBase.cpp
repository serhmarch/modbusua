#include "CnCfgBase.h"

CnCfgBase::CnCfgBase()
{
    CN_REF_INIT
}

CnCfgBase::~CnCfgBase() = default;

const Cn::Config &CnCfgBase::config() const
{
    return m_config;
}

void CnCfgBase::setConfig(const Cn::Config &config) 
{
    auto it = config.find(CnSTR("Name"));
    if (it != config.end())
        setName(it->second.toString());
    
    m_config = config;
}