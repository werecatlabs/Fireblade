#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIMenu.h>
#include <FBRenderUI/CUIMenuItem.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIMenu::CUIMenu() :
            m_uiNumMenuItems( 0 ),
            m_uiNumListItems( 0 ),
            m_iItemIdx( 0 ),
            m_cursorIdx( 0 ),
            m_prevCursorIdx( 0 )
        {
        }

        CUIMenu::~CUIMenu()
        {
            removeAllChildren();

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto overlayManager = graphicsSystem->getOverlayManager();
            FB_ASSERT( overlayManager );

            overlayManager->removeElement( m_container );
        }

        void CUIMenu::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            if( !overlayManager )
                return;

            const TiXmlElement *element = pNode->ToElement();
            FB_ASSERT_TRUE( !element );
            const String id = "";  // XMLUtil::getString(element->Attribute("id"));
            if( id.length() > 0 )
            {
                setName( id );
            }

            String value;
            f32 left = StringUtil::parseFloat( String( element->Attribute( "posX" ) ) );
            f32 top = StringUtil::parseFloat( String( element->Attribute( "posY" ) ) );
            f32 width = StringUtil::parseFloat( String( element->Attribute( "width" ) ) );
            f32 height = StringUtil::parseFloat( String( element->Attribute( "height" ) ) );

            // m_container = overlayManager->addElement(String("Panel"), getName());
            m_container->setMetricsMode( render::IOverlayElement::GMM_RELATIVE );

            //m_container->setLeft( left );
            //m_container->setTop( top );
            //m_container->setWidth( width );
            //m_container->setHeight( height );

            setPosition( Vector2F( left, top ) );
            setSize( Vector2F( width, height ) );

            // auto parentType = parent->getType();
            // if (parentType == (String("Layout")))
            //{
            //	auto overlay = overlayManager->findOverlay(parent->getName());
            //	overlay->addElement(m_container);
            // }
            // else
            //{
            //	SmartPtr<render::IOverlayElementContainer> cont;// =
            //overlayManager->findElement(parent->getName()); 	cont->addChild(m_container);
            // }

            // bool isVisible = StringUtil::parseBool(XMLUtil::getString(element->Attribute("visible")));
            // if (parent)
            //{
            //	isVisible = isVisible && parent->isVisible();
            // }

            // setVisible(isVisible);
        }

        bool CUIMenu::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            if( !isEnabled() )
                return false;

            // if(!isInFocus())
            //	return false;

            // switch(event->getEventType())
            //{
            // case IInputEvent::USER_INPUT_EVENT:
            // case IInputEvent::KEY_INPUT_EVENT:
            //	{
            //		if(event->getKeyboardState()->isPressedDown())
            //		{
            //			if(event->getKeyboardState()->getKeyCode() == IKeyboardState::KEY_UP)
            //			{
            //				decrementCursor();
            //				return true;
            //			}
            //			else if(event->getKeyboardState()->getKeyCode() == IKeyboardState::KEY_DOWN)
            //			{
            //				increamentCursor();
            //				return true;
            //			}
            //		}
            //	}
            //	break;
            // case IInputEvent::JOYSTICK_INPUT_EVENT:
            //	{
            //		for(u32 i=0; i<m_inputListeners.size(); ++i)
            //		{
            //			IGUIItemInputListener* inputListener = m_inputListeners[i];
            //			if(inputListener->onEvent(event))
            //				return true;
            //		}

            //		switch(event->getJoystickState()->getEventType())
            //		{
            //		case IJoystickState::EVT_TYPE_BUTTON_PRESSED:
            //			{
            //				static u32 upBtnId = StringUtil::getHash("Up");
            //				static u32 downBtnId = StringUtil::getHash("Down");

            //				if(event->getGameInputState()->getAction() == upBtnId)
            //				{
            //					decrementCursor();
            //					return true;
            //				}
            //				else if(event->getGameInputState()->getAction() == downBtnId)
            //				{
            //					increamentCursor();
            //					return true;
            //				}
            //			}
            //			break;
            //		case IJoystickState::EVT_TYPE_BUTTON_RELEASED:
            //			{
            //			}
            //			break;
            //		case IJoystickState::EVT_TYPE_POV_MOVED:
            //			{
            //				if(event->getJoystickState()->getPOV() == IJoystickState::POV_North)
            //				{
            //					decrementCursor();
            //					return true;
            //				}
            //				else if(event->getJoystickState()->getPOV() == IJoystickState::POV_South)
            //				{
            //					increamentCursor();
            //					return true;
            //				}
            //			}
            //			break;
            //		default:
            //			{
            //			}
            //		};
            //	}
            //	break;
            // default:
            //	{
            //	}
            //};

            return CUIElement::handleEvent( event );
        }

        void CUIMenu::populateMenuItemList()
        {
            ////m_aMenuItemList.set_used(0);
            // for (u32 i = 0; i < m_children.size(); ++i)
            //{
            //	CUIElement* child = (CUIElement*)m_children[i];
            //	if (child->getType() == (String("MenuItem")))
            //	{
            //		m_aMenuItemList.push_back(child);
            //	}
            // }

            // m_uiNumListItems = m_aMenuItemList.size();	//default value
            // m_uiNumMenuItems = m_aMenuItemList.size();
        }

        void CUIMenu::incrementCursor()
        {
            int counter = 0;
            bool isMenuItemEnabled = false;
            while( !isMenuItemEnabled && counter < m_uiNumMenuItems )
            {
                m_prevCursorIdx = m_cursorIdx;
                m_cursorIdx++;

                WrapCursor();

                auto pPrevMenuItem = (CUIMenuItem *)m_aMenuItemList[m_prevCursorIdx];
                auto pCurMenuItem = (CUIMenuItem *)m_aMenuItemList[m_cursorIdx];

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

        void CUIMenu::decrementCursor()
        {
            int counter = 0;
            bool isMenuItemEnabled = false;
            while( !isMenuItemEnabled && counter < m_uiNumMenuItems )
            {
                m_prevCursorIdx = m_cursorIdx;
                m_cursorIdx--;

                WrapCursor();

                auto pPrevMenuItem = (CUIMenuItem *)m_aMenuItemList[m_prevCursorIdx];
                auto pCurMenuItem = (CUIMenuItem *)m_aMenuItemList[m_cursorIdx];

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

        int CUIMenu::getNumMenuItems() const
        {
            // populateMenuItemList();
            return (s32)m_aMenuItemList.size();
        }

        void CUIMenu::WrapCursor()
        {
            auto diff = m_uiNumListItems - m_uiNumMenuItems;

            if( m_iItemIdx <= diff && diff >= 0 )
            {
                if( m_cursorIdx >= static_cast<s32>( m_aMenuItemList.size() ) && m_iItemIdx == diff )
                {
                    m_iItemIdx = 0;
                    m_cursorIdx = 0;
                }
                else if( m_cursorIdx >= static_cast<s32>( m_aMenuItemList.size() ) )
                {
                    m_iItemIdx++;

                    if( m_iItemIdx > diff )
                    {
                        m_iItemIdx = diff;
                    }

                    m_cursorIdx = (s32)m_aMenuItemList.size() - 1;
                }
                else if( m_cursorIdx < 0 && m_iItemIdx == 0 )
                {
                    m_iItemIdx = diff;
                    m_cursorIdx = (s32)m_aMenuItemList.size() - 1;
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
                if( m_cursorIdx >= static_cast<s32>( m_aMenuItemList.size() ) ||
                    m_cursorIdx >= m_uiNumListItems )
                {
                    m_iItemIdx = 0;
                    m_cursorIdx = 0;
                }
                else if( m_cursorIdx < 0 )
                {
                    m_cursorIdx =
                        MathI::min( static_cast<s32>( m_aMenuItemList.size() ), m_uiNumListItems ) - 1;
                }
            }

            // fail safe
            if( m_cursorIdx < 0 )
            {
                m_iItemIdx = 0;
                m_cursorIdx = 0;
            }
        }

        void CUIMenu::setCursorPosition( u32 cursorIdx )
        {
            m_prevCursorIdx = m_cursorIdx;
            m_cursorIdx = cursorIdx;
            WrapCursor();

            auto pPrevMenuItem = (CUIMenuItem *)m_aMenuItemList[m_prevCursorIdx];
            auto pCurMenuItem = (CUIMenuItem *)m_aMenuItemList[m_cursorIdx];

            pPrevMenuItem->setFocus( false );
            pPrevMenuItem->setHighlighted( false );

            pCurMenuItem->setFocus( true );
            pCurMenuItem->setHighlighted( true );
        }

        u32 CUIMenu::getCursorPosition() const
        {
            return m_cursorIdx;
        }

        //
        // Callbacks
        //

        void CUIMenu::OnSelectPrevItem()
        {
        }

        void CUIMenu::OnSelectNextItem()
        {
        }

        void CUIMenu::OnWrapSelectionStart()
        {
        }

        void CUIMenu::OnWrapSelectionEnd()
        {
        }

        void CUIMenu::onAddChild( CUIElement *child )
        {
            populateMenuItemList();
        }

        void CUIMenu::onChildChangedState( CUIElement *child )
        {
            // check if menu items are enabled
            bool itemsEnabled = false;
            for( u32 i = 0; i < m_aMenuItemList.size(); ++i )
            {
                CUIElement *menuItem = m_aMenuItemList[i];
                if( menuItem->isEnabled() )
                {
                    itemsEnabled = true;
                    break;
                }
            }

            // check if menu item are visible
            bool itemsVisible = false;
            for( u32 i = 0; i < m_aMenuItemList.size(); ++i )
            {
                CUIElement *menuItem = m_aMenuItemList[i];
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

        void CUIMenu::onToggleVisibility()
        {
            if( isVisible() )
            {
                m_container->setVisible( true );

                if( m_cursorIdx < static_cast<s32>( m_aMenuItemList.size() ) )
                {
                    // if(isInFocus())
                    {
                        auto pCurMenuItem = (CUIMenuItem *)m_aMenuItemList[m_cursorIdx];
                        pCurMenuItem->setFocus( true );
                        pCurMenuItem->setHighlighted( true );
                    }
                }
            }
            else
            {
                m_container->setVisible( false );
            }

            CUIElement::onToggleVisibility();
        }

        void CUIMenu::onGainFocus()
        {
            if( m_aMenuItemList.size() > 0 )
            {
                auto pCurMenuItem = (CUIMenuItem *)m_aMenuItemList[m_cursorIdx];
                pCurMenuItem->setFocus( true );
                pCurMenuItem->setHighlighted( true );
                pCurMenuItem->setSelected( true );
            }
            else
            {
                FB_ASSERT_TRUE( true );
                printf( "CGUIMenu::onGainFocus - Error" );
            }
        }

        void CUIMenu::onLostFocus()
        {
            if( m_aMenuItemList.size() > 0 )
            {
                auto pCurMenuItem = (CUIMenuItem *)m_aMenuItemList[m_cursorIdx];
                pCurMenuItem->setFocus( false );
                pCurMenuItem->setHighlighted( false );
                pCurMenuItem->setSelected( false );
            }
            else
            {
                FB_ASSERT_TRUE( true );
                printf( "CGUIMenu::onLostFocus - Error" );
            }
        }

        void CUIMenu::setNumListItems( u32 numListItems )
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

        u32 CUIMenu::getNumListItems() const
        {
            return m_uiNumListItems;
        }

        void CUIMenu::setCurrentItemIndex( u32 index )
        {
            m_iItemIdx = index;
        }

        u32 CUIMenu::getCurrentItemIndex() const
        {
            return m_iItemIdx;
        }

        void CUIMenu::setPosition( const Vector2F &position )
        {
            //m_container->setLeft( position.X() );
            //m_container->setTop( position.Y() );
            CUIElement::setPosition( position );
        }
    }  // namespace ui
}  // end namespace fb
