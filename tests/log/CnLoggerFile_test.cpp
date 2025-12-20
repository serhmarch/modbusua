#include "gtest/gtest.h"

#include <log/CnLoggerFile.h>


TEST(CnLoggerFileTest, toMaxSize_test)
{
    bool ok;

    size_t size = CnLoggerFile::toMaxSize("1024", &ok);
    size_t expected = 1024U;
    EXPECT_TRUE(ok);
    EXPECT_EQ(size, expected);

    size = CnLoggerFile::toMaxSize("2048", &ok);
    expected = 2048U;
    EXPECT_TRUE(ok);
    EXPECT_EQ(size, expected);

    size = CnLoggerFile::toMaxSize("invalid_size", &ok);
    expected = 0U;
    EXPECT_FALSE(ok);
    EXPECT_EQ(size, expected);

    size = CnLoggerFile::toMaxSize("10KB", &ok);
    expected = 10U * 1024U;
    EXPECT_TRUE(ok);
    EXPECT_EQ(size, expected);

    size = CnLoggerFile::toMaxSize("20 KB", &ok);
    expected = 20U * 1024U;
    EXPECT_TRUE(ok);
    EXPECT_EQ(size, expected);

    size = CnLoggerFile::toMaxSize("1MB", &ok);
    expected = 1U * 1024U * 1024U;
    EXPECT_TRUE(ok);
    EXPECT_EQ(size, expected);

    size = CnLoggerFile::toMaxSize("2 gb", &ok);
    expected = 2U * 1024U * 1024U * 1024U;
    EXPECT_TRUE(ok);
    EXPECT_EQ(size, expected);
}