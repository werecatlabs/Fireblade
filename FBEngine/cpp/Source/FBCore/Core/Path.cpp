#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Core/LogManager.h>
#include <fstream>
#include <algorithm>  // for std::for_each
#include <cassert>    // for assert
#include <cstddef>    // for std::size_t
#include <iostream>   // for std::cout
#include <ostream>    // for std::endl

#if defined FB_PLATFORM_WIN32
#    include <direct.h>  // for _chdir
#    include <io.h>      // for _access
#    include <windows.h>
#elif defined FB_PLATFORM_APPLE
#    include <FBCore/Base/OSX/macUtils.h>
#endif

#if FB_USE_BOOST
#    include <boost/filesystem/operations.hpp>
#    include <boost/crc.hpp>      // for boost::crc_basic, boost::crc_optimal
#    include <boost/cstdint.hpp>  // for boost::uint16_t
#    include <boost/filesystem/operations.hpp>
#    include <boost/range/iterator_range.hpp>
#    include <boost/range/algorithm.hpp>
#    include <boost/range/iterator.hpp>
#    include <boost/uuid/detail/md5.hpp>
#    include <boost/algorithm/hex.hpp>
#endif

#if FB_USE_OPENSSL
#    include <openssl/md5.h>
#endif

#if defined FB_PLATFORM_WIN32
#    include <filesystem>
#elif defined FB_PLATFORM_APPLE
#    include <filesystem>
#elif defined FB_PLATFORM_LINUX
#    include <experimental/filesystem>
#endif

namespace fb
{
    // template <>
    // DirectoryListing BaseFileSystem<String>::getDirectoryListing(const String& path);

    // template <>
    // DirectoryListingW BaseFileSystem<StringW>::getDirectoryListing(const StringW& path);

    template <>
    Array<String> BasePath<String>::getFolders( const String &path, bool recursive );

    template <>
    Array<StringW> BasePath<StringW>::getFolders( const StringW &path, bool recursive );

    template <>
    Array<String> BasePath<String>::getFiles( const String &path, const String &extension );

    template <>
    Array<StringW> BasePath<StringW>::getFiles( const StringW &path, const StringW &extension );

    template <>
    String BasePath<String>::getFilePath( const String &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.remove_filename().string();
    }

    template <>
    StringW BasePath<StringW>::getFilePath( const StringW &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.remove_filename().wstring();
    }

    template <>
    String BasePath<String>::getFileName( const String &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.filename().string();
    }

    template <>
    StringW BasePath<StringW>::getFileName( const StringW &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.filename().wstring();
    }

    template <>
    String BasePath<String>::getFileExtension( const String &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.extension().string();
    }

    template <>
    StringW BasePath<StringW>::getFileExtension( const StringW &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.extension().wstring();
    }

    template <>
    String BasePath<String>::getFileNameWithoutExtension( const String &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.filename().replace_extension().string();
    }

    template <>
    StringW BasePath<StringW>::getFileNameWithoutExtension( const StringW &filePath )
    {
        boost::filesystem::path path( filePath );
        return path.filename().replace_extension().wstring();
    }

    template <class T>
    void BasePath<T>::setWorkingDirectory( const T &dir )
    {
        boost::filesystem::path path( dir );
        current_path( path );
    }

    template <>
    String BasePath<String>::getWorkingDirectory()
    {
        auto str = boost::filesystem::current_path().string();
        return StringUtil::cleanupPath( str );
    }

    template <>
    StringW BasePath<StringW>::getWorkingDirectory()
    {
        auto str = boost::filesystem::current_path().wstring();
        return StringUtilW::cleanupPath( str );
    }

    template <>
    Array<String> BasePath<String>::getFolders( const String &path, bool recursive )
    {
        Array<String> files;

        try
        {
            boost::filesystem::path dir = path;

            if( recursive )
            {
                boost::filesystem::recursive_directory_iterator it( dir ), end;

                for( auto &entry : boost::make_iterator_range( it, end ) )
                {
                    if( is_directory( entry ) )
                    {
                        files.push_back( StringUtil::cleanupPath( entry.path().string() ) );
                    }
                }
            }
            else
            {
                boost::filesystem::directory_iterator it( dir ), end;

                for( auto &entry : boost::make_iterator_range( it, end ) )
                {
                    if( is_directory( entry ) )
                    {
                        files.push_back( StringUtil::cleanupPath( entry.path().string() ) );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return files;
    }

    template <>
    Array<StringW> BasePath<StringW>::getFolders( const StringW &path, bool recursive )
    {
        Array<StringW> files;

        boost::filesystem::path dir = path;

        if( recursive )
        {
            boost::filesystem::recursive_directory_iterator it( dir ), end;

            for( auto &entry : boost::make_iterator_range( it, end ) )
            {
                if( is_directory( entry ) )
                {
                    files.push_back( entry.path().wstring() );
                }
            }
        }
        else
        {
            boost::filesystem::directory_iterator it( dir ), end;

            for( auto &entry : boost::make_iterator_range( it, end ) )
            {
                if( is_directory( entry ) )
                {
                    files.push_back( entry.path().wstring() );
                }
            }
        }

        return files;
    }

    template <>
    Array<String> BasePath<String>::getPaths( const String &path, bool recursive )
    {
        Array<String> files;

        boost::filesystem::path dir = path;

        if( recursive )
        {
            boost::filesystem::recursive_directory_iterator it( dir ), end;

            for( auto &entry : boost::make_iterator_range( it, end ) )
            {
                if( is_directory( entry ) )
                {
                    files.push_back( entry.path().string() );
                }
            }
        }
        else
        {
            boost::filesystem::directory_iterator it( dir ), end;

            for( auto &entry : boost::make_iterator_range( it, end ) )
            {
                if( is_directory( entry ) )
                {
                    files.push_back( entry.path().string() );
                }
            }
        }

        return files;
    }

    template <>
    Array<StringW> BasePath<StringW>::getPaths( const StringW &path, bool recursive )
    {
        Array<StringW> files;

        boost::filesystem::path dir = path;

        if( recursive )
        {
            boost::filesystem::recursive_directory_iterator it( dir ), end;

            for( auto &entry : boost::make_iterator_range( it, end ) )
            {
                if( is_directory( entry ) )
                {
                    files.push_back( entry.path().wstring() );
                }
            }
        }
        else
        {
            boost::filesystem::directory_iterator it( dir ), end;

            for( auto &entry : boost::make_iterator_range( it, end ) )
            {
                if( is_directory( entry ) )
                {
                    files.push_back( entry.path().wstring() );
                }
            }
        }

        return files;
    }

    template <>
    Array<String> BasePath<String>::getFiles( const String &path )
    {
        Array<String> files;
        files.reserve( 8 );

        boost::filesystem::path dir = path;
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                auto fileName = StringUtil::cleanupPath( entry.path().string() );
                files.push_back( fileName );
            }
        }

        return files;
    }

    template <>
    Array<StringW> BasePath<StringW>::getFiles( const StringW &path )
    {
        Array<StringW> files;
        files.reserve( 8 );

        boost::filesystem::path dir = path;

        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                auto filePath = StringUtilW::cleanupPath( entry.path().wstring() );
                files.push_back( filePath );
            }
        }

        return files;
    }

    template <>
    Array<StringW> BasePath<StringW>::getFilesAsAbsolutePaths( const StringW &path, bool recursive )
    {
        Array<StringW> fileList;

#if defined FB_PLATFORM_WIN32
        if( ( GetFileAttributesW( path.c_str() ) & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
        {
            WIN32_FIND_DATAW fileData;
            auto handle = INVALID_HANDLE_VALUE;

            StringW searchPath = path + L"/*";
            handle = FindFirstFileW( searchPath.c_str(), &fileData );

            if( handle != INVALID_HANDLE_VALUE )
            {
                while( FindNextFileW( handle, &fileData ) )
                {
                    StringW fileName = fileData.cFileName;
                    if( fileName != L".." )
                    {
                        StringW filePath = path + L"/" + fileData.cFileName;
                        if( ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
                        {
                            fileList.push_back( filePath );
                        }
                        else
                        {
                            if( recursive )
                            {
                                Array<StringW> subPaths = getFilesAsAbsolutePaths( filePath, recursive );
                                if( !subPaths.empty() )
                                {
                                    Array<StringW> newPaths( fileList.size() + subPaths.size() );
                                    std::merge( fileList.begin(), fileList.end(), subPaths.begin(),
                                                subPaths.end(), newPaths.begin() );
                                    fileList = newPaths;
                                }
                            }
                        }
                    }
                }

                FindClose( handle );
            }
        }
#endif

        return fileList;
    }

    template <>
    Array<String> BasePath<String>::getFiles( const String &path, const String &extension )
    {
        Array<String> fileList;

#if defined FB_PLATFORM_WIN32
        if( ( GetFileAttributesA( path.c_str() ) & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
        {
            WIN32_FIND_DATA fileData;
            auto handle = INVALID_HANDLE_VALUE;

            String searchPath = path + "/*";
            handle = FindFirstFileA( searchPath.c_str(), &fileData );

            if( handle != INVALID_HANDLE_VALUE )
            {
                while( FindNextFileA( handle, &fileData ) )
                {
                    if( strcmp( fileData.cFileName, ".." ) != 0 )
                    {
                        String filePath = path + String( fileData.cFileName );
                        if( ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
                        {
                            String fileName( fileData.cFileName );
                            if( getFileExtension( fileName ) == extension )
                                fileList.push_back( fileName );
                        }
                    }
                }

                FindClose( handle );
            }
        }
#endif

        return fileList;
    }

    template <>
    Array<String> BasePath<String>::getFilesAsAbsolutePaths( const String &path, bool recursive )
    {
        Array<String> files;
        files.reserve( 8 );

        boost::filesystem::path dir = path;
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                auto fileName = entry.path().string();
                files.push_back( fileName );
            }
            else
            {
                if( recursive )
                {
                    auto subFolderPath = entry.path().string();
                    auto subFolderFiles = getFilesAsAbsolutePaths( subFolderPath, recursive );
                    files.insert( files.end(), subFolderFiles.begin(), subFolderFiles.end() );
                }
            }
        }

        return files;
    }

    template <>
    Array<StringW> BasePath<StringW>::getFiles( const StringW &path, const StringW &extension )
    {
        Array<StringW> fileList;

#if defined FB_PLATFORM_WIN32
        if( ( GetFileAttributesW( path.c_str() ) & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
        {
            WIN32_FIND_DATAW fileData;
            auto handle = INVALID_HANDLE_VALUE;

            StringW searchPath = path + L"/*";
            handle = FindFirstFileW( searchPath.c_str(), &fileData );

            if( handle != INVALID_HANDLE_VALUE )
            {
                while( FindNextFileW( handle, &fileData ) )
                {
                    if( StringW( fileData.cFileName ) != L".." )
                    {
                        StringW filePath = path + StringW( fileData.cFileName );
                        if( ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
                        {
                            StringW fileName( fileData.cFileName );
                            if( getFileExtension( fileName ) == extension )
                                fileList.push_back( fileName );
                        }
                    }
                }

                FindClose( handle );
            }
        }
#endif

        return fileList;
    }

    template <>
    Array<String> BasePath<String>::getFilesAsAbsolutePaths( const String &path, const String &extension,
                                                             bool recursive )
    {
        Array<String> files;
        files.reserve( 8 );

        boost::filesystem::path dir = path;
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                auto fileName = entry.path().string();
                if( getFileExtension( fileName ) == extension )
                {
                    files.push_back( fileName );
                }
            }
            else
            {
                if( recursive )
                {
                    auto subFolderPath = entry.path().string();
                    auto subFolderFiles = getFilesAsAbsolutePaths( subFolderPath, recursive );
                    //files.insert( files.end(), subFolderFiles.begin(), subFolderFiles.end() );

                    for( auto &file : subFolderFiles )
                    {
                        if( getFileExtension( file ) == extension )
                        {
                            files.push_back( file );
                        }
                    }
                }
            }
        }

        return files;
    }

    template <>
    Array<StringW> BasePath<StringW>::getFilesAsAbsolutePaths( const StringW &path,
                                                               const StringW &extension, bool recursive )
    {
        Array<StringW> files;
        files.reserve( 8 );

        boost::filesystem::path dir = path;
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                auto fileName = entry.path().wstring();
                if( getFileExtension( fileName ) == extension )
                {
                    files.push_back( fileName );
                }
            }
            else
            {
                if( recursive )
                {
                    auto subFolderPath = entry.path().wstring();
                    auto subFolderFiles = getFilesAsAbsolutePaths( subFolderPath, recursive );
                    //files.insert( files.end(), subFolderFiles.begin(), subFolderFiles.end() );

                    for( auto &file : subFolderFiles )
                    {
                        if( getFileExtension( file ) == extension )
                        {
                            files.push_back( file );
                        }
                    }
                }
            }
        }

        return files;
    }

    template <>
    String BasePath<String>::getLeaf( const String &filePath )
    {
        boost::filesystem::path path( filePath );
        auto fileName = path.filename();
        return fileName.string();
    }

    template <>
    StringW BasePath<StringW>::getLeaf( const StringW &filePath )
    {
        boost::filesystem::path path( filePath );
        auto fileName = path.filename();
        return fileName.wstring();
    }

    template <class T>
    bool BasePath<T>::isFolder( const T &path )
    {
        boost::filesystem::path p( path );
        return boost::filesystem::is_directory( p );
    }

    template <class T>
    bool BasePath<T>::isFile( const T &filePath )
    {
        boost::filesystem::path path( filePath );
        return boost::filesystem::is_regular_file( path );
    }

    template <class T>
    bool BasePath<T>::isExistingFolder( const T &path )
    {
#if defined FB_PLATFORM_WIN32
        std::ifstream file( path );
        if( file.is_open() )
        {
            return true;
        }

        return false;
#else
        boost::filesystem::path p( path );
        return boost::filesystem::exists( p );
#endif
    }

    template <class T>
    bool BasePath<T>::isExistingFile( const T &filePath )
    {
#if defined FB_PLATFORM_WIN32
        std::ifstream file( filePath );
        if( file.is_open() )
        {
            return true;
        }

        return false;
#else
        return boost::filesystem::exists( filePath );
#endif
    }

    template <>
    Array<String> BasePath<String>::getFilePathFolders( [[maybe_unused]] const String &filePath )
    {
#if FB_USE_BOOST
        Array<String> folders;

        boost::filesystem::path path( filePath );

        auto fileName = path.filename();
        auto folder = fileName.string();
        folders.push_back( folder );

        while( !path.empty() )
        {
            path = path.parent_path();

            fileName = path.filename();
            folder = fileName.string();

            folders.push_back( folder );
        }

        return folders;
#else
        return Array<String>();
#endif
    }

    template <>
    Array<StringW> BasePath<StringW>::getFilePathFolders( [[maybe_unused]] const StringW &filePath )
    {
#if FB_USE_BOOST
        Array<StringW> folders;

        boost::filesystem::path path( filePath );

        auto fileName = path.filename();
        auto folder = fileName.wstring();
        folders.push_back( folder );

        while( !path.empty() )
        {
            path = path.parent_path();

            fileName = path.filename();
            folder = fileName.wstring();

            folders.push_back( folder );
        }

        return folders;
#else
        return Array<StringW>();
#endif
    }

    template <class T>
    bool BasePath<T>::isFilesEqual( const T &lFilePath, const T &rFilePath )
    {
#if defined FB_PLATFORM_WIN32
        std::ifstream lFile( lFilePath, std::ios::in | std::ios::binary );
        std::ifstream rFile( rFilePath, std::ios::in | std::ios::binary );

        if( !lFile.good() || !rFile.good() )
        {
            return false;
        }

        std::streamsize lReadBytesCount = 0;
        std::streamsize rReadBytesCount = 0;

        const int bufferSize = 1048576 * 10;
        auto p_lBuffer = new char[bufferSize];
        auto p_rBuffer = new char[bufferSize];

        do
        {
            lFile.read( p_lBuffer, bufferSize );
            rFile.read( p_rBuffer, bufferSize );
            lReadBytesCount = lFile.gcount();
            rReadBytesCount = rFile.gcount();

            if( lReadBytesCount != rReadBytesCount ||
                std::memcmp( p_lBuffer, p_rBuffer, static_cast<size_t>( lReadBytesCount ) ) != 0 )
            {
                return false;
            }
        } while( lFile.good() || rFile.good() );

        delete[] p_lBuffer;
        delete[] p_rBuffer;

        return true;
#else
        return false;
#endif
    }

    template <class T>
    size_t BasePath<T>::getFileSize( const T &filename )
    {
#if defined FB_PLATFORM_WIN32
        std::ifstream in( filename, std::ifstream::in | std::ifstream::binary );
        in.seekg( 0, std::ifstream::end );
        return (size_t)in.tellg();
#else
        return boost::filesystem::file_size( filename );
#endif
    }

    template <>
    void BasePath<String>::copyFile( const String &srcPath, const String &dstPath )
    {
        auto newPath = getFilePath( dstPath );
        auto cleanDst = StringUtility<String>::cleanupPath( newPath );

        auto bSuccess = true;
        boost::system::error_code ec;

        auto retries = 0;
        boost::filesystem::path dir( cleanDst );
        while( !is_directory( dir ) && retries++ < 100 )
        {
            bSuccess = false;

            if( boost::filesystem::create_directories( cleanDst, ec ) )
            {
                bSuccess = true;
                break;
            }

            Thread::yield();
        }

        if( !bSuccess && ec.value() != 0 )
        {
            auto message = String( "Cannot create directory." ) + cleanDst + " " + ec.message();
            throw Exception( message.c_str() );
        }

#if 0
		std::ifstream  src(srcPath.c_str(), std::ios::binary);
		std::ofstream  dst(cleanDst.c_str(), std::ios::trunc | std::ios::binary);

		dst << src.rdbuf();
#else
        boost::filesystem::copy_file( srcPath, dstPath,
                                      boost::filesystem::copy_options::overwrite_existing );
#endif
    }

    template <>
    void BasePath<StringW>::copyFile( const StringW &srcPath, const StringW &dstPath )
    {
        StringW newPath = getFilePath( dstPath );
        StringW cleanDst = StringUtility<StringW>::replaceAll( newPath, L"\\", L"/" );
        cleanDst = StringUtility<StringW>::replaceAll( cleanDst, L"//", L"/" );

        bool bSuccess = true;
        boost::system::error_code ec;

        int retries = 0;
        boost::filesystem::path dir( cleanDst );
        while( !is_directory( dir ) && retries++ < 100 )
        {
            bSuccess = false;

            if( boost::filesystem::create_directories( cleanDst, ec ) )
            {
                bSuccess = true;
                break;
            }

            // Thread::sleep(10.0);
        }

        if( !bSuccess && ec.value() != 0 )
        {
            auto message = String( "Cannot create directory." );
            throw Exception( message.c_str() );
        }

#if 0
		std::ifstream  src(srcPath.c_str(), std::ios::binary);
		std::ofstream  dst(cleanDst.c_str(), std::ios::trunc | std::ios::binary);

		dst << src.rdbuf();
#else
        boost::filesystem::copy_file( srcPath, dstPath,
                                      boost::filesystem::copy_options::overwrite_existing );
#endif
    }

    template <>
    void BasePath<String>::createDirectories( const String &dst )
    {
        String cleanDst = StringUtil::replaceAll( dst, "\\", "/" );
        cleanDst = StringUtil::replaceAll( cleanDst, "//", "/" );

        bool bSuccess = true;
        boost::system::error_code ec;

        int retries = 0;
        boost::filesystem::path dir( cleanDst );
        while( !is_directory( dir ) && retries++ < 100 )
        {
            bSuccess = false;

            if( boost::filesystem::create_directories( cleanDst, ec ) )
            {
                bSuccess = true;
                break;
            }

            // Thread::sleep(1.0);
        }

        if( !bSuccess && ec.value() != 0 )
        {
            String message = String( "Cannot create directory." ) + cleanDst + " " + ec.message();
            throw Exception( message.c_str() );
        }
    }

    template <>
    void BasePath<StringW>::createDirectories( const StringW &dst )
    {
        StringW cleanDst = StringUtility<StringW>::replaceAll( dst, L"\\", L"/" );
        cleanDst = StringUtility<StringW>::replaceAll( cleanDst, L"//", L"/" );

        bool bSuccess = true;
        boost::system::error_code ec;

        int retries = 0;
        boost::filesystem::path dir( cleanDst );
        while( !is_directory( dir ) && retries++ < 100 )
        {
            bSuccess = false;

            if( boost::filesystem::create_directories( cleanDst, ec ) )
            {
                bSuccess = true;
                break;
            }

            // Thread::sleep(1.0);
        }

        if( !bSuccess && ec.value() != 0 )
        {
            String message = String( "Cannot create directory." ) +
                             StringUtility<StringW>::toUTF16to8( cleanDst ) + " " + ec.message();
            throw Exception( message.c_str() );
        }
    }

    template <>
    String BasePath<String>::getMD5( [[maybe_unused]] const String &filePath )
    {
#if FB_USE_OPENSSL
        if( !BasePath<String>::isExistingFile( filePath ) )
        {
            return String( "" );
        }

        unsigned char c[MD5_DIGEST_LENGTH + 1];
        memset( c, 0, sizeof( c ) );

        const char *filename = filePath.c_str();
        int i;
        FILE *inFile = fopen( filename, "rb" );
        MD5_CTX mdContext;
        int bytes;

        const int bufferSize = 1048576 * 10;
        unsigned char *data = new unsigned char[bufferSize];

        if( inFile == NULL )
        {
            delete[] data;
            return String( "" );
        }

        MD5_Init( &mdContext );
        while( ( bytes = fread( data, 1, bufferSize, inFile ) ) != 0 )
            MD5_Update( &mdContext, data, bytes );

        MD5_Final( c, &mdContext );

        c[MD5_DIGEST_LENGTH] = 0;
        String retVal = (char *)c;

        fclose( inFile );

        delete[] data;
        return retVal;
#else
        if( isExistingFile( filePath ) )
        {
            std::fstream stream( filePath, std::fstream::in );
            stream.seekg( 0, std::ios_base::end );
            auto size = static_cast<size_t>( stream.tellg() );
            stream.seekg( 0, std::ios_base::beg );

            auto data = new unsigned char[size];
            stream.read( (char *)data, size );

            return getMD5( data, size );
        }

        return String( "" );
#endif
    }

    template <>
    String BasePath<StringW>::getMD5( [[maybe_unused]] const StringW &filePath )
    {
#if FB_USE_OPENSSL
        if( !BasePath<StringW>::isExistingFile( filePath ) )
        {
            return String( "" );
        }

        std::fstream stream( filePath, std::fstream::in | std::fstream::binary );
        if( !stream.is_open() )
        {
            throw Exception( "Cannot open file." );
        }

        const int bufferSize = 1048576 * 10;
        unsigned char *data = new unsigned char[bufferSize];
        size_t bytesRead = 0;
        size_t totalBytesRead = 0;

        unsigned char c[MD5_DIGEST_LENGTH + 1];
        memset( c, 0, sizeof( c ) );

        MD5_CTX mdContext;

        MD5_Init( &mdContext );

        while( !stream.eof() )
        {
            stream.read( (char *)data, bufferSize );
            bytesRead = (size_t)stream.gcount();
            totalBytesRead += bytesRead;

            MD5_Update( &mdContext, data, bytesRead );
        }

        MD5_Final( c, &mdContext );

        c[MD5_DIGEST_LENGTH] = 0;
        String retVal = (char *)c;

        delete[] data;
        return retVal;
#else
        if( isExistingFile( filePath ) )
        {
#    if defined FB_PLATFORM_WIN32
            std::fstream stream( filePath, std::fstream::in );
#    else
            std::fstream stream( StringUtil::toUTF16to8( filePath ), std::fstream::in );
#    endif

            stream.seekg( 0, std::ios_base::end );
            auto size = static_cast<size_t>( stream.tellg() );
            stream.seekg( 0, std::ios_base::beg );

            auto data = new unsigned char[size];
            stream.read( (char *)data, size );

            return getMD5( data, size );
        }

        return String( "" );
#endif
    }

    template <class T>
    String BasePath<T>::getMD5( [[maybe_unused]] void *data, [[maybe_unused]] size_t size )
    {
#if FB_USE_OPENSSL
        if( !BasePath<String>::isExistingFile( filePath ) )
        {
            return String( "" );
        }

        unsigned char c[MD5_DIGEST_LENGTH + 1];
        memset( c, 0, sizeof( c ) );

        const char *filename = filePath.c_str();
        int i;
        FILE *inFile = fopen( filename, "rb" );
        MD5_CTX mdContext;
        int bytes;

        const int bufferSize = 1048576 * 10;
        unsigned char *data = new unsigned char[bufferSize];

        if( inFile == NULL )
        {
            delete[] data;
            return String( "" );
        }

        MD5_Init( &mdContext );
        while( ( bytes = fread( data, 1, bufferSize, inFile ) ) != 0 )
            MD5_Update( &mdContext, data, bytes );

        MD5_Final( c, &mdContext );

        c[MD5_DIGEST_LENGTH] = 0;
        String retVal = (char *)c;

        fclose( inFile );

        delete[] data;
        return retVal;
#else
        boost::uuids::detail::md5 s;
        // char hash[20] = {0};

        s.process_bytes( data, size );
        boost::uuids::detail::md5::digest_type digest;
        s.get_digest( digest );

        const auto charDigest = reinterpret_cast<const char *>( &digest );
        std::string result;
        boost::algorithm::hex( charDigest, charDigest + sizeof( boost::uuids::detail::md5::digest_type ),
                               std::back_inserter( result ) );

        return result;
#endif
    }

    template <>
    void BasePath<String>::deleteFiles( [[maybe_unused]] const String &path )
    {
        auto files = getFiles( path );
        for( auto &fileName : files )
        {
            auto filePath = path + String( "/" ) + fileName;

            boost::filesystem::path dir( filePath );
            boost::filesystem::remove( dir );
        }

        auto folders = getFolders( path, false );
        for( auto &folder : folders )
        {
            try
            {
                auto folderPath = path + "/" + folder + "/";
                // deleteFiles(folderPath);

                boost::filesystem::path dir( folderPath );
                if( is_directory( dir ) )
                {
                    remove_all( dir );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_ERROR( e.what() );
            }
        }
    }

    template <class T>
    void BasePath<T>::deleteFolder( const T &filePath )
    {
        boost::filesystem::path dir( filePath );
        boost::filesystem::remove( dir );
    }

    template <class T>
    void BasePath<T>::deleteFile( const T &filePath )
    {
        boost::filesystem::path dir( filePath );
        boost::filesystem::remove( dir );
    }

    template <>
    void BasePath<StringW>::deleteFiles( [[maybe_unused]] const StringW &path )
    {
        Array<StringW> files = getFiles( path );
        for( u32 i = 0; i < files.size(); ++i )
        {
            StringW fileName = files[i];
            StringW filePath = path + StringW( L"/" ) + fileName;

            boost::filesystem::path dir( filePath );
            boost::filesystem::remove( dir );
        }

        Array<StringW> folders = getFolders( path, false );
        for( u32 i = 0; i < folders.size(); ++i )
        {
            try
            {
                StringW folder = folders[i];
                StringW folderPath = path + StringW( L"/" ) + folder + StringW( L"/" );
                // deleteFiles(folderPath);

                boost::filesystem::path dir( folderPath );
                if( is_directory( dir ) )
                {
                    remove_all( dir );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    }

    template <>
    String BasePath<String>::getAbsolutePath( const String &path )
    {
        auto full_path( boost::filesystem::initial_path<boost::filesystem::path>() );
        full_path = system_complete( boost::filesystem::path( path ) );
        full_path = full_path.lexically_normal();

        auto absolutePath = full_path.string();
        return StringUtil::cleanupPath( absolutePath );
    }

    template <>
    StringW BasePath<StringW>::getAbsolutePath( const StringW &path )
    {
        auto full_path( boost::filesystem::initial_path<boost::filesystem::path>() );
        full_path = system_complete( boost::filesystem::path( path ) );
        full_path = full_path.lexically_normal();

        auto absolutePath = StringW( full_path.wstring() );
        return StringUtilW::cleanupPath( absolutePath );
    }

    template <>
    String BasePath<String>::getAbsolutePath( const String &path, const String &relativePath )
    {
        try
        {
            if( StringUtil::isNullOrEmpty( relativePath ) )
            {
                return String( "" );
            }

            auto p = boost::filesystem::canonical( relativePath.c_str(), path.c_str() );
            auto absolutePath = String( p.string() );
            return StringUtil::cleanupPath( absolutePath );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return String( "" );
    }

    template <>
    StringW BasePath<StringW>::getAbsolutePath( const StringW &path, const StringW &relativePath )
    {
        if( StringUtilW::isNullOrEmpty( relativePath ) )
        {
            return StringW( L"" );
        }

        auto p = boost::filesystem::canonical( relativePath.c_str(), path.c_str() );
        auto absolutePath = StringW( p.wstring() );
        return StringUtilW::cleanupPath( absolutePath );
    }

    template <>
    String BasePath<String>::lexically_normal( const String &path )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path );
        auto l = p.lexically_normal();
        return StringUtil::cleanupPath( l.string() );
#else
        auto p = std::filesystem::path( path );
        auto l = p.lexically_normal();
        return StringUtil::cleanupPath( l.string() );
#endif
    }

    template <>
    StringW BasePath<StringW>::lexically_normal( const StringW &path )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path );
        auto l = p.lexically_normal();
        return StringUtilW::cleanupPath( l.wstring() );
#else
        auto p = std::filesystem::path( path );
        auto l = p.lexically_normal();
        return StringUtilW::cleanupPath( l.wstring() );
#endif
    }

    template <>
    String BasePath<String>::lexically_normal( const String &path, const String &relativePath )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path + "/" + relativePath );
        auto l = p.lexically_normal();
        return StringUtil::cleanupPath( l.string() );
#else
        auto p = std::filesystem::path( path + "/" + relativePath );
        auto l = p.lexically_normal();
        return StringUtil::cleanupPath( l.string() );
#endif
    }

    template <>
    StringW BasePath<StringW>::lexically_normal( const StringW &path, const StringW &relativePath )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path + L"/" + relativePath );
        auto l = p.lexically_normal();
        return StringUtilW::cleanupPath( l.wstring() );
#else
        auto p = std::filesystem::path( path + L"/" + relativePath );
        auto l = p.lexically_normal();
        return StringUtilW::cleanupPath( l.wstring() );
#endif
    }

    template <>
    String BasePath<String>::lexically_relative( const String &path, const String &relativePath )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path );
        auto r = boost::filesystem::path( relativePath );
        auto l = p.lexically_relative( r );
        return StringUtil::cleanupPath( l.string() );
#else
        auto p = std::filesystem::path( path );
        auto r = std::filesystem::path( relativePath );
        auto l = p.lexically_relative( r );
        return StringUtil::cleanupPath( l.string() );
#endif
    }

    template <>
    StringW BasePath<StringW>::lexically_relative( const StringW &path, const StringW &relativePath )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path );
        auto r = boost::filesystem::path( relativePath );
        auto l = p.lexically_relative( r );
        return StringUtilW::cleanupPath( l.wstring() );
#else
        auto p = std::filesystem::path( path );
        auto r = std::filesystem::path( relativePath );
        auto l = p.lexically_relative( r );
        return StringUtilW::cleanupPath( l.wstring() );
#endif
    }

    template <>
    String BasePath<String>::lexically_proximate( const String &path, const String &relativePath )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path );
        auto r = boost::filesystem::path( relativePath );
        auto l = p.lexically_proximate( r );
        return StringUtil::cleanupPath( l.string() );
#else
        auto p = std::filesystem::path( path );
        auto r = std::filesystem::path( relativePath );
        auto l = p.lexically_proximate( r );
        return StringUtil::cleanupPath( l.string() );
#endif
    }

    template <>
    StringW BasePath<StringW>::lexically_proximate( const StringW &path, const StringW &relativePath )
    {
#if defined FB_USE_BOOST
        auto p = boost::filesystem::path( path );
        auto r = boost::filesystem::path( relativePath );
        auto l = p.lexically_proximate( r );
        return StringUtilW::cleanupPath( l.wstring() );
#else
        auto p = std::filesystem::path( path );
        auto r = std::filesystem::path( relativePath );
        auto l = p.lexically_proximate( r );
        return StringUtilW::cleanupPath( l.wstring() );
#endif
    }

    template <class T>
    void _append( boost::filesystem::path &p, boost::filesystem::path::iterator begin,
                  boost::filesystem::path::iterator end )
    {
        for( ; begin != end; ++begin )
        {
            p /= *begin;
        }
    }

    // Return path when appended to a_From will resolve to same as a_To
    boost::filesystem::path make_relative( boost::filesystem::path a_From, boost::filesystem::path a_To )
    {
        a_From = absolute( a_From );
        a_To = absolute( a_To );
        boost::filesystem::path ret;
        auto itrFrom( a_From.begin() ), itrTo( a_To.begin() );
        // Find common base
        for( auto toEnd( a_To.end() ), fromEnd( a_From.end() );
             itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo )
            ;
        // Navigate backwards in directory to reach previously found base
        for( auto fromEnd( a_From.end() ); itrFrom != fromEnd; ++itrFrom )
        {
            if( ( *itrFrom ) != "." )
                ret /= "..";
        }
        // Now navigate down the directory branch
        _append<boost::filesystem::path::iterator>( ret, itrTo, a_To.end() );
        return ret;
    }

    template <>
    String BasePath<String>::getRelativePath( const String &path, const String &relativePath )
    {
        boost::filesystem::path basePath( path );
        boost::filesystem::path relPath( relativePath );

        auto sRelativePath = make_relative( basePath, relPath ).string();
        return StringUtil::cleanupPath( sRelativePath );
    }

    template <>
    StringW BasePath<StringW>::getRelativePath( const StringW &path, const StringW &relativePath )
    {
        boost::filesystem::path basePath( path );
        boost::filesystem::path relPath( relativePath );

        auto sRelativePath = make_relative( basePath, relPath ).wstring();
        return StringUtilW::cleanupPath( sRelativePath );
    }

    template <class T>
    void BasePath<T>::rename( const T &pathOld, const T &pathNew )
    {
        boost::filesystem::path pathOldObject( pathOld );
        boost::filesystem::path pathNewObject( pathNew );
        boost::filesystem::rename( pathOldObject, pathNewObject );
    }

    template <>
    String BasePath<String>::macBundlePath()
    {
#if defined FB_PLATFORM_APPLE
        return fb::macBundlePath();
#else
        return "";
#endif
    }

    template <>
    StringW BasePath<StringW>::macBundlePath()
    {
        return L"";
    }

    template <class T>
    bool BasePath<T>::isPathAbsolute( const T &basePath )
    {
#ifdef FB_USE_BOOST
        boost::filesystem::path path( basePath );
        return path.is_absolute();
#else
        return false;
#endif
    }

    template <class T>
    bool BasePath<T>::isPathRelative( [[maybe_unused]] const T &basePath,
                                      [[maybe_unused]] const T &relativePath )
    {
#ifdef FB_USE_BOOST
        boost::filesystem::path path( relativePath.c_str() );
        return path.is_relative();
#else
        return false;
#endif
    }

    template <class T>
    bool BasePath<T>::hasFileName( const String &path )
    {
        boost::filesystem::path p( path );
        return p.has_filename();
    }

    template <>
    String BasePath<String>::readAllText( const String &path )
    {
        std::fstream fs;
        fs.open( path, std::fstream::in );

        const size_t bufSize = 4096;
        char pBuf[bufSize];
        std::string result;

        size_t nr = 1;
        while( !fs.eof() && nr > 0 )
        {
            fs.read( pBuf, bufSize );
            nr = static_cast<size_t>( fs.gcount() );

            result.append( pBuf, nr );
        }

        fs.close();

        return result;
    }

    template <>
    void BasePath<String>::writeAllText( const String &path, const String &contents )
    {
        std::fstream fs;
        fs.open( path, std::fstream::out );

        fs << contents;

        fs.close();
    }

    template <>
    StringW BasePath<StringW>::readAllText( const StringW &path )
    {
        std::wfstream fs;

#if defined FB_PLATFORM_WIN32
        fs.open( path, std::wfstream::in );
#else
        std::string sPath = StringUtil::toUTF16to8( path );
        fs.open( sPath, std::wfstream::in );
#endif

        const size_t bufSize = 4096;
        wchar_t pBuf[bufSize];
        StringW result;

        size_t nr = 1;
        while( !fs.eof() && nr > 0 )
        {
            fs.read( pBuf, bufSize * sizeof( wchar_t ) );
            nr = static_cast<size_t>( fs.gcount() );

            result.append( pBuf, nr / sizeof( wchar_t ) );
        }

        fs.close();

        return result;
    }

    template <>
    void BasePath<StringW>::writeAllText( const StringW &path, const StringW &contents )
    {
        std::wfstream fs;

#if defined FB_PLATFORM_WIN32
        fs.open( path, std::wfstream::out );
#else
        std::string sPath = StringUtil::toUTF16to8( path );
        fs.open( sPath, std::wfstream::out );
#endif

        fs << contents;

        fs.close();
    }

    // explicit instantiation
    template class BasePath<String>;
    template class BasePath<StringW>;
}  // namespace fb
