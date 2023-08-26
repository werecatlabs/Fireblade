#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageTransform3.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageTransform3, StateMessage );

    StateMessageTransform3::StateMessageTransform3()
    {
    }

    StateMessageTransform3::~StateMessageTransform3()
    {
    }

    Transform3<real_Num> StateMessageTransform3::getTransform() const
    {
        return m_transform;
    }

    void StateMessageTransform3::setTransform( const Transform3<real_Num> &val )
    {
        m_transform = val;
    }
}  // end namespace fb
