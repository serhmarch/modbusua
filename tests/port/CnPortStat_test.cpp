#include "gtest/gtest.h"
#include <port/CnPortStat.h>

TEST(CnPortStatTest, DefaultConstructor)
{
    CnPortStat stat;
    EXPECT_EQ(stat.StatThreadCycleCount(), 0);
    EXPECT_EQ(stat.StatThreadCycleSum(), 0);
    EXPECT_EQ(stat.StatThreadLastCycle(), 0);
    EXPECT_EQ(stat.StatThreadMinCycle(), 0xFFFFFFFF);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 0);
    EXPECT_EQ(stat.StatThreadAvgCycle(), 0);
    EXPECT_EQ(stat.StatGoodCount(), 0);
    EXPECT_EQ(stat.StatBadCount(), 0);
}

TEST(CnPortStatTest, SetCycleTime)
{
    CnPortStat stat;
    
    stat.setStatCycleTime(100);
    EXPECT_EQ(stat.StatThreadCycleCount(), 1);
    EXPECT_EQ(stat.StatThreadCycleSum(), 100);
    EXPECT_EQ(stat.StatThreadLastCycle(), 100);
    EXPECT_EQ(stat.StatThreadMinCycle(), 100);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 100);
    EXPECT_EQ(stat.StatThreadAvgCycle(), 100);
    
    stat.setStatCycleTime(200);
    EXPECT_EQ(stat.StatThreadCycleCount(), 2);
    EXPECT_EQ(stat.StatThreadCycleSum(), 300);
    EXPECT_EQ(stat.StatThreadLastCycle(), 200);
    EXPECT_EQ(stat.StatThreadMinCycle(), 100);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 200);
    EXPECT_EQ(stat.StatThreadAvgCycle(), 150);
    
    stat.setStatCycleTime(50);
    EXPECT_EQ(stat.StatThreadCycleCount(), 3);
    EXPECT_EQ(stat.StatThreadCycleSum(), 350);
    EXPECT_EQ(stat.StatThreadLastCycle(), 50);
    EXPECT_EQ(stat.StatThreadMinCycle(), 50);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 200);
    EXPECT_EQ(stat.StatThreadAvgCycle(), 116); // 350/3 = 116.666... rounds to 116
}

TEST(CnPortStatTest, SetStatusGood)
{
    CnPortStat stat;
    CnTimestamp timestamp = 1234567890123456789;
    Modbus::StatusCode status = Modbus::Status_Good;
    
    stat.setStatStatus(status, timestamp);
    
    EXPECT_EQ(stat.StatGoodCount(), 1);
    EXPECT_EQ(stat.StatBadCount(), 0);
    EXPECT_EQ(stat.StatStatus(), status);
    EXPECT_EQ(stat.StatTimestamp(), timestamp);
    EXPECT_EQ(stat.StatLastSuccessTimestamp(), timestamp);
    EXPECT_EQ(stat.StatLastErrorStatus(), Modbus::Status_Good);
    EXPECT_EQ(stat.StatLastErrorTimestamp(), CnTimestamp());
    EXPECT_EQ(stat.StatLastErrorText(), CnString());
}

TEST(CnPortStatTest, SetStatusBad)
{
    CnPortStat stat;
    CnTimestamp timestamp = 1234567890123456789;
    Modbus::StatusCode status = Modbus::Status_Bad;
    CnString errorText(CnSTR("Connection failed"));
    
    stat.setStatStatus(status, timestamp, errorText);
    
    EXPECT_EQ(stat.StatGoodCount(), 0);
    EXPECT_EQ(stat.StatBadCount(), 1);
    EXPECT_EQ(stat.StatStatus(), status);
    EXPECT_EQ(stat.StatTimestamp(), timestamp);
    EXPECT_EQ(stat.StatLastSuccessTimestamp(), CnTimestamp());
    EXPECT_EQ(stat.StatLastErrorStatus(), status);
    EXPECT_EQ(stat.StatLastErrorTimestamp(), timestamp);
    EXPECT_EQ(stat.StatLastErrorText(), errorText);
}

TEST(CnPortStatTest, SetStatusMixed)
{
    CnPortStat stat;
    CnTimestamp timestamp1 = 1000000000000000000;
    CnTimestamp timestamp2 = 2000000000000000000;
    CnTimestamp timestamp3 = 3000000000000000000;
    CnString errorText(CnSTR("Timeout error"));
    
    // First good status
    stat.setStatStatus(Modbus::Status_Good, timestamp1);
    EXPECT_EQ(stat.StatGoodCount(), 1);
    EXPECT_EQ(stat.StatBadCount(), 0);
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Good);
    EXPECT_EQ(stat.StatTimestamp(), timestamp1);
    EXPECT_EQ(stat.StatLastSuccessTimestamp(), timestamp1);
    
    // Bad status
    stat.setStatStatus(Modbus::Status_Bad, timestamp2, errorText);
    EXPECT_EQ(stat.StatGoodCount(), 1);
    EXPECT_EQ(stat.StatBadCount(), 1);
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Bad);
    EXPECT_EQ(stat.StatTimestamp(), timestamp2);
    EXPECT_EQ(stat.StatLastSuccessTimestamp(), timestamp1);
    EXPECT_EQ(stat.StatLastErrorStatus(), Modbus::Status_Bad);
    EXPECT_EQ(stat.StatLastErrorTimestamp(), timestamp2);
    EXPECT_EQ(stat.StatLastErrorText(), errorText);
    
    // Another good status
    stat.setStatStatus(Modbus::Status_Good, timestamp3);
    EXPECT_EQ(stat.StatGoodCount(), 2);
    EXPECT_EQ(stat.StatBadCount(), 1);
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Good);
    EXPECT_EQ(stat.StatTimestamp(), timestamp3);
    EXPECT_EQ(stat.StatLastSuccessTimestamp(), timestamp3);
    EXPECT_EQ(stat.StatLastErrorStatus(), Modbus::Status_Bad);
    EXPECT_EQ(stat.StatLastErrorTimestamp(), timestamp2);
    EXPECT_EQ(stat.StatLastErrorText(), errorText);
}

TEST(CnPortStatTest, ClearStatistics)
{
    CnPortStat stat;
    CnTimestamp timestamp = 1234567890123456789;
    
    // Set some values
    stat.setStatCycleTime(100);
    stat.setStatCycleTime(200);
    stat.setStatStatus(Modbus::Status_Good, timestamp);
    stat.setStatStatus(Modbus::Status_Bad, timestamp + 1000, CnSTR("Error"));
    
    EXPECT_NE(stat.StatThreadCycleCount(), 0);
    EXPECT_NE(stat.StatGoodCount(), 0);
    
    // Clear
    stat.clear();
    
    EXPECT_EQ(stat.StatThreadCycleCount(), 0);
    EXPECT_EQ(stat.StatThreadCycleSum(), 0);
    EXPECT_EQ(stat.StatThreadLastCycle(), 0);
    EXPECT_EQ(stat.StatThreadMinCycle(), 0xFFFFFFFF);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 0);
    EXPECT_EQ(stat.StatThreadAvgCycle(), 0);
    EXPECT_EQ(stat.StatGoodCount(), 0);
    EXPECT_EQ(stat.StatBadCount(), 0);
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Good);
    EXPECT_EQ(stat.StatTimestamp(), CnTimestamp());
    EXPECT_EQ(stat.StatLastSuccessTimestamp(), CnTimestamp());
    EXPECT_EQ(stat.StatLastErrorStatus(), Modbus::Status_Good);
    EXPECT_EQ(stat.StatLastErrorTimestamp(), CnTimestamp());
    // Note: LastErrorText is not cleared by clear()
}

TEST(CnPortStatTest, CycleTimeEdgeCases)
{
    CnPortStat stat;
    
    // Set very small cycle time
    stat.setStatCycleTime(1);
    EXPECT_EQ(stat.StatThreadMinCycle(), 1);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 1);
    
    // Set very large cycle time
    stat.setStatCycleTime(0xFFFFFFFE);
    EXPECT_EQ(stat.StatThreadMinCycle(), 1);
    EXPECT_EQ(stat.StatThreadMaxCycle(), 0xFFFFFFFE);
    
    // Set zero cycle time
    stat.setStatCycleTime(0);
    EXPECT_EQ(stat.StatThreadMinCycle(), 0);
}

TEST(CnPortStatTest, StatusWithoutErrorText)
{
    CnPortStat stat;
    CnTimestamp timestamp = 1234567890123456789;
    
    // Bad status without error text
    stat.setStatStatus(Modbus::Status_Bad, timestamp);
    
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Bad);
    EXPECT_EQ(stat.StatBadCount(), 1);
    EXPECT_EQ(stat.StatLastErrorText(), CnString());
}

TEST(CnPortStatTest, UncertainStatus)
{
    CnPortStat stat;
    CnTimestamp timestamp = 1234567890123456789;
    
    stat.setStatStatus(Modbus::Status_Uncertain, timestamp);
    
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Uncertain);
    EXPECT_EQ(stat.StatGoodCount(), 0);
    EXPECT_EQ(stat.StatBadCount(), 1);  // Uncertain is counted as bad
    EXPECT_EQ(stat.StatTimestamp(), timestamp);
}

TEST(CnPortStatTest, MultipleGoodStatuses)
{
    CnPortStat stat;
    
    for (int i = 1; i <= 10; i++)
    {
        stat.setStatStatus(Modbus::Status_Good, i * 1000000000000000000ULL);
    }
    
    EXPECT_EQ(stat.StatGoodCount(), 10);
    EXPECT_EQ(stat.StatBadCount(), 0);
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Good);
    EXPECT_EQ(stat.StatTimestamp().rawValue(), 10000000000000000000ULL);
    EXPECT_EQ(stat.StatLastSuccessTimestamp().rawValue(), 10000000000000000000ULL);
}

TEST(CnPortStatTest, MultipleBadStatuses)
{
    CnPortStat stat;
    
    for (int i = 1; i <= 5; i++)
    {
        CnString error = CnSTR("Error ") + Cn::toString(i);
        stat.setStatStatus(Modbus::Status_Bad, i * 1000000000000000000ULL, error);
    }
    
    EXPECT_EQ(stat.StatGoodCount(), 0);
    EXPECT_EQ(stat.StatBadCount(), 5);
    EXPECT_EQ(stat.StatStatus(), Modbus::Status_Bad);
    EXPECT_EQ(stat.StatTimestamp().rawValue(), 5000000000000000000ULL);
    EXPECT_EQ(stat.StatLastErrorTimestamp().rawValue(), 5000000000000000000ULL);
    EXPECT_EQ(stat.StatLastErrorText(), CnSTR("Error 5"));
}
