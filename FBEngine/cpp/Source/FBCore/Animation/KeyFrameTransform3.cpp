#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/KeyFrameTransform3.h>

namespace fb
{

    KeyFrameTransform3::KeyFrameTransform3( f32 time, const Vector3<real_Num> &position,
                                            const Quaternion<real_Num> &orientation,
                                            const Vector3<real_Num> &scale ) :
        m_orientation( orientation ),
        m_position( position ),
        m_scale( scale )
    {
        m_time = time;
    }

    KeyFrameTransform3::KeyFrameTransform3() = default;

    KeyFrameTransform3::~KeyFrameTransform3() = default;

    auto KeyFrameTransform3::getPosition() const -> fb::Vector3<real_Num>
    {
        return m_position;
    }

    void KeyFrameTransform3::setPosition( const Vector3<real_Num> &position )
    {
        m_position = position;
    }

    auto KeyFrameTransform3::getOrientation() const -> fb::Quaternion<real_Num>
    {
        return m_orientation;
    }

    void KeyFrameTransform3::setOrientation( const Quaternion<real_Num> &orientation )
    {
        m_orientation = orientation;
    }

    auto KeyFrameTransform3::getScale() const -> fb::Vector3<real_Num>
    {
        return m_scale;
    }

    void KeyFrameTransform3::setScale( const Vector3<real_Num> &scale )
    {
        m_scale = scale;
    }

}  // namespace fb
