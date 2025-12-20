#include "CnDeviceBaseItem.h"

CnDeviceBaseItem::CnDeviceBaseItem(CnDevice *device, const CnString &name) : CnBaseItem(name),
    m_device(device)
{
    //m_device->IncrementRef(); Note: no need to hold ref because system must guarantee that item delete before device
}

CnDeviceBaseItem::CnDeviceBaseItem(CnDevice *device) : CnBaseItem(CnString()),
    m_device(device)
{
}
