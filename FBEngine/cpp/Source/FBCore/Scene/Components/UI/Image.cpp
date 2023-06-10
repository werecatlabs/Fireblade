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
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Image, UIComponent );

        Image::Image()
        {
        }

        Image::~Image()
        {
            unload( nullptr );
        }

        void Image::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                createImage();
                UIComponent::load( data );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        IFSM::ReturnType Image::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            Component::handleComponentEvent( state, eventType );

            switch( eventType )
            {
            case IFSM::Event::Change:
            {
            }
            break;
            case IFSM::Event::Enter:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Destroyed:
                {
                }
                break;
                case State::Edit:
                case State::Play:
                {
                    setupCanvas();

                    auto image = getImage();
                    if( !image )
                    {
                        createImage();
                    }

                    setupMaterial();
                    updateOrder();
                    updateTransform();
                    updateVisibility();
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Edit:
                case State::Play:
                {
                    auto image = getImage();

                    if( auto canvas = fb::static_pointer_cast<scene::Layout>( getCanvas() ) )
                    {
                        if( auto layout = canvas->getLayout() )
                        {
                            layout->removeChild( image );
                        }
                    }
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Pending:
            {
            }
            break;
            case IFSM::Event::Complete:
            {
            }
            break;
            case IFSM::Event::NewState:
            {
            }
            break;
            case IFSM::Event::WaitForChange:
            {
            }
            break;
            default:
            {
            }
            break;
            }

            return IFSM::ReturnType::Ok;
        }

        void Image::createImage()
        {
            auto applicationManager = core::IApplicationManager::instance();
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

                    image->setVisible( true );
                }
            }
        }

        void Image::setupMaterial()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
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

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto renderUI = applicationManager->getRenderUI();
                    FB_ASSERT( renderUI );

                    if( m_image )
                    {
                        //auto parentContainer = getParentContainer();
                        //if( parentContainer )
                        //{
                        //    parentContainer->removeChild( m_image );
                        //}

                        renderUI->removeElement( m_image );

                        m_image->unload( nullptr );
                        m_image = nullptr;
                    }

                    UIComponent::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Image::updateDirty( u32 flags, u32 oldFlags )
        {
            if( auto actor = getActor() )
            {
                if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
                {
                    auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) &&
                                   actor->isEnabledInScene();

                    auto image = getImage();
                    if( !image )
                    {
                        createImage();
                    }

                    if( image )
                    {
                        image->setVisible( visible );
                    }

                    setupMaterial();
                    updateTransform();
                }
                else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                         BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                {
                    auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) &&
                                   actor->isEnabledInScene();

                    auto image = getImage();
                    if( !image )
                    {
                        createImage();
                    }

                    if( image )
                    {
                        image->setVisible( visible );
                    }

                    setupMaterial();
                    updateTransform();
                }
            }
        }

        void Image::parentChanged( SmartPtr<scene::IActor> newParent, SmartPtr<scene::IActor> oldParent )
        {
            auto image = getImage();
            if( !image )
            {
                createImage();
            }
        }

        void Image::hierarchyChanged()
        {
            auto image = getImage();
            if( !image )
            {
                createImage();
            }
        }

        void Image::childAdded( SmartPtr<scene::IActor> child )
        {
        }

        void Image::childRemoved( SmartPtr<scene::IActor> child )
        {
        }

        SmartPtr<ui::IUIImage> Image::getImage() const
        {
            return m_image;
        }

        void Image::setImage( SmartPtr<ui::IUIImage> image )
        {
            m_image = image;
        }

        void Image::updateDimensions()
        {
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

        SmartPtr<Properties> Image::getProperties() const
        {
            auto properties = UIComponent::getProperties();

            static const auto imagePathStr = String( "Texture" );

            properties->setProperty( imagePathStr, m_texture );

            //auto &materialPathProperty = properties->getPropertyObject( imagePathStr );
            //materialPathProperty.setType( "file" );

            return properties;
        }

        void Image::setProperties( SmartPtr<Properties> properties )
        {
            UIComponent::setProperties( properties );

            static const auto imagePathStr = String( "Texture" );

            SmartPtr<render::ITexture> texture;
            properties->getPropertyValue( imagePathStr, texture );

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

        bool Image::isValid() const
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

        Array<SmartPtr<ISharedObject>> Image::getChildObjects() const
        {
            auto objects = UIComponent::getChildObjects();

            objects.push_back( m_image );
            return objects;
        }

        SmartPtr<render::ITexture> Image::getTexture() const
        {
            return m_texture;
        }

        void Image::setTexture( SmartPtr<render::ITexture> texture )
        {
            m_texture = texture;
        }

        String Image::getTextureName() const
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
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto texture = resourceDatabase->loadResourceByType<render::ITexture>( textureName );
            setTexture( texture );
        }

    }  // namespace scene
}  // end namespace fb
