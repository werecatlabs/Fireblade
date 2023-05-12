#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIScrollingTextElement.h>
#include <FBRenderUI/CUIManager.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIScrollingTextElement::CUIScrollingTextElement()
        {
            setType( String( "ScrollingTxtElement" ) );
        }

        CUIScrollingTextElement::~CUIScrollingTextElement()
        {
            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );
        }

        void CUIScrollingTextElement::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            const TiXmlElement *element = pNode->ToElement();
            FB_ASSERT_TRUE( !element );
            const String id( element->Attribute( "id" ) );
            if( id.length() > 0 )
            {
                setName( id );
            }

            String value;
            f32 left;
            f32 top;
            f32 width;
            f32 height;

            value = element->Attribute( "posX" );
            sscanf( value.c_str(), "%f", &left );
            value = element->Attribute( "posY" );
            sscanf( value.c_str(), "%f", &top );

            value = element->Attribute( "width" );
            sscanf( value.c_str(), "%f", &width );
            value = element->Attribute( "height" );
            sscanf( value.c_str(), "%f", &height );

            String material;
            material = element->Attribute( "material" );

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

            // m_container = overlayManager->addElement(String("Panel"), getName());

            SmartPtr<render::IOverlayElementContainer>
                cont;  // = overlayManager->findElement(parent->getName());
            cont->addChild( m_container );

            //m_container->setLeft( left );
            //m_container->setTop( top );
            //m_container->setWidth( width );
            //m_container->setHeight( height );

            setPosition( Vector2F( left, top ) );
            setSize( Vector2F( width, height ) );

            String halignment( element->Attribute( "halign" ) );
            if( halignment == ( String( "center" ) ) )
            {
                // m_pOverlayElement->setHorizontalAlignment(Ogre::GHA_CENTER);
            }

            bool isVisible = true;
            value = String( element->Attribute( "visible" ) );
            if( value == ( String( "false" ) ) )
                isVisible = false;

            if( parent )
                isVisible = isVisible && parent->isVisible();

            setVisible( isVisible );

            auto zorder = m_container->getZOrder();
            m_container->setZOrder( zorder + (u32)parent->getChildren()->size() + 1 );
        }

        bool CUIScrollingTextElement::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            bool eventHandled = false;

            if( !isEnabled() )
                return eventHandled;

            // switch(event->getEventType())
            //{
            // case IInputEvent::MOUSE_INPUT_EVENT:
            // case IInputEvent::USER_INPUT_EVENT:
            // case IInputEvent::KEY_INPUT_EVENT:
            // case IInputEvent::JOYSTICK_INPUT_EVENT:
            //	{
            //	}
            //	break;
            // default:
            //	{
            //	}
            // };

            CUIElement::handleEvent( event );
            return eventHandled;
        }

        //
        // Callbacks
        //

        void CUIScrollingTextElement::onToggleVisibility()
        {
            m_container->setVisible( isVisible() );

            CUIElement::onToggleVisibility();
        }

        void CUIScrollingTextElement::onToggleHighlight()
        {
            CUIElement::onToggleHighlight();
        }
    }  // namespace ui
}  // end namespace fb
