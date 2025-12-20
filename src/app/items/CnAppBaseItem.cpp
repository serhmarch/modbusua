#include "CnAppBaseItem.h"

CnAppBaseItem::CnAppBaseItem(CnApp *System, const CnString &name) : CnBaseItem(name),
    m_system(System)
{
}
