#ifndef CNUADEVICEMESSAGELIST_H
#define CNUADEVICEMESSAGELIST_H

#include <device/messages/CnDeviceMessageList.h>

#include <CnUaCore.h>
#include <CnUaServer.h>

class CnUaDevice;

// ------------------------------------------------------------------------------------------
// ------------------------------ CnUaDevicePokeMessageList ------------------------------
// ------------------------------------------------------------------------------------------

class CnUaDevicePokeMessageList : public CnDevicePokeMessageList
{
public:
    CnUaDevicePokeMessageList(CnUaDevice *device, CnUaServer *server, const CnString &name);

public:
    void initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId);
    void finalizeUa();

protected:
    void addMessageInner(CnDeviceMessageBase *m) override;
    CnDeviceMessageBase *createMessage(CnDeviceModbusItem *item) override;

private:
    CnUaServer *m_server;
    CnString m_name;
    CnUa::NodeId m_nodeIdParent;
    CnUa::NodeId m_nodeIdParentRef;
    CnUa::NodeId m_nodeId;
};

// ------------------------------------------------------------------------------------------
// ------------------------------ CnUaDevicePollMessageList ------------------------------
// ------------------------------------------------------------------------------------------

class CnUaDevicePollMessageList : public CnDevicePollMessageList
{
public:
    CnUaDevicePollMessageList(CnUaDevice *device, CnUaServer *server, const CnString &name);

public:
    void initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId);
    void finalizeUa();

protected:
    void addMessageInner(CnDeviceMessageBase *m) override;
    CnDeviceMessageBase *createMessage(CnDeviceModbusItem *item) override;

private:
    void innerInitUaNode();
    void innerClearUaNode();

private:
    CnUaServer *m_server;
    CnString m_name;
    CnUa::NodeId m_nodeIdParent;
    CnUa::NodeId m_nodeIdParentRef;
    CnUa::NodeId m_nodeId;
};

#endif // CNUADEVICEMESSAGELIST_H