#ifndef __FBMesh__H
#define __FBMesh__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Mesh/IMesh.h>

#include <FBCore/Core/Array.h>

namespace fb
{
    
    /// Implementation of a mesh.
    class Mesh : public IMesh
    {
    public:
        Mesh();
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

        /** @copydoc IMesh::setName */
        void setName( const String &name ) override;

        /** @copydoc IMesh::getName */
        String getName() const override;

        /** @copydoc IMesh::updateAABB */
        void updateAABB( bool forceSubMeshUpdate = false ) override;

        /** @copydoc IMesh::getAABB */
        AABB3F getAABB() const override;

        /** @copydoc IMesh::setAABB */
        void setAABB( const AABB3F &aabb ) override;

        /** @copydoc IMesh::clone */
        SmartPtr<IMesh> clone() const override;

        /** @copydoc IMesh::getAnimationInterface */
        SmartPtr<IAnimationInterface> getAnimationInterface() const override;

        /** @copydoc IMesh::setAnimationInterface */
        void setAnimationInterface( SmartPtr<IAnimationInterface> animationInterface ) override;

        /** @copydoc IMesh::getSkeletonName */
        bool getHasSharedVertexData() const override;

        /** @copydoc IMesh::getSkeletonName */
        void setHasSharedVertexData( bool val ) override;

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

    protected:
        SmartPtr<IAnimationInterface> m_animationInterface;
        Array<SmartPtr<ISubMesh>> m_subMeshes;
        AABB3F m_aabb;
        bool m_hasSharedVertexData = false;
    };
} // end namespace fb

#endif
