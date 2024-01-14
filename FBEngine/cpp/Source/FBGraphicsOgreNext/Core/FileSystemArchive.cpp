#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Core/FileSystemArchive.h>
#include "FBOgreDataStream.h"
#include <FBCore/FBCore.h>

namespace fb::render
{

    FileSystemArchive::FileSystemArchive( const String &name ) : Ogre::Archive( name, "filesystem" )
    {
    }

    FileSystemArchive::~FileSystemArchive() = default;

    auto FileSystemArchive::isCaseSensitive() const -> bool
    {
        return false;
    }

    void FileSystemArchive::load()
    {
    }

    void FileSystemArchive::unload()
    {
    }

    auto FileSystemArchive::open( const String &name, bool readOnly /*= true */ ) const
        -> Ogre::DataStreamPtr
    {
        auto isBinary = true;
        auto fileExt = Path::getFileExtension( name );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".gsls" || fileExt == ".hlsl" )
        {
            isBinary = false;
        }

        if( fileExt == ".ttf" )
        {
            isBinary = true;
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

        auto stream = fileSystem->open( name, true, isBinary, false, false );
        if( !stream )
        {
            stream = fileSystem->open( name, true, isBinary, false, true );
        }

        if( stream )
        {
            return Ogre::DataStreamPtr( new FBOgreDataStream( stream ) );
        }

        return {};
    }

    auto FileSystemArchive::open( const String &name, bool readOnly /*= true */ ) -> Ogre::DataStreamPtr
    {
        auto isBinary = true;
        auto fileExt = Path::getFileExtension( name );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".gsls" || fileExt == ".hlsl" )
        {
            isBinary = false;
        }

        if( fileExt == ".ttf" )
        {
            isBinary = true;
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

        auto stream = fileSystem->open( name, true, isBinary, false, false );
        if( !stream )
        {
            stream = fileSystem->open( name, true, isBinary, false, true );
        }

        if( stream )
        {
            return Ogre::DataStreamPtr( new FBOgreDataStream( stream ) );
        }

        return {};
    }

    auto FileSystemArchive::list( bool recursive /*= true*/, bool dirs /*= false */ ) const
        -> Ogre::StringVectorPtr
    {
        return {};
    }

    auto FileSystemArchive::list( bool recursive /*= true*/, bool dirs /*= false */ )
        -> Ogre::StringVectorPtr
    {
        return {};
    }

    auto FileSystemArchive::listFileInfo( bool recursive /*= true*/, bool dirs /*= false */ ) const
        -> Ogre::FileInfoListPtr
    {
        return {};
    }

    auto FileSystemArchive::listFileInfo( bool recursive /*= true*/, bool dirs /*= false */ )
        -> Ogre::FileInfoListPtr
    {
        return {};
    }

    auto FileSystemArchive::find( const String &pattern, bool recursive /*= true*/,
                                  bool dirs /*= false */ ) const -> Ogre::StringVectorPtr
    {
        return {};
    }

    auto FileSystemArchive::find( const String &pattern, bool recursive /*= true*/,
                                  bool dirs /*= false */ ) -> Ogre::StringVectorPtr
    {
        return {};
    }

    auto FileSystemArchive::exists( const String &name ) const -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );
        auto existing = fileSystem->isExistingFile( name, false, false );
        if( !existing )
        {
            existing = fileSystem->isExistingFile( name, true, false );
            if( !existing )
            {
                existing = fileSystem->isExistingFile( name, true, true );
            }
        }

        return existing;
    }

    auto FileSystemArchive::exists( const String &name ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );
        auto existing = fileSystem->isExistingFile( name, false, false );
        if( !existing )
        {
            existing = fileSystem->isExistingFile( name, true, false );
            if( !existing )
            {
                existing = fileSystem->isExistingFile( name, true, true );
            }
        }

        return existing;
    }

    auto FileSystemArchive::getModifiedTime( const String &filename ) const -> time_t
    {
        return 0;
    }

    auto FileSystemArchive::getModifiedTime( const String &filename ) -> time_t
    {
        return 0;
    }

    auto FileSystemArchive::findFileInfo( const String &pattern, bool recursive /*= true*/,
                                          bool dirs /*= false */ ) const -> Ogre::FileInfoListPtr
    {
        return {};
    }

    auto FileSystemArchive::findFileInfo( const String &pattern, bool recursive /*= true*/,
                                          bool dirs /*= false */ ) -> Ogre::FileInfoListPtr
    {
        return {};
    }

}  // namespace fb::render
