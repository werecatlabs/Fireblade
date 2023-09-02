#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/CFileSystem.h>
#include <FBFileSystem/CFileSystemArchive.h>
#include <FBFileSystem/ObfuscatedZipArchive.h>
#include <FBFileSystem/ZipArchive.h>
#include <FBFileSystem/FolderListingT.h>
#include <FBFileSystem/FileDataStream.h>
#include <FBFileSystem/CNativeFileDialog.h>
#include <FBFileSystem/CFileList.h>
#include <FBCore/FBCore.h>

#include <filesystem>
#include <fstream>
#include <stdio.h>

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
    FB_CLASS_REGISTER_DERIVED( fb, CFileSystem, IFileSystem );

    CFileSystem::CFileSystem()
    {
    }

    CFileSystem::~CFileSystem()
    {
        unload( nullptr );
    }

    void CFileSystem::load( SmartPtr<ISharedObject> data )
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

    void CFileSystem::reload( SmartPtr<ISharedObject> data )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        unload( data );
        load( data );
    }

    void CFileSystem::unload( SmartPtr<ISharedObject> data )
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
                }

                m_fileArchives = nullptr;

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    bool CFileSystem::isExistingFile( const String &filePath, bool ignorePath, bool ignoreCase ) const
    {
        try
        {
            if( !Path::hasFileName( filePath ) )
            {
                return false;
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
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto projectPath = applicationManager->getProjectPath();
                    if( StringUtil::isNullOrEmpty( projectPath ) )
                    {
                        projectPath = Path::getWorkingDirectory();
                    }

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

    bool CFileSystem::isExistingFile( const String &path, const String &filePath, bool ignorePath,
                                      bool ignoreCase ) const
    {
        return false;
    }

    bool CFileSystem::isExistingFolder( const String &path ) const
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
            auto applicationManager = core::IApplicationManager::instance();

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

    bool CFileSystem::isExistingFile( const StringW &filePath ) const
    {
        return false;
    }

    bool CFileSystem::isExistingFolder( const StringW &path ) const
    {
        return false;
    }

    void CFileSystem::createDirectories( const String &path )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
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

    void CFileSystem::deleteFilesFromPath( const String &path )
    {
        auto files = Path::getFiles( path );
        for( u32 i = 0; i < files.size(); ++i )
        {
            std::string fileName = files[i];
            std::string filePath = path + std::string( "/" ) + fileName;

            boost::filesystem::path dir( filePath );
            boost::filesystem::remove( dir );
        }

        auto folders = Path::getFolders( path, false );
        for( u32 i = 0; i < folders.size(); ++i )
        {
            try
            {
                std::string folder = folders[i];
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

    void CFileSystem::deleteFilesFromPathW( const StringW &path )
    {
        auto files = PathW::getFiles( path );
        for( u32 i = 0; i < files.size(); ++i )
        {
            StringW fileName = files[i];
            StringW filePath = path + StringW( L"/" ) + fileName;

            boost::filesystem::path dir( filePath );
            boost::filesystem::remove( dir );
        }

        auto folders = PathW::getFolders( path, false );
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
            catch( ... )
            {
                FB_LOG_ERROR( "Unknown exception." );
            }
        }
    }

    void CFileSystem::getFileNamesInFolder( const String &path, Array<String> &fileNames )
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

    void CFileSystem::getSubFolders( const String &path, Array<String> &folderNames )
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
                            folderNames.push_back( fileName );
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

    bool CFileSystem::isFolder( const String &path )
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

    bool CFileSystem::setWorkingDirectory( const String &directory )
    {
#if FB_USE_BOOST
        boost::filesystem::path path( directory );
        current_path( path );
        return true;
#else
        return false;
#endif
    }

    String CFileSystem::getWorkingDirectory()
    {
#if FB_USE_BOOST
        return boost::filesystem::current_path().string();
#else
        return "";
#endif
    }

    bool CFileSystem::addFileArchive( const String &filename, bool ignoreCase /*=true*/,
                                      bool ignorePaths /*=true*/, ArchiveType archiveType /*=0*/,
                                      const String &password /*= StringUtil::EmptyString*/ )
    {
        if( StringUtil::isNullOrEmpty( filename ) )
        {
            FB_EXCEPTION( "Invalid file name." );
        }

        addArchive( filename, archiveType );
        return true;
    }

    SmartPtr<INativeFileDialog> CFileSystem::openFileDialog()
    {
        return fb::make_ptr<CNativeFileDialog>();
    }

    SmartPtr<IStream> CFileSystem::open( const String &filePath )
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

    SmartPtr<IStream> CFileSystem::openW( const StringW &filePath )
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

    SmartPtr<IStream> CFileSystem::open( const String &filePath, bool input, bool binary, bool truncate,
                                         bool ignorePath, bool ignoreCase )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            auto applicationManager = core::IApplicationManager::instance();
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

    String CFileSystem::getAbsolutePath( const String &filename ) const
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

    Array<FileInfo> CFileSystem::getFilesWithExtension( const String &extension ) const
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

    Array<FileInfo> CFileSystem::getFilesWithExtension( const String &path,
                                                        const String &extension ) const
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

    Array<String> CFileSystem::getFileNamesWithExtension( const String &extension ) const
    {
        Array<String> fileNames;
        fileNames.reserve( 32 );

        getFileNamesWithExtension( extension, fileNames );
        return fileNames;
    }

    void CFileSystem::getFileNamesWithExtension( const String &extension,
                                                 Array<String> &fileNames ) const
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

    s32 CFileSystem::isInSameDirectory( const String &path, const String &file )
    {
        size_t subA = 0;
        size_t subB = 0;
        size_t pos;

        if( path.length() && path != file )
            return -1;

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

    String CFileSystem::getFileDir( const String &filename ) const
    {
        FileInfo info;

        if( findFileInfo( filename, info ) )
        {
            return String( info.path.c_str() );
        }

        return String( "" );
    }

    void CFileSystem::addFolder( SmartPtr<IFolderExplorer> parent )
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

    void CFileSystem::addFolder( const String &folderPath, bool recursive )
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

    void CFileSystem::addArchive( const String &filePath )
    {
        auto ext = Path::getFileExtension( filePath );
        auto type = getTypeFromTypeName( ext );
        addArchive( filePath, type );
    }

    void CFileSystem::addArchive( const String &filePath, const String &typeName )
    {
        auto type = getTypeFromTypeName( typeName );
        addArchive( filePath, type );
    }

    void CFileSystem::addArchive( const String &filePath, ArchiveType type )
    {
        try
        {
            FB_ASSERT( IArchive::typeInfo() != 0 );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto pThis = getSharedFromThis<CFileSystem>();
            FB_ASSERT( pThis );

            switch( type )
            {
            case ArchiveType::Folder:
            case ArchiveType::FileSystem:
            {
                auto a = factoryManager->make_ptr<CFileSystemArchive>();
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

    bool CFileSystem::removeFileListener( SmartPtr<IFileListener> fileListener )
    {
        return false;
    }

    void CFileSystem::addFileListener( SmartPtr<IFileListener> fileListener )
    {
    }

    SmartPtr<IArchive> CFileSystem::getFileArchive( u32 index )
    {
        return nullptr;
    }

    bool CFileSystem::moveFileArchive( u32 sourceIndex, s32 relative )
    {
        return NULL;
    }

    bool CFileSystem::removeFileArchive( const String &filename )
    {
        return false;
    }

    bool CFileSystem::removeFileArchive( u32 index )
    {
        return false;
    }

    u32 CFileSystem::getFileArchiveCount() const
    {
        return 0;
    }

    Array<u8> CFileSystem::readAllBytes( const String &path )
    {
        return Array<u8>();
    }

    String CFileSystem::readAllText( const String &path )
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

    void CFileSystem::writeAllBytes( const String &path, Array<u8> bytes )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto filePath = Path::lexically_relative( projectPath, path );

            std::fstream fs;
            fs.open( filePath, std::fstream::out );

            fs.close();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CFileSystem::writeAllText( const String &path, const String &contents )
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
                auto applicationManager = core::IApplicationManager::instance();
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

    String CFileSystem::getBase64String( SmartPtr<IStream> &pStream )
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

    String CFileSystem::getBase64String( std::ifstream &is )
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

    String CFileSystem::getBytesString( SmartPtr<IStream> &pStream )
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

    String CFileSystem::getBytesString( std::ifstream &is )
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

    void CFileSystem::copyFile( const String &srcPath, const String &dstPath )
    {
        Path::copyFile( srcPath, dstPath );
    }

    void CFileSystem::deleteFile( const String &filePath )
    {
        if( Path::isPathAbsolute( filePath ) )
        {
            Path::deleteFile( filePath );
        }
        else
        {
            auto applicationManager = core::IApplicationManager::instance();
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

    String CFileSystem::getFilePath( const String &path )
    {
        return Path::getFilePath( path );
    }

    String CFileSystem::getFileName( const String &path )
    {
        return Path::getFileName( path );
    }

    IFileSystem::ArchiveType CFileSystem::getTypeFromTypeName( const String &typeName ) const
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

    String CFileSystem::getFileHash( const String &pFilePath )
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

    Array<String> CFileSystem::getFolders( const String &path, bool recursive )
    {
        return Path::getFolders( path, recursive );
    }

    Array<StringW> CFileSystem::getFoldersW( const StringW &path, bool recursive )
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

        return Array<StringW>();
    }

    Array<String> CFileSystem::getFiles( const std::string &path, bool partialPathMatch )
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

    SharedPtr<ConcurrentArray<FileInfo>> CFileSystem::getFilesPtr() const
    {
        return m_files;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> CFileSystem::getFolderArchivesPtr() const
    {
        return m_folderArchives;
    }

    void CFileSystem::setFolderArchivesPtr( SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> p )
    {
        m_folderArchives = p;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IArchive>>> CFileSystem::getFileArchivesPtr() const
    {
        return m_fileArchives;
    }

    Array<FileInfo> CFileSystem::getFiles() const
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            return Array<FileInfo>( files.begin(), files.end() );
        }

        return Array<FileInfo>();
    }

    void CFileSystem::setFiles( const Array<FileInfo> &files )
    {
        auto p = fb::make_shared<ConcurrentArray<FileInfo>>();
        *p = ConcurrentArray<FileInfo>( files.begin(), files.end() );
        m_files = p;
    }

    void CFileSystem::addFilePtr( const FileInfo &file )
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

    void CFileSystem::addFolderArchive( SmartPtr<IArchive> archive )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto p = getFolderArchivesPtr();
        if (!p)
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

    void CFileSystem::removeFolderArchive( SmartPtr<IArchive> archive )
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

    void CFileSystem::addFileArchive( SmartPtr<IArchive> archive )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto p = getFileArchivesPtr();
        if (!p)
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

    void CFileSystem::removeFileArchive( SmartPtr<IArchive> archive )
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

    Array<String> CFileSystem::getFilesAsAbsolutePaths( const String &path, bool recursive )
    {
#if FB_USE_BOOST
        Array<std::string> files;

        boost::filesystem::path dir = path;
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
            if( !is_directory( entry ) )
                files.push_back( entry.path().string() );

        return files;
#else
        return Array<String>();
#endif
    }

    Array<String> CFileSystem::getFileNamesWithExtension( const String &path, const String &extension )
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

    Array<StringW> CFileSystem::getFilesW( const StringW &path )
    {
        return Array<StringW>();
    }

    Array<StringW> CFileSystem::getFilesAsAbsolutePathsW( const StringW &path, bool recursive )
    {
        return Array<StringW>();
    }

    Array<StringW> CFileSystem::getFilesW( const StringW &path, const StringW &extension )
    {
#if FB_USE_BOOST
        Array<StringW> files;

        boost::filesystem::path dir = ".";
        boost::filesystem::directory_iterator it( dir ), end;

        for( auto &entry : boost::make_iterator_range( it, end ) )
            if( !is_directory( entry ) )
                files.push_back( entry.path().wstring() );

        return files;
#else
        return Array<StringW>();
#endif
    }

    SmartPtr<IFolderExplorer> CFileSystem::getFolderListing( const String &path )
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

    SmartPtr<IFolderExplorer> CFileSystem::getFolderListing( const String &path, const String &ext )
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

    SmartPtr<IFolderExplorerW> CFileSystem::getFolderListingW( const StringW &path )
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

    SmartPtr<IFolderExplorerW> CFileSystem::getFolderListingW( const StringW &path, const StringW &ext )
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

    void CFileSystem::addFile( const FileInfo &file )
    {
        //#ifdef _DEBUG
        //		FB_LOG(String("File added to file system filePath: ") + file.filePath.c_str());
        //#endif

        addFilePtr( file );
    }

    void CFileSystem::addFiles( const Array<FileInfo> &newFiles )
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

    void CFileSystem::removeFile( const FileInfo &file )
    {
        auto currentFiles = getFiles();
        auto it = std::find( currentFiles.begin(), currentFiles.end(), file );
        if( it != currentFiles.end() )
        {
            currentFiles.erase( it );
        }

        setFiles( currentFiles );
    }

    void CFileSystem::removeFiles( const Array<FileInfo> &files )
    {
        for( auto file : files )
        {
            removeFile( file );
        }
    }

    bool CFileSystem::findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath ) const
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

    bool CFileSystem::findFileInfo( const String &filePath, FileInfo &fileInfo, bool ignorePath ) const
    {
        if( Path::isPathAbsolute( filePath ) )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

    Array<FileInfo> CFileSystem::getSystemFiles() const
    {
        auto p = getFilesPtr();
        if( p )
        {
            auto &files = *p;
            return Array<FileInfo>( files.begin(), files.end() );
        }

        return Array<FileInfo>();
    }

    void CFileSystem::refreshAll( bool async )
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

    void CFileSystem::refreshPath( const String &path, bool async )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();

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

    bool CFileSystem::isValid() const
    {
        return true;
    }
}  // end namespace fb
