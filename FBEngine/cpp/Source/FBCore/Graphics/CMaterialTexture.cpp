#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CMaterialTexture.h>
#include <FBCore/Graphics/CMaterialTechnique.h>
#include <FBCore/Graphics/CMaterialPass.h>
#include <FBCore/Graphics/CMaterial.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialTexture, CMaterialNode<IMaterialTexture> );

        CMaterialTexture::CMaterialTexture()
        {
        }

        CMaterialTexture::~CMaterialTexture()
        {
            unload( nullptr );
        }

        void CMaterialTexture::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                if( auto texture = getTexture() )
                {
                    if( !texture->isLoaded() )
                    {
                        graphicsSystem->loadObject( texture );
                    }
                }

                //if( auto texture = getTexture() )
                //{
                //    auto pTexture = fb::static_pointer_cast<CTextureOgre>( m_texture );

                //    auto t = pTexture->getTexture();
                //    FB_ASSERT( t );

                //    if( t )
                //    {
                //        createTextureUnitState();

                //        m_textureUnitState->setTexture( t );
                //    }
                //}

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTexture::createTextureUnitState()
        {
            //auto parent = getParent();
            //auto parentPass = fb::static_pointer_cast<CMaterialPass>( parent );
            //if( parentPass )
            //{
            //    auto pass = parentPass->getPass();
            //    FB_ASSERT( pass );

            //    if( pass )
            //    {
            //        //m_textureUnitState = pass->createTextureUnitState();
            //    }
            //}
        }

        void CMaterialTexture::reload( SmartPtr<ISharedObject> data )
        {
        }

        void CMaterialTexture::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    m_animator = nullptr;
                    m_texture = nullptr;
                    CMaterialNode<IMaterialTexture>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String CMaterialTexture::getTextureName() const
        {
            return m_texturePath;
        }

        void CMaterialTexture::setTextureName( const String &name )
        {
            //m_textureUnitState->setTextureName( name );

            //auto textureManager = Ogre::TextureManager::getSingletonPtr();
            //auto texture =
            //    textureManager->load( name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
            //if( texture )
            //{
            //    m_textureUnitState->setTexture( texture );
            //}
        }

        SmartPtr<ITexture> CMaterialTexture::getTexture() const
        {
            return m_texture;
        }

        void CMaterialTexture::setTexture( SmartPtr<ITexture> texture )
        {
            m_texture = texture;
        }

        void CMaterialTexture::setScale( const Vector3F &scale )
        {
            m_scale = scale;
        }

        SmartPtr<IAnimator> CMaterialTexture::getAnimator() const
        {
            return m_animator;
        }

        void CMaterialTexture::setAnimator( SmartPtr<IAnimator> animator )
        {
            m_animator = animator;
        }

        ColourF CMaterialTexture::getTint() const
        {
            return m_tint;
        }

        void CMaterialTexture::setTint( const ColourF &tint )
        {
            m_tint = tint;
        }

        void CMaterialTexture::_getObject( void **ppObject )
        {
            *ppObject = nullptr;
        }

        SmartPtr<ISharedObject> CMaterialTexture::toData() const
        {
            try
            {
                auto properties = fb::make_ptr<Properties>();

                if( m_texture )
                {
                    auto handle = m_texture->getHandle();
                    properties->setProperty( "texturePath", handle->getUUID() );
                }

                return properties;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CMaterialTexture::fromData( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto properties = fb::static_pointer_cast<Properties>( data );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();
                if( !resourceDatabase )
                {
                    FB_LOG( "Resource database null." );
                }

                if( resourceDatabase )
                {
                    auto texturePath = properties->getProperty( "texturePath" );
                    if( !StringUtil::isNullOrEmpty( texturePath ) )
                    {
                        if( auto textureResource = resourceDatabase->loadResourceById( texturePath ) )
                        {
                            auto texture = fb::static_pointer_cast<ITexture>( textureResource );

                            if( !texture->isLoaded() )
                            {
                                texture->load( nullptr );
                            }

                            setTexture( texture );
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<Properties> CMaterialTexture::getProperties() const
        {
            try
            {
                auto properties = CMaterialNode<IMaterialTexture>::getProperties();

                static const auto texturePathStr = String( "Texture" );

                properties->setProperty( "scale", m_scale );
                properties->setProperty( "tint", m_tint );

                auto texture = getTexture();
                properties->setProperty( texturePathStr, texture );

                auto material = getMaterial();
                if( material )
                {
                    auto materialType = material->getMaterialType();
                    switch( materialType )
                    {
                    case IMaterial::MaterialType::Standard:
                    case IMaterial::MaterialType::StandardSpecular:
                    case IMaterial::MaterialType::StandardTriPlanar:
                    case IMaterial::MaterialType::TerrainStandard:
                    case IMaterial::MaterialType::TerrainSpecular:
                    case IMaterial::MaterialType::TerrainDiffuse:
                    case IMaterial::MaterialType::Skybox:
                    {
                        //auto textureType =
                        //    static_cast<IMaterial::SkyboxTextureTypes>( getTextureType() );
                        //switch( textureType )
                        //{
                        //case IMaterial::SkyboxCubeTextureTypes::Cube:
                        //{
                        //    properties->setProperty( "Generate", "GenerateButton", "button", false );
                        //}
                        //break;
                        //default:
                        //{
                        //}
                        //break;
                        //};
                    }
                    break;
                    case IMaterial::MaterialType::SkyboxCubemap:
                    case IMaterial::MaterialType::UI:
                    case IMaterial::MaterialType::Custom:
                    {
                    }
                    break;
                    default:
                    {
                    }
                    break;
                    };
                }

                return properties;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CMaterialTexture::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto textureManager = graphicsSystem->getTextureManager();
                FB_ASSERT( textureManager );

                static const auto texturePathStr = String( "Texture" );

                properties->getPropertyValue( "scale", m_scale );
                properties->getPropertyValue( "tint", m_tint );

                auto texture = SmartPtr<ITexture>();
                properties->getPropertyValue( texturePathStr, texture );

                if( getTexture() != texture )
                {
                    setTexture( texture );
                }

                if( properties->hasProperty( "Generate" ) )
                {
                    auto &generateButton = properties->getPropertyObject( "Generate" );
                    if( generateButton.getAttribute( "click" ) == "true" )
                    {
                        Array<SmartPtr<ITexture>> skyboxTextures;

                        auto parent = getParent();
                        auto pass = fb::static_pointer_cast<IMaterialPass>( parent );

                        auto textureUnits = pass->getTextureUnits();
                        for( auto textureUnit : textureUnits )
                        {
                            auto texture = textureUnit->getTexture();
                            skyboxTextures.push_back( texture );
                        }

                        textureManager->createSkyBoxCubeMap( skyboxTextures );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Array<SmartPtr<ISharedObject>> CMaterialTexture::getChildObjects() const
        {
            try
            {
                auto objects = CMaterialNode<IMaterialTexture>::getChildObjects();
                objects.push_back( m_animator );
                objects.push_back( m_texture );
                return objects;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return Array<SmartPtr<ISharedObject>>();
        }

        u32 CMaterialTexture::getTextureType() const
        {
            return m_textureType;
        }

        void CMaterialTexture::setTextureType( u32 textureType )
        {
            m_textureType = textureType;
        }

        void CMaterialTexture::MaterialTextureStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CMaterialTexture::MaterialTextureStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CMaterialTexture::MaterialTextureStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

    }  // end namespace render
}  // end namespace fb
