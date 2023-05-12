#ifndef _CGUIScrollingText_H
#define _CGUIScrollingText_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIScrollingText.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIScrollingText : public CUIElement<IUIScrollingText>
        {
        public:
            enum ScrollingTextState
            {
                STS_START,
                STS_SCROLLING,
                STS_FADEOUT,

                STS_COUNT
            };

            CUIScrollingText();
            ~CUIScrollingText() override;

            //!
            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            //! Sets the position of the cursor
            //! param - idx - the index of the menu item
            void setCursorPosition( u32 cursorIdx );
            u32 getCursorPosition() const;

            void setNumListItems( u32 numListItems );
            u32 getNumListItems() const;

            void setCurrentItemIndex( u32 index );
            u32 getCurrentItemIndex() const;

            void increamentCursor(); // move the cursor to the next menu item
            void decrementCursor();  // move the cursor to the previous menu item

            int getNumMenuItems() const;

            void OnEnterState( u8 state );
            void OnUpdateState( u8 state );
            void OnLeaveState( u8 state );

        private:
            void onAddChild( CUIElement *child );
            void onChildChangedState( CUIElement *child );
            void onToggleVisibility() override;
            void onGainFocus() override;
            void onLostFocus() override;

            void populateMenuItemList();

            void WrapCursor(); //! ensure the cursor values is within bounds

            //
            // Callbacks
            //
            void OnSelectPrevItem();
            void OnSelectNextItem();
            void OnWrapSelectionStart();
            void OnWrapSelectionEnd();

            SmartPtr<render::IOverlayElementContainer> m_container;

            Array<IUIElement *> m_elements; //

            s32 m_uiNumMenuItems;
            s32 m_uiNumListItems;
            s32 m_iItemIdx;      // store the index of an item in a list
            s32 m_cursorIdx;     // the index of the menu item which the cursor is on
            s32 m_prevCursorIdx; // the menu item the Cursor was previously on
        };
    } // end namespace ui
}     // end namespace fb

#endif
