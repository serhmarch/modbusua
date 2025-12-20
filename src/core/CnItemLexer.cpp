#include "CnItemLexer.h"

#include <cctype>

#include <CnUtils.h>

using namespace std;

CnString CnItemToken::m_strEmpty;

CnItemToken CnItemToken::createEOL()
{
    CnItemToken tok;
    return tok;
}

CnItemToken CnItemToken::createInt(uint32_t i, int pos)
{
    CnItemToken tok(pos);
    tok.m_type = TOK_INT;
    tok.m_num = i;
    return tok;
}

CnItemToken CnItemToken::createModbus(uint32_t i, int pos)
{
    CnItemToken tok(pos);
    tok.m_type = TOK_MODBUS;
    tok.m_num = i;
    return tok;
}

CnItemToken CnItemToken::createSym(CnChar c, int pos)
{
    CnItemToken tok(pos);
    tok.m_type = TOK_SYMBOL;
    tok.m_str = c;
    return tok;
}

CnItemToken CnItemToken::createWord(const CnString & word, int pos)
{
    CnItemToken tok(pos);
    tok.m_type = TOK_WORD;
    tok.m_str = word;
    return tok;
}

CnItemToken::CnItemToken(int pos)
{
    m_type = TOK_EOL;
    m_pos = pos;
}

CnItemToken::CnItemToken(const CnItemToken & other)
{
    this->m_type = other.m_type;
    this->m_str = other.m_str;
    this->m_num = other.m_num;
    this->m_pos = other.m_pos;
}

bool CnItemToken::operator==(const CnItemToken &other) const
{
    return (  (this->m_type == other.m_type) &&
              (this->m_pos  == other.m_pos ) &&
             ((this->m_type == TOK_EOL)                                   ||
              (this->m_type == TOK_INT    && this->m_num == other.m_num)  ||
              (this->m_type == TOK_MODBUS && this->m_num == other.m_num)) ||
             ((this->m_type == TOK_WORD || this->m_type == TOK_SYMBOL) && this->m_str == other.m_str)
           );
}

CnItemTokenList::CnItemTokenList()
{
    m_tokEOL = CnItemToken::createEOL();
    m_i = m_tokens.begin();
}

CnItemTokenList::CnItemTokenList(const CnItemTokenList &other)
{
    m_tokEOL = CnItemToken::createEOL();
    m_tokens = other.m_tokens;
    m_i = m_tokens.begin();

}

void CnItemTokenList::append(const CnItemToken &token)
{
    m_tokens.push_back(token);
    m_i = m_tokens.begin();
}

const CnItemToken &CnItemTokenList::getNext() const
{
    Tokens_t::const_iterator it = m_i;
    ++it;
    return *it;
}

const CnItemToken &CnItemTokenList::next()
{
    ++m_i;
    return current();
}

int CnItemTokenList::position(const CnItemToken& token)
{
    int i = 0;
    for (Tokens_t::const_iterator it = m_tokens.begin(); it != m_tokens.end(); it++)
    {
        CnItemToken t = *it;
        if (token == t)
            return i;
        i++;
    }
    return -1;
}

void CnItemTokenList::clear()
{
    m_tokens.clear();
    m_i = m_tokens.begin();
}

CnItemLexer::CnItemLexer()
{
}

bool CnItemLexer::parse(const CnString &exp)
{
    setSource(exp);
    m_tokens.clear();
    while (m_pos < m_len)
    {
        if (passSpaces())
        {
            if (m_pos >= m_len)
                break;
        }
        int mpos = m_pos;
        uint32_t i;
        CnItemToken::Type type;
        CnString s;
        if (parseModbusOrInt(i, type))
        {
            if (hasError())
                return false;
            if (type == CnItemToken::TOK_MODBUS)
                m_tokens.append(CnItemToken::createModbus(i, mpos));
            else
                m_tokens.append(CnItemToken::createInt(i, mpos));
        }
        else if (parseWord(s))
        {
            if (hasError())
                return false;
            m_tokens.append(CnItemToken::createWord(s, mpos));
        }
        else
        {
            m_tokens.append(CnItemToken::createSym(m_source.at(m_pos), m_pos));
            m_pos++;
        }
    }
    m_tokens.append(CnItemToken::createEOL());
    return true;
}

void CnItemLexer::setSource(const CnString &exp)
{
    //m_source = Cn::toUpper(exp);
    m_source = exp;
    m_len = static_cast<int>(m_source.length());
    m_pos = 0;
}

bool CnItemLexer::parseModbusOrInt(uint32_t & out, CnItemToken::Type & type)
{
    CnChar c = m_source.at(m_pos);
    if (isdigit(c))
    {
        CnString ws(1, c);
        while ((++m_pos < m_len) && (isdigit(m_source.at(m_pos))))
            ws.push_back(m_source.at(m_pos));
        uint32_t num = Cn::toInt(ws);
        if (ws.size() == 6)
        {
            switch (c)
            {
            case CnCHR('0'):
            case CnCHR('1'):
            case CnCHR('3'):
            case CnCHR('4'):
            {
                uint32_t cellNum = num % 100000;
                if (cellNum > 0)
                {
                    type = CnItemToken::TOK_MODBUS;
                    out = num;
                    return true;
                }
            }
                break;
            }
        }
        type = CnItemToken::TOK_INT;
        out = num;
        return true;
    }
    return false;
}

bool CnItemLexer::parseWord(CnString & out)
{
    CnChar c = m_source.at(m_pos);
    if (isalpha(c) || c == CnCHR('_'))
    {
        CnString ws(1, std::toupper(c));
        while ((++m_pos < m_len) && (isalnum(c = m_source.at(m_pos)) || c == CnCHR('_')))
            ws.push_back(std::toupper(c));
        out = ws;
        return true;
    }
    return false;
}

bool CnItemLexer::passSpaces()
{
    if (isspace(m_source.at(m_pos)))
    {
        while ((++m_pos < m_len) && isspace(m_source.at(m_pos)))
            ; // NOP
        return true;
    }
    return false;
}

