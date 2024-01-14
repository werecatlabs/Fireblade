#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageVisible.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageVisible, StateMessage );

    //---------------------------------------------
    auto StateMessageVisible::isVisible() const -> bool
    {
        return m_isVisible;
    }

    //---------------------------------------------
    void StateMessageVisible::setVisible( bool val )
    {
        m_isVisible = val;
    }

    //---------------------------------------------
    auto StateMessageVisible::getCascade() const -> bool
    {
        return m_cascade;
    }

    //---------------------------------------------
    void StateMessageVisible::setCascade( bool val )
    {
        m_cascade = val;
    }
}  // end namespace fb
