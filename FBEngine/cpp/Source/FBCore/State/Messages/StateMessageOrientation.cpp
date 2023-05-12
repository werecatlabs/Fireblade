#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageOrientation.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageOrientation, StateMessage );

    void StateMessageOrientation::setOrientation( const QuaternionF &val )
    {
        m_orientation = val;
    }

    QuaternionF StateMessageOrientation::getOrientation() const
    {
        return m_orientation;
    }
}  // end namespace fb
