#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/InputField.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb::scene, InputField, UIComponent );

    InputField::InputField() = default;

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

    void InputField::createUI()
    {
    }

}  // namespace fb::scene
