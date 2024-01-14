#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/FileList.h>
#include <FBCore/IO/FileSystem.h>
#include <FBCore/IO/MemoryFile.h>
#include <FBCore/IO/ObfuscatedZipArchive.h>
#include <FBCore/IO/ObfuscatedZipFile.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Path.h>
#include <cstdio>
#include <cstdlib>
#include <zzip/plugin.h>
#include <zzip/zzip.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ObfuscatedZipArchive, IArchive );

    const int XOR_VALUE = 13;

    // Change this magic number to a value of your choosing.
    static int xor_value = 0;
    static zzip_plugin_io_handlers xor_handlers = {};
    // Change "OBFUSZIP" to match the file extension of your choosing.
    static zzip_strings_t xor_fileext[] = { ".zip", nullptr };

    _zzip_plugin_io ObfuscatedZip_PluginIo;

    std::map<s32, SmartPtr<IStream>> gObfuscatedZipFiles;

    // Static method that un-obfuscates an obfuscated file.
    static auto xor_read( int fd, void *buf, zzip_size_t len ) -> zzip_ssize_t
    {
#if FB_COMPILER == FB_COMPILER_MSVC
        auto bytes = _read( fd, buf, static_cast<u32>( len ) );
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

        m_fileList = fb::make_ptr<FileList>( name, false, false );

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
                    fileInfo.compressedSize = static_cast<u32>( -1 );
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

    auto ObfuscatedZipArchive::getType() const -> u8
    {
        return static_cast<u8>( IFileSystem::ArchiveType::ObfuscatedZip );
    }

    auto ObfuscatedZipArchive::getPassword() const -> String
    {
        return m_password;
    }

    void ObfuscatedZipArchive::setPassword( const String &password )
    {
        m_password = password;
    }

    auto ObfuscatedZipArchive::open( const String &filename, bool input, bool binary, bool truncate,
                                     bool ignorePath ) -> SmartPtr<IStream>
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

    auto ObfuscatedZipArchive::exists( const String &filename, bool ignorePath, bool ignoreCase ) const
        -> bool
    {
        return m_fileList->exists( filename, ignorePath, ignoreCase );
    }

    auto ObfuscatedZipArchive::isReadOnly() const -> bool
    {
        return false;
    }

    auto ObfuscatedZipArchive::getPath() const -> String
    {
        return String( m_path.c_str() );
    }

    void ObfuscatedZipArchive::setPath( const String &path )
    {
        m_path = path;
    }

    auto ObfuscatedZipArchive::getIgnorePaths() const -> bool
    {
        return m_ignorePaths;
    }

    void ObfuscatedZipArchive::setIgnorePaths( bool ignorePaths )
    {
        m_ignorePaths = ignorePaths;
    }

    auto ObfuscatedZipArchive::getIgnoreCase() const -> bool
    {
        return m_ignoreCase;
    }

    void ObfuscatedZipArchive::setIgnoreCase( bool ignoreCase )
    {
        m_ignoreCase = ignoreCase;
    }

    auto ObfuscatedZipArchive::findFileInfo( const String &filePath, FileInfo &fileInfo,
                                             bool ignorePath /*= false */ ) const -> bool
    {
        return m_fileList->findFileInfo( filePath, fileInfo, ignorePath );
    }

    auto ObfuscatedZipArchive::findFileInfo( hash64 id, FileInfo &fileInfo,
                                             bool ignorePath /*= false */ ) const -> bool
    {
        return m_fileList->findFileInfo( id, fileInfo, ignorePath );
    }

    auto ObfuscatedZipArchive::getFileList() const -> SmartPtr<IFileList>
    {
        return m_fileList;
    }

    auto ObfuscatedZipArchive::getFiles() const -> Array<FileInfo>
    {
        return m_fileList->getFiles();
    }

    /// Utility method to format out zzip errors
    auto ObfuscatedZipArchive::getZzipErrorDescription( s32 zzipError ) -> String
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
