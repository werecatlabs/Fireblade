#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Slider.h>
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
    FB_CLASS_REGISTER_DERIVED( fb::scene, Slider, UIComponent );

    Slider::Slider() = default;

    Slider::~Slider()
    {
        unload( nullptr );
    }

    void Slider::load( SmartPtr<ISharedObject> data )
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

    void Slider::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        m_slider = nullptr;
        UIComponent::unload( data );

        setLoadingState( LoadingState::Unloaded );
    }

    auto Slider::getSlider() const -> SmartPtr<ui::IUISlider>
    {
        return m_slider;
    }

    void Slider::setSlider( SmartPtr<ui::IUISlider> slider )
    {
        m_slider = slider;
    }

    void Slider::createUI()
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

                auto slider = renderUI->addElementByType<ui::IUISlider>();
                setSlider( slider );
                setElement( slider );

                updateVisibility();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }
}  // namespace fb::scene
