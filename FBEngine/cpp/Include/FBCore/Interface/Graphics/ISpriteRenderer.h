#ifndef ISpriteRenderer_h__
#define ISpriteRenderer_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Matrix4.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for rendering 2D sprites.
         */
        class ISpriteRenderer : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ISpriteRenderer() override = default;

            /**
             * Renders a texture as a sprite.
             *
             * @param renderData    A shared object containing additional rendering data.
             * @param texture       The texture to use for rendering.
             * @param transform     The transformation matrix for the sprite.
             * @param colour        The colour to apply to the sprite.
             */
            virtual void render( const SmartPtr<ISharedObject> &renderData,
                                 const SmartPtr<ITexture> &texture, const Matrix4F &transform,
                                 const ColourF &colour ) = 0;

            /**
             * Renders a material as a sprite.
             *
             * @param renderData    A shared object containing additional rendering data.
             * @param material      The material to use for rendering.
             * @param transform     The transformation matrix for the sprite.
             * @param colour        The colour to apply to the sprite.
             */
            virtual void render( const SmartPtr<ISharedObject> &renderData,
                                 const SmartPtr<IMaterial> &material, const Matrix4F &transform,
                                 const ColourF &colour ) = 0;

            /**
             * Gets the internal object used for rendering.
             *
             * @param ppObject  A pointer to the internal object.
             */
            virtual void _getObject( void **ppObject ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // ISpriteRenderer_h__
