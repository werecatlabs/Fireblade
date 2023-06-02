#ifndef IMaterialNodeTexture_h__
#define IMaterialNodeTexture_h__

#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {

        /**
         * An interface for a material texture node.
         */
        class IMaterialTexture : public IMaterialNode
        {
        public:
            /** Virtual destructor. */
            ~IMaterialTexture() override = default;

            /** Gets the texture name.
            @return A string object containing the name.
            */
            virtual String getTextureName() const = 0;

            /** Sets the texture name.
            @param name A string object containing the name.
            */
            virtual void setTextureName( const String &name ) = 0;

            /** Gets the texture.
            @return A pointer to the texture.
            */
            virtual SmartPtr<ITexture> getTexture() const = 0;

            /** Sets the texture.
            @param texture A pointer to the texture.
            */
            virtual void setTexture( SmartPtr<ITexture> texture ) = 0;

            /** Sets the scale of the texture.
            @param scale The scaling factor as a 3D vector.
            */
            virtual void setScale( const Vector3F &scale ) = 0;

            /** Gets the animator for the texture.
            @return A pointer to the animator.
            */
            virtual SmartPtr<IAnimator> getAnimator() const = 0;

            /** Sets the animator for the texture.
            @param animator A pointer to the animator.
            */
            virtual void setAnimator( SmartPtr<IAnimator> animator ) = 0;

            /** Gets the tint color for the texture.
            @return The tint color as a `ColourF` object.
            */
            virtual ColourF getTint() const = 0;

            /** Sets the tint color for the texture.
            @param tint The tint color as a `ColourF` object.
            */
            virtual void setTint( const ColourF &tint ) = 0;

            /** Gets the texture type.
            @return An unsigned integer indicating the texture type.
            */
            virtual u32 getTextureType() const = 0;

            /** Sets the texture type.
            @param textureType An unsigned integer indicating the texture type.
            */
            virtual void setTextureType( u32 textureType ) = 0;

            /** Gets a pointer to the underlying object. This is dependent on the graphics library used.
             */
            virtual void _getObject( void **ppObject ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialNodeTexture_h__
