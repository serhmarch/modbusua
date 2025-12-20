#include <CnThread.h>

#include <CnEvent.h>

static thread_local std::unique_ptr<CnThread> t_wrapper;

class CnThreadData
{
public:
    static DWORD WINAPI run(LPVOID lpParameter)
    {
        CnThread *thread = reinterpret_cast<CnThread*>(lpParameter);
        t_wrapper = std::unique_ptr<CnThread>(thread);
        int r = thread->run();
        t_wrapper.release();
        return static_cast<DWORD>(r);
    }
};

CnThread::Handle CnThread::currentThreadId()
{
    HANDLE realHandle;
    DuplicateHandle(GetCurrentProcess(),
                    GetCurrentThread(),
                    GetCurrentProcess(),
                    &realHandle,
                    0,
                    FALSE,
                    DUPLICATE_SAME_ACCESS);
    return realHandle;
}

CnThread* CnThread::currentThread()
{
    if (!t_wrapper)
        t_wrapper = std::make_unique<CnThread>();
    return t_wrapper.get();
}

CnThread::~CnThread()
{
    if (m_threadHandle)
        CloseHandle(m_threadHandle);
    for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); ++it)
        delete it->second;    
}

bool CnThread::isRunning() const
{
    if (m_threadHandle == NULL)
        return false;
    DWORD lpExitCode;
    if (GetExitCodeThread(m_threadHandle, &lpExitCode))
        return lpExitCode == STILL_ACTIVE;
    return false;
}

bool CnThread::start()
{
    if (isRunning())
        return true;
    if (m_threadHandle)
        CloseHandle(m_threadHandle);
    m_ctrlRun = true;
    m_threadHandle = CreateThread(NULL,              // default security attributes
                                  0,                 // use default stack size  
                                  CnThreadData::run, // thread function name
                                  this,              // argument to thread function 
                                  0,                 // use default creation flags 
                                  NULL);             // returns the thread identifier 
    return m_threadHandle != NULL;
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
        Sleep(0);
    }
}
