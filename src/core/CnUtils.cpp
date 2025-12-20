#include "CnUtils.h"

#include <iomanip>
#include <cassert>

#include <CnStd_stdio.h>
#include <CnStd_string.h>
#include <CnStd_sstream.h>

#include <app/CnApp.h>

namespace Cn {

void sendEvent(CnObject *object, CnEvent *event)
{
    CnApp::global()->sendEvent(object, event);
}

void postEvent(CnObject *object, CnEvent *event)
{
    CnApp::global()->postEvent(object, event);
}

bool toBool(const CnChar *s, bool *ok)
{
    if (s == nullptr)
    {
        if (ok)
            *ok = false;
        return false;
    }

    CnChar firstChar = s[0];
    if (firstChar == CnCHR('1') || firstChar == CnCHR('t') || firstChar == CnCHR('T') ||
        firstChar == CnCHR('y') || firstChar == CnCHR('Y'))
    {
        if (ok)
            *ok = true;
        return true;
    }
    else if (firstChar == CnCHR('0') || firstChar == CnCHR('f') || firstChar == CnCHR('F') ||
             firstChar == CnCHR('n') || firstChar == CnCHR('N'))
    {
        if (ok)
            *ok = true;
        return false;
    }
    else
    {
        if (ok)
            *ok = false;
        return false;
    }
}

int8_t toInt8(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    int8_t num = static_cast<int8_t>(CnStd::strtol(s, &end, base) );
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

uint8_t toUInt8(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    uint8_t num = static_cast<uint8_t>(CnStd::strtoul(s, &end, base) );
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

int16_t toInt16(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    int16_t num = static_cast<int16_t>(CnStd::strtol(s, &end, base) );
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

uint16_t toUInt16(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    uint16_t num = static_cast<uint16_t>(CnStd::strtoul(s, &end, base) );
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

int32_t toInt32(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    int32_t num = static_cast<int32_t>(CnStd::strtol(s, &end, base) );
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

uint32_t toUInt32(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    uint32_t num = static_cast<uint32_t>(CnStd::strtoul(s, &end, base) );
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

int64_t toInt64(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    int64_t num = CnStd::strtoll(s, &end, base);
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

uint64_t toUInt64(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    uint64_t num = CnStd::strtoull(s, &end, base);
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

float toFloat(const CnChar *s, bool *ok)
{
    CnChar *end;
    float num = CnStd::strtof(s, &end);
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0.0f;
}

double toDouble(const CnChar *s, bool *ok)
{
    CnChar *end;
    double num = CnStd::strtod(s, &end);
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0.0;
}

int toInt(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    int num = CnStd::strtol(s, &end, base); 
    // TODO: check func if str consists of whitespaces
    // TODO: finish it
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

unsigned int toUInt(const CnChar *s, bool *ok, int base)
{
    CnChar *end;
    unsigned int num = CnStd::strtoul(s, &end, base);
    bool isOk = (*end == CnCHR('\0')) && (end != s);
    if (ok)
        *ok = isOk;
    if (isOk)
        return num;
    return 0;
}

CnString toUpper(const CnChar *s, size_t size)
{
    CnString r(s);
    const CnChar *p = r.data();
    for (size_t i = 0; i < size; i++)
        r[i] = toupper(p[i]);
    return r;
}

CnString toUpper(const CnChar * s)
{
    size_t size = CnStd::strlen(s);
    return toUpper(s, size);
}

CnString toLower(const CnChar *s, size_t size)
{
    CnString r(s);
    const CnChar *p = r.data();
    for (size_t i = 0; i < size; i++)
        r[i] = tolower(p[i]);
    return r;
}

CnString toLower(const CnChar * s)
{
    size_t size = CnStd::strlen(s);
    return toLower(s, size);
}

CnStringList split(const CnString &s, CnChar sep)
{
    CnStringList ls;
    const CnChar *b = s.data();
    const CnChar *p;
    while ((p = CnStd::strchr(b, sep)) != nullptr)
    {
        ptrdiff_t sz = (p - b);
        ls.push_back(CnString(b, sz));
        b = p + 1;
    }
    ls.push_back(b);
    return ls;
}

CnString trim(const CnString &s) 
{
    // Trim leading whitespace
    CnString::const_iterator start = s.begin();
    while (start != s.end() && isspace(*start))
        ++start;

    // Trim trailing whitespace
    CnString::const_iterator end = s.end();
    while (end != start && isspace(*(end - 1))) 
        --end;

    // Construct the trimmed CnString
    return CnString(start, end-start);
}

CnString bytesToString(const uint8_t* bytes, uint16_t size)
{
    CnStd::stringstream ss;
    ss << std::hex << std::setfill(CnCHR('0')) << std::uppercase;

    for (uint16_t i = 0; i < size; ++i) {
        ss << std::setw(2) << static_cast<int>(bytes[i]);
        if (i < size - 1) {
            ss << CnCHR(' '); // Add a whitespace between bytes
        }
    }

    return CnString::fromChars(ss.str().data());
}

CnString toString(double v, CnChar format, int precision)
{
    assert(format == CnCHR('g') 
        || format == CnCHR('f')
        || format == CnCHR('e')
        || format == CnCHR('E'));

    // Format string like "%.6g"
    CnChar fmt[8];
    CnStd::snprintf(fmt, sizeof(fmt), CnSTR("%%.%d%c"), precision, format);

    // Temporary buffer for result
    CnChar buffer[64]; // 64 is enough for typical use cases
    CnStd::snprintf(buffer, sizeof(buffer), fmt, v);

    return CnString(buffer);
}

CnStringList toStringList(const CnString &s)
{
    CnString output;
    CnStringList outputs;
    size_t start = 0;
    size_t end = 0;
    while ((end = s.find_first_of(CnSTR(";, "), start)) != CnString::npos) 
    {
        output = s.substr(start, end - start);
        output.erase(0, output.find_first_not_of(CnCHR(' ')));
        output.erase(output.find_last_not_of(CnCHR(' ')) + 1);
        if (!output.empty())
            outputs.push_back(output);
        start = end + 1;
    }
    output = s.substr(start);
    output.erase(0, output.find_first_not_of(CnCHR(' ')));
    output.erase(output.find_last_not_of(CnCHR(' ')) + 1);
    if (!output.empty())
        outputs.push_back(output);
    return outputs;
}

CnString toString(const CnStringList &ls, CnChar sep)
{
    CnStd::ostringstream oss;
    if (!ls.empty())
    {
        CnStringList::const_iterator it = ls.begin();
        oss << *it;
        for (++it; it != ls.end(); ++it)
            oss << CnCHR(';') << *it;
    }
    return CnString::fromChars(oss.str().data());
}

Cn::StatusCode ModbusToCnStatusCode(Modbus::StatusCode status)
{
    if (Modbus::StatusIsGood(status))
        return Cn::Status_Good;
    if (Modbus::StatusIsBad(status))
        return Cn::Status_Bad;
    return Cn::Status_Uncertain;
}

Color toColor(LogFlag flag)
{
    switch (flag)
    {
    case Log_Error          : return Color_Red;
    case Log_Warning        : return Color_Yellow;
    default:
        return Color_Default;
    }
}

CnString DataSuffixToString(DataSuffix suffix)
{
    switch (suffix)
    {
    case Suffix_Default       : return CnSTR("Default");
    case Suffix_Bool          : return CnSTR("Bool");
    case Suffix_Int16         : return CnSTR("Int16");
    case Suffix_UInt16        : return CnSTR("UInt16");
    case Suffix_Int32         : return CnSTR("Int32");
    case Suffix_UInt32        : return CnSTR("UInt32");
    case Suffix_Int64         : return CnSTR("Int64");
    case Suffix_UInt64        : return CnSTR("UInt64");
    case Suffix_Float         : return CnSTR("Float");
    case Suffix_Double        : return CnSTR("Double");
    case Suffix_SwappedInt32  : return CnSTR("SwappedInt32");
    case Suffix_SwappedUInt32 : return CnSTR("SwappedUInt32");
    case Suffix_SwappedInt64  : return CnSTR("SwappedInt64");
    case Suffix_SwappedUInt64 : return CnSTR("SwappedUInt64");
    case Suffix_SwappedFloat  : return CnSTR("SwappedFloat");
    case Suffix_SwappedDouble : return CnSTR("SwappedDouble");
    case Suffix_ByteArray     : return CnSTR("ByteArray");
    default:
        return CnString();
    }
}

DataSuffix DataTypeToSuffix(DataType dataType)
{
    switch (dataType)
    {
    case Data_Bool  : return Suffix_Bool   ;
    case Data_Int16 : return Suffix_Int16  ;
    case Data_UInt16: return Suffix_UInt16 ;
    case Data_Int32 : return Suffix_Int32  ;
    case Data_UInt32: return Suffix_UInt32 ;
    case Data_Int64 : return Suffix_Int64  ;
    case Data_UInt64: return Suffix_UInt64 ;
    case Data_Int   : return Suffix_Int32  ;
    case Data_UInt  : return Suffix_UInt32 ;
    case Data_Float : return Suffix_Float  ;
    case Data_Double: return Suffix_Double ;
    default         : return Suffix_Default;
    }
}

DataSuffix DataTypeToSwappedSuffix(DataType dataType)
{
    switch (dataType)
    {
    case Data_Int32 : return Suffix_SwappedInt32 ;
    case Data_UInt32: return Suffix_SwappedUInt32;
    case Data_Int64 : return Suffix_SwappedInt64 ;
    case Data_UInt64: return Suffix_SwappedUInt64;
    case Data_Int   : return Suffix_SwappedInt32 ;
    case Data_UInt  : return Suffix_SwappedUInt32;
    case Data_Float : return Suffix_SwappedFloat ;
    case Data_Double: return Suffix_SwappedDouble;
    default         : return Suffix_Default      ;
    }
}

void swap32(const void * src, void * dst)
{
    const uint16_t *ts = reinterpret_cast<const uint16_t*>(src);
    uint16_t *td = reinterpret_cast<uint16_t*>(dst);
    td[0] = ts[1];
    td[1] = ts[0];
}

void swap64(const void * src, void * dst)
{
    const uint16_t *ts = reinterpret_cast<const uint16_t*>(src);
    uint16_t *td = reinterpret_cast<uint16_t*>(dst);
    td[0] = ts[1];
    td[1] = ts[0];
    td[2] = ts[3];
    td[3] = ts[2];
}

CnChar* ModbusMessageName(CnChar* buffer, Modbus::MemoryType mem, uint16_t offset, uint16_t count)
{
    CnStd::snprintf(buffer, CN_MB_MSGNAME_MAX, CnSTR("%i%05hu:%i%05hu"), mem, offset + 1, mem, offset + count);
    return buffer;
}

CnVariant toVariant(const CnTimestamp& v)
{
    return CnVariant(v);
}

} // namespace Cn 