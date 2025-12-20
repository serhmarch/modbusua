/*!
 * \file   CnPortRunnable.h
 * \brief  Class `CnPortRunnable` implements the logic for one Modbus port
 * 
 * \author serhmarch
 * \date   November 2024
 */
#ifndef CNPORTRUNNABLE_H
#define CNPORTRUNNABLE_H

#include <ModbusClientPort.h>

#include <CnGlobal.h>
#include <CnObject.h>

class CnPort;
class CnDevice;
class CnDeviceRunnable;
class CnLoggerManager;

/*! \brief Class `CnPortRunnable` implements the logic for one Modbus port.

*/

class CnPortRunnable : public CnObject
{
public:
    /// \details Constructs the runnable for a single Modbus port.
    CnPortRunnable(CnPort *port);

    /// \details Destructs the runnable for a single Modbus port.
    ~CnPortRunnable();

public:
    /// \details Runs the algorithm for a single Modbus port.
    void run();

private:
    void processToDelete();
    void addDevice(CnDevice* device);
    void removeDevice(CnDevice* device);

private:
    CnPort* m_port;

private: // Modbus
    ModbusClientPort* m_clientPort;

private: // CnDeviceRunnable
    typedef CnList<CnDeviceRunnable*> Sequence_t;
    Sequence_t m_sequence;
    CnSet<CnDeviceRunnable*> m_devicesToDelete;

    Sequence_t::const_iterator findRunnableIterator(CnDevice* device) const;
    inline CnDeviceRunnable* findRunnable(CnDevice* device) const { return *findRunnableIterator(device); }
    void checkSynchConfig();
    void synchConfig(const Cn::Config &config);

private:
    void slotTx(const Modbus::Char *source, const uint8_t* buff, uint16_t size);
    void slotRx(const Modbus::Char *source, const uint8_t* buff, uint16_t size);

private:
    CnLoggerManager *m_logger;
    uint32_t m_configChangeCounter;
    uint64_t m_tmLastRequest;
    CnList<CnDeviceRunnable*> m_pokeQueue;
};

#endif // CNPORTRUNNABLE_H
