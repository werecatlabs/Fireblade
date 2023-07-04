#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/ObfuscatedZipArchive.h>
#include <FBFileSystem/ObfuscatedZipFile.h>
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
    FB_CLASS_REGISTER_DERIVED( fb, ObfuscatedZipArchive, SharedObject<IArchive> );

    const int XOR_VALUE = 13;

    // Change this magic number to a value of your choosing.
    static int xor_value = 0;
    static zzip_plugin_io_handlers xor_handlers = {};
    // Change "OBFUSZIP" to match the file extension of your choosing.
    static zzip_strings_t xor_fileext[] = { ".zip", nullptr };

    _zzip_plugin_io ObfuscatedZip_PluginIo;

    std::map<s32, SmartPtr<IStream>> gObfuscatedZipFiles;

    // Static method that un-obfuscates an obfuscated file.
    static zzip_ssize_t xor_read( int fd, void *buf, zzip_size_t len )
    {
#if defined FB_PLATFORM_WIN32
        auto bytes = read( fd, buf, static_cast<u32>( len ) );
#else
        auto bytes = read( fd, buf, len );
#endif

        auto pch = static_cast<char *>( buf );
        for( size_t i = 0; i < bytes; ++i )
        {
            pch[i] ^= XOR_VALUE;
        }

        return bytes;
    }

    ObfuscatedZipArchive::ObfuscatedZipArchive( const String &name, bool ignoreCase, bool ignorePaths ) :
        mZzipDir( nullptr ),
        m_path( name )
    {
    }

    ObfuscatedZipArchive::~ObfuscatedZipArchive()
    {
        unload( nullptr );
    }

    void ObfuscatedZipArchive::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );

        if( !xor_value )
        {
            xor_value = 13;
        }

        zzip_init_io( &xor_handlers, 0 );
        xor_handlers.fd.read = &xor_read;

        auto name = getPath();

        m_fileList = fb::make_ptr<CFileList>( name, false, false );

        if( !mZzipDir )
        {
            zzip_error_t zzipError;
            // mZzipDir = zzip_dir_open(name.c_str(), &zzipError);
            mZzipDir = zzip_dir_open_ext_io( name.c_str(), &zzipError, xor_fileext, &xor_handlers );
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

                if( std::string( zzipEntry.d_name ).find( ".fbmeshbin" ) != std::string::npos )
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

                fileInfo.filePath = zzipEntry.d_name;
                fileInfo.filePathLowerCase = StringUtil::make_lower( fileInfo.filePath );

                fileInfo.path = filePath;

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

                m_fileInfoList.push_back( fileInfo );
            }
        }

        m_fileList->setFiles( m_fileInfoList );

        setLoadingState( LoadingState::Loaded );
    }

    void ObfuscatedZipArchive::reload( SmartPtr<ISharedObject> data )
    {
    }

    void ObfuscatedZipArchive::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        if( mZzipDir )
        {
            zzip_dir_close( mZzipDir );
            mZzipDir = nullptr;
        }

        setLoadingState( LoadingState::Unloaded );
    }

    u8 ObfuscatedZipArchive::getType() const
    {
        return static_cast<u8>( IFileSystem::ArchiveType::ObfuscatedZip );
    }

    String ObfuscatedZipArchive::getPassword() const
    {
        return m_password;
    }

    void ObfuscatedZipArchive::setPassword( const String &password )
    {
        m_password = password;
    }

    SmartPtr<IStream> ObfuscatedZipArchive::open( const String &filename, bool input, bool binary,
                                                  bool truncate, bool ignorePath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );  

        auto filePath = filename;
        if( ignorePath )
        {
            filePath = Path::getFileName( filePath );
        }

        // Format not used here (always binary)
        auto zzipFile = zzip_file_open( mZzipDir, filePath.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS );

        if( !zzipFile )
        {
            auto zerr = zzip_error( mZzipDir );
            auto zzDesc = getZzipErrorDescription( zerr );
            FB_LOG_MESSAGE( "Archive", String( " - Unable to open file " ) + filename + ", error was '" +
                                           zzDesc + "'" );
            return nullptr;
        }

        // Get uncompressed size too
        ZZIP_STAT zstat;
        zzip_dir_stat( mZzipDir, filePath.c_str(), &zstat, ZZIP_CASEINSENSITIVE );

        auto size = static_cast<size_t>( zstat.st_size );
        return fb::make_ptr<ObfuscatedZipFile>( this, filePath, zzipFile, static_cast<u32>( size ) );
    }

    bool ObfuscatedZipArchive::exists( const String &filename, bool ignorePath, bool ignoreCase ) const
    {
        return m_fileList->exists( filename, ignorePath, ignoreCase );
    }

    bool ObfuscatedZipArchive::isReadOnly() const
    {
        return false;
    }

    String ObfuscatedZipArchive::getPath() const
    {
        return String( m_path.c_str() );
    }

    void ObfuscatedZipArchive::setPath( const String &path )
    {
        m_path = path;
    }

    bool ObfuscatedZipArchive::getIgnorePaths() const
    {
        return m_ignorePaths;
    }

    void ObfuscatedZipArchive::setIgnorePaths( bool ignorePaths )
    {
        m_ignorePaths = ignorePaths;
    }

    bool ObfuscatedZipArchive::getIgnoreCase() const
    {
        return m_ignoreCase;
    }

    void ObfuscatedZipArchive::setIgnoreCase( bool ignoreCase )
    {
        m_ignoreCase = ignoreCase;
    }

    bool ObfuscatedZipArchive::findFileInfo( const String &filePath, FileInfo &fileInfo,
                                             bool ignorePath /*= false */ ) const
    {
        return m_fileList->findFileInfo( filePath, fileInfo, ignorePath );
    }

    bool ObfuscatedZipArchive::findFileInfo( hash64 id, FileInfo &fileInfo,
                                             bool ignorePath /*= false */ ) const
    {
        return m_fileList->findFileInfo( id, fileInfo, ignorePath );
    }

    SmartPtr<IFileList> ObfuscatedZipArchive::getFileList() const
    {
        return m_fileList;
    }

    Array<FileInfo> ObfuscatedZipArchive::getFiles() const
    {
        return m_fileList->getFiles();
    }

    /// Utility method to format out zzip errors
    String ObfuscatedZipArchive::getZzipErrorDescription( s32 zzipError )
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
