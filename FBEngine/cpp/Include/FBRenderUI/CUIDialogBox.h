#ifndef _CGUIDIALOGBOX_H
#define _CGUIDIALOGBOX_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIDialogBox : public CUIElement<IUIElement>
        {
        public:
            CUIDialogBox();
            ~CUIDialogBox() override;

            //! initializes the dialog box
            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            //! Set scroll speed
            void setScrollSpeed( s32 scroll_speed );

            //! returns the string in the dialog box
            String getString();

            void onToggleVisibility() override;

        private:
            //
            // Callbacks
            //
            void OnScroll();
            void OnClose();

            Ogre::Overlay *m_pOverlay;
            Ogre::OverlayContainer *m_pContainer;

            String m_Text[2]; // array to store the string that is to be displayed and the full string.

            u32 m_scroll_speed; // store the number of nano second it takes to move onto the next
            // character
            u32 m_time_to_next_char; // store time the next character should be displayed

            bool m_bPauseDialog; //
            bool m_bIsFinished;  // let us know if the dialog has finished been shown
        };

        // dialog box with character icon
        class IconDialogBox
        {
        };

        using CGUIDialogBoxPtr = SmartPtr<CUIDialogBox>;
    } // end namespace ui
}     // end namespace fb

#endif
