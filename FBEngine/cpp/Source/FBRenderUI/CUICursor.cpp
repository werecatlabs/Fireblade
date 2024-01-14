#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUICursor.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{

    CUICursor::CUICursor()
    {
        m_type = "Container";
    }

    CUICursor::~CUICursor()
    {
        removeAllChildren();

        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
        overlayManager->removeElement( m_container );
    }

    void CUICursor::initialise()
    {
        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
        SmartPtr<render::IOverlay> overlay = overlayManager->addOverlay( String( "CursorOverlay" ) );
        overlay->setZOrder( 650 );

        String id( "CGUICursor" );
        setName( id );

        String value;
        f32 left = 0.0f;
        f32 top = 0.0f;
        f32 width = 0.2f;
        f32 height = 0.2f;

        /*value = element->Attribute("posX");
        sscanf(value.c_str(), "%f", &left);
        value = element->Attribute("posY");
        sscanf(value.c_str(), "%f", &top);

        value = element->Attribute("width");
        sscanf(value.c_str(), "%f", &width);
        value = element->Attribute("height");
        sscanf(value.c_str(), "%f", &height);*/

        // m_container = overlayManager->addElement(String("Panel"), id);
        m_container->setMetricsMode( render::IOverlayElement::GMM_RELATIVE );

        //m_container->setMaterialName( String( "BaseWhiteNoLighting" ) );

        //m_container->setLeft( left );
        //m_container->setTop( top );
        //m_container->setWidth( width );
        //m_container->setHeight( height );

        setPosition( Vector2F( left, top ) );
        setSize( Vector2F( width, height ) );

        overlay->addElement( m_container );

        overlay->setVisible( isVisible() );
        m_container->setVisible( isVisible() );
    }

    auto CUICursor::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        // switch( event->getEventType() )
        //{
        // case IInputEvent::MOUSE_INPUT_EVENT:
        //	{
        //		//Mouse event
        //		if (event->getMouseState()->getEventType() ==
        //IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
        //		{
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
        //			Vector2F position(event->getMouseState()->getRelativePosition().X(),
        //event->getMouseState()->getRelativePosition().Y()); 			setPosition(position);

        //			Vector2F size = getSize();

        //			m_container->setLeft(position.X());
        //			m_container->setTop(position.Y());
        //			//m_pContainer->setWidth(size.X());
        //			//m_pContainer->setHeight(size.Y());
        //		}
        //	}
        //	break;
        // default:
        //	{
        //	}
        //};

        return false;
    }

    void CUICursor::setMaterialName( const String &materialName )
    {
        //m_container->setMaterialName( materialName );
    }

    void CUICursor::setSize( const Vector2F &size )
    {
        CUIElement::setSize( size );

        //m_container->setWidth( size.X() );
        //m_container->setHeight( size.Y() );
    }

    void CUICursor::onToggleVisibility()
    {
        m_container->setVisible( isVisible() );

        CUIElement::onToggleVisibility();
    }
}  // namespace fb::ui
