#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UISlider.h>
#include "FBGraphicsOgreNext/UI/UIManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriSlider.h"
#include "ColibriGui/ColibriManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb::ui, UISlider, UIElement<IUISlider> );

        UISlider::UISlider()
        {
            createStateContext();
        }

        UISlider::~UISlider()
        {
            unload( nullptr );
        }

        void UISlider::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                auto ui = fb::static_pointer_cast<UIManager>( applicationManager->getRenderUI() );

                auto window = ui->getLayoutWindow();
                auto colibriManager = ui->getColibriManager();
                m_slider = colibriManager->createWidget<Colibri::Slider>( window );
                m_slider->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 1920, 1080 ) );

                setWidget( m_slider );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UISlider::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                m_slider = nullptr;
                UIElement<IUISlider>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    }  // namespace ui
}  // namespace fb
