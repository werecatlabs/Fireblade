#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/KeyFrameTransform2.h>

namespace fb
{

    KeyFrameTransform2::KeyFrameTransform2()
    {
    }

    KeyFrameTransform2::~KeyFrameTransform2()
    {
    }

    fb::Vector2<real_Num> KeyFrameTransform2::getPosition() const
    {
        return m_position;
    }

    void KeyFrameTransform2::setPosition( const Vector2<real_Num> &position )
    {
        m_position = position;
    }

    fb::Vector2<real_Num> KeyFrameTransform2::getScale() const
    {
        return m_scale;
    }

    void KeyFrameTransform2::setScale( const Vector2<real_Num> &scale )
    {
        m_scale = scale;
    }

    fb::f32 KeyFrameTransform2::getRotation() const
    {
        return m_rotation;
    }

    void KeyFrameTransform2::setRotation( f32 rotation )
    {
        m_rotation = rotation;
    }

}  // namespace fb
