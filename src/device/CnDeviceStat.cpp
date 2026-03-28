#include "CnDeviceStat.h"

CnDeviceStat::CnDeviceStat()
{
    StateEnableDevice             = false;
    StateConnectionAlive          = false;
    StateIncluded                 = false;
    clear();
}

void CnDeviceStat::clear()
{
    CnCriticalSectionLocker _(&cs);
    SinceTimestamp = CnTimestamp::current();
    // Request status
    Common = RequestStatus();
    Poke = RequestStatus();
    Poll = RequestStatus();    
    // Misc
    InnerState = 0;
}

uint32_t CnDeviceStat::StatState() const
{
    CnCriticalSectionLocker _(&cs);
    uint32_t v = 0;
    v |= StateEnableDevice             * StateFlag_EnableDevice            ;
    v |= StateConnectionAlive          * StateFlag_ConnectionAlive         ;
    v |= StateIncluded                 * StateFlag_Included                ;
    return v;
}

void CnDeviceStat::setStatPokeResult(Modbus::StatusCode status, CnTimestamp timestamp, const CnString & err)
{
    setStatRequestStatus(status, timestamp, err, Poke);
}

void CnDeviceStat::setStatPollResult(Modbus::StatusCode status, CnTimestamp timestamp, const CnString & err)
{
    setStatRequestStatus(status, timestamp, err, Poll);
}

void CnDeviceStat::setStatRequestStatus(Modbus::StatusCode status, CnTimestamp timestamp, const CnString &err, RequestStatus &stat)
{
    CnCriticalSectionLocker _(&cs);
    stat.Status = status;
    stat.Timestamp = timestamp;
    // Common Status
    Common.Status = status;
    Common.Timestamp = timestamp;

    if (Modbus::StatusIsGood(status))
    {
        stat.GoodCount++;
        stat.LastSuccessTimestamp = timestamp;
        // Common Status
        Common.GoodCount++;
        Common.LastSuccessTimestamp = timestamp;
    }
    else
    {
        stat.BadCount++;
        stat.LastErrorStatus = status;
        stat.LastErrorTimestamp = timestamp;
        stat.LastErrorText = err;
        // Common Status
        Common.BadCount++;
        Common.LastErrorStatus = status;
        Common.LastErrorTimestamp = timestamp;
        Common.LastErrorText = err;
    }
}
