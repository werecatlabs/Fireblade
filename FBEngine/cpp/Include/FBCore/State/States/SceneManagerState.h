#ifndef SceneManagerState_h__
#define SceneManagerState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Base/ColourF.h>

namespace fb
{
    class SceneManagerState : public BaseState
    {
    public:
        SceneManagerState();
        ~SceneManagerState() override;

        String getSkyboxMaterialName() const;
        void setSkyboxMaterialName( const String &skyboxMaterialName );

        bool getEnableSkybox() const;
        void setEnableSkybox( bool enableSkybox );

        ColourF getAmbientLight() const;
        void setAmbientLight( const ColourF &ambientLight );

        bool getEnableShadows() const;
        void setEnableShadows( bool enableShadows );

        FB_CLASS_REGISTER_DECL;

    protected:
        ColourF m_ambientLight = ColourF::White;
        String m_skyboxMaterialName;
        atomic_bool m_enableSkybox = false;
        atomic_bool m_enableShadows = false;
    };
}  // end namespace fb

#endif  // SceneManagerState_h__
