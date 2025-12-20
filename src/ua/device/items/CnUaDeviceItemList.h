#ifndef CNUADEVICEITEMLIST_H
#define CNUADEVICEITEMLIST_H

#include <device/items/CnDeviceItemList.h>

#include <CnUaCore.h>
#include <CnUaServer.h>

class CnUaDeviceItemList :public CnDeviceItemList
{
public:
    CnUaDeviceItemList(CnUaServer *server,
                          const CnString &deviceName,
                          const CnString &name);

    ~CnUaDeviceItemList() override;

public:
    void initializeUa(const CnUa::NodeId &parentId, const CnUa::NodeId &refTypeId);
    void finalizeUa();

protected:
    void addItemInner(CnDeviceBaseItem *item) override;
    void removeItemInner(CnDeviceBaseItem *item) override;

private:
    void innerInitUaNode();
    void innerClearUaNode();
    void innerAddUaItem(CnDeviceBaseItem *item);
    void innerClearUaItem(CnDeviceBaseItem *item);

private:
    CnUaServer *m_server;
    CnString m_deviceName;
    CnString m_name;
    CnUa::NodeId m_nodeIdParent;
    CnUa::NodeId m_nodeIdParentRef;
    CnUa::NodeId m_nodeId;
};

#endif // CNUADEVICEITEMLIST_H