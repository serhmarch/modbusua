#include "CnCfgDeviceItemParser.h"

#include <cctype>

#include <CnStd_string.h>
#include <CnStd_stdio.h>

CnCfgDeviceItemParser::CnCfgDeviceItemParser()
{
    clearError();
}

CnCfgDeviceItemParser::~CnCfgDeviceItemParser()
{
}

CnCfgDeviceItem *CnCfgDeviceItemParser::parse(const CnChar *itemReference)
{
    clearError();
    m_parsed.clear();
    if (!m_lexer.parse(itemReference))
    {
        CnStd::strncpy_s(m_lastError, CN_CDIP_ERROR_SZ, m_lexer.errorString().data(), m_lexer.errorString().size());
        return nullptr;
    }
    if (tryParseModbusItem())
    {
        if (hasError())
            return nullptr;
    }
    else
    {
        const CnString &ref = m_lexer.source();
        if (ref.size())
            CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Unknown item reference: %s"), ref.data());
        else
            CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("No (empty) item reference"));
        return nullptr;
    }
    if (!m_lexer.currentToken().isEOL())
    {
        CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing item reference: Some extra-symbols at end of the ItemReference"));
        return nullptr;
    }
    return createItem();
}

bool CnCfgDeviceItemParser::tryParseModbusItem()
{
    if (m_lexer.currentToken().isModbus())
    {
        m_parsed.type = CnDeviceBaseItem::Item_Modbus;
        m_parsed.subtype = m_lexer.currentToken().Modbus() / 100000; // Memory Address Type {T}={0,1,3,4}
        m_parsed.offset = m_lexer.currentToken().Modbus() % 100000 - 1;
        m_lexer.nextToken(); // move to next token for further parsing
        if (m_lexer.currentToken().Symbol() == CnCHR('-'))
        {
            // --- parsing {T}yyyyy-end Modbus-address
            if (!m_lexer.nextToken().isModbus())
            {
                CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item Array: item reference must have format '{T}xxxxx-{T}yyyyy <arraysuffix>[, msgid=<msgid>]'. {T}yyyyy-address is missing"));
                return true;
            }
            // --- check {T}yyyyy-end Modbus-address correctness
            uint32_t mb = m_lexer.currentToken().Modbus();
            uint16_t mb_type = mb / 100000;
            if (mb_type != m_parsed.subtype)
            {
                CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item Array: item reference must have format '{T}xxxxx-{T}yyyyy <arraysuffix>[, msgid=<msgid>]'. {T}xxxxx-address type must be equal to {T}yyyyy-address type"));
                return true;
            }
            uint16_t mb_offset = static_cast<uint16_t>(mb % 100000 - 1);
            if (m_parsed.offset > mb_offset)
            {
                CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item Array: item reference must have format '{T}xxxxx-{T}yyyyy <arraysuffix>[, msgid=<msgid>]'. {T}xxxxx-address must be less or equal to {T}yyyyy-address"));
                return true;
            }
            // --- parsing <arraysuffix>
            if (!m_lexer.nextToken().isWord())
            {
                CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item Array: item reference must have format '{T}xxxxx-{T}yyyyy <arraysuffix>[, msgid=<msgid>]'. <arraysuffix> is missing"));
                return true;
            }
            if (m_lexer.currentToken().Word() == CnSTR("B"))
            {
                m_parsed.suffix = Cn::Suffix_ByteArray;
                m_parsed.count = mb_offset - m_parsed.offset + 1;
                m_lexer.nextToken();
            }
            else
            {
                CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item Array: item reference must have format '{T}xxxxx-{T}yyyyy <arraysuffix>[, msgid=<msgid>]'. Unknown <arraysuffix>: %s"), m_lexer.currentToken().Word().data());
                return true;
            }
        }
        else if (tryParseDataTypeSuffix() && hasError())
            return true;
        while (m_lexer.currentToken().isSymbolComma())
        {
            m_lexer.nextToken(); // move to next token for further parsing
            if (!tryParseModbusItemMessageId() && !tryParseModbusItemPeriod())
            {
                CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item: item reference must have format '{T}xxxxx[-{T}yyyyy <arraysuffix>|<suffix>][, {msgid=<msgid>|<period>}]*'. There is must be 'msgid'-keyword after comma"));
                return true;
            }
            if (hasError())
                return true;
        }
        if (!m_lexer.currentToken().isEOL())
        {
            CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing Modbus Item: item reference must have format '{T}xxxxx[-{T}yyyyy <arraysuffix>|<suffix>][, msgid=<msgid>]'"));
            return true;
        }
        return true;
    }
    return false;
}

bool CnCfgDeviceItemParser::tryParseModbusItemMessageId()
{
    if (m_lexer.currentToken().Word() == CnSTR("MSGID"))
    {
        if (m_lexer.nextToken().Symbol() != CnCHR('='))
        {
            CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing ModbusItem message Id: item reference must have format '{T}xxxxx [<suffix>][, msgid=<msgid>]'. There is no '='-symbol after 'msgid'-keyword"));
            return true;
        }
        if (m_lexer.nextToken().isInt())
            m_parsed.messageId = Cn::toString(m_lexer.currentToken().Int());
        else if (m_lexer.currentToken().isWord())
            m_parsed.messageId = m_lexer.currentToken().Word();
        else
        {
            CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Parsing ModbusItem message Id: item reference must have format '{T}xxxxx [<suffix>][, msgid=<msgid>]'. Unknown <msgid>"));
            return true;
        }
        m_lexer.nextToken(); // move to next token for further parsing
        return true;
    }
    return false;
}

bool CnCfgDeviceItemParser::tryParseModbusItemPeriod()
{
    if (m_lexer.currentToken().isInt())
    {
        m_parsed.period = m_lexer.currentToken().Int();
        m_lexer.nextToken(); // move to next token for further parsing
        return true;
    }
    return false;
}

bool CnCfgDeviceItemParser::tryParseDataTypeSuffix()
{
    if (m_lexer.currentToken().isWord())
    {
        const CnString &suffix = m_lexer.currentToken().Word();
        if      (suffix == CnSTR("S")  ) m_parsed.suffix = Cn::Suffix_Int16;
        else if (suffix == CnSTR("R")  ) m_parsed.suffix = Cn::Suffix_UInt16;
        else if (suffix == CnSTR("I")  ) m_parsed.suffix = Cn::Suffix_Int32;
        else if (suffix == CnSTR("L")  ) m_parsed.suffix = Cn::Suffix_Int32;
        else if (suffix == CnSTR("U")  ) m_parsed.suffix = Cn::Suffix_UInt32;
        else if (suffix == CnSTR("LL") ) m_parsed.suffix = Cn::Suffix_Int64;
        else if (suffix == CnSTR("UL") ) m_parsed.suffix = Cn::Suffix_UInt64;
        else if (suffix == CnSTR("F")  ) m_parsed.suffix = Cn::Suffix_Float;
        else if (suffix == CnSTR("LF") ) m_parsed.suffix = Cn::Suffix_Double;
        else if (suffix == CnSTR("IS") ) m_parsed.suffix = Cn::Suffix_SwappedInt32;
        else if (suffix == CnSTR("LS") ) m_parsed.suffix = Cn::Suffix_SwappedInt32;
        else if (suffix == CnSTR("US") ) m_parsed.suffix = Cn::Suffix_SwappedUInt32;
        else if (suffix == CnSTR("LLS")) m_parsed.suffix = Cn::Suffix_SwappedInt64;
        else if (suffix == CnSTR("ULS")) m_parsed.suffix = Cn::Suffix_SwappedUInt64;
        else if (suffix == CnSTR("FS") ) m_parsed.suffix = Cn::Suffix_SwappedFloat;
        else if (suffix == CnSTR("LFS")) m_parsed.suffix = Cn::Suffix_SwappedDouble;
        else
        {
            CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Unknown suffix: %s"), suffix.data());
            return true;
        }
        m_lexer.nextToken(); // move to next token for further parsing
        return true;
    }
    return false;
}

CnCfgDeviceItem *CnCfgDeviceItemParser::createItem()
{
    CnCfgDeviceItem *item = nullptr;
    switch (m_parsed.type)
    {
    case CnDeviceBaseItem::Item_Modbus:
        item = new CnCfgDeviceItem(m_parsed.type, m_parsed.suffix);
        item->setMemoryType(static_cast<Modbus::MemoryType>(m_parsed.subtype));
        item->setOffset(m_parsed.offset);
        item->setCount(m_parsed.count);
        item->setPeriod(m_parsed.period);
        item->setMessageId(m_parsed.messageId);
        //if (!item)
        //    CnStd::snprintf(m_lastError, CN_CDIP_ERROR_SZ, CnSTR("Unapplied DataType for Modbus ")+Cn::toString(m_parsed.subtype)+CnSTR("x-Item: ") + Cn::DataSuffixToString(m_parsed.suffix);
        break;
    default:
        return nullptr;
    }
    item->setItemReference(m_lexer.source());
    return item;
}

const CnChar *CnCfgDeviceItemParser::currentItemTypeStr() const
{
    switch (m_parsed.type)
    {
    case CnDeviceBaseItem::Item_Modbus              : return CnSTR("Modbus");
    case CnDeviceBaseItem::Item_Command             : return CnSTR("Command");
    case CnDeviceBaseItem::Item_Setting             : return CnSTR("Setting");
    case CnDeviceBaseItem::Item_Statistic           : return CnSTR("Statistic");
    case CnDeviceBaseItem::Item_Config              : return CnSTR("Config");
    default:
        return CnSTR("None");
    }        
}
