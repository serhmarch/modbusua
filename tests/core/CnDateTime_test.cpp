#include <gtest/gtest.h>

#include <CnDateTime.h>

#include <CnStd_stdio.h>

#define TEST_DATETIME_year         2023
#define TEST_DATETIME_month        12
#define TEST_DATETIME_day          31
#define TEST_DATETIME_hour         23
#define TEST_DATETIME_minute       58
#define TEST_DATETIME_second       13
#define TEST_DATETIME_milliseconds 537
#define TEST_DATETIME_dayOfWeek    0    

static inline CnDateTime createDateTimeForTest(bool local) 
{ 
    return CnDateTime(TEST_DATETIME_year        ,
                           TEST_DATETIME_month       ,
                           TEST_DATETIME_day         ,
                           TEST_DATETIME_hour        ,
                           TEST_DATETIME_minute      ,
                           TEST_DATETIME_second      ,
                           TEST_DATETIME_milliseconds,
                           local);
}

static inline bool compareForTest(const CnDateTime& dt1, bool local) 
{
    return ((dt1.isNull      () == false                     ) &&
            (dt1.isUtc       () == !local                    ) &&
            (dt1.isLocal     () == local                     ) &&
            (dt1.year        () == TEST_DATETIME_year        ) && 
            (dt1.month       () == TEST_DATETIME_month       ) &&
            (dt1.day         () == TEST_DATETIME_day         ) &&
            (dt1.hour        () == TEST_DATETIME_hour        ) &&
            (dt1.minute      () == TEST_DATETIME_minute      ) &&
            (dt1.second      () == TEST_DATETIME_second      ) &&
            (dt1.milliseconds() == TEST_DATETIME_milliseconds) &&
            (dt1.dayOfWeek   () == TEST_DATETIME_dayOfWeek   ));
}

static inline bool compareForTest(const CnDateTime& dt1, const CnDateTime& dt2) 
{
    return ((dt1.isNull      () == dt2.isNull      ()) &&
            (dt1.isUtc       () == dt2.isUtc       ()) &&
            (dt1.isLocal     () == dt2.isLocal     ()) &&
            (dt1.year        () == dt2.year        ()) && 
            (dt1.month       () == dt2.month       ()) &&
            (dt1.day         () == dt2.day         ()) &&
            (dt1.hour        () == dt2.hour        ()) &&
            (dt1.minute      () == dt2.minute      ()) &&
            (dt1.second      () == dt2.second      ()) &&
            (dt1.milliseconds() == dt2.milliseconds()) &&
            (dt1.dayOfWeek   () == dt2.dayOfWeek   ()));
}

TEST(CnDateTimeTest, toString)
{
    CnDateTime dt = createDateTimeForTest(true);
    CnString formatted = dt.toString(CnSTR("%Y-%M-%D %h:%m:%s.%f"));

    CnChar expected[100];
    CnStd::snprintf(expected, 100, CnSTR("%04d-%02d-%02d %02d:%02d:%02d.%03d"),
             TEST_DATETIME_year,
             TEST_DATETIME_month,
             TEST_DATETIME_day,
             TEST_DATETIME_hour,
             TEST_DATETIME_minute,
             TEST_DATETIME_second,
             TEST_DATETIME_milliseconds);

    EXPECT_EQ(formatted, expected);
}

TEST(CnDateTimeTest, CopyConstructorTest)
{
    CnDateTime dt1 = createDateTimeForTest(true);
    CnDateTime dt2(dt1);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, MoveConstructorTest)
{
    bool local = true;
    CnDateTime dt1 = createDateTimeForTest(local);
    CnDateTime dt2(std::move(dt1));

    EXPECT_TRUE(compareForTest(dt2, local));
    EXPECT_TRUE(dt1.isNull());
}

TEST(CnDateTimeTest, CopyAssignmentOperatorTest)
{
    CnDateTime dt1 = createDateTimeForTest(true);
    CnDateTime dt2;
    dt2 = dt1;

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, MoveAssignmentOperatorTest)
{
    CnDateTime dt1 = createDateTimeForTest(true);
    CnDateTime dt2;
    dt2 = std::move(dt1);

    EXPECT_TRUE(compareForTest(dt2, true));
    EXPECT_TRUE(dt1.isNull());
}

TEST(CnDateTimeTest, InvalidFormatSpecifierTest)
{
    CnDateTime dt = createDateTimeForTest(true);
    CnString formatted = dt.toString(CnSTR("%X"));

    EXPECT_EQ(formatted, CnSTR("%X"));
}

TEST(CnDateTimeTest, MixedFormatStringTest)
{
    CnDateTime dt = createDateTimeForTest(true);
    CnString formatted = dt.toString(CnSTR("Year: %Y, Month: %M, Day: %D"));

    CnChar expected[100];
    CnStd::snprintf(expected, 100, CnSTR("Year: %04d, Month: %02d, Day: %02d"),
             TEST_DATETIME_year,
             TEST_DATETIME_month,
             TEST_DATETIME_day);

    EXPECT_EQ(formatted, expected);
}

TEST(CnDateTimeTest, NullDateTimeToStringTest)
{
    CnDateTime dt;
    CnString formatted = dt.toString(CnSTR("%Y-%M-%D %h:%m:%s.%f"));

    EXPECT_EQ(formatted, CnString());
}

TEST(CnDateTimeTest, AddNegativeMillisecondsTest)
{
    CnDateTime dt1 = createDateTimeForTest(true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds() - 500, true);

    dt1.addMilliseconds(-500);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddNegativeDaysTest)
{
    CnDateTime dt1 = createDateTimeForTest(true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day() - 1, dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds(), true);

    dt1.addDays(-1);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, LeapYearTest) 
{
    CnDateTime dt1(2024, 2, 28, 23, 59, 59, 999, true);
    dt1.addMilliseconds(1);

    EXPECT_EQ(dt1.year(), 2024);
    EXPECT_EQ(dt1.month(), 2);
    EXPECT_EQ(dt1.day(), 29);
}

TEST(CnDateTimeTest, NonLeapYearTest) 
{
    CnDateTime dt1(2023, 2, 28, 23, 59, 59, 999, true);
    dt1.addMilliseconds(1);

    EXPECT_EQ(dt1.year(), 2023);
    EXPECT_EQ(dt1.month(), 3);
    EXPECT_EQ(dt1.day(), 1);
}

TEST(CnDateTimeTest, AddMillisecondsTest)
{
    CnDateTime dt1(2023, 2, 28, 23, 59, 59, 0, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds() + 500, true);

    dt1.addMilliseconds(500);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddSecondsTest)
{
    CnDateTime dt1(2023, 2, 28, 23, 59, 0, 999, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), dt1.hour(), dt1.minute(), dt1.second() + 10, dt1.milliseconds(), true);

    dt1.addSeconds(10);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddMinutesTest)
{
    CnDateTime dt1(2023, 2, 28, 23, 0, 59, 999, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), dt1.hour(), dt1.minute() + 5, dt1.second(), dt1.milliseconds(), true);

    dt1.addMinutes(5);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddHoursTest)
{
    CnDateTime dt1(2023, 2, 28, 21, 59, 59, 999, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), dt1.hour() + 2, dt1.minute(), dt1.second(), dt1.milliseconds(), true);

    dt1.addHours(2);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddDaysTest)
{
    CnDateTime dt1(2023, 12, 28, 12, 59, 59, 999, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day() + 1, dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds(), true);

    dt1.addDays(1);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddMonthsTest)
{
//    CnDateTime dt1 = createDateTimeForTest(true);
//    CnDateTime dt2(dt1.year(), dt1.month() + 1, dt1.day(), dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds(), true);
//
//    dt1.addMonths(1);
//
//    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, AddYearsTest)
{
//    CnDateTime dt1 = createDateTimeForTest(true);
//    CnDateTime dt2(dt1.year() + 1, dt1.month(), dt1.day(), dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds(), true);
//
//    dt1.addYears(1);
//
//    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, InvalidDateTest)
{
//    CnDateTime dt(2023, 2, 30, 0, 0, 0, 0, true);
//
//    EXPECT_TRUE(dt.isNull());
}

TEST(CnDateTimeTest, EndOfMonthTest)
{
    CnDateTime dt1(2023, 1, 31, 23, 59, 59, 999, true);
    dt1.addMilliseconds(1);

    EXPECT_EQ(dt1.year(), 2023);
    EXPECT_EQ(dt1.month(), 2);
    EXPECT_EQ(dt1.day(), 1);
}

TEST(CnDateTimeTest, StartOfYearTest)
{
    CnDateTime dt1(2023, 12, 31, 23, 59, 59, 999, true);
    dt1.addMilliseconds(1);

    EXPECT_EQ(dt1.year(), 2024);
    EXPECT_EQ(dt1.month(), 1);
    EXPECT_EQ(dt1.day(), 1);
}

TEST(CnDateTimeTest, SubtractMillisecondsTest)
{
    CnDateTime dt1(2023, 1, 31, 23, 59, 59, 500, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds() - 250, true);

    dt1.addMilliseconds(-250);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}

TEST(CnDateTimeTest, SubtractDaysTest)
{
    CnDateTime dt1(2023, 12, 31, 23, 59, 59, 999, true);
    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day() - 3, dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds(), true);

    dt1.addDays(-3);

    EXPECT_TRUE(compareForTest(dt1, dt2));
}
//Test_CnDateTime::Test_CnDateTime() :
//    TestModule("CnDateTime")
//{
//}
//
//void Test_CnDateTime::initTestCase_data()
//{
//    addColumn<bool>("local");
//    newRow("utc")   << false;
//    newRow("local") << true ;
//}
//
//void Test_CnDateTime::testExec()
//{
//    TEST_METHOD(constructorDefault    );
//    TEST_METHOD(constructorCopy       );
//    TEST_METHOD(constructorFull       );
//    TEST_METHOD(constructorSYSTEMTIME );
//    TEST_METHOD(constructorFILETIME   );
//    TEST_METHOD(constructorTimestamp  );
//    TEST_METHOD(toSystemTime          );
//    TEST_METHOD(toFileTime            );
//    TEST_METHOD(toLocal               );
//    TEST_METHOD(toUtc                 );
//    TEST_METHOD(toTimestamp           );
//    TEST_METHOD(toString              );
//    TEST_METHOD(clear                 );
//    TEST_METHOD(clearTimeOnly         );
//    TEST_METHOD(addMilliseconds       );
//    TEST_METHOD(addSeconds            );
//    TEST_METHOD(addMinutes            );
//    TEST_METHOD(addHours              );
//    TEST_METHOD(addDays               );
//    TEST_METHOD(operatorIsEqual       );
//    TEST_METHOD(operatorIsNonEqual    );
//    TEST_METHOD(operatorIsLessThen    );
//    TEST_METHOD(operatorIsGreaterThen );
//    TEST_METHOD(operatorIsLessEqual   );
//    TEST_METHOD(operatorIsGreaterEqual);
//    TEST_METHOD(operatorAssign        );
//}
//
//void Test_CnDateTime::constructorDefault()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt(local);
//    TEST_IS_TRUE((dt.isUtc()   == !local) && 
//                 (dt.isLocal() == local ) && 
//                  dt.isNull());
//}
//
//void Test_CnDateTime::constructorCopy()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1);
//
//    TEST_IS_TRUE((dt1.isUtc() == !local) && (dt1.isLocal() == local) && !dt1.isNull() &&
//                 (dt2.isUtc() == !local) && (dt2.isLocal() == local) && !dt2.isNull() &&
//                 compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::constructorFull()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    TEST_IS_TRUE(compareForTest(dt1, local));
//}
//
//void Test_CnDateTime::constructorSYSTEMTIME()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    SYSTEMTIME st;
//    st.wYear         = TEST_DATETIME_year        ;
//    st.wMonth        = TEST_DATETIME_month       ;
//    st.wDay          = TEST_DATETIME_day         ;
//    st.wHour         = TEST_DATETIME_hour        ;
//    st.wMinute       = TEST_DATETIME_minute      ;
//    st.wSecond       = TEST_DATETIME_second      ;
//    st.wMilliseconds = TEST_DATETIME_milliseconds;
//
//    CnDateTime dt1(st, local);
//    TEST_IS_TRUE(compareForTest(dt1, local));
//}
//
//void Test_CnDateTime::constructorFILETIME()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    SYSTEMTIME st;
//    st.wYear         = TEST_DATETIME_year        ;
//    st.wMonth        = TEST_DATETIME_month       ;
//    st.wDay          = TEST_DATETIME_day         ;
//    st.wHour         = TEST_DATETIME_hour        ;
//    st.wMinute       = TEST_DATETIME_minute      ;
//    st.wSecond       = TEST_DATETIME_second      ;
//    st.wMilliseconds = TEST_DATETIME_milliseconds;
//    
//    FILETIME ft;
//    SystemTimeToFileTime(&st, &ft);
//
//    CnDateTime dt1(ft, local);
//    TEST_IS_TRUE(compareForTest(dt1, local));
//}
//
//void Test_CnDateTime::constructorTimestamp()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    SYSTEMTIME st;
//    st.wYear         = TEST_DATETIME_year        ;
//    st.wMonth        = TEST_DATETIME_month       ;
//    st.wDay          = TEST_DATETIME_day         ;
//    st.wHour         = TEST_DATETIME_hour        ;
//    st.wMinute       = TEST_DATETIME_minute      ;
//    st.wSecond       = TEST_DATETIME_second      ;
//    st.wMilliseconds = TEST_DATETIME_milliseconds;
//    
//    FILETIME ft;
//    SystemTimeToFileTime(&st, &ft);
//
//    CnTimestamp tm(ft, local);
//
//    CnDateTime dt1(tm, local);
//    TEST_IS_TRUE(compareForTest(dt1, local));
//}
//
//void Test_CnDateTime::toSystemTime()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    SYSTEMTIME st1, st2;
//    GetSystemTime(&st1);
//    CnDateTime dt(st1, local);
//
//    st2 = dt.toSystemTime();
//
//    TEST_IS_TRUE(memcmp(&st1, &st2, sizeof(st1)) == 0);
//}
//
//void Test_CnDateTime::toFileTime()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    FILETIME ft1, ft2;
//    GetSystemTimeAsFileTime(&ft1);
//    CnDateTime dt1(ft1, local);
//
//    ft2 = dt1.toFileTime();
//    CnDateTime dt2(ft2, local);
//
//    TEST_IS_EQUAL(*reinterpret_cast<uint64_t*>(&ft1) / CN_NANOSEC100_IN_MILLISEC, *reinterpret_cast<uint64_t*>(&ft2) / CN_NANOSEC100_IN_MILLISEC);
//}
//
//void Test_CnDateTime::toLocal()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = CnDateTime::currentDateTime(local);
//    CnDateTime dt2 = dt1.toLocal();
//
//    // TODO: improve test not just utc != local but time shift
//    bool res = compareForTest(dt1, dt2);
//    res = !(local ^ res);
//    TEST_IS_TRUE(res);
//}
//
//void Test_CnDateTime::toUtc()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = CnDateTime::currentDateTime(local);
//    CnDateTime dt2 = dt1.toUtc();
//
//    // TODO: improve test not just utc != local but time shift
//    bool res = compareForTest(dt1, dt2);
//    res = (local ^ res);
//    TEST_IS_TRUE(res);
//}
//
//void Test_CnDateTime::toTimestamp()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnTimestamp tm1 = CnTimestamp::current();
//    CnDateTime dt(tm1, local);
//
//    CnTimestamp tm2 = dt.toTimestamp();
//
//    TEST_IS_EQUAL(tm1.milliseconds(), tm2.milliseconds());
//}
//
//void Test_CnDateTime::toString()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt = createDateTimeForTest(local);
//    CnString s1 = dt.toString(CnSTR("%Y.%M.%D %h:%m:%s.%f");
//
//    wchar_t buff[100];
//    wsprintf(buff, CnSTR("%04d.%02d.%02d %02d:%02d:%02d.%03d",
//        dt.year(),
//        dt.month(),
//        dt.day(),
//        dt.hour(),
//        dt.minute(),
//        dt.second(),
//        dt.milliseconds());
//    CnString s2(buff);
//
//    TEST_IS_EQUAL(s1, s2);
//}
//
//void Test_CnDateTime::clear()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(local);
//
//    dt1.clear();
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::clearTimeOnly()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1.year(), dt1.month(), dt1.day(), 0, 0, 0, 0, local);
//
//    dt1.clearTimeOnly();
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::addMilliseconds()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1.year()+1, 1, 1, 0, 0, 3, 200, local);
//
//    dt1.addMilliseconds(120163);
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::addSeconds()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1.year() + 1, 1, 1, 0, 0, dt1.second()+1, dt1.milliseconds(), local);
//
//    dt1.addSeconds(121);
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::addMinutes()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1.year() + 1, 1, 1, 1, dt1.minute()+1, dt1.second(), dt1.milliseconds(), local);
//
//    dt1.addMinutes(121);
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::addHours()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1.year() + 1, 1, 2, 0, dt1.minute(), dt1.second(), dt1.milliseconds(), local);
//
//    dt1.addHours(25);
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::addDays()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2(dt1.year() + 1, 2, 1, dt1.hour(), dt1.minute(), dt1.second(), dt1.milliseconds(), local);
//
//    dt1.addDays(32);
//
//    TEST_IS_TRUE(compareForTest(dt1, dt2));
//}
//
//void Test_CnDateTime::operatorIsEqual()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2 = createDateTimeForTest(local);
//
//    TEST_IS_TRUE(dt1 == dt2);
//}
//
//void Test_CnDateTime::operatorIsNonEqual()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2 = createDateTimeForTest(local);
//
//    dt2.addMilliseconds(1);
//
//    TEST_IS_TRUE(dt1 != dt2);
//}
//
//void Test_CnDateTime::operatorIsLessThen()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2 = createDateTimeForTest(local);
//
//    dt2.addMilliseconds(1);
//
//    TEST_IS_TRUE(dt1 < dt2);
//}
//
//void Test_CnDateTime::operatorIsGreaterThen()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2 = createDateTimeForTest(local);
//
//    dt1.addMilliseconds(1);
//
//    TEST_IS_TRUE(dt1 > dt2);
//}
//
//void Test_CnDateTime::operatorIsLessEqual()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2 = createDateTimeForTest(local);
//
//    TEST_IS_TRUE(dt1 <= dt2);
//
//    dt2.addMilliseconds(1);
//    TEST_IS_TRUE(dt1 <= dt2);
//}
//
//void Test_CnDateTime::operatorIsGreaterEqual()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2 = createDateTimeForTest(local);
//
//    TEST_IS_TRUE(dt1 >= dt2);
//
//    dt1.addMilliseconds(1);
//    TEST_IS_TRUE(dt1 >= dt2);
//}
//
//void Test_CnDateTime::operatorAssign()
//{
//    TEST_FETCH_GLOBAL(bool, local);
//
//    CnDateTime dt1 = createDateTimeForTest(local);
//    CnDateTime dt2;
//
//    dt2 = dt1;
//
//    TEST_IS_TRUE(dt1 == dt2);
//}
//