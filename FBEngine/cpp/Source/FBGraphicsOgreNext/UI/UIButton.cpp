#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UIButton.h>
#include "FBGraphicsOgreNext/UI/UIManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriButton.h"
#include "ColibriGui/ColibriManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, UIButton, UIElement<IUIButton> );

        UIButton::UIButton() : m_label( "Button" )
        {
            createStateContext();
        }

        UIButton::~UIButton()
        {
            unload( nullptr );
        }

        void UIButton::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto ui = fb::static_pointer_cast<UIManager>( applicationManager->getRenderUI() );
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                ScopedLock lock( graphicsSystem );

                auto window = ui->getLayoutWindow();
                auto colibriManager = ui->getColibriManager();
                m_button = colibriManager->createWidget<Colibri::Button>( window );
                m_button->m_minSize = Ogre::Vector2( 350, 64 );
                m_button->getLabel()->setText( "button" );
                m_button->sizeToFit();

                setWidget( m_button );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIButton::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                m_button = nullptr;
                UIElement<ui::IUIButton>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String UIButton::getLabel() const
        {
            return m_label;
        }

        void UIButton::setLabel( const String &label )
        {
            m_label = label;

            if( auto stateContext = getStateContext() )
            {
                stateContext->setDirty( true );
            }
        }

        void UIButton::setTextSize( f32 textSize )
        {
        }

        f32 UIButton::getTextSize() const
        {
            return 0;
        }

        void UIButton::handleStateChanged( SmartPtr<IState> &state )
        {
            UIElement<IUIButton>::handleStateChanged( state );

            if( m_button )
            {
                auto label = m_button->getLabel();
                label->setText( m_label );
            }
        }

    }  // namespace ui
}  // namespace fb
