#ifndef _IDynamicMesh_H_
#define _IDynamicMesh_H_

#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    namespace render
    {
        /**
         * Interface for a dynamic graphics mesh.
         */
        class IDynamicMesh : public IGraphicsObject
        {
        public:
            /**
             * Virtual destructor.
             */
            ~IDynamicMesh() override = default;

            /**
             * Sets the mesh.
             *
             * @param mesh The mesh to set.
             */
            virtual void setMesh( SmartPtr<IMesh> mesh ) = 0;

            /**
             * Gets the mesh.
             *
             * @return The mesh.
             */
            virtual SmartPtr<IMesh> getMesh() const = 0;

            /**
             * Sets the sub mesh.
             *
             * @param subMesh The sub mesh to set.
             */
            virtual void setSubMesh( SmartPtr<ISubMesh> subMesh ) = 0;

            /**
             * Gets the sub mesh.
             *
             * @return The sub mesh.
             */
            virtual SmartPtr<ISubMesh> getSubMesh() const = 0;

            /**
             * Sets flag to update the mesh.
             *
             * @param dirty If true, the mesh is marked dirty and needs to be updated.
             */
            virtual void setDirty( bool dirty ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
