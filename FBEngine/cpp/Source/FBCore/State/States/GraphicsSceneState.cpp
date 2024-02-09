#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/GraphicsSceneState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, GraphicsSceneState, BaseState );

    GraphicsSceneState::GraphicsSceneState() = default;

    GraphicsSceneState::~GraphicsSceneState() = default;

    auto GraphicsSceneState::getSkyboxMaterialName() const -> String
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_skyboxMaterialName;
    }

    void GraphicsSceneState::setSkyboxMaterialName( const String &skyboxMaterialName )
    {
        if( getSkyboxMaterialName() != skyboxMaterialName )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_skyboxMaterialName = skyboxMaterialName;
            setDirty( true );
        }
    }

    auto GraphicsSceneState::getEnableSkybox() const -> bool
    {
        return m_enableSkybox;
    }

    void GraphicsSceneState::setEnableSkybox( bool enableSkybox )
    {
        if( getEnableSkybox() != enableSkybox )
        {
            m_enableSkybox = enableSkybox;
            setDirty( true );
        }
    }

    auto GraphicsSceneState::getAmbientLight() const -> ColourF
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_ambientLight;
    }

    void GraphicsSceneState::setAmbientLight( const ColourF &ambientLight )
    {
        //if( getAmbientLight() != ambientLight )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_ambientLight = ambientLight;
            setDirty( true );
        }
    }

    auto GraphicsSceneState::getEnableShadows() const -> bool
    {
        return m_enableShadows;
    }

    void GraphicsSceneState::setEnableShadows( bool enableShadows )
    {
        m_enableShadows = enableShadows;
    }

}  // end namespace fb
