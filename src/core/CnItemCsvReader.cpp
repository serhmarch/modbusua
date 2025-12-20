#include "CnItemCsvReader.h"

#include <CnUtils.h>

CnItemCsvReader::CnItemCsvReader(const CnString &fileName, CnChar separator) :
    m_separator(separator)
{
    m_file.setFileName(fileName);
    if (!m_file.open(Cn::Open_ReadOnly))
        m_errorString = CnSTR("Error: Unable to open file ")+ fileName;
}

bool CnItemCsvReader::readNext()
{
    if (isEOF()) 
    {
        return false; // EOF or invalid stream
    }

    CnString line;
    if (!m_file.getline(line)) 
    {
        return false; // EOF or error
    }

    trim(line);

    if (line.empty()) 
    {
        return readNext(); // Skip empty lines
    }

    if (!parseKeyValue(line)) 
    {
        readNext();
        //return false; // Error while parsing key-value
    }

    return true;
}

void CnItemCsvReader::trim(CnString& str)
{
    const CnChar* spaces = CnSTR(" \t\r\n");
    size_t start = str.find_first_not_of(spaces);
    if (start == CnString::npos)
    {
        str.clear();
        return;
    }
    size_t end = str.find_last_not_of(spaces, str.length()-1);
    if (start != 0 || end != (str.length()-1))
        str = str.substr(start, end - start + 1);
}

bool CnItemCsvReader::parseKeyValue(CnString& line) 
{
    trim(line);
    CnList<CnString> result;
    CnString currentField;
    bool inQuotes = false;
    size_t i = 0;
    while (i < line.length())
    {
        CnChar ch = line[i];
        if (ch == CnCHR('"')) 
        {
            if (inQuotes && (i + 1 < line.length()) && (line[i + 1] == CnCHR('"')))
            {
                // Two consecutive quotes inside quoted field -> append a single quote
                currentField.push_back(CnCHR('"'));
                ++i; // skip the next quote
            } 
            else 
            {
                // Toggle inQuotes status
                inQuotes = !inQuotes;
            }
        } 
        else if ((ch == m_separator) && !inQuotes) 
        {
            // Separator outside quotes -> end of current field
            result.push_back(currentField);
            currentField.clear();
        } 
        else 
        {
            // Regular character
            currentField.push_back(ch);
        }
        ++i;
    }
    // Add the last field
    result.push_back(currentField);
    if (result.size() == 2)
    {
        m_key = result.front();
        m_value = result.back();
        return true;
    }
    return false;
}