#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/FileSystem.h>
#include <FBCore/IO/FileSystemArchive.h>
#include <FBCore/IO/ObfuscatedZipArchive.h>
#include <FBCore/IO/ZipArchive.h>
#include <FBCore/IO/FolderListingT.h>
#include <FBCore/IO/FileDataStream.h>
#include <FBCore/IO/NativeFileDialog.h>
#include <FBCore/IO/FileList.h>
#include <FBCore/FBCore.h>

#include <cstdio>
#include <filesystem>
#include <fstream>

#if FB_USE_BOOST
#    include <boost/filesystem/operations.hpp>
#    include <boost/crc.hpp>      // for boost::crc_basic, boost::crc_optimal
#    include <boost/cstdint.hpp>  // for boost::uint16_t
#    include <boost/range/iterator_range.hpp>
#    include <boost/range/algorithm.hpp>
#    include <boost/range/iterator.hpp>
#endif

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FileSystem, IFileSystem );

    FileSystem::FileSystem()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "FileSystem" );
        }
    }

    FileSystem::~FileSystem()
    {
        unload( nullptr );
    }

    void FileSystem::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            setLoadingState( LoadingState::Loading );

            const auto size = 4096;

            constexpr auto fileInfoSize = sizeof( FileInfo );
            //m_files.reserve( size );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FileSystem::reload( SmartPtr<ISharedObject> data )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        unload( data );
        load( data );
    }

    void FileSystem::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                if( auto p = getFilesPtr() )
                {
                    auto &files = *p;
                    files.clear();

                    m_files = nullptr;
                }

                if( auto p = getFolderArchivesPtr() )
                {
                    auto &folderArchives = *p;
                    for( auto a : folderArchives )
                    {
                        a->unload( data );
                        a = nullptr;
                    }

                    setFolderArchivesPtr( nullptr );
                }

                if( auto p = getFileArchivesPtr() )
                {
                    auto &fileArchives = *p;
                    for( auto a : fileArchives )
                    {
                        a->unload( data );
                    }

                    m_fileArchives = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto FileSystem::isExistingFile( const String &filePath, bool ignorePath, bool ignoreCase ) const
        -> bool
    {
        try
        {
            if( !Path::hasFileName( filePath ) )
            {
                return false;
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            if( applicationManager->isEditor() )
            {
                auto existing = false;
                if( Path::isPathAbsolute( filePath ) )
                {
                    existing = Path::isExistingFile( filePath );
                }
                else
                {
                    existing = Path::isExistingFile( projectPath + filePath );
                }

                if( existing )
                {
                    return true;
                }
            }

            if( auto pFiles = getFilesPtr() )
            {
                auto &files = *pFiles;

                for( auto &file : files )
                {
                    if( file.filePath == filePath || file.filePathLowerCase == filePath ||
                        file.fileName == filePath || file.fileNameLowerCase == filePath )
                    {
                        return true;
                    }
                }

                auto filePathLower = StringUtil::make_lower( filePath );
                auto pFilePathLower = filePathLower.c_str();

                for( auto &file : files )
                {
                    if( file.filePath == pFilePathLower || file.filePathLowerCase == pFilePathLower ||
                        file.fileName == pFilePathLower || file.fileNameLowerCase == pFilePathLower )
                    {
                        return true;
                    }
                }

                if( Path::isPathAbsolute( filePath ) )
                {
                    auto relativePath = Path::getRelativePath( projectPath, filePath );

                    for( auto &file : files )
                    {
                        if( file.filePath == relativePath || file.filePathLowerCase == relativePath ||
                            file.fileName == relativePath || file.fileNameLowerCase == relativePath )
                        {
                            return true;
                        }
                    }
                }
            }

            if( const auto p = getFolderArchivesPtr() )
            {
                const auto &folderArchives = *p;
                for( auto archive : folderArchives )
                {
                    auto hasFile = archive->exists( filePath, ignorePath, ignoreCase );
                    if( hasFile )
                    {
                        return true;
                    }
                }
            }

            // check archives
            if( const auto p = getFileArchivesPtr() )
            {
                const auto &fileArchives = *p;
                for( auto archive : fileArchives )
                {
                    auto hasFile = archive->exists( filePath, ignorePath, ignoreCase );
                    if( hasFile )
                    {
                        return true;
                    }
                }
            }

            //FB_ASSERT( !Path::isExistingFile( filePath ) && Path::isFile(filePath) );
            return false;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return false;
    }

    auto FileSystem::isExistingFile( const String &path, const String &filePath, bool ignorePath,
                                     bool ignoreCase ) const -> bool
    {
        return false;
    }

    auto FileSystem::isExistingFolder( const String &path ) const -> bool
    {
        try
        {
            if( Path::isPathAbsolute( path ) )
            {
#if FB_USE_BOOST
                return boost::filesystem::exists( path );
#else
                return false;
#endif
            }
            auto applicationManager = core::ApplicationManager::instance();

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto absolutePath = Path::lexically_normal( projectPath, path );

#if FB_USE_BOOST
            return Path::isExistingFile( absolutePath );
#else
            return false;
#endif

            return false;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return false;
    }

    auto FileSystem::isExistingFile( const StringW &filePath ) const -> bool
    {
        return false;
    }

    auto FileSystem::isExistingFolder( const StringW &path ) const -> bool
    {
        return false;
    }

    void FileSystem::createDirectories( const String &path )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto cleanDst = String( "" );

            if( Path::isPathAbsolute( path ) )
            {
                cleanDst = StringUtil::cleanupPath( path );
            }
            else
            {
                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                auto absolutePath = Path::lexically_normal( projectPath, path );
                cleanDst = StringUtil::cleanupPath( absolutePath );
            }

            bool bSuccess = true;
            boost::system::error_code ec;

            auto retries = 0;
            boost::filesystem::path dir( cleanDst );
            while( !is_directory( dir ) && retries++ < 10 )
            {
                bSuccess = false;

                if( boost::filesystem::create_directories( cleanDst, ec ) )
                {
                    bSuccess = true;
                    break;
                }

                Thread::sleep( 0.2 );
            }

            if( !bSuccess && ec.value() != 0 )
            {
                auto message = String( "Cannot create directory." ) + cleanDst + " " + ec.message();
                throw Exception( message );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FileSystem::deleteFilesFromPath( const String &path )
    {
        auto files = Path::getFiles( path );
        for( auto fileName : files )
        {
            std::string filePath = path + std::string( "/" ) + fileName;

            boost::filesystem::path dir( filePath );
            boost::filesystem::remove( dir );
        }

        auto folders = Path::getFolders( path, false );
        for( auto folder : folders )
        {
            try
            {
                std::string folderPath = path + "/" + folder + "/";
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
            catch( ... )
            {
                FB_LOG_ERROR( "Unknown exception." );
            }
        }
    }

    void FileSystem::deleteFilesFromPathW( const StringW &path )
    {
        auto files = PathW::getFiles( path );
        for( auto fileName : files )
        {
            StringW filePath = path + StringW( L"/" ) + fileName;

            boost::filesystem::path dir( filePath );
            boost::filesystem::remove( dir );
        }

        auto folders = PathW::getFolders( path, false );
        for( auto folder : folders )
        {
            try
            {
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
            catch( ... )
            {
                FB_LOG_ERROR( "Unknown exception." );
            }
        }
    }

    void FileSystem::getFileNamesInFolder( const String &path, Array<String> &fileNames )
    {
#ifdef FB_PLATFORM_WIN32
        if( isFolder( path ) )
        {
            WIN32_FIND_DATAA fileData;
            auto handle = INVALID_HANDLE_VALUE;

            String searchPath = path + "/*";
            handle = FindFirstFileA( searchPath.c_str(), &fileData );

            if( handle != INVALID_HANDLE_VALUE )
            {
                while( FindNextFileA( handle, &fileData ) )
                {
                    String fileName = fileData.cFileName;
                    if( fileName == ".." )  // IF ITS NOT THE PARENT FILE
                    {
                        fileNames.push_back( fileName );
                    }
                }

                FindClose( handle );
            }
        }
        else
        {
            throw Exception( "FileSystem::getFileNamesInFolder - Error path is not a folder" );
        }
#else
#endif
    }

    void FileSystem::getSubFolders( const String &path, Array<String> &folderNames )
    {
#ifdef FB_PLATFORM_WIN32
        if( isFolder( path ) )
        {
            WIN32_FIND_DATAA fileData;
            auto handle = INVALID_HANDLE_VALUE;

            String searchPath = path + "/*";
            handle = FindFirstFileA( searchPath.c_str(), &fileData );

            if( handle != INVALID_HANDLE_VALUE )
            {
                while( FindNextFileA( handle, &fileData ) )
                {
                    if( fileData.dwFileAttributes &
                        FILE_ATTRIBUTE_DIRECTORY )  // IF ITS NOT THE PARENT FILE
                    {
                        String fileName = StringUtil::toString( fileData.cFileName );
                        if( fileName != String( ".." ) )
                        {
                            folderNames.push_back( fileName );
                        }
                    }
                }

                FindClose( handle );
            }
        }
        else
        {
            FB_EXCEPTION( "FileSystem::getFileNamesInFolder - Error path is not a folder" );
        }
#else
#endif
    }

    auto FileSystem::isFolder( const String &path ) -> bool
    {
#if FB_USE_BOOST
        return boost::filesystem::is_directory( path );
#elif defined FB_PLATFORM_WIN32
        auto str = path;
        return ( GetFileAttributesA( str.c_str() ) & FILE_ATTRIBUTE_DIRECTORY ) != 0;
#else
        return false;
#endif
    }

    auto FileSystem::setWorkingDirectory( const String &directory ) -> bool
    {
#if FB_USE_BOOST
        boost::filesystem::path path( directory );
        current_path( path );
        return true;
#else
        return false;
#endif
    }

    auto FileSystem::getWorkingDirectory() -> String
    {
#if FB_USE_BOOST
        return boost::filesystem::current_path().string();
#else
        return "";
#endif
    }

    auto FileSystem::addFileArchive( const String &filename, bool ignoreCase /*=true*/,
                                     bool ignorePaths /*=true*/, ArchiveType archiveType /*=0*/,
                                     const String &password /*= StringUtil::EmptyString*/ ) -> bool
    {
        if( StringUtil::isNullOrEmpty( filename ) )
        {
            FB_EXCEPTION( "Invalid file name." );
        }

        addArchive( filename, archiveType );
        return true;
    }

    auto FileSystem::openFileDialog() -> SmartPtr<INativeFileDialog>
    {
        return fb::make_ptr<NativeFileDialog>();
    }

    auto FileSystem::open( const String &filePath ) -> SmartPtr<IStream>
    {
        auto isBinary = true;
        auto fileExt = Path::getFileExtension( filePath );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".glsl" || fileExt == ".hlsl" || fileExt == ".cfg" ||
            fileExt == ".h" || fileExt == ".cpp" )
        {
            isBinary = false;
        }

        if( fileExt == ".ttf" )
        {
            isBinary = true;
        }

        return open( filePath, true, isBinary, false, false, false );
    }

    auto FileSystem::openW( const StringW &filePath ) -> SmartPtr<IStream>
    {
        auto str = StringUtil::toUTF16to8( filePath );

        auto isBinary = true;
        auto fileExt = Path::getFileExtension( str );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".glsl" || fileExt == ".hlsl" || fileExt == ".cfg" ||
            fileExt == ".h" || fileExt == ".cpp" )
        {
            isBinary = false;
        }

        if( fileExt == ".ttf" )
        {
            isBinary = true;
        }

        return open( str, true, isBinary, false, false, false );
    }

    auto FileSystem::open( const String &filePath, bool input, bool binary, bool truncate,
                           bool ignorePath, bool ignoreCase ) -> SmartPtr<IStream>
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            s32 flags = input ? std::ifstream::in : std::ifstream::out;

            if( binary )
            {
                flags = flags | std::ifstream::binary;
            }

            if( truncate )
            {
                flags = flags | std::ifstream::trunc;
            }

            auto f = flags;

#if !FB_FINAL
            if( Path::isPathAbsolute( filePath ) )
            {
                if( !input )
                {
                    auto stream = new std::fstream( filePath, f );
                    if( stream->is_open() )
                    {
                        auto pStream = fb::make_ptr<FileDataStream>();
                        pStream->setFStream( stream );
                        return pStream;
                    }

                    delete stream;
                }
                else
                {
                    auto stream = new std::ifstream( filePath, f );
                    if( stream->is_open() )
                    {
                        auto pStream = fb::make_ptr<FileDataStream>();
                        pStream->setInStream( stream );
                        return pStream;
                    }

                    delete stream;
                }
            }
            else
            {
                auto absolutePath = Path::lexically_normal( projectPath, filePath );

                if( !input )
                {
                    auto stream = new std::fstream( absolutePath, f );
                    if( stream->is_open() )
                    {
                        auto pStream = fb::make_ptr<FileDataStream>();
                        pStream->setFStream( stream );
                        return pStream;
                    }

                    delete stream;
                }
                else
                {
                    auto stream = new std::ifstream( absolutePath, f );
                    if( stream->is_open() )
                    {
                        auto pStream = fb::make_ptr<FileDataStream>();
                        pStream->setInStream( stream );
                        return pStream;
                    }

                    delete stream;
                }
            }
#endif

            auto fileId = StringUtil::getHash64( filePath );

            if( auto pFiles = getFilesPtr() )
            {
                auto &files = *pFiles;

                for( auto &file : files )
                {
                    if( file.fileId == fileId )
                    {
                        auto absolutePath = Path::getAbsolutePath( projectPath, file.filePath.c_str() );
                        auto stream = new std::fstream( absolutePath, f );
                        if( stream->is_open() )
                        {
                            auto pStream = fb::make_ptr<FileDataStream>();
                            pStream->setFStream( stream );
                            return pStream;
                        }

                        delete stream;
                    }
                }

                for( auto &file : files )
                {
                    if( file.filePath == filePath || file.filePathLowerCase == filePath ||
                        file.fileName == filePath || file.fileNameLowerCase == filePath )
                    {
                        auto absolutePath = Path::getAbsolutePath( projectPath, file.filePath.c_str() );
                        auto stream = new std::fstream( absolutePath, f );
                        if( stream->is_open() )
                        {
                            auto pStream = fb::make_ptr<FileDataStream>();
                            pStream->setFStream( stream );
                            return pStream;
                        }

                        delete stream;
                    }
                }
            }

            if( auto p = getFolderArchivesPtr() )
            {
                auto &archives = *p;

                for( auto &archive : archives )
                {
                    if( archive->exists( filePath, ignorePath, ignoreCase ) )
                    {
                        if( auto file = archive->open( filePath, input, binary, truncate, ignorePath ) )
                        {
                            return file;
                        }
                    }
                }
            }

            if( const auto p = getFileArchivesPtr() )
            {
                const auto &fileArchives = *p;
                for( auto archive : fileArchives )
                {
                    if( archive )
                    {
                        if( archive->exists( filePath, ignorePath, ignoreCase ) )
                        {
                            if( auto file =
                                    archive->open( filePath, input, binary, truncate, ignorePath ) )
                            {
                                return file;
                            }
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto FileSystem::getAbsolutePath( const String &filename ) const -> String
    {
#if defined( _FB_WINDOWS_CE_PLATFORM_ )
        return filename;
#elif defined( _FB_WINDOWS_API_ )
        char *p = 0;
        char fpath[_MAX_PATH];
#    if defined( _IRR_WCHAR_FILESYSTEM )
        p = _wfullpath( fpath, filename.c_str(), _MAX_PATH );
        StringW tmp( p );
        tmp.replace( L'\\', L'/' );
#    else
        p = _fullpath( fpath, filename.c_str(), _MAX_PATH );
        String tmp( p );
        tmp.replace( '\\', '/' );
#    endif
        return tmp;
#elif( defined( _IRR_POSIX_API_ ) || defined( _IRR_OSX_PLATFORM_ ) )
        c8 *p = 0;
        c8 fpath[4096];
        fpath[0] = 0;
        p = realpath( filename.c_str(), fpath );
        if( !p )
        {
            // content in fpath is unclear at this point
            if( !fpath[0] )  // seems like fpath wasn't altered, use our best guess
            {
                return flattenFilename( filename );
            }
            else
                return fpath;
        }
        if( filename[filename.size() - 1] == '/' )
            return p + "/";
        else
            return p;
#else
        return filename;
#endif
    }

    auto FileSystem::getFilesWithExtension( const String &extension ) const -> Array<FileInfo>
    {
        Array<FileInfo> files;
        files.reserve( 128 );

        if( auto pFiles = getFilesPtr() )
        {
            auto &systemFiles = *pFiles;

            for( auto &file : systemFiles )
            {
                auto fileName = file.filePath;
                auto fileExtension = Path::getFileExtension( fileName );

                if( extension == fileExtension )
                {
                    files.push_back( file );
                }
            }
        }

        if( const auto p = getFolderArchivesPtr() )
        {
            const auto &archives = *p;
            for( auto &archive : archives )
            {
                auto archiveFiles = archive->getFiles();

                for( auto &file : archiveFiles )
                {
                    const auto &fileName = file.filePath;
                    auto fileExtension = Path::getFileExtension( fileName );

                    if( extension == fileExtension )
                    {
                        files.push_back( file );
                    }
                }
            }
        }

        if( const auto p = getFileArchivesPtr() )
        {
            const auto &archives = *p;
            for( auto &archive : archives )
            {
                auto archiveFiles = archive->getFiles();

                for( auto &file : archiveFiles )
                {
                    const auto &fileName = file.filePath;
                    auto fileExtension = Path::getFileExtension( fileName );

                    if( extension == fileExtension )
                    {
                        files.push_back( file );
                    }
                }
            }
        }

        return files;
    }

    auto FileSystem::getFilesWithExtension( const String &path, const String &extension ) const
        -> Array<FileInfo>
    {
        Array<FileInfo> files;
        files.reserve( 128 );
        if( auto pFiles = getFilesPtr() )
        {
            auto &systemFiles = *pFiles;
            for( auto &file : systemFiles )
            {
                if( file.path == path || file.path.find( path ) != String::npos )
                {
                    auto fileName = file.filePath;
                    auto fileExtension = Path::getFileExtension( fileName );
                    if( extension == fileExtension )
                    {
                        files.push_back( file );
                    }
                }
            }
        }
        if( const auto p = getFolderArchivesPtr() )
        {
            const auto &archives = *p;
            for( auto &archive : archives )
            {
                auto archiveFiles = archive->getFiles();
                for( auto &file : archiveFiles )
                {
                    if( file.path == path || file.path.find( path ) != String::npos )
                    {
                        const auto &fileName = file.filePath;
                        auto fileExtension = Path::getFileExtension( fileName );
                        if( extension == fileExtension )
                        {
                            files.push_back( file );
                        }
                    }
                }
            }
        }
        if( const auto p = getFileArchivesPtr() )
        {
            const auto &archives = *p;
            for( auto &archive : archives )
            {
                auto archiveFiles = archive->getFiles();
                for( auto &file : archiveFiles )
                {
                    if( file.path == path || file.path.find( path ) != String::npos )
                    {
                        const auto &fileName = file.filePath;
                        auto fileExtension = Path::getFileExtension( fileName );
                        if( extension == fileExtension )
                        {
                            files.push_back( file );
                        }
                    }
                }
            }
        }
        return files;
    }

    auto FileSystem::getFileNamesWithExtension( const String &extension ) const -> Array<String>
    {
        Array<String> fileNames;
        fileNames.reserve( 32 );

        getFileNamesWithExtension( extension, fileNames );
        return fileNames;
    }

    void FileSystem::getFileNamesWithExtension( const String &extension, Array<String> &fileNames ) const
    {
        if( auto pFiles = getFilesPtr() )
        {
            auto &files = *pFiles;

            for( auto &file : files )
            {
                auto fileName = file.filePath;
                auto fileExtension = Path::getFileExtension( fileName );

                if( extension == fileExtension )
                {
                    fileNames.push_back( fileName );
                }
            }
        }

        if( const auto p = getFolderArchivesPtr() )
        {
            const auto &archives = *p;
            for( auto &archive : archives )
            {
                auto archiveFiles = archive->getFiles();

                for( auto &file : archiveFiles )
                {
                    const auto &fileName = file.filePath;
                    auto fileExtension = Path::getFileExtension( fileName );

                    if( extension == fileExtension )
                    {
                        fileNames.push_back( fileName );
                    }
                }
            }
        }

        if( const auto p = getFileArchivesPtr() )
        {
            const auto &archives = *p;
            for( auto &archive : archives )
            {
                auto archiveFiles = archive->getFiles();

                for( auto &file : archiveFiles )
                {
                    const auto &fileName = file.filePath;
                    auto fileExtension = Path::getFileExtension( fileName );

                    if( extension == fileExtension )
                    {
                        fileNames.push_back( fileName );
                    }
                }
            }
        }
    }

    auto FileSystem::isInSameDirectory( const String &path, const String &file ) -> s32
    {
        size_t subA = 0;
        size_t subB = 0;
        size_t pos;

        if( path.length() && path != file )
        {
            return -1;
        }

        pos = 0;
        while( ( pos = path.find( '/', pos ) ) >= 0 )
        {
            subA += 1;
            pos += 1;
        }

        pos = 0;
        while( ( pos = file.find( '/', pos ) ) >= 0 )
        {
            subB += 1;
            pos += 1;
        }

        return static_cast<s32>( subB - subA );
    }

    auto FileSystem::getFileDir( const String &filename ) const -> String
    {
        FileInfo info;

        if( findFileInfo( filename, info ) )
        {
            return String( info.path.c_str() );
        }

        return String( "" );
    }

    void FileSystem::addFolder( SmartPtr<IFolderExplorer> parent )
    {
        if( parent )
        {
            auto folderName = parent->getFolderName();
            addArchive( folderName, ArchiveType::Folder );

            auto subFolders = parent->getSubFolders();
            for( auto &subFolder : subFolders )
            {
                addFolder( subFolder );
            }
        }
    }

    void FileSystem::addFolder( const String &folderPath, bool recursive )
    {
        if( recursive )
        {
            if( auto folderListing = getFolderListing( folderPath ) )
            {
                addFolder( folderListing );
            }
        }
        else
        {
            addArchive( folderPath, ArchiveType::Folder );
        }
    }

    void FileSystem::addArchive( const String &filePath )
    {
        auto ext = Path::getFileExtension( filePath );
        auto type = getTypeFromTypeName( ext );
        addArchive( filePath, type );
    }

    void FileSystem::addArchive( const String &filePath, const String &typeName )
    {
        auto type = getTypeFromTypeName( typeName );
        addArchive( filePath, type );
    }

    void FileSystem::addArchive( const String &filePath, ArchiveType type )
    {
        try
        {
            FB_ASSERT( IArchive::typeInfo() != 0 );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto pThis = getSharedFromThis<FileSystem>();
            FB_ASSERT( pThis );

            switch( type )
            {
            case ArchiveType::Folder:
            case ArchiveType::FileSystem:
            {
                auto a = factoryManager->make_ptr<FileSystemArchive>();
                a->setPath( filePath );
                addFolderArchive( a );
                a->load( nullptr );
            }
            break;
            case ArchiveType::Zip:
            {
                auto a = factoryManager->make_ptr<ZipArchive>( filePath, true, true );
                addFileArchive( a );
                a->load( nullptr );
            }
            break;
            case ArchiveType::ObfuscatedZip:
            {
                auto a = factoryManager->make_ptr<ObfuscatedZipArchive>( filePath, true, true );
                addFileArchive( a );
                a->load( nullptr );
            }
            break;
            case ArchiveType::Unknown:
            {
            }
            break;
            default:
            {
            }
            break;
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto FileSystem::removeFileListener( SmartPtr<IFileListener> fileListener ) -> bool
    {
        return false;
    }

    void FileSystem::addFileListener( SmartPtr<IFileListener> fileListener )
    {
    }

    auto FileSystem::getFileArchive( u32 index ) -> SmartPtr<IArchive>
    {
        return nullptr;
    }

    auto FileSystem::moveFileArchive( u32 sourceIndex, s32 relative ) -> bool
    {
        return NULL;
    }

    auto FileSystem::removeFileArchive( const String &filename ) -> bool
    {
        return false;
    }

    auto FileSystem::removeFileArchive( u32 index ) -> bool
    {
        return false;
    }

    auto FileSystem::getFileArchiveCount() const -> u32
    {
        return 0;
    }

    auto FileSystem::readAllBytes( const String &path ) -> Array<u8>
    {
        return {};
    }

    auto FileSystem::readAllText( const String &path ) -> String
    {
        if( Path::isPathAbsolute( path ) )
        {
            auto stream = new std::fstream( path, std::fstream::in );
            if( stream->is_open() )
            {
                auto pStream = fb::make_ptr<FileDataStream>();
                pStream->setFStream( stream );

                return pStream->getAsString();
            }
            else
            {
                delete stream;
            }
        }

        auto stream = open( path, true, false, false, false, false );
        if( !stream )
        {
            stream = open( path, true, false, false, true, true );
        }

        if( stream )
        {
            return stream->getAsString();
        }

        return String( "" );
    }

    void FileSystem::writeAllBytes( const String &path, u8 *bytes, u32 size )
    {
        try
        {
            auto filePath = String();

            if( Path::isPathAbsolute( path ) )
            {
                filePath = path;
            }
            else
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                filePath = Path::lexically_relative( projectPath, path );
            }

            std::fstream fs;
            fs.open( filePath, std::fstream::binary | std::fstream::out | std::fstream::trunc );

            fs.write( reinterpret_cast<const char *>( bytes ), size );

            fs.close();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FileSystem::writeAllBytes( const String &path, Array<u8> bytes )
    {
        writeAllBytes( path, bytes.data(), (u32)bytes.size() );
    }

    void FileSystem::writeAllText( const String &path, const String &contents )
    {
        try
        {
            auto p = Path::getFilePath( path );
            if( !StringUtil::isNullOrEmpty( p ) )
            {
                if( !isExistingFolder( p ) )
                {
                    createDirectories( p );
                }
            }

            if( Path::isPathAbsolute( path ) )
            {
                std::fstream fs;
                fs.open( path, std::fstream::out | std::fstream::trunc );

                fs << contents;

                fs.close();
            }
            else
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                auto filePath = Path::lexically_normal( projectPath, path );

                std::fstream fs;
                fs.open( filePath, std::fstream::out | std::fstream::trunc );

                fs << contents;

                fs.close();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto FileSystem::getBase64String( SmartPtr<IStream> &pStream ) -> String
    {
        std::string tempData;
        tempData.reserve( 4096 );

        char buf[4096];
        size_t bytes_read = pStream->read( buf, sizeof( buf ) );
        while( bytes_read > 0 )
        {
            tempData.append( buf, bytes_read );
            bytes_read = pStream->read( buf, sizeof( buf ) );
        }

        return StringUtil::encodeBase64( (u8 *)tempData.c_str(), tempData.length() );
    }

    auto FileSystem::getBase64String( std::ifstream &is ) -> String
    {
        std::string tempData;
        tempData.reserve( 4096 );

        char buf[4096];
        while( is.read( buf, sizeof( buf ) ).gcount() > 0 )
        {
            tempData.append( buf, static_cast<size_t>( is.gcount() ) );
        }

        return StringUtil::encodeBase64( (u8 *)tempData.c_str(), tempData.length() );
    }

    auto FileSystem::getBytesString( SmartPtr<IStream> &pStream ) -> String
    {
        constexpr int bufferSize = 4096;

        std::string tempData;
        tempData.reserve( bufferSize );

        char buf[bufferSize];
        size_t bytes_read = pStream->read( buf, sizeof( buf ) );
        while( bytes_read > 0 )
        {
            tempData.append( buf, bytes_read );
            bytes_read = pStream->read( buf, sizeof( buf ) );
        }

        return tempData;
    }

    auto FileSystem::getBytesString( std::ifstream &is ) -> String
    {
        constexpr int bufferSize = 4096;

        std::string tempData;
        tempData.reserve( bufferSize );

        char buf[bufferSize];
        while( is.read( buf, sizeof( buf ) ).gcount() > 0 )
        {
            tempData.append( buf, static_cast<size_t>( is.gcount() ) );
        }

        return tempData;
    }

    void FileSystem::copyFolder( const String &srcPath, const String &dstPath )
    {
        Path::copyFolder( srcPath, dstPath );
    }

    void FileSystem::copyFile( const String &srcPath, const String &dstPath )
    {
        Path::copyFile( srcPath, dstPath );
    }

    void FileSystem::deleteFile( const String &filePath )
    {
        if( Path::isPathAbsolute( filePath ) )
        {
            Path::deleteFile( filePath );
        }
        else
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto absolutePath = Path::getAbsolutePath( projectPath, filePath );

            Path::deleteFile( absolutePath );
        }
    }

    auto FileSystem::getFilePath( const String &path ) -> String
    {
        return Path::getFilePath( path );
    }

    auto FileSystem::getFileName( const String &path ) -> String
    {
        return Path::getFileName( path );
    }

    auto FileSystem::getTypeFromTypeName( const String &typeName ) const -> IFileSystem::ArchiveType
    {
        if( typeName == "FileSystem" )
        {
            return ArchiveType::FileSystem;
        }
        if( typeName == "OBFUSZIP" )
        {
            return ArchiveType::ObfuscatedZip;
        }

        return ArchiveType::Unknown;
    }

    auto FileSystem::getFileHash( const String &pFilePath ) -> String
    {
        // std::fstream is(filePath, std::ios::in | std::ios::binary);
        // if (is.is_open())
        //{
        //	auto stream = SmartPtr<IStream>(new Ogre::FileStreamDataStream(&is, false));
        //	if (stream->isReadable())
        //	{
        //		unsigned char digest[SHA256::DIGEST_SIZE];
        //		memset(digest, 0, SHA256::DIGEST_SIZE);

        //		SHA256 ctx = SHA256();
        //		ctx.init();

        //		constexpr int bufferSize = 1024 * 10;
        //		char buffer[bufferSize];
        //		memset(buffer, 0, bufferSize);

        //		while (!stream->eof())
        //		{
        //			size_t bytesRead = stream->read(buffer, bufferSize);
        //			if (bytesRead == 0)
        //			{
        //				break;
        //			}

        //			ctx.update((unsigned char*)buffer, bytesRead);
        //			memset(buffer, 0, bufferSize);
        //		}

        //		ctx.final(digest);

        //		char buf[2 * SHA256::DIGEST_SIZE + 1];
        //		buf[2 * SHA256::DIGEST_SIZE] = 0;
        //		for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
        //			sprintf(buf + i * 2, "%02x", digest[i]);

        //		return std::string(buf);
        //	}
        //}

        return "";
    }

    auto FileSystem::getFolders( const String &path, bool recursive ) -> Array<String>
    {
        return Path::getFolders( path, recursive );
    }

    auto FileSystem::getFoldersW( const StringW &path, bool recursive ) -> Array<StringW>
    {
        try
        {
#if FB_USE_BOOST
            Array<StringW> files;
            files.reserve( 1024 );

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
#else
            return Array<StringW>();
#endif
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return {};
    }

    auto FileSystem::getFiles( const std::string &path, bool partialPathMatch ) -> Array<String>
    {
        Array<String> results;

        try
        {
            if( auto pFiles = getFilesPtr() )
            {
                auto &files = *pFiles;

                for( auto &file : files )
                {
                    if( !file.isDirectory )
                    {
                        if( file.path == path ||
                            ( partialPathMatch && file.path.find( path ) != String::npos ) )
                        {
                            results.push_back( file.filePath );
                        }
                    }
                }
            }

            if( auto p = getFolderArchivesPtr() )
            {
                auto &archives = *p;

                for( auto &archive : archives )
                {
                    if( archive )
                    {
                        auto fileList = archive->getFileList();
                        if( auto pFiles = fileList->getFilesPtr() )
                        {
                            auto &files = *pFiles;
                            for( auto &file : files )
                            {
                                if( !file.isDirectory )
                                {
                                    if( file.path == path ||
                                        ( partialPathMatch && file.path.find( path ) != String::npos ) )
                                    {
                                        results.push_back( file.filePath );
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if( const auto p = getFileArchivesPtr() )
            {
                const auto &fileArchives = *p;
                for( auto archive : fileArchives )
                {
                    if( archive )
                    {
                        auto fileList = archive->getFileList();
                        if( auto pFiles = fileList->getFilesPtr() )
                        {
                            auto &files = *pFiles;
                            for( auto &file : files )
                            {
                                if( !file.isDirectory )
                                {
                                    if( file.path == path ||
                                        ( partialPathMatch && file.path.find( path ) != String::npos ) )
                                    {
                                        results.push_back( file.filePath );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        catch( const std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return results;
    }

    auto FileSystem::getFilesPtr() const -> SharedPtr<ConcurrentArray<FileInfo>>
    {
        return m_files;
    }

    auto FileSystem::getFolderArchivesPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IArchive>>>
    {
        return m_folderArchives;
    }

    void FileSystem::setFolderArchivesPtr( SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> p )
    {
        m_folderArchives = p;
    }

    auto FileSystem::getFileArchivesPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IArchive>>>
    {
        return m_fileArchives;
    }

    auto FileSystem::getFiles() const -> Array<FileInfo>
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            return Array<FileInfo>( files.begin(), files.end() );
        }

        return {};
    }

    void FileSystem::setFiles( const Array<FileInfo> &files )
    {
        auto p = fb::make_shared<ConcurrentArray<FileInfo>>();
        *p = ConcurrentArray<FileInfo>( files.begin(), files.end() );
        m_files = p;
    }

    void FileSystem::addFilePtr( const FileInfo &file )
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            files.push_back( file );
        }
        else
        {
            auto newPtr = fb::make_shared<ConcurrentArray<FileInfo>>();
            auto &newFiles = *newPtr;
            newFiles.reserve( 1024 );
            newFiles.push_back( file );

            m_files = newPtr;
        }
    }

    void FileSystem::addFolderArchive( SmartPtr<IArchive> archive )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto p = getFolderArchivesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IArchive>>>();
            setFolderArchivesPtr( p );
        }

        if( p )
        {
            auto &archives = *p;
            archives.push_back( archive );
        }
    }

    void FileSystem::removeFolderArchive( SmartPtr<IArchive> archive )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto p = getFolderArchivesPtr() )
        {
            auto &archives = *p;

            auto newPtr = fb::make_shared<ConcurrentArray<SmartPtr<IArchive>>>();

            auto newFiles = Array<SmartPtr<IArchive>>( archives.begin(), archives.end() );

            auto it = std::find( newFiles.begin(), newFiles.end(), archive );
            if( it != newFiles.end() )
            {
                newFiles.erase( it );
            }

            *newPtr = ConcurrentArray<SmartPtr<IArchive>>( newFiles.begin(), newFiles.end() );

            m_folderArchives = newPtr;
        }
    }

    void FileSystem::addFileArchive( SmartPtr<IArchive> archive )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto p = getFileArchivesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IArchive>>>();
            m_fileArchives = p;
        }

        if( p )
        {
            auto &archives = *p;
            archives.push_back( archive );
        }
    }

    void FileSystem::removeFileArchive( SmartPtr<IArchive> archive )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto p = getFileArchivesPtr() )
        {
            auto &archives = *p;

            auto newPtr = fb::make_shared<ConcurrentArray<SmartPtr<IArchive>>>();

            auto newFiles = Array<SmartPtr<IArchive>>();
            newFiles.insert( newFiles.end(), archives.begin(), archives.end() );

            auto it = std::find( newFiles.begin(), newFiles.end(), archive );
            if( it != newFiles.end() )
            {
                newFiles.erase( it );
            }

            m_fileArchives = newPtr;
        }
    }

    auto FileSystem::getFilesAsAbsolutePaths( const String &path, bool recursive ) -> Array<String>
    {
#if FB_USE_BOOST
        Array<std::string> files;

        boost::filesystem::path dir = path;
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                files.push_back( entry.path().string() );
            }
        }

        return files;
#else
        return Array<String>();
#endif
    }

    auto FileSystem::getFileNamesWithExtension( const String &path, const String &extension )
        -> Array<String>
    {
#if FB_USE_BOOST
        auto fileWithExtension = Array<String>();
        auto files = getFiles( path, false );
        for( auto &file : files )
        {
            if( file.find( extension ) != String::npos )
            {
                fileWithExtension.push_back( file );
            }
        }

        return fileWithExtension;
#else
        return Array<String>();
#endif
    }

    auto FileSystem::getFilesW( const StringW &path ) -> Array<StringW>
    {
        return {};
    }

    auto FileSystem::getFilesAsAbsolutePathsW( const StringW &path, bool recursive ) -> Array<StringW>
    {
        return {};
    }

    auto FileSystem::getFilesW( const StringW &path, const StringW &extension ) -> Array<StringW>
    {
#if FB_USE_BOOST
        Array<StringW> files;

        boost::filesystem::path dir = ".";
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
        {
            if( !is_directory( entry ) )
            {
                files.push_back( entry.path().wstring() );
            }
        }

        return files;
#else
        return Array<StringW>();
#endif
    }

    auto FileSystem::getFolderListing( const String &path ) -> SmartPtr<IFolderExplorer>
    {
        try
        {
            auto listing = fb::make_ptr<DirectoryListing>();

            auto cleanPath = StringUtil::cleanupPath( path );
            listing->setFolderName( cleanPath );

            auto folders = Path::getFolders( cleanPath );
            for( auto &folder : folders )
            {
                auto directoryListing = getFolderListing( folder );
                listing->addSubFolder( directoryListing );
            }

            auto files = Path::getFiles( cleanPath );
            listing->setFiles( files );

            return listing;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto FileSystem::getFolderListing( const String &path, const String &ext )
        -> SmartPtr<IFolderExplorer>
    {
        try
        {
            auto listing = fb::make_ptr<DirectoryListing>();

            listing->setFolderName( path );

            auto folders = Path::getFolders( path );
            for( auto &folder : folders )
            {
                auto directoryListing = getFolderListing( folder );
                listing->addSubFolder( directoryListing );
            }

            auto files = Path::getFiles( path );
            listing->setFiles( files );

            return listing;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto FileSystem::getFolderListingW( const StringW &path ) -> SmartPtr<IFolderExplorerW>
    {
        try
        {
            auto listing = fb::make_ptr<DirectoryListingW>();

            listing->setFolderName( path );

            auto folders = PathW::getFolders( path );
            for( auto &folder : folders )
            {
                auto directoryListing = getFolderListingW( folder );
                listing->addSubFolder( directoryListing );
            }

            auto files = PathW::getFiles( path );
            listing->setFiles( files );

            return listing;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto FileSystem::getFolderListingW( const StringW &path, const StringW &ext )
        -> SmartPtr<IFolderExplorerW>
    {
        try
        {
            auto listing = fb::make_ptr<DirectoryListingW>();

            listing->setFolderName( path );

            auto folders = PathW::getFolders( path );
            for( auto &folder : folders )
            {
                auto directoryListing = getFolderListingW( folder );
                listing->addSubFolder( directoryListing );
            }

            auto files = PathW::getFiles( path );
            listing->setFiles( files );

            return listing;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void FileSystem::addFile( const FileInfo &file )
    {
        //#ifdef _DEBUG
        //		FB_LOG(String("File added to file system filePath: ") + file.filePath.c_str());
        //#endif

        addFilePtr( file );
    }

    void FileSystem::addFiles( const Array<FileInfo> &newFiles )
    {
        //#ifdef _DEBUG
        //		for(auto file : files)
        //		{
        //			FB_LOG(String("File added to file system filePath: ") + file.filePath.c_str());
        //		}
        //#endif

        if( auto p = getFilesPtr() )
        {
            auto &files = *p;

            auto newPtr = fb::make_shared<ConcurrentArray<FileInfo>>();

            auto newFileArray = Array<FileInfo>( newFiles.begin(), newFiles.end() );
            newFileArray.insert( newFileArray.end(), files.begin(), files.end() );
            *newPtr = ConcurrentArray<FileInfo>( newFileArray.begin(), newFileArray.end() );

            m_files = newPtr;
        }
        else if( !newFiles.empty() )
        {
            auto newPtr = fb::make_shared<ConcurrentArray<FileInfo>>();

            auto newFileArray = Array<FileInfo>( newFiles.begin(), newFiles.end() );
            *newPtr = ConcurrentArray<FileInfo>( newFileArray.begin(), newFileArray.end() );

            m_files = newPtr;
        }
    }

    void FileSystem::removeFile( const FileInfo &file )
    {
        auto currentFiles = getFiles();
        auto it = std::find( currentFiles.begin(), currentFiles.end(), file );
        if( it != currentFiles.end() )
        {
            currentFiles.erase( it );
        }

        setFiles( currentFiles );
    }

    void FileSystem::removeFiles( const Array<FileInfo> &files )
    {
        for( auto file : files )
        {
            removeFile( file );
        }
    }

    auto FileSystem::findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath ) const -> bool
    {
        if( auto pFiles = getFilesPtr() )
        {
            auto &files = *pFiles;
            for( auto &file : files )
            {
                if( file.fileId == id )
                {
                    fileInfo = file;
                    return true;
                }
            }
        }

        if( auto p = getFolderArchivesPtr() )
        {
            auto &archives = *p;
            for( auto &a : archives )
            {
                if( a->findFileInfo( id, fileInfo, ignorePath ) )
                {
                    return true;
                }
            }
        }

        if( auto p = getFileArchivesPtr() )
        {
            auto &archives = *p;
            for( auto &a : archives )
            {
                if( a->findFileInfo( id, fileInfo, ignorePath ) )
                {
                    return true;
                }
            }
        }

        return false;
    }

    auto FileSystem::findFileInfo( const String &filePath, FileInfo &fileInfo, bool ignorePath ) const
        -> bool
    {
        if( Path::isPathAbsolute( filePath ) )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto projectPath = applicationManager->getProjectPath();

            auto relativePath = Path::getRelativePath( projectPath, filePath );

            if( auto pFiles = getFilesPtr() )
            {
                auto &files = *pFiles;
                for( auto &file : files )
                {
                    if( file.absolutePath == filePath )
                    {
                        fileInfo = file;
                        return true;
                    }
                }
            }
        }
        else
        {
            if( auto pFiles = getFilesPtr() )
            {
                auto &files = *pFiles;
                for( auto &file : files )
                {
                    if( file.filePath == filePath || file.filePathLowerCase == filePath )
                    {
                        fileInfo = file;
                        return true;
                    }
                }

                auto fileName = Path::getFileName( filePath );
                for( auto &file : files )
                {
                    if( file.fileName == fileName || file.fileNameLowerCase == fileName )
                    {
                        fileInfo = file;
                        return true;
                    }
                }
            }
        }

        if( auto p = getFolderArchivesPtr() )
        {
            auto &archives = *p;
            for( auto &a : archives )
            {
                if( a->findFileInfo( filePath, fileInfo, ignorePath ) )
                {
                    return true;
                }
            }
        }

        if( auto p = getFileArchivesPtr() )
        {
            auto &archives = *p;
            for( auto &a : archives )
            {
                if( a->findFileInfo( filePath, fileInfo, ignorePath ) )
                {
                    return true;
                }
            }
        }

        return false;
    }

    auto FileSystem::getSystemFiles() const -> Array<FileInfo>
    {
        auto p = getFilesPtr();
        if( p )
        {
            auto &files = *p;
            return Array<FileInfo>( files.begin(), files.end() );
        }

        return {};
    }

    void FileSystem::refreshAll( bool async )
    {
        try
        {
            if( auto p = getFolderArchivesPtr() )
            {
                auto &folderArchives = *p;
                for( auto archive : folderArchives )
                {
                    archive->reload( nullptr );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FileSystem::refreshPath( const String &path, bool async )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto absolutePath = Path::lexically_normal( projectPath, path );

            if( auto p = getFolderArchivesPtr() )
            {
                auto &folderArchives = *p;
                for( auto archive : folderArchives )
                {
                    auto archivePath = archive->getPath();
                    if( archivePath == path || archivePath == absolutePath )
                    {
                        archive->reload( nullptr );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto FileSystem::isValid() const -> bool
    {
        return true;
    }
}  // end namespace fb
