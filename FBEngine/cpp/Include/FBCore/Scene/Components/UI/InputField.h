#ifndef InputFieldComponent_h__
#define InputFieldComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        class InputField : public UIComponent
        {
        public:
            InputField();
            ~InputField() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // InputFieldComponent_h__
