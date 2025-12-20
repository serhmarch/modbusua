/*!
 * \file   CnUaThread.h
 * \brief  
 * 
 * \author serhmarch
 * \date   December 2024
 */
#ifndef CNUATHREAD_H
#define CNUATHREAD_H

#include <CnThread.h>

class CnUaServer;


/*! \brief 

    \details 
*/

class CnUaThread : public CnThread
{
public:
    /// \details . 
    CnUaThread(CnUaServer *server);

public:
    /// \details . 
    bool start() override;

    /// \details . 
    void waitForStop() override;

protected:
    /// \details
    int run() override;

private:
    CnUaServer *m_server;
};

#endif // CNUATHREAD_H
