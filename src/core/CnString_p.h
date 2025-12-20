/*!
 * \file   CnString.h
 * \brief  Містить клас CnString для роботи з рядками тексту
 *
 * \author serhmarch
 * \date   September 2025
 */
#ifndef CNSTRING_P_H
#define CNSTRING_P_H

#include <CnCore.h>
#include <string.h>

const CnChar* const CN_STRING_NULL = CnSTR("");

class CnStringPrivate
{
public:
    CnStringPrivate() 
    {
        this->ref = 1;
        this->size = 0;
        this->data = const_cast<CnChar*>(CN_STRING_NULL);
    }

    CnStringPrivate(const CnChar *str, size_t size) 
    {
        this->ref = 1;
        this->size = size;
        this->data = (CnChar*)::calloc(size+1, sizeof(CnChar)); //new CnChar[size+1];
        memcpy(this->data, str, size);
        this->data[size] = CnCHR('\0');
    }

    ~CnStringPrivate() 
    {
        if (this->data != CN_STRING_NULL)
            ::free(this->data); //delete[] this->data;
    }

public:
    inline void incRef() { ++ref; }
    inline void decRef() { if (--ref < 1) delete this; }
    inline bool isNull() const { return data == CN_STRING_NULL; }
    inline bool isEmpty() const { return size == 0; }

public:
    CnRefCount_t ref;
    size_t size;
    CnChar* data;    
};

#endif // CNSTRING_P_H