#include <gtest/gtest.h>

#include <fstream>

#include <CnStd_filesystem.h>
#include <CnDir.h>

namespace fs = std::filesystem;

#define _TMP_DIR_PARENT "."
#define _TMP_DIR_NAME   "tmpcndirtest"
#define _TMP_DIR_PATH   _TMP_DIR_PARENT "/" _TMP_DIR_NAME

#define TMP_DIR_PARENT CnSTR(_TMP_DIR_PARENT)
#define TMP_DIR_NAME   CnSTR(_TMP_DIR_NAME  )
#define TMP_DIR_PATH   CnSTR(_TMP_DIR_PATH  )

class CnDirTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a temporary configuration for testing
        fs::create_directory(TMP_DIR_PATH);
        fs::permissions(fs::path(TMP_DIR_PATH), fs::perms::owner_all, fs::perm_options::add);
    }

    void TearDown() override
    {
        // Remove the temporary configuration after testing
        fs::permissions(fs::path(TMP_DIR_PATH), fs::perms::owner_all, fs::perm_options::add);
        try
        {
            fs::remove_all(TMP_DIR_PATH);
        }
        catch (const std::exception&)
        {
            CnDir dir(TMP_DIR_PATH);
            dir.removeRecursively();
            // Optionally log or ignore the exception
        }
    }
};

TEST_F(CnDirTest, cleanPath_test)
{
    CnString origin, expected;

    origin = CnSTR(".."); expected = CnSTR("..");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("../test2/test3"); expected = CnSTR("../test2/test3");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/../test2"); expected = CnSTR("test2");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/test2/../test3"); expected = CnSTR("test/test3");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/test2/test3/.."); expected = CnSTR("test/test2");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/../test2/test3"); expected = CnSTR("test2/test3");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/..next/test2"); expected = CnSTR("test/..next/test2");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/test2\\test3\\test4\\"); expected = CnSTR("test/test2/test3/test4");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/test2/test3/../../test4"); expected = CnSTR("test/test4");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);

    origin = CnSTR("test/test2/test3/../\\../test4"); expected = CnSTR("test/test4");
    EXPECT_EQ(CnDir::cleanPath(origin), expected);
}

TEST_F(CnDirTest, DefaultConstructorTest)
{
    CnDir dir;
    EXPECT_TRUE(dir.isNull());
}

TEST_F(CnDirTest, PathConstructorTest)
{
    CnDir dir(TMP_DIR_PATH);
    EXPECT_FALSE(dir.isNull());
    EXPECT_EQ(dir.path(), TMP_DIR_PATH);

    CnString s = dir.absolutePath();
    fs::path p = fs::absolute(fs::path(TMP_DIR_PATH));
}

TEST_F(CnDirTest, CopyConstructorTest)
{
    CnDir dir1(TMP_DIR_PATH);
    CnDir dir2(dir1);

    EXPECT_FALSE(dir2.isNull());
    EXPECT_EQ(dir2.path(), dir1.path());
}
TEST_F(CnDirTest, MoveConstructorTest)
{
    CnDir dir1(TMP_DIR_PATH);
    CnDir dir2(std::move(dir1));

    EXPECT_FALSE(dir2.isNull());
    EXPECT_EQ(dir2.path(), TMP_DIR_PATH);
    EXPECT_TRUE(dir1.isNull());
}

TEST_F(CnDirTest, CopyAssignmentOperatorTest)
{
    CnDir dir1(TMP_DIR_PATH);

    CnDir dir2;
    dir2 = dir1;
    EXPECT_FALSE(dir2.isNull());
    EXPECT_EQ(dir2.path(), dir1.path());

    CnDir dir3;
    dir3 = dir1;
    EXPECT_FALSE(dir3.isNull());
    EXPECT_EQ(dir3.path(), dir1.path());
}

TEST_F(CnDirTest, MoveAssignmentOperatorTest)
{
    CnDir dir1(TMP_DIR_PATH);
    CnDir dir2;
    dir2 = std::move(dir1);

    EXPECT_FALSE(dir2.isNull());
    EXPECT_EQ(dir2.path(), TMP_DIR_PATH);
    EXPECT_TRUE(dir1.isNull());

    CnDir dir3(TMP_DIR_PATH);
    CnDir dir4(CnSTR("move/assign/test"));
    dir4 = std::move(dir3);

    EXPECT_FALSE(dir4.isNull());
    EXPECT_EQ(dir4.path(), TMP_DIR_PATH);
    EXPECT_TRUE(dir3.isNull());
}

TEST_F(CnDirTest, exists_test)
{
    CnDir dir(TMP_DIR_PATH);
    EXPECT_TRUE(dir.exists());
    EXPECT_TRUE(dir.isReadable());
    EXPECT_TRUE(dir.isEmpty());
    EXPECT_TRUE(dir.isRelative());
    EXPECT_FALSE(dir.isAbsolute());
    EXPECT_FALSE(dir.isRoot());

    CnDir dir2(dir.path()+"/nonexistent");
    EXPECT_FALSE(dir2.exists());
}

TEST_F(CnDirTest, setPath_test)
{
    CnDir dir;
    EXPECT_TRUE(dir.isNull());

    dir.setPath(TMP_DIR_PATH);
    EXPECT_FALSE(dir.isNull());
    EXPECT_EQ(dir.path(), TMP_DIR_PATH);
}

TEST_F(CnDirTest, absoluteFilePath_test)
{
    CnString cwd = CnFileInfo::normalizePathSeparators(CnStd::toString(fs::current_path()));

    CnDir dir(TMP_DIR_PATH);
    CnString filePath = dir.absoluteFilePath("testfile.txt");
    CnString expectedPath = cwd + CN_PATH_SEP + TMP_DIR_PATH + "/testfile.txt";
    EXPECT_EQ(filePath, expectedPath);
}

TEST_F(CnDirTest, relativeFilePath_test)
{
#ifdef WIN32
    CnDir dir(CnSTR("C:/Users/user/dir"));
#else
    CnDir dir(CnSTR("/home/user/dir"));
#endif
    CnString relPath1 = dir.relativeFilePath("testfile.txt");
    EXPECT_EQ(relPath1, CnSTR("testfile.txt"));

    CnString relPath2 = dir.relativeFilePath("subdir/testfile.txt");
    EXPECT_EQ(relPath2, CnSTR("subdir/testfile.txt"));

#ifdef WIN32
    CnString relPath3 = dir.relativeFilePath("C:/Users/user3/dir2/testfile.txt");
#else
    CnString relPath3 = dir.relativeFilePath("/home/user3/dir2/testfile.txt");
#endif
    EXPECT_EQ(relPath3, CnSTR("../../user3/dir2/testfile.txt"));

#ifdef WIN32
    CnString relPath4 = dir.relativeFilePath("C:/Users/user/dir/testfile.txt");
#else
    CnString relPath4 = dir.relativeFilePath("/home/user/dir/testfile.txt");
#endif
    EXPECT_EQ(relPath4, CnSTR("testfile.txt"));

#ifdef WIN32
    CnDir root(CnSTR("C:/"));   
#else
    CnDir root(CnSTR("/"));
#endif
    CnString relPath5 = root.relativeFilePath(root.path()+CnSTR("subdir/testfile.txt"));
    EXPECT_EQ(relPath5, CnSTR("subdir/testfile.txt"));
}

TEST_F(CnDirTest, canonicalPath_test)
{
    CnDir dir(TMP_DIR_PATH);
    CnString filePath = dir.canonicalPath();
    CnString expectedPath = CnFileInfo::normalizePathSeparators(CnStd::toString(fs::canonical(fs::path(TMP_DIR_PATH))));
    EXPECT_EQ(filePath, expectedPath);
}

TEST_F(CnDirTest, dirName_test)
{
    CnDir dir(TMP_DIR_PATH);
    CnString extected = TMP_DIR_NAME;
    EXPECT_EQ(dir.dirName(), extected);
}

TEST_F(CnDirTest, filePath_test)
{
    CnDir dir(TMP_DIR_PATH);
    CnString filePath = dir.filePath("testfile.txt");
    CnString expectedPath = CnString(TMP_DIR_PATH) + CnSTR("/testfile.txt");
    EXPECT_EQ(filePath, expectedPath);
}

TEST_F(CnDirTest, isAbsolute_test)
{
    CnDir dir(TMP_DIR_PATH);

    EXPECT_FALSE(dir.isAbsolute());
    EXPECT_TRUE(dir.isRelative());

    dir.makeAbsolute();
    EXPECT_TRUE(dir.isAbsolute());
    EXPECT_FALSE(dir.isRelative());
}

TEST_F(CnDirTest, isEmpty_test)
{
    CnDir dir(TMP_DIR_PATH);
    EXPECT_TRUE(dir.isEmpty());

    // Create a file in the directory
    std::ofstream(TMP_DIR_PATH "/testfile.txt").close();
    EXPECT_FALSE(dir.isEmpty());
}

TEST_F(CnDirTest, isReadable_test)
{
    CnDir dir(TMP_DIR_PATH);
    EXPECT_TRUE(dir.isReadable());

#ifdef _WIN32
    // On Windows, std::filesystem::permissions does not reliably enforce non-readable
    // directories in user mode due to NTFS ACL semantics. Skip the negative check.
    //GTEST_SKIP() << "Skipping non-readable directory check on Windows due to unreliable permission changes.";
#else
    // Change permissions to make it non-readable (POSIX)
    fs::permissions(fs::path(TMP_DIR_PATH), fs::perms::owner_read |
                                            fs::perms::group_read |
                                            fs::perms::others_read,
                                            fs::perm_options::remove);
    dir.refresh();
    EXPECT_FALSE(dir.isReadable());
#endif
}

TEST_F(CnDirTest, isRelative_test)
{
    CnDir rel(CnSTR("subdir/test"));
    EXPECT_TRUE(rel.isRelative());

    CnDir abs(CnDir::cwd() + CnSTR("/subdir/test"));
    EXPECT_FALSE(abs.isRelative());
}

TEST_F(CnDirTest, isRoot_test)
{
    CnDir dir1(CnSTR("subdir/test"));
    EXPECT_FALSE(dir1.isRoot());

#ifdef WIN32
    CnDir dir2(CnSTR("C:/"));   
#else
    CnDir dir2(CnSTR("/"));
#endif
    EXPECT_TRUE(dir2.isRoot());
}

TEST_F(CnDirTest, root_path_test)
{
#ifdef WIN32
    CnDir root(CnSTR("C:/"));   
#else
    CnDir root(CnSTR("/"));
#endif
    CnString path = root.filePath(CnSTR("subdir/testfile.txt"));
    CnString expected = root.path() + CnSTR("subdir/testfile.txt");
    EXPECT_EQ(path, expected);

    CnString absPath = root.absoluteFilePath(CnSTR("subdir/testfile.txt"));
    CnString expectedAbs = root.path() + CnSTR("subdir/testfile.txt");
    EXPECT_EQ(absPath, expectedAbs);
}

TEST_F(CnDirTest, filter_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Default filter should be retrievable (no strict expectation on default value)
    auto f0 = dir.filter();

    // Set simple file-only filter and verify round-trip
    dir.setFilter(CnDir::Files);
    EXPECT_EQ(dir.filter(), CnDir::Files);

    // Combine flags (Readable | Hidden) with Files and verify round-trip
    dir.setFilter(CnDir::Files | CnDir::Readable | CnDir::Hidden);
    EXPECT_EQ(dir.filter(), (CnDir::Files | CnDir::Readable | CnDir::Hidden));

    // Switch to directories-only and verify
    dir.setFilter(CnDir::Dirs | CnDir::NoDotAndDotDot);
    EXPECT_EQ(dir.filter(), (CnDir::Dirs | CnDir::NoDotAndDotDot));
}

TEST_F(CnDirTest, nameFilters_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Initially, name filters may be empty
    auto nf0 = dir.nameFilters();
    EXPECT_TRUE(nf0.empty());

    // Set single filter and verify round-trip
    CnStringList filters1{CnSTR("*.txt")};
    dir.setNameFilters(filters1);
    auto got1 = dir.nameFilters();
    EXPECT_EQ(got1.size(), filters1.size());
    if (got1.size() == filters1.size()) {
        auto itG = got1.begin();
        auto itF = filters1.begin();
        for (; itG != got1.end() && itF != filters1.end(); ++itG, ++itF) {
            EXPECT_EQ(*itG, *itF);
        }
    }

    // Set multiple filters and verify order is preserved
    CnStringList filters2{CnSTR("file*.txt"), CnSTR("*.md"), CnSTR("test*")};
    dir.setNameFilters(filters2);
    auto got2 = dir.nameFilters();
    EXPECT_EQ(got2.size(), filters2.size());
    if (got2.size() == filters2.size()) {
        auto itG = got2.begin();
        auto itF = filters2.begin();
        for (; itG != got2.end() && itF != filters2.end(); ++itG, ++itF) {
            EXPECT_EQ(*itG, *itF);
        }
    }
}

TEST_F(CnDirTest, sorting_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Default sorting is implementation-defined; just ensure callable
    auto s0 = dir.sorting();

    // Set sorting by name and verify
    dir.setSorting(CnDir::Name);
    EXPECT_EQ(dir.sorting(), CnDir::Name);

    // Set sorting by size and verify
    dir.setSorting(CnDir::Size);
    EXPECT_EQ(dir.sorting(), CnDir::Size);
}

TEST_F(CnDirTest, count_test)
{
    CnDir dir(TMP_DIR_PATH);
    fs::path p = TMP_DIR_PATH;

    // Create 3 files and 1 subdirectory
    std::ofstream(p / "a.txt").close();
    std::ofstream(p / "b.md").close();
    std::ofstream(p / "c.cpp").close();
    fs::create_directory(p / "subdir");

    // Count files only
    dir.setFilter(CnDir::Files);
    dir.setNameFilters({});
    EXPECT_EQ(dir.count(), 3);

    // Count directories only (excluding . and ..)
    dir.setFilter(CnDir::Dirs | CnDir::NoDotAndDotDot);
    EXPECT_EQ(dir.count(), 1);

    // Count with name filter (*.txt)
    dir.setFilter(CnDir::Files);
    dir.setNameFilters({CnSTR("*.txt")});
    EXPECT_EQ(dir.count(), 1);
}

TEST_F(CnDirTest, entryInfoList_nameFilters_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Create list of different files within TMP_DIR_PATH
    // using std::filesystem
    fs::path dirPath = TMP_DIR_PATH;

    // Create files with different extensions
    std::vector<std::string> files = {
        "file1.txt", "file23.cpp", "file24.h", "file4.txt", "file5.md", "test.py"
    };
    for (const auto& f : files) {
        std::ofstream(dirPath / f).close();
    }

    CnStringList nameFilters;

    // Set name filters to only include .txt files
    nameFilters = {CnSTR("*.txt")};
    auto entryInfos = dir.entryInfoList(nameFilters);

    std::vector<CnString> foundFiles;
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    // Should only contain file1.txt and file4.txt
    EXPECT_EQ(foundFiles.size(), 2);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("file1.txt")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("file4.txt")), foundFiles.end());

    // --------------------------------------------------
    // Set name filters to only test* files
    nameFilters = {CnSTR("test*")};
    entryInfos = dir.entryInfoList(nameFilters);

    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    // Should only contain test.py
    EXPECT_EQ(foundFiles.size(), 1);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("test.py")), foundFiles.end());

}

TEST_F(CnDirTest, entryInfoList_filters_test)
{
    auto s = fs::current_path();
    CnDir dir(TMP_DIR_PATH);

    // Create list of different files within TMP_DIR_PATH
    // using std::filesystem
    fs::path dirPath = TMP_DIR_PATH;
    fs::create_directory(dirPath / "subdir1");
    fs::create_directory(dirPath / "subdir2");

    // Create simple text file available for read/write
    std::ofstream frw(dirPath / "file1.txt");
    frw << "This is a read/write file." << std::endl;
    frw.close();
    fs::permissions(dirPath / "file1.txt", fs::perms::owner_read | fs::perms::owner_write);
    // Create read-only text file
    std::ofstream fro(dirPath / "file2.txt");
    fro << "This is a read-only file." << std::endl;
    fro.close();
    fs::permissions(dirPath / "file2.txt", fs::perms::owner_read);
    // Create hidden file
    std::ofstream fh(dirPath / ".hiddenfile");
    fh << "This is a hidden file." << std::endl;
    fh.close();
    fs::permissions(dirPath / ".hiddenfile", fs::perms::owner_read | fs::perms::owner_write);
#ifdef WIN32
    // Set hidden attribute on Windows
    SetFileAttributesW((dirPath / ".hiddenfile").wstring().c_str(), FILE_ATTRIBUTE_HIDDEN);
#endif
    // Create executable file
    std::ofstream fe(dirPath / "script.bat");
    fe << "#!/bin/bash\necho Hello, World!" << std::endl;
    fe.close();
    fs::permissions(dirPath / "script.bat", fs::perms::owner_read | fs::perms::owner_exec);
//#ifndef WIN32
//    // Create sym link (if supported)`
//    fs::create_symlink(dirPath / "file1.txt", dirPath / "symlink_to_file1");
//#endif

    // --------------------------------------------------
    auto entryInfos = dir.entryInfoList(CnDir::Dirs | CnDir::NoDotAndDotDot);
    std::vector<CnString> foundFiles;
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    // Should only contain subdir1 and subdir2
    EXPECT_EQ(foundFiles.size(), 2);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("subdir1")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("subdir2")), foundFiles.end());

    // --------------------------------------------------
    entryInfos = dir.entryInfoList(CnDir::AllDirs);
    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    // Should only contain subdir1 and subdir2
    EXPECT_EQ(foundFiles.size(), 4);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR(".")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("..")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("subdir1")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("subdir2")), foundFiles.end());

    // --------------------------------------------------
    entryInfos = dir.entryInfoList(CnDir::Files|CnDir::Readable);
    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    EXPECT_EQ(foundFiles.size(), 4);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("file1.txt")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("file2.txt")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR(".hiddenfile")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("script.bat")), foundFiles.end());

    // --------------------------------------------------
    entryInfos = dir.entryInfoList(CnDir::Files|CnDir::Writable);
    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    EXPECT_EQ(foundFiles.size(), 2);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("file1.txt")), foundFiles.end());
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR(".hiddenfile")), foundFiles.end());

    // --------------------------------------------------
    entryInfos = dir.entryInfoList(CnDir::Files|CnDir::Executable);
    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    EXPECT_EQ(foundFiles.size(), 1);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR("script.bat")), foundFiles.end());
    // --------------------------------------------------
    entryInfos = dir.entryInfoList(CnDir::Files|CnDir::Hidden);
    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    EXPECT_EQ(foundFiles.size(), 1);
    EXPECT_NE(std::find(foundFiles.begin(), foundFiles.end(), CnSTR(".hiddenfile")), foundFiles.end());
}

TEST_F(CnDirTest, entryInfoList_sort_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Create list of different files within TMP_DIR_PATH
    // using std::filesystem
    fs::path dirPath = TMP_DIR_PATH;

    // Create files with different extensions and sizes
    std::vector<std::pair<std::string, std::string>> files = {
        {"fileD.txt", "This is file D, ok."},
        {"fileB.txt", "This is file B."},
        {"fileA.txt", "This is file A, yes."},
        {"fileC.txt", "This is file C with more content."}
    };
    for (const auto& [filename, content] : files) {
        std::ofstream(dirPath / filename) << content;
    }

    // Sort by name
    auto entryInfos = dir.entryInfoList(CnDir::Files, CnDir::Name);
    std::vector<CnString> foundFiles;
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    ASSERT_EQ(foundFiles.size(), 4);
    EXPECT_EQ(foundFiles[0], CnSTR("fileA.txt"));
    EXPECT_EQ(foundFiles[1], CnSTR("fileB.txt"));
    EXPECT_EQ(foundFiles[2], CnSTR("fileC.txt"));
    EXPECT_EQ(foundFiles[3], CnSTR("fileD.txt"));

    // Sort by size
    entryInfos = dir.entryInfoList(CnDir::Files, CnDir::Size);
    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    ASSERT_EQ(foundFiles.size(), 4);
    EXPECT_EQ(foundFiles[0], CnSTR("fileB.txt")); // Smallest
    EXPECT_EQ(foundFiles[1], CnSTR("fileD.txt"));
    EXPECT_EQ(foundFiles[2], CnSTR("fileA.txt")); 
    EXPECT_EQ(foundFiles[3], CnSTR("fileC.txt")); // Largest
}

TEST_F(CnDirTest, entryInfoList_nameFilters_filters_sort_test)
{
    CnDir dir(TMP_DIR_PATH);

    fs::create_directory(fs::path(TMP_DIR_PATH) / "subdir1");
    fs::create_directory(fs::path(TMP_DIR_PATH) / "subdir2");
    fs::create_directory(fs::path(TMP_DIR_PATH) / "testdir");
    std::ofstream(fs::path(TMP_DIR_PATH) / ".hiddenfile") << "This is a hidden file.";
    std::ofstream(fs::path(TMP_DIR_PATH) / "script.sh") << "This is a script file.";
    std::ofstream(fs::path(TMP_DIR_PATH) / "file1.txt") << "This is file 1.";
    std::ofstream(fs::path(TMP_DIR_PATH) / "file2.txt") << "This is file 2.";
    std::ofstream(fs::path(TMP_DIR_PATH) / "testfile.txt") << "This is a test file.";

    auto entryInfos = dir.entryInfoList({CnSTR("file*.txt"), CnSTR("*.sh")}, CnDir::Files, CnDir::Name);
    std::vector<CnString> foundFiles;

    foundFiles.clear();
    for (const auto& info : entryInfos) {
        foundFiles.push_back(info.fileName());
    }

    ASSERT_EQ(foundFiles.size(), 3);
    EXPECT_EQ(foundFiles[0], CnSTR("file1.txt"));
    EXPECT_EQ(foundFiles[1], CnSTR("file2.txt"));
    EXPECT_EQ(foundFiles[2], CnSTR("script.sh"));

}

TEST_F(CnDirTest, entryList_nameFilters_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Create files with different names/extensions
    fs::path p = TMP_DIR_PATH;
    std::vector<std::string> files = {
        "alpha.txt", "beta.md", "gamma.txt", "delta.cpp", "testcase.py"
    };
    for (const auto& f : files) {
        std::ofstream(p / f).close();
    }

    // Filter *.txt
    auto names = dir.entryList(CnStringList{CnSTR("*.txt")});
    EXPECT_EQ(names.size(), 2);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("alpha.txt")), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("gamma.txt")), names.end());

    // Filter test* and *.md together
    names = dir.entryList(CnStringList{CnSTR("test*"), CnSTR("*.md")});
    EXPECT_EQ(names.size(), 2);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("beta.md")), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("testcase.py")), names.end());
}

TEST_F(CnDirTest, entryList_test)
{
    CnDir dir(TMP_DIR_PATH);
    fs::path p = TMP_DIR_PATH;

    // Create directories and files, including hidden and executable
    fs::create_directory(p / "sub1");
    fs::create_directory(p / "sub2");
    std::ofstream(p / "visible.txt").close();
    std::ofstream(p / ".hiddenfile").close();
    std::ofstream(p / "run.bat").close();
#ifdef WIN32
    SetFileAttributesW((p / ".hiddenfile").wstring().c_str(), FILE_ATTRIBUTE_HIDDEN);
#endif
    fs::permissions(p / "run.bat", fs::perms::owner_read | fs::perms::owner_exec);

    // List directories only (exclude . and ..)
    auto names = dir.entryList(CnDir::Dirs | CnDir::NoDotAndDotDot);
    EXPECT_EQ(names.size(), 2);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("sub1")), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("sub2")), names.end());

    // List files only
    names = dir.entryList(CnDir::Files);
    EXPECT_EQ(names.size(), 3);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("visible.txt")), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("run.bat")), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR(".hiddenfile")), names.end());

    // List hidden files only
    names = dir.entryList(CnDir::Files | CnDir::Hidden);
    EXPECT_EQ(names.size(), 1);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR(".hiddenfile")), names.end());

    // List executable files only
    names = dir.entryList(CnDir::Files | CnDir::Executable);
    EXPECT_EQ(names.size(), 1);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("run.bat")), names.end());
}

TEST_F(CnDirTest, makeAbsolute_test)
{
    CnDir dir(TMP_DIR_PATH);

    EXPECT_FALSE(dir.isAbsolute());
    EXPECT_TRUE(dir.isRelative());

    dir.makeAbsolute();
    EXPECT_TRUE(dir.isAbsolute());
    EXPECT_FALSE(dir.isRelative());
}

TEST_F(CnDirTest, cd_test)
{
    CnDir dir(TMP_DIR_PATH);

    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), TMP_DIR_NAME);

    // Change to a subdirectory (create it first)
    fs::create_directory(fs::path(TMP_DIR_PATH) / "subdir");
    bool cdResult = dir.cd("subdir");
    EXPECT_TRUE(cdResult);
    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), CnSTR("subdir"));

    // Change to a non-existent directory
    cdResult = dir.cd("nonexistent");
    EXPECT_FALSE(cdResult);
    EXPECT_TRUE(dir.exists()); // Still in "subdir"
    EXPECT_EQ(dir.dirName(), CnSTR("subdir"));
}

TEST_F(CnDirTest, cdUp_test)
{
    CnDir dir(TMP_DIR_PATH);

    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), TMP_DIR_NAME);

    // Change to a subdirectory (create it first)
    fs::create_directory(fs::path(TMP_DIR_PATH) / "subdir");
    bool cdResult = dir.cd("subdir");
    EXPECT_TRUE(cdResult);
    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), CnSTR("subdir"));

    // Change back to parent directory
    bool cdUpResult = dir.cdUp();
    EXPECT_TRUE(cdUpResult);
    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), TMP_DIR_NAME);

    // Change up from root directory (should fail)
    cdUpResult = dir.cdUp();
    EXPECT_FALSE(cdUpResult);
    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), TMP_DIR_NAME);
}

TEST_F(CnDirTest, mkdir_test)
{
    CnDir dir(TMP_DIR_PATH);

    EXPECT_TRUE(dir.exists());
    EXPECT_EQ(dir.dirName(), TMP_DIR_NAME);

    // Create a new directory
    bool mkdirResult = dir.mkdir("newdir");
    EXPECT_TRUE(mkdirResult);

    // Check if the new directory exists
    //CnDir newDir = dir;
    //EXPECT_TRUE(newDir.cd("newdir"));
    //EXPECT_TRUE(newDir.exists());
    //EXPECT_EQ(newDir.dirName(), CnSTR("newdir"));
}

TEST_F(CnDirTest, mkpath_test)
{
    CnDir dir(TMP_DIR_PATH);

    // Create nested path
    bool ok = dir.mkpath("a/b/c");
    EXPECT_TRUE(ok);

    // Verify deepest directory exists
    CnDir check(TMP_DIR_PATH);
    EXPECT_TRUE(check.cd("a"));
    EXPECT_TRUE(check.cd("b"));
    EXPECT_TRUE(check.cd("c"));
    EXPECT_TRUE(check.exists());
}

TEST_F(CnDirTest, remove_test)
{
    CnDir dir(TMP_DIR_PATH);
    // Create a file then remove it via CnDir::remove
    std::ofstream(fs::path(TMP_DIR_PATH) / "toremove.txt").close();
    EXPECT_TRUE(dir.remove("toremove.txt"));
    auto names = dir.entryList(CnDir::Files);
    EXPECT_EQ(std::find(names.begin(), names.end(), CnSTR("toremove.txt")), names.end());
}

TEST_F(CnDirTest, removeRecursively_test)
{
    CnDir dir(TMP_DIR_PATH);
    // Build nested structure with files
    fs::create_directories(fs::path(TMP_DIR_PATH) / "x/y/z");
    std::ofstream(fs::path(TMP_DIR_PATH) / "x" / "f1.txt").close();
    std::ofstream(fs::path(TMP_DIR_PATH) / "x" / "y" / "f2.txt").close();

    // Remove recursively from subdir 'x'
    CnDir sub(TMP_DIR_PATH);
    ASSERT_TRUE(sub.cd("x"));
    EXPECT_TRUE(sub.removeRecursively());

    // Ensure 'x' no longer exists
    auto names = dir.entryList(CnDir::AllDirs);
    EXPECT_EQ(std::find(names.begin(), names.end(), CnSTR("x")), names.end());
}

TEST_F(CnDirTest, rename_test)
{
    CnDir dir(TMP_DIR_PATH);
    // Create a file and rename it
    std::ofstream(fs::path(TMP_DIR_PATH) / "oldname.txt").close();
    EXPECT_TRUE(dir.rename("oldname.txt", "newname.txt"));

    auto names = dir.entryList(CnDir::Files);
    EXPECT_NE(std::find(names.begin(), names.end(), CnSTR("newname.txt")), names.end());
    EXPECT_EQ(std::find(names.begin(), names.end(), CnSTR("oldname.txt")), names.end());
}

TEST_F(CnDirTest, rmdir_test)
{
    CnDir dir(TMP_DIR_PATH);
    fs::create_directory(fs::path(TMP_DIR_PATH) / "emptydir");
    EXPECT_TRUE(dir.rmdir(CnSTR("emptydir")));

    auto names = dir.entryList(CnDir::AllDirs);
    EXPECT_EQ(std::find(names.begin(), names.end(), CnSTR("emptydir")), names.end());
}

TEST_F(CnDirTest, rmpath_test)
{
    CnDir dir(TMP_DIR_PATH);
    // Create nested then remove path tree
    ASSERT_TRUE(dir.mkpath(CnSTR("rm/a/b")));
    // Verify parent 'rm/a' is exists
    EXPECT_TRUE(dir.exists(CnSTR("rm")));
    EXPECT_TRUE(dir.exists(CnSTR("rm/a")));
    EXPECT_TRUE(dir.exists(CnSTR("rm/a/b")));

    // Now remove 'rm/a/b'
    EXPECT_TRUE(dir.rmpath(CnSTR("rm/a/b")));
    // Verify parent 'rm/a' still exists, but 'rm/a/b' is gone
    CnDir d2(TMP_DIR_PATH);
    EXPECT_FALSE(d2.exists(CnSTR("rm/a/b")));
    EXPECT_FALSE(d2.exists(CnSTR("rm/a")));
    EXPECT_FALSE(d2.exists(CnSTR("rm")));
}

TEST_F(CnDirTest, refresh_test)
{
    CnDir dir(TMP_DIR_PATH);
    // Initially empty and readable
    EXPECT_TRUE(dir.isEmpty());
    EXPECT_TRUE(dir.isReadable());

    // Create a file; refresh should pick up non-empty state
    std::ofstream(fs::path(TMP_DIR_PATH) / "created_by_refresh.txt").close();
    dir.refresh();
    EXPECT_FALSE(dir.isEmpty());

#ifndef WIN32
    // Toggle permissions to remove read; refresh should update readability
    fs::permissions(fs::path(TMP_DIR_PATH),
                    fs::perms::owner_read | fs::perms::group_read | fs::perms::others_read,
                    fs::perm_options::remove);
    dir.refresh();
    EXPECT_FALSE(dir.isReadable());
#endif
}

TEST_F(CnDirTest, swap_test)
{
    CnDir a(TMP_DIR_PATH);
    CnDir b(CnSTR("."));

    // Prepare different paths to observe swap effect
    std::ofstream(fs::path(TMP_DIR_PATH) / "only_in_a.txt").close();
    fs::path cwd = fs::current_path();
    std::ofstream(cwd / "only_in_b.txt").close();

    a.swap(b);

    auto namesA = a.entryList(CnDir::Files);
    auto namesB = b.entryList(CnDir::Files);

    EXPECT_NE(std::find(namesA.begin(), namesA.end(), CnSTR("only_in_b.txt")), namesA.end());
    EXPECT_EQ(std::find(namesA.begin(), namesA.end(), CnSTR("only_in_a.txt")), namesA.end());
    EXPECT_NE(std::find(namesB.begin(), namesB.end(), CnSTR("only_in_a.txt")), namesB.end());
    EXPECT_EQ(std::find(namesB.begin(), namesB.end(), CnSTR("only_in_b.txt")), namesB.end());

    // Cleanup created file in current directory to avoid residue
    fs::remove(cwd / "only_in_b.txt");
}
