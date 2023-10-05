#ifndef CLight_h__
#define CLight_h__

#include <FBCore/Graphics/CGraphicsObject.h>
#include <FBCore/Interface/Graphics/ILight.h>

namespace fb
{
    namespace render
    {
        class CLight : public CGraphicsObject<ILight>
        {
        public:
            CLight();
            ~CLight() override;

            void setType( LightTypes type ) override;

            LightTypes getType() const override;

            void setDiffuseColour( const ColourF &colour ) override;

            ColourF getDiffuseColour() const override;

            void setSpecularColour( const ColourF &colour ) override;

            ColourF getSpecularColour() const override;

            void setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic ) override;

            f32 getAttenuationRange() const override;

            f32 getAttenuationConstant() const override;

            f32 getAttenuationLinear() const override;

            f32 getAttenuationQuadric() const override;

            void setDirection( const Vector3<real_Num> &vec ) override;

            Vector3<real_Num> getDirection() const override;
        };
    } // namespace render
}     // namespace fb

#endif  // CLight_h__
