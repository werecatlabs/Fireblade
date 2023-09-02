#ifndef FileViewWindow_h__
#define FileViewWindow_h__

#include "ui/EditorWindow.h"
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace editor
    {
        class FileViewWindow : public EditorWindow
        {
        public:
            FileViewWindow( SmartPtr<ui::IUIWindow> parent );
            ~FileViewWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection() override;

        protected:
            String m_filePath;
            SmartPtr<ui::IUIText> m_text;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // FileViewWindow_h__
