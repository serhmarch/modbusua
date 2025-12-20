#include "gtest/gtest.h"
#include <device/CnDevice.h>

TEST(CnDeviceTest, SetConfig)
{
    CnDevice dev;
    Cn::Config cfg;
    const CnDeviceConfig::Strings &s = CnDeviceConfig::Strings::instance();
    cfg[s.DefaultPeriod] = 1000;
    dev.setConfig(cfg);

    auto DefaultPeriod = dev.CfgDefaultPeriod();
    EXPECT_EQ(DefaultPeriod, 1000);

}
