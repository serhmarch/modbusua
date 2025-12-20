#include "CnBaseItem.h"

CnBaseItem::CnBaseItem(const CnString &name) :
    m_name(name)
{
    m_context = nullptr;    
}

CnBaseItem::~CnBaseItem()
{
}

Cn::Access CnBaseItem::access() const
{
    return Cn::Access_Read;
}

void CnBaseItem::update(const CnVariant &value, Cn::StatusCode status, CnTimestamp timestamp)
{
    // Note: base implementation do nothing
}
