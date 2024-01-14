#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageProperties.h"
#include "FBCore/Core/Properties.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageProperties, StateMessage );

    StateMessageProperties::StateMessageProperties() = default;

    StateMessageProperties::~StateMessageProperties() = default;

    auto StateMessageProperties::getProperties() const -> SmartPtr<Properties>
    {
        return m_properties;
    }

    void StateMessageProperties::setProperties( SmartPtr<Properties> properties )
    {
        m_properties = properties;
    }
}  // end namespace fb
