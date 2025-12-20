#include "CnDeviceMessageBase.h"

void CnDeviceMessageBase::signalRemovingItem(CnDeviceModbusItem* item)
{
    emitSignal(__func__, &CnDeviceMessageBase::signalRemovingItem, item);
}