#ifndef IFileList_h__
#define IFileList_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/FileInfo.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    /** Provides a list of files and folders. */
    class IFileList : public ISharedObject
    {
    public:
        ~IFileList() override = default;

        /**
         * @brief Returns the number of files and directories in the file list.
         *
         * @return The number of files and directories in the file list.
         */
        virtual u32 getNumFiles() const = 0;

        /**
         * @brief Returns the name of a file in the list, based on an index.
         *
         * The String is not included in this name. Use getFullFileName for this.
         *
         * @param index The zero-based index of the file which name should be returned.
         *              The index must be less than the amount getFileCount() returns.
         * @return The file name of the file. Returns 0, if an error occurred.
         */
        virtual String getFileName( u32 index ) const = 0;

        /**
         * @brief Returns the full name of a file in the list, including the String, based on an index.
         *
         * @param index The zero-based index of the file which name should be returned.
         *              The index must be less than the amount getFileCount() returns.
         * @return The file name of the file. Returns 0 if an error occurred.
         */
        virtual String getFullFileName( u32 index ) const = 0;

        /**
         * @brief Returns the size of a file in the file list, based on an index.
         *
         * @param index The zero-based index of the file which should be returned.
         *              The index must be less than the amount getFileCount() returns.
         * @return The size of the file in bytes.
         */
        virtual u32 getFileSize( u32 index ) const = 0;

        /**
         * @brief Returns the file offset of a file in the file list, based on an index.
         *
         * @param index The zero-based index of the file which should be returned.
         *              The index must be less than the amount getFileCount() returns.
         * @return The offset of the file in bytes.
         */
        virtual u32 getFileOffset( u32 index ) const = 0;

        /**
         * @brief Searches for a file or folder in the list.
         *
         * Searches for a file by name.
         *
         * @param filename The name of the file to search for.
         * @param isFolder True if you are searching for a directory String, false if you are searching for a file.
         * @return Returns the index of the file in the file list, or -1 if no matching name was found.
         */
        virtual s32 findFile( const String &filename, bool isFolder = false ) const = 0;

        /**
         * @brief Sets the path for the file list.
         *
         * @param path The path to set.
         */
        virtual void setPath( const String &path ) = 0;

        /**
         * @brief Returns the path for the file list.
         *
         * @return The path for the file list.
         */
        virtual String getPath() const = 0;

        /**
         * @brief Sorts the file list. You should call this after adding any items to the file list.
         */
        virtual void sort() = 0;

        /**
         * @brief Returns an array of FileInfo objects for all files in the list.
         *
         * @return An array of FileInfo objects for all files in the list.
         */
        virtual Array<FileInfo> getFiles() const = 0;

        /**
         * Sets the list of files in this file list.
         *
         * @param files The array of `FileInfo` objects representing the files to set.
         */
        virtual void setFiles( Array<FileInfo> files ) = 0;

        /**
         * Gets the shared pointer to the array of `FileInfo` objects representing the files in this file list.
         *
         * @return The shared pointer to the array of `FileInfo` objects.
         */
        virtual SharedPtr<ConcurrentArray<FileInfo>> getFilesPtr() const = 0;

        /**
         * Sets the shared pointer to the array of `FileInfo` objects representing the files in this file list.
         *
         * @param p The shared pointer to the array of `FileInfo` objects to set.
         */
        virtual void setFilesPtr( SharedPtr<ConcurrentArray<FileInfo>> p ) = 0;

        /**
         * Adds a `FileInfo` object to the array of files in this file list.
         *
         * @param file The `FileInfo` object to add.
         */
        virtual void addFilePtr( const FileInfo &file ) = 0;

        /**
         * Removes a `FileInfo` object from the array of files in this file list.
         *
         * @param file The `FileInfo` object to remove.
         */
        virtual void removeFilePtr( const FileInfo &file ) = 0;

        /**
         * Checks whether a file with the specified file path exists in this file list.
         *
         * @param filePath The file path of the file to check.
         * @param ignorePath Whether to ignore the path when checking for the file.
         * @param ignoreCase Whether to ignore the case when checking for the file.
         * @return `true` if the file exists in the file list, `false` otherwise.
         */
        virtual bool exists( const String &filePath, bool ignorePath = false,
                             bool ignoreCase = false ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IFileList_h__
