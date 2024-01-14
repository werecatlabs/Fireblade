#include <FBRenderUI/FBRenderUIPCH.h>
#include "FBRenderUI/CUIToggleButton.h"
#include "FBRenderUI/CUIManager.h"
#include "FBRenderUI/CUIItemTemplate.h"
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb::ui
{

    CUIToggleButton::CUIToggleButton() : m_toggleGroup( nullptr ), m_isToggled( false )
    {
        m_type = "ToggleButton";
    }

    CUIToggleButton::~CUIToggleButton()
    {
        // if(!m_toggleGroup.isNull())
        //{
        //	m_toggleGroup->removeToggleButton(this);
        // }
    }

    void CUIToggleButton::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
    {
        // CUIButton::initialise(parent, pNode);

        const TiXmlElement *element = pNode->ToElement();
        FB_ASSERT_TRUE( !element );

        const String templateId( element->Attribute( "template" ) );
        if( templateId.length() > 0 )
        {
            SmartPtr<CUIItemTemplate>
                templateItem;  // = CGUIManager::getSingletonPtr()->findElement(templateId);
            if( templateItem != nullptr )
            {
                Properties &propertyGroup = templateItem->getPropertyGroup();
                propertyGroup.getPropertyValue( String( "toggleMaterial" ), m_toggleMaterial );
            }
            else
            {
                String message = String( "Could not find template: " ) + String( templateId );
                // MessageBoxUtil::show(message.c_str());
            }
        }

        const char *toggleMaterial = element->Attribute( "toggleMaterial" );
        if( toggleMaterial )
        {
            m_toggleMaterial = toggleMaterial;
        }

        const String toggleGroupId( element->Attribute( "toggleGroup" ) );
        if( toggleGroupId.length() > 0 )
        {
            // CGUIManager* guiManager = CGUIManager::getSingletonPtr();

            // if (guiManager->getRoot()->hasChildById(toggleGroupId))
            //{
            //	SmartPtr<CGUIToggleGroup> toggleGroup;// = guiManager->findElement(toggleGroupId);
            //	toggleGroup->addToggleButton(this);

            //	m_toggleGroup = toggleGroup;
            //}
            // else
            //{
            //	SmartPtr<CGUIToggleGroup> toggleGroup;// = new CGUIToggleGroup(toggleGroupId);
            //	guiManager->getRoot()->addChild(toggleGroup.get());

            //	toggleGroup->addToggleButton(this);

            //	m_toggleGroup = toggleGroup;
            //}
        }
    }

    auto CUIToggleButton::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        // switch (event->getEventType())
        //{
        // case IInputEvent::MOUSE_INPUT_EVENT:
        //{
        //	// mouse
        //	const SmartPtr<IMouseState>& mouseState = event->getMouseState();

        //	if (event->getMouseState()->getEventType() ==
        //IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
        //	{
        //		Vector2F point(event->getMouseState()->getRelativePosition().X(),
        //event->getMouseState()->getRelativePosition().Y()); 		Vector2F postion =
        //getAbsolutePosition(); 		AABB2<f32> rect(postion, postion + getSize()); 		if
        //(rect.isInside(point))
        //		{
        //			if (m_isToggled)
        //			{
        //				if (m_toggleGroup != nullptr)
        //				{
        //					if (m_toggleGroup->getCurToggledButton() != this)
        //					{
        //						setToggled(false);
        //					}
        //				}
        //				else
        //				{
        //					setToggled(false);
        //				}
        //			}
        //			else
        //			{
        //				setToggled(true);

        //				if (m_toggleGroup != nullptr)
        //					m_toggleGroup->OnSetButtonToggled(this);
        //			}

        //			//getLayout()->onActivate(this);

        //			return true;
        //		}
        //	}
        //	else if (event->getMouseState()->getEventType() ==
        //IMouseState::MOUSE_EVENT_LEFT_RELEASED)
        //	{
        //	}
        //	else if (event->getMouseState()->getEventType() ==
        //IMouseState::MOUSE_EVENT_MIDDLE_BUTTON_PRESSED)
        //	{
        //	}
        //	else if (event->getMouseState()->getEventType() == IMouseState::MOUSE_EVENT_MOVED)
        //	{
        //		Vector2F point(event->getMouseState()->getRelativePosition().X(),
        //event->getMouseState()->getRelativePosition().Y()); 		Vector2F postion =
        //getAbsolutePosition(); 		AABB2<f32> rect(postion, postion + getSize()); 		if
        //(rect.isInside(point))
        //		{
        //			if (m_hoverMaterial.length() > 0)
        //				m_container->setMaterialName(m_hoverMaterial);
        //		}
        //		else
        //		{
        //			if (!m_isToggled)
        //				m_container->setMaterialName(m_defaultMaterial);
        //			else
        //				m_container->setMaterialName(m_toggleMaterial);
        //		}
        //	}
        //}
        // break;
        // default:
        //{
        //}
        //};

        return false;
    }

    void CUIToggleButton::setToggled( bool toggled )
    {
        // if (m_isToggled != toggled)
        //{
        //	m_isToggled = toggled;

        //	if (m_isToggled)
        //	{
        //		m_container->setMaterialName(m_toggleMaterial);
        //	}
        //	else
        //	{
        //		m_container->setMaterialName(m_defaultMaterial);
        //	}
        //}
    }

    auto CUIToggleButton::isToggled() const -> bool
    {
        return m_isToggled;
    }

    fb::ui::IUIToggle::ToggleType CUIToggleButton::getToggleType() const
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    void CUIToggleButton::setToggleType( ToggleType toggleType )
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    fb::ui::IUIToggle::ToggleState CUIToggleButton::getToggleState() const
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    void CUIToggleButton::setToggleState( ToggleState toggleState )
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    String CUIToggleButton::getLabel() const
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    void CUIToggleButton::setLabel( const String &label )
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    void CUIToggleButton::setTextSize( f32 textSize )
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

    fb::f32 CUIToggleButton::getTextSize() const
    {
        throw std::logic_error( "The method or operation is not implemented." );
    }

}  // namespace fb::ui
