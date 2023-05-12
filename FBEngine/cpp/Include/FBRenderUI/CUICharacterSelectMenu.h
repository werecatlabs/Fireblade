#ifndef _CGUICharacterSelectMenu_H
#define _CGUICharacterSelectMenu_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUICharacterSelect.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUICharacterSelectMenu : public CUIElement<IUICharacterSelect>
        {
        public:
            CUICharacterSelectMenu();
            ~CUICharacterSelectMenu() override;

            //!
            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            //! Sets the position of the cursor
            //! param - idx - the index of the menu item
            void setCursorPosition( u32 cursorIdx, u32 cursorPosition ) override;
            u32 getCursorPosition( u32 cursorIdx ) const override;

            void setNumListItems( u32 numListItems ) override;
            u32 getNumListItems() const override;

            void setCurrentItemIndex( u32 index ) override;
            u32 getCurrentItemIndex() const override;

            void increamentCursor( s32 cursorIndex ) override; // move the cursor to the next menu item
            void decrementCursor( s32 cursorIndex ) override;
            // move the cursor to the previous menu item

            int getNumMenuItems() const override;

            u32 getCurrentCursorIndex() const override;

            void setPosition( const Vector2F &position ) override;

        private:
            void onAddChild( CUIElement *child );
            void onChildChangedState( CUIElement *child );
            void onToggleVisibility() override;
            void onGainFocus() override;
            void onLostFocus() override;

            void populateMenuItemList();

            void WrapCursor( s32 cursorIndex ); //! ensure the cursor values is within bounds

            //
            // Callbacks
            //
            void OnSelectPrevItem();
            void OnSelectNextItem();
            void OnWrapSelectionStart();
            void OnWrapSelectionEnd();

            bool inputEvent( const SmartPtr<IInputEvent> &event );

            SmartPtr<IUILayout> m_layout;

            SmartPtr<render::IOverlayElementContainer> m_container;

            Array<SmartPtr<IUIElement>> m_menuItems; //

            u32 m_curCursorIndex;

            s32 m_uiNumMenuItems;
            s32 m_uiNumListItems;
            s32 m_iItemIdx; // store the index of an item in a list

            Array<s32> m_cursorIdx;     // the index of the menu item which the cursor is on
            Array<s32> m_prevCursorIdx; // the menu item the Cursor was previously on
        };
    } // end namespace ui
}     // end namespace fb

#endif
