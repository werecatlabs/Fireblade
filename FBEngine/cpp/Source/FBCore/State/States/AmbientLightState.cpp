#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/AmbientLightState.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, AmbientLightState, BaseState );

    AmbientLightState::AmbientLightState() = default;
    AmbientLightState::~AmbientLightState() = default;

    void AmbientLightState::setEnvmapScale( f32 envmapScale )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_envmapScale = envmapScale;
        setDirty( true );
    }

    auto AmbientLightState::getEnvmapScale() const -> f32
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_envmapScale;
    }

    void AmbientLightState::setHemisphereDir( Vector3<real_Num> hemisphereDir )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_hemisphereDir = hemisphereDir;
        setDirty( true );
    }

    auto AmbientLightState::getHemisphereDir() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_hemisphereDir;
    }

    void AmbientLightState::setUpperHemisphere( const ColourF &upperHemisphere )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_upperHemisphere = upperHemisphere;
        setDirty( true );
    }

    auto AmbientLightState::getUpperHemisphere() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_upperHemisphere;
    }

    void AmbientLightState::setAmbientColour( const ColourF &ambientColour )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_ambientColour = ambientColour;
        setDirty( true );
    }

    auto AmbientLightState::getAmbientColour() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_ambientColour;
    }

    void AmbientLightState::setLowerHemisphere( const ColourF &lowerHemisphere )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_lowerHemisphere = lowerHemisphere;
        setDirty( true );
    }

    auto AmbientLightState::getLowerHemisphere() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_lowerHemisphere;
    }

}  // namespace fb
