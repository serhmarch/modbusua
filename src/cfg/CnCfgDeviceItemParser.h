/*********************************************************************
 * \file   CnCfgDeviceItemParser.h
 * \brief  Парсер 'item reference' для об'єкта-пристрою/ПЛК
 *
 * \author serhmarch
 * \date   August 2025
 *********************************************************************/
#ifndef CNCFGDEVICEITEMPARSER_H
#define CNCFGDEVICEITEMPARSER_H

#define CN_CDIP_ERROR_SZ 1024

#include <CnItemLexer.h>

#include "CnCfgDeviceItem.h"

class CnDevice;

/*! \brief Parser for device/PLC "item reference" strings.

    \details `CnCfgDeviceItemParser` converts a textual "item reference"
    into a structured `CnCfgDeviceItem`. It uses a tokenizing lexer to
    interpret item kind, offsets, Modbus memory, data suffix, optional
    bit descriptors, message identifier, and polling period. Parsing
    errors are recorded and can be queried after a parse attempt.

    \sa `CnItemLexer`, `CnCfgDeviceItem`
*/
class CnCfgDeviceItemParser
{
public:
    /// \details Constructs the parser instance.
    CnCfgDeviceItemParser();

    /// \details Destroys the parser instance and releases resources.
    ~CnCfgDeviceItemParser();

public:
    /// \details Parses the `itemReference` text and returns a new item.
    CnCfgDeviceItem *parse(const CnChar *itemReference);

    /// \details Convenience overload that accepts `CnString` input.
    inline CnCfgDeviceItem *parse(const CnString &itemReference) { return parse(itemReference.data()); }

    /// \details Returns `true` if the previous parse recorded an error.
    inline bool hasError() const { return m_lastError[0] != '\0'; }

    /// \details Returns a C-string with the last error message.
    inline const CnChar* errorString() const { return m_lastError; }

    /// \details Clears the error flag to prepare for a new parse.
    inline void clearError() { m_lastError[0] = '\0'; }

    /// \details Returns the current lexer position in the input.
    inline int pos() const { return m_lexer.currentToken().pos(); }

private:
    bool tryParseModbusItem();
    bool tryParseModbusItemMessageId();
    bool tryParseModbusItemPeriod();
    bool tryParseDataTypeSuffix();

private:
    CnCfgDeviceItem* createItem();

    const CnChar* currentItemTypeStr() const;

private:
    CnItemLexer m_lexer;

    struct
    {
        CnDeviceBaseItem::ItemType type;
        uint16_t subtype;
        uint16_t offset;
        uint16_t count;
        Cn::DataSuffix suffix;
        CnString messageId;
        int period;

        void clear() 
        {
            type = CnDeviceBaseItem::Item_None;
            count = 0;
            suffix = Cn::Suffix_Default;
            messageId.clear();
            period = -1;
        }
    } m_parsed;

    CnChar m_lastError[CN_CDIP_ERROR_SZ];
};

#endif // CNCFGDEVICEITEMPARSER_H
