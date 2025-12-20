/*!
 * \file   CnCfgBase.h
 * \brief  
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNCFGBASE_H
#define CNCFGBASE_H

#include <CnGlobal.h>

/*! \brief Base class for project configuration entities.

    \details `CnCfgBase` provides common fields and interfaces for
    configuration objects: a human-readable `name` and key-value
    settings stored in `Cn::Config`. It serves as the parent class for
    higher-level configuration types such as `CnCfgProject`,
    `CnCfgPort`, and `CnCfgDevice`.

    \sa `CnCfgProject`, `CnCfgPort`, `CnCfgDevice`
*/
class CnCfgBase
{
    CN_REF_COUNTING

public:
    /// \details Default constructor. Initializes an empty configuration base.
    CnCfgBase();

public:
    /// \details Returns the human-readable configuration name.
    inline const CnString &name() const { return m_name; }
    /// \details Sets the human-readable configuration name.
    inline void setName(const CnString &name) { m_name = name; }

public:
    /// \details Returns key-value settings associated with this object.
    virtual const Cn::Config &config() const;
    /// \details Sets key-value settings associated with this object.
    virtual void setConfig(const Cn::Config &config);

protected:
    CnString m_name;
    Cn::Config m_config;
};

#endif // CNCFGBASE_H