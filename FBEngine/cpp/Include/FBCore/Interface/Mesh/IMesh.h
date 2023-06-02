#ifndef __IMesh_h__
#define __IMesh_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{

    /** An interface for a mesh object. */
    class IMesh : public ISharedObject
    {
    public:
        /* Destructor. */
        ~IMesh() override = default;

        /** Adds a submesh to the mesh. */
        virtual void addSubMesh( SmartPtr<ISubMesh> subMesh ) = 0;

        /** Removes a submesh from the mesh. */
        virtual void removeSubMesh( SmartPtr<ISubMesh> subMesh ) = 0;

        /** Removes all submeshes from the mesh. */
        virtual void removeAllSubMeshes() = 0;

        /** Gets all submeshes in the mesh. */
        virtual Array<SmartPtr<ISubMesh>> getSubMeshes() const = 0;

        /** Sets the name of the mesh. */
        virtual void setName( const String &name ) = 0;

        /** Gets the name of the mesh. */
        virtual String getName() const = 0;

        /** Updates the axis-aligned bounding box of the mesh. */
        virtual void updateAABB( bool forceSubMeshUpdate = false ) = 0;

        /** Gets the axis-aligned bounding box of the mesh. */
        virtual AABB3F getAABB() const = 0;

        /** Sets the axis-aligned bounding box of the mesh. */
        virtual void setAABB( const AABB3F &aabb ) = 0;

        /** Clones the mesh. */
        virtual SmartPtr<IMesh> clone() const = 0;

        /** Gets the animation interface for the mesh. */
        virtual SmartPtr<IAnimationInterface> getAnimationInterface() const = 0;

        /** Sets the animation interface for the mesh. */
        virtual void setAnimationInterface( SmartPtr<IAnimationInterface> animationInterface ) = 0;

        /** Gets whether the mesh has shared vertex data. */
        virtual bool getHasSharedVertexData() const = 0;

        /** Sets whether the mesh has shared vertex data. */
        virtual void setHasSharedVertexData( bool hasSharedVertexData ) = 0;

        /** Gets whether the mesh has a skeleton. */
        virtual bool hasSkeleton() const = 0;

        /** Gets the name of the skeleton used by the mesh. */
        virtual String getSkeletonName() const = 0;

        /** Gets the number of level-of-detail levels for the mesh. */
        virtual u32 getNumLodLevels() const = 0;

        /** Gets whether the mesh's edge list has been built. */
        virtual bool isEdgeListBuilt() const = 0;

        /** Gets whether the mesh has vertex animation. */
        virtual bool hasVertexAnimation() const = 0;

        /** Compares the mesh with another mesh. */
        virtual bool compare( SmartPtr<IMesh> other ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IMesh_h__
