#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/FBOgreArchive.h>
#include <FBGraphicsOgreNext/FBOgreDataStream.h>
#include <FBCore/FBCore.h>

namespace fb
{

    FBOgreArchive::FBOgreArchive( const Ogre::String &name, const Ogre::String &archType ) :
        Ogre::Archive( name, archType )
    {
    }

    FBOgreArchive::~FBOgreArchive()
    {
    }

    void FBOgreArchive::load()
    {
    }

    void FBOgreArchive::unload()
    {
    }

    Ogre::DataStreamPtr FBOgreArchive::open( const Ogre::String &filename,
                                             bool readOnly /*= true */ ) const
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto isBinary = true;
        auto fileExt = Path::getFileExtension( filename );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".glsl" || fileExt == ".hlsl" )
        {
            isBinary = false;
        }

        if( fileExt == ".ttf" )
        {
            isBinary = true;
        }

        auto path = getName();
        auto filePath = path + "/" + filename;

        auto stream = fileSystem->open( filename, true, isBinary, false, false );
        if( !stream )
        {
            stream = fileSystem->open( filePath, true, isBinary, false, false );
        }

        if( stream )
        {
            return Ogre::DataStreamPtr( new FBOgreDataStream( stream ) );
        }

        return Ogre::DataStreamPtr();
    }

    Ogre::DataStreamPtr FBOgreArchive::open( const Ogre::String &filename, bool readOnly )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto isBinary = true;
        auto fileExt = Path::getFileExtension( filename );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".glsl" || fileExt == ".hlsl" )
        {
            isBinary = false;
        }

        if( fileExt == ".ttf" )
        {
            isBinary = true;
        }

        auto path = getName();

        auto filePath = String();

        if( !Path::isPathAbsolute( filename ) )
        {
            if( !StringUtil::contains( filename, path ) )
            {
                filePath = Path::lexically_normal( path, filename );
            }
            else
            {
                filePath = filename;
            }
        }
        else
        {
            filePath = filename;
        }

        filePath = StringUtil::cleanupPath( filePath );

        auto stream = fileSystem->open( filePath, true, isBinary, false, false, false );
        if( !stream )
        {
            stream = fileSystem->open( filePath, true, isBinary, false, false, true );
        }

        if( !stream )
        {
            if( !Path::isPathAbsolute( filePath ) )
            {
                auto mediaPath = applicationManager->getMediaPath();
                auto absolutePath = Path::lexically_normal( mediaPath, filePath );
                stream = fileSystem->open( absolutePath, true, isBinary, false, false, true );
            }
            else
            {
                stream = fileSystem->open( filePath, true, isBinary, false, false, true );
            }
        }

        if( stream )
        {
            return Ogre::DataStreamPtr( new FBOgreDataStream( stream ) );
        }

        return Ogre::DataStreamPtr();
    }

    Ogre::StringVectorPtr FBOgreArchive::list( bool recursive /*= true*/, bool dirs /*= false */ )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto path = getName();
        auto files = fileSystem->getFilesAsAbsolutePaths( path, false );
        return Ogre::StringVectorPtr( new Ogre::StringVector( files.begin(), files.end() ) );
    }

    Ogre::FileInfoListPtr FBOgreArchive::listFileInfo( bool recursive /*= true*/,
                                                       bool dirs /*= false */ )
    {
        return Ogre::FileInfoListPtr();
    }

    Ogre::StringVectorPtr FBOgreArchive::find( const Ogre::String &pattern, bool recursive /*= true*/,
                                               bool dirs /*= false */ )
    {
        return Ogre::StringVectorPtr();
    }

    bool FBOgreArchive::exists( const Ogre::String &filename )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto folderName = getName();
        auto filePath = Path::lexically_normal( folderName, filename );

        auto fileExists = fileSystem->isExistingFile( filePath, false, false );
        if( !fileExists )
        {
            fileExists = fileSystem->isExistingFile( filePath, false, true );
        }

        //FB_ASSERT( fileExists == true );

        return fileExists;
    }

    Ogre::FileInfoListPtr FBOgreArchive::findFileInfo( const Ogre::String &pattern,
                                                       bool recursive /*= true*/,
                                                       bool dirs /*= false */ ) const
    {
        return Ogre::FileInfoListPtr();
    }

    Ogre::FileInfoListPtr FBOgreArchive::findFileInfo( const Ogre::String &pattern, bool recursive,
                                                       bool dirs )
    {
        return Ogre::FileInfoListPtr();
    }

    time_t FBOgreArchive::getModifiedTime( const Ogre::String &filename )
    {
        return 0;
    }

    bool FBOgreArchive::isCaseSensitive() const
    {
        return false;
    }

}  // namespace fb
