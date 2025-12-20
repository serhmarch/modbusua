/*!
 * \file   CnString.h
 * \brief  Contains the CnString class for working with text strings
 *
 * \author serhmarch
 * \date   September 2025
 */
#ifndef CNSTRING_H
#define CNSTRING_H

#include <iterator>
#include <CnCore.h>
#include <CnStd_string.h>

class CnStringPrivate;

/*! \brief The CnString class is designed for convenient work with text strings.

    \details The CnString class is used for working with text strings.
    It provides a set of functions and methods for creating, modifying,
    and analyzing text strings. The CnString class is similar to the standard C++ std::string class,
    but it is adapted for use in the current app and supports both narrow (char) and wide (wchar_t) character types,
    depending on the CN_USE_WSTRING macro definition.

    \sa CnStd::string
*/
class CnString
{
public:
    /// \details Iterator type for mutable access to string characters.
    typedef CnChar *iterator;

    /// \details Iterator type for immutable access to string characters.
    typedef const CnChar *const_iterator;

    /// \details Alias for iterator type.
    typedef iterator Iterator;

    /// \details Alias for const_iterator type.
    typedef const_iterator ConstIterator;

    /// \details Reverse iterator type for mutable access.
    typedef std::reverse_iterator<iterator> reverse_iterator;

    /// \details Reverse iterator type for immutable access.
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    /// \details Special value indicating 'not found' or 'until end'.
    static const size_t npos;

public: // Constructors and destructor
    /// \details Default constructor. Creates an empty string.
    CnString();

    /// \details Constructs a string from a character array and size.
    CnString(const CnChar *str, size_t size);

    /// \details Constructs a string from a null-terminated character array.
    CnString(const CnChar *str);

    /// \details Constructs a string of given size filled with the specified character.
    CnString(size_t size, CnChar ch);

    /// \details Constructs a string containing a single character.
    CnString(CnChar ch);

    /// \details Copy constructor.
    CnString(const CnString &other);

    /// \details Move constructor.
    CnString(CnString &&other);

    /// \details Destructor.
    ~CnString();

public: // Assignment
    /// \details Copy assignment operator.
    CnString& operator=(const CnString &other);

    /// \details Move assignment operator.
    CnString& operator=(CnString &&other);

    /// \details Assigns from a null-terminated character array.
    CnString& operator=(const CnChar *str);

    /// \details Assigns from a single character.
    CnString& operator=(CnChar ch);

    /// \details Assigns from an initializer list of characters.
    CnString& operator=(std::initializer_list<CnChar> ilist);

public: // Iterators
    /// \details Returns an iterator to the beginning of the string.
    iterator begin();

    /// \details Returns a const iterator to the beginning of the string.
    const_iterator begin() const;

    /// \details Returns a const iterator to the beginning of the string.
    const_iterator cbegin() const;

    /// \details Returns a const iterator to the beginning of the string.
    const_iterator constBegin() const;

    /// \details Returns an iterator to the end of the string.
    iterator end();

    /// \details Returns a const iterator to the end of the string.
    const_iterator end() const;

    /// \details Returns a const iterator to the end of the string.
    const_iterator cend() const;

    /// \details Returns a const iterator to the end of the string.
    const_iterator constEnd() const;

    /// \details Returns a reverse iterator to the beginning of the reversed string.
    inline reverse_iterator rbegin() { return reverse_iterator(end()); }

    /// \details Returns a reverse iterator to the end of the reversed string.
    inline reverse_iterator rend() { return reverse_iterator(begin()); }

    /// \details Returns a const reverse iterator to the beginning of the reversed string.
    inline const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

    /// \details Returns a const reverse iterator to the end of the reversed string.
    inline const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    /// \details Returns a const reverse iterator to the beginning of the reversed string.
    inline const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }

    /// \details Returns a const reverse iterator to the end of the reversed string.
    inline const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }


public: // Element access
    /// \details Returns the character at the specified position with bounds checking.
    CnChar at(size_t i) const;

    /// \details Returns a reference to the character at the specified position.
    CnChar& operator[](size_t pos);

    /// \details Returns a const reference to the character at the specified position.
    const CnChar& operator[](size_t pos)  const; 

    /// \details Returns a pointer to the underlying character array.
    const CnChar* data() const;

    /// \details Returns a pointer to a null-terminated character array.
    inline const CnChar* c_str() const { return data(); }

    /// \details Returns the first character in the string.
    CnChar front() const;

    /// \details Returns the last character in the string.
    CnChar back() const;

public: // Capacity
    /// \details Returns the number of characters in the string.
    size_t size() const;

    /// \details Returns the length of the string.
    inline size_t length() const { return size(); }

    /// \details Returns true if the string is null.
    bool isNull() const;

    /// \details Returns true if the string is empty.
    bool isEmpty() const;

    /// \details Returns true if the string is empty.
    inline bool empty() const { return isEmpty(); }
    //size_t max_size() const;

public: // Modifiers
    /// \details Appends a character to the end of the string.
    void push_back(CnChar ch);

    /// \details Removes the last character from the string.
    void pop_back();

    /// \details Inserts characters at the specified index.
    CnString& insert(size_t index, size_t count, CnChar ch);

    /// \details Inserts a character array at the specified index.
    CnString& insert(size_t index, const CnChar* s);

    /// \details Inserts a character array of given count at the specified index.
    CnString& insert(size_t index, const CnChar* s, size_t count);

    /// \details Inserts another string at the specified index.
    CnString& insert(size_t index, const CnString& str);

    /// \details Erases characters from the string starting at index.
    CnString& erase(size_t index = 0, size_t count = npos);

    /// \details Erases the character at the given position.
    iterator erase(const_iterator position);

    /// \details Erases characters in the range [first, last).
    iterator erase(const_iterator first, const_iterator last);

    /// \details Appends characters to the string.
    CnString& append(size_t count, CnChar ch );

    /// \details Appends a character array to the string.
    CnString& append(const CnChar* s );

    /// \details Appends a character array of given count to the string.
    CnString& append(const CnChar* s, size_t count );

    /// \details Appends another string to the string.
    CnString& append(const CnString& str );

    /// \details Appends an initializer list of characters to the string.
    CnString& append(std::initializer_list<CnChar> ilist );

    /// \details Appends another string to the string (operator+=).
    inline CnString& operator+=(const CnString& str) { return this->append(str); }

    /// \details Appends a character to the string (operator+=).
    inline CnString& operator+=(CnChar ch) { return this->append(1, ch); }

    /// \details Appends a character array to the string (operator+=).
    inline CnString& operator+=(const CnChar* s) { return this->append(s); }

    /// \details Appends an initializer list of characters to the string (operator+=).
    inline CnString& operator+=(std::initializer_list<CnChar> ilist) { return this->append(ilist); }

    /// \details Replaces part of the string with another string.
    CnString& replace(size_t pos, size_t count, const CnString& str);

    /// \details Replaces a range of characters with another string.
    CnString& replace(const_iterator first, const_iterator last, const CnString& str);

    /// \details Replaces part of the string with another string.
    CnString& replace(size_t pos, size_t count, const CnChar* str);

    /// \details Replaces a range of characters with another string.
    CnString& replace(const_iterator first, const_iterator last, const CnChar* str);

    /// \details Replaces part of the string with a character array.
    CnString& replace(size_t pos, size_t count, size_t count2, CnChar ch);

    /// \details Replaces part of the string with a character array.
    CnString& replace(const_iterator first, const_iterator last, size_t count2, CnChar ch);

    /// \details Replaces character in the `pos` position of the string with an another character.
    CnString& replace(size_t pos, CnChar ch);

    /// \details Swaps the contents of this string with another.
    void swap(CnString& other);

    /// \details Clears the contents of the string.
    void clear();

    /// \details Resizes the string to the specified size.
    void resize(size_t size);

public: // Search
    /// \details Finds the first occurrence of a string starting from position.
    size_t find(const CnString& str, size_t pos = 0) const;

    /// \details Finds the first occurrence of a character array starting from position.
    size_t find(const CnChar* s, size_t pos, size_t count) const;

    /// \details Finds the first occurrence of a character array starting from position.
    size_t find(const CnChar* s, size_t pos = 0) const;

    /// \details Finds the first occurrence of a character starting from position.
    size_t find(CnChar ch, size_t pos = 0) const;

    /// \details Finds the last occurrence of a string before position.
    size_t rfind(const CnString& str, size_t pos = npos) const;

    /// \details Finds the last occurrence of a character array before position.
    size_t rfind(const CnChar* s, size_t pos, size_t count) const;

    /// \details Finds the last occurrence of a character array before position.
    size_t rfind(const CnChar* s, size_t pos = npos) const;

    /// \details Finds the last occurrence of a character before position.
    size_t rfind(CnChar ch, size_t pos = npos) const;

    /// \details Finds the first occurrence of any character from the string starting from position.
    size_t find_first_of(const CnString& str, size_t pos = 0) const;

    /// \details Finds the first occurrence of any character from the character array starting from position.
    size_t find_first_of(const CnChar* s, size_t pos, size_t count) const;

    /// \details Finds the first occurrence of any character from the character array starting from position.
    size_t find_first_of(const CnChar* s, size_t pos = 0) const;

    /// \details Finds the first occurrence of a character starting from position.
    size_t find_first_of(CnChar ch, size_t pos = 0) const;

    /// \details Finds the first character not matching any in the string starting from position.
    size_t find_first_not_of(const CnString& str, size_t pos = 0) const;

    /// \details Finds the first character not matching any in the character array starting from position.
    size_t find_first_not_of(const CnChar* s, size_t pos, size_t count) const;

    /// \details Finds the first character not matching any in the character array starting from position.
    size_t find_first_not_of(const CnChar* s, size_t pos = 0) const;

    /// \details Finds the first character not matching the specified character starting from position.
    size_t find_first_not_of(CnChar ch, size_t pos = 0) const;

    /// \details Finds the last occurrence of any character from the string before position.
    size_t find_last_of(const CnString& str, size_t pos = npos) const;

    /// \details Finds the last occurrence of any character from the character array before position.
    size_t find_last_of(const CnChar* s, size_t pos, size_t count) const;

    /// \details Finds the last occurrence of any character from the character array before position.
    size_t find_last_of(const CnChar* s, size_t pos = npos) const;

    /// \details Finds the last occurrence of a character before position.
    size_t find_last_of(CnChar ch, size_t pos = npos) const;

    /// \details Finds the last character not matching any in the string before position.
    size_t find_last_not_of(const CnString& str, size_t pos = npos) const;

    /// \details Finds the last character not matching any in the character array before position.
    size_t find_last_not_of(const CnChar* s, size_t pos, size_t count) const;

    /// \details Finds the last character not matching any in the character array before position.
    size_t find_last_not_of(const CnChar* s, size_t pos = npos) const;

    /// \details Finds the last character not matching the specified character before position.
    size_t find_last_not_of(CnChar ch, size_t pos = npos) const;

public: // Operations
    /// \details Compares a substring with a character array.
    int compare(size_t pos1, size_t count1, const CnChar* s, size_t count2 = npos) const;

    /// \details Compares the string with a character array.
    int compare(const CnChar* s) const;

    /// \details Compares a substring with another string.
    int compare(size_t pos1, size_t count1, const CnString& str) const;

    /// \details Compares the string with another string.
    int compare(const CnString& str) const;

    /// \details Returns true if the string starts with the specified character.
    bool starts_with(CnChar ch) const;

    /// \details Returns true if the string starts with the specified character array.
    bool starts_with(const CnChar *str) const;

    /// \details Returns true if the string starts with the specified string.
    bool starts_with(const CnString &str) const;

    /// \details Returns true if the string ends with the specified character.
    bool ends_with(CnChar ch) const;

    /// \details Returns true if the string ends with the specified character array.
    bool ends_with(const CnChar *str) const;

    /// \details Returns true if the string ends with the specified string.
    bool ends_with(const CnString &str) const;

    /// \details Returns true if the string contains the specified character.
    bool constains(CnChar ch) const;

    /// \details Returns true if the string contains the specified character array.
    bool constains(const CnChar *str) const;

    /// \details Returns true if the string contains the specified string.
    bool constains(const CnString &str) const;

    /// \details Returns a substring starting at position with given count.
    CnString substr(size_t pos = 0, size_t count = npos) const;

public: // std strings
    /// \details Returns a pointer to the underlying char array (narrow string).
    const char* chars() const;

    /// \details Returns a pointer to the underlying wchar_t array (wide string).
    const wchar_t* wchars() const;

    /// \details Converts to std::string.
    std::string toStdString() const;

    /// \details Converts to std::wstring.
    std::wstring toStdWString() const;

public: // static conversions
    /// \details Creates a CnString from std::string.
    static CnString fromStdString(const std::string &str);

    /// \details Creates a CnString from std::wstring.
    static CnString fromStdWString(const std::wstring &str);

    /// \details Creates a CnString from a char array.
    static CnString fromChars(const char* str);

    /// \details Creates a CnString from a char array of given length.
    static CnString fromChars(const char* str, size_t len);

    /// \details Creates a CnString from a wchar_t array.
    static CnString fromWChars(const wchar_t* str);

    /// \details Creates a CnString from a wchar_t array of given length.
    static CnString fromWChars(const wchar_t* str, size_t len);

    /// \details Finds the last occurrence of any character from brk in chars before position.
    static size_t find_last_of_chars(const CnChar* chars, const CnChar* brk, size_t pos, size_t brkcount);
        
private:
    CnStringPrivate* d;
};

// Non related members

/// \details Concatenates two CnString objects.
inline CnString operator+(const CnString& lhs, const CnString& rhs) { CnString result(lhs); result.append(rhs); return result; }

/// \details Concatenates a CnString and a character array.
inline CnString operator+(const CnString& lhs, const CnChar* rhs) { CnString result(lhs); result.append(rhs); return result; }

/// \details Concatenates a character array and a CnString.
inline CnString operator+(const CnChar* lhs, const CnString& rhs) { CnString result(lhs); result.append(rhs); return result; }

/// \details Concatenates a CnString and a character.
inline CnString operator+(const CnString& lhs, CnChar rhs) { CnString result(lhs); result.append(1, rhs); return result; }

/// \details Concatenates a character and a CnString.
inline CnString operator+(CnChar lhs, const CnString& rhs) { CnString result(1, lhs); result.append(rhs); return result; }

/// \details Checks if two CnString objects are equal.
inline bool operator==(const CnString& lhs, const CnString& rhs) { return lhs.compare(rhs) == 0; }

/// \details Checks if a CnString and a character array are equal.
inline bool operator==(const CnString& lhs, const CnChar* rhs) { return lhs.compare(rhs) == 0; }

/// \details Checks if a character array and a CnString are equal.
inline bool operator==(const CnChar* lhs, const CnString& rhs) { return rhs.compare(lhs) == 0; }

/// \details Checks if two CnString objects are not equal.
inline bool operator!=(const CnString& lhs, const CnString& rhs) { return lhs.compare(rhs) != 0; }

/// \details Checks if a CnString and a character array are not equal.
inline bool operator!=(const CnString& lhs, const CnChar* rhs) { return lhs.compare(rhs) != 0; }

/// \details Checks if a character array and a CnString are not equal.
inline bool operator!=(const CnChar* lhs, const CnString& rhs) { return rhs.compare(lhs) != 0; }

/// \details Checks if lhs is less than rhs (CnString).
inline bool operator<(const CnString& lhs, const CnString& rhs) { return lhs.compare(rhs) < 0; }

/// \details Checks if lhs is less than rhs (CnString, character array).
inline bool operator<(const CnString& lhs, const CnChar* rhs) { return lhs.compare(rhs) < 0; }

/// \details Checks if lhs is less than rhs (character array, CnString).
inline bool operator<(const CnChar* lhs, const CnString& rhs) { return rhs.compare(lhs) > 0; }

/// \details Checks if lhs is greater than rhs (CnString).
inline bool operator>(const CnString& lhs, const CnString& rhs) { return lhs.compare(rhs) > 0; }

/// \details Checks if lhs is greater than rhs (CnString, character array).
inline bool operator>(const CnString& lhs, const CnChar* rhs) { return lhs.compare(rhs) > 0; }

/// \details Checks if lhs is greater than rhs (character array, CnString).
inline bool operator>(const CnChar* lhs, const CnString& rhs) { return rhs.compare(lhs) < 0; }

/// \details Checks if lhs is less than or equal to rhs (CnString).
inline bool operator<=(const CnString& lhs, const CnString& rhs) { return lhs.compare(rhs) <= 0; }

/// \details Checks if lhs is less than or equal to rhs (CnString, character array).
inline bool operator<=(const CnString& lhs, const CnChar* rhs) { return lhs.compare(rhs) <= 0; }

/// \details Checks if lhs is less than or equal to rhs (character array, CnString).
inline bool operator<=(const CnChar* lhs, const CnString& rhs) { return rhs.compare(lhs) >= 0; }

/// \details Checks if lhs is greater than or equal to rhs (CnString).
inline bool operator>=(const CnString& lhs, const CnString& rhs) { return lhs.compare(rhs) >= 0; }

/// \details Checks if lhs is greater than or equal to rhs (CnString, character array).
inline bool operator>=(const CnString& lhs, const CnChar* rhs) { return lhs.compare(rhs) >= 0; }

/// \details Checks if lhs is greater than or equal to rhs (character array, CnString).
inline bool operator>=(const CnChar* lhs, const CnString& rhs) { return rhs.compare(lhs) <= 0; }

// Input/Output stream operators
/// Writes the contents of a CnString to an output stream.
std::basic_ostream<CnChar>& operator<<(std::basic_ostream<CnChar>& os, const CnString& str);

/// Reads the contents of an input stream into a CnString.
std::basic_istream<CnChar>& operator>>(std::basic_istream<CnChar>& os, CnString& str);

// Hash function
namespace std {

/// Specialization of std::hash for CnString.
/// Enables usage of CnString in hash-based containers like std::unordered_map.
template <>
struct hash<CnString>
{
    /// Computes the hash value for a CnString.
    size_t operator()(const CnString& str) const
    { 
        return std::hash<CnStd::string>{}(CnStd::string(str.data(), str.size()));
    }
};

} // namespace std

#endif // CNSTRING_H