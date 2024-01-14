#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageBlendMapValue.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageBlendMapValue, StateMessage );

    //--------------------------------------------
    StateMessageBlendMapValue::StateMessageBlendMapValue() :
        m_coordinates( Vector2I::zero() ),
        m_blendValue( 0.f )
    {
    }

    //--------------------------------------------
    StateMessageBlendMapValue::~StateMessageBlendMapValue() = default;

    //--------------------------------------------
    auto StateMessageBlendMapValue::getCoordinates() const -> Vector2I
    {
        return m_coordinates;
    }

    //--------------------------------------------
    void StateMessageBlendMapValue::setCoordinates( const Vector2I &val )
    {
        m_coordinates = val;
    }

    //--------------------------------------------
    auto StateMessageBlendMapValue::getBlendValue() const -> f32
    {
        return m_blendValue;
    }

    //--------------------------------------------
    void StateMessageBlendMapValue::setBlendValue( f32 val )
    {
        m_blendValue = val;
    }
}  // end namespace fb
