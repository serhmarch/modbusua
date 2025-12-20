/*!
 * \file   CnCfgBuilder_test.cpp
 * \brief  Unit tests for CnCfgBuilder class
 *
 * \author GitHub Copilot
 * \date   November 2024
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
        int a = 0;
        int b;
        b = a;
    }

protected:
    CnPort *createPort(const CnCfgPort */*cfg*/) override { return nullptr; }
    CnDevice *createDevice(const CnCfgDevice */*cfg*/) override { return nullptr; }
};

// Global test environment to initialize CnApp once for all tests
class CnGlobalTestEnvironment : public ::testing::Environment
{
public:
    void SetUp() override
    {
        // Initialize CnApp singleton once for all tests
        testApp = new CnAppTest();
    }

    void TearDown() override
    {
        // Clean up CnApp singleton after all tests
        delete testApp;
        testApp = nullptr;
    }

    static CnAppTest *testApp;
};

CnAppTest *CnGlobalTestEnvironment::testApp = nullptr;


// Register the global test environment
::testing::Environment* const cnapp_env = 
    ::testing::AddGlobalTestEnvironment(new CnGlobalTestEnvironment);
