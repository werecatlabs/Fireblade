#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/Mesh.h>
#include <FBCore/FBCore.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, Mesh, IMesh );

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

    void Mesh::updateAABB( bool forceSubMeshUpdate )
    {
        if( m_subMeshes.size() > 0 )
        {
            m_aabb.setMinimum( Vector3<real_Num>( 1e10, 1e10, 1e10 ) );
            m_aabb.setMaximum( Vector3<real_Num>( -1e10, -1e10, -1e10 ) );
        }
        else
        {
            m_aabb.reset( Vector3<real_Num>::zero() );
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

    auto Mesh::getAABB() const -> AABB3<real_Num>
    {
        return m_aabb;
    }

    void Mesh::setAABB( const AABB3<real_Num> &aabb )
    {
        m_aabb = aabb;
    }

    auto Mesh::clone() const -> SmartPtr<IMesh>
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto newMesh = factoryManager->make_ptr<Mesh>();
        auto subMeshes = getSubMeshes();

        for( auto &subMesh : subMeshes )
        {
            auto newSubMesh = subMesh->clone();
            FB_ASSERT( newSubMesh );
            newMesh->addSubMesh( newSubMesh );
        }

        auto aabb = getAABB();
        newMesh->setAABB( aabb );

        auto animationInterface = getAnimationInterface();
        auto cloneAnimationInterface = animationInterface->clone();
        newMesh->setAnimationInterface( cloneAnimationInterface );

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

    void Mesh::setHasSharedVertexData( bool hasSharedVertexData )
    {
        m_hasSharedVertexData = hasSharedVertexData;
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
