#include <gtest/gtest.h>

#include <CnTimestamp.h>
#include <CnDateTime.h>
#include <CnVariant.h>

TEST(CnVariantTest, DefaultConstructor)
{
    CnVariant variant;
    EXPECT_EQ(variant.toInt(nullptr), 0);
    EXPECT_EQ(variant.toString(nullptr), "");
}

TEST(CnVariantTest, BoolConstructor)
{
    CnVariant variant(true);
    EXPECT_EQ(variant.toBool(nullptr), true);
    EXPECT_EQ(variant.toString(nullptr), "1");
}

TEST(CnVariantTest, IntConstructor)
{
    CnVariant variant(42);
    EXPECT_EQ(variant.toInt(nullptr), 42);
    EXPECT_EQ(variant.toString(nullptr), "42");
}

TEST(CnVariantTest, DoubleConstructor)
{
    CnVariant variant(3.14);
    EXPECT_DOUBLE_EQ(variant.toDouble(nullptr), 3.14);
    EXPECT_EQ(variant.toString(nullptr), "3.14");
}

TEST(CnVariantTest, StringConstructor)
{
    CnString str("Hello");
    CnVariant variant(str);
    EXPECT_EQ(variant.toString(nullptr), "Hello");
}

TEST(CnVariantTest, CopyConstructor)
{
    CnVariant original(42);
    CnVariant copy(original);
    EXPECT_EQ(copy.toInt(nullptr), 42);
}

TEST(CnVariantTest, MoveConstructor)
{
    CnVariant original(42);
    CnVariant moved(std::move(original));
    EXPECT_EQ(moved.toInt(nullptr), 42);
}

TEST(CnVariantTest, AssignmentOperator)
{
    CnVariant variant;
    variant = CnVariant(42);
    EXPECT_EQ(variant.toInt(nullptr), 42);
}

TEST(CnVariantTest, MoveAssignmentOperator)
{
    CnVariant variant;
    CnVariant temp(42);
    variant = std::move(temp);
    EXPECT_EQ(variant.toInt(nullptr), 42);
}

TEST(CnVariantTest, ClearMethod)
{
    CnVariant variant(42);
    variant.clear();
    EXPECT_EQ(variant.toInt(nullptr), 0);
}

TEST(CnVariantTest, ToTimestamp)
{
    CnTimestamp tm1(123456789987654321LL);
    CnVariant variant(tm1);
    CnTimestamp tm2 = variant.toTimestamp(nullptr);
    EXPECT_EQ(tm1, tm2);
}

TEST(CnVariantTest, ToDateTime)
{
    CnDateTime dt1(2023, 3, 14, 15, 9, 26, 987);
    CnVariant variant(dt1);
    CnDateTime dt2 = variant.toDateTime(nullptr);
    EXPECT_EQ(dt1, dt2);
}

TEST(CnVariantTest, StringToBool)
{
    {
        CnVariant v(CnString("true"));
        bool ok = false;
        EXPECT_TRUE(v.toBool(&ok));
        EXPECT_TRUE(ok);
    }
    {
        CnVariant v(CnString("false"));
        bool ok = false;
        EXPECT_FALSE(v.toBool(&ok));
        EXPECT_TRUE(ok);
    }
    {
        CnVariant v(CnString("1"));
        bool ok = false;
        EXPECT_TRUE(v.toBool(&ok));
        EXPECT_TRUE(ok);
    }
    {
        CnVariant v(CnString("0"));
        bool ok = false;
        EXPECT_FALSE(v.toBool(&ok));
        EXPECT_TRUE(ok);
    }
}

TEST(CnVariantTest, StringToInt8)
{
    CnVariant v1(CnString("127"));
    bool ok1 = false;
    EXPECT_EQ(v1.toInt8(&ok1), static_cast<int8_t>(127));
    EXPECT_TRUE(ok1);

    CnVariant v2(CnString("-128"));
    bool ok2 = false;
    EXPECT_EQ(v2.toInt8(&ok2), static_cast<int8_t>(-128));
    EXPECT_TRUE(ok2);
}

TEST(CnVariantTest, StringToUInt8)
{
    CnVariant v(CnString("255"));
    bool ok = false;
    EXPECT_EQ(v.toUInt8(&ok), static_cast<uint8_t>(255));
    EXPECT_TRUE(ok);
}

TEST(CnVariantTest, StringToInt16)
{
    CnVariant v1(CnString("32767"));
    bool ok1 = false;
    EXPECT_EQ(v1.toInt16(&ok1), static_cast<int16_t>(32767));
    EXPECT_TRUE(ok1);

    CnVariant v2(CnString("-32768"));
    bool ok2 = false;
    EXPECT_EQ(v2.toInt16(&ok2), static_cast<int16_t>(-32768));
    EXPECT_TRUE(ok2);
}

TEST(CnVariantTest, StringToUInt16)
{
    CnVariant v(CnString("65535"));
    bool ok = false;
    EXPECT_EQ(v.toUInt16(&ok), static_cast<uint16_t>(65535));
    EXPECT_TRUE(ok);
}

TEST(CnVariantTest, StringToInt32)
{
    CnVariant v1(CnString("2147483647"));
    bool ok1 = false;
    EXPECT_EQ(v1.toInt32(&ok1), static_cast<int32_t>(2147483647));
    EXPECT_TRUE(ok1);

    CnVariant v2(CnString("-2147483648"));
    bool ok2 = false;
    EXPECT_EQ(v2.toInt32(&ok2), static_cast<int32_t>(-2147483648LL));
    EXPECT_TRUE(ok2);
}

TEST(CnVariantTest, StringToUInt32)
{
    CnVariant v(CnString("4294967295"));
    bool ok = false;
    EXPECT_EQ(v.toUInt32(&ok), static_cast<uint32_t>(4294967295UL));
    EXPECT_TRUE(ok);
}

TEST(CnVariantTest, StringToInt64)
{
    CnVariant v1(CnString("9223372036854775807"));
    bool ok1 = false;
    EXPECT_EQ(v1.toInt64(&ok1), static_cast<int64_t>(9223372036854775807LL));
    EXPECT_TRUE(ok1);

    CnVariant v2(CnString("-9223372036854775808"));
    bool ok2 = false;
    EXPECT_EQ(v2.toInt64(&ok2), static_cast<int64_t>(-9223372036854775807LL - 1));
    EXPECT_TRUE(ok2);
}

TEST(CnVariantTest, StringToUInt64)
{
    CnVariant v(CnString("18446744073709551615"));
    bool ok = false;
    EXPECT_EQ(v.toUInt64(&ok), static_cast<uint64_t>(18446744073709551615ULL));
    EXPECT_TRUE(ok);
}

TEST(CnVariantTest, StringToFloat)
{
    CnVariant v(CnString("3.14159"));
    bool ok = false;
    EXPECT_FLOAT_EQ(v.toFloat(&ok), 3.14159f);
    EXPECT_TRUE(ok);
}

TEST(CnVariantTest, StringToDouble)
{
    CnVariant v(CnString("2.718281828"));
    bool ok = false;
    EXPECT_DOUBLE_EQ(v.toDouble(&ok), 2.718281828);
    EXPECT_TRUE(ok);
}
