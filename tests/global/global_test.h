/*!
 * \file   global_test.h
 * \brief  Global test environment for unit tests
 *
 * \author serhmarch
 * \date   December 2025
 */
#include <gtest/gtest.h>
#include <cfg/CnCfgPort.h>
#include <cfg/CnCfgDevice.h>
#include <cfg/CnCfgDeviceItem.h>
#include <app/CnApp.h>
#include <port/CnPort.h>
#include <device/CnDevice.h>


// Test-only implementation of CnApp for testing purposes
class CnAppTest : public CnApp
{
public:
    CnAppTest() : CnApp() 
    {
    }

protected:
    CnPort *createPort(const CnCfgPort */*cfg*/) override { return nullptr; }
    CnDevice *createDevice(const CnCfgDevice */*cfg*/) override { return nullptr; }
};
