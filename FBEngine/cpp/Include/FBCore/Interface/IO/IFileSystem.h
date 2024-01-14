#ifndef __IFileSystem__
#define __IFileSystem__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{

    /**
     * @brief Interface for a file system.
     *
     * This class defines an interface for a file system, providing methods for opening, adding, and removing files and archives,
     * querying file and directory information, and more.
     */
    class IFileSystem : public ISharedObject
    {
    public:
        /**
         * @brief The type of archive.
         */
        enum class ArchiveType
        {
            Folder,         ///< A folder archive.
            FileSystem,     ///< A file system archive.
            Zip,            ///< A ZIP archive.
            ObfuscatedZip,  ///< An obfuscated ZIP archive.
            Unknown,        ///< An unknown archive type.

            Count
        };

        /**
         * @brief Destructor.
         */
        ~IFileSystem() override = default;

        /** Create native file dialog. */
        virtual SmartPtr<INativeFileDialog> openFileDialog() = 0;

        /**
         * @brief Opens a file for access. Searches the resource paths.
         *
         * @param filePath The path of the file to open.
         * @return A pointer to the opened file stream.
         */
        virtual SmartPtr<IStream> open( const String &filePath ) = 0;

        /**
         * @brief Opens a file for access. Searches the resource paths.
         *
         * @param filePath The path of the file to open.
         * @return A pointer to the opened file stream.
         */
        virtual SmartPtr<IStream> openW( const StringW &filePath ) = 0;

        /**
         * @brief Opens a file for access.
         *
         * @param filePath The path of the file to open.
         * @param input Whether the file is opened for input or output.
         * @param binary Whether the file is opened in binary mode.
         * @param truncate Whether the file is truncated when opened.
         * @param ignorePath Whether the path is ignored when searching for the file.
         * @param ignoreCase Whether the file extension is case-sensitive.
         * @return A pointer to the opened file stream.
         */
        virtual SmartPtr<IStream> open( const String &filePath, bool input, bool binary, bool truncate,
                                        bool ignorePath = false, bool ignoreCase = false ) = 0;

        /**
         * @brief Adds an archive to the file system.
         *
         * @param filename The filename of the archive to add.
         * @param ignoreCase Whether the archive is case-sensitive.
         * @param ignorePaths Whether paths are ignored when searching for files.
         * @param archiveType The type of the archive.
         * @param password The password for the archive, if any.
         * @return Whether the archive was added successfully.
         */
        virtual bool addFileArchive( const String &filename, bool ignoreCase = true,
                                     bool ignorePaths = true,
                                     ArchiveType archiveType = ArchiveType::FileSystem,
                                     const String &password = StringUtil::EmptyString ) = 0;

        /**
         * Adds a folder to the file system.
         *
         * @param folderPath The path of the folder to add.
         * @param recursive Whether to search subfolders recursively. Defaults to false.
         */
        virtual void addFolder( const String &folderPath, bool recursive = false ) = 0;

        /**
         * Adds an archive file to the file system.
         *
         * @param filePath The path of the archive file to add.
         */
        virtual void addArchive( const String &filePath ) = 0;

        /**
         * Adds an archive file to the file system with a specified type name.
         *
         * @param filePath The path of the archive file to add.
         * @param typeName The name of the archive type.
         */
        virtual void addArchive( const String &filePath, const String &typeName ) = 0;

        /**
         * Adds an archive file to the file system with a specified type.
         *
         * @param filePath The path of the archive file to add.
         * @param type The archive type.
         */
        virtual void addArchive( const String &filePath, ArchiveType type ) = 0;

        /**
         * Gets the number of archives currently attached to the file system.
         *
         * @return The number of archives.
         */
        virtual u32 getFileArchiveCount() const = 0;

        /**
         * Removes an archive file from the file system.
         *
         * This will close the archive and free any file handles, but will not close resources which
         * have already been loaded and are now cached, for example textures and meshes.
         *
         * @param index The index of the archive to remove.
         * @return True on success, false on failure.
         */
        virtual bool removeFileArchive( u32 index ) = 0;

        /**
         * Removes an archive file from the file system.
         *
         * This will close the archive and free any file handles, but will not close resources which
         * have already been loaded and are now cached, for example textures and meshes.
         *
         * @param filename The path of the archive file to remove.
         * @return True on success, false on failure.
         */
        virtual bool removeFileArchive( const String &filename ) = 0;

        /**
         * Changes the search order of attached archives.
         * \param sourceIndex The index of the archive to change
         * \param relative The relative change in position, archives with a lower index are searched first
         * \return True if successful, otherwise false
         */
        virtual bool moveFileArchive( u32 sourceIndex, s32 relative ) = 0;

        /**
         * Get the archive at a given index.
         * \param index The index of the archive to get
         * \return A smart pointer to the archive
         */
        virtual SmartPtr<IArchive> getFileArchive( u32 index ) = 0;

        /**
         * Get the current working directory.
         * \return Current working directory as a string
         */
        virtual String getWorkingDirectory() = 0;

        /**
         * Changes the current working directory.
         * \param directory A string specifying the new working directory. The string is operating system dependent.
         * \return True if successful, otherwise false
         */
        virtual bool setWorkingDirectory( const String &directory ) = 0;

        /**
         * Converts a relative string to an absolute (unique) string, resolving symbolic links if required.
         * \param filename Possibly relative file or directory name to query.
         * \return Absolute filename which points to the same file
         */
        virtual String getAbsolutePath( const String &filename ) const = 0;

        /**
         * Determines if a file exists and could be opened.
         * \param filePath The string identifying the file which should be tested for existence
         * \param ignorePath If true, only the file name part of the path is checked
         * \param ignoreCase If true, the case of the file name is ignored
         * \return True if the file exists and can be opened, false otherwise
         */
        virtual bool isExistingFile( const String &filePath, bool ignorePath = false,
                                     bool ignoreCase = true ) const = 0;

        /**
         * Determines if a file exists and could be opened.
         * \param path The file path
         * \param filePath The string identifying the file which should be tested for existence
         * \param ignorePath If true, only the file name part of the path is checked
         * \param ignoreCase If true, the case of the file name is ignored
         * \return True if the file exists and can be opened, false otherwise
         */
        virtual bool isExistingFile( const String &path, const String &filePath, bool ignorePath = false,
                                     bool ignoreCase = true ) const = 0;

        /**
         * Determines if a folder exists.
         * \param path The path of the folder to check
         * \return True if the folder exists, false otherwise
         */
        virtual bool isExistingFolder( const String &path ) const = 0;

        /**
         * Determines if a file exists and could be opened.
         *
         * \param filePath The string identifying the file which should be tested for existence.
         * \return `true` if the file exists, and `false` if it does not exist or an error occurred.
         */
        virtual bool isExistingFile( const StringW &filePath ) const = 0;

        /**
         * Determines if a folder exists.
         *
         * \param path The path identifying the folder to test.
         * \return `true` if the folder exists, and `false` if it does not exist or an error occurred.
         */
        virtual bool isExistingFolder( const StringW &path ) const = 0;

        /**
         * Gets the directory path of a given file path.
         *
         * \param filename The path of the file.
         * \return The directory path of the file.
         */
        virtual String getFileDir( const String &filename ) const = 0;

        /**
         * Creates all directories in the specified path.
         *
         * \param path The path of the directory.
         */
        virtual void createDirectories( const String &path ) = 0;

        /**
         * Deletes all files in the specified directory path.
         *
         * \param path The path of the directory.
         */
        virtual void deleteFilesFromPath( const String &path ) = 0;

        /**
         * Deletes all files in the specified directory path.
         *
         * \param path The path of the directory.
         */
        virtual void deleteFilesFromPathW( const StringW &path ) = 0;

        /**
         * Gets all files in the archives with a specified extension.
         *
         * \param extension The extension of the files to get.
         * \return An array containing information about the files with the specified extension.
         */
        virtual Array<FileInfo> getFilesWithExtension( const String &extension ) const = 0;

        /**
         * Gets all files in the archives with a specified extension.
         *
         * \param extension The extension of the files to get.
         * \return An array containing information about the files with the specified extension.
         */
        virtual Array<FileInfo> getFilesWithExtension( const String &path,
                                                       const String &extension ) const = 0;

        /**
         * Gets the file names with a specified extension.
         *
         * \param extension The extension of the files to get.
         * \return An array containing the names of the files with the specified extension.
         */
        virtual Array<String> getFileNamesWithExtension( const String &extension ) const = 0;

        /**
         * Gets the file names with a specified extension.
         *
         * \param extension The extension of the files to get.
         * \param fileNames An array that will be filled with the names of the files with the specified extension.
         */
        virtual void getFileNamesWithExtension( const String &extension,
                                                Array<String> &fileNames ) const = 0;

        /**
         * Gets the file names in a specified folder.
         *
         * \param path The path of the folder.
         * \param fileNames An array that will be filled with the names of the files in the specified folder.
         */
        virtual void getFileNamesInFolder( const String &path, Array<String> &fileNames ) = 0;

        /**
         * Gets the names of all subfolders in the specified folder.
         *
         * \param path The path of the folder.
         * \param folderNames An array that will be filled with the names of the subfolders in the specified folder.
         */
        virtual void getSubFolders( const String &path, Array<String> &folderNames ) = 0;

        /** Determines if the specified path is a folder.
            @param path: The path to query.
            @return True if the path exists and is a folder, false otherwise.
        */
        virtual bool isFolder( const String &path ) = 0;

        /** Adds a file listener to the file system.
            @param fileListener: A pointer to the file listener to add.
        */
        virtual void addFileListener( SmartPtr<IFileListener> fileListener ) = 0;

        /** Removes a file listener from the file system.
            @param fileListener: A pointer to the file listener to remove.
            @return True if the listener was successfully removed, false otherwise.
        */
        virtual bool removeFileListener( SmartPtr<IFileListener> fileListener ) = 0;

        /** Reads all bytes from a file.
            @param path: The path to the file to read.
            @return An array of bytes read from the file.
        */
        virtual Array<u8> readAllBytes( const String &path ) = 0;

        /** Reads all text from a file.
            @param path: The path to the file to read.
            @return A string containing the text read from the file.
        */
        virtual String readAllText( const String &path ) = 0;

        /** Writes all bytes to a file. */
        virtual void writeAllBytes( const String &path, u8 *bytes, u32 size ) = 0;

        /** Writes all bytes to a file.
            @param path: The path to the file to write.
            @param bytes: The array of bytes to write to the file.
        */
        virtual void writeAllBytes( const String &path, Array<u8> bytes ) = 0;

        /** Writes all text to a file.
            @param path: The path to the file to write.
            @param contents: The string of text to write to the file.
        */
        virtual void writeAllText( const String &path, const String &contents ) = 0;

        /** Gets a base64-encoded string representation of the contents of a stream.
            @param pStream: A pointer to the stream to get the contents of.
            @return A base64-encoded string representation of the contents of the stream.
        */
        virtual String getBase64String( SmartPtr<IStream> &pStream ) = 0;

        /** Gets a bytes string representation of the contents of a stream.
            @param pStream: A pointer to the stream to get the contents of.
            @return A bytes string representation of the contents of the stream.
        */
        virtual String getBytesString( SmartPtr<IStream> &pStream ) = 0;

        virtual void copyFolder( const String &srcPath, const String &dstPath ) = 0;

        /** Copies a file from one path to another.
            @param srcPath: The source path of the file to copy.
            @param dstPath: The destination path to copy the file to.
        */
        virtual void copyFile( const String &srcPath, const String &dstPath ) = 0;

        /** Deletes a file.
            @param filePath: The path of the file to delete.
        */
        virtual void deleteFile( const String &filePath ) = 0;

        /** Gets the path of a file.
            @param path: The path to a file or directory.
            @return The path of the file.
        */
        virtual String getFilePath( const String &path ) = 0;

        /** Gets the name of a file from a path.
            @param path: The path to the file.
            @return The name of the file.
        */
        virtual String getFileName( const String &path ) = 0;

        /**
         * Gets the MD5 hash of a file.
         *
         * @param pFilePath The path to the file to hash.
         * @return The MD5 hash of the file as a hexadecimal string.
         */
        virtual String getFileHash( const String &pFilePath ) = 0;

        /**
         * Gets the names of all folders within the specified path.
         *
         * @param path The path to the parent directory.
         * @param recursive Whether or not to recursively search subdirectories for folders.
         * @return An array of folder names.
         */
        virtual Array<String> getFolders( const String &path, bool recursive = false ) = 0;

        /**
         * Gets the names of all folders within the specified path.
         *
         * @param path The path to the parent directory.
         * @param recursive Whether or not to recursively search subdirectories for folders.
         * @return An array of folder names as wide strings.
         */
        virtual Array<StringW> getFoldersW( const StringW &path, bool recursive = false ) = 0;

        /**
         * Gets the names of all files within the specified path.
         *
         * @param path The path to the directory containing the files.
         * @return An array of file names.
         */
        virtual Array<String> getFiles( const String &path, bool partialPathMatch = false ) = 0;

        /**
         * Gets the absolute paths of all files within the specified path.
         *
         * @param path The path to the directory containing the files.
         * @param recursive Whether or not to recursively search subdirectories for files.
         * @return An array of file names with their absolute paths.
         */
        virtual Array<String> getFilesAsAbsolutePaths( const String &path, bool recursive = false ) = 0;

        /**
         * Gets the names of all files within the specified path with the given extension.
         *
         * @param path The path to the directory containing the files.
         * @param extension The file extension to filter by.
         * @return An array of file names.
         */
        virtual Array<String> getFileNamesWithExtension( const String &path,
                                                         const String &extension ) = 0;

        /**
         * Gets the names of all files within the specified path.
         *
         * @param path The path to the directory containing the files.
         * @return An array of file names as wide strings.
         */
        virtual Array<StringW> getFilesW( const StringW &path ) = 0;

        /**
         * Gets the absolute paths of all files within the specified path.
         *
         * @param path The path to the directory containing the files.
         * @param recursive Whether or not to recursively search subdirectories for files.
         * @return An array of file names with their absolute paths as wide strings.
         */
        virtual Array<StringW> getFilesAsAbsolutePathsW( const StringW &path,
                                                         bool recursive = false ) = 0;

        /**
         * Gets the names of all files within the specified path with the given extension.
         *
         * @param path The path to the directory containing the files.
         * @param extension The file extension to filter by as a wide string.
         * @return An array of file names as wide strings.
         */
        virtual Array<StringW> getFilesW( const StringW &path, const StringW &extension ) = 0;

        /**
         * \brief Gets a folder listing for the specified path.
         *
         * \param path The path to get the folder listing for.
         * \return A smart pointer to an `IFolderListing` object representing the folder listing.
         */
        virtual SmartPtr<IFolderExplorer> getFolderListing( const String &path ) = 0;

        /**
         * \brief Gets a folder listing for the specified path and extension.
         *
         * \param path The path to get the folder listing for.
         * \param ext The extension to filter the results by.
         * \return A smart pointer to an `IFolderListing` object representing the folder listing.
         */
        virtual SmartPtr<IFolderExplorer> getFolderListing( const String &path, const String &ext ) = 0;

        /**
         * \brief Gets a wide-character folder listing for the specified path.
         *
         * \param path The path to get the folder listing for.
         * \return A smart pointer to an `IFolderListingW` object representing the folder listing.
         */
        virtual SmartPtr<IFolderExplorerW> getFolderListingW( const StringW &path ) = 0;

        /**
         * \brief Gets a wide-character folder listing for the specified path and extension.
         *
         * \param path The path to get the folder listing for.
         * \param ext The extension to filter the results by.
         * \return A smart pointer to an `IFolderListingW` object representing the folder listing.
         */
        virtual SmartPtr<IFolderExplorerW> getFolderListingW( const StringW &path,
                                                              const StringW &ext ) = 0;

        /**
         * \brief Adds a file to the file system.
         *
         * \param file The `FileInfo` object representing the file to add.
         */
        virtual void addFile( const FileInfo &file ) = 0;

        /**
         * \brief Adds multiple files to the file system.
         *
         * \param files An `Array` of `FileInfo` objects representing the files to add.
         */
        virtual void addFiles( const Array<FileInfo> &files ) = 0;

        /**
         * \brief Removes a file from the file system.
         *
         * \param file The `FileInfo` object representing the file to remove.
         */
        virtual void removeFile( const FileInfo &file ) = 0;

        /**
         * \brief Removes multiple files from the file system.
         *
         * \param files An `Array` of `FileInfo` objects representing the files to remove.
         */
        virtual void removeFiles( const Array<FileInfo> &files ) = 0;

        /**
         * \brief Finds the `FileInfo` object for the specified file ID.
         *
         * \param id The file ID to search for.
         * \param fileInfo A reference to a `FileInfo` object to store the results in.
         * \param ignorePath If `true`, the search will ignore the file path.
         * \return `true` if a `FileInfo` object was found for the specified file ID, `false` otherwise.
         */
        virtual bool findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath = false ) const = 0;

        /**
         * \brief Finds the `FileInfo` object for the file at the specified path.
         *
         * \param filePath The path of the file to search for.
         * \param fileInfo A reference to a `FileInfo` object to store the results in.
         * \param ignorePath If `true`, the search will ignore the file path.
         * \return `true` if a `FileInfo` object was found for the specified file path, `false` otherwise.
         */
        virtual bool findFileInfo( const String &filePath, FileInfo &fileInfo,
                                   bool ignorePath = false ) const = 0;

        /**
        Returns a list of system files.
        @return A list of `FileInfo` structures containing information about the system files.
        */
        virtual Array<FileInfo> getSystemFiles() const = 0;

        /**
        Updates the list of files stored.

        @param async Specifies if the operation is asynchronous.
        If it's synchronous it will run on the caller thread.
        */
        virtual void refreshAll( bool async ) = 0;

        /**
        Updates the list of files stored for the specified path.

        @param path The path of the folder to update.
        @param async Specifies if the operation is asynchronous.
        If it's synchronous it will run on the caller thread.
        */
        virtual void refreshPath( const String &path, bool async ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif
