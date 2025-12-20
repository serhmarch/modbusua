#include "CnPortStat.h"

CnPortStat::CnPortStat()
{
    clear();
}

void CnPortStat::clear()
{
    CnCriticalSectionLocker _(&cs);
    ThreadCycleCount = 0;
    ThreadCycleSum   = 0;
    ThreadLastCycle  = 0;
    ThreadMinCycle   = 0xFFFFFFFF;
    ThreadMaxCycle   = 0;
    ThreadAvgCycle   = 0;
    GoodCount = 0;
    BadCount = 0;
    Status = Modbus::Status_Good;
    Timestamp = 0;
    LastSuccessTimestamp = 0;
    LastErrorStatus = Modbus::Status_Good;
    LastErrorTimestamp = 0;
}

void CnPortStat::setStatCycleTime(uint32_t time)
{
    CnCriticalSectionLocker _(&cs);
    ThreadCycleCount++;
    ThreadCycleSum += time;
    ThreadAvgCycle = static_cast<uint32_t>(ThreadCycleSum / ThreadCycleCount);
    ThreadLastCycle = time;
    if (time < ThreadMinCycle)
        ThreadMinCycle = time;
    if (time > ThreadMaxCycle)
        ThreadMaxCycle = time;
}

void CnPortStat::setStatStatus(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err)
{
    CnCriticalSectionLocker _(&cs);
    Status = status;
    Timestamp = timestamp;
    if (Modbus::StatusIsGood(status))
    {
        GoodCount++;
        LastSuccessTimestamp = timestamp;
    }
    else
    {
        BadCount++;
        LastErrorStatus = status;
        LastErrorTimestamp = timestamp;
        LastErrorText = err;
    }
}
