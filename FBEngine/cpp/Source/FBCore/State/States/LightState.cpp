#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/LightState.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, LightState, BaseState );

    LightState::LightState()
    {
    }

    LightState::~LightState()
    {
    }

    ColourF LightState::getDiffuseColour() const
    {
        SpinRWMutex::ScopedLock lock(m_mutex, false);
        return m_diffuseColour;
    }

    void LightState::setDiffuseColour( const ColourF &diffuseColour )
    {
        SpinRWMutex::ScopedLock lock(m_mutex, true);
        m_diffuseColour = diffuseColour;
        setDirty( true );
    }

    ColourF LightState::getSpecularColour() const
    {
        SpinRWMutex::ScopedLock lock(m_mutex, false);
        return m_specularColour;
    }

    void LightState::setSpecularColour( const ColourF &specularColour )
    {
        SpinRWMutex::ScopedLock lock(m_mutex, true);
        m_specularColour = specularColour;
        setDirty( true );
    }

    Vector3F LightState::getDirection() const
    {
        return m_direction;
    }

    void LightState::setDirection( const Vector3F &direction )
    {
        m_direction = direction;
    }

    Vector4F LightState::getAttenuation() const
    {
        SpinRWMutex::ScopedLock lock(m_mutex, false);
        return m_attenuation;
    }

    void LightState::setAttenuation( const Vector4F &attenuation )
    {
        SpinRWMutex::ScopedLock lock(m_mutex, true);
        m_attenuation = attenuation;
        setDirty( true );
    }

    hash32 LightState::getLightType() const
    {
        return m_lightType;
    }

    void LightState::setLightType( hash32 lightType )
    {
        m_lightType = lightType;
    }

    f32 LightState::getRange() const
    {
        return m_range;
    }

    void LightState::setRange( f32 range )
    {
        m_range = range;
    }

    f32 LightState::getConstant() const
    {
        return m_constant;
    }

    void LightState::setConstant( f32 constant )
    {
        m_constant = constant;
    }

    f32 LightState::getLinear() const
    {
        return m_linear;
    }

    void LightState::setLinear( f32 linear )
    {
        m_linear = linear;
    }

    f32 LightState::getQuadratic() const
    {
        return m_quadratic;
    }

    void LightState::setQuadratic( f32 quadratic )
    {
        m_quadratic = quadratic;
    }
}  // end namespace fb
