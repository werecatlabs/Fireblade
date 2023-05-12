#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageProperties.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageProperties, StateMessage );

    //-------------------------------------------------
    StateMessageProperties::StateMessageProperties()
    {
    }

    //-------------------------------------------------
    StateMessageProperties::~StateMessageProperties()
    {
    }

    //-------------------------------------------------
    Properties &StateMessageProperties::getProperties()
    {
        return m_properties;
    }

    //-------------------------------------------------
    const Properties &StateMessageProperties::getProperties() const
    {
        return m_properties;
    }

    //-------------------------------------------------
    void StateMessageProperties::setProperties( const Properties &val )
    {
        m_properties = val;
    }
}  // end namespace fb
