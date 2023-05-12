#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIScrollingText.h>
#include <FBRenderUI/CUIScrollingTextElement.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIScrollingText::CUIScrollingText() :
            m_uiNumMenuItems( 0 ),
            m_uiNumListItems( 0 ),
            m_iItemIdx( 0 ),
            m_cursorIdx( 0 ),
            m_prevCursorIdx( 0 )
        {
            setType( String( "ScrollingText" ) );
        }

        CUIScrollingText::~CUIScrollingText()
        {
            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );
        }

        void CUIScrollingText::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

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

            // m_container = overlayManager->addElement(String("Panel"), getName());
            m_container->setMetricsMode( render::IOverlayElement::GMM_RELATIVE );

            //m_container->setLeft( left );
            //m_container->setTop( top );
            //m_container->setWidth( width );
            //m_container->setHeight( height );

            setPosition( Vector2F( left, top ) );
            setSize( Vector2F( width, height ) );

            // const String& parentType = parent->getType();
            // if (parentType == (String("Layout")))
            //{
            //	SmartPtr<render::IOverlay> overlay = overlayManager->findOverlay(parent->getName());
            //	overlay->addElement(m_container);
            // }
            // else
            //{
            //	SmartPtr<render::IOverlayElementContainer> cont;// =
            //overlayManager->findElement(parent->getName()); 	cont->addChild(m_container);
            // }

            bool isVisible = true;
            value = element->Attribute( "visible" );
            if( value == ( String( "false" ) ) )
                isVisible = false;

            if( parent )
                isVisible = isVisible && parent->isVisible();

            setVisible( isVisible );
        }

        bool CUIScrollingText::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            if( !isEnabled() )
                return false;

            // if(!isInFocus())
            //	return false;

            // switch(event->getEventType())
            //{
            // case IInputEvent::USER_INPUT_EVENT:
            // case IInputEvent::KEY_INPUT_EVENT:
            // case IInputEvent::JOYSTICK_INPUT_EVENT:
            //	{
            //		for(u32 i=0; i<m_inputListeners.size(); ++i)
            //		{
            //			IGUIItemInputListener* inputListener = m_inputListeners[i];
            //			if(inputListener->onEvent(event))
            //				return true;
            //		}

            //		/*GameInputMgrPtr& gameInputManager =
            //IApplicationManager::instance()->getGameInputManager(); 		GameInputDevicePtr gameInputDevice
            //= gameInputManager->findGameInputDevice(String("PlayerInput0"));
            //		if(gameInputDevice->getJustPressed(String("Up")))
            //		{
            //		decrementCursor();
            //		return true;
            //		}
            //		else if(gameInputDevice->getJustPressed(String("Down")))
            //		{
            //		increamentCursor();
            //		return true;
            //		}*/
            //	}
            //	break;
            // default:
            //	{
            //	}
            //};

            return CUIElement::handleEvent( event );
        }

        void CUIScrollingText::populateMenuItemList()
        {
            ////m_elements.set_used(0);
            // for (u32 i = 0; i < m_children.size(); ++i)
            //{
            //	CUIElement* child = (CUIElement*)m_children[i];
            //	if (child->getType() == (String("MenuItem")))
            //	{
            //		m_elements.push_back(child);
            //	}
            // }

            // m_uiNumListItems = m_elements.size();	//default value
            // m_uiNumMenuItems = m_elements.size();
        }

        void CUIScrollingText::increamentCursor()
        {
            int counter = 0;
            bool isMenuItemEnabled = false;
            while( !isMenuItemEnabled && counter < m_uiNumMenuItems )
            {
                m_prevCursorIdx = m_cursorIdx;
                m_cursorIdx++;

                WrapCursor();

                SmartPtr<CUIScrollingTextElement> pPrevMenuItem;  // = m_elements[m_prevCursorIdx];
                SmartPtr<CUIScrollingTextElement> pCurMenuItem;   // = m_elements[m_cursorIdx];

                pPrevMenuItem->setFocus( false );
                pPrevMenuItem->setHighlighted( false );
                pPrevMenuItem->setSelected( false );

                pCurMenuItem->setFocus( true );
                pCurMenuItem->setHighlighted( true );
                pCurMenuItem->setSelected( true );

                isMenuItemEnabled = pCurMenuItem->isEnabled();
                counter++;
            }
        }

        void CUIScrollingText::decrementCursor()
        {
            int counter = 0;
            bool isMenuItemEnabled = false;
            while( !isMenuItemEnabled && counter < m_uiNumMenuItems )
            {
                m_prevCursorIdx = m_cursorIdx;
                m_cursorIdx--;

                WrapCursor();

                SmartPtr<CUIScrollingTextElement> pPrevMenuItem;  // = m_elements[m_prevCursorIdx];
                SmartPtr<CUIScrollingTextElement> pCurMenuItem;   // = m_elements[m_cursorIdx];

                pPrevMenuItem->setFocus( false );
                pPrevMenuItem->setHighlighted( false );
                pPrevMenuItem->setSelected( false );

                pCurMenuItem->setFocus( true );
                pCurMenuItem->setHighlighted( true );
                pCurMenuItem->setSelected( true );

                isMenuItemEnabled = pCurMenuItem->isEnabled();
                counter++;
            }
        }

        int CUIScrollingText::getNumMenuItems() const
        {
            // populateMenuItemList();
            return (s32)m_elements.size();
        }

        void CUIScrollingText::WrapCursor()
        {
            s32 diff = m_uiNumListItems - m_uiNumMenuItems;

            if( m_iItemIdx <= diff && diff >= 0 )
            {
                if( m_cursorIdx >= static_cast<s32>( m_elements.size() ) && m_iItemIdx == diff )
                {
                    m_iItemIdx = 0;
                    m_cursorIdx = 0;
                }
                else if( m_cursorIdx >= static_cast<s32>( m_elements.size() ) )
                {
                    m_iItemIdx++;

                    if( m_iItemIdx > diff )
                    {
                        m_iItemIdx = diff;
                    }

                    m_cursorIdx = (s32)m_elements.size() - 1;
                }
                else if( m_cursorIdx < 0 && m_iItemIdx == 0 )
                {
                    m_iItemIdx = diff;
                    m_cursorIdx = (s32)m_elements.size() - 1;
                }
                else if( m_cursorIdx < 0 )
                {
                    m_iItemIdx--;

                    if( m_iItemIdx < 0 )
                    {
                        m_iItemIdx = 0;
                    }

                    m_cursorIdx = 0;
                }
            }
            else
            {
                if( m_cursorIdx >= static_cast<s32>( m_elements.size() ) ||
                    m_cursorIdx >= m_uiNumListItems )
                {
                    m_iItemIdx = 0;
                    m_cursorIdx = 0;
                }
                else if( m_cursorIdx < 0 )
                {
                    m_cursorIdx =
                        MathI::min( static_cast<s32>( m_elements.size() ), m_uiNumListItems ) - 1;
                }
            }

            // fail safe
            if( m_cursorIdx < 0 )
            {
                m_iItemIdx = 0;
                m_cursorIdx = 0;
            }
        }

        void CUIScrollingText::setCursorPosition( u32 cursorIdx )
        {
            // m_prevCursorIdx = m_cursorIdx;
            // m_cursorIdx = cursorIdx;
            // WrapCursor();

            // SmartPtr<CUIScrollingTextElement> pPrevMenuItem = m_elements[m_prevCursorIdx];
            // SmartPtr<CUIScrollingTextElement> pCurMenuItem = m_elements[m_cursorIdx];

            // pPrevMenuItem->setFocus(false);
            // pPrevMenuItem->setHighlighted(false);

            // pCurMenuItem->setFocus(true);
            // pCurMenuItem->setHighlighted(true);
        }

        u32 CUIScrollingText::getCursorPosition() const
        {
            return m_cursorIdx;
        }

        //
        // Callbacks
        //

        void CUIScrollingText::OnSelectPrevItem()
        {
        }

        void CUIScrollingText::OnSelectNextItem()
        {
        }

        void CUIScrollingText::OnWrapSelectionStart()
        {
        }

        void CUIScrollingText::OnWrapSelectionEnd()
        {
        }

        void CUIScrollingText::onAddChild( CUIElement *child )
        {
            populateMenuItemList();
        }

        void CUIScrollingText::onChildChangedState( CUIElement *child )
        {
            // check if menu items are enabled
            bool itemsEnabled = false;
            for( u32 i = 0; i < m_elements.size(); ++i )
            {
                IUIElement *menuItem = m_elements[i];
                if( menuItem->isEnabled() )
                {
                    itemsEnabled = true;
                    break;
                }
            }

            // check if menu item are visible
            bool itemsVisible = false;
            for( u32 i = 0; i < m_elements.size(); ++i )
            {
                IUIElement *menuItem = m_elements[i];
                if( menuItem->isVisible() )
                {
                    itemsVisible = true;
                    break;
                }
            }

            // todo change so that these of only trigger is the menu has no items to display
            // these are being trigger every time the menu is repopulated
            /*for(u32 i=0; i<m_listenerList.size(); ++i)
            {
            IGUIItemListener* listener = m_listenerList[i];
            if(listener->getType()==("IGUIMenuListener"))
            {
            IGUIMenuListener* menuListener = (IGUIMenuListener*)listener;
            if(!itemsEnabled)
            menuListener->onNoMenuItemsEnabled(this);

            if(!itemsVisible)
            menuListener->onNoMenuItemsVisible(this);
            }
            }*/

            CUIElement::onChildChangedState( child );
        }

        void CUIScrollingText::onToggleVisibility()
        {
            if( isVisible() )
            {
                // m_container->show();

                if( m_cursorIdx < static_cast<s32>( m_elements.size() ) )
                {
                    if( isInFocus() )
                    {
                        SmartPtr<CUIScrollingTextElement> pCurMenuItem;  // = m_elements[m_cursorIdx];
                        pCurMenuItem->setFocus( true );
                        pCurMenuItem->setHighlighted( true );
                    }
                }
            }
            else
            {
                // m_container->hide();
            }

            CUIElement::onToggleVisibility();
        }

        void CUIScrollingText::onGainFocus()
        {
            if( m_elements.size() > 0 )
            {
                SmartPtr<CUIScrollingTextElement> pCurMenuItem;  // = m_elements[m_cursorIdx];
                pCurMenuItem->setFocus( true );
                pCurMenuItem->setHighlighted( true );
                pCurMenuItem->setSelected( true );
            }
            else
            {
                FB_ASSERT_TRUE( true );
                printf( "CGUIScrollingText::onGainFocus - Error" );
            }
        }

        void CUIScrollingText::onLostFocus()
        {
            if( m_elements.size() > 0 )
            {
                SmartPtr<CUIScrollingTextElement> pCurMenuItem;  // = m_elements[m_cursorIdx];
                pCurMenuItem->setFocus( false );
                pCurMenuItem->setHighlighted( false );
                pCurMenuItem->setSelected( false );
            }
            else
            {
                FB_ASSERT_TRUE( true );
                printf( "CGUIScrollingText::onLostFocus - Error" );
            }
        }

        void CUIScrollingText::setNumListItems( u32 numListItems )
        {
            // m_uiNumListItems = numListItems;

            ////quick hack to test to save renaming functions
            // if (m_uiNumListItems == 0)
            //{
            //	for (u32 i = 0; i < m_listeners.size(); ++i)
            //	{
            //		IUIElementListener* listener = m_listeners[i];
            //		if (listener->getType() == (String("IGUIMenuListener")))
            //		{
            //			IUIMenuListener* menuListener = (IUIMenuListener*)listener;
            //			//if(!itemsEnabled)
            //			menuListener->onNoMenuItemsEnabled(this);

            //			//if(!itemsVisible)
            //			menuListener->onNoMenuItemsVisible(this);
            //		}
            //	}
            //}
        }

        u32 CUIScrollingText::getNumListItems() const
        {
            return m_uiNumListItems;
        }

        void CUIScrollingText::setCurrentItemIndex( u32 index )
        {
            m_iItemIdx = index;
        }

        u32 CUIScrollingText::getCurrentItemIndex() const
        {
            return m_iItemIdx;
        }

        void CUIScrollingText::OnEnterState( u8 state )
        {
            switch( state )
            {
            case STS_START:
                break;
            default:
            {
            }
            }
        }

        void CUIScrollingText::OnUpdateState( u8 state )
        {
            switch( state )
            {
            case STS_START:
                break;
            default:
            {
            }
            }
        }

        void CUIScrollingText::OnLeaveState( u8 state )
        {
            switch( state )
            {
            case STS_START:
                break;
            default:
            {
            }
            }
        }
    }  // namespace ui
}  // end namespace fb
