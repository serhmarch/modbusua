#include <gtest/gtest.h>

#include <CnString.h>

TEST(CnStringTest, DefaultConstructorTest)
{
    CnString str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_TRUE(str.isNull());
    EXPECT_TRUE(str.isEmpty());
    EXPECT_NE(str.data(), nullptr);
}

TEST(CnStringTest, ConstructorFromCStringSizeTest)
{
    const CnChar* cstr = CnSTR("Hello, World!");
    size_t len = 5; // Only take "Hello"
    CnString str(cstr, len);
    EXPECT_EQ(str.size(), len);
    EXPECT_FALSE(str.isNull());
    EXPECT_FALSE(str.isEmpty());
    EXPECT_EQ(CnStd::strncmp(str.data(), cstr, len), 0);
    EXPECT_EQ(str.data()[len], CnCHR('\0')); // Ensure null-termination
}

TEST(CnStringTest, ConstructorFromCStringTest)
{
    const CnChar* cstr = CnSTR("Hello, World!");
    CnString str(cstr);
    EXPECT_EQ(str.size(), CnStd::strlen(cstr));
    EXPECT_FALSE(str.isNull());
    EXPECT_FALSE(str.isEmpty());
    EXPECT_STREQ(str.data(), cstr);
}

TEST(CnStringTest, CopyAssigmentOperatorTest)
{
    CnString str1(CnCHR("Hello"));

    CnString str2;
    str2 = str1;
    EXPECT_EQ(str2.size(), str1.size());
    EXPECT_FALSE(str2.isNull());
    EXPECT_FALSE(str2.isEmpty());
    EXPECT_STREQ(str2.data(), str1.data());

    CnString str3(CnCHR("Hello3"));
    str3 = str1;
    EXPECT_EQ(str3.size(), str1.size());
    EXPECT_FALSE(str3.isNull());
    EXPECT_FALSE(str3.isEmpty());
    EXPECT_STREQ(str3.data(), str1.data());
}

TEST(CnStringTest, fromChars_cstr_test)
{
    const char* s = "Hello, World!";
    CnString str = CnString::fromChars(s);
    EXPECT_EQ(str.size(), CnStd::strlen(CnSTR("Hello, World!")));
    EXPECT_FALSE(str.isNull());
    EXPECT_FALSE(str.isEmpty());
    EXPECT_STREQ(str.data(), CnSTR("Hello, World!"));
}

TEST(CnStringTest, fromChars_cstr_len_test)
{
    const char* s = "Hello, World!";
    size_t len = 5;
    CnString str = CnString::fromChars(s, len);
    EXPECT_EQ(str.size(), len);
    EXPECT_FALSE(str.isNull());
    EXPECT_FALSE(str.isEmpty());
    EXPECT_EQ(CnStd::strncmp(str.data(), CnSTR("Hello"), len), 0);
    EXPECT_EQ(str.data()[len], CnCHR('\0'));
}

TEST(CnStringTest, insert_single_char_at_end_test)
{
    CnString str(CnCHR("ABCD"));
    str.insert(4, 1, CnCHR('X'));
    EXPECT_EQ(str.size(), 5);
    EXPECT_STREQ(str.data(), CnSTR("ABCDX"));
}

TEST(CnStringTest, insert_count_char_test)
{
    CnString str(CnCHR("ABCD"));
    str.insert(2, 3, CnCHR('X'));
    EXPECT_EQ(str.size(), 7);
    EXPECT_STREQ(str.data(), CnSTR("ABXXXCD"));
    
    str.insert(0, 3, CnCHR('0'));
    EXPECT_EQ(str.size(), 10);
    EXPECT_STREQ(str.data(), CnSTR("000ABXXXCD"));
    
    str.insert(10, 3, CnCHR('9'));
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("000ABXXXCD999"));
}

TEST(CnStringTest, insert_cstr_test)
{
    CnString str(CnCHR("ABCD"));
    str.insert(2, CnCHR("XYZ"));
    EXPECT_EQ(str.size(), 7);
    EXPECT_STREQ(str.data(), CnSTR("ABXYZCD"));
    
    str.insert(0, CnCHR("123"));
    EXPECT_EQ(str.size(), 10);
    EXPECT_STREQ(str.data(), CnSTR("123ABXYZCD"));
    
    str.insert(10, CnCHR("789"));
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("123ABXYZCD789"));
}

TEST(CnStringTest, insert_cstr_count_test)
{
    CnString str(CnCHR("ABCD"));
    str.insert(2, CnCHR("WXYZ"), 2); // Insert "WX"
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABWXCD"));
    
    str.insert(0, CnCHR("12345"), 3); // Insert "123"
    EXPECT_EQ(str.size(), 9);
    EXPECT_STREQ(str.data(), CnSTR("123ABWXCD"));
    
    str.insert(9, CnCHR("67890"), 2); // Insert "67"
    EXPECT_EQ(str.size(), 11);
    EXPECT_STREQ(str.data(), CnSTR("123ABWXCD67"));
}

TEST(CnStringTest, insert_string_test)
{
    CnString str(CnCHR("ABCD"));
    CnString toInsert(CnCHR("MNO"));
    str.insert(2, toInsert);
    EXPECT_EQ(str.size(), 7);
    EXPECT_STREQ(str.data(), CnSTR("ABMNOCD"));
    
    str.insert(0, toInsert);
    EXPECT_EQ(str.size(), 10);
    EXPECT_STREQ(str.data(), CnSTR("MNOABMNOCD"));
    
    str.insert(10, toInsert);
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("MNOABMNOCDMNO"));
}

TEST(CnStringTest, erase_index_count_test)
{
    CnString str(CnCHR("ABCDEFGHIJ"));
    str.erase(2, 4); // Remove "CDEF"
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABGHIJ"));
    
    str.erase(0, 2); // Remove "AB"
    EXPECT_EQ(str.size(), 4);
    EXPECT_STREQ(str.data(), CnSTR("GHIJ"));
    
    str.erase(2, 10); // Remove from index 2 to end
    EXPECT_EQ(str.size(), 2);
    EXPECT_STREQ(str.data(), CnSTR("GH"));
    
    str.erase(0, 5); // Remove all
    EXPECT_EQ(str.size(), 0);
    EXPECT_STREQ(str.data(), CnSTR(""));
}

TEST(CnStringTest, erase_itposition_test)
{
    CnString str(CnCHR("ABCDEFGHIJ"));
    auto it = str.begin() + 2; // Points to 'C'
    str.erase(it);
    EXPECT_EQ(str.size(), 9);
    EXPECT_STREQ(str.data(), CnSTR("ABDEFGHIJ"));
    
    it = str.begin(); // Points to 'A'
    str.erase(it);
    EXPECT_EQ(str.size(), 8);
    EXPECT_STREQ(str.data(), CnSTR("BDEFGHIJ"));
    
    it = str.end() - 1; // Points to 'J'
    str.erase(it);
    EXPECT_EQ(str.size(), 7);
    EXPECT_STREQ(str.data(), CnSTR("BDEFGHI"));
}

TEST(CnStringTest, erase_itrange_test)
{
    CnString str(CnCHR("ABCDEFGHIJ"));
    auto first = str.begin() + 2; // Points to 'C'
    auto last = str.begin() + 6;  // Points to 'G'
    str.erase(first, last); // Remove "CDEF"
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABGHIJ"));
    
    first = str.begin(); // Points to 'A'
    last = str.begin() + 2; // Points to 'G'
    str.erase(first, last); // Remove "AB"
    EXPECT_EQ(str.size(), 4);
    EXPECT_STREQ(str.data(), CnSTR("GHIJ"));
    
    first = str.begin() + 2; // Points to 'I'
    last = str.end(); // Points to end
    str.erase(first, last); // Remove "IJ"
    EXPECT_EQ(str.size(), 2);
    EXPECT_STREQ(str.data(), CnSTR("GH"));
    
    first = str.begin(); // Points to 'G'
    last = str.end(); // Points to end
    str.erase(first, last); // Remove all
    EXPECT_EQ(str.size(), 0);
    EXPECT_STREQ(str.data(), CnSTR(""));
}

TEST(CnStringTest, push_back_test)
{
    CnString str(CnCHR("ABC"));
    str.push_back(CnCHR('D'));
    str.push_back(CnCHR('E'));
    str.push_back(CnCHR('F'));
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABCDEF"));
}

TEST(CnStringTest, pop_back_test)
{
    CnString str(CnCHR("ABCDE"));
    str.pop_back();
    EXPECT_EQ(str.size(), 4);
    EXPECT_STREQ(str.data(), CnSTR("ABCD"));
    str.pop_back();
    str.pop_back();
    EXPECT_EQ(str.size(), 2);
    EXPECT_STREQ(str.data(), CnSTR("AB"));
    str.pop_back();
    str.pop_back();
    EXPECT_EQ(str.size(), 0);
    EXPECT_STREQ(str.data(), CnSTR(""));
    // Popping from empty string should do nothing
    str.pop_back();
    EXPECT_EQ(str.size(), 0);
    EXPECT_STREQ(str.data(), CnSTR(""));
}

TEST(CnStringTest, append_count_char_test)
{
    CnString str(CnCHR("ABC"));
    str.append(3, CnCHR('X'));
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABCXXX"));
}

TEST(CnStringTest, append_cstr_test)
{
    CnString str(CnCHR("ABC"));
    str.append(CnCHR("DEF"));
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABCDEF"));
}

TEST(CnStringTest, append_cstr_count_test)
{
    CnString str(CnCHR("ABC"));
    str.append(CnCHR("WXYZ"), 2); // Append "WX"
    EXPECT_EQ(str.size(), 5);
    EXPECT_STREQ(str.data(), CnSTR("ABCWX"));
}

TEST(CnStringTest, append_str)
{
    CnString str(CnCHR("ABC"));
    CnString toAppend(CnCHR("MNO"));
    str.append(toAppend);
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABCMNO"));
}

TEST(CnStringTest, append_initializer_list_test)
{
    CnString str(CnCHR("ABC"));
    str.append({CnCHR('X'), CnCHR('Y'), CnCHR('Z')});
    EXPECT_EQ(str.size(), 6);
    EXPECT_STREQ(str.data(), CnSTR("ABCXYZ"));
}

TEST(CnStringTest, CnStringTest_replace_pos_count_str_test)
{
    CnString str(CnCHR("Hello, World!"));
    CnString replacement(CnCHR("Universe"));
    str.replace(7, 5, replacement); // Replace "World" with "Universe"
    EXPECT_EQ(str.size(), 16);
    EXPECT_STREQ(str.data(), CnSTR("Hello, Universe!"));
    
    str.replace(0, 5, CnCHR("Hi")); // Replace "Hello" with "Hi"
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("Hi, Universe!"));
    
    str.replace(4, 20, CnCHR("Everyone")); // Replace from index 4 to end
    EXPECT_EQ(str.size(), 12);
    EXPECT_STREQ(str.data(), CnSTR("Hi, Everyone"));
}

TEST(CnStringTest, CnStringTest_replace_itfirst_itlast_str_test)
{
    CnString str(CnCHR("Hello, World!"));
    CnString replacement(CnCHR("Universe"));
    auto first = str.begin() + 7; // Points to 'W'
    auto last = str.begin() + 12; // Points to '!'
    str.replace(first, last, replacement); // Replace "World" with "Universe"
    EXPECT_EQ(str.size(), 16);
    EXPECT_STREQ(str.data(), CnSTR("Hello, Universe!"));
    
    first = str.begin(); // Points to 'H'
    last = str.begin() + 5; // Points to ','
    str.replace(first, last, CnCHR("Hi")); // Replace "Hello" with "Hi"
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("Hi, Universe!"));
    
    first = str.begin() + 4; // Points to 'U'
    last = str.end(); // Points to end
    str.replace(first, last, CnCHR("Everyone")); // Replace from index 4 to end
    EXPECT_EQ(str.size(), 12);
    EXPECT_STREQ(str.data(), CnSTR("Hi, Everyone"));
}

TEST(CnStringTest, CnStringTest_replace_pos_count_count2_ch_test)
{
    CnString str(CnCHR("Hello, World!"));
    // Replace "World" (5 chars starting at pos 7) with 3 '*'
    str.replace(7, 5, 3, CnCHR('*'));
    EXPECT_EQ(str.size(), 11);
    EXPECT_STREQ(str.data(), CnSTR("Hello, ***!"));

    // Replace beginning "Hello" (5 chars starting at pos 0) with two '!'
    str.replace(0, 5, 2, CnCHR('!'));
    EXPECT_EQ(str.size(), 8);
    EXPECT_STREQ(str.data(), CnSTR("!!, ***!"));

    // Replace from pos 4 to end with four '#'
    str.replace(4, 100, 4, CnCHR('#'));
    EXPECT_EQ(str.size(), 8);
    EXPECT_STREQ(str.data(), CnSTR("!!, ####"));
}

TEST(CnStringTest, CnStringTest_replace_itfirst_itlast_count2_ch_test)
{
    CnString str(CnCHR("ABCDEFGHIJ"));
    // Replace middle range [pos 2, pos 6) => "CDEF" with three 'X'
    auto first = str.begin() + 2;
    auto last = str.begin() + 6;
    str.replace(first, last, 3, CnCHR('X'));
    EXPECT_EQ(str.size(), 9);
    EXPECT_STREQ(str.data(), CnSTR("ABXXXGHIJ"));

    // Replace beginning range [0,2) => "AB" with single '0'
    first = str.begin();
    last = str.begin() + 2;
    str.replace(first, last, 1, CnCHR('0'));
    EXPECT_EQ(str.size(), 8);
    EXPECT_STREQ(str.data(), CnSTR("0XXXGHIJ"));

    // Replace tail range [6, end) => "IJ" with four '9'
    first = str.begin() + 6;
    last = str.end();
    str.replace(first, last, 4, CnCHR('9'));
    EXPECT_EQ(str.size(), 10);
    EXPECT_STREQ(str.data(), CnSTR("0XXXGH9999"));
}

TEST(CnStringTest, CnStringTest_replace_pos_ch_test)
{
    CnString str(CnCHR("Hello, World!"));
    // Replace single character at specific positions
    str.replace(7, CnCHR('w')); // "World" -> "world"
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("Hello, world!"));

    str.replace(0, CnCHR('h')); // "Hello" -> "hello"
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("hello, world!"));

    // Replace last character
    str.replace(str.size() - 1, CnCHR('?'));
    EXPECT_EQ(str.size(), 13);
    EXPECT_STREQ(str.data(), CnSTR("hello, world?"));
}

TEST(CnStringTest, swap_test)
{
    CnString str1(CnCHR("Hello"));
    CnString str2(CnCHR("World"));
    str1.swap(str2);
    EXPECT_EQ(str1.size(), 5);
    EXPECT_STREQ(str1.data(), CnSTR("World"));
    EXPECT_EQ(str2.size(), 5);
    EXPECT_STREQ(str2.data(), CnSTR("Hello"));
}

TEST(CnStringTest, clear_test)
{
    CnString str(CnCHR("Hello, World!"));
    str.clear();
    EXPECT_EQ(str.size(), 0);
    EXPECT_TRUE(str.isEmpty());
    EXPECT_STREQ(str.data(), CnSTR(""));
}

TEST(CnStringTest, resize_test)
{
    CnString str(CnCHR("Hello"));
    str.resize(10);
    EXPECT_EQ(str.size(), 10);
    EXPECT_STREQ(str.data(), CnSTR("Hello\0\0\0\0\0")); // Extra chars should be null
    
    str.resize(3);
    EXPECT_EQ(str.size(), 3);
    EXPECT_STREQ(str.data(), CnSTR("Hel")); // Should truncate to "Hel"
}

TEST(CnStringTest, find_test)
{
    CnString str(CnCHR("Hello, World! Hello, Universe!"));

    size_t pos = str.find(CnString(CnCHR("World")));
    EXPECT_EQ(pos, 7);
    
    pos = str.find(CnCHR("World"));
    EXPECT_EQ(pos, 7);
    
    pos = str.find(CnCHR("Hello"));
    EXPECT_EQ(pos, 0);
    
    pos = str.find(CnCHR("Hello"), 8);
    EXPECT_EQ(pos, 14);
    
    pos = str.find(CnCHR("XYZ"));
    EXPECT_EQ(pos, CnString::npos);
    
    pos = str.find(CnCHR('H'));
    EXPECT_EQ(pos, 0);
    
    pos = str.find(CnCHR('H'), 1);
    EXPECT_EQ(pos, 14);

    pos = str.find(CnString(CnCHR("Hi")));
    EXPECT_EQ(pos, CnString::npos);
}

TEST(CnStringTest, rfind_test)
{
    CnString str(CnCHR("Hello, World! Hello, Universe!"));

    size_t pos = str.rfind(CnString(CnCHR("Hello")));
    EXPECT_EQ(pos, 14);
    
    pos = str.rfind(CnCHR("Hello"));
    EXPECT_EQ(pos, 14);
    
    pos = str.rfind(CnCHR("Hello"), 10, 3);
    EXPECT_EQ(pos, 0);
    
    pos = str.rfind(CnCHR("World"), 10);
    EXPECT_EQ(pos, 7);
    
    pos = str.rfind(CnCHR("XYZ"));
    EXPECT_EQ(pos, CnString::npos);
    
    pos = str.rfind(CnCHR('!'));
    EXPECT_EQ(pos, 29);
    
    pos = str.rfind(CnCHR('H'), 13);
    EXPECT_EQ(pos, 0);

    pos = str.rfind(CnString(CnCHR("Hi")));
    EXPECT_EQ(pos, CnString::npos);
}

TEST(CnStringTest, compare_test)
{
    // Comparing empty strings
    EXPECT_EQ(CnString(CnSTR("")).compare(0, 0, CnSTR("")), 0);
    EXPECT_EQ(CnString(CnSTR("")).compare(0, 1, CnSTR("")), 0);
    EXPECT_LT(CnString(CnSTR("")).compare(1, 0, CnSTR("")), 2);
    EXPECT_LT(CnString(CnSTR("")).compare(0, 1, CnSTR(""), 1), 2);

    CnString s1(CnSTR("Hello\0World!"));
    EXPECT_EQ(s1.compare(0, 5, CnSTR("Hell")), 1);
    EXPECT_EQ(s1.compare(1, 8, CnSTR("ello")), 0);
    EXPECT_NE(s1.compare(1, 8, CnSTR("ello"), 2), 0);

    CnString s2(CnSTR("Hello"));
    CnString s3(CnSTR("World"));
    CnString s4(CnSTR("Hello"));
    
    EXPECT_LT(s2.compare(s3), 0); // "Hello" < "World"
    EXPECT_GT(s3.compare(s2), 0); // "World" > "Hello"
    EXPECT_EQ(s2.compare(s4), 0); // "Hello" == "Hello"
    
    EXPECT_EQ(s2.compare(CnSTR("Hello")), 0);
    EXPECT_NE(s2.compare(CnSTR("World")), 0);
    EXPECT_LT(s2.compare(CnSTR("World")), 0);
    
    EXPECT_EQ(s2.compare(0, 3, CnSTR("Hel")), 0);
    EXPECT_EQ(s2.compare(2, 3, CnSTR("llo")), 0);
    EXPECT_NE(s2.compare(0, 3, CnSTR("Wor")), 0);
    
    EXPECT_EQ(s2.compare(0, 5, CnSTR("HelloWorld"), 5), 0);
    EXPECT_EQ(s2.compare(0, 3, CnSTR("HelloWorld"), 3), 0);
    EXPECT_NE(s2.compare(0, 3, CnSTR("World"), 3), 0);
    
    CnString empty1;
    CnString empty2;
    EXPECT_EQ(empty1.compare(empty2), 0);
    EXPECT_LT(empty1.compare(s2), 0);
    EXPECT_GT(s2.compare(empty1), 0);

    CnString s5(CnSTR("abc"));
    EXPECT_NE(s5.compare(0, 3, "a", 1), 0); // "abc" != "a"
}


TEST(CnStringTest, starts_with_char_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.starts_with(CnCHR('H')));
    EXPECT_FALSE(str.starts_with(CnCHR('h')));
    EXPECT_FALSE(str.starts_with(CnCHR('W')));
}

TEST(CnStringTest, starts_with_cstr_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.starts_with(CnCHR("Hello")));
    EXPECT_FALSE(str.starts_with(CnCHR("hello")));
    EXPECT_FALSE(str.starts_with(CnCHR("World")));
}

TEST(CnStringTest, starts_with_string_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.starts_with(CnString(CnCHR("Hello"))));
    EXPECT_FALSE(str.starts_with(CnString(CnCHR("hello"))));
    EXPECT_FALSE(str.starts_with(CnString(CnCHR("World"))));
}

TEST(CnStringTest, ends_with_char_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.ends_with(CnCHR('!')));
    EXPECT_FALSE(str.ends_with(CnCHR('.')));
    EXPECT_FALSE(str.ends_with(CnCHR('d')));
}

TEST(CnStringTest, ends_with_cstr_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.ends_with(CnCHR("World!")));
    EXPECT_FALSE(str.ends_with(CnCHR("world!")));
    EXPECT_FALSE(str.ends_with(CnCHR("Hello")));
}

TEST(CnStringTest, ends_with_string_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.ends_with(CnString(CnCHR("World!"))));
    EXPECT_FALSE(str.ends_with(CnString(CnCHR("world!"))));
    EXPECT_FALSE(str.ends_with(CnString(CnCHR("Hello"))));
}

TEST(CnStringTest, contains_char_test)
{
    CnString str(CnCHR("Hello, World!"));
    EXPECT_TRUE(str.constains(CnCHR('H')));
    EXPECT_TRUE(str.constains(CnCHR('o')));
    EXPECT_FALSE(str.constains(CnCHR('z')));
}