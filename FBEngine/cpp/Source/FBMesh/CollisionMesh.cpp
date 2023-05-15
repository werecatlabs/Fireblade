#include <FBMesh/FBMeshPCH.h>
#include <FBMesh/CollisionMesh.h>
#include <FBMesh/CollisionSubMesh.h>
#include <FBCore/Base/Exception.h>
#include <FBMesh/CSubMesh.h>
#include <FBMesh/CMesh.h>

namespace fb
{
    CollisionMesh::CollisionMesh()
    {
    }

    CollisionMesh::~CollisionMesh()
    {
    }

    void CollisionMesh::load( SmartPtr<ISharedObject> data )
    {
    }

    void CollisionMesh::unload( SmartPtr<ISharedObject> data )
    {
    }

    CollisionMesh::CollisionMesh( const SmartPtr<IMesh> &mesh, const Matrix4F &transform )
    {
        auto subMeshes = mesh->getSubMeshes();
        for(auto subMesh : subMeshes)
        {
            SmartPtr<CollisionSubMesh> collisionSubMesh(
                new CollisionSubMesh( mesh, subMesh, transform ) );
            m_subMeshes.push_back( collisionSubMesh );
        }
    }

    void CollisionMesh::HitData::setHitDistance( f32 hitDistance )
    {
        m_hitDistance = hitDistance;
    }

    f32 CollisionMesh::HitData::getHitDistance() const
    {
        return m_hitDistance;
    }

    CollisionMesh::HitData::HitData( float distance ) :
        m_hitDistance( distance )
    {
    }

    CollisionMesh::HitData::HitData() :
        m_hitDistance( 0.0f )
    {
    }

    bool CollisionMesh::rayCast( const Vector3F &origin, const Vector3F &dir,
                                 Array<HitData> &hits )
    {
        bool retValue = false;

        for(auto &subMesh : m_subMeshes)
        {
            Array<float> distances;
            retValue = subMesh->rayCast( origin, dir, distances );

            if(retValue)
            {
                hits.push_back( HitData( distances[0] ) );
                return true;
            }
        }

        return retValue;
    }

    bool CollisionMesh::rayCast( const Vector3F &origin, const Vector3F &dir, Array<f32> &hits )
    {
        bool retValue = false;

        for(auto &subMesh : m_subMeshes)
        {
            retValue = subMesh->rayCast( origin, dir, hits );
            if(retValue)
            {
                return true;
            }
        }

        return retValue;
    }
} // end namespace fb
