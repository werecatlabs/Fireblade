#ifndef InputManagerWindow_h__
#define InputManagerWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/EditorWindow.h"

namespace fb
{
    namespace editor
    {

        class InputManagerWindow : public EditorWindow
        {
        public:
            InputManagerWindow();
            ~InputManagerWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<ui::IUIInputManager> getInputManager() const;

            void setInputManager( SmartPtr<ui::IUIInputManager> inputManager );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<ui::IUIInputManager> m_inputManager;
        };

    }  // namespace editor
}  // namespace fb

#endif  // InputManagerWindow_h__
