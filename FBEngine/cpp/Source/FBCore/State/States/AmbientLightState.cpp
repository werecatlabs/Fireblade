#include <FBCore/FBCorePCH.h>
#include "FBCore/State/States/AmbientLightState.h"

namespace fb
{

    AmbientLightState::AmbientLightState() = default;
    AmbientLightState::~AmbientLightState() = default;

    void AmbientLightState::setEnvmapScale( f32 envmapScale )
    {
        m_envmapScale = envmapScale;
    }

    auto AmbientLightState::getEnvmapScale() const -> f32
    {
        return m_envmapScale;
    }

    void AmbientLightState::setHemisphereDir( Vector3<real_Num> hemisphereDir )
    {
        m_hemisphereDir = hemisphereDir;
    }

    auto AmbientLightState::getHemisphereDir() const -> Vector3<real_Num>
    {
        return m_hemisphereDir;
    }

    void AmbientLightState::setUpperHemisphere( const ColourF &upperHemisphere )
    {
        m_upperHemisphere = upperHemisphere;
    }

    auto AmbientLightState::getUpperHemisphere() const -> ColourF
    {
        return m_upperHemisphere;
    }

    void AmbientLightState::setAmbientColour( const ColourF &ambientColour )
    {
        m_ambientColour = ambientColour;
    }

    auto AmbientLightState::getAmbientColour() const -> ColourF
    {
        return m_ambientColour;
    }

    void AmbientLightState::setLowerHemisphere( const ColourF &lowerHemisphere )
    {
        m_lowerHemisphere = lowerHemisphere;
    }

    auto AmbientLightState::getLowerHemisphere() const -> ColourF
    {
        return m_lowerHemisphere;
    }

}  // namespace fb
