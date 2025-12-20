/*********************************************************************
 * \file   CnItemLexer.h
 * \brief  Contains classes for lexical analysis of source text
 * 
 * \author serhmarch
 * \date   November 2024
 *********************************************************************/
#ifndef CNITEMLEXER_H
#define CNITEMLEXER_H

#include <CnTypes.h>

/*! \brief Represents a token in the source string.

    \details The `CnItemToken` class contains a single token from the source string, such as a word, number, Modbus address, symbol, or end of line.
    It provides functions to determine the type of token, its position in the source string, etc.
    A Modbus address must contain 6 decimal digits and have the format:\n
    `{0|1|3|4}xxxxx`, where the first digit can be one of {0|1|3|4}, and the next 5 must be in the range 00001-65536

    \sa `CnItemLexer`
*/
class CnItemToken
{
public:
    /// \brief Specifies the type of token.
    enum Type
    {
    TOK_EOL,    ///< End of line `END OF LINE`
    TOK_INT,    ///< Integer number
    TOK_MODBUS, ///< Modbus address
    TOK_SYMBOL, ///< Symbol
    TOK_WORD    ///< Word
    };

public:
    /// \details Creates an end-of-line token.
    static CnItemToken createEOL();

    /// \details Creates a number token, where `number` is the value and `pos` is the position in the source string.
    static CnItemToken createInt(uint32_t number, int pos);

    /// \details Creates a Modbus address token, where `address` is the value and `pos` is the position in the source string.
    static CnItemToken createModbus(uint32_t address, int pos);

    /// \details Creates a symbol token, where `symbol` is the value and `pos` is the position in the source string.
    static CnItemToken createSym(CnChar symbol, int pos);

    /// \details Creates a word token, where `word` is the value and `pos` is the position in the source string.
    static CnItemToken createWord(const CnString &word, int pos);

public:
    /// \details Default constructor creates an end-of-line token.
    CnItemToken(int pos = -1);

    /// \details Copy constructor creates a copy of the `other` token.
    CnItemToken(const CnItemToken &other);

public:
    /// \details Returns the position of the token in the source string.
    inline int pos()  const { return m_pos; }

    /// \details Returns the type of the token.
    inline Type type() const { return m_type; }

    /// \details Returns `true` if the token is end of line, `false` otherwise.
    inline bool isEOL() const { return m_type == TOK_EOL; }

    /// \details Returns `true` if the token is a number, `false` otherwise. 
    /// Modbus address is also considered a number.
    inline bool isInt() const { return (m_type == TOK_INT || m_type == TOK_MODBUS); }

    /// \details Returns `true` if the token is a Modbus address, `false` otherwise.
    inline bool isModbus() const { return m_type == TOK_MODBUS; }

    /// \details Returns `true` if the token is a symbol, `false` otherwise.
    inline bool isSymbol() const { return m_type == TOK_SYMBOL; }

    /// \details Returns `true` if the token is a comma symbol, `false` otherwise.
    inline bool isSymbolComma() const { return m_type == TOK_SYMBOL && m_str.at(0) == CnCHR(','); }

    /// \details Returns `true` if the token is a dot symbol, `false` otherwise.
    inline bool isSymbolDot() const { return m_type == TOK_SYMBOL && m_str.at(0) == CnCHR('.'); }

    /// \details Returns `true` if the token is a word, `false` otherwise.
    inline bool isWord() const { return m_type == TOK_WORD; }

    /// \details Returns the number if the token is a number, `0` otherwise.
    inline uint32_t Int() const { return isInt() ? m_num : 0; }

    /// \details Returns the Modbus address value if the token is a Modbus address, `0` otherwise.
    inline uint32_t Modbus() const { return isModbus() ? m_num : 0; }

    /// \details Returns the symbol value if the token is a symbol, `'\0'` otherwise.
    inline CnChar Symbol() const { return isSymbol() ? m_str.at(0) : CnCHR('\0'); }

    /// \details Returns the word value if the token is a word, empty string otherwise.
    inline const CnString  &Word() const { return isWord() ? m_str : m_strEmpty; }

    /// \details Compares tokens for equivalence. 
    /// Tokens are considered equivalent if they have the same type and position in the source string and have the same value specific to their type.
    bool operator==(const CnItemToken& other) const;

private:
    static CnString m_strEmpty;

private:
    Type m_type;
    CnString m_str;
    uint32_t m_num;
    int m_pos;
};

/*! \brief Contains a list of tokens.

    \details Contains a list of tokens and functions for working with this list.
    Also provides functions to determine the current token and move between list elements.
    \sa `CnItemToken`
*/
class CnItemTokenList
{
public:
    /// \details Default constructor.
    CnItemTokenList();

    /// \details Copy constructor.
    CnItemTokenList(const CnItemTokenList &other);

public:
    /// \details Adds a token to the list.
    void append(const CnItemToken& token);

    /// \details Returns the current token.
    inline const CnItemToken& current() const { return *m_i; }

    /// \details Returns the token following the current one.
    const CnItemToken& getNext() const;

    /// \details Moves the token pointer to the next and returns it.
    const CnItemToken& next();

    /// \details Returns the position in the string of the given `token`.
    int position(const CnItemToken &token);

    /// \details Returns `true` if the list is empty, `false` otherwise.
    inline bool isEmpty() const { return m_tokens.empty(); }

    /// \details Returns the number of tokens in the list.
    inline size_t count() const { return m_tokens.size(); }

    /// \details Clears the list of tokens.
    void clear();

private:
    typedef CnList<CnItemToken> Tokens_t;

private:
    Tokens_t m_tokens;
    Tokens_t::const_iterator m_i;
    CnItemToken m_tokEOL;
};

/*! \brief String lexical analysis class.

    \details The class converts a string (character array) into a list of higher-level tokens, such as numbers, words, etc.
    \sa `CnItemToken`
*/
class CnItemLexer
{
public:
    /// \details Default constructor.
    CnItemLexer();

public:
    /// \details Function converts the string `exp` into a list of higher-level tokens.
    /// Returns `true` if successful, `false` otherwise.
    bool parse(const CnString &exp);

public:
    /// \details Returns a reference to the constant list of tokens.
    inline const CnItemTokenList &tokens() const { return m_tokens; }

    /// \details Returns a reference to the constant current token.
    inline const CnItemToken& currentToken() const { return m_tokens.current(); }

    /// \details Returns a reference to the constant token following the current one.
    inline const CnItemToken& getNextToken() const { return m_tokens.getNext(); }

    /// \details Moves the token pointer to the next and returns a reference to the constant token.
    inline const CnItemToken& nextToken() { return m_tokens.next(); }

    /// \details Returns the number of tokens in the list.
    inline size_t countTokens() const { return m_tokens.count(); }

    /// \details Returns `true` if the token list is empty, `false` otherwise.
    inline bool isEmpty() const { return m_tokens.isEmpty(); }

public:
    /// \details Returns the initial (source) string.
    inline const CnString &source() const { return m_source; }

    /// \details Returns `true` if an error occurred during the previous conversion, `false` otherwise.
    inline bool hasError() const { return !m_error.empty(); }

    /// \details Returns the text of the last error.
    inline const CnString &errorString() const { return m_error; }

private:
    void setSource(const CnString& exp);
    bool parseModbusOrInt(uint32_t& out, CnItemToken::Type &type);
    bool parseWord(CnString& out);
    bool passSpaces();

private:
    CnString m_source;
    CnString m_error;
    int m_len;
    int m_pos;
    CnItemTokenList m_tokens;
};

#endif // CNITEMLEXER_H
