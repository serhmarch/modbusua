/*!
 * \file   CnFile.h
 * \brief  Клас для роботи із файлом
 * 
 * \author serhmarch
 * \date   September 2025
*/
#ifndef CNFILE_H
#define CNFILE_H

#include <CnString.h>
#include <CnFileInfo.h>

class CnFilePrivate;

/// \brief The CnFile class provides file operations and manipulation.
///
/// \details CnFile allows opening, reading, writing, and closing files, as well as querying file properties and handling errors.
/// It supports various file modes, permissions, and can work with both text and binary files. The API is inspired by Qt's QFile.
class CnFile
{
public:
    enum Error
    {
        NoError             = 0	 , ///< No error occurred.
        ReadError	        = 1	 , ///< An error occurred when reading from the file.
        WriteError	        = 2	 , ///< An error occurred when writing to the file.
        FatalError	        = 3	 , ///< A fatal error occurred.
        ResourceError	    = 4	 , ///< Out of resources (e.g., too many open files, out of memory, etc.)
        OpenError	        = 5	 , ///< The file could not be opened.
        AbortError	        = 6	 , ///< The operation was aborted.
        TimeOutError	    = 7	 , ///< A timeout occurred.
        UnspecifiedError	= 8	 , ///< An unspecified error occurred.
        RemoveError	        = 9	 , ///< The file could not be removed.
        RenameError	        = 10 , ///< The file could not be renamed.
        PositionError	    = 11 , ///< The position in the file could not be changed.
        ResizeError	        = 12 , ///< The file could not be resized.
        PermissionsError	= 13 , ///< The file could not be accessed.
        CopyError	        = 14   ///< The file could not be copied.
    };

public:
    /// \brief Checks if the specified file exists.
    /// \param fileName The file name or path to check.
    /// \return True if the file exists, false otherwise.
    static bool exists(const CnString &fileName);

public:
    /// \brief Constructs a CnFile object for the given file name.
    /// \param fileName The file name or path.
    CnFile(const CnString &fileName);

    /// \brief Copy constructor.
    /// \param other The CnFile object to copy.
    CnFile(const CnFile &other);

    /// \brief Move constructor.
    /// \param other The CnFile object to move.
    CnFile(CnFile &&other);

    /// \brief Default constructor. Creates a null CnFile object.
    CnFile();

    /// \brief Destructor.
    ~CnFile();

public:
    /// \brief Move assignment operator.
    CnFile &operator=(CnFile &&other);

    /// \brief Copy assignment operator.
    CnFile &operator=(const CnFile &other);

    /// \brief Inequality comparison operator.
    bool operator!=(const CnFile &other) const;

    /// \brief Equality comparison operator.
    bool operator==(const CnFile &other) const;

    /// \brief Writes the string data to the file.
    /// \param data The string to write.
    CnFile& operator<<(const CnString &str);

    /// \brief Writes a single character to the file.
    /// \param ch The character to write.
    CnFile& operator<<(CnChar ch);


public:
    /// \brief Sets the file name for this object.
    /// \param fileName The file name or path.
    void setFileName(const CnString &fileName);

    /// \brief Returns the file name (without path).
    CnString fileName() const;

    /// \brief Returns the full file path including the file name.
    CnString filePath() const;

    /// \brief Returns the directory path containing the file.
    CnString dirPath() const;

    /// \brief Returns the directory path (alias for dirPath()).
    inline CnString path() const { return dirPath(); }

public:
    /// \brief Returns true if the object is null (not set to any file).
    bool isNull() const;

    /// \brief Returns true if the file or directory exists.
    bool exists() const;

    /// \brief Opens the file with the specified mode.
    /// \param mode The open mode (read, write, etc.). This is a set of flags from Cn::OpenMode.
    /// \return True if the file was opened successfully, false otherwise.
    bool open(int mode);

    /// \brief Closes the file.
    void close();

    /// \brief Returns true if the file is open.
    bool isOpen() const;

    /// \brief Returns the file information object.
    CnFileInfo info() const;

    /// \brief Returns true if the file is readable.
    bool isReadable() const;

    /// \brief Returns true if the file is writable.
    bool isWritable() const;

    /// \brief Returns true if the file is executable.
    bool isExecutable() const;

    /// \brief Returns true if the file is hidden.
    bool isHidden() const;

    /// \brief Returns true if the file is a system file.
    bool isSystem() const;

    /// \brief Returns true if the path refers to a regular file.
    bool isFile() const;

    /// \brief Returns true if the path refers to a directory.
    bool isDir() const;

    /// \brief Returns true if the file is a symbolic link.
    bool isSymLink() const;

    /// \brief Returns true if the file is a shortcut.
    bool isShortcut() const;

    /// \brief Returns true if the path is absolute.
    bool isAbsolute() const;

    /// \brief Returns true if the path is relative.
    bool isRelative() const;

    /// \brief Returns true if the path refers to the root directory.
    bool isRoot() const;

    /// \brief Returns the target path of the symbolic link.
    /// \return The target path if the file is a symbolic link, empty string otherwise.
    CnString symLinkTarget() const;

public:
    /// \brief Returns the size of the file in bytes.
    int64_t size() const;

    /// \brief Returns the current position in the file.
    int64_t pos() const;

    /// \brief Seeks to the specified position in the file.
    /// \param pos The position to seek to.
    /// \return True if successful, false otherwise.
    bool seek(int64_t pos);

    /// \brief Returns true if the end of the file has been reached.
    bool atEnd() const;

    /// \brief Flushes the file's buffers.
    bool flush();

    /// \brief Resizes the file to the specified size.
    /// \param size The new size of the file in bytes.
    /// \return True if successful, false otherwise.
    bool resize(int64_t size);

    /// \brief Removes the file from the filesystem.
    /// \return True if successful, false otherwise.
    bool remove();

    /// \brief Renames the file to the specified new name.
    /// \param newName The new file name or path.
    /// \return True if successful, false otherwise.
    bool rename(const CnString &newName);

    /// \brief Copies the file to the specified new name.
    /// \param newName The destination file name or path.
    /// \return True if successful, false otherwise.
    bool copy(const CnString &newName);

    /// \brief Creates a symbolic link to the file at the specified link name.
    /// \param linkName The name or path of the symbolic link to create. Link can be relative of the current file dir.
    /// \return True if successful, false otherwise.
    /// \note To create a valid link on Windows, linkName must have a .lnk file extension.
    bool link(const CnString &linkName);

public:
    /// \brief Reads up to maxSize bytes from the file into data.
    /// \param data The buffer to read into.
    /// \param maxSize The maximum number of bytes to read.
    /// \return The number of bytes read.
    int64_t read(char *data, int64_t maxSize);

    /// \brief Reads a line from the file into data, up to maxSize bytes.
    /// \param data The buffer to read the line into.
    /// \param maxSize The maximum number of bytes to read.
    /// \return The number of bytes read.
    int64_t readLine(char *data, int64_t maxSize);

    /// \brief Writes size bytes from data to the file.
    /// \param data The buffer to write from.
    /// \param size The number of bytes to write.
    /// \return The number of bytes written.
    int64_t write(const char *data, int64_t size = -1);

    /// \brief Reads a single character from the file.
    /// \return The character read, or -1 on error or end of file.
    int get();

    /// \brief Reads a line from the file.
    /// \param line The line read from the file.
    /// \return True if successful, false otherwise.
    bool getline(CnString &line);

    /// \brief Peeks at the next maxSize bytes from the file without advancing the file position.
    /// \param data The buffer to peek into.
    /// \param maxSize The maximum number of bytes to peek.
    /// \return The number of bytes peeked.
    int64_t peek(char *data, int64_t maxSize);

public:
    /// \brief Returns the file permissions as a bitmask.
    int permissions() const;

    /// \brief Sets the file permissions.
    /// \param permissions The permissions to set.
    bool setPermissions(int permissions);

public:
    /// \brief Returns the last error code.
    Error lastError() const;

    /// \brief Returns the last error string.
    CnString errorString() const;

    /// \brief Clears the last error.
    void clearError();

public:
    /// \brief Refreshes the file information from the filesystem.
    void refresh();

    /// \brief Swaps the contents with another CnFile object.
    /// \param other The other CnFile object.
    void swap(CnFile &other);

private:
    CnFilePrivate *d;
};

#endif // CNFILE_H