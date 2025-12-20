#include "gtest/gtest.h"
#include "core/CnConfReader.h"
#include <fstream>

class CnConfReaderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a temporary configuration file for testing
        std::ofstream testFile("test.conf");
        testFile << "[Group1]\n";
        testFile << "key1=value1\n";
        testFile << "key2=value2\n";
        testFile << "[Group2]\n";
        testFile << " key3 = value3 \n";
        testFile << " key4 keypart4 = value4 valuepart4 \n";
        testFile << " \"key5\" = \"value5\" \n";
        testFile << " 'key6' = 'value6' \n";
        testFile.close();
    }

    void TearDown() override
    {
        // Remove the temporary configuration file after testing
        std::remove("test.conf");
    }
};

TEST_F(CnConfReaderTest, OpenFile)
{
    CnConfReader reader("test.conf");
    EXPECT_TRUE(reader.isOpen());
    EXPECT_FALSE(reader.hasError());
}

TEST_F(CnConfReaderTest, ReadGroup)
{
    CnConfReader reader("test.conf");
    EXPECT_EQ(reader.readNext(), CnConfReader::Group);
    EXPECT_EQ(reader.groupName(), "Group1");
}

TEST_F(CnConfReaderTest, ReadKeyValuePair)
{
    CnConfReader reader("test.conf");

     // Read Group1
    EXPECT_EQ(reader.readNext(), CnConfReader::Group);
    EXPECT_EQ(reader.groupName(), "Group1");
    EXPECT_EQ(reader.readNext(), CnConfReader::KeyValuePair);
    EXPECT_EQ(reader.key(), "key1");
    EXPECT_EQ(reader.value(), "value1");
    EXPECT_EQ(reader.readNext(), CnConfReader::KeyValuePair);
    EXPECT_EQ(reader.key(), "key2");
    EXPECT_EQ(reader.value(), "value2");

    // Read Group2
    EXPECT_EQ(reader.readNext(), CnConfReader::Group); 
    EXPECT_EQ(reader.groupName(), "Group2");

    EXPECT_EQ(reader.readNext(), CnConfReader::KeyValuePair);
    EXPECT_EQ(reader.key(), "key3");
    EXPECT_EQ(reader.value(), "value3");

    EXPECT_EQ(reader.readNext(), CnConfReader::KeyValuePair);
    EXPECT_EQ(reader.key(), "key4 keypart4");
    EXPECT_EQ(reader.value(), "value4 valuepart4");

    EXPECT_EQ(reader.readNext(), CnConfReader::KeyValuePair);
    EXPECT_EQ(reader.key(), "key5");
    EXPECT_EQ(reader.value(), "value5");
    
    EXPECT_EQ(reader.readNext(), CnConfReader::KeyValuePair);
    EXPECT_EQ(reader.key(), "key6");
    EXPECT_EQ(reader.value(), "value6");
}

TEST_F(CnConfReaderTest, ReadEndOfFile)
{
    CnConfReader reader("test.conf");
    while (reader.readNext() != CnConfReader::EndOfFile)
    {
        // Keep reading until EOF
    }
    EXPECT_EQ(reader.currentToken(), CnConfReader::EndOfFile);
}

TEST_F(CnConfReaderTest, HandleInvalidFile)
{
    CnConfReader reader("nonexistent.conf");
    EXPECT_FALSE(reader.isOpen());
    EXPECT_TRUE(reader.hasError());
}