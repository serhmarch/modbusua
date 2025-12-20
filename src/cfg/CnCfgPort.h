/*!
 * \file   CnCfgPort.h
 * \brief  
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNCFGPORT_H
#define CNCFGPORT_H

#include "CnCfgBase.h"

/*! \brief Communication port configuration.

    \details `CnCfgPort` models a logical communication port inside the
    project and inherits common fields from `CnCfgBase`. It is used by
    `CnCfgProject` to bind devices to the appropriate port.

    \sa `CnCfgProject`, `CnCfgDevice`
*/
class CnCfgPort : public CnCfgBase
{
public:
    using CnCfgBase::CnCfgBase;
};

typedef CnSharedPointer<CnCfgPort> CnCfgPortPtr;

#endif // CNCFGPORT_H