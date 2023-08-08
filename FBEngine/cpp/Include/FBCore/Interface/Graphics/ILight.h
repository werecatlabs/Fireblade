#ifndef _ILight_H
#define _ILight_H

#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for a graphics light.
         */
        class ILight : public IGraphicsObject
        {
        public:
            /**
             * Defines the type of light.
             */
            enum class LightTypes
            {
                ///< Point light sources give off light equally in all directions, so require only position not direction
                LT_POINT = 0,
                ///< Directional lights simulate parallel light beams from a distant source, hence have direction but no position
                LT_DIRECTIONAL = 1,

                ///< Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff
                LT_SPOTLIGHT = 2,

                Count
            };

            /**
             * Hash codes for material properties.
             */
            static const hash_type VISIBILITY_MASK_HASH;  ///< Hash code for the light visibility mask.
            static const hash_type LIGHT_TYPE_HASH;       ///< Hash code for the light type.
            static const hash_type DIFFUSE_COLOUR_HASH;   ///< Hash code for the light's diffuse colour.
            static const hash_type SPECULAR_COLOUR_HASH;  ///< Hash code for the light's specular colour.

            /** Virtual destructor. */
            ~ILight() override = default;

            /**
             * Sets the type of the light.
             */
            virtual void setType( LightTypes type ) = 0;

            /**
             * Returns the type of the light.
             */
            virtual LightTypes getType() const = 0;

            /**
             * Sets the diffuse colour of the light.
             */
            virtual void setDiffuseColour( const ColourF &colour ) = 0;

            /**
             * Returns the diffuse colour of the light.
             */
            virtual ColourF getDiffuseColour() const = 0;

            /**
             * Sets the specular colour of the light.
             */
            virtual void setSpecularColour( const ColourF &colour ) = 0;

            /**
             * Returns the specular colour of the light.
             */
            virtual ColourF getSpecularColour() const = 0;

            /**
             * Sets the attenuation parameters of the light.
             */
            virtual void setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic ) = 0;

            /**
             * Returns the range of the attenuation of the light.
             */
            virtual f32 getAttenuationRange() const = 0;

            /**
             * Returns the constant attenuation factor of the light.
             */
            virtual f32 getAttenuationConstant() const = 0;

            /**
             * Returns the linear attenuation factor of the light.
             */
            virtual f32 getAttenuationLinear() const = 0;

            /**
             * Returns the quadratic attenuation factor of the light.
             */
            virtual f32 getAttenuationQuadric() const = 0;

            /**
             * Sets the direction in which the light points.
             */
            virtual void setDirection( const Vector3<real_Num> &vec ) = 0;

            /**
             * Returns the direction in which the light points.
             */
            virtual Vector3<real_Num> getDirection() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
