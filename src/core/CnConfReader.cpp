#include "CnConfReader.h"

#include <stdio.h>
#include <ctype.h>

#include <CnFileInfo.h>

#include "CnUtils.h"

#define CN_CONFREADER_EOF (std::char_traits<char>::eof())

CnConfReader::CnConfReader(const CnString &fileName) : 
    m_currentToken(EndOfFile)
{
    CnFileInfo fi(fileName);
    m_row = 1;
    m_col = 0;
    m_file.setFileName(fileName);
    m_dir.setPath(fi.dirPath());
    if (!m_file.open(Cn::Open_ReadOnly))
        m_errorString = CnSTR("Error: Unable to open file ")+ fileName;
    else
        nextChar();
}

CnConfReader::~CnConfReader()
{
    if (isOpen())
        m_file.close();
}

CnConfReader::Token CnConfReader::readNext()
{
    std::string buffer;

    passSpace();

    if (isEOF())
    {
        m_currentToken = EndOfFile;
        return m_currentToken;
    }
    else if (m_ch == CnCHR('['))
    {
        // Parse a group/section header, e.g. [GroupName]
        while (nextChar() != CN_CONFREADER_EOF && m_ch != CnCHR(']'))
        {
            if (isEndOfLine())
            {
                m_errorString = CnSTR("Error: Group/section name must be finished with ']'");
                m_currentToken = EndOfFile;
                return m_currentToken;
            }
            buffer += m_ch;
        }

        if (m_ch != CnCHR(']'))
        {
            m_errorString = CnSTR("Error: Group/section name must be finished with ']'");
            m_currentToken = EndOfFile;
            return m_currentToken;
        }

        passLine(); // skip to the end of the line
        m_str1 = Cn::toString(buffer);
        m_currentToken = Group;
    }
    else if (isComment())
    {
        // Comment line, skip it
        passLine();
        return readNext(); // Recursively read the next line
    }
    else if (isEndOfLine())
    {
        // Empty line, skip it
        passLine();
        return readNext(); // Recursively read the next line
    }
    else
    {
        if (!parseString(buffer, CnCHR('=')))
        {
            if (m_errorString.empty())
                m_errorString = CnSTR("Error: Missed key-value pair separator '='");
            m_currentToken = EndOfFile;
            return m_currentToken;
        }
        m_str1 = Cn::toString(buffer);
        buffer.clear();
        passSpace();
        parseString(buffer);
        passLine();
        m_str2 = Cn::toString(buffer);
        m_currentToken = KeyValuePair;
    }

    return m_currentToken;
}

int CnConfReader::nextChar()
{
    m_ch = m_file.get();
    ++m_col;
    return m_ch;
}

// Helper to skip over whitespace
void CnConfReader::passSpace()
{
    while (!isEOF() && (m_ch == CnCHR(' ') || m_ch == CnCHR('\t') || m_ch == CnCHR('\v')))
    {
        if (isComment())
        {
            while (nextChar() != CN_CONFREADER_EOF && !isEndOfLine())
                ; // goto end of current line
            break;
        }
        nextChar();
    }
}

// Helper to skip the rest of the line (used for comments or after parsing a line)
void CnConfReader::passLine()
{
    while (m_ch != CN_CONFREADER_EOF)
    {  
        if (m_ch == CnCHR('\r'))
        {
            if (nextChar() == CnCHR('\n'))
                nextChar();
            m_col = 1;
            ++m_row;
            break;
        }
        if (m_ch == CnCHR('\n'))
        {
            nextChar();
            m_col = 1;
            ++m_row;
            break;
        }
        nextChar();
    }
}

bool CnConfReader::parseString(std::string &buffer, int endChar)
{
    if (m_ch == '"' || m_ch == '\'')
    {
        int quoteChar = m_ch;
        while (nextChar() != CN_CONFREADER_EOF && (m_ch != quoteChar) && !isEndOfLine())
            buffer += m_ch;
        if (m_ch != quoteChar)
        {
            m_errorString = CnSTR("Error: Not finished quotes");
            return false;
        }
        nextChar(); // Skip the closing quote
        passSpace();
    }
    else
    {
        buffer += m_ch;
        while (nextChar() != CN_CONFREADER_EOF && m_ch != endChar && !isEndOfLine())
        {
            if (isComment())
            {
                // Comment line, skip it
                passSpace();
                break;
            }
            buffer += m_ch;
        }
        while (buffer.size() && std::isspace(buffer.back()))
            buffer.pop_back(); // Remove trailing spaces
    }
    if (endChar != CnEMPTY_CHAR) 
    {
        if (m_ch != endChar)
            return false;
        else
            nextChar();
    }
    return true;
}

