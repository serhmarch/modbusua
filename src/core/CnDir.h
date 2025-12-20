/*!
 * \file   CnDir.h
 * \brief  Contains the CnDir class for directory operations and manipulation
 *
 * \author serhmarch
 * \date   September 2025
*/
#ifndef CNDIR_H
#define CNDIR_H

#include <CnFileInfo.h>

class CnDirPrivate;

/* \brief The CnDir class provides directory operations and manipulation.
  
   \details The `CnDir` class provides system-independent directory operations and manipulation, similar to Qt's `QDir`.
   It allows creating, removing, listing, and querying directories and their contents, as well as handling paths and permissions.
   The class supports recursive operations, filtering, and can work with both absolute and relative paths.
 */
class CnDir
{
public:
    static const CnChar PathSep; ///< Native path separator character ('/' or '\', depending on platform)

public:
    enum Filters
    {
        NoFilter        = 0x0000                , ///< No filtering.
        Dirs            = 0x0001                , ///< List directories that match the filters.
        AllDirs         = 0x0400                , ///< List all directories; i.e. don't apply the filters to directory names.
        Files           = 0x0002                , ///< List files.
        Drives          = 0x0004                , ///< List disk drives (ignored under Unix).
        NoSymLinks      = 0x0008                , ///< Do not list symbolic links (ignored by operating systems that don't support symbolic links).
        NoDot           = 0x2000                , ///< Do not list the special entry ".".
        NoDotDot        = 0x4000                , ///< Do not list the special entry "..".
        NoDotAndDotDot  = NoDot | NoDotDot      , ///< Do not list the special entries "." and "..".
        AllEntries      = Dirs | Files | Drives , ///< List directories, files, drives and symlinks (this does not list broken symlinks unless you specify System).
        Readable        = 0x0010                , ///< List files for which the application has read access. The Readable value needs to be combined with Dirs or Files.
        Writable        = 0x0020                , ///< List files for which the application has write access. The Writable value needs to be combined with Dirs or Files.
        Executable      = 0x0040                , ///< List files for which the application has execute access. The Executable value needs to be combined with Dirs or Files.
        Modified        = 0x0080                , ///< Only list files that have been modified (ignored on Unix).
        Hidden          = 0x0100                , ///< List hidden files (on Unix, files starting with a ".").
        System          = 0x0200                , ///< List system files (on Unix, FIFOs, sockets and device files are included; on Windows, .lnk files are included)
        CaseSensitive   = 0x0800                  ///< Make name filter matching case sensitive (default is case insensitive).
    };

    enum SortFlags
    {
        NoSort     = 0x0000, ///< No sorting applied
        Name       = 0x0001, ///< Sort by name
        Time       = 0x0002, ///< Sort by modification time
        Size       = 0x0004, ///< Sort by file size
        Unsorted   = 0x0008, ///< Entries are not sorted
        DirsFirst  = 0x0010, ///< List directories before files
        DirsLast   = 0x0020, ///< List directories after files
        Reversed   = 0x0040, ///< Reverse the sort order
        IgnoreCase = 0x0080  ///< Ignore case when sorting
    };

public:
    /// \brief Returns the current working directory.
    /// \return The current working directory as a string.
    static CnString cwd();

    /// \brief Returns the current working directory path (alias for cwd()).
    inline static CnString currentPath() { return cwd(); }

    /// \brief Returns the current working directory path (alias for cwd()).
    inline static CnString workingDirPath() { return cwd(); }

    /// \brief Cleans the given path, removing redundant separators and resolving relative elements.
    /// \param path The path to clean.
    /// \return The cleaned path as a string.
    static CnString cleanPath(const CnString &path);

    /// \brief Returns the platform-specific directory separator character.
    /// \return The separator character ('/' or '\').
    static CnChar separator();

    /// \brief Checks if the file name matches any of the specified name filters.
    /// \param fileName The file name to check.
    /// \param nameFilters The list of name filters to match against.
    /// \param caseSensitive Whether the match should be case sensitive (default: true).
    /// \return True if the file name matches any filter, false otherwise.
    static bool matchNameFilters(const CnString &fileName, const CnStringList &nameFilters, bool caseSensitive = true);

public:
    /// \brief Constructs a CnDir object for the given path, name filters, entry filters, and sort order.
    /// \param path The directory path.
    CnDir(const CnString &path, const CnStringList &nameFilters, int filters = AllEntries, int sort = NoSort);

    /// \brief Constructs a CnDir object for the given path.
    /// \param path The directory path.
    CnDir(const CnString &path);

    /// \brief Copy constructor.
    /// \param other The CnDir object to copy.
    CnDir(const CnDir &other);

    /// \brief Move constructor.
    /// \param other The CnDir object to move.
    CnDir(CnDir &&other);

    /// \brief Default constructor. Creates a null CnDir object.
    CnDir();

    /// \brief Destructor.
    ~CnDir();

public:
    /// \brief Copy assignment operator.
    CnDir &operator=(const CnDir &other);

    /// \brief Move assignment operator.
    CnDir &operator=(CnDir &&other);

    /// \brief Equality comparison operator.
    bool operator==(const CnDir &other) const;

    /// \brief Inequality comparison operator.
    bool operator!=(const CnDir &other) const;

public:
    /// \brief Returns true if the CnDir object is null (no path set).
    bool isNull() const;

    /// \brief Returns true if the current directory exists.
    bool exists() const;

    /// \brief Returns true if the current directory exists.
    bool exists(const CnString &path) const;

    /// \brief Returns the directory path.
    CnString path() const;

    /// \brief Sets the directory path for this object.
    /// \param path The directory path.
    void setPath(const CnString &path);

    /// \brief Returns the absolute file path for the given file name.
    /// \param fileName The file name to resolve.
    /// \return The absolute file path as a string.
    CnString absoluteFilePath(const CnString &fileName) const;

    /// \brief Returns the relative file path for the given file name.
    /// \param fileName The file name to resolve.
    /// \return The relative file path as a string.
    CnString relativeFilePath(const CnString &fileName) const;

    /// \brief Returns the absolute path of the directory.
    CnString absolutePath() const;

    /// \brief Returns the canonical path of the directory, resolving symbolic links and redundant elements.
    CnString canonicalPath() const;

    /// \brief Returns the name of the directory.
    CnString dirName() const;

    /// \brief Returns the file path for the given file name, relative to the directory.
    /// \param fileName The file name to resolve.
    /// \return The file path as a string.
    CnString filePath(const CnString &fileName) const;

    /// \brief Returns true if the directory path is absolute.
    bool isAbsolute() const;

    /// \brief Returns true if the directory is empty, according to the specified filters.
    /// \param filters The entry filters to apply.
    bool isEmpty(int filters = (AllEntries | NoDotAndDotDot)) const;

    /// \brief Returns true if the directory is readable.
    bool isReadable() const;

    /// \brief Returns true if the directory path is relative.
    bool isRelative() const;

    /// \brief Returns true if the directory is the root directory.
    bool isRoot() const;

    /// \brief Returns the current entry filter flags.
    int filter() const;

    /// \brief Sets the entry filter flags.
    /// \param filters The filter flags to set.
    void setFilter(int filters);

    /// \brief Returns the current name filters.
    CnStringList nameFilters() const;

    /// \brief Sets the name filters for entries.
    /// \param nameFilters The list of name filters to set.
    void setNameFilters(const CnStringList &nameFilters);

    /// \brief Returns the current sorting flags.
    int sorting() const;

    /// \brief Sets the sorting flags.
    /// \param sort The sorting flags to set.
    void setSorting(int sort);

    /// \brief Returns the number of entries in the directory.
    size_t count() const;

    /// \brief Returns a list of file information entries matching the specified name filters, filters, and sort order.
    /// \param nameFilters The list of name filters to apply.
    /// \param filters The entry filter flags.
    /// \param sort The sorting flags.
    /// \return List of CnFileInfo objects.
    CnFileInfoList entryInfoList(const CnStringList &nameFilters, int filters = AllEntries, int sort = NoSort) const;

    /// \brief Returns a list of file information entries matching the specified filters and sort order.
    /// \param filters The entry filter flags.
    /// \param sort The sorting flags.
    /// \return List of CnFileInfo objects.
    CnFileInfoList entryInfoList(int filters = AllEntries, int sort = NoSort) const;

    /// \brief Returns a list of entry names matching the specified name filters, filters, and sort order.
    /// \param nameFilters The list of name filters to apply.
    /// \param filters The entry filter flags.
    /// \param sort The sorting flags.
    /// \return List of entry names as CnStringList.
    CnStringList entryList(const CnStringList &nameFilters, int filters = AllEntries, int sort = NoSort) const;

    /// \brief Returns a list of entry names matching the specified filters and sort order.
    /// \param filters The entry filter flags.
    /// \param sort The sorting flags.
    /// \return List of entry names as CnStringList.
    CnStringList entryList(int filters = AllEntries, int sort = NoSort) const;

public: // dir operations
    /// \brief Converts the directory path to an absolute path.
    /// \return True if successful, false otherwise.
    bool makeAbsolute();

    /// \brief Changes the current directory to the specified subdirectory.
    /// \param dirName The name of the subdirectory to change to.
    /// \return True if successful, false otherwise.
    bool cd(const CnString &dirName);

    /// \brief Changes the current directory to its parent directory.
    /// \return True if successful, false otherwise.
    bool cdUp();

    /// \brief Creates a new subdirectory with the specified name and mode.
    /// \param dirName The name of the subdirectory to create.
    /// \param mode The permissions mode to set (default: Cn::Permission_Empty).
    /// \return True if successful, false otherwise.
    bool mkdir(const CnString &dirName, int mode = Cn::Permission_Empty) const;

    /// \brief Creates the specified directory path, including intermediate directories, with the given mode.
    /// \param dirPath The directory path to create.
    /// \param mode The permissions mode to set (default: Cn::Permission_Empty).
    /// \return True if successful, false otherwise.
    bool mkpath(const CnString &dirPath, int mode = Cn::Permission_Empty) const;

    /// \brief Removes the specified file or subdirectory from the directory.
    /// \param fileName The name of the file or subdirectory to remove.
    /// \return True if successful, false otherwise.
    bool remove(const CnString &fileName);

    /// \brief Removes the directory and all its contents recursively.
    /// \return True if successful, false otherwise.
    bool removeRecursively();

    /// \brief Renames a file or subdirectory from oldName to newName.
    /// \param oldName The current name of the file or subdirectory.
    /// \param newName The new name to assign.
    /// \return True if successful, false otherwise.
    bool rename(const CnString &oldName, const CnString &newName);

    /// \brief Removes the specified subdirectory from the directory.
    /// \param dirName The name of the subdirectory to remove.
    /// \return True if successful, false otherwise.
    bool rmdir(const CnString &dirName) const;

    /// \brief Removes the specified directory path and all its contents.
    /// \param dirPath The directory path to remove.
    /// \return True if successful, false otherwise.
    bool rmpath(const CnString &dirPath) const;

    /// \brief Refreshes the directory information from the filesystem.
    void refresh();

    /// \brief Swaps the contents with another CnDir object.
    /// This is a fast operation that does not involve copying data.
    /// \param other The other CnDir object to swap with.
    void swap(CnDir &other);

private:
    CnFileInfoList innerEntryInfoList(const CnStringList &nameFilters, int filters, int sort) const;

private:
    CnDirPrivate *d;
};

#endif // CNDIR_H
