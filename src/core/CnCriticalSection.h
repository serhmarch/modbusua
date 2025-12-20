/*!
 * \file   CnCriticalSection.h
 * \brief  Contains the description of the critical section (mutex) class
 *
 * \author serhmarch
 * \date   September 2025
 */
#ifndef CNCRITICALSECTION_H
#define CNCRITICALSECTION_H

#include <CnCore.h>

/*! \brief The `CnCriticalSection` class is designed to provide exclusive access to a resource in a multithreaded environment.

    \details The `CnCriticalSection` class implements mutex functionality and is a wrapper for the WinAPI `CRITICAL_SECTION` structure, simplifying its usage.\n
    An object of the `CnCriticalSection` class locks a resource by calling the `lock()` function, or if the resource is already locked by another thread, waits until it is released.
    After the resource has been used, it must be released using the `unlock()` function so that another thread can access the resource.
    \code{.cpp}
    CnCriticalSection cs;
    cs.lock();
    // do something with resource ...
    cs.unlock();
    \endcode
    \sa CnCriticalSectionLocker
*/

class CnCriticalSection
{
public:
    ///\details Constructor of the `CnCriticalSection` class. Initializes the mutex object in an unlocked state using the WinAPI function `InitializeCriticalSection`.
    CnCriticalSection()
    { 
#ifdef WIN32
        InitializeCriticalSection(&m_cs);
#else
        pthread_mutexattr_init(&m_attr);
        pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_cs, &m_attr);
        pthread_mutexattr_destroy(&m_attr);
#endif
    }

    ///\details Destructor of the `CnCriticalSection` class. Releases the mutex object using the WinAPI function `DeleteCriticalSection`.
    ~CnCriticalSection()
    {
#ifdef WIN32
        DeleteCriticalSection(&m_cs);
#else
        pthread_mutex_destroy(&m_cs);
#endif
    }

public:
    ///\details This function locks the mutex object using the WinAPI function `EnterCriticalSection`. If the mutex is already locked by another thread, the function waits until it is unlocked.
    inline void lock()
    {
#ifdef WIN32
        EnterCriticalSection(&m_cs);
#else
        pthread_mutex_lock(&m_cs);
#endif
    }

    ///\details This function unlocks the mutex object using the WinAPI function `LeaveCriticalSection`.
    inline void unlock()
    {
#ifdef WIN32
        LeaveCriticalSection(&m_cs);
#else
        pthread_mutex_unlock(&m_cs);
#endif
    }

private:
#ifdef WIN32
    CRITICAL_SECTION m_cs;
#else
    pthread_mutex_t m_cs;
    pthread_mutexattr_t m_attr;
#endif
};


/*! \brief The `CnCriticalSectionLocker` class simplifies the use of the `CnCriticalSection` class.

    \details Locking and unlocking `CnCriticalSection` in complex functions, operators, or exception handling code can lead to errors and is difficult to debug.
    `CnCriticalSectionLocker` can be used in such situations to ensure that the mutex state is always clearly defined.\n
    `CnCriticalSectionLocker` should be created inside the function where you need to lock the `CnCriticalSection`.
    After creating `CnCriticalSectionLocker`, the mutex will be locked.
    You can unlock and relock the mutex using the unlock() and relock() functions.
    If the mutex is locked, it will be unlocked when `CnCriticalSectionLocker` is destroyed.\n
    For example, this complex function locks the `CnCriticalSection` upon entering the function and unlocks the mutex at all exit points:
    \code{.cpp}
    CnCriticalSection mutex;
    int complexFunction(int flag)
    {
        mutex.lock();

        int retVal = 0;

        switch (flag) {
        case 0:
        case 1:
            retVal = moreComplexFunction(flag);
            break;
        case 2:
            {
                int status = anotherFunction();
                if (status < 0) {
                    mutex.unlock();
                    return -2;
                }
                retVal = status + flag;
            }
            break;
        default:
            if (flag > 10) {
                mutex.unlock();
                return -1;
            }
            break;
        }

        mutex.unlock();
        return retVal;
    }    
    \endcode
    This function example will become more complicated as it is developed, increasing the likelihood of errors.\n
    Using `CnCriticalSectionLocker` greatly simplifies the code and makes it more readable:
    \code{.cpp}
    CnCriticalSection mutex;
    int complexFunction(int flag)
    {
        CnCriticalSectionLocker locker(&mutex);

        int retVal = 0;

        switch (flag) {
        case 0:
        case 1:
            return moreComplexFunction(flag);
        case 2:
            {
                int status = anotherFunction();
                if (status < 0)
                    return -2;
                retVal = status + flag;
            }
            break;
        default:
            if (flag > 10)
                return -1;
            break;
        }

        return retVal;
    }
    \endcode
    \sa CnCriticalSection
*/

class CnCriticalSectionLocker
{
public:
    ///\details Creates a `CnCriticalSectionLocker` and locks the mutex. The mutex will be unlocked (unlock() will be called) when the `CnCriticalSectionLocker` is destroyed.
    CnCriticalSectionLocker(CnCriticalSection *cs) : m_cs(cs) { m_cs->lock(); }

    ///\details Destroys the `CnCriticalSectionLocker` and unlocks the mutex that was locked in the constructor.
    ~CnCriticalSectionLocker() { m_cs->unlock(); }

public:
    ///\details This function returns a pointer to the mutex object passed to the constructor.
    inline CnCriticalSection *criticalSection() const { return m_cs; }

    ///\details This function locks a previously unlocked mutex object.
    inline void relock() { m_cs->lock(); }

    ///\details This function unlocks the mutex object.
    inline void unlock() { m_cs->unlock(); }

private:
    CnCriticalSection *m_cs;
};


#endif // CNCRITICALSECTION_H