#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Ogre/ResourceLoadingListener.h>
#include <FBCore/FBCore.h>
#include <FBGraphicsOgre/Core/FBOgreDataStream.h>
#include <FBGraphicsOgre/Ogre/OgreMemoryStream.h>
#include <OgreDataStream.h>
#include <OgreResource.h>
#include <OgreResourceManager.h>

namespace fb
{
    namespace render
    {

        bool ResourceLoadingListener::resourceCollision( Ogre::Resource *resource,
                                                         Ogre::ResourceManager *resourceManager )
        {
#ifdef _DEBUG
            auto resourceName = resource->getName();
            auto resourceGroup = resource->getGroup();
            auto message = "Warning: Resource name collision. Resource Name: " + resourceName +
                           " Resource Group Name: " + resourceGroup;

            FB_LOG( message );
#endif

            return false;
        }

        void ResourceLoadingListener::resourceStreamOpened( const Ogre::String &name,
                                                            const Ogre::String &group,
                                                            Ogre::Resource *resource,
                                                            Ogre::DataStreamPtr &dataStream )
        {
#ifdef _DEBUG
            auto message =
                "Resource stream opened. Resource Name: " + name + " Resource Group Name: " + group;

            FB_LOG( message );
#endif
        }

        Ogre::DataStreamPtr ResourceLoadingListener::resourceLoading( const Ogre::String &name,
                                                                      const Ogre::String &group,
                                                                      Ogre::Resource *resource )
        {
#ifdef _DEBUG
            auto debugFileExt = Path::getFileExtension( name );
            if( debugFileExt == ".png" )
            {
                auto stop = 0;
                stop = 0;
            }
#endif

            auto isBinary = true;
            auto fileExt = Path::getFileExtension( name );
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

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();

            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

            auto stream = fileSystem->open( name, true, isBinary, false, false );
            if( !stream )
            {
                stream = fileSystem->open( name, true, isBinary, false, true );
            }

            if( stream )
            {
                if( resource )
                {
                    resource->changeGroupOwnership(
                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
                }

                return Ogre::DataStreamPtr( new FBOgreDataStream( stream ) );
            }

#ifdef _DEBUG
            FB_LOG( "Resource not found: " + name );
#endif

            return Ogre::DataStreamPtr();
        }

        bool ResourceLoadingListener::grouplessResourceExists( const String &name )
        {
#ifdef _DEBUG
            auto fileExt = Path::getFileExtension( name );
            if( fileExt == ".dds" )
            {
                auto stop = 0;
                stop = 0;
            }
#endif

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );
            return fileSystem->isExistingFile( name );
        }

        Ogre::DataStreamPtr ResourceLoadingListener::grouplessResourceLoading( const String &name )
        {
            auto isBinary = true;
            auto fileExt = Path::getFileExtension( name );
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

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

            auto fileName = Path::getFileName( name );

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

        Ogre::DataStreamPtr ResourceLoadingListener::grouplessResourceOpened(
            const String &name, Ogre::Archive *archive, Ogre::DataStreamPtr &dataStream )
        {
#ifdef _DEBUG
            auto fileExt = Path::getFileExtension( name );
            if( fileExt == ".dds" )
            {
                auto stop = 0;
                stop = 0;
            }
#endif

            return dataStream;
        }
    }  // end namespace render
}  // end namespace fb
