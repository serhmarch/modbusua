/*!
 * \file   CnCfgBuilder.h
 * \brief  
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNCFGBUILDER_H
#define CNCFGBUILDER_H

#include <CnGlobal.h>
#include <CnConfReader.h>

class CnConfReader;
class CnItemCsvReader;
class CnCfgProject;
class CnCfgPort;
class CnCfgDevice;
class CnCfgDeviceItemParser;

/*! \brief Project configuration builder for `modbusua`.

    \details `CnCfgBuilder` reads configuration sources, constructs
    a `CnCfgProject`, and populates ports, devices, and their items.
    It manages grouped settings (`Cn::GroupConfig`), tracks errors,
    and caches file readers to assemble a consistent configuration
    tree.

    \sa `CnCfgProject`, `CnCfgPort`, `CnCfgDevice`, `CnCfgDeviceItemParser`
*/
class CnCfgBuilder
{
public:
    struct Strings
    {
        const CnString NullString ; ///< 
        const CnString KeyInclude ; ///< 
        const CnString ItemPrefix ; ///< 
        const CnString KeyItemFile; ///< 

        /// \details Initializes common string constants used by the builder.
        Strings();

        /// \details Returns the singleton instance of string constants.
        static const Strings& instance();
    };

public:
    /// \details Constructs a configuration builder.
    CnCfgBuilder();
    /// \details Destroys the configuration builder and releases resources.
    ~CnCfgBuilder();

public:
    /// \details Loads a project configuration by name and returns the root.
    CnCfgProject *load(const CnString &name);

    /// \details Returns `true` if an error has been recorded.
    inline bool hasError() const { return m_errorString.size(); }

    /// \details Returns the last error message collected during loading.
    inline const CnString& errorString() const { return m_errorString; }
    
    /// \details Clears the recorded error state and message.
    inline void clearError() { m_errorString.clear(); }

private:
    void fillProject(CnCfgProject *project);
    Cn::Config readKeyValuePairs();
    inline void takeGroupConfig(Cn::GroupConfig &groups) { groups.swap(m_groups); }
    void addConfReader(const CnString &fileName);
    CnString absolutePath(const CnString &fileName) const;

private:
    bool parseDeviceItem(CnCfgDevice *dev, const CnString &itemName, const CnString &itemReference);
    CnConfReader *currentReader() const;
    CnConfReader::Token readerReadNext();
    CnConfReader::Token readerCurrentToken() const;
    const CnChar* readerName() const;
    const CnString& readerGroupName() const;
    const CnString& readerKey() const;
    const CnString& readerValue() const;
    const CnString& readerErrorString() const;
    bool readerHasError() const;

private:
    void clearCache();

private:
    bool validateName(const CnString &name);

private:
    CnList<CnConfReader*> m_readers;
    Cn::GroupConfig m_groups;
    CnString m_errorString;
    CnCfgDeviceItemParser* m_deviceItemParser;
};

#endif // CNCFGBUILDER_H
