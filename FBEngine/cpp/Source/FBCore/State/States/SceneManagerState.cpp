#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/SceneManagerState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, SceneManagerState, BaseState );

    SceneManagerState::SceneManagerState()
    {
    }

    SceneManagerState::~SceneManagerState()
    {
    }

    String SceneManagerState::getSkyboxMaterialName() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_skyboxMaterialName;
    }

    void SceneManagerState::setSkyboxMaterialName( const String &skyboxMaterialName )
    {
        if( getSkyboxMaterialName() != skyboxMaterialName )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_skyboxMaterialName = skyboxMaterialName;
            setDirty( true );
        }
    }

    bool SceneManagerState::getEnableSkybox() const
    {
        return m_enableSkybox;
    }

    void SceneManagerState::setEnableSkybox( bool enableSkybox )
    {
        if( getEnableSkybox() != enableSkybox )
        {
            m_enableSkybox = enableSkybox;
            setDirty( true );
        }
    }

    ColourF SceneManagerState::getAmbientLight() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_ambientLight;
    }

    void SceneManagerState::setAmbientLight( const ColourF &ambientLight )
    {
        //if( getAmbientLight() != ambientLight )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_ambientLight = ambientLight;
            setDirty( true );
        }
    }

bool SceneManagerState::getEnableShadows() const
    {
        return m_enableShadows;
    }

    void SceneManagerState::setEnableShadows( bool enableShadows )
    {
        m_enableShadows = enableShadows;
    }

}  // end namespace fb
