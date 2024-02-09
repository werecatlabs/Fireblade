#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Image.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/UI/IUILayout.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIImage.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Image, UIComponent );
    const String Image::TextureStr = String( "Texture" );

    Image::Image() = default;

    Image::~Image()
    {
        unload( nullptr );
    }

    void Image::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            createUI();
            UIComponent::load( data );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Image::createUI()
    {
        auto element = getElement();
        if( !element )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto renderUI = applicationManager->getRenderUI();
            FB_ASSERT( renderUI );

            if( auto actor = getActor() )
            {
                if( auto parentActor = actor->getSceneRoot() )
                {
                    auto image = renderUI->addElementByType<ui::IUIImage>();
                    setImage( image );
                    setElement( image );

                    updateVisibility();
                    updateColour();
                }
            }
        }
    }

    void Image::setupMaterial()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto baseMaterialName = String( "DefaultUI" );
            auto materialResource =
                resourceDatabase->findResourceByType<render::IMaterial>( baseMaterialName );
            if( materialResource )
            {
                if( auto actor = getActor() )
                {
                    if( auto materialComponent = actor->getComponent<Material>() )
                    {
                        if( !m_material )
                        {
                            if( auto handle = getHandle() )
                            {
                                auto uuid = handle->getUUID();
                                if( StringUtil::isNullOrEmpty( uuid ) )
                                {
                                    uuid = StringUtil::getUUID();
                                }

                                //FB_ASSERT( materialResource->getMaterialType() ==
                                //           render::IMaterial::MaterialType::UI );

                                auto clonedMaterial =
                                    resourceDatabase->cloneResourceByType<render::IMaterial>(
                                        materialResource, uuid );
                                FB_ASSERT( clonedMaterial );
                                //FB_ASSERT( clonedMaterial->getMaterialType() ==
                                //           render::IMaterial::MaterialType::UI );

                                m_material = clonedMaterial;
                                //m_material = materialResource;
                            }
                        }

                        materialComponent->setMaterial( m_material );
                    }
                }
            }

            if( m_material )
            {
                if( !m_material->isLoaded() )
                {
                    graphicsSystem->loadObject( m_material );
                }
            }

            if( m_material )
            {
                m_material->setTexture( m_texture );
            }

            if( m_image )
            {
                m_image->setMaterial( m_material );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Image::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                m_image = nullptr;
                UIComponent::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Image::getImage() const -> SmartPtr<ui::IUIImage>
    {
        return m_image;
    }

    void Image::setImage( SmartPtr<ui::IUIImage> image )
    {
        m_image = image;
    }

    void Image::updateMaterials()
    {
        if( auto actor = getActor() )
        {
            auto materials = actor->getComponentsByType<Material>();
            for( auto material : materials )
            {
                if( auto pMaterial = material->getMaterial() )
                {
                    if( m_image )
                    {
                        auto materialType = pMaterial->getMaterialType();
                        if( materialType == render::IMaterial::MaterialType::UI )
                        {
                            m_image->setMaterial( pMaterial );
                        }
                    }
                }
            }
        }

        if( auto texture = getTexture() )
        {
            if( m_image )
            {
                m_image->setTexture( texture );
            }
        }
    }

    auto Image::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = UIComponent::getProperties();

        auto texture = getTexture();
        properties->setProperty( TextureStr, texture );

        return properties;
    }

    void Image::setProperties( SmartPtr<Properties> properties )
    {
        UIComponent::setProperties( properties );

        SmartPtr<render::ITexture> texture;
        properties->getPropertyValue( TextureStr, texture );

        if( texture != m_texture )
        {
            m_texture = texture;

            setupMaterial();

            if( auto actor = getActor() )
            {
                if( auto material = actor->getComponent<Material>() )
                {
                    material->updateImageComponent();
                }
            }

            updateMaterials();
        }
    }

    auto Image::isValid() const -> bool
    {
        const auto &state = getLoadingState();
        switch( state )
        {
        case LoadingState::Unloaded:
        {
            return m_image == nullptr;
        }
        break;
        case LoadingState::Loading:
        {
            return m_image == nullptr;
        }
        break;
        case LoadingState::Loaded:
        {
            return m_image != nullptr && m_image->isValid();
        }
        break;

        case LoadingState::Unloading:
        {
            return m_image == nullptr;
        }
        break;
        default:
        {
        }
        }

        return false;
    }

    auto Image::getTexture() const -> SmartPtr<render::ITexture>
    {
        return m_texture;
    }

    void Image::setTexture( SmartPtr<render::ITexture> texture )
    {
        m_texture = texture;
    }

    auto Image::getTextureName() const -> String
    {
        if( auto texture = getTexture() )
        {
            if( auto handle = texture->getHandle() )
            {
                return handle->getName();
            }
        }

        return "";
    }

    void Image::setTextureName( const String &textureName )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();
        FB_ASSERT( resourceDatabase );

        auto texture = resourceDatabase->loadResourceByType<render::ITexture>( textureName );
        setTexture( texture );
    }

}  // namespace fb::scene
