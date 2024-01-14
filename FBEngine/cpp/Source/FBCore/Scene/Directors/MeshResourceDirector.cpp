#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Directors/MeshResourceDirector.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, MeshResourceDirector, Director );

    MeshResourceDirector::MeshResourceDirector() = default;

    MeshResourceDirector::~MeshResourceDirector() = default;

    SmartPtr<Properties> MeshResourceDirector::getProperties() const
    {
        auto properties = ResourceDirector::getProperties();
        properties->setProperty( "resourcePath", getResourcePath() );
        properties->setProperty( "scale", m_scale );
        properties->setProperty( "constraints", m_constraints );
        properties->setProperty( "animation", m_animation );
        properties->setProperty( "visibility", m_visibility );
        properties->setProperty( "cameras", m_cameras );
        properties->setProperty( "swapZY", m_swapZY );
        properties->setProperty( "rotate90DegreesX", m_rotate90DegreesX );
        properties->setProperty( "lights", m_lights );
        properties->setProperty( "lightmapUVs", m_lightmapUVs );

        properties->setPropertyAsButton( "Save", "SaveButton" );
        properties->setPropertyAsButton( "Import", "ImportButton" );

        return properties;
    }

    void MeshResourceDirector::setProperties( SmartPtr<Properties> properties )
    {
        ResourceDirector::setProperties( properties );

        properties->getPropertyValue( "resourcePath", m_resourcePath );
        properties->getPropertyValue( "scale", m_scale );
        properties->getPropertyValue( "constraints", m_constraints );
        properties->getPropertyValue( "animation", m_animation );
        properties->getPropertyValue( "visibility", m_visibility );
        properties->getPropertyValue( "cameras", m_cameras );
        properties->getPropertyValue( "swapZY", m_swapZY );
        properties->getPropertyValue( "rotate90DegreesX", m_rotate90DegreesX );
        properties->getPropertyValue( "lights", m_lights );
        properties->getPropertyValue( "lightmapUVs", m_lightmapUVs );

        if(properties->isButtonPressed( "Save" ))
        {
            save();
        }

        if(properties->isButtonPressed( "Import" ))
        {
            import();
        }
    }

    void MeshResourceDirector::setLightmapUVs( bool lightmapUVs )
    {
        m_lightmapUVs = lightmapUVs;
    }

    bool MeshResourceDirector::getLightmapUVs() const
    {
        return m_lightmapUVs;
    }

    void MeshResourceDirector::setLights( bool lights )
    {
        m_lights = lights;
    }

    bool MeshResourceDirector::getLights() const
    {
        return m_lights;
    }

    void MeshResourceDirector::setSwapZY( bool swapZY )
    {
        m_swapZY = swapZY;
    }

    bool MeshResourceDirector::getSwapZY() const
    {
        return m_swapZY;
    }

    void MeshResourceDirector::setCameras( bool cameras )
    {
        m_cameras = cameras;
    }

    bool MeshResourceDirector::getCameras() const
    {
        return m_cameras;
    }

    void MeshResourceDirector::setVisibility( bool visibility )
    {
        m_visibility = visibility;
    }

    bool MeshResourceDirector::getVisibility() const
    {
        return m_visibility;
    }

    void MeshResourceDirector::setConstraints( bool constraints )
    {
        m_constraints = constraints;
    }

    bool MeshResourceDirector::getConstraints() const
    {
        return m_constraints;
    }

    void MeshResourceDirector::setScale( f32 scale )
    {
        m_scale = scale;
    }

    f32 MeshResourceDirector::getScale() const
    {
        return m_scale;
    }

    void MeshResourceDirector::setRotate90DegreesX( bool rotate90DegreesX )
    {
        m_rotate90DegreesX = rotate90DegreesX;
    }

    bool MeshResourceDirector::getRotate90DegreesX() const
    {
        return m_rotate90DegreesX;
    }

} // namespace fb::scene
