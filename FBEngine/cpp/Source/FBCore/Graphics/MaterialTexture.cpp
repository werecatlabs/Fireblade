#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/MaterialTexture.h>
#include <FBCore/Graphics/MaterialTechnique.h>
#include <FBCore/Graphics/MaterialPass.h>
#include <FBCore/Graphics/Material.h>
#include <FBCore/Interface/Animation/IAnimator.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Graphics/ITextureManager.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Core/LogManager.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, MaterialTexture, MaterialNode<IMaterialTexture> );

    MaterialTexture::MaterialTexture() = default;

    MaterialTexture::~MaterialTexture()
    {
        unload( nullptr );
    }

    void MaterialTexture::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
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

    void MaterialTexture::createTextureUnitState()
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

    void MaterialTexture::reload( SmartPtr<ISharedObject> data )
    {
    }

    void MaterialTexture::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                m_animator = nullptr;
                m_texture = nullptr;
                MaterialNode<IMaterialTexture>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto MaterialTexture::getTextureName() const -> String
    {
        return m_texturePath;
    }

    void MaterialTexture::setTextureName( const String &name )
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

    auto MaterialTexture::getTexture() const -> SmartPtr<ITexture>
    {
        return m_texture;
    }

    void MaterialTexture::setTexture( SmartPtr<ITexture> texture )
    {
        m_texture = texture;
    }

    void MaterialTexture::setScale( const Vector3<real_Num> &scale )
    {
        m_scale = scale;
    }

    auto MaterialTexture::getAnimator() const -> SmartPtr<IAnimator>
    {
        return m_animator;
    }

    void MaterialTexture::setAnimator( SmartPtr<IAnimator> animator )
    {
        m_animator = animator;
    }

    auto MaterialTexture::getTint() const -> ColourF
    {
        return m_tint;
    }

    void MaterialTexture::setTint( const ColourF &tint )
    {
        m_tint = tint;
    }

    void MaterialTexture::_getObject( void **ppObject )
    {
        *ppObject = nullptr;
    }

    auto MaterialTexture::toData() const -> SmartPtr<ISharedObject>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            ISharedObject::ScopedLock lock( graphicsSystem );

            auto properties = factoryManager->make_ptr<Properties>();

            if( auto texture = getTexture() )
            {
                auto handle = texture->getHandle();
                properties->setProperty( "texture", handle->getUUID() );
            }

            return properties;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void MaterialTexture::fromData( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ISharedObject::ScopedLock lock( graphicsSystem );

            auto properties = fb::static_pointer_cast<Properties>( data );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            if( !resourceDatabase )
            {
                FB_LOG( "Resource database null." );
            }

            if( resourceDatabase )
            {
                auto texturePath = properties->getProperty( texturePathStr );
                if( !StringUtil::isNullOrEmpty( texturePath ) )
                {
                    if( auto textureResource = resourceDatabase->loadResourceById( texturePath ) )
                    {
                        auto texture = fb::static_pointer_cast<ITexture>( textureResource );

                        if( !texture->isLoaded() )
                        {
                            texture->load( nullptr );
                        }

                        //setTexture( texture );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto MaterialTexture::getProperties() const -> SmartPtr<Properties>
    {
        try
        {
            auto properties = MaterialNode<IMaterialTexture>::getProperties();

            properties->setProperty( scaleStr, m_scale );
            properties->setProperty( tintStr, m_tint );

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

    void MaterialTexture::setProperties( SmartPtr<Properties> properties )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto textureManager = graphicsSystem->getTextureManager();
            FB_ASSERT( textureManager );

            properties->getPropertyValue( scaleStr, m_scale );
            properties->getPropertyValue( tintStr, m_tint );

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

    auto MaterialTexture::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        try
        {
            auto objects = MaterialNode<IMaterialTexture>::getChildObjects();
            objects.reserve( 2 );

            objects.emplace_back( m_animator );
            objects.emplace_back( m_texture );
            return objects;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return {};
    }

    auto MaterialTexture::getTextureType() const -> u32
    {
        return m_textureType;
    }

    void MaterialTexture::setTextureType( u32 textureType )
    {
        m_textureType = textureType;
    }

    void MaterialTexture::MaterialTextureStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
    }

    void MaterialTexture::MaterialTextureStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void MaterialTexture::MaterialTextureStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

}  // namespace fb::render
