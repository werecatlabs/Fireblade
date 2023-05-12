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

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // SliderComponent_h__
