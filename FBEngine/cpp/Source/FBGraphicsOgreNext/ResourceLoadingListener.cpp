#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/ResourceLoadingListener.h>
#include <FBGraphicsOgreNext/FBOgreDataStream.h>
#include <FBGraphicsOgreNext/OgreMemoryStream.h>
#include <FBGraphicsOgreNext/FBOgreDataStream.h>
#include <FBCore/FBCore.h>
#include <OgreDataStream.h>
#include <OgreResource.h>
#include <OgreResourceManager.h>

namespace fb::render
{

    auto ResourceLoadingListener::resourceCollision( Ogre::Resource *resource,
                                                     Ogre::ResourceManager *resourceManager ) -> bool
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

    auto ResourceLoadingListener::resourceLoading( const Ogre::String &name, const Ogre::String &group,
                                                   Ogre::Resource *resource ) -> Ogre::DataStreamPtr
    {
        auto isBinary = true;
        auto fileExt = Path::getFileExtension( name );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".glsl" || fileExt == ".hlsl" )
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

        return {};
    }

    auto ResourceLoadingListener::grouplessResourceExists( const String &name ) -> bool
    {
#ifdef _DEBUG
        auto fileExt = Path::getFileExtension( name );
        if( fileExt == ".dds" )
        {
            auto stop = 0;
            stop = 0;
        }
#endif

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

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto textureManager = graphicsSystem->getTextureManager();
        auto materialManager = graphicsSystem->getMaterialManager();

        if( textureManager->getById( name ) != nullptr )
        {
            existing = true;
        }

        if( materialManager->getById( name ) != nullptr )
        {
            existing = true;
        }

        return existing;
    }

    auto ResourceLoadingListener::grouplessResourceLoading( const String &name ) -> Ogre::DataStreamPtr
    {
        auto isBinary = true;
        auto fileExt = Path::getFileExtension( name );
        if( fileExt == ".compositor" || fileExt == ".material" || fileExt == ".program" ||
            fileExt == ".fontdef" || fileExt == ".glsl" || fileExt == ".hlsl" )
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

    auto ResourceLoadingListener::grouplessResourceOpened( const String &name, Ogre::Archive *archive,
                                                           Ogre::DataStreamPtr &dataStream )
        -> Ogre::DataStreamPtr
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

}  // namespace fb::render
