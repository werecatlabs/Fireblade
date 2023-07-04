#include <FBRenderUI/FBRenderUIPCH.h>
#include "FBRenderUI/CUITextEntry.h"
#include "FBRenderUI/CUIManager.h"
#include "FBRenderUI/CUIItemTemplate.h"
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUITextEntry::CUITextEntry() : m_nextCursorFlash( 0 )
        {
            setType( String( "TextEntry" ) );
        }

        CUITextEntry::~CUITextEntry()
        {
            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_overlayTxt );
        }

        void CUITextEntry::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

            const TiXmlElement *element = pNode->ToElement();
            FB_ASSERT_TRUE( !element );
            {
                const String id( element->Attribute( "id" ) );
                if( id.length() > 0 )
                {
                    setName( id );
                }
                else
                {
                    int halt = 0;
                    halt = 0;
                }
            }

            String captionTxt;
            String value;
            String fontName( "BlueHighway" );
            f32 charHeight = 0.1f;
            String alignment;

            f32 left;
            f32 top;
            f32 width;
            f32 height;

            const String templateId( element->Attribute( "template" ) );
            if( templateId.length() > 0 )
            {
                SmartPtr<CUIItemTemplate>
                    templateItem;  // = CGUIManager::getSingletonPtr()->findElement(templateId);
                if( templateItem != nullptr )
                {
                    const Properties &propertyGroup = templateItem->getPropertyGroup();

                    propertyGroup.getPropertyValue( String( "width" ), width );
                    propertyGroup.getPropertyValue( String( "height" ), height );

                    propertyGroup.getPropertyValue( String( "text" ), captionTxt );

                    propertyGroup.getPropertyValue( String( "fontName" ), fontName );
                    propertyGroup.getPropertyValue( String( "charHeight" ), charHeight );
                    propertyGroup.getPropertyValue( String( "alignment" ), alignment );
                }
                else
                {
                    String message = String( "Could not find template: " ) + String( templateId );
                    // MessageBoxUtil::show(message.c_str());
                }
            }

            const String pCaptionTxt( element->Attribute( "text" ) );
            if( pCaptionTxt.length() > 0 )
            {
                captionTxt = pCaptionTxt;
            }

            value = element->Attribute( "posX" );
            sscanf( value.c_str(), "%f", &left );
            value = element->Attribute( "posY" );
            sscanf( value.c_str(), "%f", &top );

            value = element->Attribute( "width" );
            sscanf( value.c_str(), "%f", &width );
            value = element->Attribute( "height" );
            sscanf( value.c_str(), "%f", &height );

            // m_overlayTxt = overlayManager->addElement(String("TextArea"), getName());

            SmartPtr<render::IOverlayElementContainer>
                container;  // = overlayManager->findElement(parent->getName());
            container->addChild( m_overlayTxt );

            m_overlayTxt->setFontName( fontName );

            const String pCharHeightStr( element->Attribute( "charHeight" ) );
            if( pCharHeightStr.length() > 0 )
            {
                charHeight = StringUtil::parseFloat( pCharHeightStr );
            }

            m_overlayTxt->setCharHeight( charHeight );

            const String pAlignmentStr( element->Attribute( "alignment" ) );
            if( pAlignmentStr.length() > 0 )
            {
                alignment = pAlignmentStr;
            }

            //if( alignment == ( String( "center" ) ) )
            //{
            //    m_overlayTxt->setAlignment(
            //        static_cast<u8>( render::IOverlayElementText::Alignment::Center ) );
            //}
            //else if( alignment == ( String( "left" ) ) )
            //{
            //    m_overlayTxt->setAlignment(
            //        static_cast<u8>( render::IOverlayElementText::Alignment::Left ) );
            //}
            //else if( alignment == ( String( "right" ) ) )
            //{
            //    m_overlayTxt->setAlignment(
            //        static_cast<u8>( render::IOverlayElementText::Alignment::Right ) );
            //}

            m_overlayTxt->setCaption( captionTxt );

            //m_overlayTxt->setLeft( left );
            //m_overlayTxt->setTop( top );
            //m_overlayTxt->setWidth( width );
            //m_overlayTxt->setHeight( height );

            auto zorder = container->getZOrder();
            auto newZOrder = zorder + (u32)parent->getChildren()->size() + 1;
            m_overlayTxt->setZOrder( newZOrder );
        }

        bool CUITextEntry::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            // switch( event->getEventType() )
            //{
            // case IInputEvent::MOUSE_INPUT_EVENT:
            //	{
            //		//Mouse event
            //		if (event->getMouseState()->getEventType() ==
            //IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
            //		{
            //			Vector2F point(event->getMouseState()->getRelativePosition().X(),
            //event->getMouseState()->getRelativePosition().Y()); 			Vector2F postion =
            //getAbsolutePosition(); 			AABB2F rect(postion, postion + getSize()); 			if(rect.isInside(point))
            //			{
            //				//onActivate();
            //				return true;
            //			}
            //		}
            //		else if (event->getMouseState()->getEventType() ==
            //IMouseState::MOUSE_EVENT_LEFT_RELEASED)
            //		{
            //		}
            //		else if (event->getMouseState()->getEventType() ==
            //IMouseState::MOUSE_EVENT_MIDDLE_BUTTON_PRESSED )
            //		{
            //		}
            //		else if (event->getMouseState()->getEventType() == IMouseState::MOUSE_EVENT_MOVED )
            //		{
            //			Vector2F point(event->getMouseState()->getRelativePosition().X(),
            //event->getMouseState()->getRelativePosition().Y()); 			Vector2F postion =
            //getAbsolutePosition(); 			AABB2F rect(postion, postion + getSize()); 			if(rect.isInside(point))
            //			{
            //				//if(m_hoverMaterial.size() > 0)
            //				//	m_pOverlayElement->setMaterialName(m_hoverMaterial.c_str());
            //			}
            //			else
            //			{
            //				//m_pOverlayElement->setMaterialName(m_defaultMaterial.c_str());
            //			}
            //		}
            //	}
            //	break;
            // case IInputEvent::KEY_INPUT_EVENT:
            //	{
            //		if(event->getKeyboardState()->isPressedDown() &&
            //event->getKeyboardState()->getKeyCode())
            //		{
            //			//check for special keys
            //			//e.g. return, backspace etc

            //			String curString = getText();
            //			curString += event->getKeyboardState()->getChar();
            //			setText(curString);
            //		}
            //	}
            //	break;
            // default:
            //	{
            //	}
            //};

            return false;
        }

        void CUITextEntry::update()
        {
            auto timer = core::IApplicationManager::instance()->getTimer();
            m_nextCursorFlash = timer->getTimeMilliseconds() + 1000;
        }

        void CUITextEntry::setText( const String &text )
        {
            m_text = text;
            m_overlayTxt->setCaption( m_text );
        }

        String CUITextEntry::getText() const
        {
            return m_text;
        }

        void CUITextEntry::setTextSize( f32 textSize )
        {
        }

        //
        // Callbacks
        //

        f32 CUITextEntry::getTextSize() const
        {
            return 0.0f;
        }

        void CUITextEntry::onToggleVisibility()
        {
            m_overlayTxt->setVisible( isVisible() );

            CUIElement::onToggleVisibility();
        }

        void CUITextEntry::onToggleHighlight()
        {
            if( isHighlighted() )
                m_overlayTxt->setColour( ColourF( 1, 0, 0, 1 ) );
            else
                m_overlayTxt->setColour( ColourF( 1, 1, 1, 1 ) );

            CUIElement::onToggleHighlight();
        }
    }  // namespace ui
}  // end namespace fb
