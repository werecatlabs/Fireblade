#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIMenuItem.h>
#include <FBRenderUI/CUIManager.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIMenuItem::CUIMenuItem()
        {
            setType( String( "MenuItem" ) );
        }

        CUIMenuItem::~CUIMenuItem()
        {
            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );
        }

        void CUIMenuItem::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            const TiXmlElement *element = pNode->ToElement();
            FB_ASSERT_TRUE( !element );
            const String id = XMLUtil::getString( element->Attribute( "id" ) );
            if( id.length() > 0 )
            {
                setName( id );
            }

            String value;
            f32 left;
            f32 top;
            f32 width;
            f32 height;

            value = XMLUtil::getString( element->Attribute( "posX" ) );
            sscanf( value.c_str(), "%f", &left );
            value = XMLUtil::getString( element->Attribute( "posY" ) );
            sscanf( value.c_str(), "%f", &top );

            value = XMLUtil::getString( element->Attribute( "width" ) );
            sscanf( value.c_str(), "%f", &width );
            value = XMLUtil::getString( element->Attribute( "height" ) );
            sscanf( value.c_str(), "%f", &height );

            String material = XMLUtil::getString( element->Attribute( "material" ) );

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            if( !overlayManager )
                return;

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

            String halignment = XMLUtil::getString( element->Attribute( "halign" ) );
            if( halignment == String( "center" ) )
            {
                // m_pOverlayElement->setHorizontalAlignment(Ogre::GHA_CENTER);
            }

            bool isVisible = true;
            value = XMLUtil::getString( element->Attribute( "visible" ) );
            if( value == String( "false" ) )
                isVisible = false;

            if( parent )
                isVisible = isVisible && parent->isVisible();

            setVisible( isVisible );

            auto zorder = m_container->getZOrder();
            m_container->setZOrder( zorder + (u32)parent->getChildren()->size() + 1 );
        }

        bool CUIMenuItem::handleEvent( const SmartPtr<IInputEvent> &event )
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

            return eventHandled || CUIElement::handleEvent( event );
        }

        //
        // Callbacks
        //

        void CUIMenuItem::onToggleVisibility()
        {
            m_container->setVisible( isVisible() );

            CUIElement::onToggleVisibility();
        }

        void CUIMenuItem::onToggleHighlight()
        {
            CUIElement::onToggleHighlight();
        }
    }  // namespace ui
}  // end namespace fb
