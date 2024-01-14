#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Texture.h>
#include <FBCore/FBCore.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, Texture, ResourceGraphics<ITexture> );
    u32 Texture::m_ext = 0;

    Texture::Texture()
    {
        setResourceType( ResourceType::Texture );
    }

    Texture::~Texture()
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

    void Texture::update()
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

    void Texture::save()
    {
        //if( m_texture )
        {
            // m_texture->writeContentsToFile( "test.png", 0, 100 );
        }
    }

    void Texture::saveToFile( const String &filePath )
    {
    }

    void Texture::loadFromFile( const String &filePath )
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

            auto applicationManager = core::ApplicationManager::instance();
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

    void Texture::load( SmartPtr<ISharedObject> data )
    {
    }

    void Texture::reload( SmartPtr<ISharedObject> data )
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

    void Texture::unload( SmartPtr<ISharedObject> data )
    {
        if( auto renderTarget = getRenderTarget() )
        {
            renderTarget->unload( nullptr );
            setRenderTarget( nullptr );
        }

        ResourceGraphics<ITexture>::unload( data );
    }

    auto Texture::getRenderTarget() const -> SmartPtr<IRenderTarget>
    {
        return m_renderTarget;
    }

    void Texture::setRenderTarget( SmartPtr<IRenderTarget> rt )
    {
        m_renderTarget = rt;
    }

    void Texture::copyToTexture( SmartPtr<ITexture> &target )
    {
        // SmartPtr<CTextureOgre> other = target;

        // m_texture->copyToTexture(other->m_texture);
    }

    void Texture::copyData( void *data, const Vector2I &size )
    {
    }

    auto Texture::getSize() const -> Vector2I
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<TextureState>() )
            {
                return state->getSize();
            }
        }

        return Vector2I::zero();
    }

    void Texture::setSize( const Vector2I &size )
    {
        if( auto context = getStateContext() )
        {
            if( auto state = context->getStateByType<TextureState>() )
            {
                state->setSize( size );
            }
        }
    }

    void Texture::_getObject( void **ppObject ) const
    {
    }

    auto Texture::getActualSize() const -> Vector2I
    {
        return Vector2I::zero();
    }

    void Texture::getTextureGPU( void **ppTexture ) const
    {
    }

    void Texture::getTextureFinal( void **ppTexture ) const
    {
    }

    auto Texture::getTextureHandle() const -> size_t
    {
        return m_textureHandle;
    }

    auto Texture::getUsageFlags() const -> u32
    {
        return m_usageFlags;
    }

    void Texture::setUsageFlags( u32 usageFlags )
    {
        m_usageFlags = usageFlags;
    }

    void Texture::fromData( SmartPtr<ISharedObject> data )
    {
    }

    auto Texture::toData() const -> SmartPtr<ISharedObject>
    {
        auto data = fb::make_ptr<Properties>();
        data->setProperty( "uuid", getHandle()->getUUID() );
        return data;
    }

    auto Texture::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = ResourceGraphics<ITexture>::getProperties();

        static const auto materialTypeStr = String( "Material Type" );

        auto size = getSize();
        properties->setProperty( "Size", size );

        return properties;
    }

    void Texture::setProperties( SmartPtr<Properties> properties )
    {
        ResourceGraphics<ITexture>::setProperties( properties );
    }

    auto Texture::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( 4 );

        if( auto renderTarget = getRenderTarget() )
        {
            objects.emplace_back( renderTarget );
        }

        return objects;
    }

    auto Texture::getTextureType() const -> ITexture::TextureType
    {
        return m_textureType;
    }

    void Texture::setTextureType( TextureType textureType )
    {
        m_textureType = textureType;
    }

    void Texture::createStateObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto factoryManager = applicationManager->getFactoryManager();

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

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

    void Texture::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
        if( auto owner = getOwner() )
        {
            if( message->isDerived<StateMessageVector2I>() )
            {
                auto vector2Message = fb::static_pointer_cast<StateMessageVector2I>( message );
                auto value = vector2Message->getValue();
                auto type = vector2Message->getType();

                if( type == STATE_MESSAGE_TEXTURE_SIZE )
                {
                    owner->setSize( value );
                }
            }
        }
    }

    void Texture::StateListener::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void Texture::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto Texture::StateListener::getOwner() const -> SmartPtr<Texture>
    {
        return m_owner;
    }

    void Texture::StateListener::setOwner( SmartPtr<Texture> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::render
