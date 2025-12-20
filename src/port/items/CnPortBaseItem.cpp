#include "CnPortBaseItem.h"

CnPortBaseItem::CnPortBaseItem(CnPort *port, const CnString &name) : CnBaseItem(name),
    m_port(port)
{
    //m_port->IncrementRef(); Note: no need to hold ref because system must guarantee that item delete before port
}
