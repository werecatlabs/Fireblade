#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CTextureManagerOgre.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgre.h>
#include <FBGraphicsOgre/Wrapper/CVideoTextureOgre.h>
#include <FBGraphicsOgre/Wrapper/CCubemap.h>
#include <FBGraphicsOgre/Wrapper/CRenderTexture.h>
#include <FBCore/FBCore.h>
#include <OgreTextureManager.h>

namespace fb
{
    namespace render
    {
        CTextureManagerOgre::CTextureManagerOgre()
        {
            m_textureListener = fb::make_ptr<TextureListener>();
        }

        CTextureManagerOgre::~CTextureManagerOgre()
        {
            unload( nullptr );
        }

        void CTextureManagerOgre::preUpdate()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            for( auto rt : m_renderTargets )
            {
                rt->preUpdate();
            }
        }

        void CTextureManagerOgre::update()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            for( auto rt : m_renderTargets )
            {
                rt->update();
            }
        }

        void CTextureManagerOgre::postUpdate()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            for( auto rt : m_renderTargets )
            {
                rt->postUpdate();
            }
        }

        void CTextureManagerOgre::load( SmartPtr<ISharedObject> data )
        {
        }

        void CTextureManagerOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                FB_ASSERT( isValid() );

                if( const auto &loadingState = getLoadingState();
                    loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    for( auto &rtt : m_renderTargets )
                    {
                        rtt->unload( nullptr );
                    }

                    m_renderTargets.clear();

                    if( const auto pTextures = getTextures() )
                    {
                        auto &textures = *pTextures;
                        for( auto &t : textures )
                        {
                            t->unload( nullptr );
                        }

                        setTextures( nullptr );
                    }

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<ITexture> CTextureManagerOgre::createManual( const String &name, const String &group,
                                                              u8 texType, u32 width, u32 height,
                                                              u32 depth, s32 num_mips, u8 format,
                                                              s32 usage /*= TU_DEFAULT*/ )
        {
            auto eTexType = static_cast<Ogre::TextureType>( texType );
            auto ePixelFormat = static_cast<Ogre::PixelFormat>( format );

            auto textureManager = Ogre::TextureManager::getSingletonPtr();
            auto tex = textureManager->createManual( name, group, eTexType, width, height, depth,
                                                     num_mips, ePixelFormat, usage );

            auto texture = fb::make_ptr<CTextureOgre>();
            texture->initialise( tex );

            return texture;
        }

        SmartPtr<IResource> CTextureManagerOgre::create( const String &name )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto texture = factoryManager->make_ptr<CTextureOgre>();
                FB_ASSERT( texture );

                texture->addObjectListener( m_textureListener );

                auto handle = texture->getHandle();
                FB_ASSERT( handle );

                handle->setName( name );

                auto uuid = StringUtil::getUUID();
                handle->setUUID( uuid );

                FileInfo fileInfo;
                if( fileSystem->findFileInfo( name, fileInfo, false ) )
                {
                    texture->setFileSystemId( fileInfo.fileId );
                }
                else if( fileSystem->findFileInfo( name, fileInfo, true ) )
                {
                    texture->setFileSystemId( fileInfo.fileId );
                }

                texture->setFilePath( name );

                addTexture( texture );

                return texture;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CTextureManagerOgre::create( const String &uuid, const String &name )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto texture = factoryManager->make_ptr<CTextureOgre>();
                FB_ASSERT( texture );

                texture->addObjectListener( m_textureListener.get() );

                auto handle = texture->getHandle();
                FB_ASSERT( handle );

                handle->setName( name );
                handle->setUUID( uuid );

                FileInfo fileInfo;
                if( fileSystem->findFileInfo( name, fileInfo, false ) )
                {
                    texture->setFileSystemId( fileInfo.fileId );
                }
                else if( fileSystem->findFileInfo( name, fileInfo, true ) )
                {
                    texture->setFileSystemId( fileInfo.fileId );
                }

                texture->setFilePath( name );

                addTexture( texture );

                return texture;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CTextureManagerOgre::load( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                auto textureResource = getByName( name );
                if( textureResource )
                {
                    auto texture = fb::static_pointer_cast<CTextureOgre>( textureResource );
                    texture->load( nullptr );
                    return texture;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CTextureManagerOgre::getByName( const String &name )
        {
            try
            {
                if( auto pTextures = getTextures() )
                {
                    auto textures = *pTextures;
                    for( auto &texture : textures )
                    {
                        auto handle = texture->getHandle();
                        if( handle->getName() == name )
                        {
                            return texture;
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

        SmartPtr<IResource> CTextureManagerOgre::getById( const String &uuid )
        {
            try
            {
                if( auto pTextures = getTextures() )
                {
                    auto textures = *pTextures;
                    for( auto &texture : textures )
                    {
                        auto handle = texture->getHandle();
                        if( handle->getUUID() == uuid )
                        {
                            return texture;
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

        Pair<SmartPtr<IResource>, bool> CTextureManagerOgre::createOrRetrieve( const String &uuid,
                                                                               const String &path,
                                                                               const String &type )
        {
            try
            {
                auto textureResource = getById( uuid );
                if( textureResource )
                {
                    auto texture = fb::static_pointer_cast<CTextureOgre>( textureResource );
                    return std::pair<SmartPtr<IResource>, bool>( texture, false );
                }

                auto texture = create( uuid, path );
                FB_ASSERT( texture );

                return Pair<SmartPtr<IResource>, bool>( texture, true );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return Pair<SmartPtr<IResource>, bool>( nullptr, false );
        }

        Pair<SmartPtr<IResource>, bool> CTextureManagerOgre::createOrRetrieve( const String &path )
        {
            try
            {
                auto textureResource = getByName( path );
                if( textureResource )
                {
                    auto texture = fb::static_pointer_cast<CTextureOgre>( textureResource );
                    return std::pair<SmartPtr<IResource>, bool>( texture, false );
                }

                auto texture = create( path );
                FB_ASSERT( texture );

                return Pair<SmartPtr<IResource>, bool>( texture, true );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return Pair<SmartPtr<IResource>, bool>( nullptr, false );
        }

        void CTextureManagerOgre::saveToFile( const String &filePath, SmartPtr<IResource> resource )
        {
        }

        SmartPtr<IResource> CTextureManagerOgre::loadFromFile( const String &filePath )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

                auto materialName = Path::getFileNameWithoutExtension( filePath );

                auto pTextures = getTextures();
                if( pTextures )
                {
                    auto &textures = *pTextures;
                    for( auto material : textures )
                    {
                        auto handle = material->getHandle();
                        auto currentMaterialName = handle->getName();

                        if( materialName == currentMaterialName )
                        {
                            return material;
                        }
                    }
                }

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto stream = fileSystem->open( filePath );
                if( stream )
                {
                    auto material = create( filePath );

                    FileInfo fileInfo;
                    if( fileSystem->findFileInfo( filePath, fileInfo ) )
                    {
                        auto fileId = fileInfo.fileId;
                        material->setFileSystemId( fileId );
                    }

                    return material;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IVideoTexture> CTextureManagerOgre::createVideoTexture( const String &name )
        {
            auto videoTexture = fb::make_ptr<CVideoTextureOgre>();
            return videoTexture;
        }

        SmartPtr<ICubemap> CTextureManagerOgre::addCubemap()
        {
            auto cubemap = fb::make_ptr<CCubemap>();
            return cubemap;
        }

        SmartPtr<ITexture> CTextureManagerOgre::createRenderTexture()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto texture = factoryManager->make_ptr<CTextureOgre>();
            texture->setUsageFlags( ITexture::TU_RENDERTARGET );

            auto renderTarget = factoryManager->make_ptr<CRenderTexture>();
            renderTarget->setTexture( texture );
            renderTarget->load( nullptr );
            texture->setRenderTarget( renderTarget );

            graphicsSystem->loadObject( texture );

            addTexture( texture );
            addRenderTexture( texture );

            return texture;
        }

        void CTextureManagerOgre::destroyRenderTexture( SmartPtr<ITexture> texture )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( texture )
            {
                texture->unload( nullptr );

                removeTexture( texture );

                auto it = std::find( m_renderTargets.begin(), m_renderTargets.end(), texture );
                if( it != m_renderTargets.end() )
                {
                    m_renderTargets.erase( it );
                }
            }
        }

        SmartPtr<ITexture> CTextureManagerOgre::createSkyBoxCubeMap( Array<String> skyboxTextures )
        {
            return nullptr;
        }

        SmartPtr<ITexture> CTextureManagerOgre::createSkyBoxCubeMap(
            Array<SmartPtr<ITexture>> skyboxTextures )
        {
            return nullptr;
        }

        SmartPtr<ITexture> CTextureManagerOgre::createSkyBoxCubeMap( SmartPtr<IMaterial> material )
        {
            return nullptr;
        }

        void CTextureManagerOgre::_getObject( void **ppObject ) const
        {
        }

        void CTextureManagerOgre::addTexture( SmartPtr<ITexture> texture )
        {
            FB_DEBUG_TRACE;

            auto pTextures = getTextures();
            if( !pTextures )
            {
                pTextures = fb::make_shared<ConcurrentArray<SmartPtr<ITexture>>>();
                setTextures( pTextures );
            }

            if( pTextures )
            {
                auto &textures = *pTextures;
                textures.push_back( texture );
            }
        }

        void CTextureManagerOgre::removeTexture( SmartPtr<ITexture> texture )
        {
            FB_DEBUG_TRACE;

            if( auto pTextures = getTextures() )
            {
                auto &textures = *pTextures;

                auto texturesArray = Array<SmartPtr<ITexture>>( textures.begin(), textures.end() );
                auto it = std::find( texturesArray.begin(), texturesArray.end(), texture );
                if( it != texturesArray.end() )
                {
                    texturesArray.erase( it );
                }

                auto newTextures = fb::make_shared<ConcurrentArray<SmartPtr<ITexture>>>(
                    texturesArray.begin(), texturesArray.end() );
                setTextures( newTextures );
            }
        }

        void CTextureManagerOgre::setTextures( SharedPtr<ConcurrentArray<SmartPtr<ITexture>>> p )
        {
            m_textures = p;
        }

        SharedPtr<ConcurrentArray<SmartPtr<ITexture>>> CTextureManagerOgre::getTextures() const
        {
            return m_textures;
        }

        void CTextureManagerOgre::addRenderTexture( SmartPtr<ITexture> texture )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_renderTargets.push_back( texture );
        }

        void CTextureManagerOgre::removeRenderTexture( SmartPtr<ITexture> texture )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_renderTargets.erase(
                std::remove( m_renderTargets.begin(), m_renderTargets.end(), texture ),
                m_renderTargets.end() );
        }

        void CTextureManagerOgre::createRenderTextures()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_renderTargets.clear();
        }

        Parameter CTextureManagerOgre::TextureListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        void CTextureManagerOgre::TextureListener::loadingStateChanged( ISharedObject *sharedObject,
                                                                        LoadingState oldState,
                                                                        LoadingState newState )
        {
            if( newState == LoadingState::Loaded )
            {
            }
        }

        bool CTextureManagerOgre::TextureListener::destroy( void *ptr )
        {
            return false;
        }
    }  // end namespace render
}  // end namespace fb
