#ifndef __GraphicsObject_Light_h__
#define __GraphicsObject_Light_h__

#include <FBCore/Graphics/GraphicsObject.h>
#include <FBCore/Interface/Graphics/ILight.h>

namespace fb
{
    namespace render
    {

        /** Implementation of the ILight interface.
        */
        class Light : public GraphicsObject<ILight>
        {
        public:
            /** Constructor. */
            Light();

            /** Destructor. */
            ~Light() override;

            /** @copydoc GraphicsObject<ILight>::setType */
            void setType( LightTypes type ) override;

            /** @copydoc GraphicsObject<ILight>::getType */
            LightTypes getType() const override;

            /** @copydoc GraphicsObject<ILight>::setDiffuseColour */
            void setDiffuseColour( const ColourF &colour ) override;

            /** @copydoc GraphicsObject<ILight>::getDiffuseColour */
            ColourF getDiffuseColour() const override;

            /** @copydoc GraphicsObject<ILight>::setSpecularColour */
            void setSpecularColour( const ColourF &colour ) override;

            /** @copydoc GraphicsObject<ILight>::getSpecularColour */
            ColourF getSpecularColour() const override;

            /** @copydoc GraphicsObject<ILight>::setAttenuation */
            void setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic ) override;

            /** @copydoc GraphicsObject<ILight>::getAttenuationRange */
            f32 getAttenuationRange() const override;

            /** @copydoc GraphicsObject<ILight>::getAttenuationConstant */
            f32 getAttenuationConstant() const override;

            /** @copydoc GraphicsObject<ILight>::getAttenuationLinear */
            f32 getAttenuationLinear() const override;

            /** @copydoc GraphicsObject<ILight>::getAttenuationQuadric */
            f32 getAttenuationQuadric() const override;

            /** @copydoc GraphicsObject<ILight>::setDirection */
            void setDirection( const Vector3<real_Num> &vec ) override;

            /** @copydoc GraphicsObject<ILight>::getDirection */
            Vector3<real_Num> getDirection() const override;
        };
    }  // namespace render
}  // namespace fb

#endif  // CLight_h__
