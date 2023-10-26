#ifndef FBOgreArchive_h__
#define FBOgreArchive_h__

#include "OgreArchive.h"

namespace fb
{
    class FBOgreArchive : public Ogre::Archive
    {
    public:
        FBOgreArchive( const Ogre::String &name, const Ogre::String &archType );
        ~FBOgreArchive() override;

        void load() override;
        void unload() override;

        Ogre::DataStreamPtr open( const Ogre::String &filename, bool readOnly = true ) const;
        Ogre::DataStreamPtr open( const Ogre::String &filename, bool readOnly = true );

        Ogre::StringVectorPtr list( bool recursive = true, bool dirs = false ) override;
        Ogre::FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false ) override;
        Ogre::StringVectorPtr find( const Ogre::String &pattern, bool recursive = true,
                                    bool dirs = false ) override;

        bool exists( const Ogre::String &filename ) override;
        Ogre::FileInfoListPtr findFileInfo( const Ogre::String &pattern, bool recursive = true,
                                            bool dirs = false ) const;
        Ogre::FileInfoListPtr findFileInfo( const Ogre::String &pattern, bool recursive = true,
                                            bool dirs = false );

        time_t getModifiedTime( const Ogre::String &filename ) override;

        bool isCaseSensitive() const;
    };
}  // end namespace fb
#endif  // FBOgreArchive_h__
