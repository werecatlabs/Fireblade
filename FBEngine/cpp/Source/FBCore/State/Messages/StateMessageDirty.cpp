#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageDirty.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageDirty, StateMessage );

    //--------------------------------------------
    bool StateMessageDirty::isDirty() const
    {
        return m_isDirty;
    }

    //--------------------------------------------
    void StateMessageDirty::setDirty( bool dirty )
    {
        m_isDirty = dirty;
    }
}  // end namespace fb
