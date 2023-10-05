#ifndef CFileList_h__
#define CFileList_h__

#include <FBCore/Interface/IO/IFileList.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{
    /** Implementation of a file list */
    class CFileList : public IFileList
    {
    public:
        /** Default constructor. */ 
        CFileList() = default;

        //! Constructor
        /** @param path The path of this file archive */
        CFileList( const String &path, bool ignoreCase, bool ignorePaths );

        //! Destructor
        ~CFileList() override;

        /** @copydoc IFileList::sort */
        void sort() override;

        /** @copydoc IFileList::getNumFiles */
        u32 getNumFiles() const override;

        /** @copydoc IFileList::getFileName */
        String getFileName( u32 index ) const override;

        /** @copydoc IFileList::getFullFileName */
        String getFullFileName( u32 index ) const override;

        /** @copydoc IFileList::getFileSize */
        u32 getFileSize( u32 index ) const override;

        /** @copydoc IFileList::getFileOffset */
        u32 getFileOffset( u32 index ) const override;

        /** @copydoc IFileList::findFile */
        s32 findFile( const String &filename, bool isFolder ) const override;

        /** @copydoc IFileList::setPath */
        void setPath( const String &path ) override;

        /** @copydoc IFileList::getPath */
        String getPath() const override;

        /** @copydoc IFileList::getFiles */
        Array<FileInfo> getFiles() const;

        /** @copydoc IFileList::setFiles */
        void setFiles( Array<FileInfo> files );

        /** @copydoc IFileList::getFilesPtr */
        SharedPtr<ConcurrentArray<FileInfo>> getFilesPtr() const;

        /** @copydoc IFileList::setFilesPtr */
        void setFilesPtr( SharedPtr<ConcurrentArray<FileInfo>> p );

        /** @copydoc IFileList::findFileInfo */
        bool findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath = false ) const;

        /** @copydoc IFileList::findFileInfo */
        bool findFileInfo( const String &filePath, FileInfo &fileInfo, bool ignorePath = false ) const;

        /** @copydoc IFileList::exists */
        bool exists( const String &filePath, bool ignorePath = false,
                     bool ignoreCase = false ) const override;

        /** @copydoc IFileList::removeFilePtr */
        void addFilePtr( const FileInfo &file );

        /** @copydoc IFileList::removeFilePtr */
        void removeFilePtr( const FileInfo &file );

        FB_CLASS_REGISTER_DECL;

    protected:
        /// An array of files in the file system.
        AtomicSharedPtr<ConcurrentArray<FileInfo>> m_files;

        //! Path to the file list
        String m_path;
    };
}  // end namespace fb

#endif
