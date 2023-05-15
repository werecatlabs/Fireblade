#ifndef FBPhysxMeshShape_h__
#define FBPhysxMeshShape_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IMeshShape.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBPhysx/FBPhysxShape.h>
#include <geometry/PxBoxGeometry.h>

namespace fb
{
    namespace physics
    {
        /** Implementation of a mesh shape. */
        class PhysxMeshShape : public PhysxShape<IMeshShape>
        {
        public:
            /** Constructor */
            PhysxMeshShape();

            /** Destructor */
            ~PhysxMeshShape() override;

            /** @copydoc PhysxShape<IMeshShape>::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc PhysxShape<IMeshShape>::load */
            void unload( SmartPtr<ISharedObject> data ) override;

            Vector3<real_Num> getExtents() const;
            void setExtents( const Vector3<real_Num> &extents );

            void setAABB( const AABB3<real_Num> &box );
            AABB3<real_Num> getAABB() const;

            Transform3<real_Num> getWorldTransform( const Transform3<real_Num> &t );
            physx::PxTransform getMeshTransform( const Transform3<real_Num> &t );

            String getMeshCachePath( u32 subMeshIndex ) const;

            void build();
            void createShape() override;

            physx::PxBoxGeometry createGeometry();

            SmartPtr<IMeshResource> getMeshResource() const override;

            void setMeshResource( SmartPtr<IMeshResource> meshResource ) override;

            SmartPtr<IMesh> getMesh() const override;

            SmartPtr<IMesh> getCleanMesh() const override;
            void setCleanMesh( SmartPtr<IMesh> cleanMesh ) override;

            String getMeshPath() const;
            void setMeshPath( const String &meshPath );

            bool isValid() const override;

        protected:
            class MeshShapeStateListener : public ShapeStateListener
            {
            public:
                MeshShapeStateListener() = default;
                ~MeshShapeStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            void setIndices( s32 subMesh, const Array<u32> &indices );

            s32 getNumSubMeshes() const;

            Array<Vector3<real_Num>> &getVertices();
            const Array<Vector3<real_Num>> &getVertices() const;
            void setVertices( const Array<Vector3<real_Num>> &val );

            Array<u32> &getIndices( s32 subMeshIdx );
            Transform3<real_Num> getSubMeshTransform( s32 subMeshIdx );

            bool isConvex() const;
            void setConvex( bool convex );

            void cookMesh();
            void cookMesh( const String &path );

            Array<Array<u16>> getIndicesAs2dArray() const;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;
            SmartPtr<Properties> getProperties() const override;
            void setProperties( SmartPtr<Properties> properties ) override;

            void setupCollisionMask( physx::PxShape *shape );

            void readMeshData();

            void buildSubMesh( s32 subMeshIdx, const Array<u16> &indexBuffer,
                               const Transform3<real_Num> &t );

            void createStateObject() override;

            physx::PxRigidDynamic *m_dynamicActor = nullptr;
            physx::PxRigidStatic *m_staticActor = nullptr;

            Transform3<real_Num> m_colliderTransform;

            Vector3<real_Num> m_extents = Vector3<real_Num>::unit();

            SmartPtr<IMeshResource> m_meshResource;

            //AtomicSmartPtr<IMesh> m_mesh;
            AtomicSmartPtr<IMesh> m_cleanMesh;

            String m_meshPath;

            Array<Vector3<real_Num>> m_sharedVertices;
            Array<Array<Vector3<real_Num>>> m_vertices;
            Array<Array<u32>> m_indices;

            Array<Transform3<real_Num>> m_transforms;

            atomic_bool m_isConvex;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxMeshShape_h__
