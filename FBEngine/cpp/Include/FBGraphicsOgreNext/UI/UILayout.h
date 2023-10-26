#ifndef UILayout_h__
#define UILayout_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUILayout.h>

namespace fb
{
    namespace ui
    {

        class UILayout : public UIElement<ui::IUILayout>
        {
        public:
            UILayout();
            ~UILayout();

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IFSM> getFSM() override;

            SmartPtr<ui::IUIWindow> getUiWindow() const override;

            void setUiWindow( SmartPtr<ui::IUIWindow> uiWindow ) override;

        protected:
            Colibri::Window *m_window = nullptr;
        };

    }  // namespace render
}  // namespace fb

#endif  // UILayout_h__
