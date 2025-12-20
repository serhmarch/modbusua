#include "CnUaServer.h"

CnUaServer* CnUaServer::s_global = nullptr;

CnUaServer::Strings::Strings() :
    cfgGroup         (CnSTR("OPCUA")),
    cfgKeyPort       (CnSTR("Port")),
    cfgKeyUncertainAs(CnSTR("UncertainAs"))
{
}

const CnUaServer::Strings &CnUaServer::Strings::instance()
{
    static const Strings s;
    return s;
}

CnUaServer::Defaults::Defaults() :
    Port       (CN_UA_TCP_PORT),
    UncertainAs(UA_STATUSCODE_UNCERTAIN)
{
}

const CnUaServer::Defaults &CnUaServer::Defaults::instance()
{
    static const Defaults s;
    return s;
}

UA_StatusCode CnUaServer::toUA_StatusCode(Cn::StatusCode status)
{
    if (Cn::StatusIsGood(status))
        return UA_STATUSCODE_GOOD;
    if (Cn::StatusIsBad(status))
        return UA_STATUSCODE_BAD;
    return s_global->m_settings.UncertainAs;
}

CnUaServer::CnUaServer()
{
    /* Create a server listening on port 4840 (default) */
    m_server = UA_Server_new();

    m_namespaceIndex = UA_Server_addNamespace(m_server, CN_UA_NAMESPACE);
    // default settings
    const Defaults &d = Defaults::instance();
    m_settings.Port        = d.Port       ;
    m_settings.UncertainAs = d.UncertainAs;

    // TODO: check for s_global ptr must be null
    s_global = this;
}

CnUaServer::~CnUaServer()
{
    /* Clean up */
    UA_Server_delete(m_server);
}

Cn::Config CnUaServer::config() const
{
    const Strings& sKeys = Strings::instance();
    Cn::Config config;
    config[sKeys.cfgKeyPort       ] = m_settings.Port       ;
    config[sKeys.cfgKeyUncertainAs] = m_settings.UncertainAs;
    return config;
}

void CnUaServer::setConfig(const Cn::Config &config)
{
    const Strings& sKeys = Strings::instance();
    bool ok;
    auto end = config.end();
    auto it = end;

    it = config.find(sKeys.cfgKeyPort);
    if (it != end)
        m_settings.Port = it->second.toUInt16();

    it = config.find(sKeys.cfgKeyUncertainAs);
    if (it != end)
    {
        ok = true;
        CnString v = Cn::toUpper(it->second.toString());
        if (v == CnSTR("GOOD"))
            m_settings.UncertainAs = UA_STATUSCODE_GOOD;
        else if (v == CnSTR("BAD"))
            m_settings.UncertainAs = UA_STATUSCODE_BAD;
        else if (v == CnSTR("UNCERTAIN"))
            m_settings.UncertainAs = UA_STATUSCODE_UNCERTAIN;
        else
            ok = false;
        }
    else
        ok = false;
    if (!ok)    
        m_settings.UncertainAs = UA_STATUSCODE_UNCERTAIN;
}

UA_StatusCode CnUaServer::runUntilInterrupt()
{
    return UA_Server_runUntilInterrupt(m_server);
}
