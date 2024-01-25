#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Animator.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Animator, Component );

    Animator::~Animator()
    {
    }

    Animator::Animator()
    {
    }

    void Animator::load( SmartPtr<ISharedObject> data )
    {
        Component::load( data );
    }

    void Animator::unload( SmartPtr<ISharedObject> data )
    {
        Component::unload( data );
    }

}  // namespace fb::scene
