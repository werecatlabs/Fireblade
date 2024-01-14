#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/CollisionMesh.h>
#include <FBCore/Mesh/CollisionSubMesh.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Mesh/SubMesh.h>
#include <FBCore/Mesh/Mesh.h>
#include <FBCore/FBCore.h>

namespace fb
{

    CollisionMesh::CollisionMesh() = default;

    CollisionMesh::~CollisionMesh() = default;

    void CollisionMesh::load( SmartPtr<ISharedObject> data )
    {
    }

    void CollisionMesh::unload( SmartPtr<ISharedObject> data )
    {
    }

    CollisionMesh::CollisionMesh( const SmartPtr<IMesh> &mesh, const Matrix4F &transform )
    {
        auto subMeshes = mesh->getSubMeshes();
        for( auto subMesh : subMeshes )
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

    auto CollisionMesh::HitData::getHitDistance() const -> f32
    {
        return m_hitDistance;
    }

    CollisionMesh::HitData::HitData( float distance ) : m_hitDistance( distance )
    {
    }

    CollisionMesh::HitData::HitData()
    {
    }

    auto CollisionMesh::rayCast( const Vector3F &origin, const Vector3F &dir, Array<HitData> &hits )
        -> bool
    {
        bool retValue = false;

        for( auto &subMesh : m_subMeshes )
        {
            Array<float> distances;
            retValue = subMesh->rayCast( origin, dir, distances );

            if( retValue )
            {
                hits.emplace_back( distances[0] );
                return true;
            }
        }

        return retValue;
    }

    auto CollisionMesh::rayCast( const Vector3F &origin, const Vector3F &dir, Array<f32> &hits ) -> bool
    {
        bool retValue = false;

        for( auto &subMesh : m_subMeshes )
        {
            retValue = subMesh->rayCast( origin, dir, hits );
            if( retValue )
            {
                return true;
            }
        }

        return retValue;
    }

}  // end namespace fb
