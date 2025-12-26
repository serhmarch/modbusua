#include "global_test.h"

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
