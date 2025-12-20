#ifndef CNPORTSTAT_H
#define CNPORTSTAT_H

#include <CnGlobal.h>

class CnPortStat
{
public:
    CnPortStat();

public:
    void clear();

public: // Statistics getters

    // Thread status
    inline uint32_t StatThreadCycleCount() const { CnCriticalSectionLocker _(&cs); return ThreadCycleCount; }
    inline uint64_t StatThreadCycleSum  () const { CnCriticalSectionLocker _(&cs); return ThreadCycleSum  ; }
    inline uint32_t StatThreadLastCycle () const { CnCriticalSectionLocker _(&cs); return ThreadLastCycle ; }
    inline uint32_t StatThreadMinCycle  () const { CnCriticalSectionLocker _(&cs); return ThreadMinCycle  ; }
    inline uint32_t StatThreadMaxCycle  () const { CnCriticalSectionLocker _(&cs); return ThreadMaxCycle  ; }
    inline uint32_t StatThreadAvgCycle  () const { CnCriticalSectionLocker _(&cs); return ThreadAvgCycle  ; }

    // Request status
    inline uint32_t           StatGoodCount           () const { CnCriticalSectionLocker _(&cs); return GoodCount           ; }
    inline uint32_t           StatBadCount            () const { CnCriticalSectionLocker _(&cs); return BadCount            ; }
    inline Modbus::StatusCode StatStatus              () const { CnCriticalSectionLocker _(&cs); return Status              ; }
    inline CnTimestamp      StatTimestamp           () const { CnCriticalSectionLocker _(&cs); return Timestamp           ; }
    inline CnTimestamp      StatLastSuccessTimestamp() const { CnCriticalSectionLocker _(&cs); return LastSuccessTimestamp; }
    inline Modbus::StatusCode StatLastErrorStatus     () const { CnCriticalSectionLocker _(&cs); return LastErrorStatus     ; }
    inline CnTimestamp      StatLastErrorTimestamp  () const { CnCriticalSectionLocker _(&cs); return LastErrorTimestamp  ; }
    inline CnString           StatLastErrorText       () const { CnCriticalSectionLocker _(&cs); return LastErrorText       ; }

public: // Statistics setters

    /// \details Sets the duration of the last working cycle.
    void setStatCycleTime(uint32_t time);

    /// \details Sets statistics of the last executed Modbus request.
    void setStatStatus(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err = CnString());

private:
    // Warning: do NOT use 'Stat'-struct in algorithm because its only purpose is to read from 'ItemReferences'
    mutable CnCriticalSection cs;

    // Thread status
    uint32_t ThreadCycleCount;
    uint64_t ThreadCycleSum  ;
    uint32_t ThreadLastCycle ;
    uint32_t ThreadMinCycle  ;
    uint32_t ThreadMaxCycle  ;
    uint32_t ThreadAvgCycle  ;
    // Request status
    uint32_t           GoodCount           ;
    uint32_t           BadCount            ;
    Modbus::StatusCode Status              ;
    CnTimestamp      Timestamp           ;
    CnTimestamp      LastSuccessTimestamp;
    Modbus::StatusCode LastErrorStatus     ;
    CnTimestamp      LastErrorTimestamp  ;
    CnString           LastErrorText       ;

};

#endif // CNDEVICESTAT_H