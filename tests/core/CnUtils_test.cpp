#include <gtest/gtest.h>
#include <core/CnUtils.h>

TEST(CnUtilsTest, ToInt_ValidInput)
{
    bool ok;
    int result = Cn::toInt("123", &ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(result, 123);
}

TEST(CnUtilsTest, ToInt_InvalidInput)
{
    bool ok;
    int result = Cn::toInt("abc", &ok);
    EXPECT_FALSE(ok);
    EXPECT_EQ(result, 0);
}

TEST(CnUtilsTest, ToUpper)
{
    CnString input = "hello";
    CnString result = Cn::toUpper(input);
    EXPECT_EQ(result, "HELLO");
}

TEST(CnUtilsTest, ToLower)
{
    CnString input = "HELLO";
    CnString result = Cn::toLower(input);
    EXPECT_EQ(result, "hello");
}

TEST(CnUtilsTest, ToString_Char)
{
    char value = 65;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "65");
}

TEST(CnUtilsTest, ToString_UnsignedChar)
{
    unsigned char value = 65;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "65");
}

TEST(CnUtilsTest, ToString_Short)
{
    short value = -12345;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "-12345");
}

TEST(CnUtilsTest, ToString_UnsignedShort)
{
    unsigned short value = 12345;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "12345");
}

TEST(CnUtilsTest, ToString_PositiveInt)
{
    int value = 42;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "42");
}

TEST(CnUtilsTest, ToString_NegativeInt)
{
    int value = -42;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "-42");
}

TEST(CnUtilsTest, ToString_UnsignedInt)
{
    unsigned int value = 42;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "42");
}

TEST(CnUtilsTest, ToString_Long)
{
    long value = -1234567890L;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "-1234567890");
}

TEST(CnUtilsTest, ToString_UnsignedLong)
{
    unsigned long value = 1234567890UL;
    CnString result = Cn::toString(value);
    EXPECT_EQ(result, "1234567890");
}

TEST(CnUtilsTest, ToString_Float)
{
    float value = 3.14159f;
    CnString result = Cn::toString(value, 'f', 2);
    EXPECT_EQ(result, "3.14");
}

TEST(CnUtilsTest, ToString_Double)
{
    double value = 3.141592653589793;
    CnString result = Cn::toString(value, 'f', 5);
    EXPECT_EQ(result, "3.14159");
}

TEST(CnUtilsTest, Split)
{
    CnString input = "a,b,c";
    CnStringList result = Cn::split(input, ',');
    ASSERT_EQ(result.size(), 3);
    auto it = result.begin();
    EXPECT_EQ(*it++, "a");
    EXPECT_EQ(*it++, "b");
    EXPECT_EQ(*it  , "c");
}

TEST(CnUtilsTest, Trim)
{
    CnString input = "  hello  ";
    CnString result = Cn::trim(input);
    EXPECT_EQ(result, "hello");
}

TEST(CnUtilsTest, BytesToString)
{
    uint8_t bytes[] = {0x12, 0x34, 0x56};
    CnString result = Cn::bytesToString(bytes, 3);
    EXPECT_EQ(result, "12 34 56");
}

TEST(CnUtilsTest, ToSwappedInt32)
{
    int32_t value = 0x12345678;
    EXPECT_EQ(Cn::toSwappedInt32(value), 0x56781234);
}

TEST(CnUtilsTest, ToSwappedUInt32)
{
    uint32_t value = 0x12345678;
    EXPECT_EQ(Cn::toSwappedUInt32(value), 0x56781234);
}

TEST(CnUtilsTest, ToSwappedInt64)
{
    int64_t value = 0x123456789ABCDEF0;
    EXPECT_EQ(Cn::toSwappedInt64(value), 0x56781234DEF09ABC);
}

TEST(CnUtilsTest, ToSwappedUInt64)
{
    uint64_t value = 0x123456789ABCDEF0;
    EXPECT_EQ(Cn::toSwappedUInt64(value), 0x56781234DEF09ABC);
}

TEST(CnUtilsTest, ToSwappedFloat) 
{
    float value = 1234.5678f;
    float swapped = Cn::toSwappedFloat(value);
    EXPECT_NE(value, swapped); // Ensure the value is swapped
}

TEST(CnUtilsTest, ToSwappedDouble)
{
    double value = 1234.5678;
    double swapped = Cn::toSwappedDouble(value);
    EXPECT_NE(value, swapped); // Ensure the value is swapped
}

TEST(CnUtilsTest, ToModbus4xAddress)
{
    EXPECT_EQ(Cn::toModbus4xAddress(0), 400001);
    EXPECT_EQ(Cn::toModbus4xAddress(1), 400002);
}

TEST(CnUtilsTest, ToModbus4xOffset)
{
    EXPECT_EQ(Cn::toModbus4xOffset(400001), 0);
    EXPECT_EQ(Cn::toModbus4xOffset(400002), 1);
}

TEST(CnUtilsTest, CurrentTimestamp)
{
    CnTimestamp timestamp = Cn::currentTimestamp();
    EXPECT_GT(timestamp, CnTimestamp(0)); // Ensure the timestamp is valid
}

