#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/AnimatedMaterial.h>

namespace fb::scene
{

    AnimatedMaterial::AnimatedMaterial()
    {
    }

    AnimatedMaterial::~AnimatedMaterial()
    {
        unload( nullptr );
    }

    void AnimatedMaterial::load( SmartPtr<ISharedObject> data )
    {
        Component::load( data );
    }

    void AnimatedMaterial::unload( SmartPtr<ISharedObject> data )
    {
        Component::unload( data );
    }

}  // namespace fb::scene
