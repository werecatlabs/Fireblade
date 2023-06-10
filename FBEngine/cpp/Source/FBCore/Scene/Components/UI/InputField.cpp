#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/InputField.h>

namespace fb
{
    namespace scene
    {
        InputField::InputField()
        {
        }

        InputField::~InputField()
        {
            unload( nullptr );
        }

        void InputField::load( SmartPtr<ISharedObject> data )
        {
        }

        void InputField::unload( SmartPtr<ISharedObject> data )
        {
        }
    }  // namespace scene
}  // end namespace fb
