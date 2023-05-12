#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUICharacterSelectMenu.h>
#include <FBRenderUI/CUIMenuItem.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUICharacterSelectMenu::CUICharacterSelectMenu() :
            m_uiNumMenuItems( 0 ),
            m_uiNumListItems( 0 ),
            m_iItemIdx( 0 ),

            m_cursorIdx( 0 ),
            m_prevCursorIdx( 0 )
        {
            /*		m_prevCursorIdx.set_used(2);
                    m_cursorIdx.set_used(2);*/
            m_prevCursorIdx[0] = 0;
            m_prevCursorIdx[1] = 0;
            m_cursorIdx[0] = 0;
            m_cursorIdx[1] = 0;

            m_curCursorIndex = 0;
        }

        CUICharacterSelectMenu::~CUICharacterSelectMenu()
        {
            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );
        }

        void CUICharacterSelectMenu::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
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

            value = String( element->Attribute( "posX" ) );
            sscanf( value.c_str(), "%f", &left );
            value = String( element->Attribute( "posY" ) );
            sscanf( value.c_str(), "%f", &top );

            value = String( element->Attribute( "width" ) );
            sscanf( value.c_str(), "%f", &width );
            value = String( element->Attribute( "height" ) );
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
            // if(parentType==(String("Layout")))
            //{
            //	SmartPtr<IOverlay> overlay = overlayManager->findOverlay(parent->getName());
            //	overlay->addElement(m_container);

            //	m_layout = GUIUtil::findItemById(parent->getName());
            //}
            // else
            //{
            //	SmartPtr<IOverlayElementContainer> cont = overlayManager->findElement(parent->getName());
            //	cont->addChild(m_container);
            //}

            bool isVisible = true;
            value = String( element->Attribute( "visible" ) );
            if( value == ( String( "false" ) ) )
                isVisible = false;

            if( parent )
                isVisible = isVisible && parent->isVisible();

            setVisible( isVisible );
        }

        bool CUICharacterSelectMenu::handleEvent( const SmartPtr<IInputEvent> &event )
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
            //		SmartPtr<IInputManager>& inputManager =
            //IApplicationManager::instance()->getInputManager(); 		SmartPtr<IGameInput> gameInput =
            //inputManager->findGameInput(event->getGameInputId()); 		SmartPtr<IGameInputState>
            //gameInputState = event->getGameInputState();
            //
            //		if(gameInput)
            //			m_curCursorIndex = gameInput->getPlayerIndex();

            //		if(!inputEvent( event ))
            //		{
            //			if(gameInputState && gameInputState->getEventType() ==
            //IGameInputState::EVT_TYPE_ACTION_PRESSED)
            //			{

            //				//hack
            //				m_curCursorIndex = 0;

            //				if(gameInputState->getAction() == StringUtil::getHash("up"))
            //				{
            //					decrementCursor(m_curCursorIndex);
            //					return true;
            //				}
            //				else if(gameInputState->getAction() == StringUtil::getHash("down"))
            //				{
            //					increamentCursor(m_curCursorIndex);
            //					return true;
            //				}
            //			}

            //			return true;
            //		}
            //	}
            //	break;
            // default:
            //	{
            //	}
            //};

            return CUIElement::handleEvent( event );
        }

        bool CUICharacterSelectMenu::inputEvent( const SmartPtr<IInputEvent> &event )
        {
            Parameters params;
            // params.set_used(1);
            params[0].setPtr( event.get() );

            Parameters results;

            static const auto ON_INPUT_EVENT_HASH = StringUtil::getHash( "inputEvent" );

            SmartPtr<IScriptInvoker> invoker;  // = m_layout->getInvoker();
            if( invoker )
                invoker->event( ON_INPUT_EVENT_HASH, params, results );

            return results.size() > 0 && results[0].getBool() == true;
        }

        void CUICharacterSelectMenu::populateMenuItemList()
        {
            ////m_menuItems.set_used(0);
            // for (u32 i = 0; i < m_children.size(); ++i)
            //{
            //	auto child = m_children[i];
            //	if (child->getType() == (String("MenuItem")))
            //	{
            //		m_menuItems.push_back(child);
            //	}
            // }

            m_uiNumListItems = (s32)m_menuItems.size();  // default value
            m_uiNumMenuItems = (s32)m_menuItems.size();
        }

        void CUICharacterSelectMenu::increamentCursor( s32 cursorIndex )
        {
            int counter = 0;
            bool isMenuItemEnabled = false;
            while( !isMenuItemEnabled && counter < m_uiNumMenuItems )
            {
                m_prevCursorIdx[cursorIndex] = m_cursorIdx[cursorIndex];
                m_cursorIdx[cursorIndex]++;

                WrapCursor( cursorIndex );

                auto pPrevMenuElement = m_menuItems[m_prevCursorIdx[cursorIndex]];
                auto pCurMenuElement = m_menuItems[m_cursorIdx[cursorIndex]];

                auto pPrevMenuItem = fb::static_pointer_cast<CUIMenuItem>( pPrevMenuElement );
                auto pCurMenuItem = fb::static_pointer_cast<CUIMenuItem>( pCurMenuElement );

                pPrevMenuItem->setFocus( false );
                pPrevMenuItem->setHighlighted( false );
                pPrevMenuItem->setSelected( false );

                pCurMenuItem->setFocus( true );
                pCurMenuItem->setHighlighted( true );
                pCurMenuItem->setSelected( true );

                // Parameters params;
                ////params.set_used(4);
                // params[0].data.pData = pPrevMenuItem;
                // params[1].data.pData = pCurMenuItem;
                // params[2].data.iData = m_cursorIdx[cursorIndex];
                // params[3].data.iData = cursorIndex;

                // m_layout->getInvoker()->event(StringUtil::getHash("selectItem"), params);

                isMenuItemEnabled = pCurMenuItem->isEnabled();
                counter++;
            }
        }

        void CUICharacterSelectMenu::decrementCursor( s32 cursorIndex )
        {
            int counter = 0;
            bool isMenuItemEnabled = false;
            while( !isMenuItemEnabled && counter < m_uiNumMenuItems )
            {
                m_prevCursorIdx[cursorIndex] = m_cursorIdx[cursorIndex];
                m_cursorIdx[cursorIndex]--;

                WrapCursor( cursorIndex );

                auto pPrevMenuElement = m_menuItems[m_prevCursorIdx[cursorIndex]];
                auto pCurMenuElement = m_menuItems[m_cursorIdx[cursorIndex]];

                auto pPrevMenuItem = fb::static_pointer_cast<CUIMenuItem>( pPrevMenuElement );
                auto pCurMenuItem = fb::static_pointer_cast<CUIMenuItem>( pCurMenuElement );

                pPrevMenuItem->setFocus( false );
                pPrevMenuItem->setHighlighted( false );
                pPrevMenuItem->setSelected( false );

                pCurMenuItem->setFocus( true );
                pCurMenuItem->setHighlighted( true );
                pCurMenuItem->setSelected( true );

                // Parameters params;
                ////params.set_used(4);
                // params[0].data.pData = pPrevMenuItem;
                // params[1].data.pData = pCurMenuItem;
                // params[2].data.iData = m_cursorIdx[cursorIndex];
                // params[3].data.iData = cursorIndex;

                // m_layout->getInvoker()->event(StringUtil::getHash("selectItem"), params);

                isMenuItemEnabled = pCurMenuItem->isEnabled();
                counter++;
            }
        }

        int CUICharacterSelectMenu::getNumMenuItems() const
        {
            // populateMenuItemList();
            return (s32)m_menuItems.size();
        }

        u32 CUICharacterSelectMenu::getCurrentCursorIndex() const
        {
            return m_curCursorIndex;
        }

        void CUICharacterSelectMenu::WrapCursor( s32 cursorIndex )
        {
            auto diff = m_uiNumListItems - m_uiNumMenuItems;

            if( m_iItemIdx <= diff && diff >= 0 )
            {
                if( m_cursorIdx[cursorIndex] >= static_cast<s32>( m_menuItems.size() ) &&
                    m_iItemIdx == diff )
                {
                    m_iItemIdx = 0;
                    m_cursorIdx[cursorIndex] = 0;
                }
                else if( m_cursorIdx[cursorIndex] >= static_cast<s32>( m_menuItems.size() ) )
                {
                    m_iItemIdx++;

                    if( m_iItemIdx > diff )
                    {
                        m_iItemIdx = diff;
                    }

                    m_cursorIdx[cursorIndex] = (s32)m_menuItems.size() - 1;
                }
                else if( m_cursorIdx[cursorIndex] < 0 && m_iItemIdx == 0 )
                {
                    m_iItemIdx = diff;
                    m_cursorIdx[cursorIndex] = (s32)m_menuItems.size() - 1;
                }
                else if( m_cursorIdx[cursorIndex] < 0 )
                {
                    m_iItemIdx--;

                    if( m_iItemIdx < 0 )
                    {
                        m_iItemIdx = 0;
                    }

                    m_cursorIdx[cursorIndex] = 0;
                }
            }
            else
            {
                if( m_cursorIdx[cursorIndex] >= static_cast<s32>( m_menuItems.size() ) ||
                    m_cursorIdx[cursorIndex] >= m_uiNumListItems )
                {
                    m_iItemIdx = 0;
                    m_cursorIdx[cursorIndex] = 0;
                }
                else if( m_cursorIdx[cursorIndex] < 0 )
                {
                    m_cursorIdx[cursorIndex] =
                        MathI::min( static_cast<s32>( m_menuItems.size() ), m_uiNumListItems ) - 1;
                }
            }

            // fail safe
            if( m_cursorIdx[cursorIndex] < 0 )
            {
                m_iItemIdx = 0;
                m_cursorIdx[cursorIndex] = 0;
            }
        }

        void CUICharacterSelectMenu::setCursorPosition( u32 cursorIdx, u32 cursorPosition )
        {
            m_curCursorIndex = cursorIdx;
            m_prevCursorIdx[cursorIdx] = m_cursorIdx[cursorIdx];
            m_cursorIdx[cursorIdx] = cursorPosition;
            WrapCursor( cursorIdx );

            auto pPrevMenuElement = m_menuItems[m_prevCursorIdx[cursorIdx]];
            auto pCurMenuElement = m_menuItems[m_cursorIdx[cursorIdx]];

            auto pPrevMenuItem = fb::static_pointer_cast<CUIMenuItem>( pPrevMenuElement );
            auto pCurMenuItem = fb::static_pointer_cast<CUIMenuItem>( pCurMenuElement );

            pPrevMenuItem->setFocus( false );
            pPrevMenuItem->setHighlighted( false );
            pPrevMenuItem->setSelected( false );

            pCurMenuItem->setFocus( true );
            pCurMenuItem->setHighlighted( true );
            pCurMenuItem->setSelected( true );

            // Parameters params;
            ////params.set_used(4);
            // params[0].data.pData = pPrevMenuItem;
            // params[1].data.pData = pCurMenuItem;
            // params[2].data.iData = m_cursorIdx[cursorIdx];
            // params[3].data.iData = cursorIdx;

            // m_layout->getInvoker()->event(StringUtil::getHash("selectItem"), params);
        }

        u32 CUICharacterSelectMenu::getCursorPosition( u32 cursorIdx ) const
        {
            return m_cursorIdx[cursorIdx];
        }

        //
        // Callbacks
        //

        void CUICharacterSelectMenu::OnSelectPrevItem()
        {
        }

        void CUICharacterSelectMenu::OnSelectNextItem()
        {
        }

        void CUICharacterSelectMenu::OnWrapSelectionStart()
        {
        }

        void CUICharacterSelectMenu::OnWrapSelectionEnd()
        {
        }

        void CUICharacterSelectMenu::onAddChild( CUIElement *child )
        {
            populateMenuItemList();
        }

        void CUICharacterSelectMenu::onChildChangedState( CUIElement *child )
        {
            ////check if menu items are enabled
            // bool itemsEnabled = false;
            // for (u32 i = 0; i < m_menuItems.size(); ++i)
            //{
            //	CUIElement* menuItem = m_menuItems[i];
            //	if (menuItem->isEnabled())
            //	{
            //		itemsEnabled = true;
            //		break;
            //	}
            // }

            ////check if menu item are visible
            // bool itemsVisible = false;
            // for (u32 i = 0; i < m_menuItems.size(); ++i)
            //{
            //	CUIElement* menuItem = m_menuItems[i];
            //	if (menuItem->isVisible())
            //	{
            //		itemsVisible = true;
            //		break;
            //	}
            // }

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

        void CUICharacterSelectMenu::onToggleVisibility()
        {
            /*if(m_isVisible)
            {
            m_pOverlayElement->show();

            if(m_cursorIdx < (s32)m_menuItems.size())
            {
            if(isInFocus())
            {
            CGUIMenuItem* pCurMenuItem = (CGUIMenuItem*)m_menuItems[m_cursorIdx];
            pCurMenuItem->setFocus(true);
            pCurMenuItem->setHighlighted(true);
            }
            }
            }
            else
            m_pOverlayElement->hide();*/

            CUIElement::onToggleVisibility();
        }

        void CUICharacterSelectMenu::onGainFocus()
        {
            /*if(m_menuItems.size() > 0)
            {
            CGUIMenuItem* pCurMenuItem = (CGUIMenuItem*)m_menuItems[m_cursorIdx];
            pCurMenuItem->setFocus(true);
            pCurMenuItem->setHighlighted(true);
            pCurMenuItem->setSelected(true);
            }
            else
            {
            FB_ASSERT_TRUE(true);
            printf("CGUICharacterSelectMenu::onGainFocus - Error");
            }*/
        }

        void CUICharacterSelectMenu::onLostFocus()
        {
            /*if(m_menuItems.size() > 0)
            {
            CGUIMenuItem* pCurMenuItem = (CGUIMenuItem*)m_menuItems[m_cursorIdx];
            pCurMenuItem->setFocus(false);
            pCurMenuItem->setHighlighted(false);
            pCurMenuItem->setSelected(false);
            }
            else
            {
            FB_ASSERT_TRUE(true);
            printf("CGUICharacterSelectMenu::onLostFocus - Error");
            }*/
        }

        void CUICharacterSelectMenu::setNumListItems( u32 numListItems )
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

        u32 CUICharacterSelectMenu::getNumListItems() const
        {
            return m_uiNumListItems;
        }

        void CUICharacterSelectMenu::setCurrentItemIndex( u32 index )
        {
            m_iItemIdx = index;
        }

        u32 CUICharacterSelectMenu::getCurrentItemIndex() const
        {
            return m_iItemIdx;
        }

        void CUICharacterSelectMenu::setPosition( const Vector2F &position )
        {
            //m_container->setLeft( position.X() );
            //m_container->setTop( position.Y() );
            CUIElement::setPosition( position );
        }
    }  // namespace ui
}  // end namespace fb
