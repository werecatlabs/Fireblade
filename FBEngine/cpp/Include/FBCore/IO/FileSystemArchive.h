#ifndef CFileSystemArchive_h__
#define CFileSystemArchive_h__

#include <FBCore/Interface/IO/IArchive.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/IO/FileList.h>

namespace fb
{
    /** A file archive */
    class FileSystemArchive : public IArchive
    {
    public:
        /** Constructor */
        FileSystemArchive() = default;

        /** Constructor */
        FileSystemArchive( const String &path, bool ignoreCase, bool ignorePaths );

        /** Destructor */
        ~FileSystemArchive() override;

        /** @copydoc IObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IArchive::getFileList */
        SmartPtr<IFileList> getFileList() const override;

        void setFileList( SmartPtr<IFileList> fileList );

        /** @copydoc IArchive::getFiles */
        Array<FileInfo> getFiles() const override;

        /** @copydoc IArchive::getType */
        u8 getType() const override;

        /** @copydoc IArchive::getPassword */
        String getPassword() const override;

        /** @copydoc IArchive::setPassword */
        void setPassword( const String &val ) override;

        /** @copydoc IArchive::open */
        SmartPtr<IStream> open( const String &filePath, bool input = true, bool binary = false,
                                bool truncate = false, bool ignorePath = false ) override;

        /** @copydoc IArchive::exists */
        bool exists( const String &filePath, bool ignorePath = false,
                     bool ignoreCase = false ) const override;

        /** @copydoc IArchive::isReadOnly */
        bool isReadOnly() const override;

        /** @copydoc IArchive::getPath */
        String getPath() const override;

        /** @copydoc IArchive::setPath */
        void setPath( const String &path ) override;

        /** @copydoc IArchive::findFileInfo */
        bool findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath = false ) const;

        /** @copydoc IArchive::findFileInfo */
        bool findFileInfo( const String &filePath, FileInfo &fileInfo, bool ignorePath = false ) const;

        FB_CLASS_REGISTER_DECL;

    private:
        SharedPtr<ConcurrentArray<FileInfo>> getFilesPtr() const;
        void addFilePtr( const FileInfo &file );

        /// The password.
        String m_password;

        /// The file system path.
        String m_path;

        /// The file list.
        SmartPtr<FileList> m_fileList;
    };
}  // end namespace fb

#endif  // CFileSystemArchive_h__
