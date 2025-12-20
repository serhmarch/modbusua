/*!
 * \file   CnUtils.h
 * \brief  Contains a list of general utility functions.
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNUTILS_H
#define CNUTILS_H

#include <string>
#include <codecvt>
#include <locale>

#include <Modbus.h>

#include "CnCore.h"
#include "CnLog.h"
#include "CnVariant.h"

class CnEvent;
class CnObject;

#define CN_MB_MSGNAME_MAX 14 // '4xxxxx:4xxxxx' : 6 digits + 1(':') + 6 digits + 1('\0')

#define CN_GET_CFGMAP_VALUE(name, method) \
    Cn::Config::const_iterator it;              \
    it = config.find(Strings::instance().name); \
    if (it != config.end())                     \
    {                                           \
        auto v = it->second.method(ok);         \
        return v;                               \
    }                                           \
    else if (ok)                                \
        *ok = false;                            \
    return decltype(it->second.method(ok))();


namespace Cn {

/// \brief Associative array whose element is a 'key'(CnString)-'value'(CnVariant) pair, used for storing and transferring object configurations.
typedef CnHash<CnString, CnVariant> Config;

/// \brief Associative array whose element is a 'key'(CnString)-'value'(Cn::Config) pair, used for storing and transferring group object configurations.
typedef CnHash<CnString, Config> GroupConfig;

/// \details Template function returns the Cn::DataType type corresponding to the given data type T.
template <typename T>
constexpr DataType toDataType() 
{
    return std::is_same_v<T, Modbus::StatusCode > ? Data_ModbusStatus:
           std::is_same_v<T, Cn::StatusCode     > ? Data_CnStatus    :
           std::is_same_v<T, bool               > ? Data_Bool        :
           std::is_same_v<T, int8_t             > ? Data_Int8        :
           std::is_same_v<T, uint8_t            > ? Data_UInt8       :
           std::is_same_v<T, int16_t            > ? Data_Int16       :
           std::is_same_v<T, uint16_t           > ? Data_UInt16      :
           std::is_same_v<T, int32_t            > ? Data_Int32       :
           std::is_same_v<T, uint32_t           > ? Data_UInt32      :
           std::is_same_v<T, int64_t            > ? Data_Int64       :
           std::is_same_v<T, uint64_t           > ? Data_UInt64      :
           std::is_same_v<T, char               > ? Data_Int8        :
           std::is_same_v<T, unsigned char      > ? Data_UInt8       :
           std::is_same_v<T, short              > ? Data_Int16       :
           std::is_same_v<T, unsigned short     > ? Data_UInt16      :
           std::is_same_v<T, long               > ? Data_Int32       :
           std::is_same_v<T, unsigned long      > ? Data_UInt32      :
           std::is_same_v<T, long long          > ? Data_Int64       :
           std::is_same_v<T, unsigned long long > ? Data_UInt64      :
           std::is_same_v<T, int                > ? Data_Int         :
           std::is_same_v<T, unsigned int       > ? Data_UInt        :
           std::is_same_v<T, float              > ? Data_Float       :
           std::is_same_v<T, double             > ? Data_Double      :
           std::is_same_v<T, CnString           > ? Data_String      :
           std::is_same_v<T, CnTimestamp        > ? Data_Timestamp   :
           std::is_same_v<T, CnDateTime         > ? Data_DateTime    :
           std::is_same_v<T, CnVariant          > ? Data_Variant     :
           Data_Unknown;
}

/// \details
void sendEvent(CnObject *object, CnEvent *event);

/// \details
void postEvent(CnObject *object, CnEvent *event);

/// \details Returns a `bool` value converted from the string `s`.
/// Returns `false` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
bool toBool(const CnChar *s, bool *ok = nullptr);

/// \details Returns a `bool` value converted from the string `s`. Same as `toBool(const CnChar*,bool*)`.
inline bool toBool(const CnString &s, bool *ok = nullptr) { return toBool(s.data(), ok); }

/// \details Synonym for `toBool(const CnChar*,bool*)`.
inline bool to_bool(const CnChar *s, bool *ok = nullptr) { return toBool(s, ok); }

/// \details Synonym for `toBool(const CnString&,bool*)`.
inline bool to_bool(const CnString &s, bool *ok = nullptr) { return toBool(s, ok); }

/// \details Returns an `int8_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
int8_t toInt8(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns an `int8_t` value converted from the string `s` using the base `base`. Same as `toInt8(const CnChar*,bool*,int)`.
inline int8_t toInt8(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt8(s.data(), ok, base); }

/// \details Synonym for `toInt8(const CnChar*,bool*,int)`.
inline int8_t to_int8_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toInt8(s, ok, base); }

/// \details Synonym for `toInt8(const CnString&,bool*,int)`.
inline int8_t to_int8_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt8(s, ok, base); }

/// \details Returns a `uint8_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
uint8_t toUInt8(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns a `uint8_t` value converted from the string `s` using the base `base`. Same as `toUInt8(const CnChar*,bool*,int)`.
inline uint8_t toUInt8(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt8(s.data(), ok, base); }

/// \details Synonym for `toUInt8(const CnChar*,bool*,int)`.
inline uint8_t to_uint8_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toUInt8(s, ok, base); }

/// \details Synonym for `toUInt8(const CnString&,bool*,int)`.
inline uint8_t to_uint8_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt8(s, ok, base); }

/// \details Returns an `int16_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
int16_t toInt16(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns an `int16_t` value converted from the string `s` using the base `base`. Same as `toInt16(const CnChar*,bool*,int)`.
inline int16_t toInt16(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt16(s.data(), ok, base); }

/// \details Synonym for `toInt16(const CnChar*,bool*,int)`.
inline int16_t to_int16_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toInt16(s, ok, base); }

/// \details Synonym for `toInt16(const CnString&,bool*,int)`.
inline int16_t to_int16_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt16(s, ok, base); }

/// \details Returns a `uint16_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
uint16_t toUInt16(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns a `uint16_t` value converted from the string `s` using the base `base`. Same as `toUInt16(const CnChar*,bool*,int)`.
inline uint16_t toUInt16(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt16(s.data(), ok, base); }

/// \details Synonym for `toUInt16(const CnChar*,bool*,int)`.
inline uint16_t to_uint16_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toUInt16(s, ok, base); }

/// \details Synonym for `toUInt16(const CnString&,bool*,int)`.
inline uint16_t to_uint16_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt16(s, ok, base); }

/// \details Returns an `int32_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
int32_t toInt32(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns an `int32_t` value converted from the string `s` using the base `base`. Same as `toInt32(const CnChar*,bool*,int)`.
inline int32_t toInt32(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt32(s.data(), ok, base); }

/// \details Synonym for `toInt32(const CnChar*,bool*,int)`.
inline int32_t to_int32_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toInt32(s, ok, base); }

/// \details Synonym for `toInt32(const CnString&,bool*,int)`.
inline int32_t to_int32_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt32(s, ok, base); }

/// \details Returns a `uint32_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
uint32_t toUInt32(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns a `uint32_t` value converted from the string `s` using the base `base`. Same as `toUInt32(const CnChar*,bool*,int)`.
inline uint32_t toUInt32(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt32(s.data(), ok, base); }

/// \details Synonym for `toUInt32(const CnChar*,bool*,int)`.
inline uint32_t to_uint32_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toUInt32(s, ok, base); }

/// \details Synonym for `toUInt32(const CnString&,bool*,int)`.
inline uint32_t to_uint32_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt32(s, ok, base); }

/// \details Returns an `int64_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
int64_t toInt64(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns an `int64_t` value converted from the string `s` using the base `base`. Same as `toInt64(const CnChar*,bool*,int)`.
inline int64_t toInt64(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt64(s.data(), ok, base); }

/// \details Synonym for `toInt64(const CnChar*,bool*,int)`.
inline int64_t to_int64_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toInt64(s, ok, base); }

/// \details Synonym for `toInt64(const CnString&,bool*,int)`.
inline int64_t to_int64_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt64(s, ok, base); }

/// \details Returns a `uint64_t` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
uint64_t toUInt64(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns a `uint64_t` value converted from the string `s` using the base `base`. Same as `toUInt64(const CnChar*,bool*,int)`.
inline uint64_t toUInt64(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt64(s.data(), ok, base); }

/// \details Synonym for `toUInt64(const CnChar*,bool*,int)`.
inline uint64_t to_uint64_t(const CnChar *s, bool *ok = nullptr, int base = 10) { return toUInt64(s, ok, base); }

/// \details Synonym for `toUInt64(const CnString&,bool*,int)`.
inline uint64_t to_uint64_t(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt64(s, ok, base); }

/// \details Returns a `float` value converted from the string `s`.
/// Returns `0.0f` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
float toFloat(const CnChar *s, bool *ok = nullptr);

/// \details Returns a `float` value converted from the string `s`. Same as `toFloat(const CnChar*,bool*)`.
inline float toFloat(const CnString &s, bool *ok = nullptr) { return toFloat(s.data(), ok); }

/// \details Synonym for `toFloat(const CnChar*,bool*)`.
inline float to_float(const CnChar *s, bool *ok = nullptr) { return toFloat(s, ok); }

/// \details Synonym for `toFloat(const CnString&,bool*)`.
inline float to_float(const CnString &s, bool *ok = nullptr) { return toFloat(s, ok); }

/// \details Returns a `double` value converted from the string `s`.
/// Returns `0.0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
double toDouble(const CnChar *s, bool *ok = nullptr);

/// \details Returns a `double` value converted from the string `s`. Same as `toDouble(const CnChar*,bool*)`.
inline double toDouble(const CnString &s, bool *ok = nullptr) { return toDouble(s.data(), ok); }

/// \details Synonym for `toDouble(const CnChar*,bool*)`.
inline double to_double(const CnChar *s, bool *ok = nullptr) { return toDouble(s, ok); }

/// \details Synonym for `toDouble(const CnString&,bool*)`.
inline double to_double(const CnString &s, bool *ok = nullptr) { return toDouble(s, ok); }

/// \details Returns an `int` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
int toInt(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns an `int` value converted from the string `s` using the base `base`. Same as `toInt(const CnChar*,bool*,int)`.
inline int toInt(const CnString &s, bool *ok = nullptr, int base = 10) { return toInt(s.data(), ok, base); }

/// \details Returns an `unsigned int` value converted from the string `s` using the base `base`, which defaults to `10`.
/// Returns `0` if the conversion fails.
/// If `ok` is not `nullptr`, failure is indicated by setting `*ok` to `false`, and success by setting `*ok` to `true`.
unsigned int toUInt(const CnChar *s, bool *ok = nullptr, int base = 10);

/// \details Returns an `unsigned int` value converted from the string `s` using the base `base`. Same as `toUInt(const CnChar*,bool*,int)`.
inline unsigned int toUInt(const CnString &s, bool *ok = nullptr, int base = 10) { return toUInt(s.data(), ok, base); }

/// \details Returns a copy of the first `size` characters of the string `s` in uppercase.
CnString toUpper(const CnChar *s, size_t size);

/// \details Returns a copy of the string `s` in uppercase.
CnString toUpper(const CnChar *s);

/// \details Returns a copy of the string `s` in uppercase.
inline CnString toUpper(const CnString &s) { return toUpper(s.data(), s.size()); }

/// \details Returns a copy of the first `size` characters of the string `s` in lowercase.
CnString toLower(const CnChar *s, size_t size);

/// \details Returns a copy of the string `s` in lowercase.
CnString toLower(const CnChar *s);

/// \details Returns a copy of the string `s` in lowercase.
inline CnString toLower(const CnString &s) { return toLower(s.data(), s.size()); }

/// \details Splits the string `s` into substrings wherever `sep` occurs and returns a list of these strings.
/// If `sep` is not found in the string, `split()` returns a single-element list containing the string.
CnStringList split(const CnString &s, CnChar sep);

/// \details Returns a string from the original string `s` with leading and trailing spaces removed.
CnString trim(const CnString &s);

/// \details Returns a string representation of the byte array `bytes`.
/// Each byte occupies 2 characters in hexadecimal and a single space is placed between bytes.
CnString bytesToString(const uint8_t* bytes, uint16_t size);

/// \details Converts a value of type `CnString` to `std::string` and returns the result.
inline std::string toStdString(const CnString &s) { return s.toStdString(); }

/// \details Converts a value of type `const char *str` to `CnString` and returns the result.
inline CnString toString(const char *str) { return CnString::fromChars(str); }

/// \details Converts a value of type `const char *str` of length `len` to `CnString` and returns the result.
inline CnString toString(const char *str, size_t len) { return CnString::fromChars(str, len); }

/// \details Converts a value of type `const wchar_t *str` to `CnString` and returns the result.
inline CnString toString(const wchar_t *str) { return CnString::fromWChars(str); }

/// \details Converts a value of type `const wchar_t *str` of length `len` to `CnString` and returns the result.
inline CnString toString(const wchar_t *str, size_t len) { return CnString::fromWChars(str, len); }

/// \details Converts a value of type `std::string` to `CnString` and returns the result.
inline CnString toString(const std::string &str) { return toString(str.data(), str.length()); }

/// \details Converts a value of type `std::wstring` to `CnString` and returns the result.
inline CnString toString(const std::wstring &str) { return toString(str.data(), str.length()); }

/// \details Converts a value of type `bool` to `CnString` and returns the result.
inline CnString toString(bool v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `char` to `CnString` and returns the result.
inline CnString toString(char v) { return CnString::fromStdString(std::to_string(v)); }
    
/// \details Converts a value of type `unsigned char` to `CnString` and returns the result.
inline CnString toString(unsigned char v) { return CnString::fromStdString(std::to_string(v)); }
    
/// \details Converts a value of type `short` to `CnString` and returns the result.
inline CnString toString(short v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `unsigned short` to `CnString` and returns the result.
inline CnString toString(unsigned short v) { return CnString::fromStdString(std::to_string(v)); }
    
/// \details Converts a value of type `long` to `CnString` and returns the result.
inline CnString toString(long v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `unsigned long` to `CnString` and returns the result.
inline CnString toString(unsigned long v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `long long` to `CnString` and returns the result.
inline CnString toString(long long v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `unsigned long long` to `CnString` and returns the result.
inline CnString toString(unsigned long long v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `int` to `CnString` and returns the result.
inline CnString toString(int v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `unsigned int` to `CnString` and returns the result.
inline CnString toString(unsigned int v) { return CnString::fromStdString(std::to_string(v)); }

/// \details Converts a value of type `double` to `CnString` and returns the result.
CnString toString(double v, CnChar format = CnCHR('g'), int precision = 6);

/// \details Converts a value of type `float` to `CnString` and returns the result.
inline CnString toString(float v, CnChar format = CnCHR('g'), int precision = 6) { return toString(static_cast<double>(v), format, precision); }

/// \details Converts a string into a list of words split by the delimiters: ';', ',' or ' '.
CnStringList toStringList(const CnString &s);

/// \details Converts a list of words into a string using the separator `sep`.
CnString toString(const CnStringList &ls, CnChar sep = CnCHR(';'));

/// \details Returns a string representation of the last error code.
inline CnString getLastErrorText() { return toString(Modbus::getLastErrorText()); }

/// \details Returns a string representation of the value of type `Modbus::StatusCode`.
Cn::StatusCode ModbusToCnStatusCode(Modbus::StatusCode status);

/// \details Causes the current thread to sleep for `msec` milliseconds.
inline void msleep(uint32_t msec) { Modbus::msleep(msec); }

/// \details Returns the console text color for the flag `flag`.
Color toColor(LogFlag flag);

/// \details Sets the console text color.
void setConsoleColor(Color color);

/// \details Returns a string representation of the value of type `Cn::DataSuffix`.
CnString DataSuffixToString(DataSuffix suffix);

/// \details
DataSuffix DataTypeToSuffix(DataType dataType);

/// \details
DataSuffix DataTypeToSwappedSuffix(DataType dataType);

/// \details Returns a string representation of the value of type `Cn::DataSuffix`. Same as `DataSuffixToString`.
inline CnString toString(DataSuffix suffix) { return DataSuffixToString(suffix); }

/// \details The function changes the order of 16-bit registers in the 32-bit memory area `src` and writes the result to `dst`.
void swap32(const void *src, void *dst);

/// \details The function changes the order of 16-bit registers in the 64-bit memory area `src` and writes the result to `dst`. The order 0,1,2,3 changes to 1,0,3,2.
void swap64(const void *src, void *dst);

/// \details The function changes the order of 16-bit registers in the 32-bit input variable `src` and returns the result.
inline int32_t toSwappedInt32(int32_t src) { int32_t dst; swap32(&src, &dst); return dst; }

/// \details The function changes the order of 16-bit registers in the 32-bit input variable `src` and returns the result. Same as `toSwappedInt32()`.
inline int32_t swapped(int32_t src) { return toSwappedInt32(src); }

/// \details The function changes the order of 16-bit registers in the 32-bit input variable `src` and returns the result.
inline uint32_t toSwappedUInt32(uint32_t src) { uint32_t dst; swap32(&src, &dst); return dst; }

/// \details The function changes the order of 16-bit registers in the 32-bit input variable `src` and returns the result. Same as `toSwappedUInt32()`.
inline uint32_t swapped(uint32_t src) { return toSwappedUInt32(src); }

/// \details The function changes the order of 16-bit registers in the 64-bit input variable `src` and returns the result. The order of changed registers is the same as in the `swap64()` function.
inline int64_t toSwappedInt64(int64_t src) { int64_t dst; swap64(&src, &dst); return dst; }

/// \details The function changes the order of 16-bit registers in the 64-bit input variable `src` and returns the result. The order of changed registers is the same as in the `swap64()` function. Same as `toSwappedInt64()`.
inline int64_t swapped(int64_t src) { return toSwappedInt64(src); }

/// \details The function changes the order of 16-bit registers in the 64-bit input variable `src` and returns the result. The order of changed registers is the same as in the `swap64()` function.
inline uint64_t toSwappedUInt64(uint64_t src) { uint64_t dst; swap64(&src, &dst); return dst; }

/// \details The function changes the order of 16-bit registers in the 64-bit input variable `src` and returns the result. The order of changed registers is the same as in the `swap64()` function. Same as `toSwappedUInt64()`.
inline uint64_t swapped(uint64_t src) { return toSwappedUInt64(src); }

/// \details The function changes the order of 16-bit registers in the 32-bit input variable `src` and returns the result.
inline float toSwappedFloat(float src) { float dst; swap32(&src, &dst); return dst; }

/// \details The function changes the order of 16-bit registers in the 32-bit input variable `src` and returns the result. Same as `toSwappedFloat()`.
inline float swapped(float src) { return toSwappedFloat(src);  }

/// \details The function changes the order of 16-bit registers in the 64-bit input variable `src` and returns the result. The order of changed registers is the same as in the `swap64()` function.
inline double toSwappedDouble(double src) { double dst; swap64(&src, &dst); return dst; }

/// \details The function changes the order of 16-bit registers in the 64-bit input variable `src` and returns the result. The order of changed registers is the same as in the `swap64()` function. Same as `toSwappedDouble()`.
inline double swapped(double src) { return toSwappedDouble(src); }

/// \details The function converts the `offset` of a 4x register to an integer format representation of a 4x register and returns the result, e.g. 0 is converted to 400001, etc.
inline uint32_t toModbus4xAddress(uint16_t offset) { return 400001 + offset; }

/// \details The function converts the integer format representation `offset` of a 4x register to the offset of a 4x register and returns the result, e.g. 400001 is converted to 0, etc.
inline uint16_t toModbus4xOffset(uint32_t address) { return static_cast<uint16_t>(address-400001); } // TODO: check bounds

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(bool v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(int8_t v) { return CnVariant(static_cast<char>(v)); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(uint8_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(int16_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(uint16_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(int32_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(uint32_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(int64_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(uint64_t v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(float v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(double v) { return CnVariant(v); }

/// \details The function converts the input argument `v` to a value of type `CnVariant` and returns the result.
inline CnVariant toVariant(const CnString &v) { return CnVariant(v.data()); }

/// \details Функція перетворює вхідний аргумент `v` на значення типу `CnVariant` і повертає результат.
inline CnVariant toVariant(Modbus::StatusCode v)  { return CnVariant(static_cast<int>(v)); }

/// \details Функція перетворює вхідний аргумент `v` на значення типу `CnVariant` і повертає результат.
CnVariant toVariant(const CnTimestamp& v);

/// \details Template function (used as a general one for all enumeration types) converts the input argument `var` of type `CnVariant` to a value of enumeration type Enum of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
template <typename Enum>
inline bool fromVariantEnum(const CnVariant& var, Enum *dst)
{
    bool ok;
    int v = var.toInt(&ok);
    if (ok)
        *dst = static_cast<Enum>(v);
    return ok;
}

#define FROM_VARIANT(Type, fConvert)\
{                                   \
    bool ok;                        \
    Type v = var.fConvert(&ok);     \
    if (ok)                         \
        *dst = v;                   \
    return ok;                      \
}

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c bool of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, bool *dst) FROM_VARIANT(bool, toBool)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c int8_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant& var, int8_t *dst) FROM_VARIANT(int8_t, toInt8)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c uint8_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, uint8_t *dst) FROM_VARIANT(uint8_t, toUInt8)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c int16_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, int16_t *dst) FROM_VARIANT(int16_t, toInt16)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c uint16_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, uint16_t *dst) FROM_VARIANT(uint16_t, toUInt16)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c int32_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, int32_t *dst) FROM_VARIANT(int32_t, toInt32)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c uint32_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, uint32_t *dst) FROM_VARIANT(uint32_t, toUInt32)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c int64_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, int64_t *dst) FROM_VARIANT(int64_t, toInt64)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c uint64_t of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, uint64_t *dst) FROM_VARIANT(uint64_t, toUInt64)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c float of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, float *dst) FROM_VARIANT(float, toFloat)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c double of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, double *dst) FROM_VARIANT(double, toDouble)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c CnString of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant& var, CnString *dst)  FROM_VARIANT(CnString, toString)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c CnTimestamp of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant& var, CnTimestamp *dst) FROM_VARIANT(CnTimestamp, toTimestamp)

/// \details The function converts the input argument `var` of type `CnVariant` to a value of type \c Modbus::StatusCode of the output pointer argument and stores the result in `dst`.
/// \return `true` if the conversion is successful, `false` otherwise.
inline bool fromVariant(const CnVariant &var, Modbus::StatusCode *dst)  { return fromVariantEnum(var, dst); }

/// \details Based on the arguments `mem`, `offset`, `count`, a readable identifier for the Modbus packet is created. The result is stored in the buffer `buffer`.
/// \return Pointer to the buffer `buffer`.
CnChar* ModbusMessageName(CnChar* buffer, Modbus::MemoryType mem, uint16_t offset, uint16_t count);

/// \details Returns the current timestamp.
inline CnTimestamp currentTimestamp() { return CnTimestamp::current(); }

} // namespace Cn

#endif // CNUTILS_H