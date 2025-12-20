#include "CnCfgBuilder.h"

#include <CnConfReader.h>
#include <CnItemCsvReader.h>

#include <app/CnApp.h>
#include "CnCfgProject.h"
#include "CnCfgPort.h"
#include "CnCfgDevice.h"
#include "CnCfgDeviceItemParser.h"
#include <CnLog.h>
#include <CnFileInfo.h>

#include <port/CnPortConfig.h>
#include <device/CnDeviceConfig.h>

CnCfgBuilder::Strings::Strings() :
    NullString(CnString()),
    KeyInclude(CnSTR("include")),
    ItemPrefix(CnSTR("i:")),
    KeyItemFile(CnSTR("itemfile"))

{
}

const CnCfgBuilder::Strings &CnCfgBuilder::Strings::instance()
{
    static const Strings s;
    return s;
}

CnCfgBuilder::CnCfgBuilder()
{
    m_deviceItemParser = new CnCfgDeviceItemParser();
}

CnCfgBuilder::~CnCfgBuilder()
{
    delete m_deviceItemParser;
    clearCache();
}

CnCfgProject *CnCfgBuilder::load(const CnString &name)
{
    clearCache();
    addConfReader(name);
    CnCfgProject *project = new CnCfgProject();
    fillProject(project);
    if (this->hasError())
    {
        delete project;
        return nullptr;
    }
    project->swapGroupConfig(m_groups);    
    return project;
}

void CnCfgBuilder::fillProject(CnCfgProject *project)
{
    const Strings &sConst = Strings::instance();
    bool run = true;
    CnConfReader::Token token = readerReadNext();
    while (run)
    {
        switch (token)
        {
        case CnConfReader::Group:
        {
            CnString us = Cn::toUpper(Cn::trim(readerGroupName()));
            if (us == CnSTR("SYSTEM") || us == CnSTR("PROJECT"))
            {
                Cn::Config cfg;
                while (readerReadNext() == CnConfReader::KeyValuePair)
                {
                    if (readerKey() == sConst.KeyInclude)
                        addConfReader(readerValue());
                    else
                        cfg[readerKey()] = readerValue();
                }
                project->setConfig(cfg);
            }
            // Note: `Port` section can have name [Port], [PORT1], [port_A], [Port.Main], etc. 
            else if (us.starts_with(CnSTR("PORT")))
            {
                Cn::Config cfg;
                while (readerReadNext() == CnConfReader::KeyValuePair)
                {
                    if (readerKey() == sConst.KeyInclude)
                        addConfReader(readerValue());
                    else
                        cfg[readerKey()] = readerValue();
                }
                CnString name = Cn::trim(CnPortConfig::getCfgName(cfg));
                if (!validateName(name))
                {
                    CN_LOG_Warning(CnSTR("Incorrect port name '%s': %s"),
                                    name.data(),
                                    errorString().data());
                    clearError();
                    continue;
                }
                if (project->port(name))
                {
                    CN_LOG_Warning(CnSTR("Port with name '%s' is already exists"),
                                    name.data());
                    continue;
                }
                CnCfgPort *port = new CnCfgPort();
                port->setConfig(cfg);
                port->setName(name);
                project->portAdd(port);
            }
            // Note: `Device` section can have name [Device], [DEVICE1], [device_A], [Device.Main], etc. 
            else if (us.starts_with(CnSTR("DEVICE")))
            {
                Cn::Config cfg;
                CnList<std::pair<CnString,CnString> > items;
                CnStringList itemFiles;
                while (readerReadNext() == CnConfReader::KeyValuePair)
                {
                    CnString key = readerKey();
                    if (key == sConst.KeyInclude)
                        addConfReader(readerValue());
                    else if (key.compare(0, sConst.ItemPrefix.size(), sConst.ItemPrefix) == 0)
                    {
                        std::pair<CnString,CnString> item;
                        item.first = Cn::trim(key.substr(sConst.ItemPrefix.size()));
                        item.second = readerValue();
                        items.push_back(item);
                    }
                    else if (key == sConst.KeyItemFile)
                    {
                        const CnString& rawFileName = readerValue();
                        CnString file = this->absolutePath(rawFileName);
                        if (file.size())
                        {
                            itemFiles.push_back(file);
                        }
                        else
                            CN_LOG_Warning(CnSTR("File '%s'. Can't find itemfile '%s'"),
                                            readerName(),
                                            rawFileName.data());
                    }
                    else
                        cfg[key] = readerValue();
                }
                CnString name = Cn::trim(CnDeviceConfig::getCfgName(cfg));
                if (!validateName(name))
                {
                    CN_LOG_Warning(CnSTR("Incorrect device name '%s': %s"),
                                    name.data(),
                                    errorString().data());
                    clearError();
                    continue;
                }
                if (project->device(name))
                {
                    CN_LOG_Warning(CnSTR("Device with name '%s' is already exists"),
                                    name.data());
                    continue;
                }
                CnCfgDevice *dev = new CnCfgDevice();
                dev->setConfig(cfg);
                dev->setName(name);
                project->deviceAdd(dev);
                for (const auto& item : items)
                {
                    parseDeviceItem(dev, item.first, item.second);
                }
                for (const CnString& file : itemFiles)
                {
                    CnItemCsvReader csv(file);
                    if (csv.isOpen())
                    {
                        while (csv.readNext())
                            parseDeviceItem(dev, csv.key(), csv.value());
                    }
                    else
                        CN_LOG_Warning(CnSTR("Can't open itemfile '%s'"),
                                        file.data());
                }
            }
            else
            {
                m_groups[us] = readKeyValuePairs();
            }
        }
            break;
        case CnConfReader::EndOfFile:
            run = false;
            break;
        default:
            token = readerReadNext();
            continue;
        }
        token = readerCurrentToken();
    }
}

Cn::Config CnCfgBuilder::readKeyValuePairs()
{
    Cn::Config cfg;
    while (readerReadNext() == CnConfReader::KeyValuePair)
    {
        cfg[readerKey()] = readerValue();
    }
    return cfg;
}

void CnCfgBuilder::addConfReader(const CnString &fileName)
{
    CnString absfile = this->absolutePath(fileName);
    if (absfile.size()) 
    {
        CnConfReader *reader = new CnConfReader(absfile);
        m_readers.push_front(reader);
    }
    else
        CN_LOG_Warning("File '%s'. Can't find include file '%s'",
                       readerName(),
                       fileName.data());
}

CnString CnCfgBuilder::absolutePath(const CnString &fileName) const
{
    CnFileInfo fi(fileName);
    if (fi.exists() && fi.isAbsolute())
        return fileName;
    for (const CnConfReader *reader : m_readers)
    {
        CnString absfile = reader->dir().absoluteFilePath(fileName);
        if (CnFileInfo::exists(absfile))
            return absfile;
    }
    CnString absfile = CnApp::confDir().absoluteFilePath(fileName);
    if (CnFileInfo::exists(absfile))
        return absfile;
    return CnString();
}

bool CnCfgBuilder::parseDeviceItem(CnCfgDevice *dev, const CnString &itemName, const CnString &itemReference)
{
    CnString name = Cn::trim(itemName);
    if (!validateName(name))
    {
        CN_LOG_Warning(CnSTR("Device '%s'. Incorrect name '%s': %s"),
                        dev->name().data(),
                        itemName.data(), 
                        errorString().data());
        clearError();
        return false;
    }
    if (dev->hasItem(name))
    {
        CN_LOG_Warning(CnSTR("Device '%s'. Item with name '%s' already exists"),
                        dev->name().data(), 
                        name.data());
        return false;
    }
    CnCfgDeviceItem *item = m_deviceItemParser->parse(itemReference);
    if (!item)
    {
        CN_LOG_Warning(CnSTR("Device '%s'. Error parsing '%s' (pos=%d): %s"),
                        dev->name().data(),
                        name.data(), 
                        m_deviceItemParser->pos(),                                                                                      
                        m_deviceItemParser->errorString());
        return false;
    }
    item->setName(name);
    dev->addItem(item);
    return true;

}

CnConfReader *CnCfgBuilder::currentReader() const
{
    if (m_readers.size())
        return m_readers.front();
    return nullptr;
}

CnConfReader::Token CnCfgBuilder::readerReadNext()
{
    if (m_readers.size())
    {
        CnConfReader *reader = m_readers.front();
        CnConfReader::Token token = reader->readNext();
        if (reader->hasError())
        {
            m_errorString = CnSTR("File '") + 
                            reader->fileName() +
                            CnSTR("'(line=") +
                            Cn::toString(reader->row()) +
                            CnSTR(",col=") + 
                            Cn::toString(reader->col()) +
                            CnSTR("): ") +
                            reader->errorString();
            return CnConfReader::EndOfFile;
        }
        if (token == CnConfReader::EndOfFile)
        {
            m_readers.pop_front();
            delete reader;
            if (m_readers.size())
                return m_readers.front()->readNext();
        }
        return token;
    }
    return CnConfReader::EndOfFile;
}

CnConfReader::Token CnCfgBuilder::readerCurrentToken() const
{
    if (m_readers.size())
        return m_readers.front()->currentToken();
    return CnConfReader::EndOfFile;
}

const CnChar *CnCfgBuilder::readerName() const
{
    if (m_readers.size())
        return m_readers.front()->fileName().data();
    return "";
}

const CnString& CnCfgBuilder::readerGroupName() const
{
    if (m_readers.size())
        return m_readers.front()->groupName();
    return Strings::instance().NullString;
}


const CnString& CnCfgBuilder::readerKey() const
{
    if (m_readers.size())
        return m_readers.front()->key();
    return Strings::instance().NullString;
}

const CnString& CnCfgBuilder::readerValue() const
{
    if (m_readers.size())
        return m_readers.front()->value();
    return Strings::instance().NullString;
}

const CnString &CnCfgBuilder::readerErrorString() const
{
    if (m_readers.size())
        return m_readers.front()->errorString();
    return Strings::instance().NullString;
}

bool CnCfgBuilder::readerHasError() const
{
    if (m_readers.size())
        return m_readers.front()->hasError();
    return false;
}

void CnCfgBuilder::clearCache()
{
    m_errorString.clear();
    for (auto reader : m_readers)
        delete reader;
    m_readers.clear();
}

bool CnCfgBuilder::validateName(const CnString &s)
{
    if (s.empty())
    {
        m_errorString = CnSTR("Name is empty");
        return false;
    }

    // First char must be letter or underscore
    if (!(std::isalpha(s[0]) || s[0] == '_'))
    {
        m_errorString = CnSTR("Name must begins with '_' (underscore) or letter");
        return false;
    }

    // Remaining chars can be alnum or underscore
    for (size_t i = 1; i < s.size(); ++i)
    {
        if (!(std::isalnum(s[i]) || s[i] == '_' || s[i] == '.'))
        {
            m_errorString = CnSTR("Name must contains only '_' (underscores), '.' (points), letters or digits");
            return false;
        }
    }

    return true;
}
