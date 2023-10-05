#ifndef ZipArchive_h__
#define ZipArchive_h__

#include <FBCore/Interface/IO/IArchive.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{

    class ZipArchive : public IArchive
    {
    public:
        ZipArchive() = default;
        ZipArchive( const String &name, bool ignoreCase, bool ignorePaths );
        ~ZipArchive() override;

        /** @copydoc IObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        SmartPtr<IFileList> getFileList() const override;

        /** @copydoc IArchive::getFiles */
        Array<FileInfo> getFiles() const override;

        u8 getType() const override;

        String getPassword() const override;
        void setPassword( const String &password ) override;

        SmartPtr<IStream> open( const String &filename, bool input = true, bool binary = false,
                                bool truncate = false, bool ignorePath = false ) override;

        bool exists( const String &filename, bool ignorePath = false,
                     bool ignoreCase = false ) const override;

        bool isReadOnly() const override;

        String getPath() const override;

        void setPath( const String &path ) override;

        bool getIgnorePaths() const;

        void setIgnorePaths( bool ignorePaths );

        bool getIgnoreCase() const;

        void setIgnoreCase( bool ignoreCase );

        /** @copydoc IArchive::findFileInfo */
        bool findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath = false ) const;

        /** @copydoc IArchive::findFileInfo */
        bool findFileInfo( const String &filePath, FileInfo &fileInfo, bool ignorePath = false ) const;

        FB_CLASS_REGISTER_DECL;

    protected:
        String getZzipErrorDescription( s32 zzipError );

        /// Handle to root zip file
        ZZIP_DIR *mZzipDir = nullptr;

        /// The file list.
        SmartPtr<CFileList> m_fileList;

        /// A password.
        String m_password;

        FixedString<128> m_path;

        bool m_ignoreCase = false;
        bool m_ignorePaths = false;
    };
}  // end namespace fb

#endif  // ZipArchive_h__
