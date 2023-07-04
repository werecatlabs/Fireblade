#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgre.h>
#include <FBGraphicsOgre/Wrapper/CRenderTargetOgre.h>
#include <FBGraphicsOgre/Wrapper/CRenderTexture.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgreStateListener.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

#if defined FB_PLATFORM_WIN32
#    if FB_BUILD_RENDERER_DX11
#        include <OgreD3D11Texture.h>
#    elif FB_BUILD_RENDERER_DX9
#        include <OgreD3D9Texture.h>
#    elif FB_BUILD_RENDERER_OPENGL
#        include <OgreGLTextureCommon.h>
#    endif
#elif defined FB_PLATFORM_APPLE
#    if FB_BUILD_RENDERER_OPENGL
#        include <OgreGLTextureCommon.h>
#    endif
#endif

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CTextureOgre, CTexture );
        u32 CTextureOgre::m_ext = 0;

        CTextureOgre::CTextureOgre()
        {
            setResourceType( ResourceType::Texture );

            createStateObject();
        }

        CTextureOgre::~CTextureOgre()
        {
            try
            {
                unload( nullptr );
                destroyStateObject();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTextureOgre::update()
        {
            auto usageFlags = getUsageFlags();
            if( ( usageFlags & TU_RENDERTARGET ) != 0 )
            {
                if( auto rt = getRenderTarget() )
                {
                    rt->update();
                }
            }
        }

        void CTextureOgre::save()
        {
            if( m_texture )
            {
                // m_texture->writeContentsToFile( "test.png", 0, 100 );
            }
        }

        void CTextureOgre::saveToFile( const String &filePath )
        {
        }

        void CTextureOgre::loadFromFile( const String &filePath )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

                auto materialName = Path::getFileNameWithoutExtension( filePath );

                // auto pTextures = getTextures();
                // if( pTextures )
                //{
                //     auto &textures = *pTextures;
                //     for( auto material : textures )
                //     {
                //         auto handle = material->getHandle();
                //         auto currentMaterialName = handle->getName();

                //        if( materialName == currentMaterialName )
                //        {
                //            return material;
                //        }
                //    }
                //}

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto stream = fileSystem->open( filePath );
                if( stream )
                {
                    FileInfo fileInfo;
                    if( fileSystem->findFileInfo( filePath, fileInfo ) )
                    {
                        auto fileId = fileInfo.fileId;
                        setFileSystemId( fileId );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTextureOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto usageFlags = getUsageFlags();

                if( ( usageFlags & TU_RENDERTARGET ) != 0 )
                {
                    switch( auto type = getTextureType() )
                    {
                    case TextureType::TEX_TYPE_2D:
                    {
                        auto size = getSize();
                        if( size.x > 0 && size.y > 0 )
                        {
                            auto textureName = String( "RTT_" ) + StringUtil::toString( m_ext++ );
                            auto textureManager = Ogre::TextureManager::getSingletonPtr();

                            if( m_texture )
                            {
                                textureManager->remove( m_texture );
                                m_texture = nullptr;
                            }

                            auto texture = textureManager->createManual(
                                textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                Ogre::TEX_TYPE_2D, size.x, size.y, 0, Ogre::PF_R8G8B8,
                                Ogre::TU_RENDERTARGET );

                            m_texture = texture;

                            auto buffer = m_texture->getBuffer();
                            auto rtt = buffer->getRenderTarget();

#if defined _DEBUG
                            /*
                            auto sceneManager = graphicsSystem->getSceneManager();

                            Ogre::SceneManager *smgr = nullptr;
                            sceneManager->_getObject( (void **)&smgr );

                            auto camera = smgr->createCamera( String( "rtt" ) );

                            auto vp = rtt->addViewport( camera, 10, 0, 0, 0.5, 0.5 );
                            vp->setOverlaysEnabled( false );
                            vp->setAutoUpdated( false );
                            vp->setBackgroundColour( Ogre::ColourValue::Blue );
                            vp->setClearEveryFrame( false );
                             */
#endif

                            if( !m_renderTarget )
                            {
                                auto renderTarget = fb::make_ptr<CRenderTexture>();
                                renderTarget->setRenderTarget( rtt );
                                renderTarget->setTexture( this );
                                renderTarget->load( nullptr );
                                m_renderTarget = renderTarget;
                            }
                            else
                            {
                                auto pRenderTarget =
                                    fb::static_pointer_cast<CRenderTexture>( m_renderTarget );
                                pRenderTarget->setRenderTarget( rtt );
                                pRenderTarget->setTexture( this );
                                //pRenderTarget->load( nullptr );
                            }

                            auto handle = m_texture->getHandle();
                            m_textureHandle = handle;
                        }
                    }
                    break;
                    }
                }
                else
                {
                    switch( auto type = getTextureType() )
                    {
                    case TextureType::TEX_TYPE_2D:
                    {
                        auto textureManager = Ogre::TextureManager::getSingletonPtr();

                        auto id = getFileSystemId();

                        auto filePath = getFilePath();
                        if( !StringUtil::isNullOrEmpty( filePath ) )
                        {
                            try
                            {
                                auto resourceGroup =
                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
                                m_texture = textureManager->load( filePath, resourceGroup );
                            }
                            catch( std::exception &e )
                            {
                                FB_LOG_EXCEPTION( e );
                            }
                        }
                        else if( id != 0 )
                        {
                            FileInfo fileInfo;
                            if( fileSystem->findFileInfo( id, fileInfo ) )
                            {
                                try
                                {
                                    const auto &filePath = fileInfo.absolutePath;
                                    auto resourceGroup =
                                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
                                    m_texture = textureManager->load( filePath, resourceGroup );
                                }
                                catch( std::exception &e )
                                {
                                    FB_LOG_EXCEPTION( e );
                                }
                            }
                        }
                    }
                    break;
                    case TextureType::TEX_TYPE_CUBE_MAP:
                    {
                        try
                        {
                            auto textureManager = Ogre::TextureManager::getSingletonPtr();

                            auto resourceName = "cubemap.jpg";  // placeholder //getHandle()->getName();
                            auto textureResult = textureManager->createOrRetrieve(
                                resourceName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                false, nullptr, nullptr, Ogre::TEX_TYPE_CUBE_MAP );

                            m_texture = Ogre::dynamic_pointer_cast<Ogre::Texture>( textureResult.first );
                        }
                        catch( std::exception &e )
                        {
                            FB_LOG_EXCEPTION( e );
                        }
                    }
                    break;
                    default:
                    {
                    }
                    };

                    if( m_texture )
                    {
                        auto handle = m_texture->getHandle();
                        m_textureHandle = handle;
                    }
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTextureOgre::reload( SmartPtr<ISharedObject> data )
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

        void CTextureOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    if( auto renderTarget = getRenderTarget() )
                    {
                        renderTarget->unload( nullptr );
                        setRenderTarget( nullptr );
                    }

                    if( m_texture )
                    {
                        auto textureManager = Ogre::TextureManager::getSingletonPtr();
                        if( textureManager )
                        {
                            textureManager->remove( m_texture );
                            m_texture = nullptr;
                        }
                    }

                    CTexture::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
                else if( loadingState == LoadingState::Allocated )
                {
                    destroyStateObject();
                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTextureOgre::initialise( Ogre::TexturePtr &texture )
        {
            m_texture = texture;
        }

        void CTextureOgre::copyToTexture( SmartPtr<ITexture> &target )
        {
            // SmartPtr<CTextureOgre> other = target;

            // m_texture->copyToTexture(other->m_texture);
        }

        void CTextureOgre::copyData( void *data, const Vector2I &size )
        {
        }

        Vector2I CTextureOgre::getSize() const
        {
            if( auto context = getStateObject() )
            {
                if( auto state = context->getStateByType<TextureState>() )
                {
                    return state->getSize();
                }
            }

            return Vector2I::zero();
        }

        void CTextureOgre::setSize( const Vector2I &size )
        {
            if( auto context = getStateObject() )
            {
                if( auto state = context->getStateByType<TextureState>() )
                {
                    state->setSize( size );
                }
            }
        }

        void CTextureOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_texture.get();
        }

        Vector2I CTextureOgre::getActualSize() const
        {
            return Vector2I::zero();
        }

        void CTextureOgre::getTextureGPU( void **ppTexture ) const
        {
            if( auto texture = getTexture() )
            {
                auto p = texture.getPointer();
                *ppTexture = p;
            }
        }

        void CTextureOgre::getTextureFinal( void **ppTexture ) const
        {
            if( !m_texture )
            {
                return;
            }

#if defined FB_PLATFORM_WIN32
#    if FB_BUILD_RENDERER_DX11
            auto texture = m_texture.dynamicCast<Ogre::D3D11Texture>();
            FB_ASSERT( texture );

            auto textureResource = texture->getTextureResource();
            *ppTexture = textureResource;
#    elif FB_BUILD_RENDERER_DX9
            Ogre::RenderTarget *rtt = m_texture->getBuffer()->getRenderTarget();

            void *t = nullptr;
            rtt->getCustomAttribute( "DDBACKBUFFER", &t );
            *ppTexture = t;

            auto glTexture = m_texture.dynamicCast<Ogre::D3D9Texture>();
            auto tex = glTexture->getTexture();
            *ppTexture = tex;

#    elif FB_BUILD_RENDERER_OPENGL
            auto glTex = m_texture.dynamicCast<Ogre::GLTextureCommon>();
            auto id = glTex->getGLID();
            //*ppTexture = (void *)(*static_cast<u32 *>( &id ));
            m_textureId = id;
            *ppTexture = &m_textureId;
#    endif
#elif defined FB_PLATFORM_APPLE
#    if FB_BUILD_RENDERER_OPENGL
            auto glTex = m_texture.dynamicCast<Ogre::GLTextureCommon>();
            auto id = glTex->getGLID();
            //*ppTexture = (void *)(*static_cast<u32 *>( &id ));
            m_textureId = id;
            *ppTexture = &m_textureId;
#    endif
#endif
        }

        size_t CTextureOgre::getTextureHandle() const
        {
#if 0
            if( m_textureHandle.load() == 0 )
            {
                if( m_texture )
                {
                    auto handle = m_texture->getHandle();

#if defined FB_PLATFORM_APPLE
                    getTextureFinal( (void **)&handle );
#endif

                    m_textureHandle = handle;
                }
            }
#endif

            return m_textureHandle;
        }

        u32 CTextureOgre::getUsageFlags() const
        {
            return m_usageFlags;
        }

        void CTextureOgre::setUsageFlags( u32 usageFlags )
        {
            m_usageFlags = usageFlags;
        }

        SmartPtr<Properties> CTextureOgre::getProperties() const
        {
            if( auto properties = CResourceGraphics<ITexture>::getProperties() )
            {
                static const auto materialTypeStr = String( "Material Type" );

                auto size = getSize();
                properties->setProperty( "Size", size );

                return properties;
            }

            return nullptr;
        }

        void CTextureOgre::setProperties( SmartPtr<Properties> properties )
        {
            CResourceGraphics<ITexture>::setProperties( properties );
        }

        Array<SmartPtr<ISharedObject>> CTextureOgre::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 4 );

            if( auto renderTarget = getRenderTarget() )
            {
                objects.push_back( renderTarget );
            }

            return objects;
        }

        Ogre::TexturePtr CTextureOgre::getTexture() const
        {
            return m_texture;
        }

        void CTextureOgre::createStateObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto factoryManager = applicationManager->getFactoryManager();

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto stateObject = stateManager->addStateObject();
            FB_ASSERT( stateObject );

            auto stateListener = factoryManager->make_ptr<CTextureOgreStateListener>();
            stateListener->setOwner( this );
            stateObject->addStateListener( stateListener );

            auto state = factoryManager->make_ptr<TextureState>();
            stateObject->setState( state );
            stateObject->setOwner( this );

            setStateObject( stateObject );
            setStateListener( stateListener );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );
        }

    }  // end namespace render
}  // end namespace fb
