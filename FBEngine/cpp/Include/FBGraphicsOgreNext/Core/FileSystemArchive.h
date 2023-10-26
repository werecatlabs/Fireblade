#ifndef FileSystemArchive_h__
#define FileSystemArchive_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <OgreArchive.h>

namespace fb
{
    namespace render
    {

        class FileSystemArchive : public Ogre::Archive
        {
        public:
            FileSystemArchive( const String &name );
            ~FileSystemArchive();

            bool isCaseSensitive( void ) const override;

            void load() override;

            void unload() override;

            Ogre::DataStreamPtr open( const String &filename, bool readOnly = true ) const ;

            Ogre::DataStreamPtr open( const String &filename, bool readOnly = true ) override;

            Ogre::StringVectorPtr list( bool recursive = true, bool dirs = false ) const ;

            Ogre::StringVectorPtr list( bool recursive = true, bool dirs = false ) override;

            Ogre::FileInfoListPtr listFileInfo( bool recursive = true,
                                                bool dirs = false ) const ;

            Ogre::FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false ) override;

            Ogre::StringVectorPtr find( const String &pattern, bool recursive = true,
                                        bool dirs = false ) const ;

            Ogre::StringVectorPtr find( const String &pattern, bool recursive = true,
                                        bool dirs = false ) override;

            bool exists( const String &filename ) const ;

            bool exists( const String &filename ) override;

            time_t getModifiedTime( const String &filename ) const ;

            time_t getModifiedTime( const String &filename ) override;

            Ogre::FileInfoListPtr findFileInfo( const String &pattern, bool recursive = true,
                                                bool dirs = false ) const ;

            Ogre::FileInfoListPtr findFileInfo( const String &pattern, bool recursive = true,
                                                bool dirs = false ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // FileSystemArchive_h__
