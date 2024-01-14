#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/CameraFollow.h>
#include <FBCore/Scene/Components/Camera/CameraTarget.h>
#include <FBCore/Interface/Scene/IActor.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, CameraFollow, Component );

    CameraFollow::CameraFollow() = default;

    CameraFollow::~CameraFollow() = default;

    auto CameraFollow::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();
        properties->setPropertyAsType( "target", m_target );
        properties->setPropertyAsType( "followObject", m_followObject );
        return properties;
    }

    void CameraFollow::setProperties( SmartPtr<Properties> properties )
    {
        Component::setProperties( properties );

        properties->getPropertyAsType( "target", m_target );
        properties->getPropertyAsType( "followObject", m_followObject );
    }

    auto CameraFollow::getTarget() const -> SmartPtr<CameraTarget>
    {
        return m_target;
    }

    void CameraFollow::setTarget( SmartPtr<CameraTarget> target )
    {
        m_target = target;
    }

    auto CameraFollow::getFollowObject() const -> SmartPtr<IActor>
    {
        return m_followObject;
    }

    void CameraFollow::setFollowObject( SmartPtr<IActor> followObject )
    {
        m_followObject = followObject;
    }
}  // namespace fb::scene
