#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageAnimationEnable.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageAnimationEnable, StateMessage );

    StateMessageAnimationEnable::StateMessageAnimationEnable() = default;

    StateMessageAnimationEnable::~StateMessageAnimationEnable() = default;

    auto StateMessageAnimationEnable::getName() const -> String
    {
        return m_name;
    }

    void StateMessageAnimationEnable::setName( const String &name )
    {
        m_name = name;
    }

    auto StateMessageAnimationEnable::getTime() const -> f32
    {
        return m_time;
    }

    void StateMessageAnimationEnable::setTime( f32 time )
    {
        m_time = time;
    }

    auto StateMessageAnimationEnable::getEnabled() const -> bool
    {
        return m_isEnabled;
    }

    void StateMessageAnimationEnable::setEnabled( bool enabled )
    {
        m_isEnabled = enabled;
    }
}  // end namespace fb
