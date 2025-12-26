#include <gtest/gtest.h>

#include <CnStd_sstream.h>
#include <CnStd_filesystem.h>
#include <CnFileInfo.h>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#ifndef SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
#define SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE 0x2
#endif
#endif

namespace fs = std::filesystem;

#define _TMP_DIR_PARENT "."
#define _TMP_DIR_NAME   "tmpCnFileInfoTest"
#define _TMP_DIR_PATH   _TMP_DIR_PARENT "/" _TMP_DIR_NAME
#define _TMP_FILE_NAME  "testfile.txt"
#define _TMP_FILE_PATH  _TMP_DIR_PATH "/" _TMP_FILE_NAME


#define TMP_DIR_PARENT CnSTR(_TMP_DIR_PARENT)
#define TMP_DIR_NAME   CnSTR(_TMP_DIR_NAME  )
#define TMP_DIR_PATH   CnSTR(_TMP_DIR_PATH  )
#define TMP_FILE_NAME  CnSTR(_TMP_FILE_NAME )
#define TMP_FILE_PATH  CnSTR(_TMP_FILE_PATH )

class CnFileInfoTest : public ::testing::Test
{
protected:
    fs::path prev_cwd_;

    void SetUp() override
    {
        // Create `tmp/chainua/tests` in current user's home directory (platform independent)
        const char* homeEnv = std::getenv("HOME");
        if (!homeEnv) homeEnv = std::getenv("USERPROFILE");
        if (homeEnv && *homeEnv)
        {
            fs::path homePath(homeEnv);
            fs::path chainuaTests = homePath / "tmp" / "chainua" / "tests";
            std::error_code ec;
            fs::create_directories(chainuaTests, ec);
            // Optional: ignore errors; tests should proceed even if creation fails
            prev_cwd_ = fs::current_path();
            fs::current_path(chainuaTests, ec);
        }
        // Create a temporary configuration for testing
        fs::create_directory(TMP_DIR_PATH);
    }

    void TearDown() override
    {
        // Remove the temporary configuration after testing
        try
        {
            fs::remove_all(TMP_DIR_PATH);
            if (!prev_cwd_.empty())
            {
                std::error_code ec;
                fs::current_path(prev_cwd_, ec);
            }
        }
        catch (const std::exception& e)
        {
            // Optionally log or ignore the exception
        }
    }
};

TEST_F(CnFileInfoTest, constructor_test)
{
    CnFileInfo fileInfo1(TMP_DIR_PATH);
    EXPECT_TRUE(fileInfo1.filePath() == TMP_DIR_PATH  );
    EXPECT_TRUE(fileInfo1.dirPath()  == TMP_DIR_PARENT);
    EXPECT_TRUE(fileInfo1.fileName() == TMP_DIR_NAME  );
    EXPECT_TRUE(fileInfo1.exists());
    EXPECT_TRUE(fileInfo1.isDir());
    EXPECT_FALSE(fileInfo1.isFile());

    CnFileInfo fileInfo2;
    EXPECT_TRUE(fileInfo2.isNull());
    EXPECT_FALSE(fileInfo2.exists());
    EXPECT_FALSE(fileInfo2.isDir());
    EXPECT_FALSE(fileInfo2.isFile());

    CnFileInfo fileInfo3 = fileInfo1;
    EXPECT_TRUE(fileInfo3.filePath() == TMP_DIR_PATH  );
    EXPECT_TRUE(fileInfo3.dirPath()  == TMP_DIR_PARENT);
    EXPECT_TRUE(fileInfo3.fileName() == TMP_DIR_NAME  );
    EXPECT_TRUE(fileInfo3.exists());
    EXPECT_TRUE(fileInfo3.isDir());
    EXPECT_FALSE(fileInfo3.isFile());

    CnFileInfo fileInfo4 = std::move(fileInfo1);
    EXPECT_TRUE(fileInfo4.filePath() == TMP_DIR_PATH  );
    EXPECT_TRUE(fileInfo4.dirPath()  == TMP_DIR_PARENT);
    EXPECT_TRUE(fileInfo4.fileName() == TMP_DIR_NAME  );
    EXPECT_TRUE(fileInfo4.exists());
    EXPECT_TRUE(fileInfo4.isDir());
    EXPECT_FALSE(fileInfo4.isFile());

    CnFileInfo fileInfo5(TMP_FILE_PATH);
    EXPECT_TRUE(fileInfo5.filePath() == TMP_FILE_PATH);
    EXPECT_TRUE(fileInfo5.dirPath()  == TMP_DIR_PATH );
    EXPECT_TRUE(fileInfo5.fileName() == TMP_FILE_NAME);

    EXPECT_FALSE(fileInfo5.exists());

    // Create file using std::filesystem
    CnStd::ofstream ofs(TMP_FILE_PATH);
    ofs << "Test file content";
    ofs.close();

    fileInfo5.refresh();
    EXPECT_TRUE(fileInfo5.exists());
    EXPECT_FALSE(fileInfo5.isDir());
    EXPECT_TRUE(fileInfo5.isFile());
}

TEST_F(CnFileInfoTest, baseName_test)
{
    CnFileInfo i1("test.txt");
    EXPECT_EQ(i1.baseName(), "test");

    CnFileInfo i2("test.0.txt");
    EXPECT_EQ(i2.baseName(), "test");

    CnFileInfo i3("test.0.1.txt");
    EXPECT_EQ(i3.baseName(), "test");
}

TEST_F(CnFileInfoTest, CopyAssignmentOperatorTest)
{
    CnFileInfo fileInfo1(TMP_DIR_PATH);
    CnFileInfo fileInfo2;
    fileInfo2 = fileInfo1;

    EXPECT_FALSE(fileInfo2.isNull());
    EXPECT_EQ(fileInfo2.path(), fileInfo1.path());

    CnFileInfo fileInfo3(CnSTR("copy/path/test"));
    fileInfo3 = fileInfo1;

    EXPECT_FALSE(fileInfo3.isNull());
    EXPECT_EQ(fileInfo3.path(), fileInfo1.path());
}

TEST_F(CnFileInfoTest, MoveAssignmentOperatorTest)
{
    CnFileInfo fileInfo1(TMP_DIR_PATH);
    CnFileInfo fileInfo2;
    fileInfo2 = std::move(fileInfo1);

    EXPECT_TRUE(fileInfo1.isNull());
    EXPECT_FALSE(fileInfo2.isNull());

    CnFileInfo fileInfo3(TMP_DIR_PATH);
    CnFileInfo fileInfo4(CnSTR("copy/path/test"));
    fileInfo4 = std::move(fileInfo3);

    EXPECT_TRUE(fileInfo3.isNull());
    EXPECT_FALSE(fileInfo4.isNull());
    EXPECT_EQ(fileInfo4.path(), fileInfo2.path());
}

TEST_F(CnFileInfoTest, completeBaseName_test)
{
    CnFileInfo i1("test.txt");
    EXPECT_EQ(i1.completeBaseName(), "test");

    CnFileInfo i2("test.0.txt");
    EXPECT_EQ(i2.completeBaseName(), "test.0");

    CnFileInfo i3("test.0.1.txt");
    EXPECT_EQ(i3.completeBaseName(), "test.0.1");
}

TEST_F(CnFileInfoTest, suffix_test)
{
    CnFileInfo i1("test.txt");
    EXPECT_EQ(i1.suffix(), "txt");

    CnFileInfo i2("test.0.txt");
    EXPECT_EQ(i2.suffix(), "txt");

    CnFileInfo i3("test.0.1.txt");
    EXPECT_EQ(i3.suffix(), "txt");
}

TEST_F(CnFileInfoTest, completeSuffix_test)
{
    CnFileInfo i1("test.txt");
    EXPECT_EQ(i1.completeSuffix(), "txt");

    CnFileInfo i2("test.0.txt");
    EXPECT_EQ(i2.completeSuffix(), "0.txt");

    CnFileInfo i3("test.0.1.txt");
    EXPECT_EQ(i3.completeSuffix(), "0.1.txt");
}

TEST_F(CnFileInfoTest, symLinkTarget_test)
{
    //// Create a file and a symlink to it
    //CnStd::ofstream ofs(TMP_FILE_PATH);
    //ofs << "Symlink target content";
    //ofs.close();
    //
    //CnString symlinkName = CnSTR("test_symlink.txt");
    //CnString symlinkPath = CnSTR(_TMP_DIR_PATH  "/") + symlinkName;
    //
    //// Remove symlink if it exists
    //std::error_code ec;
    //fs::remove(fs::path(symlinkPath.data()), ec);
    //
    //// Create symlink
    //fs::create_symlink(fs::path(TMP_FILE_PATH), fs::path(symlinkPath.data()), ec);
    //ASSERT_FALSE(ec) << "Failed to create symlink: " << ec.message();
    //
    //CnFileInfo symlinkInfo(symlinkPath);
    //EXPECT_TRUE(symlinkInfo.exists());
    //EXPECT_TRUE(symlinkInfo.isSymLink());
    //
    //// symLinkTarget should return the target file path
    //EXPECT_EQ(symlinkInfo.symLinkTarget(), TMP_FILE_PATH);
    //
    //// For a regular file, symLinkTarget should return an empty string
    //CnFileInfo fileInfo(TMP_FILE_PATH);
    //EXPECT_FALSE(fileInfo.isSymLink());
    //EXPECT_EQ(fileInfo.symLinkTarget(), CnString());
    //
    //// For a null file info, symLinkTarget should return empty string
    //CnFileInfo nullInfo;
    //EXPECT_EQ(nullInfo.symLinkTarget(), CnString());
}

#ifdef _WIN32
TEST_F(CnFileInfoTest, symLinkTarget_windows_lnk_test)
{
    // Create a target file
    CnStd::ofstream ofs(TMP_FILE_PATH);
    ofs << "Shortcut target content";
    ofs.close();

    // Absolute path to target for comparison
    CnFileInfo targetInfo(TMP_FILE_PATH);
    CnString expectedAbs = targetInfo.absoluteFilePath();

    // Create a .lnk shortcut via COM
    HRESULT hrInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    bool uninit = SUCCEEDED(hrInit);
    IShellLinkA* psl = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (void**)&psl);
    if (FAILED(hr) || !psl)
    {
        if (uninit) CoUninitialize();
        GTEST_SKIP() << "CoCreateInstance for ShellLink failed. hr=" << std::hex << hr;
    }

    psl->SetPath(expectedAbs.chars());

    IPersistFile* ppf = nullptr;
    hr = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
    if (FAILED(hr) || !ppf)
    {
        psl->Release();
        if (uninit) CoUninitialize();
        GTEST_SKIP() << "IPersistFile QI failed. hr=" << std::hex << hr;
    }

    // Save shortcut to TMP_DIR_PATH/shortcut.lnk
    CnString lnkPath = CnString(TMP_DIR_PATH) + CnSTR("/shortcut.lnk");
    wchar_t wlnk[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, lnkPath.chars(), -1, wlnk, MAX_PATH);
    hr = ppf->Save(wlnk, TRUE);
    ppf->Release();
    psl->Release();
    if (uninit) CoUninitialize();

    if (FAILED(hr))
        GTEST_SKIP() << ".lnk save failed. hr=" << std::hex << hr;

    CnFileInfo linkInfo(lnkPath);
    ASSERT_TRUE(linkInfo.exists());
    ASSERT_TRUE(linkInfo.isShortcut());
    ASSERT_TRUE(linkInfo.isSymLink());

    CnString got = linkInfo.symLinkTarget();
    // Compare canonical absolute file paths for robustness on Windows
    CnString gotCan = CnFileInfo(got).canonicalFilePath();
    CnString expCan = targetInfo.canonicalFilePath();
    EXPECT_EQ(gotCan, expCan);
}

TEST_F(CnFileInfoTest, symLinkTarget_windows_symlink_test)
{
    // Create a target file and create a file symlink to it
    CnStd::ofstream ofs(TMP_FILE_PATH);
    ofs << "Symlink target content";
    ofs.close();

    // Absolute target path
    CnFileInfo targetInfo(TMP_FILE_PATH);
    CnString expectedAbs = targetInfo.absoluteFilePath();

    CnString linkPath = CnString(TMP_DIR_PATH) + CnSTR("/file_symlink.txt");

    // Remove if exists
    std::error_code ec;
    fs::remove(fs::path(linkPath.data()), ec);

    // Create symbolic link (allow unprivileged if available)
    BOOL ok = ::CreateSymbolicLinkA(linkPath.chars(), expectedAbs.chars(), 0 | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE);
    if (!ok)
    {
        DWORD err = GetLastError();
        // Privilege not held or not allowed in environment: skip test gracefully
        if (err == ERROR_PRIVILEGE_NOT_HELD || err == ERROR_INVALID_PARAMETER || err == ERROR_ACCESS_DENIED)
        {
            GTEST_SKIP() << "CreateSymbolicLink failed, err=" << err << ". Likely no privilege or Dev Mode disabled.";
        }
        ASSERT_TRUE(ok) << "CreateSymbolicLink failed with err=" << err;
    }

    CnFileInfo linkInfo(linkPath);
    ASSERT_TRUE(linkInfo.exists());
    ASSERT_TRUE(linkInfo.isSymLink());

    CnString got = linkInfo.symLinkTarget();
    CnString gotCan = CnFileInfo(got).canonicalFilePath();
    CnString expCan = targetInfo.canonicalFilePath();
    EXPECT_EQ(gotCan, expCan);
}
#endif // _WIN32


TEST_F(CnFileInfoTest, assignment_and_equality_test)
{
    CnFileInfo info1(TMP_FILE_PATH);
    CnFileInfo info2;
    info2 = info1;
    EXPECT_TRUE(info2 == info1);
    EXPECT_FALSE(info2 != info1);

    CnFileInfo info3;
    info3 = std::move(info2);
    EXPECT_TRUE(info3 == info1);
    EXPECT_FALSE(info3 != info1);
}

TEST_F(CnFileInfoTest, absoluteFilePath_test)
{
    CnString cwd = CnFileInfo::normalizePathSeparators(CnStd::toString(fs::current_path()));

    CnString file1 = CnSTR("relative/path/to/file.txt");
    CnFileInfo info1(file1);
    CnString expected1 = cwd + CN_PATH_SEP + file1;
    CnString value1 = info1.absoluteFilePath();
    EXPECT_EQ(value1, expected1);

    CnString file2 = CnSTR("relative\\path\\to\\file.txt");
    CnFileInfo info2(file2);
    CnString expected2 = cwd + CN_PATH_SEP + CnSTR("relative/path/to/file.txt");
    CnString value2 = info2.absoluteFilePath();
    EXPECT_EQ(value2, expected2);

    CnString file3 = CnSTR("relative/path/../to/file.txt");
    CnFileInfo info3(file3);
    CnString expected3 = cwd + CN_PATH_SEP + file3;
    CnString value3 = info3.absoluteFilePath();
    EXPECT_EQ(value3, expected3);
}

TEST_F(CnFileInfoTest, absolutePath_test)
{
    CnString cwd = CnFileInfo::normalizePathSeparators(CnStd::toString(fs::current_path()));

    CnString file1 = CnSTR("relative/path/to/file.txt");
    CnFileInfo info1(file1);
    CnString expected1 = cwd + CN_PATH_SEP + CnSTR("relative/path/to");
    CnString value1 = info1.absolutePath();
    EXPECT_EQ(value1, expected1);

    CnString file2 = CnSTR("relative\\path\\to\\file.txt");
    CnFileInfo info2(file2);
    CnString expected2 = cwd + CN_PATH_SEP + CnSTR("relative/path/to");
    CnString value2 = info2.absolutePath();
    EXPECT_EQ(value2, expected2);

    CnString file3 = CnSTR("relative/path/../to/file.txt");
    CnFileInfo info3(file3);
    CnString expected3 = cwd + CN_PATH_SEP + CnSTR("relative/path/../to");
    CnString value3 = info3.absolutePath();
    EXPECT_EQ(value3, expected3);
}

TEST_F(CnFileInfoTest, canonicalFilePath_test)
{
    CnString cwd = CnFileInfo::normalizePathSeparators(CnStd::toString(fs::current_path()));

    CnString file1 = CnSTR("relative/./path/../to/file.txt");
    CnFileInfo info1(file1);
    CnString expected1 = cwd + CN_PATH_SEP + CnSTR("relative/to/file.txt");
    CnString value1 = info1.canonicalFilePath();
    EXPECT_EQ(value1, expected1);

    CnString file2 = CnSTR("relative\\.\\path\\..\\to\\file.txt");
    CnFileInfo info2(file2);
    CnString expected2 = cwd + CN_PATH_SEP + CnSTR("relative/to/file.txt");
    CnString value2 = info2.canonicalFilePath();
    EXPECT_EQ(value2, expected2);

    CnString file3 = CnSTR("relative/path/../../to/file.txt");
    CnFileInfo info3(file3);
    CnString expected3 = cwd + CN_PATH_SEP + CnSTR("to/file.txt");
    CnString value3 = info3.canonicalFilePath();
    EXPECT_EQ(value3, expected3);
}

TEST_F(CnFileInfoTest, canonicalPath_test)
{
    CnString cwd = CnFileInfo::normalizePathSeparators(CnStd::toString(fs::current_path()));

    CnString file1 = CnSTR("relative/./path/../to/file.txt");
    CnFileInfo info1(file1);
    CnString expected1 = cwd + CN_PATH_SEP + CnSTR("relative/to");
    CnString value1 = info1.canonicalPath();
    EXPECT_EQ(value1, expected1);

    CnString file2 = CnSTR("relative\\.\\path\\..\\to\\file.txt");
    CnFileInfo info2(file2);
    CnString expected2 = cwd + CN_PATH_SEP + CnSTR("relative/to");
    CnString value2 = info2.canonicalPath();
    EXPECT_EQ(value2, expected2);

    CnString file3 = CnSTR("relative/path/..//../to/file.txt");
    CnFileInfo info3(file3);
    CnString expected3 = cwd + CN_PATH_SEP + CnSTR("to");
    CnString value3 = info3.canonicalPath();
    EXPECT_EQ(value3, expected3);
}