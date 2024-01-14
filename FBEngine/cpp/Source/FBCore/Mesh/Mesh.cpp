#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/Mesh.h>
#include <FBCore/FBCore.h>

namespace fb
{

    Mesh::Mesh() = default;

    Mesh::~Mesh() = default;

    void Mesh::unload( SmartPtr<ISharedObject> data )
    {
        for( auto subMesh : m_subMeshes )
        {
            subMesh->unload( nullptr );
        }

        m_subMeshes.clear();

        if( m_animationInterface )
        {
            m_animationInterface->unload( nullptr );
            m_animationInterface = nullptr;
        }
    }

    void Mesh::addSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        m_subMeshes.push_back( subMesh );
    }

    void Mesh::removeSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        auto it = std::find( m_subMeshes.begin(), m_subMeshes.end(), subMesh );
        if( it != m_subMeshes.end() )
        {
            m_subMeshes.erase( it );
        }
    }

    void Mesh::removeAllSubMeshes()
    {
        m_subMeshes.clear();
    }

    auto Mesh::getSubMeshes() const -> Array<SmartPtr<ISubMesh>>
    {
        return m_subMeshes;
    }

    void Mesh::setName( const String &name )
    {
        if( auto handle = getHandle() )
        {
            handle->setName( name );
        }
    }

    auto Mesh::getName() const -> String
    {
        if( auto handle = getHandle() )
        {
            return handle->getName();
        }

        return "";
    }

    void Mesh::updateAABB( bool forceSubMeshUpdate )
    {
        if( m_subMeshes.size() > 0 )
        {
            m_aabb.setMinimum( Vector3F( 1e10, 1e10, 1e10 ) );
            m_aabb.setMaximum( Vector3F( -1e10, -1e10, -1e10 ) );
        }
        else
        {
            m_aabb.reset( Vector3F::zero() );
        }

        for( auto &subMesh : m_subMeshes )
        {
            if( forceSubMeshUpdate )
            {
                subMesh->updateAABB();
            }

            auto aabb = subMesh->getAABB();
            m_aabb.merge( aabb );
        }
    }

    auto Mesh::getAABB() const -> AABB3F
    {
        return m_aabb;
    }

    void Mesh::setAABB( const AABB3F &aabb )
    {
        m_aabb = aabb;
    }

    auto Mesh::clone() const -> SmartPtr<IMesh>
    {
        SmartPtr<IMesh> newMesh = fb::make_ptr<Mesh>();
        Array<SmartPtr<ISubMesh>> subMeshes = getSubMeshes();
        for( auto &subMesh : subMeshes )
        {
            SmartPtr<ISubMesh> newSubMesh = subMesh->clone();
            FB_ASSERT( newSubMesh );
            newMesh->addSubMesh( newSubMesh );
        }

        AABB3F aabb = getAABB();
        newMesh->setAABB( aabb );

        // SmartPtr<IAnimationInterface> animationInterface = getAnimationInterface();
        // animationInterface->clone();

        return newMesh;
    }

    void Mesh::setAnimationInterface( SmartPtr<IAnimationInterface> animationInterface )
    {
        m_animationInterface = animationInterface;
    }

    auto Mesh::getAnimationInterface() const -> SmartPtr<IAnimationInterface>
    {
        return m_animationInterface;
    }

    void Mesh::setHasSharedVertexData( bool val )
    {
        m_hasSharedVertexData = val;
    }

    auto Mesh::hasSkeleton() const -> bool
    {
        return false;
    }

    auto Mesh::getSkeletonName() const -> String
    {
        return "";
    }

    auto Mesh::getNumLodLevels() const -> u32
    {
        return 0;
    }

    auto Mesh::isEdgeListBuilt() const -> bool
    {
        return false;
    }

    auto Mesh::hasVertexAnimation() const -> bool
    {
        return false;
    }

    auto Mesh::getHasSharedVertexData() const -> bool
    {
        return m_hasSharedVertexData;
    }

    auto Mesh::compare( SmartPtr<IMesh> other ) const -> bool
    {
        auto subMeshes = getSubMeshes();
        auto otherSubMeshes = other->getSubMeshes();
        if( subMeshes.size() != otherSubMeshes.size() )
        {
            return false;
        }

        for( auto subMesh : subMeshes )
        {
            for( auto otherSubMesh : otherSubMeshes )
            {
            }
        }

        return true;
    }

}  // end namespace fb
