#include <gtest/gtest.h>

#include <filesystem>

#include <CnStd_sstream.h>
#include <CnStd_filesystem.h>

#include <CnFile.h>

namespace fs = std::filesystem;

#define _TMP_DIR_PARENT "."
#define _TMP_DIR_NAME   "tmpcnfiletest"
#define _TMP_DIR_PATH   _TMP_DIR_PARENT "/" _TMP_DIR_NAME
#define _TMP_FILE_NAME  "testfile.txt"


#define TMP_DIR_PARENT CnSTR(_TMP_DIR_PARENT)
#define TMP_DIR_NAME   CnSTR(_TMP_DIR_NAME  )
#define TMP_DIR_PATH   CnSTR(_TMP_DIR_PATH  )
#define TMP_FILE_NAME  CnSTR(_TMP_FILE_NAME )

class CnFileTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a temporary configuration for testing
        fs::create_directory(TMP_DIR_PATH);
    }

    void TearDown() override
    {
        // Remove the temporary configuration after testing
        try
        {
            fs::remove_all(TMP_DIR_PATH);
        }
        catch (const std::exception&)
        {
            // Optionally log or ignore the exception
        }
    }
};

TEST_F(CnFileTest, open_read_write_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    CnFileInfo info = file.info();
    CnString abs = info.absoluteFilePath();


    EXPECT_FALSE(file.exists());
    EXPECT_FALSE(file.isOpen());
    EXPECT_FALSE(file.isWritable());
    EXPECT_FALSE(file.isReadable());

    EXPECT_TRUE(file.open(Cn::Open_ReadWrite));

    EXPECT_TRUE(file.exists());
    EXPECT_TRUE(file.isOpen());
    EXPECT_TRUE(file.isWritable());
    EXPECT_TRUE(file.isReadable());

    file.close();
    EXPECT_FALSE(file.isOpen());
}

TEST_F(CnFileTest, close_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_FALSE(file.isOpen());
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    EXPECT_TRUE(file.isOpen());
    file.close();
    EXPECT_FALSE(file.isOpen());
}

TEST_F(CnFileTest, fileName_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file;
    CnString expected;
    EXPECT_EQ(file.fileName(), expected);
    file.setFileName(filePath);
    EXPECT_EQ(file.fileName(), TMP_FILE_NAME);
}

TEST_F(CnFileTest, size_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    EXPECT_EQ(file.size(), 0);
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    EXPECT_EQ(file.size(), textLen);
    file.close();
    EXPECT_EQ(file.size(), textLen);
}

TEST_F(CnFileTest, pos_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    EXPECT_EQ(file.pos(), 0);
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    EXPECT_EQ(file.pos(), textLen);
    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    EXPECT_EQ(file.pos(), textLen);
    file.close();
}

TEST_F(CnFileTest, atEnd_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    EXPECT_TRUE(file.atEnd());
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    EXPECT_FALSE(file.atEnd());
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    EXPECT_TRUE(file.atEnd());
    file.close();
}

TEST_F(CnFileTest, read_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    EXPECT_EQ(file.pos(), textLen);

    // Read beyond the end
    EXPECT_EQ(file.read(buffer, 10), 0);
    EXPECT_TRUE(file.atEnd());
    file.close();
}

TEST_F(CnFileTest, readLine_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Line 1\nLine 2\nLine 3\n";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.readLine(buffer, sizeof(buffer)), 7); // "Line 1\n"
    EXPECT_STREQ(buffer, "Line 1\n");
    EXPECT_EQ(file.pos(), 7);

    EXPECT_EQ(file.readLine(buffer, sizeof(buffer)), 7); // "Line 2\n"
    EXPECT_STREQ(buffer, "Line 2\n");
    EXPECT_EQ(file.pos(), 14);

    EXPECT_EQ(file.readLine(buffer, sizeof(buffer)), 7); // "Line 3\n"
    EXPECT_STREQ(buffer, "Line 3\n");
    EXPECT_EQ(file.pos(), 21);

    // Read beyond the end
    EXPECT_EQ(file.readLine(buffer, sizeof(buffer)), 0);
    EXPECT_TRUE(file.atEnd());
    file.close();
}

TEST_F(CnFileTest, get_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "ABC\xFF"; // Include a non-ASCII character
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, textLen), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);

    EXPECT_EQ(file.get(), 'A');
    EXPECT_EQ(file.pos(), 1);

    EXPECT_EQ(file.get(), 'B');
    EXPECT_EQ(file.pos(), 2);

    EXPECT_EQ(file.get(), 'C');
    EXPECT_EQ(file.pos(), 3);

    EXPECT_EQ(file.get(), 0xFF);
    EXPECT_EQ(file.pos(), 4);

    EXPECT_EQ(file.get(), EOF);
    EXPECT_TRUE(file.atEnd());
    file.close();
}

TEST_F(CnFileTest, write_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    EXPECT_EQ(file.size(), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    file.close();
}

TEST_F(CnFileTest, seek_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);

    EXPECT_TRUE(file.seek(7));
    EXPECT_EQ(file.pos(), 7);
    EXPECT_EQ(file.read(buffer, 5), 5);
    buffer[5] = '\0';
    EXPECT_STREQ(buffer, "World");

    // Seek beyond the end
    EXPECT_TRUE(file.seek(100));
    EXPECT_EQ(file.pos(), 100);
    EXPECT_EQ(file.read(buffer, 5), 0);
    EXPECT_TRUE(file.atEnd());
    file.close();
}

TEST_F(CnFileTest, peek_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);

    EXPECT_TRUE(file.seek(0));
    EXPECT_EQ(file.pos(), 0);
    char buffer[20] = {0};
    EXPECT_EQ(file.peek(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    EXPECT_EQ(file.pos(), 0); // Position should not change after peek

    // Peek beyond the end
    EXPECT_EQ(file.peek(buffer, 30), textLen);
    EXPECT_STREQ(buffer, text);
    EXPECT_EQ(file.pos(), 0); // Position should still not change
    file.close();
}

TEST_F(CnFileTest, resize_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    EXPECT_EQ(file.size(), textLen);

    // Resize to larger size
    EXPECT_TRUE(file.resize(50));
    EXPECT_EQ(file.size(), 50);

    // Resize to smaller size
    EXPECT_TRUE(file.resize(5));
    EXPECT_EQ(file.size(), 5);

    EXPECT_TRUE(file.seek(0));
    char buffer[20] = {0};
    EXPECT_EQ(file.read(buffer, 20), 5);
    buffer[5] = '\0';
    EXPECT_STREQ(buffer, "Hello");
    file.close();
}

TEST_F(CnFileTest, flush_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    EXPECT_TRUE(file.flush());
    file.close();
}

TEST_F(CnFileTest, copy_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    file.close();
    EXPECT_TRUE(file.exists());

    CnString copyPath = CnStd::toString(fs::path(TMP_DIR_PATH) / "copy_" _TMP_FILE_NAME);
    EXPECT_TRUE(file.copy(copyPath));
    CnFile copyFile(copyPath);
    EXPECT_TRUE(copyFile.exists());
    EXPECT_TRUE(copyFile.open(Cn::Open_ReadOnly));
    char buffer[20] = {0};
    EXPECT_EQ(copyFile.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    copyFile.close();
}

TEST_F(CnFileTest, rename_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    file.close();
    EXPECT_TRUE(file.exists());

    CnString newPath = CnStd::toString(fs::path(TMP_DIR_PATH) / CnSTR("renamed_" _TMP_FILE_NAME));
    EXPECT_TRUE(file.rename(newPath));
    EXPECT_FALSE(file.exists());
    CnFile renamedFile(newPath);
    EXPECT_TRUE(renamedFile.exists());
    EXPECT_TRUE(renamedFile.open(Cn::Open_ReadOnly));
    char buffer[20] = {0};
    EXPECT_EQ(renamedFile.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    renamedFile.close();
}

TEST_F(CnFileTest, link_test)
{
#ifdef _WIN32
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    file.close();
    EXPECT_TRUE(file.exists());

    // Create shortcut with relative name (without .lnk extension)
    CnString relLinkName = CnSTR("shortcut_rel");
    EXPECT_TRUE(file.link(relLinkName));

    // Expect file created at TMP_DIR/shortcut_rel.lnk
    CnString lnkPath = CnStd::toString(fs::path(TMP_DIR_PATH) / CnSTR("shortcut_rel.lnk"));
    CnFileInfo linkInfo(lnkPath);
    linkInfo.refresh();
    ASSERT_TRUE(linkInfo.exists());
    ASSERT_TRUE(linkInfo.isShortcut());
    ASSERT_TRUE(linkInfo.isSymLink());

    // Compare canonical target paths
    CnFileInfo targetInfo(filePath);
    CnString gotCan = CnFileInfo(linkInfo.symLinkTarget()).canonicalFilePath();
    CnString expCan = targetInfo.canonicalFilePath();
    EXPECT_EQ(gotCan, expCan);

    // Ensure readable content via opening target directly
    CnFile targetFile(filePath);
    EXPECT_TRUE(targetFile.open(Cn::Open_ReadOnly));
    char buffer[20] = {0};
    EXPECT_EQ(targetFile.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    targetFile.close();

    // Create another shortcut with explicit .lnk extension inside subdir
    fs::create_directory(fs::path(TMP_DIR_PATH) / CnSTR("subdir"));
    file.setFileName(filePath);
    EXPECT_TRUE(file.link(CnSTR("subdir/myshortcut.lnk")));

    CnString lnk2Path = CnStd::toString(fs::path(TMP_DIR_PATH) / CnSTR("subdir") / CnSTR("myshortcut.lnk"));
    CnFileInfo linkInfo2(lnk2Path);
    linkInfo2.refresh();
    ASSERT_TRUE(linkInfo2.exists());
    ASSERT_TRUE(linkInfo2.isShortcut());
    CnString gotCan2 = CnFileInfo(linkInfo2.symLinkTarget()).canonicalFilePath();
    EXPECT_EQ(gotCan2, expCan);
#else
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    file.close();
    EXPECT_TRUE(file.exists());
    
    CnFileInfo linkInfo(CnStd::toString(fs::path(TMP_DIR_PATH) / CnSTR("link_" _TMP_FILE_NAME)));
    CnString linkPath = linkInfo.fileName();
    EXPECT_TRUE(file.link(linkPath));
    
    linkInfo.refresh();
    EXPECT_TRUE(linkInfo.exists());
    EXPECT_TRUE(linkInfo.isSymLink());
    EXPECT_EQ(linkInfo.size(), textLen);
    EXPECT_EQ(linkInfo.symLinkTarget(), TMP_FILE_NAME);
    
    CnFile linkFile(linkInfo.filePath());
    EXPECT_TRUE(linkFile.exists());
    EXPECT_TRUE(linkFile.open(Cn::Open_ReadOnly));
    char buffer[20] = {0};
    EXPECT_EQ(linkFile.read(buffer, textLen), textLen);
    EXPECT_STREQ(buffer, text);
    linkFile.close();

    fs::create_directory(CnSTR(_TMP_DIR_PATH "/subdir"));

    // Create file and link within subdir to it
    CnStd::ofstream(CnSTR(_TMP_DIR_PATH "/dummy.txt")).close();
    file.setFileName(CnSTR(_TMP_DIR_PATH "/dummy.txt"));
    linkPath = CnSTR("subdir/link_to_dummy.txt");
    EXPECT_TRUE(file.link(linkPath));
    
    linkFile.setFileName(CnSTR(_TMP_DIR_PATH "/subdir/link_to_dummy.txt"));
    EXPECT_TRUE(linkFile.exists());
    EXPECT_EQ(linkFile.symLinkTarget(), CnSTR("../dummy.txt"));

    // Create file within subdir and link to it within current dir
    CnStd::ofstream(CnSTR(_TMP_DIR_PATH "/subdir/dummy2.txt")).close();
    file.setFileName(CnSTR(_TMP_DIR_PATH "/subdir/dummy2.txt"));
    linkPath = CnSTR("../link_to_dummy2.txt");
    EXPECT_TRUE(file.link(linkPath));

    linkFile.setFileName(CnSTR(_TMP_DIR_PATH "/link_to_dummy2.txt"));
    EXPECT_TRUE(linkFile.exists());
    EXPECT_EQ(linkFile.symLinkTarget(), "subdir/dummy2.txt");
#endif
}

TEST_F(CnFileTest, remove_test)
{
    CnString filePath = CnStd::toString(fs::path(TMP_DIR_PATH) / TMP_FILE_NAME);
    CnFile file(filePath);
    EXPECT_TRUE(file.open(Cn::Open_ReadWrite | Cn::Open_Truncate));
    const char *text = "Hello, World!";
    int64_t textLen = (int64_t)strlen(text);
    EXPECT_EQ(file.write(text, textLen), textLen);
    file.close();
    EXPECT_TRUE(file.exists());
    EXPECT_TRUE(file.remove());
    EXPECT_FALSE(file.exists());
}

