#ifndef ISprite_h__
#define ISprite_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Matrix4.h>

namespace fb
{
    namespace render
    {
        /**
         * Represents a 2D sprite object.
         */
        class ISprite : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ISprite() override = default;

            /** Gets the texture used by the sprite. */
            virtual SmartPtr<ITexture> getTexture() const = 0;

            /** Sets the texture used by the sprite. */
            virtual void setTexture( SmartPtr<ITexture> texture ) = 0;

            /** Gets the transformation matrix for the sprite. */
            virtual Matrix4F getTransform() const = 0;

            /** Sets the transformation matrix for the sprite. */
            virtual void setTransform( const Matrix4F &transform ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // ISprite_h__
