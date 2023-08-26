#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageMaterialName.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageMaterialName, StateMessage );

    String StateMessageMaterialName::getMaterialName() const
    {
        return m_value;
    }

    void StateMessageMaterialName::setMaterialName( const String &val )
    {
        m_value = val;
    }

    u32 StateMessageMaterialName::getIndex() const
    {
        return m_index;
    }

    void StateMessageMaterialName::setIndex( u32 val )
    {
        m_index = val;
    }
}  // end namespace fb
