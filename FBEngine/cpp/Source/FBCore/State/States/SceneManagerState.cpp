#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/SceneManagerState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, SceneManagerState, BaseState );

    SceneManagerState::SceneManagerState() = default;

    SceneManagerState::~SceneManagerState() = default;

    auto SceneManagerState::getSkyboxMaterialName() const -> String
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

    auto SceneManagerState::getEnableSkybox() const -> bool
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

    auto SceneManagerState::getAmbientLight() const -> ColourF
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

    auto SceneManagerState::getEnableShadows() const -> bool
    {
        return m_enableShadows;
    }

    void SceneManagerState::setEnableShadows( bool enableShadows )
    {
        m_enableShadows = enableShadows;
    }

}  // end namespace fb
