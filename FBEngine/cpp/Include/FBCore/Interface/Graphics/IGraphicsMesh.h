#ifndef _IGraphicsMesh_H
#define _IGraphicsMesh_H

#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for a graphics mesh.
         */
        class IGraphicsMesh : public IGraphicsObject
        {
        public:
            static const hash_type RENDER_QUEUE_HASH;
            static const hash_type VISIBILITY_FLAGS_HASH;

            /** Virtual destructor. */
            ~IGraphicsMesh() override = default;

            /**
             * Sets the name of the material to be used for rendering.
             *
             * @param materialName The name of the material.
             * @param index The index of the submesh (default value is -1 for all submeshes).
             */
            virtual void setMaterialName( const String &materialName, s32 index = -1 ) = 0;

            /**
             * Gets the name of the material used by the mesh.
             *
             * @param index The index of the submesh (default value is -1 for all submeshes).
             *
             * @return The name of the material.
             */
            virtual String getMaterialName( s32 index = -1 ) const = 0;

            /**
             * Sets the material to be used for rendering.
             *
             * @param material The material to use.
             * @param index The index of the submesh (default value is -1 for all submeshes).
             */
            virtual void setMaterial( SmartPtr<IMaterial> material, s32 index = -1 ) = 0;

            /**
             * Gets the material used by the mesh.
             *
             * @param index The index of the submesh (default value is -1 for all submeshes).
             *
             * @return The material.
             */
            virtual SmartPtr<IMaterial> getMaterial( s32 index = -1 ) const = 0;

            /**
             * Sets whether hardware animation is enabled for the mesh.
             *
             * @param enabled `true` to enable hardware animation, `false` to disable it.
             */
            virtual void setHardwareAnimationEnabled( bool enabled ) = 0;

            /**
             * Checks whether the processing is done in hardware or software for the mesh.
             */
            virtual void checkVertexProcessing() = 0;

            /**
             * Gets an animation controller for this mesh.
             *
             * @return The animation controller.
             */
            virtual SmartPtr<IAnimationController> getAnimationController() = 0;

            /**
             * Gets the name of the mesh.
             *
             * @return The name of the mesh.
             */
            virtual String getMeshName() const = 0;

            /**
             * Sets the name of the mesh.
             *
             * @param meshName The name of the mesh.
             */
            virtual void setMeshName( const String &meshName ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
