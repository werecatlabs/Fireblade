#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/CMesh.h>
#include <FBCore/FBCore.h>

namespace fb
{
    CMesh::CMesh()
    {
    }

    CMesh::~CMesh()
    {
    }

    void CMesh::unload( SmartPtr<ISharedObject> data )
    {
        for(auto subMesh : m_subMeshes)
        {
            subMesh->unload( nullptr );
        }

        m_subMeshes.clear();

        if(m_animationInterface)
        {
            m_animationInterface->unload( nullptr );
            m_animationInterface = nullptr;
        }
    }

    void CMesh::addSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        m_subMeshes.push_back( subMesh );
    }

    void CMesh::removeSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        auto it = std::find( m_subMeshes.begin(), m_subMeshes.end(), subMesh );
        if(it != m_subMeshes.end())
        {
            m_subMeshes.erase( it );
        }
    }

    void CMesh::removeAllSubMeshes()
    {
        m_subMeshes.clear();
    }

    Array<SmartPtr<ISubMesh>> CMesh::getSubMeshes() const
    {
        return m_subMeshes;
    }

    void CMesh::setName( const String &name )
    {
        if (auto handle = getHandle())
        {
            handle->setName(name);
        }
    }

    String CMesh::getName() const
    {
        if( auto handle = getHandle() )
        {
            return handle->getName();
        }

        return "";
    }

    void CMesh::updateAABB( bool forceSubMeshUpdate )
    {
        if(m_subMeshes.size() > 0)
        {
            m_aabb.setMinimum( Vector3F( 1e10, 1e10, 1e10 ) );
            m_aabb.setMaximum( Vector3F( -1e10, -1e10, -1e10 ) );
        }
        else
        {
            m_aabb.reset( Vector3F::zero() );
        }

        for(auto &subMesh : m_subMeshes)
        {
            if(forceSubMeshUpdate)
            {
                subMesh->updateAABB();
            }

            auto aabb = subMesh->getAABB();
            m_aabb.merge( aabb );
        }
    }

    AABB3F CMesh::getAABB() const
    {
        return m_aabb;
    }

    void CMesh::setAABB( const AABB3F &aabb )
    {
        m_aabb = aabb;
    }

    SmartPtr<IMesh> CMesh::clone() const
    {
        SmartPtr<IMesh> newMesh = fb::make_ptr<CMesh>();
        Array<SmartPtr<ISubMesh>> subMeshes = getSubMeshes();
        for(auto &subMesh : subMeshes)
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

    void CMesh::setAnimationInterface( SmartPtr<IAnimationInterface> animationInterface )
    {
        m_animationInterface = animationInterface;
    }

    SmartPtr<IAnimationInterface> CMesh::getAnimationInterface() const
    {
        return m_animationInterface;
    }

    void CMesh::setHasSharedVertexData( bool val )
    {
        m_hasSharedVertexData = val;
    }

    bool CMesh::hasSkeleton() const
    {
        return false;
    }

    String CMesh::getSkeletonName() const
    {
        return "";
    }

    u32 CMesh::getNumLodLevels() const
    {
        return 0;
    }

    bool CMesh::isEdgeListBuilt() const
    {
        return false;
    }

    bool CMesh::hasVertexAnimation() const
    {
        return false;
    }

    bool CMesh::getHasSharedVertexData() const
    {
        return m_hasSharedVertexData;
    }

    bool CMesh::compare( SmartPtr<IMesh> other ) const
    {
        auto subMeshes = getSubMeshes();
        auto otherSubMeshes = other->getSubMeshes();
        if(subMeshes.size() != otherSubMeshes.size())
        {
            return false;
        }

        for(auto subMesh : subMeshes)
        {
            for(auto otherSubMesh : otherSubMeshes)
            {
            }
        }

        return true;
    }

} // end namespace fb
