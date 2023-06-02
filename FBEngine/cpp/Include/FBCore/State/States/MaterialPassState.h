#ifndef MaterialPassState_h__
#define MaterialPassState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Core/ColourF.h>

namespace fb
{
    class MaterialPassState : public BaseState
    {
    public:
        MaterialPassState();
        ~MaterialPassState() override;

        SmartPtr<IState> clone() const override;

        void assign( SmartPtr<IState> state ) override;

        ColourF getTint() const;

        void setTint( ColourF tint );

        ColourF getAmbient() const;

        void setAmbient( ColourF ambient );

        ColourF getDiffuse() const;

        void setDiffuse( ColourF diffuse );

        ColourF getSpecular() const;

        void setSpecular( ColourF specular );

        ColourF getEmissive() const;

        void setEmissive( ColourF emissive );

        f32 getMetalness() const;

        void setMetalness( f32 metalness );

        f32 getRoughness() const;

        void setRoughness( f32 roughness );

        bool isLightingEnabled() const;

        void setLightingEnabled( bool lightingEnabled );

        FB_CLASS_REGISTER_DECL;

    private:
        ColourF m_tint = ColourF::White;
        ColourF m_specular = ColourF::White;
        ColourF m_ambient = ColourF::White;
        ColourF m_diffuse = ColourF::White;
        ColourF m_emissive = ColourF::Black;
        f32 m_metalness = 1.0f;
        f32 m_roughness = 0.01f;
        bool m_lightingEnabled = true;
    };
}  // end namespace fb

#endif  // MaterialPassState_h__
