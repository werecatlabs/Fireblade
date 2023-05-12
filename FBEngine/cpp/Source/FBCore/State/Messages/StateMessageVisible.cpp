#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageVisible.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageVisible, StateMessage );

    //---------------------------------------------
    bool StateMessageVisible::isVisible() const
    {
        return m_isVisible;
    }

    //---------------------------------------------
    void StateMessageVisible::setVisible( bool val )
    {
        m_isVisible = val;
    }

    //---------------------------------------------
    bool StateMessageVisible::getCascade() const
    {
        return m_cascade;
    }

    //---------------------------------------------
    void StateMessageVisible::setCascade( bool val )
    {
        m_cascade = val;
    }
}  // end namespace fb
