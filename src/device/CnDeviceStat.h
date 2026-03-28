#ifndef CNDEVICESTAT_H
#define CNDEVICESTAT_H

#include <CnGlobal.h>

class CnDevice;

class CnDeviceStat
{
public:
    /// \brief Bitmask of the current Device state.
    enum StateFlags
    {
        StateFlag_EnableDevice    = 0x00000001, ///< Communication allowed
        StateFlag_ConnectionAlive = 0x00000004, ///< Connection with PLC is present
        StateFlag_Included        = 0x00000400, ///< This PLC is included in the execution sequence; it can be excluded if a communication error occurred and the restore timeout has not elapsed
    };

public:
    CnDeviceStat();

public:
    void clear();

public: // Statistics getters

    // Since timestamp
    inline CnTimestamp StatSinceTimestamp() const { CnCriticalSectionLocker _(&cs); return SinceTimestamp; } ///< \details Statistics. Timestamp since statistics is collected (after init or clear)

    // State Statistics                                              
           uint32_t StatState               () const;                                 ///< \details Statistics. Bitmask of the current gateway state
    inline bool     StatStateEnableDevice   () const { return StateEnableDevice   ; } ///< \details Statistics. Communication allowed (bit 0 of Stat.State)
    inline bool     StatStateConnectionAlive() const { return StateConnectionAlive; } ///< \details Statistics. Connection with PLC is present (bit 2 of Stat.State)
    inline bool     StatStateIncluded       () const { return StateIncluded       ; } ///< \details Statistics. PLC included in execution sequence; may be excluded if a communication error occurred and restore timeout has not elapsed (bit 10 of Stat.State)
                                                                     
    // Common Modbus-message Statistics                              
    inline uint32_t            StatCommonGoodCount                   () const { CnCriticalSectionLocker _(&cs); return Common.GoodCount           ; } ///< \details Statistics. Total number of successful Modbus messages
    inline uint32_t            StatCommonBadCount                    () const { CnCriticalSectionLocker _(&cs); return Common.BadCount            ; } ///< \details Statistics. Total number of unsuccessful Modbus messages
    inline Modbus::StatusCode  StatCommonStatus                      () const { CnCriticalSectionLocker _(&cs); return Common.Status              ; } ///< \details Statistics. Status of the last Modbus message (p.2.9)
    inline CnTimestamp         StatCommonTimestamp                   () const { CnCriticalSectionLocker _(&cs); return Common.Timestamp           ; } ///< \details Statistics. Time (PC) of the last Modbus message
    inline CnTimestamp         StatCommonLastSuccessTimestamp        () const { CnCriticalSectionLocker _(&cs); return Common.LastSuccessTimestamp; } ///< \details Statistics. Time (PC) of the last successful Modbus message
    inline Modbus::StatusCode  StatCommonLastErrorStatus             () const { CnCriticalSectionLocker _(&cs); return Common.LastErrorStatus     ; } ///< \details Statistics. Status of the last Modbus message error (p.2.9)
    inline CnTimestamp         StatCommonLastErrorTimestamp          () const { CnCriticalSectionLocker _(&cs); return Common.LastErrorTimestamp  ; } ///< \details Statistics. Time (PC) of the last unsuccessful Modbus message
    inline CnString            StatCommonLastErrorText               () const { CnCriticalSectionLocker _(&cs); return Common.LastErrorText       ; } ///< \details Statistics. Text representation of the last Modbus message error
                                                                     
    // Poke Statistics                                               
    inline uint32_t            StatPokeGoodCount                     () const { CnCriticalSectionLocker _(&cs); return Poke.GoodCount           ; } ///< \details Statistics. Number of successful simple Modbus-Write messages
    inline uint32_t            StatPokeBadCount                      () const { CnCriticalSectionLocker _(&cs); return Poke.BadCount            ; } ///< \details Statistics. Number of unsuccessful simple Modbus-Write messages
    inline Modbus::StatusCode  StatPokeStatus                        () const { CnCriticalSectionLocker _(&cs); return Poke.Status              ; } ///< \details Statistics. Status of the last simple Modbus-Write message (p.2.9)
    inline CnTimestamp         StatPokeTimestamp                     () const { CnCriticalSectionLocker _(&cs); return Poke.Timestamp           ; } ///< \details Statistics. Time (PC) of the last simple Modbus-Write message
    inline CnTimestamp         StatPokeLastSuccessTimestamp          () const { CnCriticalSectionLocker _(&cs); return Poke.LastSuccessTimestamp; } ///< \details Statistics. Time (PC) of the last successful simple Modbus-Write message
    inline Modbus::StatusCode  StatPokeLastErrorStatus               () const { CnCriticalSectionLocker _(&cs); return Poke.LastErrorStatus     ; } ///< \details Statistics. Status of the last error of a simple Modbus-Write message (p.2.9)
    inline CnTimestamp         StatPokeLastErrorTimestamp            () const { CnCriticalSectionLocker _(&cs); return Poke.LastErrorTimestamp  ; } ///< \details Statistics. Time (PC) of the last unsuccessful simple Modbus-Write message
    inline CnString            StatPokeLastErrorText                 () const { CnCriticalSectionLocker _(&cs); return Poke.LastErrorText       ; } ///< \details Statistics. Text representation of the last error of a simple Modbus-Write message
                                                                     
    // Poll Statistics                                               
    inline uint32_t            StatPollGoodCount                     () const { CnCriticalSectionLocker _(&cs); return Poll.GoodCount           ; } ///< \details Statistics. Number of successful simple Modbus-Read messages
    inline uint32_t            StatPollBadCount                      () const { CnCriticalSectionLocker _(&cs); return Poll.BadCount            ; } ///< \details Statistics. Number of unsuccessful simple Modbus-Read messages
    inline Modbus::StatusCode  StatPollStatus                        () const { CnCriticalSectionLocker _(&cs); return Poll.Status              ; } ///< \details Statistics. Status of the last simple Modbus-Read message (p.2.9)
    inline CnTimestamp         StatPollTimestamp                     () const { CnCriticalSectionLocker _(&cs); return Poll.Timestamp           ; } ///< \details Statistics. Time (PC) of the last simple Modbus-Read message
    inline CnTimestamp         StatPollLastSuccessTimestamp          () const { CnCriticalSectionLocker _(&cs); return Poll.LastSuccessTimestamp; } ///< \details Statistics. Time (PC) of the last successful simple Modbus-Read message
    inline Modbus::StatusCode  StatPollLastErrorStatus               () const { CnCriticalSectionLocker _(&cs); return Poll.LastErrorStatus     ; } ///< \details Statistics. Status of the last error of a simple Modbus-Read message (p.2.9)
    inline CnTimestamp         StatPollLastErrorTimestamp            () const { CnCriticalSectionLocker _(&cs); return Poll.LastErrorTimestamp  ; } ///< \details Statistics. Time (PC) of the last unsuccessful simple Modbus-Read message
    inline CnString            StatPollLastErrorText                 () const { CnCriticalSectionLocker _(&cs); return Poll.LastErrorText       ; } ///< \details Statistics. Text representation of the last error of a simple Modbus-Read message

    /// \details Returns the internal state of the communication algorithm with the device/PLC.
    inline int                 StatInnerState                        () const { CnCriticalSectionLocker _(&cs); return InnerState; }

public: // Statistics setters
    void setStatStateEnableDevice            (bool v) { CnCriticalSectionLocker _(&cs); StateEnableDevice             = v; } ///< \details Statistics. Communication allowed (bit 0 of Stat.State)
    void setStatStateConnectionAlive         (bool v) { CnCriticalSectionLocker _(&cs); StateConnectionAlive          = v; } ///< \details Statistics. Connection with PLC is present (bit 2 of Stat.State)
    void setStatStateIncluded                (bool v) { CnCriticalSectionLocker _(&cs); StateIncluded                 = v; } ///< \details Statistics. PLC included in execution sequence; may be excluded if a communication error occurred and restore timeout has not elapsed (bit 10 of Stat.State)

    /// \details Sets static data of the last Modbus message for direct data write.
    /// \param[in]  status  Result status
    /// \param[in]  err     Error text (if occurred)
    void setStatPokeResult(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err = CnString());

    /// \details Sets static data of the last Modbus message for direct data read.
    /// \param[in]  status  Result status
    /// \param[in]  err     Error text (if occurred)
    void setStatPollResult(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err = CnString());

    /// \details Sets the internal state of the communication algorithm with the device/PLC.
    inline void setStatInnerState(int state) { CnCriticalSectionLocker _(&cs); InnerState = state; }

private:
    struct RequestStatus;
    void setStatRequestStatus(Modbus::StatusCode status, CnTimestamp timestamp, const CnString& err, RequestStatus &stat);
    
private:
    // Warning: do NOT use 'Stat'-struct in algorithm because its only purpose is to read from 'ItemReferences'
    mutable CnCriticalSection cs;

    struct RequestStatus
    {
        RequestStatus()
        {
            this->GoodCount = 0;
            this->BadCount = 0;
            this->Status = Modbus::Status_Uncertain;
            this->Timestamp = 0;
            this->LastSuccessTimestamp = 0;
            this->LastErrorStatus = Modbus::Status_Uncertain;
            this->LastErrorTimestamp = 0;
        }

        uint32_t GoodCount;
        uint32_t BadCount;
        Modbus::StatusCode Status;
        CnTimestamp Timestamp;
        CnTimestamp LastSuccessTimestamp;
        Modbus::StatusCode LastErrorStatus;
        CnTimestamp LastErrorTimestamp;
        CnString LastErrorText;
    };

    // Since timestamp
    CnTimestamp SinceTimestamp;
    // State
    bool StateEnableDevice   ;
    bool StateConnectionAlive;
    bool StateIncluded       ;
    // Request status
    RequestStatus Common;
    RequestStatus Poke  ;
    RequestStatus Poll  ;
    // Misc
    int InnerState;

};

#endif // CNDEVICESTAT_H