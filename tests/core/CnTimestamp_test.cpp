#include <gtest/gtest.h>

#include <CnTimestamp.h>

TEST(CnTimestampTest, rawValue)
{
    uint64_t mockRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm(mockRawValue);

    EXPECT_EQ(tm.rawValue(), mockRawValue);
}

TEST(CnTimestampTest, toUtcMSecSinceEpoch)
{
    uint64_t mockRawValue = 132537600000000000ULL; // Mock raw value for 2012-01-01T00:00:00Z

    CnTimestamp tm(mockRawValue, false);
    uint64_t expectedMilliseconds = (mockRawValue / CN_NANOSEC100_IN_MILLISEC) - CN_MSEC_BETWEEN_1601_AND_1970;
    EXPECT_EQ(tm.toUtcMSecSinceEpoch(), expectedMilliseconds);
}

TEST(CnTimestampTest, toUtcSecSinceEpoch)
{
    uint64_t mockRawValue = 132537600000000000ULL; // Mock raw value for 2012-01-01T00:00:00Z

    CnTimestamp tm(mockRawValue, false); // Local time
    uint64_t expectedSeconds = (mockRawValue / CN_NANOSEC100_IN_SEC) - CN_SEC_BETWEEN_1601_AND_1970;
    EXPECT_EQ(tm.toUtcSecSinceEpoch(), expectedSeconds);
}

TEST(CnTimestampTest, toLocalMSecSinceEpoch)
{
    uint64_t mockRawValue = 132537600000000000ULL; // Mock raw value for 2012-01-01T00:00:00Z

    CnTimestamp tm(mockRawValue, true);
    uint64_t expectedMilliseconds = (mockRawValue / CN_NANOSEC100_IN_MILLISEC) - CN_MSEC_BETWEEN_1601_AND_1970;
    EXPECT_EQ(tm.toLocalMSecSinceEpoch(), expectedMilliseconds);
}

TEST(CnTimestampTest, toLocalSecSinceEpoch)
{
    uint64_t mockRawValue = 132537600000000000ULL; // Mock raw value for 2012-01-01T00:00:00Z

    CnTimestamp tm(mockRawValue, true); // Local time
    uint64_t expectedSeconds = (mockRawValue / CN_NANOSEC100_IN_SEC) - CN_SEC_BETWEEN_1601_AND_1970;
    EXPECT_EQ(tm.toLocalSecSinceEpoch(), expectedSeconds);
}

TEST(CnTimestampTest, toUtcMSec1980)
{
    uint64_t mockRawValue = 133933426930000000ULL; // Mock raw value for 2025-06-02T12:58:13

    CnTimestamp tm(mockRawValue, false);
    uint64_t expectedMilliseconds = 1433336293000ULL; // Count of milliseconds since 1 Jan 1980 
                                                      // (calc by Python like '(datetime(2025,6,2,12,58,13)-datetime(1980,1,1))//timedelta(milliseconds=1)')
    EXPECT_EQ(tm.toUtcMSec1980(), expectedMilliseconds);
}

TEST(CnTimestampTest, toUtcSec1980)
{
    uint64_t mockRawValue = 133933426930000000ULL; // Mock raw value for 2025-06-02T12:58:13

    CnTimestamp tm(mockRawValue, false); // Local time
    uint64_t expectedSeconds = 1433336293ULL; // Count of seconds since 1 Jan 1980 
                                              // (calc by Python like '(datetime(2025,6,2,12,58,13)-datetime(1980,1,1))//timedelta(seconds=1)')
    EXPECT_EQ(tm.toUtcSec1980(), expectedSeconds);
}

TEST(CnTimestampTest, toLocalMSec1980)
{
    uint64_t mockRawValue = 132537600000000000ULL; // Mock raw value for 2012-01-01T00:00:00Z

    CnTimestamp tm(mockRawValue, true);
    uint64_t expectedMilliseconds = (mockRawValue / CN_NANOSEC100_IN_MILLISEC) - CN_MSEC_BETWEEN_1601_AND_1980;
    EXPECT_EQ(tm.toLocalMSec1980(), expectedMilliseconds);
}

TEST(CnTimestampTest, toLocalSec1980)
{
    uint64_t mockRawValue = 132537600000000000ULL; // Mock raw value for 2012-01-01T00:00:00Z

    CnTimestamp tm(mockRawValue, true); // Local time
    uint64_t expectedSeconds = (mockRawValue / CN_NANOSEC100_IN_SEC) - CN_SEC_BETWEEN_1601_AND_1980;
    EXPECT_EQ(tm.toLocalSec1980(), expectedSeconds);
}
TEST(CnTimestampTest, seconds)
{
    uint64_t mockRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm(mockRawValue);

    uint64_t expectedSeconds = mockRawValue / CN_NANOSEC100_IN_SEC;
    EXPECT_EQ(tm.seconds(), expectedSeconds);
}

TEST(CnTimestampTest, addSeconds)
{
    uint64_t initialRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm1(initialRawValue);
    CnTimestamp tm2(initialRawValue);

    int64_t secondsToAdd = 10;
    tm1.addSeconds(secondsToAdd);
    tm2.addSeconds(-secondsToAdd);

    uint64_t expectedRawValuePositive = initialRawValue + (secondsToAdd * CN_NANOSEC100_IN_SEC);
    uint64_t expectedRawValueNegative = initialRawValue - (secondsToAdd * CN_NANOSEC100_IN_SEC);

    EXPECT_EQ(tm1.rawValue(), expectedRawValuePositive);
    EXPECT_EQ(tm2.rawValue(), expectedRawValueNegative);
}

TEST(CnTimestampTest, milliseconds)
{
    uint64_t mockRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm(mockRawValue);

    uint64_t expectedMilliseconds = mockRawValue / CN_NANOSEC100_IN_MILLISEC;
    EXPECT_EQ(tm.milliseconds(), expectedMilliseconds);
}

TEST(CnTimestampTest, addMilliseconds)
{
    uint64_t initialRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm1(initialRawValue);
    CnTimestamp tm2(initialRawValue);

    int64_t millisecondsToAdd = 500;
    tm1.addMilliseconds(millisecondsToAdd);
    tm2.addMilliseconds(-millisecondsToAdd);

    uint64_t expectedRawValuePositive = initialRawValue + (millisecondsToAdd * CN_NANOSEC100_IN_MILLISEC);
    uint64_t expectedRawValueNegative = initialRawValue - (millisecondsToAdd * CN_NANOSEC100_IN_MILLISEC);

    EXPECT_EQ(tm1.rawValue(), expectedRawValuePositive);
    EXPECT_EQ(tm2.rawValue(), expectedRawValueNegative);
}

TEST(CnTimestampTest, microseconds)
{
    uint64_t mockRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm(mockRawValue);

    uint64_t expectedMicroseconds = mockRawValue / CN_NANOSEC100_IN_MICROSEC;
    EXPECT_EQ(tm.microseconds(), expectedMicroseconds);
}

TEST(CnTimestampTest, addMicroseconds)
{
    uint64_t initialRawValue = 1234567890123456789ULL; // Mock raw value
    CnTimestamp tm1(initialRawValue);
    CnTimestamp tm2(initialRawValue);

    int64_t microsecondsToAdd = 1000;
    tm1.addMicroseconds(microsecondsToAdd);
    tm2.addMicroseconds(-microsecondsToAdd);

    uint64_t expectedRawValuePositive = initialRawValue + (microsecondsToAdd * CN_NANOSEC100_IN_MICROSEC);
    uint64_t expectedRawValueNegative = initialRawValue - (microsecondsToAdd * CN_NANOSEC100_IN_MICROSEC);

    EXPECT_EQ(tm1.rawValue(), expectedRawValuePositive);
    EXPECT_EQ(tm2.rawValue(), expectedRawValueNegative);
}

TEST(CnTimestampTest, operatorAssignment)
{
    uint64_t rawValue1 = 1234567890123456789ULL;
    uint64_t rawValue2 = 9876543210987654321ULL;

    CnTimestamp tm1(rawValue1);
    CnTimestamp tm2(rawValue2);

    tm1 = tm2;

    EXPECT_EQ(tm1.rawValue(), tm2.rawValue());
}

TEST(CnTimestampTest, operatorAssignmentrawValue)
{
    uint64_t millis = 1234567890123ULL; // Mock milliseconds value
    uint64_t expectedRawValue = millis * CN_NANOSEC100_IN_MILLISEC;

    CnTimestamp tm(0); // Initialize with a default value
    tm = expectedRawValue;

    EXPECT_EQ(tm.rawValue(), expectedRawValue);
}

TEST(CnTimestampTest, operatorPlusEqual)
{
    uint64_t initialRawValue = 1234567890123456789ULL;
    int64_t millisecondsToAdd = 500;

    CnTimestamp tm(initialRawValue);
    tm += millisecondsToAdd*CN_NANOSEC100_IN_MILLISEC;

    uint64_t expectedRawValue = initialRawValue + (millisecondsToAdd * CN_NANOSEC100_IN_MILLISEC);
    EXPECT_EQ(tm.rawValue(), expectedRawValue);
}

TEST(CnTimestampTest, operatorMinusEqual)
{
    uint64_t initialRawValue = 1234567890123456789ULL;
    int64_t millisecondsToSubtract = 500;

    CnTimestamp tm(initialRawValue);
    tm -= millisecondsToSubtract*CN_NANOSEC100_IN_MILLISEC;

    uint64_t expectedRawValue = initialRawValue - (millisecondsToSubtract * CN_NANOSEC100_IN_MILLISEC);
    EXPECT_EQ(tm.rawValue(), expectedRawValue);
}

TEST(CnTimestampTest, operatorIsEqual)
{
    uint64_t rawValue = 1234567890123456789ULL;
    CnTimestamp tm1(rawValue);
    CnTimestamp tm2(rawValue);

    EXPECT_TRUE(tm1 == tm2);
}

TEST(CnTimestampTest, operatorIsNonEqual)
{
    uint64_t rawValue1 = 1234567890123456789ULL;
    uint64_t rawValue2 = 1234567890123456790ULL;

    CnTimestamp tm1(rawValue1);
    CnTimestamp tm2(rawValue2);

    EXPECT_TRUE(tm1 != tm2);
}

TEST(CnTimestampTest, operatorIsLessThan)
{
    uint64_t rawValue1 = 1234567890123456789ULL;
    uint64_t rawValue2 = 1234567890123456790ULL;

    CnTimestamp tm1(rawValue1);
    CnTimestamp tm2(rawValue2);

    EXPECT_TRUE(tm1 < tm2);
}

TEST(CnTimestampTest, operatorIsGreaterThan)
{
    uint64_t rawValue1 = 1234567890123456789ULL;
    uint64_t rawValue2 = 1234567890123456788ULL;

    CnTimestamp tm1(rawValue1);
    CnTimestamp tm2(rawValue2);

    EXPECT_TRUE(tm1 > tm2);
}

TEST(CnTimestampTest, operatorIsLessEqual)
{
    uint64_t rawValue1 = 1234567890123456789ULL;
    uint64_t rawValue2 = 1234567890123456789ULL;
    uint64_t rawValue3 = 1234567890123456790ULL;

    CnTimestamp tm1(rawValue1);
    CnTimestamp tm2(rawValue2);
    CnTimestamp tm3(rawValue3);

    EXPECT_TRUE(tm1 <= tm2);
    EXPECT_TRUE(tm1 <= tm3);
}

TEST(CnTimestampTest, operatorIsGreaterEqual)
{
    uint64_t rawValue1 = 1234567890123456789ULL;
    uint64_t rawValue2 = 1234567890123456789ULL;
    uint64_t rawValue3 = 1234567890123456788ULL;

    CnTimestamp tm1(rawValue1);
    CnTimestamp tm2(rawValue2);
    CnTimestamp tm3(rawValue3);

    EXPECT_TRUE(tm1 >= tm2);
    EXPECT_TRUE(tm1 >= tm3);
}

//TEST(CnTimestampTest, seconds)
//{
//    FILETIME ft;
//    GetSystemTimeAsFileTime(&ft);
//    CnTimestamp tm(ft);
//    
//    uint64_t sec1 = tm.rawValue() / CN_NANOSEC100_IN_SEC;
//    uint64_t sec2 = tm.seconds();
//    TEST_IS_EQUAL(sec1, sec2);
//}
//
//TEST(CnTimestampTest, addSeconds_data)
//{
//    addColumn<CnTimestamp>("timestamp");
//    addColumn<int64_t         >("seconds");
//
//    CnTimestamp timestamp = CnTimestamp::current();
//    newRow("+seconds") << timestamp << +123LL ;
//    newRow("-seconds") << timestamp << -123LL;
//}
//
//TEST(CnTimestampTest, addSeconds)
//{
//    TEST_FETCH(CnTimestamp, timestamp    );
//    TEST_FETCH(int64_t         , seconds      );
//
//    CnTimestamp tm = timestamp;
//    int64_t sec1 = static_cast<int64_t>(timestamp.seconds())+seconds;
//    tm.addSeconds(seconds);
//    int64_t sec2 = static_cast<int64_t>(tm.seconds());
//    TEST_IS_EQUAL(sec1, sec2);
//}
//
//TEST(CnTimestampTest, milliseconds)
//{
//    FILETIME ft;
//    GetSystemTimeAsFileTime(&ft);
//    CnTimestamp tm(ft);
//
//    uint64_t msec1 = tm.rawValue() / CN_NANOSEC100_IN_MILLISEC;
//    uint64_t msec2 = tm.milliseconds();
//    TEST_IS_EQUAL(msec1, msec2);
//}
//
//TEST(CnTimestampTest, addMilliseconds_data)
//{
//    addColumn<CnTimestamp>("timestamp");
//    addColumn<int64_t         >("milliseconds");
//
//    CnTimestamp timestamp = CnTimestamp::current();
//    newRow("+milliseconds") << timestamp << +12003LL;
//    newRow("-milliseconds") << timestamp << -12003LL;
//}
//
//TEST(CnTimestampTest, addMilliseconds)
//{
//    TEST_FETCH(CnTimestamp, timestamp);
//    TEST_FETCH(int64_t, milliseconds);
//
//    CnTimestamp tm = timestamp;
//    int64_t msec1 = static_cast<int64_t>(timestamp.milliseconds()) + milliseconds;
//    tm.addMilliseconds(milliseconds);
//    int64_t msec2 = static_cast<int64_t>(tm.milliseconds());
//    TEST_IS_EQUAL(msec1, msec2);
//}
//
//TEST(CnTimestampTest, microseconds)
//{
//    FILETIME ft;
//    GetSystemTimeAsFileTime(&ft);
//    CnTimestamp tm(ft);
//
//    uint64_t msec1 = tm.rawValue() / CN_NANOSEC100_IN_MICROSEC;
//    uint64_t msec2 = tm.microseconds();
//    TEST_IS_EQUAL(msec1, msec2);
//}
//
//TEST(CnTimestampTest, addMicroseconds_data)
//{
//    addColumn<CnTimestamp>("timestamp");
//    addColumn<int64_t         >("microseconds");
//
//    CnTimestamp timestamp = CnTimestamp::current();
//    newRow("+microseconds") << timestamp << +12000003LL;
//    newRow("-microseconds") << timestamp << -12000003LL;
//}
//
//TEST(CnTimestampTest, addMicroseconds)
//{
//    TEST_FETCH(CnTimestamp, timestamp);
//    TEST_FETCH(int64_t, microseconds);
//
//    CnTimestamp tm = timestamp;
//    int64_t msec1 = static_cast<int64_t>(timestamp.microseconds()) + microseconds;
//    tm.addMicroseconds(microseconds);
//    int64_t msec2 = static_cast<int64_t>(tm.microseconds());
//    TEST_IS_EQUAL(msec1, msec2);
//}
//
//TEST(CnTimestampTest, operatorIsEqual)
//{
//    CnTimestamp tm1 = CnTimestamp::current();
//    CnTimestamp tm2 = tm1;
//    TEST_IS_TRUE(tm1 == tm2);
//}
//
//TEST(CnTimestampTest, operatorIsNonEqual)
//{
//    CnTimestamp tm1 = CnTimestamp::current();
//    CnTimestamp tm2 = tm1;
//    tm2.addSeconds(1);
//    TEST_IS_TRUE(tm1 != tm2);
//}
//
//TEST(CnTimestampTest, operatorIsLessThen)
//{
//    CnTimestamp tm1 = CnTimestamp::current();
//    CnTimestamp tm2 = tm1;
//    tm2.addSeconds(1);
//    TEST_IS_TRUE(tm1 < tm2);
//}
//
//TEST(CnTimestampTest, operatorIsGreaterThen)
//{
//    CnTimestamp tm1 = CnTimestamp::current();
//    CnTimestamp tm2 = tm1;
//    tm2.addSeconds(-1);
//    TEST_IS_TRUE(tm1 > tm2);
//}
//
//TEST(CnTimestampTest, operatorIsLessEqual)
//{
//    CnTimestamp tm1 = CnTimestamp::current();
//    CnTimestamp tm2 = CnTimestamp::current();
//    TEST_IS_TRUE(tm1 <= tm2);
//}
//
//TEST(CnTimestampTest, operatorIsGreaterEqual)
//{
//    CnTimestamp tm2 = CnTimestamp::current();
//    CnTimestamp tm1 = CnTimestamp::current();
//    TEST_IS_TRUE(tm1 <= tm2);
//}

