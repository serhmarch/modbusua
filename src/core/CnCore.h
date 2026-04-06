/*!
 * \file   CnCore.h
 * \brief  Contains general definitions, types, and functions for the library.
 * 
 * \author serhmarch
 * \date   November 2024
*/
#ifndef CNCORE_H
#define CNCORE_H

#include "CnConfig.h"

#define CN_VERSION_STR_HELPER(major,minor,patch) #major"."#minor"."#patch

#define CN_VERSION_STR_MAKE(major,minor,patch) CN_VERSION_STR_HELPER(major,minor,patch)

#define CN_VERSION_STR CN_VERSION_STR_MAKE(CN_VERSION_MAJOR,CN_VERSION_MINOR,CN_VERSION_PATCH)

#define CN_VERSION_WSTR CnSTR_make(CN_VERSION_STR)

#if defined(CN_EXPORTS)
#define CN_EXPORT __declspec(dllexport)
#else
#define CN_EXPORT __declspec(dllimport)
#endif

#define CN_SEC_IN_MINUTE               60
#define CN_SEC_IN_HOUR                 3600
#define CN_SEC_IN_DAY                  86400

#define CN_MINUTE_IN_HOUR              60
#define CN_MINUTE_IN_DAY               1440
#define CN_HOUR_IN_DAY                 24

#define CN_MILLISEC_IN_SEC             1000
#define CN_MILLISEC_IN_MINUTE          60000
#define CN_MILLISEC_IN_HOUR            3600000
#define CN_MILLISEC_IN_DAY             86400000

#define CN_MICROSEC_IN_MILLISEC        1000
#define CN_MICROSEC_IN_SEC             1000000
#define CN_MICROSEC_IN_MINUTE          60000000

#define CN_NANOSEC_IN_NANOSEC100       100

#define CN_NANOSEC100_IN_MICROSEC      10
#define CN_NANOSEC100_IN_MILLISEC      10000
#define CN_NANOSEC100_IN_SEC           10000000
#define CN_NANOSEC100_IN_MINUTE        600000000

#define CN_SEC_BETWEEN_1601_AND_1970   11644473600LL
#define CN_MSEC_BETWEEN_1601_AND_1970  ((CN_SEC_BETWEEN_1601_AND_1970)*CN_MILLISEC_IN_SEC)
#define CN_NANOSEC100_BETWEEN_1601_AND_1970 ((CN_SEC_BETWEEN_1601_AND_1970)*CN_NANOSEC100_IN_SEC)

#define CN_SEC_BETWEEN_1601_AND_1980   11960006400LL
#define CN_MSEC_BETWEEN_1601_AND_1980  ((CN_SEC_BETWEEN_1601_AND_1980)*CN_MILLISEC_IN_SEC)
#define CN_NANOSEC100_BETWEEN_1601_AND_1980 ((CN_SEC_BETWEEN_1601_AND_1980)*CN_NANOSEC100_IN_SEC)


#include <cstdint>
#include <thread>
#include <atomic>

#if defined(_WIN32) || defined(WIN32)

#include <Windows.h>
#include <sysinfoapi.h>

#define CN_PATH_MAX MAX_PATH

#else // _WIN32 || WIN32

#include <ctime>
#include <pthread.h>
#include <climits>

#define CN_PATH_MAX PATH_MAX

#endif // _WIN32 || WIN32


#ifdef CN_USE_WSTRING

#define CnCHR(cchr) L##cchr
#define CnSTR(cstr) L##cstr

typedef wchar_t CnChar;
//typedef std::wstring CnString;

#else // CN_USE_WSTRING

#define CnCHR(cchr) cchr
#define CnSTR(cstr) cstr

typedef char CnChar;
//typedef std::string CnString;

#endif // CN_USE_WSTRING

#define CnSTR_make(macro) CnSTR(macro)

typedef std::atomic_int CnRefCount_t;


namespace Cn {

/// \brief Operation status code.
enum StatusCode
{
    Status_Processing       = 0x80000000, ///< Operation not completed. Further execution required
    Status_Bad              = 0x40000000, ///< Operation finished with error
    Status_BadOutOfService  = 0x40000001, ///< Error. Data unavailable
    Status_BadNotConnected  = 0x40000002, ///< Error. No connection
    Status_BadConfigure     = 0x40000003, ///< Configuration error
    Status_BadNoService     = 0x40000004, ///< Cannot run program as a service
    Status_Uncertain        = 0x20000000, ///< Operation status is uncertain
    Status_Good             = 0x00000000, ///< Operation completed successfully
    Status_GoodNoData       = 0x00000001, ///< Operation completed successfully. No new data
};

/// \brief Specifies the data type
enum DataType
{
    Data_Unknown     ,
    Data_Bool        ,
    Data_Int8        ,
    Data_UInt8       ,
    Data_Int16       ,
    Data_UInt16      ,
    Data_Int32       ,
    Data_UInt32      ,
    Data_Int64       ,
    Data_UInt64      ,
    Data_Int         ,
    Data_UInt        ,
    Data_Float       ,
    Data_Double      ,
    Data_String      ,
    Data_Timestamp   ,
    Data_DateTime    ,
    Data_Variant     ,
    Data_ModbusStatus,
    Data_CnStatus 
};

/// \brief Data type suffix of the gateway.
enum DataSuffix
{ 
    Suffix_Default        , ///< Default data type
    Suffix_Bool           , ///< Boolean
    Suffix_Int16          , ///< 16-bit signed integer
    Suffix_UInt16         , ///< 16-bit unsigned integer
    Suffix_Int32          , ///< 32-bit signed integer
    Suffix_UInt32         , ///< 32-bit unsigned integer
    Suffix_Int64          , ///< 64-bit signed integer
    Suffix_UInt64         , ///< 64-bit unsigned integer
    Suffix_Float          , ///< 32-bit floating point
    Suffix_Double         , ///< 64-bit floating point
    Suffix_SwappedInt32   , ///< 32-bit signed integer with swapped registers
    Suffix_SwappedUInt32  , ///< 32-bit unsigned integer with swapped registers
    Suffix_SwappedInt64   , ///< 64-bit signed integer with swapped registers
    Suffix_SwappedUInt64  , ///< 64-bit unsigned integer with swapped registers
    Suffix_SwappedFloat   , ///< 32-bit floating point with swapped registers
    Suffix_SwappedDouble  , ///< 64-bit floating point with swapped registers
    Suffix_ByteArray        ///< Byte array
};

/// \brief Specifies the access type
enum Access
{
    Access_Read      = 1, ///< 1: Read access
    Access_Write     = 2, ///< 2: Write access
    Access_ReadWrite = Access_Read | 
                       Access_Write ///< 3: Read and write access
};

/// \brief 
typedef std::thread::id ThreadId_t;

/// \details Represents the timer variable type returned by the `GetTimer()` function. Contains the number of milliseconds elapsed since system startup.
typedef uint64_t Timer_t;

/// \brief Console text color.
enum Color
{                  //   Unix Color    Windows Color   
    Color_Black  , // 30 = Black     0 = Black
    Color_Red    , // 31 = Red       4 = Red       
    Color_Green  , // 32 = Green     2 = Green
    Color_Yellow , // 33 = Yellow    6 = Yellow 
    Color_Blue   , // 34 = Blue      1 = Blue 
    Color_Magenta, // 35 = Magenta   13 = Light Purple   
    Color_Cyan   , // 36 = Cyan      9 = Light Blue 
    Color_White  , // 37 = White     7 = White (default) 
    Color_Default  
};

/// \details Returns a general indicator that the operation result is successful.
inline bool StatusIsGood(Cn::StatusCode status) { return (status & 0xF0000000) == Cn::Status_Good; }

/// \details Returns a general indicator that the operation result is unsuccessful.
inline bool StatusIsBad(Cn::StatusCode status) { return (status & 0xF0000000) == Cn::Status_Bad; }

/// \details Returns a general indicator that the operation result is uncertain.
inline bool StatusIsUncertain(Cn::StatusCode status) { return (status & 0xF0000000) == Cn::Status_Uncertain; }

/// \details Returns a general indicator that the operation result is incomplete.
inline bool StatusIsProcessing(Cn::StatusCode status) { return status == Status_Processing; }

/// \details
inline ThreadId_t currentThreadId() { return std::this_thread::get_id(); }

/// \details Returns the value of the timer `Cn::Timer_t`.
inline Timer_t GetTimer()
{
#ifdef WIN32
    return GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Get time since system boot
    return static_cast<uint64_t>(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
#endif
}

enum Permissions
{
    Permission_Empty      = 00000,  ///< Empty permissions
    Permission_SetUserId  = 04000,
    Permission_SetGroupId = 02000,
    Permission_StickyBit  = 01000,
    Permission_ReadUser   = 00400,
    Permission_WriteUser  = 00200,
    Permission_ExecUser   = 00100,
    Permission_ReadGroup  = 00040,
    Permission_WriteGroup = 00020,
    Permission_ExecGroup  = 00010,
    Permission_ReadOther  = 00004,
    Permission_WriteOther = 00002,
    Permission_ExecOther  = 00001
};

enum OpenMode
{
    Open_ReadOnly      = 0x0001,
    Open_WriteOnly     = 0x0002,
    Open_ReadWrite     = Open_ReadOnly | Open_WriteOnly,
    Open_Append        = 0x0004,
    Open_Truncate      = 0x0008,
    Open_Text          = 0x0010,
    Open_Unbuffered    = 0x0020,
    Open_NewOnly       = 0x0040,
    Open_ExistingOnly  = 0x0080
};

} // namespace Cn

#endif // CNCORE_H