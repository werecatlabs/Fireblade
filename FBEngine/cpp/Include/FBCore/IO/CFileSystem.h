#ifndef _FB_FileSystem_H_
#define _FB_FileSystem_H_

#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/Map.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{

    /** @class CFileSystem
     *  @brief An implementation of the IFileSystem interface to manage file and folder operations.
     */
    class CFileSystem : public IFileSystem
    {
    public:
        /** @brief Constructor. */
        CFileSystem();

        /** @brief Destructor. */
        ~CFileSystem() override;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::reload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @brief Opens a file dialog for user interaction.
         *  @return A SmartPtr to the INativeFileDialog object.
         */
        SmartPtr<INativeFileDialog> openFileDialog();

        /** @copydoc IFileSystem::open */
        SmartPtr<IStream> open( const String &filePath ) override;

        /** @copydoc IFileSystem::openW */
        SmartPtr<IStream> openW( const StringW &filePath ) override;

        /** @copydoc IFileSystem::open */
        SmartPtr<IStream> open( const String &filePath, bool input, bool binary, bool truncate,
                                bool ignorePath = false, bool ignoreCase = false ) override;

        /** @copydoc IFileSystem::addFileArchive */
        bool addFileArchive( const String &filename, bool ignoreCase = true, bool ignorePath = true,
                             ArchiveType archiveType = ArchiveType::FileSystem,
                             const String &password = StringUtil::EmptyString ) override;

        /** @copydoc IFileSystem::addFolder */
        void addFolder( const String &folderPath, bool recursive = false ) override;

        /** @copydoc IFileSystem::addFolder */
        void addFolder( SmartPtr<IFolderExplorer> folderListing );

        /** @copydoc IFileSystem::addArchive */
        void addArchive( const String &filePath ) override;

        /** @copydoc IFileSystem::addArchive */
        void addArchive( const String &filePath, const String &typeName ) override;

        /** @copydoc IFileSystem::addArchive */
        void addArchive( const String &filePath, ArchiveType type ) override;

        u32 getFileArchiveCount() const override;

        bool removeFileArchive( u32 index ) override;
        bool removeFileArchive( const String &filename ) override;
        bool moveFileArchive( u32 sourceIndex, s32 relative ) override;

        SmartPtr<IArchive> getFileArchive( u32 index ) override;

        String getAbsolutePath( const String &filename ) const override;

        String getFileDir( const String &filename ) const override;

        bool isExistingFile( const String &filePath, bool ignorePath = false,
                             bool ignoreCase = true ) const override;
        bool isExistingFile( const String &path, const String &filePath, bool ignorePath = false,
                             bool ignoreCase = true ) const;

        bool isExistingFolder( const String &path ) const override;

        bool isExistingFile( const StringW &filePath ) const override;

        bool isExistingFolder( const StringW &path ) const override;

        void createDirectories( const String &path ) override;

        void deleteFilesFromPath( const String &path ) override;
        void deleteFilesFromPathW( const StringW &path ) override;

        Array<FileInfo> getFilesWithExtension( const String &extension ) const override;
        Array<FileInfo> getFilesWithExtension( const String &path, const String &extension ) const;

        Array<String> getFileNamesWithExtension( const String &extension ) const override;
        void getFileNamesWithExtension( const String &extension,
                                        Array<String> &fileNames ) const override;

        void getFileNamesInFolder( const String &path, Array<String> &fileNames ) override;

        void getSubFolders( const String &path, Array<String> &folderNames ) override;

        bool isFolder( const String &path ) override;

        bool setWorkingDirectory( const String &directory ) override;
        String getWorkingDirectory() override;

        void addFileListener( SmartPtr<IFileListener> fileListener ) override;
        bool removeFileListener( SmartPtr<IFileListener> fileListener ) override;

        Array<u8> readAllBytes( const String &path ) override;
        String readAllText( const String &path ) override;

        void writeAllBytes( const String &path, Array<u8> bytes ) override;
        void writeAllText( const String &path, const String &contents ) override;

        String getBase64String( SmartPtr<IStream> &pStream ) override;
        String getBase64String( std::ifstream &is );

        String getBytesString( SmartPtr<IStream> &pStream ) override;
        String getBytesString( std::ifstream &is );

        void copyFolder( const String &srcPath, const String &dstPath ) override;
        void copyFile( const String &srcPath, const String &dstPath ) override;
        void deleteFile( const String &filePath ) override;

        String getFilePath( const String &path ) override;
        String getFileName( const String &path ) override;

        ArchiveType getTypeFromTypeName( const String &typeName ) const;

        String getFileHash( const String &pFilePath ) override;

        Array<String> getFolders( const String &path, bool recursive = false ) override;
        Array<StringW> getFoldersW( const StringW &path, bool recursive = false ) override;

        Array<String> getFiles( const String &path, bool partialPathMatch = false ) override;
        Array<String> getFilesAsAbsolutePaths( const String &path, bool recursive = false ) override;
        Array<String> getFileNamesWithExtension( const String &path, const String &extension ) override;

        Array<StringW> getFilesW( const StringW &path ) override;
        Array<StringW> getFilesAsAbsolutePathsW( const StringW &path, bool recursive = false ) override;
        Array<StringW> getFilesW( const StringW &path, const StringW &extension ) override;

        SmartPtr<IFolderExplorer> getFolderListing( const String &path ) override;
        SmartPtr<IFolderExplorer> getFolderListing( const String &path, const String &ext ) override;

        SmartPtr<IFolderExplorerW> getFolderListingW( const StringW &path ) override;
        SmartPtr<IFolderExplorerW> getFolderListingW( const StringW &path, const StringW &ext ) override;

        /** @copydoc IFileSystem::addFile */
        void addFile( const FileInfo &file ) override;

        /** @copydoc IFileSystem::addFiles */
        void addFiles( const Array<FileInfo> &files ) override;

        /** @copydoc IFileSystem::removeFile */
        void removeFile( const FileInfo &file ) override;

        /** @copydoc IFileSystem::removeFiles */
        void removeFiles( const Array<FileInfo> &files ) override;

        /** @copydoc IFileSystem::findFileInfo */
        bool findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath = false ) const override;

        /** @copydoc IFileSystem::findFileInfo */
        bool findFileInfo( const String &filePath, FileInfo &fileInfo,
                           bool ignorePath = false ) const override;

        /** @cooydoc IFileSystem::getSystemFiles */
        Array<FileInfo> getSystemFiles() const override;

        /** @cooydoc IFileSystem::refreshAll */
        void refreshAll( bool async ) override;

        /** @cooydoc IFileSystem::refreshPath */
        void refreshPath( const String &path, bool async ) override;

        /** @cooydoc IObject::isValid */
        bool isValid() const override;

        FB_CLASS_REGISTER_DECL;
        FB_REGISTER_FACTORY_DECL;

    private:
        /** @brief Checks if a given file is in the same directory as the provided path.
         *  @param[in] path The directory path.
         *  @param[in] file The file to check.
         *  @return A non-negative integer if the file is in the same directory, -1 otherwise.
         */
        s32 isInSameDirectory( const String &path, const String &file );

        SharedPtr<ConcurrentArray<FileInfo>> getFilesPtr() const;
        SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> getFolderArchivesPtr() const;
        void setFolderArchivesPtr( SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> p );
        SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> getFileArchivesPtr() const;

        Array<FileInfo> getFiles() const;
        void setFiles( const Array<FileInfo> &files );
        void addFilePtr( const FileInfo &file );

        void addFolderArchive( SmartPtr<IArchive> archive );
        void removeFolderArchive( SmartPtr<IArchive> archive );

        void addFileArchive( SmartPtr<IArchive> archive );
        void removeFileArchive( SmartPtr<IArchive> archive );

        /// An array of files in the file system.
        AtomicSharedPtr<ConcurrentArray<FileInfo>> m_files;

        /// An array of folder archives.
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IArchive>>> m_folderArchives;

        /// An array of file archives.
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IArchive>>> m_fileArchives;

        mutable RecursiveMutex m_mutex;
    };
}  // end namespace fb

#endif
