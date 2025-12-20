#include "gtest/gtest.h"
#include "core/CnItemCsvReader.h"
#include <fstream>

class CnItemCsvReaderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a temporary CSV file for testing
        std::ofstream testFile("test.csv");
        testFile << "key1;value1\n";
        testFile << "\"key2\";\"value2\"\n";
        testFile << "key3;value3\n";
        testFile.close();
    }

    void TearDown() override
    {
        // Remove the temporary CSV file after testing
        std::remove("test.csv");
    }
};

TEST_F(CnItemCsvReaderTest, OpenFile)
{
    CnItemCsvReader reader("test.csv");
    EXPECT_TRUE(reader.isOpen());
    EXPECT_FALSE(reader.hasError());
}

TEST_F(CnItemCsvReaderTest, ReadKeyValuePairs)
{
    CnItemCsvReader reader("test.csv");

    ASSERT_TRUE(reader.readNext());
    EXPECT_EQ(reader.key(), "key1");
    EXPECT_EQ(reader.value(), "value1");

    ASSERT_TRUE(reader.readNext());
    EXPECT_EQ(reader.key(), "key2");
    EXPECT_EQ(reader.value(), "value2");

    ASSERT_TRUE(reader.readNext());
    EXPECT_EQ(reader.key(), "key3");
    EXPECT_EQ(reader.value(), "value3");

    EXPECT_FALSE(reader.readNext()); // End of file
}

TEST_F(CnItemCsvReaderTest, HandleInvalidFile)
{
    CnItemCsvReader reader("nonexistent.csv");
    EXPECT_FALSE(reader.isOpen());
    EXPECT_TRUE(reader.hasError());
}