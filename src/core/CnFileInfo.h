/*! 
 * \file   CnFileInfo.h
 * \brief  Contains the CnFileInfo class for file information and manipulation
 * \author serhmarch
 * \date   September 2025
 */
#ifndef CNFILEINFO_H
#define CNFILEINFO_H

#include <CnTypes.h>
#include <CnTimestamp.h>

#define _CN_PATH_SEP '/'
#define CN_PATH_SEP CnCHR(_CN_PATH_SEP)
#define _CN_STR_SEP "/"
#define CN_STR_SEP CnSTR(_CN_STR_SEP)

class CnFileInfoPrivate;


/*! \brief The CnFileInfo class provides information and manipulation for files and directories.
   
    \details CnFileInfo allows querying file and directory properties, including paths, permissions, ownership, and timestamps.
    It supports both absolute and relative paths, symbolic links, shortcuts, and system files. The API is inspired by Qt's QFileInfo.
*/
class CnFileInfo
{
public:
    /// \brief Checks if the specified file exists.
    /// \param fileName The file name or path to check.
    /// \return True if the file exists, false otherwise.
    static bool exists(const CnString &fileName);

    /// \brief Checks if the specified path is absolute.
    /// \param fileName The file name or path to check.
    /// \return True if the path is absolute, false otherwise.
    static bool isAbsolutePath(const CnString &fileName);

    /// \brief Checks if the specified path is relative.
    /// \param fileName The file name or path to check.
    /// \return True if the path is relative, false otherwise.
    inline static bool isRelativePath(const CnString &fileName) { return !isAbsolutePath(fileName); }

    /// \brief Normalizes path separators to the unified '/' separator.
    /// \param path The path to normalize.
    /// \return The normalized path as a string.
    static CnString normalizePathSeparators(const CnChar *path, size_t length);

    /// \brief Normalizes path separators to the unified '/' separator.
    /// \param path The path to normalize.
    /// \return The normalized path as a string.
    inline static CnString normalizePathSeparators(const CnString &path) { return normalizePathSeparators(path.data(), path.length()); }

    /// \brief Returns the root path for the operating system.
    ///
    /// \details On Windows, returns path to root drive (e.g., `C:/`).
    /// On Unix, returns root paths as "/" string.
    static CnString rootPath();

    /// \brief Normalizes root paths for operating system conventions.
    ///
    /// \details On Windows, paths like 'C:\', 'C:', 'c:', 'c:////' are normalized to 'C:/'.
    /// On Unix, root paths starting with '/' remain unchanged.
    /// \param path The path to normalize.
    /// \return The normalized root path as a string.
    static CnString normalizeRootPath(const CnString &path);

public:
    /// \brief Base length of root paths for the operating system.
    /// For Windows, this is typically 3 (e.g., `C:/`).
    /// For Unix, this is typically 1 (e.g., `/`).
    static const size_t RootPathBaseLength;

public:
    /// \brief Constructs a CnFileInfo object for the given path.
    /// \param path The file or directory path.
    CnFileInfo(const CnString &path);

    /// \brief Copy constructor.
    /// \param other The CnFileInfo object to copy.
    CnFileInfo(const CnFileInfo &other);

    /// \brief Move constructor.
    /// \param other The CnFileInfo object to move.
    CnFileInfo(CnFileInfo &&other);

    /// \brief Default constructor. Creates a null CnFileInfo object.
    CnFileInfo();

    /// \brief Destructor.
    ~CnFileInfo();

public:
    /// \brief Move assignment operator.
    CnFileInfo &operator=(CnFileInfo &&other);

    /// \brief Copy assignment operator.
    CnFileInfo &operator=(const CnFileInfo &fileinfo);

    /// \brief Inequality comparison operator.
    bool operator!=(const CnFileInfo &fileinfo) const;

    /// \brief Equality comparison operator.
    bool operator==(const CnFileInfo &fileinfo) const;

public:
    /// \brief Returns the full file path including the file name.
    CnString filePath() const;

    /// \brief Returns the file name (without path).
    CnString fileName() const;

    /// \brief Returns the directory path containing the file.
    CnString dirPath() const;

    /// \brief Returns the directory path (alias for dirPath()).
    inline CnString path() const { return dirPath(); }

    /// \brief Returns the absolute file path including the file name.
    /// \details The absolute path consists of the full path and file name. On Unix, it begins with '/', on Windows with 'D:/'.
    /// Symbolic links or redundant '.' or '..' elements are not necessarily removed.
    CnString absoluteFilePath() const;

    /// \brief Returns the absolute path (without file name).
    CnString absolutePath() const;

    /// \brief Returns the canonical file path, resolving symbolic links and redundant elements.
    CnString canonicalFilePath() const;

    /// \brief Returns the canonical path (without file name), resolving symbolic links and redundant elements.
    CnString canonicalPath() const;

    /// \brief Returns the base name of the file (without extension).
    CnString baseName() const;

    /// \brief Returns the complete base name (may include multiple extensions).
    CnString completeBaseName() const;

    /// \brief Returns the file extension (suffix).
    CnString suffix() const;

    /// \brief Returns the complete file extension (may include multiple suffixes).
    CnString completeSuffix() const;

    /// \brief Returns the target of a symbolic link, if the file is a symlink.
    ///
    /// \details Returns the absolute path to the file or directory a symbolic link points to,
    /// or an empty string if the object isn't a symbolic link.
    /// This name may not represent an existing file; it is only a string.
    /// CnFileInfo::exists() returns true if the symlink points to an existing file.
    CnString symLinkTarget() const;

public:
    /// \brief Returns true if the object is null (not set to any file).
    bool isNull() const;

    /// \brief Returns true if the file or directory exists.
    bool exists() const;

    /// \brief Returns true if the path is absolute.
    bool isAbsolute() const;

    /// \brief Returns true if the path refers to a directory.
    bool isDir() const;

    /// \brief Returns true if the file is executable.
    bool isExecutable() const;

    /// \brief Returns true if the path refers to a regular file.
    bool isFile() const;

    /// \brief Returns true if the file or directory is hidden.
    bool isHidden() const;

    /// \brief Returns true if the file is a system file.
    bool isSystem() const;

    /// \brief Returns true if the file or directory is readable.
    bool isReadable() const;

    /// \brief Returns true if the path is relative.
    bool isRelative() const;

    /// \brief Returns true if the path refers to the root directory.
    bool isRoot() const;

    /// \brief Returns true if the file is a shortcut.
    /// Shortcuts only exist on Windows and are typically .lnk files.
    /// For instance, true will be returned for shortcuts (*.lnk files) on Windows,
    /// but false will be returned on Unix (including macOS and iOS).
    ///
    /// The shortcut (.lnk) files are treated as regular files.
    /// Opening those will open the .lnk file itself.
    /// In order to open the file a shortcut references to, it must uses symLinkTarget() on a shortcut.
    ///
    /// \note: Even if a shortcut (broken shortcut) points to a non existing file, isShortcut() returns true.
    bool isShortcut() const;

    /// \brief Returns true if the file is a symbolic link or shortcut.
    /// Symbolic links exist on Unix (including macOS and iOS) and Windows (NTFS-symlink)
    /// and are typically created by the ln -s or mklink commands, respectively.
    /// Opening a symbolic link effectively opens the link's target.
    ///
    /// In addition, true will be returned for shortcuts (*.lnk files) on Windows.
    bool isSymLink() const;

    /// \brief Returns true if the file is a symbolic link.
    /// Symbolic links exist on Unix (including macOS and iOS) and Windows (NTFS-symlink)
    /// and are typically created by the ln -s or mklink commands, respectively.
    /// Opening a symbolic link effectively opens the link's target.
    ///
    /// In contrast to isSymLink(), false will be returned for shortcuts (*.lnk files) on Windows.
    bool isSymbolicLink() const;

    /// \brief Returns true if the file or directory is writable.
    bool isWritable() const;

public:
    /// \brief Returns the file permissions as a bitmask.
    int permissions() const;

    /// \brief Returns the size of the file in bytes.
    int64_t size() const;

    /// \brief Returns the owner's user ID.
    int ownerId() const;

    /// \brief Returns the owner's name.
    CnString owner() const;

    /// \brief Returns the group's ID.
    int groupId() const;

    /// \brief Returns the group's name.
    CnString group() const;

public:
    /// \brief Returns the last access time of the file.
    CnTimestamp accessTime() const;

    /// \brief Returns the last modification time of the file.
    CnTimestamp modifiedTime() const;

    /// \brief Returns the last status change time of the file.
    CnTimestamp statusTime() const;

public:
    /// \brief Sets the file or directory for this object.
    /// \param file The file or directory path.
    void setFile(const CnString &file);

    /// \brief Refreshes the file information from the filesystem.
    void refresh();

    /// \brief Swaps the contents with another CnFileInfo object.
    /// \param other The other CnFileInfo object.
    void swap(CnFileInfo &other);

private:
    CnFileInfoPrivate *d;
};

/// \brief List of CnFileInfo objects.
typedef CnList<CnFileInfo> CnFileInfoList;

#endif // CNFILEINFO_H
