#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageMaterialName.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageMaterialName, StateMessage );

    StateMessageMaterialName::StateMessageMaterialName() = default;

    StateMessageMaterialName::~StateMessageMaterialName()  = default;

    auto StateMessageMaterialName::getMaterialName() const -> String
    {
        return m_value;
    }

    void StateMessageMaterialName::setMaterialName( const String &value )
    {
        m_value = value;
    }

    auto StateMessageMaterialName::getIndex() const -> u32
    {
        return m_index;
    }

    void StateMessageMaterialName::setIndex( u32 index )
    {
        m_index = index;
    }
}  // end namespace fb
