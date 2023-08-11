#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CTexture.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CTexture, CResourceGraphics<ITexture> );
        u32 CTexture::m_ext = 0;

        CTexture::CTexture()
        {
            setResourceType( ResourceType::Texture );
        }

        CTexture::~CTexture()
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

        void CTexture::update()
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

        void CTexture::save()
        {
            //if( m_texture )
            {
                // m_texture->writeContentsToFile( "test.png", 0, 100 );
            }
        }

        void CTexture::saveToFile( const String &filePath )
        {
        }

        void CTexture::loadFromFile( const String &filePath )
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

        void CTexture::load( SmartPtr<ISharedObject> data )
        {
        }

        void CTexture::reload( SmartPtr<ISharedObject> data )
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

        void CTexture::unload( SmartPtr<ISharedObject> data )
        {
            if( auto renderTarget = getRenderTarget() )
            {
                renderTarget->unload( nullptr );
                setRenderTarget( nullptr );
            }

            CResourceGraphics<ITexture>::unload( data );
        }

        SmartPtr<IRenderTarget> CTexture::getRenderTarget() const
        {
            return m_renderTarget;
        }

        void CTexture::setRenderTarget( SmartPtr<IRenderTarget> rt )
        {
            m_renderTarget = rt;
        }

        void CTexture::copyToTexture( SmartPtr<ITexture> &target )
        {
            // SmartPtr<CTextureOgre> other = target;

            // m_texture->copyToTexture(other->m_texture);
        }

        void CTexture::copyData( void *data, const Vector2I &size )
        {
        }

        Vector2I CTexture::getSize() const
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

        void CTexture::setSize( const Vector2I &size )
        {
            if( auto context = getStateObject() )
            {
                if( auto state = context->getStateByType<TextureState>() )
                {
                    state->setSize( size );
                }
            }
        }

        void CTexture::_getObject( void **ppObject ) const
        {
        }

        Vector2I CTexture::getActualSize() const
        {
            return Vector2I::zero();
        }

        void CTexture::getTextureGPU( void **ppTexture ) const
        {
        }

        void CTexture::getTextureFinal( void **ppTexture ) const
        {
        }

        size_t CTexture::getTextureHandle() const
        {
            return m_textureHandle;
        }

        u32 CTexture::getUsageFlags() const
        {
            return m_usageFlags;
        }

        void CTexture::setUsageFlags( u32 usageFlags )
        {
            m_usageFlags = usageFlags;
        }

        void CTexture::fromData( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<ISharedObject> CTexture::toData() const
        {
            auto data = fb::make_ptr<Properties>();
            data->setProperty( "uuid", getHandle()->getUUID() );
            return data;
        }

        SmartPtr<Properties> CTexture::getProperties() const
        {
            auto properties = CResourceGraphics<ITexture>::getProperties();

            static const auto materialTypeStr = String( "Material Type" );

            auto size = getSize();
            properties->setProperty( "Size", size );

            return properties;
        }

        void CTexture::setProperties( SmartPtr<Properties> properties )
        {
            CResourceGraphics<ITexture>::setProperties( properties );
        }

        Array<SmartPtr<ISharedObject>> CTexture::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 4 );

            if( auto renderTarget = getRenderTarget() )
            {
                objects.push_back( renderTarget );
            }

            return objects;
        }

        ITexture::TextureType CTexture::getTextureType() const
        {
            return m_textureType;
        }

        void CTexture::setTextureType( TextureType textureType )
        {
            m_textureType = textureType;
        }

        void CTexture::createStateObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto factoryManager = applicationManager->getFactoryManager();

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto stateObject = stateManager->addStateObject();
            FB_ASSERT( stateObject );

            auto stateListener = fb::make_ptr<StateListener>();
            stateListener->setOwner( this );
            stateObject->addStateListener( stateListener );

            auto state = factoryManager->make_ptr<TextureState>();
            stateObject->setState( state );
            stateObject->setOwner( this );

            setStateObject( stateObject );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );
        }

        void CTexture::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
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

        void CTexture::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CTexture::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<CTexture> CTexture::StateListener::getOwner() const
        {
            auto p = m_owner.load();
            return p.lock();
        }

        void CTexture::StateListener::setOwner( SmartPtr<CTexture> owner )
        {
            m_owner = owner;
        }
    }  // end namespace render
}  // end namespace fb
