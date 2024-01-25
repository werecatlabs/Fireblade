#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/SceneLightingDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, SceneLightingDirector, Director );

    SceneLightingDirector::SceneLightingDirector()
    {
    }

    SceneLightingDirector::~SceneLightingDirector()
    {
    }

    SmartPtr<Properties> SceneLightingDirector::getProperties() const
    {
        auto properties = ResourceDirector::getProperties();
        properties->setProperty( "ambientColour", m_ambientColour );
        properties->setProperty( "upperHemisphere", m_upperHemisphere );
        properties->setProperty( "lowerHemisphere", m_lowerHemisphere );

        return properties;
    }

    void SceneLightingDirector::setProperties( SmartPtr<Properties> properties )
    {
        ResourceDirector::setProperties( properties );

        properties->getPropertyValue( "ambientColour", m_ambientColour );
        properties->getPropertyValue( "upperHemisphere", m_upperHemisphere );
        properties->getPropertyValue( "lowerHemisphere", m_lowerHemisphere );
    }

}  // namespace fb::scene
