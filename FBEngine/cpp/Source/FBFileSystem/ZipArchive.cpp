#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/ZipArchive.h>
#include <FBFileSystem/ZipFile.h>
#include <FBFileSystem/CMemoryFile.h>
#include <FBFileSystem/CFileSystem.h>
#include <FBFileSystem/CFileList.h>
#include <FBCore/FBCore.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <zzip/zzip.h>
#include <zzip/plugin.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ZipArchive, SharedObject<IArchive> );

    ZipArchive::ZipArchive( const String &name, bool ignoreCase, bool ignorePaths ) : m_path( name )
    {
    }

    ZipArchive::~ZipArchive()
    {
        unload( nullptr );
    }

    void ZipArchive::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        Array<FileInfo> fileInfoList;
        fileInfoList.reserve( 32 );

        static zzip_strings_t zipFileExt[] = { ".zip", nullptr };

        auto name = getPath();

        m_fileList = factoryManager->make_ptr<CFileList>();
        m_fileList->setPath( name );

        if( !mZzipDir )
        {
            zzip_error_t zzipError;
            mZzipDir = zzip_dir_open( name.c_str(), &zzipError );
            if( zzipError != ZZIP_NO_ERROR )
            {
                FB_LOG_ERROR( "Error opening archive: " + name );
            }

            // Cache names
            ZZIP_DIRENT zzipEntry;
            while( zzip_dir_read( mZzipDir, &zzipEntry ) )
            {
                FileInfo fileInfo;
                // info.archive = this;

                // Get basename / path
                String fileName;
                String path;

                StringUtil::splitFilename( zzipEntry.d_name, fileName, path );
                path = StringUtil::cleanupPath( path );

#ifdef _DEBUG
                if( std::string( zzipEntry.d_name ).find( ".program" ) != std::string::npos )
                {
                    int stop = 0;
                    stop = 0;
                }

                if( fileName.find( "PagedGeometry.program" ) != std::string::npos )
                {
                    int stop = 0;
                    stop = 0;
                }
#endif

                String filePath;
                if( !StringUtil::isNullOrEmpty( path ) )
                {
                    filePath = path + "/" + fileName;
                }
                else
                {
                    filePath = fileName;
                }

                filePath = StringUtil::cleanupPath( filePath );

                fileInfo.fileName = fileName;
                fileInfo.fileNameLowerCase = StringUtil::make_lower( fileInfo.fileName );

                fileInfo.filePath = filePath;
                fileInfo.filePathLowerCase = StringUtil::make_lower( fileInfo.filePath );

                fileInfo.path = path;

                fileInfo.absolutePath = filePath;

                // Get sizes
                fileInfo.compressedSize = static_cast<u32>( zzipEntry.d_csize );
                fileInfo.uncompressedSize = static_cast<u32>( zzipEntry.st_size );

                fileInfo.isDirectory = fileName.empty();

                fileInfo.fileId = StringUtil::getHash64( filePath );

                // folder entries
                if( fileInfo.fileName.empty() )
                {
                    fileInfo.fileName = fileInfo.fileName.substr( 0, fileInfo.fileName.length() - 1 );
                    //StringUtil::splitFilename(fileInfo.filename, fileInfo.basename, fileInfo.path);
                    // Set compressed size to -1 for folders; anyway nobody will check
                    // the compressed size of a folder, and if he does, its useless anyway
                    fileInfo.compressedSize = u32( -1 );
                }

                fileInfoList.push_back( fileInfo );
            }

            m_fileList->setFiles( fileInfoList );
        }

        setLoadingState( LoadingState::Loaded );
    }

    void ZipArchive::reload( SmartPtr<ISharedObject> data )
    {
    }

    void ZipArchive::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        if( mZzipDir )
        {
            zzip_dir_close( mZzipDir );
            mZzipDir = nullptr;
        }

        setLoadingState( LoadingState::Unloaded );
    }

    u8 ZipArchive::getType() const
    {
        return static_cast<u8>( IFileSystem::ArchiveType::Zip );
    }

    String ZipArchive::getPassword() const
    {
        return m_password;
    }

    void ZipArchive::setPassword( const String &password )
    {
        m_password = password;
    }

    SmartPtr<IStream> ZipArchive::open( const String &filename, bool input, bool binary, bool truncate,
                                        bool ignorePath )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FileInfo fileInfo;
        m_fileList->findFileInfo( filename, fileInfo, ignorePath );

        //auto fileIndex = m_fileList->findFile( filename, false );
        //if( fileIndex == -1 )
        //{
        //    return nullptr;
        //}

        auto &filePath = fileInfo.filePath;

        ZZIP_STAT zstat;

        // Format not used here (always binary)
        auto zzipFile = zzip_file_open( mZzipDir, filename.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS );
        if( !zzipFile )
        {
            zzipFile = zzip_file_open( mZzipDir, filePath.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS );
            zzip_dir_stat( mZzipDir, filePath.c_str(), &zstat, ZZIP_CASEINSENSITIVE );
        }
        else
        {
            zzip_dir_stat( mZzipDir, filename.c_str(), &zstat, ZZIP_CASEINSENSITIVE );
        }

        if( !zzipFile )
        {
            auto zerr = zzip_error( mZzipDir );
            auto zzDesc = getZzipErrorDescription( zerr );
            FB_LOG_MESSAGE( "Archive", String( " - Unable to open file " ) + filename + ", error was '" +
                                           zzDesc + "'" );
            return nullptr;
        }

        auto size = static_cast<size_t>( zstat.st_size );
        FB_ASSERT( size < 1e8 );

        auto zipFile = factoryManager->make_ptr<ZipFile>();
        zipFile->setArchive( this );
        zipFile->setFileName( filename );
        zipFile->setZipFile( zzipFile );
        zipFile->setSize( size );
        zipFile->load( nullptr );
        return zipFile;
    }

    bool ZipArchive::exists( const String &filename, bool ignorePath, bool ignoreCase ) const
    {
        return m_fileList->exists( filename, ignorePath, ignoreCase );
    }

    bool ZipArchive::isReadOnly() const
    {
        return false;
    }

    String ZipArchive::getPath() const
    {
        return String( m_path.c_str() );
    }

    void ZipArchive::setPath( const String &path )
    {
        m_path = path;
    }

    bool ZipArchive::getIgnorePaths() const
    {
        return m_ignorePaths;
    }

    void ZipArchive::setIgnorePaths( bool ignorePaths )
    {
        m_ignorePaths = ignorePaths;
    }

    bool ZipArchive::getIgnoreCase() const
    {
        return m_ignoreCase;
    }

    void ZipArchive::setIgnoreCase( bool ignoreCase )
    {
        m_ignoreCase = ignoreCase;
    }

    bool ZipArchive::findFileInfo( const String &filePath, FileInfo &fileInfo,
                                   bool ignorePath /*= false */ ) const
    {
        return m_fileList->findFileInfo( filePath, fileInfo, ignorePath );
    }

    bool ZipArchive::findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath /*= false */ ) const
    {
        return m_fileList->findFileInfo( id, fileInfo, ignorePath );
    }

    SmartPtr<IFileList> ZipArchive::getFileList() const
    {
        return m_fileList;
    }

    Array<FileInfo> ZipArchive::getFiles() const
    {
        return m_fileList->getFiles();
    }

    /// Utility method to format out zzip errors
    String ZipArchive::getZzipErrorDescription( s32 zzipError )
    {
        String errorMsg;
        switch( zzipError )
        {
        case ZZIP_NO_ERROR:
            break;
        case ZZIP_OUTOFMEM:
            errorMsg = "Out of memory.";
            break;
        case ZZIP_DIR_OPEN:
        case ZZIP_DIR_STAT:
        case ZZIP_DIR_SEEK:
        case ZZIP_DIR_READ:
            errorMsg = "Unable to read zip file.";
            break;
        case ZZIP_UNSUPP_COMPR:
            errorMsg = "Unsupported compression format.";
            break;
        case ZZIP_CORRUPTED:
            errorMsg = "Corrupted archive.";
            break;
        default:
            errorMsg = "Unknown error.";
            break;
        }

        return errorMsg;
    }
}  // end namespace fb
