#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CDynamicMesh.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CDynamicMesh, CGraphicsObjectOgre<IDynamicMesh> );

        CDynamicMesh::CDynamicMesh( SmartPtr<IGraphicsScene> creator )
        {
        }

        CDynamicMesh::~CDynamicMesh()
        {
        }

        void CDynamicMesh::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDynamicMesh::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IGraphicsObject> CDynamicMesh::clone( const String &name ) const
        {
            auto dynamicMesh = fb::make_ptr<CDynamicMesh>();

            auto subMesh = m_dynamicMesh->getSubMesh();
            dynamicMesh->setSubMesh( subMesh );
            dynamicMesh->setDirty( true );
            return dynamicMesh;
        }

        void CDynamicMesh::_getObject( void **ppObject ) const
        {
            *ppObject = m_dynamicMesh;
        }

        void CDynamicMesh::setMesh( SmartPtr<IMesh> mesh )
        {
            if( mesh )
            {
                auto subMeshes = mesh->getSubMeshes();
                m_subMeshes;
            }
        }

        SmartPtr<IMesh> CDynamicMesh::getMesh() const
        {
            return nullptr;
        }

        void CDynamicMesh::setSubMesh( SmartPtr<ISubMesh> subMesh )
        {
            m_dynamicMesh->setMesh( subMesh );
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

            if( sceneNode )
            {
                m_dynamicMesh->setOwner( sceneNode );
            }
        }
    }  // end namespace render
}  // end namespace fb
