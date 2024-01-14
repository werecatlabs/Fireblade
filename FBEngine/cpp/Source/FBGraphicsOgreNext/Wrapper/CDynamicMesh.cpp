#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CDynamicMesh.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/DynamicMesh.h>
#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Interface/Mesh/ISubMesh.h>
#include <Ogre.h>

#include <utility>

namespace fb::render
{
    CDynamicMesh::CDynamicMesh( SmartPtr<IGraphicsScene> creator ) : m_creator( std::move( creator ) )
    {
    }

    CDynamicMesh::~CDynamicMesh() = default;

    void CDynamicMesh::setName( const String &name )
    {
        m_name = name;
    }

    auto CDynamicMesh::getName() const -> String
    {
        return m_name;
    }

    auto CDynamicMesh::getId() const -> u32
    {
        return 0;
    }

    void CDynamicMesh::setMaterialName( const String &materialName, s32 index )
    {
        // m_dynamicMesh->setMaterial(materialName.c_str());
    }

    auto CDynamicMesh::getMaterialName( s32 index ) const -> String
    {
        return StringUtil::EmptyString;
    }

    void CDynamicMesh::setCastShadows( bool castShadows )
    {
    }

    auto CDynamicMesh::getCastShadows() const -> bool
    {
        return false;
    }

    void CDynamicMesh::setRecieveShadows( bool recieveShadows )
    {
    }

    auto CDynamicMesh::getRecieveShadows() const -> bool
    {
        return false;
    }

    void CDynamicMesh::setVisible( bool isVisible )
    {
        m_dynamicMesh->setVisible( isVisible );
    }

    auto CDynamicMesh::isVisible() const -> bool
    {
        return m_dynamicMesh->isVisible();
    }

    void CDynamicMesh::setRenderQueueGroup( u8 renderQueue )
    {
        m_dynamicMesh->setRenderQueueGroup( renderQueue );
    }

    void CDynamicMesh::setVisibilityFlags( u32 flags )
    {
        m_dynamicMesh->setVisibilityFlags( flags );
    }

    auto CDynamicMesh::getVisibilityFlags() const -> u32
    {
        return m_dynamicMesh->getVisibilityFlags();
    }

    auto CDynamicMesh::clone( const String &name ) const -> SmartPtr<IGraphicsObject>
    {
        auto dynamicMesh = SmartPtr<CDynamicMesh>( new CDynamicMesh( m_creator ) );
        SmartPtr<ISubMesh> subMesh = m_dynamicMesh->getSubMesh();
        dynamicMesh->setMesh( subMesh );
        dynamicMesh->setDirty( true );
        return dynamicMesh;
    }

    void CDynamicMesh::_getObject( void **ppObject ) const
    {
        *ppObject = m_dynamicMesh;
    }

    //
    // IDynamicMesh functions
    //

    void CDynamicMesh::setSubMesh( SmartPtr<ISubMesh> subMesh )
    {
        m_dynamicMesh->setMesh( subMesh );
    }

    void CDynamicMesh::setMesh( SmartPtr<IMesh> mesh )
    {
    }

    auto CDynamicMesh::getMesh() const -> SmartPtr<IMesh>
    {
        return nullptr;
    }

    auto CDynamicMesh::getSubMesh() const -> SmartPtr<ISubMesh>
    {
        return m_dynamicMesh->getSubMesh();
    }

    void CDynamicMesh::setDirty( bool dirty )
    {
        m_dynamicMesh->setDirty();
    }

    void CDynamicMesh::setOwner( SmartPtr<ISceneNode> owner )
    {
        m_owner = owner;

        Ogre::SceneNode *sceneNode = nullptr;
        m_owner->_getObject( reinterpret_cast<void **>( &sceneNode ) );

        if( sceneNode )
        {
            m_dynamicMesh->setOwner( sceneNode );
        }
    }
}  // namespace fb::render
