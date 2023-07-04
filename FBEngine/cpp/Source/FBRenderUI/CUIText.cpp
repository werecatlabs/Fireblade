#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIText.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, CUIText, CUIElement<IUIText> );

        CUIText::CUIText()
        {
            createStateContext();
            setType( String( "Text" ) );
        }

        CUIText::~CUIText()
        {
            unload( nullptr );
        }

        void CUIText::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    removeAllChildren();

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    auto overlayManager = graphicsSystem->getOverlayManager();
                    FB_ASSERT( overlayManager );

                    if( m_overlayText )
                    {
                        overlayManager->removeElement( m_overlayText );
                        m_overlayText = nullptr;
                    }

                    CUIElement<IUIText>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CUIText::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlayManager = graphicsSystem->getOverlayManager();
                FB_ASSERT( overlayManager );

                // String captionTxt;
                // String value;
                // String fontName("BlueHighway");
                // f32 charHeight = 0.1f;
                // String alignment("center");

                // f32 left = 0.0f;
                // f32 top = 0.0f;
                // f32 width = 1.0f;
                // f32 height = 1.0f;

                // m_overlayTxt = overlayManager->addElement(String("TextArea"), getName());

                // m_overlayTxt->setFontName(fontName);
                // m_overlayTxt->setColour(ColourF::White);

                // m_overlayTxt->setCharHeight(charHeight);

                // auto sAlignment = "center";

                // if (sAlignment == String("center"))
                //{
                //	m_overlayTxt->setAlignment(render::IOverlayElementText::Center);
                // }
                // else if (sAlignment == String("left"))
                //{
                //	m_overlayTxt->setAlignment(render::IOverlayElementText::Left);
                // }
                // else if (sAlignment == String("right"))
                //{
                //	m_overlayTxt->setAlignment(render::IOverlayElementText::Right);
                // }

                // m_overlayTxt->setCaption(captionTxt);

                // m_overlayTxt->setLeft(left);
                // m_overlayTxt->setTop(top);
                // m_overlayTxt->setWidth(width);
                // m_overlayTxt->setHeight(height);

                m_overlayText = overlayManager->addElement(
                    "TextArea", "TextArea_" + StringUtil::toString( m_nextGeneratedNameExt++ ) );
                // panel->addChild(text);
                m_overlayText->setHorizontalAlignment( 
                    static_cast<u8>( render::IOverlayElementText::Alignment::Center ) );

                m_overlayText->setCaption( "Test" );

                setOverlayElement( m_overlayText );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CUIText::setText( const String &text )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    return state->setCaption( text );
                }
            }
        }

        String CUIText::getText() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    return state->getCaption();
                }
            }

            return "";
        }

        void CUIText::setTextSize( f32 textSize )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    state->setTextSize( (u32)textSize );
                }
            }
        }

        f32 CUIText::getTextSize() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<UIElementState>() )
                {
                    return (f32)state->getTextSize();
                }
            }

            return 0.0f;
        }

        void CUIText::setVerticalAlignment( u8 alignment )
        {
            if( m_overlayText )
            {
                m_overlayText->setVerticalAlignment( alignment );
            }
        }

        u8 CUIText::getVerticalAlignment() const
        {
            if( m_overlayText )
            {
                return m_overlayText->getVerticalAlignment();
            }

            return 0;
        }

        //
        // Callbacks
        //

        void CUIText::onToggleVisibility()
        {
            if( auto overlayText = getOverlayText() )
            {
                overlayText->setVisible( isVisible() );
            }

            CUIElement::onToggleVisibility();
        }

        void CUIText::onToggleHighlight()
        {
            if( auto overlayText = getOverlayText() )
            {
                if( isHighlighted() )
                {
                    overlayText->setColour( ColourF( 1, 0, 0, 1 ) );
                }
                else
                {
                    overlayText->setColour( ColourF( 1, 1, 1, 1 ) );
                }
            }

            CUIElement::onToggleHighlight();
        }

        SmartPtr<render::IOverlayElementText> CUIText::getOverlayText() const
        {
            return m_overlayText;
        }

        void CUIText::setOverlayText( SmartPtr<render::IOverlayElementText> overlayText )
        {
            m_overlayText = overlayText;
        }

        void CUIText::setPosition( const Vector2F &position )
        {
            if( auto overlayText = getOverlayText() )
            {
                overlayText->setPosition( position );
            }

            CUIElement::setPosition( position );
        }

        void CUIText::setSize( const Vector2F &size )
        {
            if( auto overlayText = getOverlayText() )
            {
                overlayText->setSize( size );
            }

            CUIElement::setSize( size );
        }

        void CUIText::_getObject( void **ppObject ) const
        {
            *ppObject = m_overlayText.get();
        }

        void CUIText::handleStateChanged( SmartPtr<IState> &state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto elementState = fb::static_pointer_cast<UIElementState>( state );

            auto caption = elementState->getCaption();
            auto colour = elementState->getColour();
            auto textSize = elementState->getTextSize();

            if( auto overlayText = getOverlayText() )
            {
                overlayText->setCaption( caption );
                overlayText->setColour( colour );
                overlayText->setCharHeight( (f32)textSize / 1080.0f );
            }

            CUIElement<IUIText>::handleStateChanged( state );
        }

    }  // end namespace ui
}  // end namespace fb
