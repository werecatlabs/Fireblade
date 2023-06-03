#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/Mesh.h>
#include <FBCore/FBCore.h>

namespace fb
{
    Mesh::Mesh()
    {
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::unload( SmartPtr<ISharedObject> data )
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

    void Mesh::addSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        m_subMeshes.push_back( subMesh );
    }

    void Mesh::removeSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        auto it = std::find( m_subMeshes.begin(), m_subMeshes.end(), subMesh );
        if(it != m_subMeshes.end())
        {
            m_subMeshes.erase( it );
        }
    }

    void Mesh::removeAllSubMeshes()
    {
        m_subMeshes.clear();
    }

    Array<SmartPtr<ISubMesh>> Mesh::getSubMeshes() const
    {
        return m_subMeshes;
    }

    void Mesh::setName( const String &name )
    {
        if (auto handle = getHandle())
        {
            handle->setName(name);
        }
    }

    String Mesh::getName() const
    {
        if( auto handle = getHandle() )
        {
            return handle->getName();
        }

        return "";
    }

    void Mesh::updateAABB( bool forceSubMeshUpdate )
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

    AABB3F Mesh::getAABB() const
    {
        return m_aabb;
    }

    void Mesh::setAABB( const AABB3F &aabb )
    {
        m_aabb = aabb;
    }

    SmartPtr<IMesh> Mesh::clone() const
    {
        SmartPtr<IMesh> newMesh = fb::make_ptr<Mesh>();
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

    void Mesh::setAnimationInterface( SmartPtr<IAnimationInterface> animationInterface )
    {
        m_animationInterface = animationInterface;
    }

    SmartPtr<IAnimationInterface> Mesh::getAnimationInterface() const
    {
        return m_animationInterface;
    }

    void Mesh::setHasSharedVertexData( bool val )
    {
        m_hasSharedVertexData = val;
    }

    bool Mesh::hasSkeleton() const
    {
        return false;
    }

    String Mesh::getSkeletonName() const
    {
        return "";
    }

    u32 Mesh::getNumLodLevels() const
    {
        return 0;
    }

    bool Mesh::isEdgeListBuilt() const
    {
        return false;
    }

    bool Mesh::hasVertexAnimation() const
    {
        return false;
    }

    bool Mesh::getHasSharedVertexData() const
    {
        return m_hasSharedVertexData;
    }

    bool Mesh::compare( SmartPtr<IMesh> other ) const
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
