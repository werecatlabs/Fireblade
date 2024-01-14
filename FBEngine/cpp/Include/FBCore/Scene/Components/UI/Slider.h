#ifndef SliderComponent_h__
#define SliderComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {
        class Slider : public UIComponent
        {
        public:
            Slider();
            ~Slider() override;

            /** @copydoc UIComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<ui::IUISlider> getSlider() const;

            void setSlider( SmartPtr<ui::IUISlider> slider );

            FB_CLASS_REGISTER_DECL;

        protected:
            void createUI() override;

            SmartPtr<ui::IUISlider> m_slider;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // SliderComponent_h__
