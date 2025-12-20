#include "gtest/gtest.h"
#include <device/CnDeviceStat.h>

TEST(CnDeviceStatTest, DefaultConstructor)
{
    CnDeviceStat stat;
    EXPECT_EQ(stat.StatState(), 0);
    EXPECT_EQ(stat.StatInnerState(), 0);
    EXPECT_FALSE(stat.StatStateEnableDevice());
    EXPECT_FALSE(stat.StatStateIncluded());
}

TEST(CnDeviceStatTest, SetAndGetModbusStat)
{
    CnDeviceStat stat;
    CnTimestamp expectedTime;
    CnString expectedErr(CnSTR("expectedError"));
    Modbus::StatusCode expectedStatus = Modbus::Status_Uncertain;
    uint32_t expectedGoodCount = 0, expectedBadCount = 0;

    EXPECT_EQ(stat.StatCommonGoodCount           (), expectedGoodCount);
    EXPECT_EQ(stat.StatCommonBadCount            (), expectedBadCount );
    EXPECT_EQ(stat.StatCommonStatus              (), expectedStatus   );
    EXPECT_EQ(stat.StatCommonTimestamp           (), expectedTime     );
    EXPECT_EQ(stat.StatCommonLastSuccessTimestamp(), expectedTime     );
    EXPECT_EQ(stat.StatCommonLastErrorStatus     (), expectedStatus   );
    EXPECT_EQ(stat.StatCommonLastErrorTimestamp  (), expectedTime     );
    EXPECT_EQ(stat.StatCommonLastErrorText       (), CnString()       );

    //--------------------------------------------------------------------------
    // StatPoke
    //--------------------------------------------------------------------------
    expectedStatus = Modbus::Status_Good;
    ++expectedGoodCount;
    expectedTime += 1111;
    stat.setStatPokeResult(expectedStatus, expectedTime);
    EXPECT_EQ(stat.StatPokeGoodCount           (), 1);
    EXPECT_EQ(stat.StatPokeBadCount            (), 0);
    EXPECT_EQ(stat.StatPokeStatus              (), expectedStatus);
    EXPECT_EQ(stat.StatPokeTimestamp           (), expectedTime); 
    EXPECT_EQ(stat.StatPokeLastSuccessTimestamp(), expectedTime);  
    //--------------------------------------------------------------------------
    EXPECT_EQ(stat.StatCommonGoodCount           (), expectedGoodCount);
    EXPECT_EQ(stat.StatCommonBadCount            (), expectedBadCount );
    EXPECT_EQ(stat.StatCommonStatus              (), expectedStatus);
    EXPECT_EQ(stat.StatCommonTimestamp           (), expectedTime);    
    EXPECT_EQ(stat.StatCommonLastSuccessTimestamp(), expectedTime);

    expectedStatus = Modbus::Status_Bad;
    ++expectedBadCount;
    expectedTime += 1111;
    stat.setStatPokeResult(expectedStatus, expectedTime, expectedErr);
    EXPECT_EQ(stat.StatPokeGoodCount         (), 1);
    EXPECT_EQ(stat.StatPokeBadCount          (), 1);
    EXPECT_EQ(stat.StatPokeStatus            (), expectedStatus);
    EXPECT_EQ(stat.StatPokeTimestamp         (), expectedTime);    
    EXPECT_EQ(stat.StatPokeLastErrorStatus   (), expectedStatus);
    EXPECT_EQ(stat.StatPokeLastErrorTimestamp(), expectedTime);
    EXPECT_EQ(stat.StatPokeLastErrorText     (), expectedErr);
    //--------------------------------------------------------------------------
    EXPECT_EQ(stat.StatCommonGoodCount           (), expectedGoodCount);
    EXPECT_EQ(stat.StatCommonBadCount            (), expectedBadCount );
    EXPECT_EQ(stat.StatCommonStatus              (), expectedStatus);
    EXPECT_EQ(stat.StatCommonTimestamp           (), expectedTime);
    EXPECT_EQ(stat.StatCommonLastErrorStatus     (), expectedStatus);
    EXPECT_EQ(stat.StatCommonLastErrorTimestamp  (), expectedTime);
    EXPECT_EQ(stat.StatCommonLastErrorText       (), expectedErr);

    //--------------------------------------------------------------------------
    // StatPoll
    //--------------------------------------------------------------------------
    expectedStatus = Modbus::Status_Good;
    ++expectedGoodCount;
    expectedTime += 1111;
    stat.setStatPollResult(expectedStatus, expectedTime);
    EXPECT_EQ(stat.StatPollGoodCount           (), 1);
    EXPECT_EQ(stat.StatPollBadCount            (), 0);
    EXPECT_EQ(stat.StatPollStatus              (), expectedStatus);
    EXPECT_EQ(stat.StatPollTimestamp           (), expectedTime); 
    EXPECT_EQ(stat.StatPollLastSuccessTimestamp(), expectedTime);  
    //--------------------------------------------------------------------------
    EXPECT_EQ(stat.StatCommonGoodCount           (), expectedGoodCount);
    EXPECT_EQ(stat.StatCommonBadCount            (), expectedBadCount );
    EXPECT_EQ(stat.StatCommonStatus              (), expectedStatus);
    EXPECT_EQ(stat.StatCommonTimestamp           (), expectedTime);    
    EXPECT_EQ(stat.StatCommonLastSuccessTimestamp(), expectedTime);

    expectedStatus = Modbus::Status_Bad;
    ++expectedBadCount;
    expectedTime += 1111;
    stat.setStatPollResult(expectedStatus, expectedTime, expectedErr);
    EXPECT_EQ(stat.StatPollGoodCount         (), 1);
    EXPECT_EQ(stat.StatPollBadCount          (), 1);
    EXPECT_EQ(stat.StatPollStatus            (), expectedStatus);
    EXPECT_EQ(stat.StatPollTimestamp         (), expectedTime);    
    EXPECT_EQ(stat.StatPollLastErrorStatus   (), expectedStatus);
    EXPECT_EQ(stat.StatPollLastErrorTimestamp(), expectedTime);
    EXPECT_EQ(stat.StatPollLastErrorText     (), expectedErr);
    //--------------------------------------------------------------------------
    EXPECT_EQ(stat.StatCommonGoodCount           (), expectedGoodCount);
    EXPECT_EQ(stat.StatCommonBadCount            (), expectedBadCount );
    EXPECT_EQ(stat.StatCommonStatus              (), expectedStatus);
    EXPECT_EQ(stat.StatCommonTimestamp           (), expectedTime);
    EXPECT_EQ(stat.StatCommonLastErrorStatus     (), expectedStatus);
    EXPECT_EQ(stat.StatCommonLastErrorTimestamp  (), expectedTime);
    EXPECT_EQ(stat.StatCommonLastErrorText       (), expectedErr);

}