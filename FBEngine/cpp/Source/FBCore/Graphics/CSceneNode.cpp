#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CSceneNode.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        CSceneNode::CSceneNode()
        {
        }

        CSceneNode::~CSceneNode()
        {
            unload( nullptr );
        }

        void CSceneNode::load( SmartPtr<ISharedObject> data )
        {
        }

        void CSceneNode::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                m_creator = nullptr;
                m_parent = nullptr;

                auto applicationManager = core::IApplicationManager::instance();

                if( auto stateContext = getStateObject() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    if( auto stateManager = applicationManager->getStateManager() )
                    {
                        stateManager->removeStateObject( stateContext );
                    }

                    setStateObject( nullptr );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNode::detachObjectPtr( IGraphicsObject *object )
        {
        }

        void CSceneNode::setParent( SmartPtr<ISceneNode> parent )
        {
            m_parent = parent;
        }

        SmartPtr<ISceneNode> CSceneNode::getParent() const
        {
            auto p = m_parent.load();
            return p.lock();
        }

        SmartPtr<IGraphicsScene> CSceneNode::getCreator() const
        {
            auto p = m_creator.load();
            return p.lock();
        }

        void CSceneNode::setCreator( SmartPtr<IGraphicsScene> creator )
        {
            m_creator = creator;
        }

        void CSceneNode::setPosition( const Vector3<real_Num> &position )
        {
        }

        Vector3<real_Num> CSceneNode::getPosition() const
        {
            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> CSceneNode::getWorldPosition() const
        {
            return Vector3<real_Num>::zero();
        }

        void CSceneNode::setRotationFromDegrees( const Vector3<real_Num> &degrees )
        {
        }

        void CSceneNode::setOrientation( const Quaternion<real_Num> &orientation )
        {
        }

        Quaternion<real_Num> CSceneNode::getOrientation() const
        {
            return Quaternion<real_Num>::identity();
        }

        Quaternion<real_Num> CSceneNode::getWorldOrientation() const
        {
            return Quaternion<real_Num>::identity();
        }

        void CSceneNode::setScale( const Vector3<real_Num> &scale )
        {
        }

        Vector3<real_Num> CSceneNode::getScale() const
        {
            return Vector3<real_Num>::zero();
        }

        Vector3<real_Num> CSceneNode::getWorldScale() const
        {
            return Vector3<real_Num>::zero();
        }

        void *CSceneNode::_getRenderSystemTransform() const
        {
            return nullptr;
        }

        void CSceneNode::lookAt( const Vector3<real_Num> &targetPoint )
        {
        }

        void CSceneNode::setFixedYawAxis(
            bool useFixed, const Vector3<real_Num> &fixedAxis /*= Vector3<real_Num>::UNIT_Y */ )
        {
        }

        AABB3<real_Num> CSceneNode::getLocalAABB() const
        {
            return AABB3<real_Num>();
        }

        AABB3<real_Num> CSceneNode::getWorldAABB() const
        {
            return AABB3<real_Num>();
        }

        void CSceneNode::setVisible( bool isVisible, bool cascade /*= true */ )
        {
        }

        void CSceneNode::setCulled( bool culled )
        {
        }

        bool CSceneNode::isCulled() const
        {
            return false;
        }

        void CSceneNode::attachObject( SmartPtr<IGraphicsObject> object )
        {
        }

        void CSceneNode::detachObject( SmartPtr<IGraphicsObject> object )
        {
        }

        void CSceneNode::detachAllObjects()
        {
        }

        Array<SmartPtr<IGraphicsObject>> CSceneNode::getObjects() const
        {
            return Array<SmartPtr<IGraphicsObject>>();
        }

        u32 CSceneNode::getObjectsBuffer( SmartPtr<IGraphicsObject> *buffer, u32 bufferSize ) const
        {
            return 0;
        }

        u32 CSceneNode::getNumObjects() const
        {
            return 0;
        }

        SmartPtr<ISceneNode> CSceneNode::addChildSceneNode(
            const String &name /*= StringUtil::EmptyString */ )
        {
            return nullptr;
        }

        SmartPtr<ISceneNode> CSceneNode::addChildSceneNode( const Vector3<real_Num> &position )
        {
            return nullptr;
        }

        void CSceneNode::addChild( SmartPtr<ISceneNode> child )
        {
        }

        bool CSceneNode::removeChild( SmartPtr<ISceneNode> child )
        {
            return false;
        }

        void CSceneNode::add()
        {
        }

        void CSceneNode::remove()
        {
        }

        void CSceneNode::removeChildren()
        {
        }

        SmartPtr<ISceneNode> CSceneNode::findChild( const String &name )
        {
            return nullptr;
        }

        Array<SmartPtr<ISceneNode>> CSceneNode::getChildren() const
        {
            return Array<SmartPtr<ISceneNode>>();
        }

        u32 CSceneNode::getChildrenBuffer( SmartPtr<ISceneNode> *children, u32 bufferSize ) const
        {
            return 0;
        }

        u32 CSceneNode::getNumChildren() const
        {
            return 0;
        }

        void CSceneNode::needUpdate( bool forceParentUpdate /*= false */ )
        {
        }

        SmartPtr<ISceneNode> CSceneNode::clone( SmartPtr<ISceneNode> parent /*= nullptr*/,
                                                const String &name /*= StringUtil::EmptyString */ ) const
        {
            return nullptr;
        }

        void CSceneNode::showBoundingBox( bool show )
        {
        }

        bool CSceneNode::getShowBoundingBox() const
        {
            return false;
        }

        void CSceneNode::_updateBounds()
        {
        }

        void CSceneNode::setVisibilityFlags( u32 flags )
        {
        }

        void CSceneNode::_getObject( void **ppObject ) const
        {
        }

        void CSceneNode::setFlag( u32 flag, bool value )
        {
        }

        bool CSceneNode::getFlag( u32 flag ) const
        {
            return false;
        }

        SmartPtr<Properties> CSceneNode::getProperties() const
        {
            return nullptr;
        }

        void CSceneNode::setProperties( SmartPtr<Properties> properties )
        {
        }
    }  // namespace render
}  // namespace fb
