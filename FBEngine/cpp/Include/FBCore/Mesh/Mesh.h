#ifndef __FBMesh__H
#define __FBMesh__H

#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /// Implementation of a mesh.
    class Mesh : public IMesh
    {
    public:
        /** Constructor. */
        Mesh();

        /** Destructor. */
        ~Mesh() override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IMesh::addSubMesh */
        void addSubMesh( SmartPtr<ISubMesh> subMesh ) override;

        /** @copydoc IMesh::removeSubMesh */
        void removeSubMesh( SmartPtr<ISubMesh> subMesh ) override;

        /** @copydoc IMesh::removeAllSubMeshes */
        void removeAllSubMeshes() override;

        /** @copydoc IMesh::getSubMeshes */
        Array<SmartPtr<ISubMesh>> getSubMeshes() const override;

        /** @copydoc IMesh::updateAABB */
        void updateAABB( bool forceSubMeshUpdate = false ) override;

        /** @copydoc IMesh::getAABB */
        AABB3<real_Num> getAABB() const override;

        /** @copydoc IMesh::setAABB */
        void setAABB( const AABB3<real_Num> &aabb ) override;

        /** @copydoc IMesh::clone */
        SmartPtr<IMesh> clone() const override;

        /** @copydoc IMesh::getAnimationInterface */
        SmartPtr<IAnimationInterface> getAnimationInterface() const override;

        /** @copydoc IMesh::setAnimationInterface */
        void setAnimationInterface( SmartPtr<IAnimationInterface> animationInterface ) override;

        /** @copydoc IMesh::getSkeletonName */
        bool getHasSharedVertexData() const override;

        /** @copydoc IMesh::getSkeletonName */
        void setHasSharedVertexData( bool hasSharedVertexData ) override;

        /** @copydoc IMesh::getSkeletonName */
        bool hasSkeleton() const override;

        /** @copydoc IMesh::getSkeletonName */
        String getSkeletonName() const override;

        /** @copydoc IMesh::getNumLodLevels */
        u32 getNumLodLevels() const override;

        /** @copydoc IMesh::isEdgeListBuilt */
        bool isEdgeListBuilt() const override;

        /** @copydoc IMesh::hasVertexAnimation */
        bool hasVertexAnimation() const override;

        /** @copydoc IMesh::compare */
        bool compare( SmartPtr<IMesh> other ) const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        // The animation interface.
        SmartPtr<IAnimationInterface> m_animationInterface;

        // The submeshes.
        Array<SmartPtr<ISubMesh>> m_subMeshes;

        // The bounding box.
        AABB3<real_Num> m_aabb;

        // To know if the vertex data is shared between submeshes.
        bool m_hasSharedVertexData = false;
    };
}  // end namespace fb

#endif
