#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Toggle.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Interface/UI/IUIToggle.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/UI/IUILabelSliderPair.h>
#include <FBCore/Scene/Components/ComponentEvent.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
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

            // createUI();
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

        m_toggle = nullptr;
        UIComponent::unload( data );

        setLoadingState( LoadingState::Unloaded );
    }

    auto Toggle::getToggle() const -> SmartPtr<ui::IUIToggle>
    {
        return m_toggle;
    }

    void Toggle::setToggle( SmartPtr<ui::IUIToggle> toggle )
    {
        m_toggle = toggle;
    }

    auto Toggle::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = UIComponent::getProperties();
        properties->setProperty( "toggle", m_isToggled );
        properties->setPropertyAsType( "toggleTransform", m_toggleTransform );
        return properties;
    }

    void Toggle::setProperties( SmartPtr<Properties> properties )
    {
        UIComponent::setProperties( properties );

        bool toggled = isToggled();
        properties->getPropertyValue( "toggle", toggled );
        properties->getPropertyAsType( "toggleTransform", m_toggleTransform );

        setToggled( toggled );
    }

    auto Toggle::isToggled() const -> bool
    {
        return m_isToggled;
    }

    void Toggle::setToggled( bool toggled )
    {
        if( m_isToggled != toggled )
        {
            m_isToggled = toggled;
            updateTransform();
        }
    }

    auto Toggle::getToggleTransform() const -> SmartPtr<LayoutTransform>
    {
        return m_toggleTransform;
    }

    void Toggle::setToggleTransform( SmartPtr<LayoutTransform> toggleTransform )
    {
        m_toggleTransform = toggleTransform;
    }

    void Toggle::createUI()
    {
        //try
        //{
        //    auto element = getElement();
        //    if( !element )
        //    {
        //        auto applicationManager = core::ApplicationManager::instance();
        //        FB_ASSERT( applicationManager );

        //        auto renderUI = applicationManager->getRenderUI();
        //        FB_ASSERT( renderUI );

        //        auto toggle = renderUI->addElementByType<ui::IUIToggle>();
        //        setToggle( toggle );
        //        setElement( toggle );

        //        updateVisibility();
        //    }
        //}
        //catch( std::exception &e )
        //{
        //    FB_LOG_EXCEPTION( e );
        //}
    }

    auto Toggle::handleEvent( IEvent::Type eventType, hash_type eventValue,
                              const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                              SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
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
            auto halfSize = size * 0.5f;
            m_toggleTransform->setPosition( Vector2F( toggled ? halfSize.x : -halfSize.x, 0.0f ) );
        }
    }

}  // namespace fb::scene
