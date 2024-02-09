#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageOrientation.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageOrientation, StateMessage );

    void StateMessageOrientation::setOrientation( const Quaternion<real_Num> &val )
    {
        m_orientation = val;
    }

    auto StateMessageOrientation::getOrientation() const -> Quaternion<real_Num>
    {
        return m_orientation;
    }
}  // end namespace fb
