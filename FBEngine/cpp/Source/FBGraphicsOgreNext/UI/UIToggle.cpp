#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UIToggle.h>
#include "FBGraphicsOgreNext/UI/UIManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/ColibriCheckbox.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {

        FB_CLASS_REGISTER_DERIVED( fb::ui, UIToggle, UIElement<IUIToggle> );

        UIToggle::UIToggle() : m_text( "Text" )
        {
            createStateContext();
        }

        UIToggle::~UIToggle()
        {
            unload( nullptr );
        }

        void UIToggle::load( SmartPtr<ISharedObject> data )
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
                m_checkbox = colibriManager->createWidget<Colibri::Checkbox>( window );
                //m_checkbox->setText( "Text" );

                m_checkbox->m_minSize = Ogre::Vector2( 350, 32 );
                m_checkbox->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 1920, 1080 ) );
                //m_checkbox->sizeToFit();

                setWidget( m_checkbox );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIToggle::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                m_checkbox = nullptr;
                UIElement<ui::IUIToggle>::unload( data );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIToggle::setLabel( const String &text )
        {
            m_text = text;
            if( m_checkbox )
            {
                //m_checkbox->setText( text );
            }
        }

        String UIToggle::getLabel() const
        {
            return m_text;
        }

        void UIToggle::setToggled( bool checked )
        {
            m_checked = checked;
            if( m_checkbox )
            {
                //m_checkbox->setChecked( checked );
            }
        }

        bool UIToggle::isToggled() const
        {
            return m_checked;
        }

        void UIToggle::handleStateChanged( SmartPtr<IState> &state )
        {
            try
            {
                UIElement<IUIToggle>::handleStateChanged( state );

                if( auto stateContext = getStateContext() )
                {
                    if( m_checkbox )
                    {
                        //m_checkbox->setText( m_text );
                        //m_checkbox->setChecked( m_checked );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIToggle::setTextSize( f32 textSize )
        {
        }

        fb::f32 UIToggle::getTextSize() const
        {
            return 0.0f;
        }

        fb::ui::IUIToggle::ToggleType UIToggle::getToggleType() const
        {
            return m_toggleType;
        }

        void UIToggle::setToggleType( ToggleType toggleType )
        {
            m_toggleType = toggleType;
        }

        fb::ui::IUIToggle::ToggleState UIToggle::getToggleState() const
        {
            return m_toggleState;
        }

        void UIToggle::setToggleState( ToggleState toggleState )
        {
            m_toggleState = toggleState;
        }

        bool UIToggle::getShowLabel() const
        {
            return m_showLabel;
        }

        void UIToggle::setShowLabel( bool showLabel )
        {
            m_showLabel = showLabel;
        }

    }  // namespace ui
}  // namespace fb
