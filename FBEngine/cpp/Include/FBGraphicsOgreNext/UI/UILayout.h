#ifndef UILayout_h__
#define UILayout_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUILayout.h>

namespace fb
{
    namespace ui
    {
        class UILayout : public UIElement<IUILayout>
        {
        public:
            UILayout();
            ~UILayout() override;

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IFSM> getFSM() override;

            SmartPtr<IUIWindow> getUiWindow() const override;

            void setUiWindow( SmartPtr<IUIWindow> uiWindow ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Colibri::Window *m_window = nullptr;
        };
    } // namespace render
}     // namespace fb

#endif  // UILayout_h__
