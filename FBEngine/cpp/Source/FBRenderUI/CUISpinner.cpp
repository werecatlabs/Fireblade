#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUISpinner.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb::ui
{

    CUISpinner::CUISpinner()
    {
        setType( String( "Text" ) );
        m_valueType = String( "int" );
    }

    CUISpinner::~CUISpinner()
    {
        removeAllChildren();

        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
        overlayManager->removeElement( m_overlayTxt );
    }

    void CUISpinner::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
    {
        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

        const TiXmlElement *element = pNode->ToElement();
        FB_ASSERT_TRUE( !element );
        {
            const String id( element->Attribute( "id" ) );
            if( id.length() > 0 )
            {
                setName( id );
            }
        }

        m_text = String( element->Attribute( "text" ) );

        String value;

        f32 left;
        f32 top;
        f32 width;
        f32 height;

        value = String( element->Attribute( "posX" ) );
        sscanf( value.c_str(), "%f", &left );
        value = String( element->Attribute( "posY" ) );
        sscanf( value.c_str(), "%f", &top );

        value = String( element->Attribute( "width" ) );
        sscanf( value.c_str(), "%f", &width );
        value = String( element->Attribute( "height" ) );
        sscanf( value.c_str(), "%f", &height );

        StringUtil::parseArray( String( element->Attribute( "values" ) ), m_values );

        SmartPtr<render::IOverlayElementContainer>
            container;  // = overlayManager->addElement(String("Panel"), getName());

        String txtId = getName() + String( "Txt" );
        // m_overlayTxt = overlayManager->addElement(String("TextArea"), txtId);

        SmartPtr<render::IOverlayElementContainer>
            cont;  // = overlayManager->findElement(parent->getName());
        cont->addChild( container );
        container->addChild( m_overlayTxt );

        m_overlayTxt->setFontName( String( "BlueHighway" ) );

        value = String( element->Attribute( "charHeight" ) );
        f32 charHeight = 0.1f;
        if( value.length() > 0 )
        {
            sscanf( value.c_str(), "%f", &charHeight );
        }

        m_overlayTxt->setCharHeight( charHeight );

        //String alignment( element->Attribute( "alignment" ) );
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

        m_overlayTxt->setCaption( m_text );

        //m_overlayTxt->setLeft( 0 );
        //m_overlayTxt->setTop( 0 );
        //m_overlayTxt->setWidth( width );
        //m_overlayTxt->setHeight( height );

        //container->setLeft( left );
        //container->setTop( top );
        //container->setWidth( width );
        //container->setHeight( height );

        auto zorder = m_overlayTxt->getZOrder();
        auto newZOrder = zorder + static_cast<u32>( parent->getChildren()->size() ) + 1;

        m_overlayTxt->setZOrder( newZOrder );

        m_increament = 1.0;
        m_value = 1.0;
        m_minValue = 0.0;
        m_maxValue = 99.0;

        if( !m_values.empty() )
        {
            m_minValue = 0.0;
            m_maxValue = static_cast<f64>( m_values.size() - 1 );
            // m_value = m_values.find_element_index(m_text);
        }
    }

    auto CUISpinner::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        if( !isEnabled() )
        {
            return false;
        }

        if( !isInFocus() )
        {
            return false;
        }

        //	switch (event->getEventType())
        //	{
        //	case IInputEvent::USER_INPUT_EVENT:
        //	case IInputEvent::KEY_INPUT_EVENT:
        //	case IInputEvent::JOYSTICK_INPUT_EVENT:
        //		{
        //			for (u32 i = 0; i < m_inputListeners.size(); ++i)
        //			{
        //				IGUIItemInputListener* inputListener = m_inputListeners[i];
        //				//if(inputListener->onEvent(event))
        //				//	return true;
        //			}

        //			/*
        //			GameInputMgrPtr& gameInputManager =
        //IApplicationManager::instance()->getGameInputManager(); 			SmartPtr<IGameInput>
        //gameInputDevice = gameInputManager->findGameInput(event.DeviceId); 			u32 leftBtnId =
        //gameInputManager->getActionId("Left"); 			u32 rightBtnId =
        //gameInputManager->getActionId("Right");
        //
        //			if(gameInputDevice->getJustPressed(leftBtnId))
        //			{
        //				decrementValue();
        //				return true;
        //			}
        //			else if(gameInputDevice->getJustPressed(rightBtnId))
        //			{
        //				increamentValue();
        //				return true;
        //			}
        //			*/
        //		}
        //		break;
        //	default:
        //		{
        //		}
        //	}

        return CUIElement::handleEvent( event );
    }

    void CUISpinner::incrementValue()
    {
        m_value += m_increament;

        if( m_value > m_maxValue )
        {
            m_value = m_maxValue;
        }

        if( m_values.empty() )
        {
            m_text = StringUtil::toString( m_value );
            m_overlayTxt->setCaption( m_text );
        }
        else
        {
            auto index = static_cast<u32>( m_value );
            m_text = m_values[index];

            m_overlayTxt->setCaption( m_text );
        }

        onChangedState();
    }

    void CUISpinner::decrementValue()
    {
        m_value -= m_increament;
        if( m_value < m_minValue )
        {
            m_value = m_minValue;
        }

        if( m_values.empty() )
        {
            m_text = StringUtil::toString( m_value );
            m_overlayTxt->setCaption( m_text );
        }
        else
        {
            auto index = static_cast<u32>( m_value );
            m_text = m_values[index];

            m_overlayTxt->setCaption( m_text );
        }

        onChangedState();
    }

    void CUISpinner::setValue( const String &value )
    {
        float newValue = 0.f;
        sscanf( value.c_str(), "%f", &newValue );
        m_value = newValue;
        if( m_value < m_minValue )
        {
            m_value = m_minValue;
        }
        else if( m_value > m_maxValue )
        {
            m_value = m_maxValue;
        }

        if( m_valueType == ( String( "int" ) ) )
        {
            m_text = StringUtil::toString( m_value );
        }
        else
        {
            m_text = StringUtil::toString( m_value );
        }

        m_overlayTxt->setCaption( m_text );
    }

    auto CUISpinner::getValue() const -> String
    {
        return m_text;
    }

    void CUISpinner::setMinValue( const String &value )
    {
        float newValue = 0.f;
        sscanf( value.c_str(), "%f", &newValue );
        m_minValue = newValue;
    }

    auto CUISpinner::getMinValue() const -> String
    {
        return StringUtil::toString( m_minValue );
    }

    void CUISpinner::setMaxValue( const String &value )
    {
        float newValue = StringUtil::parseFloat( value );
        m_maxValue = newValue;
    }

    auto CUISpinner::getMaxValue() const -> String
    {
        return StringUtil::toString( m_maxValue );
    }

    void CUISpinner::setText( const String &text )
    {
        m_text = text;
        m_overlayTxt->setCaption( m_text );
    }

    auto CUISpinner::getText() const -> String
    {
        return m_text;
    }

    void CUISpinner::onToggleVisibility()
    {
        m_overlayTxt->setVisible( isVisible() );

        CUIElement::onToggleVisibility();
    }

    void CUISpinner::onToggleHighlight()
    {
        if( isHighlighted() )
        {
            m_overlayTxt->setColour( ColourF( 1, 0, 0, 1 ) );
        }
        else
        {
            m_overlayTxt->setColour( ColourF( 1, 1, 1, 1 ) );
        }

        CUIElement::onToggleHighlight();
    }
}  // namespace fb::ui
