#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIDialogBox.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIDialogBox::CUIDialogBox()
        {
        }

        CUIDialogBox::~CUIDialogBox()
        {
            removeAllChildren();

            // Ogre::OverlayManager* pOverlayManager = Ogre::OverlayManager::getSingletonPtr();
            // pOverlayManager->destroyOverlayElement(m_pContainer);
        }

        void CUIDialogBox::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            // Ogre::OverlayManager* pOverlayManager = Ogre::OverlayManager::getSingletonPtr();

            // const TiXmlElement* element = pNode->ToElement();
            // FB_ASSERT_TRUE(!element);
            // const String id(element->Attribute("id"));
            // if(id.length() > 0)
            //{
            //	setName(id);
            // }

            // String value;
            // f32 left;
            // f32 top;
            // f32 width;
            // f32 height;

            // value = element->Attribute("posX");
            // sscanf(value.c_str(), "%f", &left);
            // value = element->Attribute("posY");
            // sscanf(value.c_str(), "%f", &top);

            // value = element->Attribute("width");
            // sscanf(value.c_str(), "%f", &width);
            // value = element->Attribute("height");
            // sscanf(value.c_str(), "%f", &height);

            // m_pContainer = (Ogre::OverlayContainer*)pOverlayManager->createOverlayElement("Panel",
            // getName().c_str()); m_pContainer->setMetricsMode(Ogre::GMM_RELATIVE);

            // m_pContainer->setLeft(left);
            // m_pContainer->setTop(top);
            // m_pContainer->setWidth(width);
            // m_pContainer->setHeight(height);

            // Ogre::Overlay* overlay = pOverlayManager->getByName(parent->getName().c_str());
            // if(overlay)
            //{
            //	overlay->add2D(m_pContainer);
            // }
        }

        bool CUIDialogBox::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            if( !isEnabled() )
                return false;

            // switch(event->getEventType())
            //{
            // case IInputEvent::USER_INPUT_EVENT:
            //	{
            //		/*GameInputMgrPtr& gameInputManager =
            //IApplicationManager::instance()->getGameInputManager(); 		GameInputDevicePtr gameInputDevice
            //= gameInputManager->findGameInputDevice(String("PlayerInput0")); 		bool result =
            //gameInputDevice->getJustPressed(String("Action0")); 		if(result)
            //		{
            //		//hack for testing
            //		onActivate();
            //		setVisible(false);
            //		setEnabled(false);
            //		return true;
            //		}*/
            //	}
            //	break;
            // default:
            //	{
            //	}
            // };

            CUIElement::handleEvent( event );
            return false;
        }

        void CUIDialogBox::setScrollSpeed( s32 scroll_speed )
        {
        }

        String CUIDialogBox::getString()
        {
            return m_Text[0];
        }

        void CUIDialogBox::OnScroll()
        {
        }

        void CUIDialogBox::OnClose()
        {
        }

        void CUIDialogBox::onToggleVisibility()
        {
            // if(m_isVisible)
            //	m_pContainer->show();
            // else
            //	m_pContainer->hide();

            CUIElement::onToggleVisibility();
        }
    }  // namespace ui
}  // end namespace fb
