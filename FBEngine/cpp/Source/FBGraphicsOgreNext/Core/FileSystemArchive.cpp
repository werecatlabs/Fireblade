#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Core/FileSystemArchive.h>
#include "FBOgreDataStream.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        FileSystemArchive::FileSystemArchive( const String &name ) : Ogre::Archive( name, "filesystem" )
        {
        }

        FileSystemArchive::~FileSystemArchive()
        {
        }

        bool FileSystemArchive::isCaseSensitive( void ) const
        {
            return false;
        }

        void FileSystemArchive::load()
        {
        }

        void FileSystemArchive::unload()
        {
        }

        Ogre::DataStreamPtr FileSystemArchive::open( const String &name,
                                                     bool readOnly /*= true */ ) const
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

            auto applicationManager = core::IApplicationManager::instance();
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

            return Ogre::DataStreamPtr();
        }

        Ogre::DataStreamPtr FileSystemArchive::open( const String &name, bool readOnly /*= true */ )
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

            auto applicationManager = core::IApplicationManager::instance();
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

            return Ogre::DataStreamPtr();
        }

        Ogre::StringVectorPtr FileSystemArchive::list( bool recursive /*= true*/,
                                                       bool dirs /*= false */ ) const
        {
            return Ogre::StringVectorPtr();
        }

        Ogre::StringVectorPtr FileSystemArchive::list( bool recursive /*= true*/,
                                                       bool dirs /*= false */ )
        {
            return Ogre::StringVectorPtr();
        }

        Ogre::FileInfoListPtr FileSystemArchive::listFileInfo( bool recursive /*= true*/,
                                                               bool dirs /*= false */ ) const
        {
            return Ogre::FileInfoListPtr();
        }

        Ogre::FileInfoListPtr FileSystemArchive::listFileInfo( bool recursive /*= true*/,
                                                               bool dirs /*= false */ )
        {
            return Ogre::FileInfoListPtr();
        }

        Ogre::StringVectorPtr FileSystemArchive::find( const String &pattern, bool recursive /*= true*/,
                                                       bool dirs /*= false */ ) const
        {
            return Ogre::StringVectorPtr();
        }

        Ogre::StringVectorPtr FileSystemArchive::find( const String &pattern, bool recursive /*= true*/,
                                                       bool dirs /*= false */ )
        {
            return Ogre::StringVectorPtr();
        }

        bool FileSystemArchive::exists( const String &name ) const
        {
            auto applicationManager = core::IApplicationManager::instance();
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

        bool FileSystemArchive::exists( const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

        time_t FileSystemArchive::getModifiedTime( const String &filename ) const
        {
            return 0;
        }

        time_t FileSystemArchive::getModifiedTime( const String &filename )
        {
            return 0;
        }

        Ogre::FileInfoListPtr FileSystemArchive::findFileInfo( const String &pattern,
                                                               bool recursive /*= true*/,
                                                               bool dirs /*= false */ ) const
        {
            return Ogre::FileInfoListPtr();
        }

        Ogre::FileInfoListPtr FileSystemArchive::findFileInfo( const String &pattern,
                                                               bool recursive /*= true*/,
                                                               bool dirs /*= false */ )
        {
            return Ogre::FileInfoListPtr();
        }

    }  // namespace render
}  // namespace fb
