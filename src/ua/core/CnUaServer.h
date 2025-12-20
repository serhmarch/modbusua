/*!
 * \file   CnUaServer.h
 * \brief  Main class of OpcUa server gateway
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNUASERVER_H
#define CNUASERVER_H

#include <open62541/server.h>
#include "CnUaCore.h"

/*! \brief 

    \details 
 */

class CnUaServer
{
public:
    struct Strings
    {
        const CnString cfgGroup         ;
        const CnString cfgKeyPort       ;
        const CnString cfgKeyUncertainAs;

        Strings();
        static const Strings& instance();
    };

    struct Defaults
    {
        const UA_UInt16     Port       ;
        const UA_StatusCode UncertainAs;

        Defaults();
        static const Defaults& instance();
    };

public:
    static UA_StatusCode toUA_StatusCode(Cn::StatusCode status);

public:
    CnUaServer();
    ~CnUaServer();

public:
    inline UA_Server *server() const { return m_server; }
    inline UA_UInt16 namespaceIndex() const { return m_namespaceIndex; }
    inline UA_UInt16 nsIdx() const { return m_namespaceIndex; }
    inline CnUa::NodeId sourceNodeId() const { return m_sourceNodeId; }

public: // settings
    inline UA_UInt16 tcpPort() const { return m_settings.Port; }

    Cn::Config config() const;
    void setConfig(const Cn::Config& config);

public:
    //inline UA_LocalizedText toLocalizedText(const CnString &text) { return CnUa::toLocalizedText(text); }
    //inline UA_QualifiedName toQualifiedName(const CnString &name) { return CnUa::toQualifiedName(1, name); } 

public:
    UA_StatusCode runUntilInterrupt();

private:
    UA_Server *m_server;
    UA_UInt16 m_namespaceIndex;
    CnUa::NodeId m_sourceNodeId;

private:
    struct
    {
        UA_UInt16     Port       ;
        UA_StatusCode UncertainAs;
    } m_settings;
    
    static CnUaServer *s_global;
};

#endif // CNUASERVER_H