#ifndef __FBMesh_CollisionMesh_h__
#define __FBMesh_CollisionMesh_h__

#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Math/Matrix4.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    //--------------------------------------------
    class CollisionMesh : public CSharedObject<ISharedObject>
    {
    public:
        class HitData
        {
        public:
            HitData();
            HitData( f32 distance );

            f32 getHitDistance() const;
            void setHitDistance( f32 hitDistance );

        protected:
            f32 m_hitDistance = 0.0f;
        };

        CollisionMesh();
        CollisionMesh( const SmartPtr<IMesh> &mesh, const Matrix4F &transform );
        ~CollisionMesh() override;

        /** @copydoc IObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        bool rayCast( const Vector3F &origin, const Vector3F &dir, Array<f32> &hits );
        bool rayCast( const Vector3F &origin, const Vector3F &dir, Array<HitData> &hits );

    private:
        Array<SmartPtr<CollisionSubMesh>> m_subMeshes;
    };
} // end namespace fb

#endif  // CollisionMesh_h__
