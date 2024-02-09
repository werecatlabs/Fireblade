#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Animation.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Animation, SubComponent );

    Animation::Animation()
    {
    }

    Animation::~Animation()
    {
    }

    void Animation::load( SmartPtr<ISharedObject> data )
    {
        SubComponent::load( data );
    }

    void Animation::unload( SmartPtr<ISharedObject> data )
    {
        SubComponent::unload( data );
    }

    f32 Animation::getLength() const
    {
        return m_length;
    }

    void Animation::setLength( f32 length )
    {
        m_length = length;
    }
}  // namespace fb::scene
