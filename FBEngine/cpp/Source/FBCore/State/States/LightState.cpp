#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/LightState.h>

#include "FBCore/Memory/PointerUtil.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, LightState, BaseState );

    LightState::LightState() = default;

    LightState::~LightState() = default;

    auto LightState::getDiffuseColour() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_diffuseColour;
    }

    void LightState::setDiffuseColour( const ColourF &diffuseColour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_diffuseColour = diffuseColour;
        setDirty( true );
    }

    auto LightState::getSpecularColour() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_specularColour;
    }

    void LightState::setSpecularColour( const ColourF &specularColour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_specularColour = specularColour;
        setDirty( true );
    }

    auto LightState::getDirection() const -> Vector3<real_Num>
    {
        return m_direction;
    }

    void LightState::setDirection( const Vector3<real_Num> &direction )
    {
        m_direction = direction;
    }

    auto LightState::getAttenuation() const -> Vector4F
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_attenuation;
    }

    void LightState::setAttenuation( const Vector4F &attenuation )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_attenuation = attenuation;
        setDirty( true );
    }

    auto LightState::getLightType() const -> hash32
    {
        return m_lightType;
    }

    void LightState::setLightType( hash32 lightType )
    {
        m_lightType = lightType;
    }

    auto LightState::getRange() const -> f32
    {
        return m_range;
    }

    void LightState::setRange( f32 range )
    {
        m_range = range;
    }

    auto LightState::getConstant() const -> f32
    {
        return m_constant;
    }

    void LightState::setConstant( f32 constant )
    {
        m_constant = constant;
    }

    auto LightState::getLinear() const -> f32
    {
        return m_linear;
    }

    void LightState::setLinear( f32 linear )
    {
        m_linear = linear;
    }

    auto LightState::getQuadratic() const -> f32
    {
        return m_quadratic;
    }

    void LightState::setQuadratic( f32 quadratic )
    {
        m_quadratic = quadratic;
    }

    auto LightState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<LightState>();
        state->m_diffuseColour = m_diffuseColour;
        state->m_specularColour = m_specularColour;
        state->m_direction = m_direction;
        state->m_attenuation = m_attenuation;
        state->m_lightType = m_lightType;
        state->m_range = m_range;
        state->m_constant = m_constant;
        state->m_linear = m_linear;
        state->m_quadratic = m_quadratic;
        return state;
    }

}  // end namespace fb
