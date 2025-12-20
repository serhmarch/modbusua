#include "CnString.h"
#include "CnString_p.h"

#include <codecvt>
#include <locale>
#include <cstring>

#include <CnStd_string.h>
#include <CnStd_sstream.h>

const size_t CnString::npos = static_cast<size_t>(-1);

static CnStringPrivate nullCnStringData;

#ifdef CN_USE_WSTRING
static thread_local std::string tls_strbuffer;
#else
static thread_local std::wstring tls_strbuffer;
#endif // CN_USE_WSTRING

CnString::CnString()
{
    nullCnStringData.incRef();
    d = &nullCnStringData;
}

CnString::CnString(const CnChar *str, size_t size)
{
    d = new CnStringPrivate(str, size);
}

CnString::CnString(const CnChar *str)
{
    size_t len = CnStd::strlen(str);
    d = new CnStringPrivate(str, len);
}

CnString::CnString(size_t size, CnChar ch)
{
    d = new CnStringPrivate();
    if (size > 0)
    {
        d->data = (CnChar*)calloc(size+1, sizeof(CnChar));
        for (size_t i = 0; i < size; i++)
            d->data[i] = ch;
        d->data[size] = CnCHR('\0');
        d->size = size;
    }
}

CnString::CnString(CnChar ch)
{
    d = new CnStringPrivate();
    d->data = (CnChar*)calloc(2, sizeof(CnChar));
    d->data[0] = ch;
    d->data[1] = CnCHR('\0');
    d->size = 1;
}

CnString::CnString(const CnString &other)
{
    d = other.d;
    d->incRef();
}

CnString::CnString(CnString &&other)
{
    nullCnStringData.incRef();
    d = other.d;
    other.d = &nullCnStringData;
}

CnString::~CnString()
{
    d->decRef();
}

// Assignment operators
CnString& CnString::operator=(const CnString &other)
{
    if (this != &other)
    {
        other.d->incRef();
        d->decRef();
        d = other.d;
    }
    return *this;
}

CnString& CnString::operator=(CnString &&other)
{
    if (this != &other)
    {
        nullCnStringData.incRef();
        d = other.d;
        other.d = &nullCnStringData;
    }
    return *this;
}

CnString& CnString::operator=(const CnChar *str)
{
    d->decRef();
    size_t len = CnStd::strlen(str);
    d = new CnStringPrivate(str, len);
    return *this;
}

CnString& CnString::operator=(CnChar ch)
{
    d->decRef();
    d = new CnStringPrivate();
    d->data = (CnChar*)calloc(2, sizeof(CnChar));
    d->data[0] = ch;
    d->data[1] = CnCHR('\0');
    d->size = 1;
    return *this;
}

CnString& CnString::operator=(std::initializer_list<CnChar> ilist)
{
    d->decRef();
    d = new CnStringPrivate();
    d->size = ilist.size();
    d->data = (CnChar*)calloc(d->size + 1, sizeof(CnChar));
    size_t i = 0;
    for (CnChar ch : ilist)
        d->data[i++] = ch;
    d->data[d->size] = CnCHR('\0');
    return *this;
}

// Iterators

CnString::iterator CnString::begin()
{
    return d->data;
}

CnString::const_iterator CnString::begin() const
{
    return d->data;
}

CnString::const_iterator CnString::cbegin() const
{
    return d->data;
}

CnString::const_iterator CnString::constBegin() const
{
    return d->data;
}

CnString::iterator CnString::end()
{
    return d->data + d->size;
}

CnString::const_iterator CnString::end() const
{
    return d->data + d->size;
}

CnString::const_iterator CnString::cend() const
{
    return d->data + d->size;
}

CnString::const_iterator CnString::constEnd() const
{
    return d->data + d->size;
}

// Element access
CnChar CnString::at(size_t i) const
{
    return d->data[i];
}

CnChar& CnString::operator[](size_t pos)
{
    if (d->ref > 1)
    {
        CnStringPrivate* nd = new CnStringPrivate(d->data, d->size);
        d->decRef();
        d = nd;
    }
    return d->data[pos];
}

const CnChar& CnString::operator[](size_t pos) const
{
    return d->data[pos];
}

const CnChar* CnString::data() const
{
    return d->data;
}

CnChar CnString::front() const
{
    return d->size ? d->data[0] : CnCHR('\0');
}

CnChar CnString::back() const
{
    return d->size ? d->data[d->size - 1] : CnCHR('\0');
}

// Capacity
size_t CnString::size() const
{
    return d->size;
}

bool CnString::isNull() const
{
    return d == &nullCnStringData;
}

bool CnString::isEmpty() const
{
    return d->isEmpty();
}

// Modifiers
void CnString::push_back(CnChar ch)
{
    insert(d->size, 1, ch);
}

void CnString::pop_back()
{
    if (d->size == 0)
        return;
    // ensure unique
    if (d->ref > 1)
    {
        CnStringPrivate* nd = new CnStringPrivate(d->data, d->size);
        d->decRef();
        d = nd;
    }
    if (d->size > 0)
    {
        d->size -= 1;
        d->data[d->size] = CnCHR('\0');
    }
}

CnString& CnString::insert(size_t index, size_t count, CnChar ch)
{
    if (count == 0)
        return *this;
    size_t sz = d->size;
    if (index > sz)
        index = sz;

    // prepare new buffer
    size_t newSz = sz + count;
    CnChar* newData = (CnChar*)calloc(newSz + 1, sizeof(CnChar));
    // copy head
    if (index > 0 && sz > 0)
        memcpy(newData, d->data, index * sizeof(CnChar));
    // fill inserted
    for (size_t i = 0; i < count; ++i)
        newData[index + i] = ch;
    // copy tail
    if (sz > index)
        memcpy(newData + index + count, d->data + index, (sz - index) * sizeof(CnChar));
    newData[sz + count] = CnCHR('\0');

    // replace data
    if (d->ref > 1)
    {
        d->decRef();
        d = new CnStringPrivate();
        d->data = newData;
        d->size = newSz;
    }
    else
    {
        d->data = newData;
        d->size = newSz;
    }
    return *this;
}

CnString& CnString::insert(size_t index, const CnChar* s)
{
    return insert(index, s, CnStd::strlen(s));
}

CnString& CnString::insert(size_t index, const CnChar* s, size_t count)
{
    if (count == 0)
        return *this;
    size_t sz = d->size;
    if (index > sz)
        index = sz;

    CnChar* newData = (CnChar*)calloc(sz + count + 1, sizeof(CnChar));
    if (d->data && sz > 0)
        memcpy(newData, d->data, index * sizeof(CnChar));
    memcpy(newData + index, s, count * sizeof(CnChar));
    if (d->data && sz > index)
        memcpy(newData + index + count, d->data + index, (sz - index) * sizeof(CnChar));
    newData[sz + count] = CnCHR('\0');

    if (d->ref > 1)
    {
        d->decRef();
        d = new CnStringPrivate();
        d->data = newData;
        d->size = sz + count;
    }
    else
    {
        d->data = newData;
        d->size = sz + count;
    }
    return *this;
}

CnString& CnString::insert(size_t index, const CnString& str)
{
    return insert(index, str.data(), str.size());
}

CnString& CnString::erase(size_t index, size_t count)
{
    size_t sz = d->size;
    if (d->isNull() || index >= sz || count == 0)
        return *this;
    if (index + count > sz)
        count = sz - index;

    size_t newSize = sz - count;
    CnChar* newData = nullptr;
    if (newSize > 0)
    {
        newData = (CnChar*)calloc(newSize + 1, sizeof(CnChar));
        if (index > 0)
            memcpy(newData, d->data, index * sizeof(CnChar));
        if (index + count < sz)
            memcpy(newData + index, d->data + index + count, (sz - index - count) * sizeof(CnChar));
        newData[newSize] = CnCHR('\0');
    }

    if (newData == nullptr)
        clear();
    else if (d->ref > 1)
    {
        d->decRef();
        d = new CnStringPrivate();
        d->data = newData;
        d->size = newSize;
    }
    else
    {
        d->data = newData;
        d->size = newSize;
    }
    return *this;
}

CnString::iterator CnString::erase(CnString::const_iterator position)
{
    size_t index = position - d->data;
    erase(index, 1);
    return d->data + index;
}

CnString::iterator CnString::erase(CnString::const_iterator first, CnString::const_iterator last)
{
    size_t firstIndex = first - d->data;
    size_t lastIndex = last - d->data;
    erase(firstIndex, lastIndex - firstIndex);
    return d->data + firstIndex;
}

CnString& CnString::append(size_t count, CnChar ch)
{
    return insert(d->size, count, ch);
}

CnString& CnString::append(const CnChar* s)
{
    return insert(d->size, s, CnStd::strlen(s));
}

CnString& CnString::append(const CnChar* s, size_t count)
{
    return insert(d->size, s, count);
}

CnString& CnString::append(const CnString& str)
{
    return insert(d->size, str.data(), str.size());
}

CnString& CnString::append(std::initializer_list<CnChar> ilist)
{
    for (CnChar ch : ilist)
        push_back(ch);
    return *this;
}

CnString& CnString::replace(size_t pos, size_t count, const CnString& str)
{
    size_t sz = d->size;
    if (pos > sz)
        pos = sz;
    if (count > sz - pos)
        count = sz - pos;
    erase(pos, count);
    insert(pos, str.data(), str.size());
    return *this;
}

CnString& CnString::replace(const_iterator first, const_iterator last, const CnString& str)
{
    size_t pos = first - d->data;
    size_t count = last - first;
    return replace(pos, count, str);
}

CnString &CnString::replace(size_t pos, size_t count, const CnChar *str)
{
    size_t sz = d->size;
    if (pos > sz)
        pos = sz;
    if (count > sz - pos)
        count = sz - pos;
    erase(pos, count);
    insert(pos, str, CnStd::strlen(str));
    return *this;
}

CnString &CnString::replace(const_iterator first, const_iterator last, const CnChar *str)
{
    size_t pos = first - d->data;
    size_t count = last - first;
    return replace(pos, count, str);
}

CnString &CnString::replace(size_t pos, size_t count, size_t count2, CnChar ch)
{
    size_t sz = d->size;
    if (pos > sz)
        pos = sz;
    if (count > sz - pos)
        count = sz - pos;
    erase(pos, count);
    insert(pos, count2, ch);
    return *this;
}

CnString &CnString::replace(const_iterator first, const_iterator last, size_t count2, CnChar ch)
{
    size_t pos = first - d->data;
    size_t count = last - first;
    return replace(pos, count, count2, ch);
}

CnString &CnString::replace(size_t pos, CnChar ch)
{
    size_t sz = d->size;
    if (pos >= sz)
        return *this;
    if (d->ref > 1)
    {
        CnStringPrivate* nd = new CnStringPrivate(d->data, d->size);
        d->decRef();
        d = nd;
    }
    d->data[pos] = ch;
    return *this;
}

void CnString::swap(CnString& other)
{
    CnStringPrivate* tmp = d;
    d = other.d;
    other.d = tmp;
}

void CnString::clear()
{    
    CnStringPrivate* old = d;
    nullCnStringData.incRef();
    d = &nullCnStringData;
    old->decRef();
}

void CnString::resize(size_t size)
{
    if (d->size == size)
        return;
    if (d->ref > 1)
    {
        CnStringPrivate* nd = new CnStringPrivate(d->data, d->size);
        d->decRef();
        d = nd;
    }
    if (size == 0)
    {
        clear();
        return;
    }
    CnChar* newData = (CnChar*)calloc(size + 1, sizeof(CnChar));
    size_t copySize = (d->size < size) ? d->size : size;
    if (d->data && copySize > 0)
        memcpy(newData, d->data, copySize * sizeof(CnChar));
    if (size > copySize)
        memset(newData + copySize, 0, (size - copySize) * sizeof(CnChar));
    newData[size] = CnCHR('\0');
    //d->decRef();
    d->data = newData;
    d->size = size;
}

// Search
size_t CnString::find(const CnString& str, size_t pos) const
{
    return find(str.data(), pos, str.size());
}

size_t CnString::find(const CnChar* s, size_t pos, size_t count) const
{
    if (!d->data || !s || count == 0 || pos > d->size)
        return npos;
    for (size_t i = pos; i + count <= d->size; ++i)
    {
        size_t j = 0;
        while (j < count && d->data[i + j] == s[j])
            ++j;
        if (j == count)
            return i;
    }
    return npos;
}

size_t CnString::find(const CnChar* s, size_t pos) const
{
    return find(s, pos, CnStd::strlen(s));
}

size_t CnString::find(CnChar ch, size_t pos) const
{
    if (!d->data || pos >= d->size)
        return npos;
    for (size_t i = pos; i < d->size; ++i)
    {
        if (d->data[i] == ch)
            return i;
    }
    return npos;
}

size_t CnString::rfind(const CnString& str, size_t pos) const
{
    return rfind(str.data(), pos, str.size());
}

size_t CnString::rfind(const CnChar* s, size_t pos, size_t count) const
{
    if (!d->data || !s || count == 0 || d->size < count)
        return npos;
    if (pos > d->size - count)
        pos = d->size - count;
    for (size_t i = pos + 1; i-- > 0; )
    {
        size_t j = 0;
        while (j < count && d->data[i + j] == s[j])
            ++j;
        if (j == count)
            return i;
    }
    return npos;
}

size_t CnString::rfind(const CnChar* s, size_t pos) const
{
    return rfind(s, pos, CnStd::strlen(s));
}

size_t CnString::rfind(CnChar ch, size_t pos) const
{
    if (!d->data || d->size == 0)
        return npos;
    if (pos == npos || pos >= d->size)
        pos = d->size - 1;
    for (size_t i = pos + 1; i-- > 0; )
    {
        if (d->data[i] == ch)
            return i;
    }
    return npos;
}

size_t CnString::find_first_of(const CnString& str, size_t pos) const
{
    return find_first_of(str.data(), pos, str.size());
}

size_t CnString::find_first_of(const CnChar* s, size_t pos, size_t count) const
{
    if (!d->data || !s || count == 0 || pos >= d->size) return npos;
    for (size_t i = pos; i < d->size; ++i)
        for (size_t j = 0; j < count; ++j)
            if (d->data[i] == s[j]) return i;
    return npos;
}

size_t CnString::find_first_of(const CnChar* s, size_t pos) const
{
    return find_first_of(s, pos, CnStd::strlen(s));
}

size_t CnString::find_first_of(CnChar ch, size_t pos) const
{
    if (!d->data || pos >= d->size) return npos;
    for (size_t i = pos; i < d->size; ++i)
        if (d->data[i] == ch) return i;
    return npos;
}

size_t CnString::find_first_not_of(const CnString& str, size_t pos) const
{
    return find_first_not_of(str.data(), pos, str.size());
}

size_t CnString::find_first_not_of(const CnChar* s, size_t pos, size_t count) const
{
    if (!d->data || !s || count == 0 || pos >= d->size)
        return npos;
    for (size_t i = pos; i < d->size; ++i)
    {
        bool found = false;
        for (size_t j = 0; j < count; ++j)
        {
            if (d->data[i] == s[j]) 
            {
                found = true;
                break; 
            }
        }
        if (!found)
            return i;
    }
    return npos;
}

size_t CnString::find_first_not_of(const CnChar* s, size_t pos) const
{
    return find_first_not_of(s, pos, CnStd::strlen(s));
}

size_t CnString::find_first_not_of(CnChar ch, size_t pos) const
{
    if (!d->data || pos >= d->size)
        return npos;
    for (size_t i = pos; i < d->size; ++i)
    {
        if (d->data[i] != ch)
            return i;
    }
    return npos;
}

size_t CnString::find_last_of(const CnString& str, size_t pos) const
{
    return find_last_of(str.data(), pos, str.size());
}

size_t CnString::find_last_of(const CnChar* brk, size_t pos, size_t brkcount) const
{
    if (!d->data || !brk || brkcount == 0 || d->size == 0)
        return npos;
    if (pos >= d->size)
        pos = d->size - 1;
    return find_last_of_chars(d->data, brk, pos, brkcount);
}

size_t CnString::find_last_of(const CnChar* s, size_t pos) const
{
    return find_last_of(s, pos, CnStd::strlen(s));
}

size_t CnString::find_last_of(CnChar ch, size_t pos) const
{
    if (!d->data || d->size == 0)
        return npos;
    if (pos == npos || pos >= d->size)
        pos = d->size - 1;
    for (size_t i = pos + 1; i-- > 0; )
    {
        if (d->data[i] == ch)
            return i;
    }
    return npos;
}

size_t CnString::find_last_not_of(const CnString& str, size_t pos) const
{
    return find_last_not_of(str.data(), pos, str.size());
}

size_t CnString::find_last_not_of(const CnChar* s, size_t pos, size_t count) const
{
    if (!d->data || !s || count == 0 || d->size == 0)
        return npos;
    if (pos == npos || pos >= d->size)
        pos = d->size - 1;
    for (size_t i = pos + 1; i-- > 0; )
    {
        bool found = false;
        for (size_t j = 0; j < count; ++j)
        {
            if (d->data[i] == s[j])
            {
                found = true;
                break;
            }
        }
        if (!found)
            return i;
    }
    return npos;
}

size_t CnString::find_last_not_of(const CnChar* s, size_t pos) const
{
    return find_last_not_of(s, pos, CnStd::strlen(s));
}

size_t CnString::find_last_not_of(CnChar ch, size_t pos) const
{
    if (!d->data || d->size == 0)
        return npos;
    if (pos == npos || pos >= d->size)
        pos = d->size - 1;
    for (size_t i = pos + 1; i-- > 0; )
    {
        if (d->data[i] != ch)
            return i;
    }
    return npos;
}

// Operations
int CnString::compare(size_t pos1, size_t count1, const CnChar* s, size_t count2) const
{
    // Adjust pos1 and count1 to be within bounds
    if (pos1 > d->size)
        pos1 = d->size;
    
    if (count1 > d->size - pos1)
        count1 = d->size - pos1;
    
    // If count2 is npos, use strlen to determine the length of s
    if (count2 == npos)
        count2 = CnStd::strlen(s);
    
    // Determine the length of the comparison
    size_t rlen = (count1 < count2) ? count1 : count2;
    
    // Perform the comparison
    int result = 0;
    if (rlen > 0)
    {
        result = CnStd::strncmp(d->data + pos1, s, rlen);
    }
    
    // If the compared parts are equal, the result depends on the lengths
    if (result == 0)
    {
        if (count1 < count2)
            return -1;
        else if (count1 > count2)
            return 1;
    }
    
    return result;
}

int CnString::compare(const CnChar *s) const
{
    return compare(0, d->size, s);
}

int CnString::compare(size_t pos1, size_t count1, const CnString &str) const
{
    return compare(pos1, count1, str.data(), str.size());
}

int CnString::compare(const CnString& str) const
{
    return compare(0, d->size, str.data(), str.size());
}

bool CnString::starts_with(CnChar ch) const
{
    return d->size > 0 && d->data[0] == ch;
}

bool CnString::starts_with(const CnChar *str) const
{
    if (!d->data || !str)
        return false;
    size_t len = CnStd::strlen(str);
    if (d->size < len)
        return false;
    for (size_t i = 0; i < len; ++i)
    {
        if (d->data[i] != str[i])
            return false;
    }
    return true;
}

bool CnString::starts_with(const CnString &str) const
{
    return starts_with(str.data());
}

bool CnString::ends_with(CnChar ch) const
{
    return d->size > 0 && d->data[d->size - 1] == ch;
}

bool CnString::ends_with(const CnChar *str) const
{
    if (!d->data || !str)
        return false;
    size_t len = CnStd::strlen(str);
    if (d->size < len)
        return false;
    for (size_t i = 0; i < len; ++i)
    {
        if (d->data[d->size - len + i] != str[i])
            return false;
    }
    return true;
}

bool CnString::ends_with(const CnString &str) const
{
    return ends_with(str.data());
}

bool CnString::constains(CnChar ch) const
{
    if (!d->data)
        return false;
    for (size_t i = 0; i < d->size; ++i)
    {
        if (d->data[i] == ch)
            return true;
    }
    return false;
}

bool CnString::constains(const CnChar *str) const
{
    return find(str) != npos;
}

bool CnString::constains(const CnString &str) const
{
    return find(str) != npos;
}

CnString CnString::substr(size_t pos, size_t count) const
{
    if (!d->data || pos >= d->size)
        return CnString();
    if (count == npos || pos + count > d->size)
        count = d->size - pos;
    return CnString(d->data + pos, count);
}

const char *CnString::chars() const
{
#ifdef CN_USE_WSTRING
    tls_strbuffer = this->toStdString();
    return tls_strbuffer.c_str();
#else
    return d->data;
#endif
}

const wchar_t *CnString::wchars() const
{
#ifdef CN_USE_WSTRING
    return d->data;
#else
    tls_strbuffer = this->toStdWString();
    return tls_strbuffer.c_str();
#endif
}

std::string CnString::toStdString() const
{
    if (d->isNull())
        return std::string();
#ifdef CN_USE_WSTRING
    return std::wstring_convert<std::codecvt_utf8<wchar_t> >().to_bytes(d->data, d->data + d->size);
#else
    return std::string(d->data, d->size);
#endif
}

std::wstring CnString::toStdWString() const
{
    if (d->isNull())
        return std::wstring();
#ifdef CN_USE_WSTRING
    return std::wstring(d->data, d->size);
#else
    return std::wstring_convert<std::codecvt_utf8<wchar_t> >().from_bytes(d->data, d->data + d->size);
#endif
}

// static conversions
CnString CnString::fromStdString(const std::string &str)
{
#ifdef CN_USE_WSTRING
    std::wstring s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
    return CnString(s.c_str(), s.size());
#else
    return CnString(str.c_str(), str.size());
#endif
}

CnString CnString::fromStdWString(const std::wstring &str)
{
#ifdef CN_USE_WSTRING
    return CnString(str.c_str(), str.size());
#else
    std::string s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
    return CnString(s.c_str(), s.size());
#endif
}
    
CnString CnString::fromChars(const char* str)
{
#ifdef CN_USE_WSTRING
    std::wstring s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
    return CnString(s.c_str(), s.size());
#else
    return CnString(str);
#endif
}

CnString CnString::fromChars(const char *str, size_t len)
{
#ifdef CN_USE_WSTRING
    std::wstring s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str, str+len);
    return CnString(s.c_str(), s.size());
#else
    return CnString(str, len);
#endif
}

CnString CnString::fromWChars(const wchar_t* str)
{
#ifdef CN_USE_WSTRING
    return CnString(str);
#else
    std::string s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
    return CnString(s.c_str(), s.size());
#endif
}

CnString CnString::fromWChars(const wchar_t * str, size_t len)
{
#ifdef CN_USE_WSTRING
    return CnString(str, len);
#else
    std::string s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str, str+len);
    return CnString(s.c_str(), s.size());
#endif
}

size_t CnString::find_last_of_chars(const CnChar *chars, const CnChar *brk, size_t pos, size_t brkcount)
{
    for (size_t i = pos + 1; i-- > 0; )
    {
        for (size_t j = 0; j < brkcount; ++j)
        {
            if (chars[i] == brk[j])
                return i;
        }
    }
    return npos;
}

// Input/Output stream operators
std::basic_ostream<CnChar>& operator<<(std::basic_ostream<CnChar>& os, const CnString& str)
{
    for (size_t i = 0; i < str.size(); ++i)
        os << str[i];
    return os;
}

std::basic_istream<CnChar>& operator>>(std::basic_istream<CnChar>& is, CnString& str)
{
    str.clear();
    CnChar ch;
    while (is.get(ch))
    {
        if (ch == '\n' || ch == '\r')
            break;
        str.push_back(ch);
    }
    return is;
}
