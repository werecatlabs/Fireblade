#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/ToggleGroup.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb::scene, ToggleGroup, UIComponent );

    ToggleGroup::ToggleGroup() = default;

    ToggleGroup::~ToggleGroup() = default;

    void ToggleGroup::load( SmartPtr<ISharedObject> data )
    {
    }

    void ToggleGroup::unload( SmartPtr<ISharedObject> data )
    {
    }

    void ToggleGroup::createUI()
    {
    }

}  // namespace fb::scene
