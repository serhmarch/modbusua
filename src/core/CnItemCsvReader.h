/*!
 * \file   CnConfReader.h
 * \brief  Class for initial processing of configuration file
 *
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNITEMCSVREADER_H
#define CNITEMCSVREADER_H

#include <CnString.h>
#include <CnFile.h>

/*! \brief The CnItemCsvReader class is designed for reading key-value pairs from a CSV file.

    \details The CnItemCsvReader class is used for reading key-value pairs from a CSV file,
    where each line contains a key and a value separated by a specified delimiter (default is ';').

    It reads the file sequentially, returning the key and value of each line.

    The class does not store the contents of the file; it only provides methods to read it line by line.
*/
class CnItemCsvReader
{
public:
    /// \details Constructs a CnItemCsvReader object for the specified file name and separator character.
    explicit CnItemCsvReader(const CnString &fileName, CnChar separator = CnCHR(';'));

public:
    /// \details Returns true if the CSV file is open, false otherwise.
    inline bool isOpen() const { return m_file.isOpen(); }

    /// \details Checks if there was an error reading the CSV file.
    inline bool hasError() const { return m_errorString.size(); }

    /// \details Returns the error message if there was an error reading the file.
    inline CnString errorString() const { return m_errorString; }

public:
    /// \details Returns the current key and value read from the CSV file.
    inline CnString key() const { return m_key; }

    /// \details Returns the current key and value read from the CSV file.
    inline CnString value() const { return m_value; }

public:
    /// \details Reads the next line from the CSV file and parses the key and value.
    /// \return True if a line was successfully read and parsed,
    /// false if the end of the file was reached or an error occurred.
    bool readNext();

private:
    void trim(CnString& str);
    bool parseKeyValue(CnString& line);
    inline bool isEOF() const { return m_file.atEnd(); }

private:
    CnFile m_file;
    CnChar m_separator;
    CnString m_key;
    CnString m_value;
    CnString m_errorString;
};

#endif // CNITEMCSVREADER_H
