#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/CameraTarget.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, CameraTarget, Component );

    CameraTarget::CameraTarget() = default;

    CameraTarget::~CameraTarget() = default;

    auto CameraTarget::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();
        properties->setProperty( "offsetPosition", m_offsetPosition );
        properties->setProperty( "offsetRotation", m_offsetRotation );
        return properties;
    }

    void CameraTarget::setProperties( SmartPtr<Properties> properties )
    {
        Component::setProperties( properties );

        properties->getPropertyValue( "offsetPosition", m_offsetPosition );
        properties->getPropertyValue( "offsetRotation", m_offsetRotation );
    }

    auto CameraTarget::getOffsetPosition() const -> Vector3<real_Num>
    {
        return m_offsetPosition;
    }

    void CameraTarget::setOffsetPosition( const Vector3<real_Num> &offsetPosition )
    {
        m_offsetPosition = offsetPosition;
    }

    auto CameraTarget::getOffsetRotation() const -> Quaternion<real_Num>
    {
        return m_offsetRotation;
    }

    void CameraTarget::setOffsetRotation( const Quaternion<real_Num> &offsetRotation )
    {
        m_offsetRotation = offsetRotation;
    }
}  // namespace fb::scene
