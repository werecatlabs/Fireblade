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

    FBOgreArchive::~FBOgreArchive() = default;

    void FBOgreArchive::load()
    {
    }

    void FBOgreArchive::unload()
    {
    }

    auto FBOgreArchive::open( const Ogre::String &filename, bool readOnly /*= true */ ) const
        -> Ogre::DataStreamPtr
    {
        auto applicationManager = core::ApplicationManager::instance();
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

        return {};
    }

    auto FBOgreArchive::open( const Ogre::String &filename, bool readOnly ) -> Ogre::DataStreamPtr
    {
        auto applicationManager = core::ApplicationManager::instance();
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

        return {};
    }

    auto FBOgreArchive::list( bool recursive /*= true*/, bool dirs /*= false */ )
        -> Ogre::StringVectorPtr
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();

        auto path = getName();
        auto files = fileSystem->getFilesAsAbsolutePaths( path, false );
        return Ogre::StringVectorPtr( new Ogre::StringVector( files.begin(), files.end() ) );
    }

    auto FBOgreArchive::listFileInfo( bool recursive /*= true*/, bool dirs /*= false */ )
        -> Ogre::FileInfoListPtr
    {
        return {};
    }

    auto FBOgreArchive::find( const Ogre::String &pattern, bool recursive /*= true*/,
                              bool dirs /*= false */ ) -> Ogre::StringVectorPtr
    {
        return {};
    }

    auto FBOgreArchive::exists( const Ogre::String &filename ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
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

    auto FBOgreArchive::findFileInfo( const Ogre::String &pattern, bool recursive /*= true*/,
                                      bool dirs /*= false */ ) const -> Ogre::FileInfoListPtr
    {
        return {};
    }

    auto FBOgreArchive::findFileInfo( const Ogre::String &pattern, bool recursive, bool dirs )
        -> Ogre::FileInfoListPtr
    {
        return {};
    }

    auto FBOgreArchive::getModifiedTime( const Ogre::String &filename ) -> time_t
    {
        return 0;
    }

    auto FBOgreArchive::isCaseSensitive() const -> bool
    {
        return false;
    }

}  // namespace fb
