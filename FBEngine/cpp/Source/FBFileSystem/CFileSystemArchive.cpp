#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/CFileSystemArchive.h>
#include <FBFileSystem/FileDataStream.h>
#include <FBFileSystem/CFileSystem.h>
#include <FBFileSystem/CFileSystemArchive.h>
#include <FBFileSystem/CMemoryFile.h>
#include <FBCore/FBCore.h>
#include <fstream>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, CFileSystemArchive, CSharedObject<IArchive> );

    CFileSystemArchive::CFileSystemArchive( const String &path, bool ignoreCase, bool ignorePaths ) :
        m_path( path )
    {
        setEnableReferenceTracking( true );
    }

    CFileSystemArchive::~CFileSystemArchive()
    {
        unload( nullptr );
    }

    void CFileSystemArchive::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto path = getPath();
            //FB_ASSERT( !StringUtil::isNullOrEmpty( path ) );

            m_fileList = factoryManager->make_ptr<CFileList>();
            m_fileList->setPath( path );
            FB_ASSERT( m_fileList->getReferences() == 1 );

#ifdef _DEBUG
            if( StringUtil::contains( path, "Scripts" ) )
            {
                int stop = 0;
                stop = 0;
            }
#endif

            auto files = Path::getFiles( path );

            auto projectPath = applicationManager->getProjectPath();
            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
            }

            auto workingDirectory = Path::getWorkingDirectory();

            for( const auto &file : files )
            {
                FileInfo fileInfo;

                auto absolutePath = StringUtil::cleanupPath( file );
                auto filePath = Path::getRelativePath( projectPath, absolutePath );

                auto fileName = Path::getFileName( filePath );
                //FB_ASSERT( fileInfo.fileName.capacity() > fileName.size() );
                fileInfo.fileName = fileName;

                auto fileNameLowerCase = StringUtil::make_lower( fileName );
                //FB_ASSERT( fileInfo.fileNameLowerCase.capacity() > fileNameLowerCase.size() );
                fileInfo.fileNameLowerCase = fileNameLowerCase;

                //FB_ASSERT( fileInfo.filePath.capacity() > filePath.size() );
                fileInfo.filePath = filePath;

                auto filePathLowerCase = StringUtil::make_lower( filePath );
                //FB_ASSERT( fileInfo.filePathLowerCase.capacity() > filePathLowerCase.size() );
                fileInfo.filePathLowerCase = filePathLowerCase;

                //FB_ASSERT( fileInfo.absolutePath.capacity() > path.size() );
                fileInfo.path = path;

                //FB_ASSERT( fileInfo.path.capacity() > absolutePath.size() );

                if( !Path::isPathAbsolute( absolutePath ) )
                {
                    absolutePath = Path::lexically_normal( workingDirectory, file );
                }

                FB_ASSERT( Path::isPathAbsolute( absolutePath ) );
                fileInfo.absolutePath = absolutePath;

                fileInfo.isDirectory = false;

                fileInfo.fileId = StringUtil::getHash64( filePath );

#ifdef _DEBUG
                if( StringUtil::contains( absolutePath, "Materials/BackgroundUI.mat" ) )
                {
                    int stop = 0;
                    stop = 0;
                }
#endif

                addFilePtr( fileInfo );
            }

            FB_ASSERT( m_fileList->getReferences() == 1 );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CFileSystemArchive::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            unload( data );
            load( data );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CFileSystemArchive::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                if( m_fileList )
                {
                    m_fileList->setFilesPtr( nullptr );
                    m_fileList->unload( nullptr );
                    m_fileList = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SmartPtr<IFileList> CFileSystemArchive::getFileList() const
    {
        return m_fileList;
    }

    Array<FileInfo> CFileSystemArchive::getFiles() const
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            return Array<FileInfo>( files.begin(), files.end() );
        }

        return Array<FileInfo>();
    }

    u8 CFileSystemArchive::getType() const
    {
        return static_cast<u8>( IFileSystem::ArchiveType::Folder );
    }

    String CFileSystemArchive::getPassword() const
    {
        return m_password;
    }

    void CFileSystemArchive::setPassword( const String &val )
    {
        m_password = val;
    }

    SmartPtr<IStream> CFileSystemArchive::open( const String &filePath, bool input, bool binary,
                                                bool truncate, bool ignorePath )
    {
        try
        {
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

            auto fileId = StringUtil::getHash64( filePath );

            FileInfo fileInfo;
            if( findFileInfo( filePath, fileInfo, ignorePath ) )
            {
                if( !input )
                {
                    auto stream = new std::fstream( fileInfo.absolutePath, f );
                    if( stream->is_open() )
                    {
                        auto pStream = fb::make_ptr<FileDataStream>();
                        pStream->setFileInfo( fileInfo );
                        pStream->setFStream( stream );
                        return pStream;
                    }

                    delete stream;
                }
                else
                {
                    auto stream = new std::ifstream( fileInfo.absolutePath, f );
                    if( stream->is_open() )
                    {
                        auto pStream = fb::make_ptr<FileDataStream>();
                        pStream->setFileInfo( fileInfo );
                        pStream->setInStream( stream );
                        return pStream;
                    }

                    delete stream;
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    bool CFileSystemArchive::exists( const String &filePath, bool ignorePath, bool ignoreCase ) const
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;

            if( ignorePath )
            {
                auto fileName = Path::getFileName( filePath );
                auto fileNameLower = StringUtil::make_lower( fileName );
                for( auto &file : files )
                {
                    if( ignoreCase )
                    {
                        if( file.fileNameLowerCase == fileNameLower )
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if( file.fileName == fileName )
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
                if( Path::isPathAbsolute( filePath ) )
                {
                    auto filePathClean = StringUtil::cleanupPath( filePath );
                    auto filePathLower = StringUtil::make_lower( filePathClean );

                    for( auto &file : files )
                    {
                        if( ignoreCase )
                        {
                            if( StringUtil::make_lower( file.absolutePath ) == filePathLower )
                            {
                                return true;
                            }
                        }
                        else
                        {
                            if( file.absolutePath == filePathClean )
                            {
                                return true;
                            }
                        }
                    }
                }
                else
                {
                    auto filePathLower = StringUtil::make_lower( filePath );
                    for( auto &file : files )
                    {
                        if( ignoreCase )
                        {
                            if( file.filePathLowerCase == filePathLower )
                            {
                                return true;
                            }
                        }
                        else
                        {
                            if( file.filePath == filePath )
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }

        return false;
    }

    bool CFileSystemArchive::isReadOnly() const
    {
        return false;
    }

    String CFileSystemArchive::getPath() const
    {
        return m_path;
    }

    void CFileSystemArchive::setPath( const String &path )
    {
        m_path = path;
        m_path = StringUtil::replaceAll( m_path.c_str(), "\\", "/" );
        m_path = StringUtil::replaceAll( m_path.c_str(), "//", "/" );
    }

    bool CFileSystemArchive::findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath ) const
    {
        if( m_fileList )
        {
            return m_fileList->findFileInfo( id, fileInfo, ignorePath );
        }

        return false;
    }

    bool CFileSystemArchive::findFileInfo( const String &filePath, FileInfo &fileInfo,
                                           bool ignorePath ) const
    {
        if( m_fileList )
        {
            return m_fileList->findFileInfo( filePath, fileInfo, ignorePath );
        }

        return false;
    }

    SharedPtr<ConcurrentArray<FileInfo>> CFileSystemArchive::getFilesPtr() const
    {
        if( m_fileList )
        {
            return m_fileList->getFilesPtr();
        }

        return nullptr;
    }

    void CFileSystemArchive::addFilePtr( const FileInfo &file )
    {
        if( m_fileList )
        {
            m_fileList->addFilePtr( file );
        }
    }

}  // end namespace fb
