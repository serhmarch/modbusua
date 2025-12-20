#include <CnThread.h>

#include <pthread.h>
#include <unistd.h>

#include "CnUnix.h"

#include <CnEvent.h>

static thread_local std::unique_ptr<CnThread> t_wrapper;

class CnThreadData
{
public:
    static void* run(void *lpParameter)
    {
        CnThread *thread = reinterpret_cast<CnThread*>(lpParameter);
        t_wrapper = std::unique_ptr<CnThread>(thread);
        int r = thread->run();
        t_wrapper.release();
        return reinterpret_cast<void*>(r);
    }
};

CnThread* CnThread::currentThread()
{
    if (!t_wrapper)
        t_wrapper = std::make_unique<CnThread>();
    return t_wrapper.get();
}

CnThread::Handle CnThread::currentThreadId()
{
    return reinterpret_cast<CnThread::Handle>(pthread_self());
}

CnThread::~CnThread()
{
    if (m_threadHandle)
        pthread_detach(reinterpret_cast<pthread_t>(m_threadHandle));
    for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); ++it)
        delete it->second;    
}

bool CnThread::isRunning() const
{
    if (m_threadHandle == 0)
        return false;
    void* status;
    int result = pthread_tryjoin_np(reinterpret_cast<pthread_t>(m_threadHandle), &status);    
    return result != 0;
}

bool CnThread::start()
{
    if (isRunning())
        return true;
    if (m_threadHandle)
        pthread_detach(reinterpret_cast<pthread_t>(m_threadHandle));
    m_ctrlRun = true;
    int result = pthread_create(reinterpret_cast<pthread_t*>(&m_threadHandle),
                                NULL,
                                CnThreadData::run,
                                this);
    return result == 0;
}

void CnThread::stop()
{
    m_ctrlRun = false;
}

void CnThread::waitForStop()
{
    while (isRunning())
    {
        stop();
        sleep(0);
    }
}

