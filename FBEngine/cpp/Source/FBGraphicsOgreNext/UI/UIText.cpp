#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UIText.h>
#include "FBGraphicsOgreNext/UI/UIManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {

        UIText::UIText()
            : m_text("Text")
        {
            createStateContext();
        }

        UIText::~UIText()
        {
            unload( nullptr );
        }

        void UIText::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto ui = fb::static_pointer_cast<UIManager>( applicationManager->getRenderUI() );

                auto window = ui->getLayoutWindow();
                auto colibriManager = ui->getColibriManager();
                m_label = colibriManager->createWidget<Colibri::Label>( window );
                m_label->setText( "Text" );
                //
                m_label->m_minSize = Ogre::Vector2( 350, 32 );
                m_label->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 1920, 1080 ) );
                m_label->sizeToFit();

                setWidget( m_label );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIText::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                m_label = nullptr;
                UIElement<ui::IUIText>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIText::setText( const String &text )
        {
            m_text = text;

            if( auto stateObject = getStateObject() )
            {
                stateObject->setDirty( true );
            }
        }

        String UIText::getText() const
        {
            return m_text;
        }

        void UIText::setTextSize( f32 textSize )
        {
            m_textSize = textSize;
        }

        f32 UIText::getTextSize() const
        {
            return m_textSize;
        }

        void UIText::setVerticalAlignment( u8 alignment )
        {
            m_verticalAlignment = alignment;
        }

        u8 UIText::getVerticalAlignment() const
        {
            return m_verticalAlignment;
        }

        void UIText::setHorizontalAlignment( u8 alignment )
        {
            m_horizontalAlignment = alignment;
        }

        u8 UIText::getHorizontalAlignment() const
        {
            return m_horizontalAlignment;
        }

        void UIText::handleStateChanged( SmartPtr<IState> &state )
        {
            m_label->setDefaultFontSize( Colibri::FontSize( m_textSize ) );
            m_label->setText( m_text );
            m_label->setTextVertAlignment(
                (Colibri::TextVertAlignment::TextVertAlignment)m_verticalAlignment );
            m_label->setTextHorizAlignment(
                (Colibri::TextHorizAlignment::TextHorizAlignment)m_horizontalAlignment );
        }

    }  // namespace ui
}  // namespace fb
