#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CRenderTargetOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreTextureGpuManager.h>
#include <Ogre.h>
#include <OgreRoot.h>

#if defined FB_PLATFORM_WIN32
#    if FB_BUILD_RENDERER_DX11
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/Direct3D11/include/OgreD3D11TextureGpu.h>
#    elif FB_BUILD_RENDERER_GL3PLUS
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#    elif FB_BUILD_RENDERER_OPENGL
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#    endif
#elif defined FB_PLATFORM_LINUX
#    if FB_BUILD_RENDERER_OPENGL
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusPrerequisites.h>
#        include <FBGraphicsOgreNext/Ogre/RenderSystems/GL3Plus/include/OgreGL3PlusTextureGpu.h>
#    endif
#endif

namespace fb::render
{

    CTextureOgreNext::CTextureOgreNext()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "CTextureOgreNext" );
        }

        m_usageFlags = TU_STATIC;
        setResourceType( ResourceType::Texture );
        createStateObject();
    }

    CTextureOgreNext::~CTextureOgreNext()
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

    void CTextureOgreNext::save()
    {
        if( auto texture = getTexture() )
        {
            auto filePath = getFilePath();
            texture->writeContentsToFile( filePath, 0, 100 );
        }
    }

    void CTextureOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( isLoaded() == false );
            FB_ASSERT( getTexture() == nullptr );

            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            ISharedObject::ScopedLock lock( graphicsSystem );

            auto usageFlags = getUsageFlags();
            if( usageFlags == TU_RENDERTARGET )
            {
                using namespace Ogre;

                m_size = Vector2I( 1280, 720 );

                auto root = Ogre::Root::getSingletonPtr();
                auto textureManager = root->getRenderSystem()->getTextureGpuManager();

                uint32 iblSpecularFlag = 0;

                auto rt =
                    textureManager->createOrRetrieveTexture( "RTT",
                                                             GpuPageOutStrategy::Discard,          //
                                                             TextureFlags::RenderToTexture |       //
                                                                 TextureFlags::AllowAutomipmaps |  //
                                                                 iblSpecularFlag,                  //
                                                             TextureTypes::Type2D );
                rt->scheduleTransitionTo( GpuResidency::OnStorage );
                uint32 resolution = 512u;

                rt->setResolution( m_size.X(), m_size.Y() );

                // mDynamicCubemap->setNumMipmaps(Ogre::PixelFormatGpuUtils::getMaxMipmapCount(resolution));
                // if (mIblQuality != MipmapsLowest)
                //{
                //	// Limit max mipmap to 16x16
                //	mDynamicCubemap->setNumMipmaps(mDynamicCubemap->getNumMipmaps() - 4u);
                // }

                rt->setPixelFormat( PFG_RGBA8_UNORM_SRGB );
                rt->scheduleTransitionTo( GpuResidency::Resident );
                // rt->scheduleTransitionTo( GpuResidency::OnStorage );

                setTexture( rt );

                size_t iTexture = 0;
                getTextureFinal( reinterpret_cast<void **>( &iTexture ) );

                m_textureHandle = iTexture;

                if( !m_renderTarget )
                {
                    auto renderTarget = fb::make_ptr<CRenderTargetOgreNext<IRenderTarget>>();
                    //renderTarget->setRenderTarget( rt );
                    renderTarget->setTexture( this );
                    m_renderTarget = renderTarget;
                }
                else
                {
                    auto pRenderTarget =
                        fb::static_pointer_cast<CRenderTargetOgreNext<IRenderTarget>>( m_renderTarget );
                    //pRenderTarget->setRenderTarget( rtt );
                    pRenderTarget->setTexture( this );
                }
            }
            else
            {
                FB_ASSERT( getTexture() == nullptr );

                auto root = Ogre::Root::getSingletonPtr();
                auto renderSystem = root->getRenderSystem();
                auto textureManager = renderSystem->getTextureGpuManager();

                FB_ASSERT( textureManager );
                auto textureName = getFilePath();

                if( !StringUtil::isNullOrEmpty( textureName ) )
                {
                    auto imagePtr = new Ogre::Image2();
                    imagePtr->load( textureName,
                                    Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

                    auto texture = textureManager->createOrRetrieveTexture(
                        textureName, Ogre::GpuPageOutStrategy::Discard,
                        Ogre::TextureFlags::AutomaticBatching |
                            Ogre::TextureFlags::PrefersLoadingFromFileAsSRGB,
                        Ogre::TextureTypes::Type2D,
                        Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

                    // Schedule a transition for the texture to become resident in GPU memory
                    texture->scheduleTransitionTo( Ogre::GpuResidency::GpuResidency::Resident, imagePtr,
                                                   true );

                    setTexture( texture );

                    size_t iTexture = 0;
                    //getTextureFinal( (void **)&iTexture );

                    m_textureHandle = iTexture;
                }
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CTextureOgreNext::reload( SmartPtr<ISharedObject> data )
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

    void CTextureOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            destroyStateObject();
            // Ogre::TextureManager::getSingleton().remove(m_texture->getHandle());
            Texture::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CTextureOgreNext::getSize() const -> Vector2I
    {
        return m_size;
    }

    void CTextureOgreNext::setSize( const Vector2I &size )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto timer = applicationManager->getTimer();

        auto renderTask = graphicsSystem->getRenderTask();
        auto stateTask = graphicsSystem->getStateTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            if( m_size != size )
            {
                if( timer->getTimeSinceLevelLoad() > 1.0 && m_nextResize < timer->getTime() )
                {
                    if( size.X() > 0 && size.X() < 4096 && size.Y() > 0 && size.Y() < 4096 )
                    {
                        if( auto texture = getTexture() )
                        {
                            if( texture->getResidencyStatus() != Ogre::GpuResidency::OnStorage )
                            {
                                texture->scheduleTransitionTo( Ogre::GpuResidency::OnStorage );
                            }

                            texture->setResolution( size.X(), size.Y() );
                            texture->scheduleTransitionTo( Ogre::GpuResidency::Resident );
                        }

                        m_size = size;

                        m_nextResize = timer->getTime() + 3.0;
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageVector2I>();
                    message->setValue( size );

                    if( auto stateContext = getStateContext() )
                    {
                        stateContext->addMessage( stateTask, message );
                    }
                }
            }
        }
        else
        {
            auto message = factoryManager->make_ptr<StateMessageVector2I>();
            message->setValue( size );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( stateTask, message );
            }
        }
    }

    void CTextureOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_texture;
    }

    auto CTextureOgreNext::getActualSize() const -> Vector2I
    {
        return Vector2I::zero();
    }

    auto CTextureOgreNext::getTexture() const -> Ogre::TextureGpu *
    {
        return m_texture;
    }

    void CTextureOgreNext::createStateObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto factoryManager = applicationManager->getFactoryManager();

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        FB_ASSERT( getStateContext() == nullptr );

        auto stateContext = stateManager->addStateObject();
        FB_ASSERT( stateContext );

        auto stateListener = fb::make_ptr<StateListener>();
        stateListener->setOwner( this );
        stateContext->addStateListener( stateListener );

        auto state = factoryManager->make_ptr<TextureState>();
        stateContext->addState( state );
        stateContext->setOwner( this );

        setStateContext( stateContext );

        auto stateTask = graphicsSystem->getStateTask();
        state->setTaskId( stateTask );
    }

    void CTextureOgreNext::setTexture( Ogre::TextureGpu *texture )
    {
        m_texture = texture;
    }

    void CTextureOgreNext::getTextureGPU( void **ppTexture ) const
    {
        *ppTexture = m_texture;
    }

    void CTextureOgreNext::getTextureFinal( void **ppTexture ) const
    {
        if( auto ogreTexture = getTexture() )
        {
#if defined FB_PLATFORM_WIN32
#    if FB_BUILD_RENDERER_DX11
            auto texture = static_cast<Ogre::D3D11TextureGpu *>( ogreTexture );
            auto tex = texture->getDefaultDisplaySrv();
            *ppTexture = tex;
#    elif FB_BUILD_RENDERER_GL3PLUS
            auto texture = (Ogre::GL3PlusTextureGpu *)ogreTexture;
            auto tex = texture->getDisplayTextureName();
            *ppTexture = (void *)tex;
#    elif FB_BUILD_RENDERER_OPENGL
            auto texture = (Ogre::GL3PlusTextureGpu *)ogreTexture;
            auto tex = texture->getFinalTextureName();
#    endif
#endif
        }
    }

    auto CTextureOgreNext::getTextureHandle() const -> size_t
    {
        //if( m_textureHandle.load() == 0 )
        //{
        //    void* iTexture = 0;
        //    getTextureFinal( (void **)&iTexture );
        //    m_textureHandle = reinterpret_cast<size_t>( iTexture );
        //}

        return m_textureHandle;
    }

    auto CTextureOgreNext::getUsageFlags() const -> u32
    {
        return m_usageFlags;
    }

    void CTextureOgreNext::setUsageFlags( u32 usageFlags )
    {
        m_usageFlags = usageFlags;
    }

}  // namespace fb::render
