#ifndef CNSHAREDPOINTER_H
#define CNSHAREDPOINTER_H

#include <CnCore.h>

#include <string>
#include <cstddef>
#include <cstdint>
#include <utility>

template <class T>
class CnSharedPointer
{
public:
    // constructors
    CnSharedPointer()
    {
        m_node = nullptr;
    }

    CnSharedPointer(std::nullptr_t)
    {
        m_node = nullptr;
    }

    template <class X>
    CnSharedPointer(X *ptr)
    {
        if (ptr)
            ptr->IncrementRef();
        m_node = ptr;
    }

    CnSharedPointer(const CnSharedPointer &other)
    {
        if (other.m_node)
            other.m_node->IncrementRef();
        m_node = other.m_node;
    }

    template <class X>
    CnSharedPointer(const CnSharedPointer<X> &other)
    {
        if (other.m_node)
            other.m_node->IncrementRef();
        m_node = other.m_node;
    }

    CnSharedPointer(CnSharedPointer &&other)
    {
        m_node = other.m_node;
        other.m_node = nullptr;
    }

    template <class X>
    CnSharedPointer(CnSharedPointer<X> &&other)
    {
        m_node = other.m_node;
        other.m_node = nullptr;
    }

    ~CnSharedPointer()
    {
        if (m_node)
            m_node->DecrementRef();
    }


public:
    // basic accessor functions
    inline T *data() const { return m_node; }
    inline T *get() const { return data(); }
    inline bool isNull() const { return m_node == nullptr; }
    inline operator bool() const { return m_node != nullptr; }
    inline bool operator!() const { return isNull(); }
    inline T &operator*() const { return *m_node; }
    inline T *operator->() const { return m_node; }
    inline operator T*() const { return data(); }
    inline void swap(CnSharedPointer<T> &other)
    {
        T *n = m_node;
        m_node = other.m_node;
        other.m_node = n;
    }

    inline CnSharedPointer &operator=(const CnSharedPointer &other)
    {
        CnSharedPointer copy(other);
        swap(copy);
        return *this;
    }

    template <class X>
    inline CnSharedPointer &operator=(const CnSharedPointer<X> &other)
    {
        CnSharedPointer copy(other);
        swap(copy);
        return *this;
    }

    inline CnSharedPointer &operator=(const CnSharedPointer &&other)
    {
        CnSharedPointer moved(std::move(other));
        swap(moved);
        return *this;
    }

    template <class X>
    inline CnSharedPointer &operator=(const CnSharedPointer<X> &&other)
    {
        CnSharedPointer moved(std::move(other));
        swap(moved);
        return *this;
    }

    inline void clear()
    {
        if (m_node)
            m_node->DecrementRef();
    }

    inline void reset() { clear(); }

    inline void reset(T *t)
    {
        CnSharedPointer<T> other(t);
        swap(other);
    }

    // casts:
    template <class X> inline CnSharedPointer<X> staticCast()  const { return CnSharedPointer<X>(static_cast<X*>(m_node)); }
    template <class X> inline CnSharedPointer<X> dynamicCast() const { return CnSharedPointer<X>(dynamic_cast<X*>(m_node)); }
    template <class X> inline CnSharedPointer<X> constCast()   const { return CnSharedPointer<X>(const_cast<X*>(m_node)); }

private:
    T *m_node;

public:
};

template<class T, class X> inline bool operator==(const CnSharedPointer<T> &ptr1, const CnSharedPointer<X> &ptr2) { return ptr1.data() == ptr2.data(); }
template<class T, class X> inline bool operator!=(const CnSharedPointer<T> &ptr1, const CnSharedPointer<X> &ptr2) { return ptr1.data() != ptr2.data(); }
template<class T, class X> inline bool operator==(const CnSharedPointer<T> &ptr1, const X *ptr2) { return ptr1.data() == ptr2; }
template<class T, class X> inline bool operator!=(const CnSharedPointer<T> &ptr1, const X *ptr2) { return ptr1.data() != ptr2; }
template<class T, class X> inline bool operator==(const T *ptr1, const CnSharedPointer<X> &ptr2) { return ptr1 == ptr2.data(); }
template<class T, class X> inline bool operator!=(const T *ptr1, const CnSharedPointer<X> &ptr2) { return ptr1 != ptr2.data(); }

#define CN_REF_COUNTING                                                           \
public:                                                                           \
    inline void IncrementRef() { m_refCount++; }                                  \
    inline void DecrementRef() { m_refCount--; if (m_refCount < 1) delete this; } \
private:                                                                          \
    CnRefCount_t m_refCount;                                                      \


#define CN_REF_INIT m_refCount = 0;

namespace std {

template <class T>
struct hash< CnSharedPointer<T> >
{
    std::size_t operator()(const CnSharedPointer<T>& ptr) const
    {
        return std::hash<T*>()(ptr.get());
    }
};

} // namespace std

#endif // CNSHAREDPOINTER_H
