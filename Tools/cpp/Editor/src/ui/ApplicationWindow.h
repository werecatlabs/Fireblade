#ifndef ApplicationWindow_h__
#define ApplicationWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"

namespace fb
{
    namespace editor
    {

        class ApplicationWindow : public BaseWindow
        {
        public:
            enum
            {
                NEW_COMP_BTN_ID,
                DELETE_COMP_BTN_ID,
            };

            ApplicationWindow();
            ~ApplicationWindow();

        protected:
            void populateComponentTree();

            void OnNewComponent();
            void OnDeleteComponent();

            // wxTreeCtrl* m_tree;

            // wxButton* m_newBtn;
            // wxButton* m_deleteBtn;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // ApplicationWindow_h__
