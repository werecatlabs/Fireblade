#ifndef LightState_h__
#define LightState_h__

#include <FBCore/State/States/GraphicsObjectState.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class LightState : public GraphicsObjectState
    {
    public:
        LightState();
        ~LightState() override;

        ColourF getDiffuseColour() const;
        void setDiffuseColour( const ColourF &diffuseColour );

        ColourF getSpecularColour() const;
        void setSpecularColour( const ColourF &specularColour );

        Vector3F getDirection() const;

        void setDirection( const Vector3F &direction );

        Vector4F getAttenuation() const;
        void setAttenuation( const Vector4F &attenuation );

        hash32 getLightType() const;
        void setLightType( hash32 lightType );

        f32 getRange() const;

        void setRange( f32 range );

        f32 getConstant() const;

        void setConstant( f32 constant );

        f32 getLinear() const;

        void setLinear( f32 linear );

        f32 getQuadratic() const;

        void setQuadratic( f32 quadratic );

        SmartPtr<IState> clone() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        ColourF m_diffuseColour;
        ColourF m_specularColour;
        Vector3F m_direction = Vector3F::unitY();

        Vector4F m_attenuation;
        hash32 m_lightType = 0;

        f32 m_range = 1000.0f;

        f32 m_constant = 1.0f;
        f32 m_linear = 1.0f;
        f32 m_quadratic =1.0f;
    };
}  // end namespace fb

#endif  // LightState_h__
