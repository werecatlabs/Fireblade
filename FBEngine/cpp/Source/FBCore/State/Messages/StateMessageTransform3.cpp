#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageTransform3.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageTransform3, StateMessage );

    StateMessageTransform3::StateMessageTransform3() = default;

    StateMessageTransform3::~StateMessageTransform3() = default;

    auto StateMessageTransform3::getTransform() const -> Transform3<real_Num>
    {
        return m_transform;
    }

    void StateMessageTransform3::setTransform( const Transform3<real_Num> &val )
    {
        m_transform = val;
    }
}  // end namespace fb
