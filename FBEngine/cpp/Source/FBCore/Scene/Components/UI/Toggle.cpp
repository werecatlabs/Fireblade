#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Toggle.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Interface/UI/IUIToggle.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/UI/IUILabelSliderPair.h>
#include <FBCore/Scene/Components/ComponentEvent.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBCore/Scene/Components/UI/Image.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUISlider.h>
#include <FBCore/Interface/UI/IUIText.h>
#include <FBCore/Interface/UI/IUIButton.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Toggle, UIComponent );

    Toggle::Toggle() = default;

    Toggle::~Toggle()
    {
        unload( nullptr );
    }

    void Toggle::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            UIComponent::load( data );

            auto applicationManager = core::ApplicationManager::instance();
            auto inputDeviceManager = applicationManager->getInputDeviceManager();
            inputDeviceManager->addListener( m_elementListener );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Toggle::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        auto applicationManager = core::ApplicationManager::instance();
        auto inputDeviceManager = applicationManager->getInputDeviceManager();
        inputDeviceManager->removeListener( m_elementListener );

        UIComponent::unload( data );

        setLoadingState( LoadingState::Unloaded );
    }

    SmartPtr<Properties> Toggle::getProperties() const
    {
        auto properties = UIComponent::getProperties();
        properties->setProperty( "toggle", m_isToggled );
        properties->setPropertyAsType( "toggleTransform", m_toggleTransform );
        properties->setProperty( "toggledColour", m_toggledColour );
        properties->setProperty( "untoggledColour", m_untoggledColour );
        return properties;
    }

    void Toggle::setProperties( SmartPtr<Properties> properties )
    {
        UIComponent::setProperties( properties );

        bool toggled = isToggled();
        properties->getPropertyValue( "toggle", toggled );
        properties->getPropertyAsType( "toggleTransform", m_toggleTransform );
        properties->getPropertyValue( "toggledColour", m_toggledColour );
        properties->getPropertyValue( "untoggledColour", m_untoggledColour );

        setToggled( toggled );
    }

    bool Toggle::isToggled() const
    {
        return m_isToggled;
    }

    void Toggle::setToggled( bool toggled )
    {
        m_isToggled = toggled;

        updateTransform();
        updateColour();
    }

    SmartPtr<LayoutTransform> Toggle::getToggleTransform() const
    {
        return m_toggleTransform;
    }

    void Toggle::setToggleTransform( SmartPtr<LayoutTransform> toggleTransform )
    {
        m_toggleTransform = toggleTransform;
    }

    auto Toggle::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        UIComponent::handleComponentEvent( state, eventType );

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
                updateVisibility();
                updateColour();
            }
            break;
            }
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    Parameter Toggle::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
    {
        if( eventValue == IEvent::CLICK_HASH )
        {
            auto toggled = isToggled();
            setToggled( !toggled );
        }
        else
        {
            return UIComponent::handleEvent( eventType, eventValue, arguments, sender, object, event );
        }

        return {};
    }

    void Toggle::updateTransform()
    {
        auto toggled = isToggled();

        if( m_toggleTransform )
        {
            auto layoutTransform = getActor()->getComponent<LayoutTransform>();
            auto size = layoutTransform->getSize();
            auto halfSize = size * real_Num( 0.5 );

            auto pos = Vector2( toggled ? halfSize.x : -halfSize.x, real_Num( 0.0 ) );
            m_toggleTransform->setPosition( pos );
        }
    }

    void Toggle::updateColour()
    {
        if( auto actor = getActor() )
        {
            if( auto image = actor->getComponent<Image>() )
            {
                auto element = image->getElement();
                element->setColour( m_isToggled ? m_toggledColour : m_untoggledColour );
            }
        }
    }

    void Toggle::setUntoggledColour( const ColourF &untoggledColour )
    {
        m_untoggledColour = untoggledColour;
    }

    ColourF Toggle::getUntoggledColour() const
    {
        return m_untoggledColour;
    }

    void Toggle::setToggledColour( const ColourF &toggledColour )
    {
        m_toggledColour = toggledColour;
    }

    ColourF Toggle::getToggledColour() const
    {
        return m_toggledColour;
    }
}  // namespace fb::scene
