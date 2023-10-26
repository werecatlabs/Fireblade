#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CDynamicMesh.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/DynamicMesh.h>
#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Interface/Mesh/ISubMesh.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        CDynamicMesh::CDynamicMesh( SmartPtr<IGraphicsScene> creator ) :
            m_creator( creator )
        {
        }

        CDynamicMesh::~CDynamicMesh()
        {
        }

        void CDynamicMesh::setName( const String &name )
        {
            m_name = name;
        }

        String CDynamicMesh::getName() const
        {
            return m_name;
        }

        u32 CDynamicMesh::getId() const
        {
            return 0;
        }

        void CDynamicMesh::setMaterialName( const String &materialName, s32 index )
        {
            // m_dynamicMesh->setMaterial(materialName.c_str());
        }

        String CDynamicMesh::getMaterialName( s32 index ) const
        {
            return StringUtil::EmptyString;
        }

        void CDynamicMesh::setCastShadows( bool castShadows )
        {
        }

        bool CDynamicMesh::getCastShadows() const
        {
            return false;
        }

        void CDynamicMesh::setRecieveShadows( bool recieveShadows )
        {
        }

        bool CDynamicMesh::getRecieveShadows() const
        {
            return false;
        }

        void CDynamicMesh::setVisible( bool isVisible )
        {
            m_dynamicMesh->setVisible( isVisible );
        }

        bool CDynamicMesh::isVisible() const
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

        u32 CDynamicMesh::getVisibilityFlags() const
        {
            return m_dynamicMesh->getVisibilityFlags();
        }

        SmartPtr<IGraphicsObject> CDynamicMesh::clone( const String &name ) const
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

        SmartPtr<IMesh> CDynamicMesh::getMesh() const
        {
            return nullptr;
        }

        SmartPtr<ISubMesh> CDynamicMesh::getSubMesh() const
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
            m_owner->_getObject( (void **)&sceneNode );

            if(sceneNode)
            {
                m_dynamicMesh->setOwner( sceneNode );
            }
        }
    } // end namespace render
}     // end namespace fb
