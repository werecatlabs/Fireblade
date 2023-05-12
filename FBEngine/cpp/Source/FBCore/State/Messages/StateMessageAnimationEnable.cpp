#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageAnimationEnable.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageAnimationEnable, StateMessage );

    StateMessageAnimationEnable::StateMessageAnimationEnable() : m_time( 0.f ), m_isEnabled( false )
    {
    }

    StateMessageAnimationEnable::~StateMessageAnimationEnable()
    {
    }

    String StateMessageAnimationEnable::getName() const
    {
        return m_name;
    }

    void StateMessageAnimationEnable::setName( const String &name )
    {
        m_name = name;
    }

    f32 StateMessageAnimationEnable::getTime() const
    {
        return m_time;
    }

    void StateMessageAnimationEnable::setTime( f32 time )
    {
        m_time = time;
    }

    bool StateMessageAnimationEnable::getEnabled() const
    {
        return m_isEnabled;
    }

    void StateMessageAnimationEnable::setEnabled( bool enabled )
    {
        m_isEnabled = enabled;
    }
}  // end namespace fb
