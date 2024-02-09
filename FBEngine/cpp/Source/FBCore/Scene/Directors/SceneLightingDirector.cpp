#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/SceneLightingDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, SceneLightingDirector, Director );

    SceneLightingDirector::SceneLightingDirector() = default;
    SceneLightingDirector::~SceneLightingDirector() = default;

    SmartPtr<Properties> SceneLightingDirector::getProperties() const
    {
        auto properties = ResourceDirector::getProperties();
        properties->setProperty( "ambientColour", m_ambientColour );
        properties->setProperty( "upperHemisphere", m_upperHemisphere );
        properties->setProperty( "lowerHemisphere", getLowerHemisphere() );

        return properties;
    }

    void SceneLightingDirector::setProperties( SmartPtr<Properties> properties )
    {
        ResourceDirector::setProperties( properties );

        properties->getPropertyValue( "ambientColour", m_ambientColour );
        properties->getPropertyValue( "upperHemisphere", m_upperHemisphere );
        properties->getPropertyValue( "lowerHemisphere", getLowerHemisphere() );
    }

    void SceneLightingDirector::setEnvmapScale( f32 envmapScale )
    {
        m_envmapScale = envmapScale;
    }

    f32 SceneLightingDirector::getEnvmapScale() const
    {
        return m_envmapScale;
    }

    void SceneLightingDirector::setHemisphereDir( Vector3<real_Num> hemisphereDir )
    {
        m_hemisphereDir = hemisphereDir;
    }

    Vector3<real_Num> SceneLightingDirector::getHemisphereDir() const
    {
        return m_hemisphereDir;
    }

    void SceneLightingDirector::setUpperHemisphere( const ColourF &upperHemisphere )
    {
        m_upperHemisphere = upperHemisphere;
    }

    ColourF SceneLightingDirector::getUpperHemisphere() const
    {
        return m_upperHemisphere;
    }

    void SceneLightingDirector::setAmbientColour( const ColourF &ambientColour )
    {
        m_ambientColour = ambientColour;
    }

    ColourF SceneLightingDirector::getAmbientColour() const
    {
        return m_ambientColour;
    }

    void SceneLightingDirector::setLowerHemisphere( const ColourF &lowerHemisphere )
    {
        m_lowerHemisphere = lowerHemisphere;
    }

    ColourF SceneLightingDirector::getLowerHemisphere() const
    {
        return m_lowerHemisphere;
    }
} // namespace fb::scene
