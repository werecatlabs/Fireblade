#ifndef UISlider_h__
#define UISlider_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUISlider.h>

namespace fb
{
    namespace ui
    {
        class UISlider : public UIElement<IUISlider>
        {
        public:
            UISlider();
            ~UISlider() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Colibri::Slider *m_slider = nullptr;
        };

    }  // namespace ui
}  // namespace fb

#endif  // UISlider_h__
