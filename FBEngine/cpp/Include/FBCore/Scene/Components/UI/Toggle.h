#ifndef ToggleComponent_h__
#define ToggleComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        class Toggle : public UIComponent
        {
        public:
            Toggle();
            ~Toggle() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // ToggleComponent_h__
