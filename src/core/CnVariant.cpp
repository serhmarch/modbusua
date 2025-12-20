#include "CnVariant.h"

#include "CnUtils.h"
#include "CnTimestamp.h"
#include "CnDateTime.h"

CnVariant::CnVariant()
{
    m_type = vNull;
    m_int64 = 0;
}

CnVariant::CnVariant(bool v)
{
    m_type = vBool;
    m_bool = v;
}

CnVariant::CnVariant(int8_t v)
{
    m_type = vInt8;
    m_int8 = v;
}

CnVariant::CnVariant(uint8_t v)
{
    m_type = vUInt8;
    m_uint8 = v;
}

CnVariant::CnVariant(int16_t v)
{
    m_type = vInt16;
    m_int16 = v;
}

CnVariant::CnVariant(uint16_t v)
{
    m_type = vUInt16;
    m_uint16 = v;
}

CnVariant::CnVariant(int32_t v)
{
    m_type = vInt32;
    m_int32 = v;
}

CnVariant::CnVariant(uint32_t v)
{
    m_type = vUInt32;
    m_uint32 = v;
}

CnVariant::CnVariant(int64_t v)
{
    m_type = vInt64;
    m_int64 = v;
}

CnVariant::CnVariant(uint64_t v)
{
    m_type = vUInt64;
    m_uint64 = v;
}

CnVariant::CnVariant(float v)
{
    m_type = vFloat;
    m_float = v;
}

CnVariant::CnVariant(double v)
{
    m_type = vDouble;
    m_double = v;
}

CnVariant::CnVariant(const CnString &v)
{
    m_type = vString;
    m_string = new CnString(v);
}

CnVariant::CnVariant(const CnChar *v)
{
    m_type = vString;
    m_string = new CnString(v);
}

CnVariant::CnVariant(CnTimestamp v)
{
    m_type = vTimestamp;
    m_timestamp = v;
}

CnVariant::CnVariant(const CnDateTime &v)
{
    m_type = vDateTime;
    m_datetime = new CnDateTime(v);
}

CnVariant::CnVariant(const CnVariant &other)
{
    m_type = other.m_type;
    switch (m_type)
    {
    case vString:
        m_string = new CnString(*other.m_string);
        break;
    case vDateTime:
        m_datetime = new CnDateTime(*other.m_datetime);
        break;
    default:
        m_int64 = other.m_int64;
        break;
    }
}

CnVariant::CnVariant(CnVariant &&other)
{
    m_type = other.m_type;
    m_int64 = other.m_int64;
    other.m_type = vNull;
}

CnVariant::~CnVariant()
{
    clear();
}

void CnVariant::clear()
{
    switch (m_type)
    {
    case vString:
        delete m_string;
        break;
    case vTimestamp:
        m_timestamp.~CnTimestamp();
        break;
    case vDateTime:
        delete m_datetime;
        break;
    }
    m_type = vNull;
    m_int64 = 0;
}

#define VARIANT_CONVERT(Type)           \
    bool okInner = true;                    \
    Type res = 0;                           \
    switch (m_type)                         \
    {                                       \
    case vString:                           \
        return Cn::to_##Type(*m_string, ok);\
    case vBool:                             \
    case vInt8:                             \
    case vUInt8:                            \
        res = static_cast<Type>(m_int8);    \
        break;                              \
    case vInt16:                            \
    case vUInt16:                           \
        res = static_cast<Type>(m_int16);   \
        break;                              \
    case vInt32:                            \
    case vUInt32:                           \
        res = static_cast<Type>(m_int32);   \
        break;                              \
    case vFloat:                            \
        res = static_cast<Type>(m_float);   \
        break;                              \
    case vInt64:                            \
    case vUInt64:                           \
    case vTimestamp:                        \
        res = static_cast<Type>(m_int64);   \
        break;                              \
    case vDouble:                           \
        res = static_cast<Type>(m_double);  \
        break;                              \
    default:                                \
        okInner = false;                    \
        break;                              \
    }                                       \
    if (ok)                                 \
        *ok = okInner;                      \
    return res;

bool CnVariant::toBool(bool *ok) const
{
    VARIANT_CONVERT(bool)
}

int8_t CnVariant::toInt8(bool *ok) const
{
    VARIANT_CONVERT(int8_t)
}

uint8_t CnVariant::toUInt8(bool *ok) const
{
    VARIANT_CONVERT(uint8_t)
}

int16_t CnVariant::toInt16(bool *ok) const
{
    VARIANT_CONVERT(int16_t)
}

uint16_t CnVariant::toUInt16(bool *ok) const
{
    VARIANT_CONVERT(uint16_t)
}

int32_t CnVariant::toInt32(bool *ok) const
{
    VARIANT_CONVERT(int32_t)
}

uint32_t CnVariant::toUInt32(bool *ok) const
{
    VARIANT_CONVERT(uint32_t)
}

int64_t CnVariant::toInt64(bool *ok) const
{
    VARIANT_CONVERT(int64_t)
}

uint64_t CnVariant::toUInt64(bool *ok) const
{
    VARIANT_CONVERT(uint64_t)
}

float CnVariant::toFloat(bool *ok) const
{
    VARIANT_CONVERT(float)
}

double CnVariant::toDouble(bool *ok) const 
{
    VARIANT_CONVERT(double)
}

CnString CnVariant::toString(bool *ok) const
{
    bool okInner = true;   
    CnString res;
    switch (m_type)
    {
    case vBool:
        res = Cn::toString(m_bool);
        break;
    case vInt8:
        res = Cn::toString(m_int8);
        break;
    case vUInt8:
        res = Cn::toString(m_uint8);
        break;
    case vInt16:
        res = Cn::toString(m_int16);
        break;
    case vUInt16:
        res = Cn::toString(m_uint16);
        break;
    case vInt32:
        res = Cn::toString(m_int32);
        break;
    case vUInt32:
        res = Cn::toString(m_uint32);
        break;
    case vInt64:
        res = Cn::toString(m_int64);
        break;
    case vUInt64:
        res = Cn::toString(m_uint64);
        break;
    case vFloat:
        res = Cn::toString(m_float);
        break;
    case vDouble:
        res = Cn::toString(m_double);
        break;
    case vString:
        res = *m_string;
        break;
    case vTimestamp:
        res = CnDateTime(m_timestamp).toString();
        break;
    case vDateTime:
        res = m_datetime->toString();
        break;
    default:
        okInner = false;
        break;
    }
    if (ok)
        *ok = okInner;
    return res;
}

CnTimestamp CnVariant::toTimestamp(bool *ok) const
{
    bool okInner = true;                    
    CnTimestamp res;                           
    switch (m_type)                         
    {                                       
    case vString:                           
        res = Cn::toInt(*m_string, ok); 
        break;                              
    case vBool:                             
    case vInt8:                             
    case vUInt8:                            
        res = m_int8;    
        break;                              
    case vInt16:                            
    case vUInt16:                           
        res = m_int16;   
        break;                              
    case vInt32:                             
    case vUInt32:                            
        res = m_int32;    
        break;                              
    case vFloat:                            
        res = static_cast<uint64_t>(m_float);    
        break;                              
    case vInt64:                         
    case vUInt64:                        
        res = m_int64;    
        break;                              
    case vDouble:                           
        res = static_cast<uint64_t>(m_double);    
        break;                              
    case vTimestamp:                        
        res = m_timestamp;
        break; 
    case vDateTime:
        res = m_datetime->toTimestamp();                             
        break; 
    default:                                
        okInner = false;                    
        break;                              
    }                                       
    if (ok)                                 
        *ok = okInner;                      
    return res;
}

CnDateTime CnVariant::toDateTime(bool *ok) const
{
    switch (m_type)
    {
    case vDateTime:
        if (ok)
            *ok = true;
        return *m_datetime;    
    case vTimestamp:
        if (ok)
            *ok = true;
        return CnDateTime(m_timestamp);    
    default:
        if (ok)
            *ok = false;
        return CnDateTime();
    } 
}

CnVariant &CnVariant::operator=(const CnVariant &other)
{
    clear();
    m_type = other.m_type;
    switch (m_type)
    {
    case vString:
        m_string = new CnString(*other.m_string);
        break;
    case vDateTime:
        m_datetime = new CnDateTime(*other.m_datetime);
        break;
    default:
        m_int64 = other.m_int64;
        break;
    }
    return *this;
}

CnVariant &CnVariant::operator=(CnVariant &&other)
{
    clear();
    m_type = other.m_type;
    m_int64 = other.m_int64;
    other.m_type = vNull;
    return *this;
}
