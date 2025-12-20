#include "CnUaThread.h"

#include <CnLog.h>
#include <CnEventLoop.h>

#include <open62541/server_config_default.h>
#include <CnUaServer.h>

CnUaThread::CnUaThread(CnUaServer *server) :
    m_server(server)
{
}

bool CnUaThread::start()
{
    CN_LOG_ThreadStartStop(CnSTR("CnUaThread::start()"));
    return CnThread::start();
}

void CnUaThread::waitForStop()
{
    CN_LOG_ThreadStartStop(CnSTR("CnUaThread::waitForStop()"));
    CnThread::waitForStop();
    CN_LOG_ThreadStartStop(CnSTR("CnUaThread: stopped"));
}

int CnUaThread::run()
{
    CnEventLoop loop;
    CN_LOG_ThreadStartStop(CnSTR("CnUaThread::run(): begin"));
    UA_Server *server = m_server->server();
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_UInt16 tcpPort;
    m_ctrlRun = true;
    while (m_ctrlRun)
    {
        UA_ServerConfig *config = UA_Server_getConfig(server);
        tcpPort = m_server->tcpPort();
        UA_ServerConfig_setMinimal(config, tcpPort, NULL);
        /* Run the server */
        retval = UA_Server_run_startup(server);
        if (retval != UA_STATUSCODE_GOOD)
        {
            CN_LOG_Error(CnSTR("UA_Server_run_startup() failed"));
            return retval;        
        }

        while (m_ctrlRun)
        {
            loop.processEvents();
            if (tcpPort != m_server->tcpPort())
                break;
            UA_Server_run_iterate(server, true);
        }

        /* Shut down the server */
        retval = UA_Server_run_shutdown(server);
    }   
    return retval;        
}
