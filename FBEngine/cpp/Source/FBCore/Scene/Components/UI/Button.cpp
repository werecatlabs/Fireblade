#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Button.h>
#include <FBCore/Scene/Components/ComponentEvent.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIText.h>
#include <FBCore/Interface/UI/IUIButton.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Button, UIComponent );

    Button::Button() : m_caption( "Button" )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto clickedEvent = factoryManager->make_ptr<ComponentEvent>();
        clickedEvent->setEventHash( IEvent::ACTIVATE_HASH );
        addEvent( clickedEvent );

        auto onClickEventListener = factoryManager->make_ptr<ComponentEventListener>();
        onClickEventListener->setEvent( clickedEvent );
        onClickEventListener->setComponent( this );
        clickedEvent->addListener( onClickEventListener );

        auto hoverEvent = factoryManager->make_ptr<ComponentEvent>();
        hoverEvent->setEventHash( IEvent::TOGGLE_HIGHLIGHT_HASH );
        addEvent( hoverEvent );

        auto onHoverEventListener = factoryManager->make_ptr<ComponentEventListener>();
        onHoverEventListener->setEvent( hoverEvent );
        onHoverEventListener->setComponent( this );
        hoverEvent->addListener( onHoverEventListener );
    }

    Button::~Button()
    {
        unload( nullptr );
    }

    void Button::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            // createUI();
            UIComponent::load( data );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Button::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                m_button = nullptr;
                UIComponent::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Button::handleEvent( IEvent::Type eventType, hash_type eventValue,
                              const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                              SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        const auto button = getButton();
        if( object == button )
        {
            auto events = getEvents();
            for( auto &event : events )
            {
                auto listeners = event->getListeners();
                for( auto &listener : listeners )
                {
                    listener->handleEvent( eventType, eventValue, arguments, sender, object, event );
                }
            }
        }

        return UIComponent::handleEvent( eventType, eventValue, arguments, sender, object, event );
    }

    void Button::createUI()
    {
        try
        {
            auto element = getElement();
            if( !element )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                FB_ASSERT( renderUI );

                auto button = renderUI->addElementByType<ui::IUIButton>();
                setButton( button );

                setElement( button );
                updateCaption();

                updateVisibility();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Button::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = UIComponent::getChildObjects();

        if( auto button = getButton() )
        {
            objects.emplace_back( button );
        }

        return objects;
    }

    auto Button::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = UIComponent::getProperties() )
        {
            properties->setProperty( "image", m_image );
            properties->setProperty( "highlightedImage", m_highlightedImage );
            properties->setProperty( "pressedImage", m_pressedImage );

            properties->setProperty( "normalColour", m_normalColour );
            properties->setProperty( "highlightedColour", m_highlightedColour );
            properties->setProperty( "pressedColour", m_pressedColour );
            properties->setProperty( "disabledColour", m_disabledColour );

            properties->setProperty( "isSimpleButton", m_isSimpleButton );
            properties->setProperty( "caption", m_caption );
            properties->setProperty( "textSize", m_textSize );

            return properties;
        }

        return nullptr;
    }

    void Button::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "image", m_image );
        properties->getPropertyValue( "highlightedImage", m_highlightedImage );
        properties->getPropertyValue( "pressedImage", m_pressedImage );

        properties->getPropertyValue( "normalColour", m_normalColour );
        properties->getPropertyValue( "highlightedColour", m_highlightedColour );
        properties->getPropertyValue( "pressedColour", m_pressedColour );
        properties->getPropertyValue( "disabledColour", m_disabledColour );

        properties->getPropertyValue( "isSimpleButton", m_isSimpleButton );
        properties->getPropertyValue( "caption", m_caption );
        properties->getPropertyValue( "textSize", m_textSize );

        UIComponent::setProperties( properties );

        updateCaption();

        if( auto text = fb::static_pointer_cast<ui::IUIText>( getElement() ) )
        {
            text->setTextSize( static_cast<f32>( m_textSize ) );
        }
    }

    void Button::updateElementState()
    {
        updateCaption();

        if( auto text = fb::dynamic_pointer_cast<ui::IUIButton>( getElement() ) )
        {
            text->setTextSize( static_cast<f32>( m_textSize ) );
        }
    }

    void Button::updateCaption()
    {
        if( m_button )
        {
            if( m_isSimpleButton )
            {
                m_button->setLabel( m_caption );
            }
            else
            {
                m_button->setLabel( "" );
            }
        }
    }

    auto Button::getButton() const -> SmartPtr<ui::IUIButton>
    {
        return m_button;
    }

    void Button::setButton( SmartPtr<ui::IUIButton> button )
    {
        m_button = button;
    }

    auto Button::isSimpleButton() const -> bool
    {
        return m_isSimpleButton;
    }

    void Button::setSimpleButton( bool simpleButton )
    {
        m_isSimpleButton = simpleButton;
        updateCaption();
    }

    auto Button::getCaption() const -> String
    {
        return m_caption;
    }

    void Button::setCaption( const String &caption )
    {
        m_caption = caption;
        updateCaption();
    }
}  // namespace fb::scene
