#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIImage.h>
#include <FBRenderUI/CUIContainer.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, CUIImage, CUIElement<IUIImage> );

    CUIImage::CUIImage()
    {
        createStateContext();
        m_type = "Image";
    }

    CUIImage::~CUIImage()
    {
        unload( nullptr );
    }

    void CUIImage::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

#if FB_USE_MYGUI
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                FB_ASSERT( renderUI );

                IUIManager::ScopedLock lock( renderUI );

                if( auto gui = MyGUI::Gui::getInstancePtr() )
                {
                    auto image = gui->createWidget<MyGUI::ImageBox>(
                        "ImageBox", MyGUI::IntCoord( 0, 0, 100, 100 ), MyGUI::Align::Default, "Main" );
                    image->setImageTexture( "checker.png" );
                    m_imageBox = image;

                    setWidget( image );
                }
#else
                String value;
                f32 left = 0.0f;
                f32 top = 0.0f;
                f32 width = 1.0f;
                f32 height = 1.0f;
                String alignment;

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlayManager = graphicsSystem->getOverlayManager();
                FB_ASSERT( overlayManager );

                auto name = String( "Image_" ) + StringUtil::toString( m_nextGeneratedNameExt++ );
                setName( name );

                auto container = overlayManager->addElement( String( "Panel" ), getName() );

                //m_container->setLeft( left );
                //m_container->setTop( top );
                //m_container->setWidth( width );
                //m_container->setHeight( height );

                container->setVisible( true );

                //m_position = Vector2F( left, top );
                //m_size = Vector2F( width, height );

                setOverlayElement( container );
                setContainerObject( container );
#endif

                if( auto stateContext = getStateContext() )
                {
                    stateContext->setDirty( true );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CUIImage::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    auto overlayManager = graphicsSystem->getOverlayManager();
                    FB_ASSERT( overlayManager );

                    if( m_container )
                    {
                        overlayManager->removeElement( m_container );
                        m_container = nullptr;
                    }

                    CUIElement<IUIImage>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CUIImage::onToggleVisibility()
        {
            auto container = getContainerObject();
            if( container )
            {
                container->setVisible( isVisible() );
            }

            CUIElement::onToggleVisibility();
        }

        void CUIImage::setMaterialName( const String &materialName )
        {
            //auto container = getContainerObject();
            //if( container )
            //{
            //    container->setMaterialName( materialName );
            //}
        }

        auto CUIImage::getMaterialName() const -> String
        {
            //auto container = getContainerObject();
            //if( container )
            //{
            //    return container->getMaterialName();
            //}

            return String( "" );
        }

        void CUIImage::setTexture( SmartPtr<render::ITexture> texture )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    state->setTexture( texture );
                }
            }
        }

        auto CUIImage::getTexture() const -> SmartPtr<render::ITexture>
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    return state->getTexture();
                }
            }

            return nullptr;
        }

        void CUIImage::setMaterial( SmartPtr<render::IMaterial> material )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    state->setMaterial( material );
                }
            }
        }

        auto CUIImage::getMaterial() const -> SmartPtr<render::IMaterial>
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    return state->getMaterial();
                }
            }

            return nullptr;
        }

        void CUIImage::setPosition( const Vector2F &position )
        {
            if( auto container = getContainerObject() )
            {
                container->setPosition( position );
            }

            CUIElement<IUIImage>::setPosition( position );
        }

        void CUIImage::setSize( const Vector2F &size )
        {
            if( auto container = getContainerObject() )
            {
                container->setSize( size );
            }

            CUIElement<IUIImage>::setSize( size );
        }

        void CUIImage::_getObject( void **ppObject ) const
        {
            *ppObject = m_container.get();
        }

        auto CUIImage::getContainerObject() const -> SmartPtr<render::IOverlayElementContainer>
        {
            return m_container;
        }

        void CUIImage::setContainerObject( SmartPtr<render::IOverlayElementContainer> container )
        {
            m_container = container;
        }

        void CUIImage::handleStateChanged( SmartPtr<IState> &state )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto elementState = fb::static_pointer_cast<UIElementState>( state );

            auto material = elementState->getMaterial();
            auto container = getContainerObject();
            if( container )
            {
                container->setMaterial( material );
            }
        }

        auto CUIImage::getProperties() const -> SmartPtr<Properties>
        {
            auto properties = CUIElement<IUIImage>::getProperties();
            return properties;
        }

        void CUIImage::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                CUIElement<IUIImage>::setProperties( properties );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        auto CUIImage::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
        {
            auto objects = CUIElement<IUIImage>::getChildObjects();

            objects.emplace_back( m_container );

            return objects;
        }

        auto CUIImage::isValid() const -> bool
        {
            const auto &state = getLoadingState();
            switch( state )
            {
            case LoadingState::Unloaded:
            {
                return m_container == nullptr;
            }
            break;
            case LoadingState::Loading:
            {
                return m_container == nullptr;
            }
            break;
            case LoadingState::Loaded:
            {
                return m_container != nullptr && m_container->isValid();
            }
            break;

            case LoadingState::Unloading:
            {
                return m_container == nullptr;
            }
            break;
            default:
            {
            }
            }

            return false;
        }
}  // namespace fb::ui
