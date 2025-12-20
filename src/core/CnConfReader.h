/*!
 * \file   CnConfReader.h
 * \brief  Class for initial processing of configuration file
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNCONFREADER_H
#define CNCONFREADER_H

#include <CnString.h>
#include <CnFile.h>
#include <CnDir.h>

#define CnEMPTY_CHAR -2 // because -1 is EOF

/*! \brief The CnConfReader class is designed for initial processing of configuration files.

    \details The CnConfReader class is used for initial processing of configuration files
    in the INI format (with groups and key-value pairs).

    It reads the file sequentially, returning tokens that indicate whether a group name,
    a key-value pair, or the end of the file has been reached.

    The class does not store the contents of the file; it only provides methods to read it token by token.
*/
class CnConfReader
{
public:
    /// \brief Specifies the type of token.
    enum Token
    {
        Group       , ///< Group name token type
        KeyValuePair, ///< Key-value pair token type
        EndOfFile     ///< End of file token type
    };

public:
    /// \details Constructs a CnConfReader object for the specified file name.
    /// \param fileName The name or path of the configuration file.
    explicit CnConfReader(const CnString &fileName);

    /// \details Destructor.
    ~CnConfReader();

public:
    /// \details Returns the name of the configuration file (without path).
    inline CnString fileName() const { return m_file.fileName(); }

    /// \details Returns the full path of the configuration file.
    inline CnString dirName() const { return m_dir.dirName(); }

    /// \details Returns the directory object containing the configuration file.
    inline CnDir dir() const { return m_dir; }

    /// \details Checks if the configuration file is open.
    inline bool isOpen() const { return m_file.isOpen(); }

    /// \details Checks if there was an error reading the configuration file.
    inline bool hasError() const { return m_errorString.size(); }

    /// \brief Returns the error message if there was an error reading the file.
    inline const CnString& errorString() const { return m_errorString; }

    /// \details Returns the current row number in the configuration file (1-based).
    inline int row() const { return m_row; }

    /// \details Returns the current column number in the configuration file (1-based).
    inline int col() const { return m_col; }

public:
    /// \details Returns the name of the current group (valid if current token is Group).
    const CnString& groupName() const { return m_str1; }

    /// \details Returns the key and value of the current key-value pair (valid if current token is KeyValuePair).
    const CnString& key() const { return m_str1; }

    /// \details Returns the value of the current key-value pair (valid if current token is KeyValuePair).
    const CnString& value() const { return m_str2; }

public:
    /// \details Reads the next token from the configuration file.
    /// \return The type of the next token (Group, KeyValuePair, or End
    Token readNext();

    /// \details Returns the type of the current token.
    /// \return The current token type.
    inline Token currentToken() const { return m_currentToken; }

private:
    int nextChar();
    void passSpace();
    void passLine();
    bool parseString(std::string &buffer, int endChar = CnEMPTY_CHAR);
    inline bool isComment() const { return m_ch == CnCHR('#') || m_ch == CnCHR(';'); }
    inline bool isEndOfLine() const { return m_ch == CnCHR('\r') || m_ch == CnCHR('\n'); }
    inline bool isEOF() const { return m_ch == -1; /*m_file.atEnd();*/ }

private:
    CnFile m_file;
    CnDir m_dir;
    Token m_currentToken;
    int m_ch;
    int m_row;
    int m_col;
    CnString m_str1;
    CnString m_str2;
    CnString m_errorString;
};

#endif // CNCONFREADER_H
