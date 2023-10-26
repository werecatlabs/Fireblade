#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

namespace fb
{
    namespace render
    {
        u32 CSceneNodeOgreNext::m_nameExt = 0;

        FB_CLASS_REGISTER_DERIVED( fb, CSceneNodeOgreNext, ISceneNode );

        CSceneNodeOgreNext::CSceneNodeOgreNext() :
            m_sceneNode( nullptr ),

            m_flags( 0 ),
            m_lastUpdate( 0 ),
            m_transformUpdate( 0 ),

            m_isCulled( true )
        {
            m_nodeListener = nullptr;

            static const auto SceneNodeStr = String( "SceneNode" );
            auto name = SceneNodeStr + StringUtil::toString( m_nameExt++ );

            auto handle = getHandle();
            FB_ASSERT( handle );

            handle->setName( name );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            FB_ASSERT( stateManager );
            FB_ASSERT( graphicsSystem );
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();
            FB_ASSERT( stateObject );

            setStateObject( stateObject );

            auto stateListener = factoryManager->make_ptr<SceneNodeStateListener>();
            stateListener->setOwner( this );
            m_stateListener = stateListener;
            stateObject->addStateListener( m_stateListener );

            auto sceneNodeState = factoryManager->make_ptr<SceneNodeState>();
            //sceneNodeState->setSceneNode( this );
            stateObject->setState( sceneNodeState );
            stateObject->setOwner( this );

            m_state = sceneNodeState;

            auto renderTask = graphicsSystem->getStateTask();
            sceneNodeState->setTaskId( renderTask );
        }

        CSceneNodeOgreNext::CSceneNodeOgreNext( SmartPtr<IGraphicsScene> creator ) :
            m_sceneNode( nullptr ),

            m_flags( 0 ),
            m_lastUpdate( 0 ),
            m_transformUpdate( 0 ),

            m_isCulled( true )
        {
            m_nodeListener = nullptr;

            m_creator = creator;

            static const auto SceneNodeStr = String( "SceneNode" );
            auto name = SceneNodeStr + StringUtil::toString( m_nameExt++ );

            auto handle = getHandle();
            handle->setName( name );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();
            FB_ASSERT( stateObject );

            setStateObject( stateObject );

            auto sceneNodeStateListener = factoryManager->make_ptr<SceneNodeStateListener>();
            sceneNodeStateListener->setOwner( this );
            m_stateListener = sceneNodeStateListener;
            stateObject->addStateListener( m_stateListener );

            auto sceneNodeState = factoryManager->make_ptr<SceneNodeState>();
            //sceneNodeState->setSceneNode( this );
            stateObject->setState( sceneNodeState );
            stateObject->setOwner( this );

            m_state = sceneNodeState;

            auto renderTask = Thread::Task::Render;
            sceneNodeState->setTaskId( renderTask );
        }

        CSceneNodeOgreNext::~CSceneNodeOgreNext()
        {
            unload( nullptr );
        }

        void CSceneNodeOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
#if _DEBUG
                auto task = Thread::getCurrentTask();
                auto renderTask =
                    core::IApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
                FB_ASSERT( task == renderTask );
#endif

                setLoadingState( LoadingState::Loading );

                Ogre::SceneManager *smgr = nullptr;

                if( auto creator = getCreator() )
                {
                    creator->_getObject( (void **)&smgr );
                }

                auto handle = getHandle();
                FB_ASSERT( handle );

                auto name = handle->getName();
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                m_sceneNode = smgr->createSceneNode();

                // auto parent = getParent();
                // if (parent)
                //{
                //	Ogre::SceneNode* parentSceneNode = nullptr;
                //	parent->_getObject((void**)&parentSceneNode);

                //	if (parentSceneNode)
                //	{
                //		parentSceneNode->addChild(m_sceneNode);
                //	}
                //}

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto stateManager = applicationManager->getStateManager() )
                {
                    auto stateObject = getStateObject();
                    if( stateObject )
                    {
                        auto stateListener = getStateListener();
                        if( stateListener )
                        {
                            stateObject->removeStateListener( stateListener );
                        }

                        stateManager->removeStateObject( stateObject );

                        stateObject->unload( nullptr );
                        setStateObject( nullptr );
                    }

                    auto stateListener = getStateListener();
                    if( stateListener )
                    {
                        stateListener->unload( nullptr );
                        setStateListener( nullptr );
                    }

                    if( auto state = getState() )
                    {
                        state->setOwner( nullptr );
                        state->unload( nullptr );
                        state = nullptr;
                    }
                }

                // remove all the children
                removeAllChildren();

                if( auto pParent = getParent() )
                {
                    auto parent = fb::static_pointer_cast<CSceneNodeOgreNext>( pParent );
                    if( parent )
                    {
                        parent->removeChild( this );
                    }
                }

                if( m_nodeListener )
                {
                    if( m_sceneNode )
                    {
                        m_sceneNode->setListener( nullptr );
                    }

                    delete m_nodeListener;
                    m_nodeListener = nullptr;
                }

                SmartPtr<CSceneManagerOgreNext> smgr = getCreator();
                if( smgr )
                {
                    // remove from scene manager map
                    smgr->removeSceneNode( this );

                    // remove ogre scene node
                    if( m_sceneNode != nullptr )
                    {
                        m_sceneNode->detachAllObjects();
                        m_graphicsObjects.clear();

                        auto ogreSmgr = smgr->getSceneManager();
                        FB_ASSERT( ogreSmgr );

                        ogreSmgr->destroySceneNode( m_sceneNode );
                        m_sceneNode = nullptr;
                    }
                }
                else
                {
                    m_sceneNode = nullptr;
                }

                CSceneNode::unload( data );
                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgreNext::initialise( Ogre::SceneNode *sceneNode )
        {
            m_sceneNode = sceneNode;

            // m_nodeListener = new NodeListener(this);
            // m_sceneNode->setListener(m_nodeListener);

            setLoadingState( LoadingState::Loaded );
        }

        void CSceneNodeOgreNext::destroy()
        {
            // remove ogre scene node
            if( m_sceneNode != nullptr )
            {
                // remove all the objects from the scene node
                detachAllObjects();

                Ogre::SceneManager *ogreSmgr = nullptr;

                if( auto creator = getCreator() )
                {
                    creator->_getObject( (void **)&ogreSmgr );
                }

                ogreSmgr->destroySceneNode( m_sceneNode );
                m_sceneNode = nullptr;
            }
        }

        void CSceneNodeOgreNext::setMaterialName( const String &materialName, bool cascade )
        {
            for( u32 i = 0; i < m_graphicsObjects.size(); ++i )
            {
                // m_graphicsObjects[i]->setMaterialName(materialName);
            }

            if( cascade )
            {
                for( u32 i = 0; i < m_children.size(); ++i )
                {
                    SmartPtr<CSceneNodeOgreNext> child = m_children[i];
                    child->setMaterialName( materialName );
                }
            }
        }

        void CSceneNodeOgreNext::setFixedYawAxis( bool useFixed, const Vector3F &fixedAxis )
        {
            if( m_sceneNode )
            {
                Ogre::Vector3 axis( fixedAxis.X(), fixedAxis.Y(), fixedAxis.Z() );
                m_sceneNode->setFixedYawAxis( useFixed, axis );
            }
        }

        AABB3F CSceneNodeOgreNext::getLocalAABB() const
        {
            return calculateAABB();
        }

        AABB3F CSceneNodeOgreNext::getWorldAABB() const
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto graphicsSystem = applicationManager->getGraphicsSystem();
            // auto factoryManager = applicationManager->getFactoryManager();
            // auto renderTask = graphicsSystem->getRenderTask();

            // SmartPtr<SceneNodeState> state = m_stateObject->getState();
            // if (state)
            //{
            //	return state->getWorldAABB();
            // }

            Ogre::AxisAlignedBox box;

            // auto currentTaskId = Thread::getCurrentTask();
            // if (currentTaskId != renderTask)
            //{
            //
            //	m_sceneNode->_update(true, true);
            //	box = m_sceneNode->_getWorldAABB();
            // }
            // else
            //{
            //	m_sceneNode->_update(true, true);
            //	box = m_sceneNode->_getWorldAABB();
            // }

            const Ogre::Vector3 &minPoint = box.getMinimum();
            const Ogre::Vector3 &maxPoint = box.getMaximum();

            return AABB3F( minPoint.x, minPoint.y, minPoint.z, maxPoint.x, maxPoint.y, maxPoint.z );
        }

        void CSceneNodeOgreNext::setVisible( bool visible, bool cascade /*= true*/ )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto renderTask = graphicsSystem->getRenderTask();
            const auto stateTask = graphicsSystem->getStateTask();
            const auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                if( m_sceneNode )
                {
                    m_sceneNode->setVisible( visible, cascade );
                }
            }
            else
            {
                auto stateMessageVisible = factoryManager->make_ptr<StateMessageVisible>();
                stateMessageVisible->setVisible( visible );
                stateMessageVisible->setCascade( cascade );

                auto stateObject = getStateObject();
                stateObject->addMessage( stateTask, stateMessageVisible );
            }
        }

        bool CSceneNodeOgreNext::isVisible() const
        {
            return true;
        }

        void CSceneNodeOgreNext::setCulled( bool culled )
        {
            m_isCulled = culled;

            // m_sceneNode->setShouldCull(m_isCulled);
        }

        bool CSceneNodeOgreNext::isCulled() const
        {
            return m_isCulled;
        }

        void CSceneNodeOgreNext::attachObject( SmartPtr<IGraphicsObject> object )
        {
            FB_ASSERT( object );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto renderTask = graphicsSystem->getRenderTask();
            const auto stateTask = graphicsSystem->getStateTask();
            const auto task = Thread::getCurrentTask();

            if( isLoaded() && task == renderTask )
            {
                FB_ASSERT( m_sceneNode );

                const auto &objectLoadingState = object->getLoadingState();
                if( objectLoadingState == LoadingState::Loaded )
                {
                    ISharedObject::ScopedLock lock( graphicsSystem );

                    if( object->isDerived<ICamera>() )
                    {
                        auto camera = fb::static_pointer_cast<CCameraOgreNext>( object );

                        Ogre::MovableObject *moveable = nullptr;
                        object->_getObject( (void **)&moveable );

                        if( moveable )
                        {
                            if( !moveable->isAttached() )
                            {
                                m_sceneNode->attachObject( moveable );
                            }
                            else
                            {
                                moveable->detachFromParent();
                                m_sceneNode->attachObject( moveable );
                            }
                        }

                        object->setOwner( this );
                        m_graphicsObjects.push_back( object );
                    }
                    else
                    {
                        Ogre::MovableObject *moveable = nullptr;
                        object->_getObject( (void **)&moveable );

                        if( moveable )
                        {
                            if( !moveable->isAttached() )
                            {
                                m_sceneNode->attachObject( moveable );
                            }
                            else
                            {
                                FB_LOG_ERROR( "CSceneNodeOgre::attachObject Object already attached." );
                            }
                        }

                        object->setOwner( this );
                        m_graphicsObjects.push_back( object );
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_ATTACH_OBJECT );
                    message->setObject( object );

                    if( auto stateObject = getStateObject() )
                    {
                        stateObject->addMessage( stateTask, message );
                    }
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_ATTACH_OBJECT );
                message->setObject( object );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( stateTask, message );
                }
            }
        }

        void CSceneNodeOgreNext::detachObject( SmartPtr<IGraphicsObject> object )
        {
            FB_ASSERT( object );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto renderTask = graphicsSystem->getRenderTask();
            auto stateTask = graphicsSystem->getStateTask();
            auto task = Thread::getCurrentTask();

            if( isLoaded() && task == renderTask )
            {
                ISharedObject::ScopedLock lock( graphicsSystem );

                Ogre::MovableObject *moveable = nullptr;
                object->_getObject( (void **)&moveable );

                if( moveable )
                {
                    if( auto parent = moveable->getParentSceneNode() )
                    {
                        if( moveable )
                        {
                            parent->detachObject( moveable );
                        }
                    }
                }

                auto it = std::find( m_graphicsObjects.begin(), m_graphicsObjects.end(), object );
                if( it != m_graphicsObjects.end() )
                {
                    m_graphicsObjects.erase( it );
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_DETACH_OBJECT );
                message->setObject( object );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( stateTask, message );
                }
            }
        }

        void CSceneNodeOgreNext::detachAllObjects()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto renderTask = graphicsSystem->getRenderTask();
            const auto stateTask = graphicsSystem->getStateTask();
            const auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                m_sceneNode->detachAllObjects();
                m_graphicsObjects.clear();
            }
            else
            {
                auto stateMessage = factoryManager->make_ptr<StateMessageType>();
                stateMessage->setType( STATE_MESSAGE_DETACH_ALL_OBJECTS );

                auto stateObject = getStateObject();
                if( stateObject )
                {
                    stateObject->addMessage( stateTask, stateMessage );
                }
            }
        }

        Array<SmartPtr<IGraphicsObject>> CSceneNodeOgreNext::getObjects() const
        {
            return m_graphicsObjects;
        }

        SmartPtr<ISceneNode> CSceneNodeOgreNext::addChildSceneNode( const String &name )
        {
            SmartPtr<ISceneNode> sceneNode;

            if( auto creator = getCreator() )
            {
                if( StringUtil::isNullOrEmpty( name ) )
                {
                    sceneNode = creator->addSceneNode();
                }
                else
                {
                    sceneNode = creator->addSceneNode( name );
                }
            }

            addChild( sceneNode );

            return sceneNode;
        }

        SmartPtr<ISceneNode> CSceneNodeOgreNext::addChildSceneNode( const Vector3F &position )
        {
            if( auto creator = getCreator() )
            {
                auto sceneNode = creator->addSceneNode();
                addChild( sceneNode );
                sceneNode->setPosition( position );
                return sceneNode;
            }

            return nullptr;
        }

        u32 CSceneNodeOgreNext::getNumObjects() const
        {
            return static_cast<u32>( m_graphicsObjects.size() );
        }

        void CSceneNodeOgreNext::addChild( SmartPtr<ISceneNode> child )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto renderTask = graphicsSystem->getRenderTask();
            const auto stateTask = graphicsSystem->getStateTask();
            const auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            const auto &childLoadingState = child->getLoadingState();

            if( loadingState == LoadingState::Loaded && childLoadingState == LoadingState::Loaded &&
                task == renderTask )
            {
                const auto &childLoadingState = child->getLoadingState();
                if( childLoadingState == LoadingState::Loaded )
                {
                    auto parent = child->getParent();
                    if( parent )
                    {
                        child->remove();  // remove from old parent
                    }
                }

                auto pThis = getSharedFromThis<ISceneNode>();
                child->setParent( pThis );

                m_children.push_back( child );

                Ogre::SceneNode *sceneNode = nullptr;
                child->_getObject( (void **)&sceneNode );

                if( m_sceneNode )
                {
                    if( sceneNode )
                    {
                        m_sceneNode->addChild( sceneNode );
                    }
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_ADD_CHILD );
                message->setObject( child );

                auto stateObject = getStateObject();
                if( stateObject )
                {
                    stateObject->addMessage( stateTask, message );
                }
            }
        }

        bool CSceneNodeOgreNext::removeChild( SmartPtr<ISceneNode> child )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto renderTask = graphicsSystem->getRenderTask();
            const auto stateTask = graphicsSystem->getStateTask();
            const auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloading && loadingState != LoadingState::Unloaded )
            {
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    const auto &childLoadingState = child->getLoadingState();
                    if( childLoadingState == LoadingState::Loaded )
                    {
                        auto it = std::find( m_children.begin(), m_children.end(), child );
                        if( it != m_children.end() )
                        {
                            m_children.erase( it );

                            child->setParent( nullptr );

                            Ogre::SceneNode *sceneNode = nullptr;
                            child->_getObject( (void **)&sceneNode );

                            if( sceneNode )
                            {
                                m_sceneNode->removeChild( sceneNode );
                            }

                            return true;
                        }

                        return false;
                    }
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_REMOVE_CHILD );
                    message->setObject( child );

                    auto stateObject = getStateObject();
                    stateObject->addMessage( stateTask, message );
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_REMOVE_CHILD );
                    message->setObject( child );

                    auto stateObject = getStateObject();
                    if( stateObject )
                    {
                        stateObject->addMessage( stateTask, message );
                    }
                }

                return true;
            }

            return false;
        }

        void CSceneNodeOgreNext::add()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto renderTask = graphicsSystem->getRenderTask();
            const auto stateTask = graphicsSystem->getStateTask();
            const auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                // if (!m_sceneNode->isInSceneGraph())
                if( auto creator = getCreator() )
                {
                    creator->getRootSceneNode()->addChild( this );
                }
            }
            else
            {
                auto stateMessage = factoryManager->make_ptr<StateMessageType>();
                stateMessage->setType( STATE_MESSAGE_ADD );

                auto stateObject = getStateObject();
                stateObject->addMessage( stateTask, stateMessage );
            }
        }

        void CSceneNodeOgreNext::remove()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                const auto renderTask = graphicsSystem->getRenderTask();
                const auto stateTask = graphicsSystem->getStateTask();
                const auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    auto parent = getParent();
                    if( parent )
                    {
                        auto parentSceneNode = fb::static_pointer_cast<CSceneNodeOgreNext>( parent );
                        parentSceneNode->removeChild( this );
                    }
                }
                else
                {
                    auto stateMessage = factoryManager->make_ptr<StateMessageType>();
                    stateMessage->setType( STATE_MESSAGE_REMOVE );

                    auto stateObject = getStateObject();
                    stateObject->addMessage( stateTask, stateMessage );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgreNext::removeAllChildren()
        {
            try
            {
                if( auto pParent = getParent() )
                {
                    for( auto child : m_children )
                    {
                        auto parent = fb::static_pointer_cast<CSceneNodeOgreNext>( pParent );
                        parent->removeChild( this );
                    }
                }

                m_children.clear();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<ISceneNode> CSceneNodeOgreNext::findChild( const String &name )
        {
            Array<SmartPtr<ISceneNode>> children;
            getChildren( children );

            for( size_t i = 0; i < children.size(); ++i )
            {
                auto &sceneNode = children[i];
                const auto handle = sceneNode->getHandle();
                const auto sceneNodeName = handle->getName();

                if( name == sceneNodeName )
                {
                    return sceneNode;
                }
            }

            return nullptr;
        }

        void CSceneNodeOgreNext::getChildren( Array<SmartPtr<ISceneNode>> &children ) const
        {
            auto numChildren = m_children.size();
            children.reserve( numChildren );

            for( u32 i = 0; i < numChildren; ++i )
            {
                SmartPtr<ISceneNode> sceneNode( m_children[i] );
                children.push_back( sceneNode );
            }
        }

        Array<SmartPtr<ISceneNode>> CSceneNodeOgreNext::getChildren() const
        {
            return m_children;
        }

        u32 CSceneNodeOgreNext::getNumChildren() const
        {
            return static_cast<u32>( m_children.size() );
        }

        void CSceneNodeOgreNext::needUpdate( bool forceParentUpdate )
        {
            //
            // m_sceneNode->needUpdate(forceParentUpdate);
        }

        SmartPtr<ISceneNode> CSceneNodeOgreNext::clone( SmartPtr<ISceneNode> parent,
                                                        const String &name ) const
        {
            SmartPtr<CSceneNodeOgreNext> sceneNode = parent->addChildSceneNode();

            // set properties
            sceneNode->setPosition( getPosition() );
            sceneNode->setOrientation( getOrientation() );
            sceneNode->setScale( getScale() );

            // clone attached objects
            for( u32 i = 0; i < m_graphicsObjects.size(); ++i )
            {
                const SmartPtr<IGraphicsObject> &graphicsObject = m_graphicsObjects[i];

                SmartPtr<IGraphicsObject> cloneGraphicsObject = graphicsObject->clone();
                sceneNode->attachObject( cloneGraphicsObject );
            }

            return sceneNode;
        }

        void CSceneNodeOgreNext::showBoundingBox( bool show )
        {
            //
            // return m_sceneNode->showBoundingBox(show);
        }

        bool CSceneNodeOgreNext::getShowBoundingBox() const
        {
            return false;  // m_sceneNode->getShowBoundingBox();
        }

        void CSceneNodeOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = m_sceneNode;
        }

        void CSceneNodeOgreNext::setTransformationDirty( u32 msgType )
        {
        }

        Ogre::SceneNode *CSceneNodeOgreNext::getSceneNode() const
        {
            return m_sceneNode;
        }

        void CSceneNodeOgreNext::_updateBoundingBox()
        {
        }

        void CSceneNodeOgreNext::updateBounds()
        {
        }

        void CSceneNodeOgreNext::setVisibilityFlags( u32 flags )
        {
            m_visibilityFlags = flags;

            for( u32 i = 0; i < m_graphicsObjects.size(); ++i )
            {
                SmartPtr<IGraphicsObject> graphicsObject = m_graphicsObjects[i];
                graphicsObject->setVisibilityFlags( flags );
            }

            for( u32 i = 0; i < m_children.size(); ++i )
            {
                m_children[i]->setVisibilityFlags( flags );
            }
        }

        u32 CSceneNodeOgreNext::getVisibilityFlags() const
        {
            return m_visibilityFlags;
        }

        void CSceneNodeOgreNext::setFlag( u32 flag, bool value )
        {
            m_flags = BitUtil::setFlagValue( static_cast<u32>( m_flags ), flag, value );
        }

        bool CSceneNodeOgreNext::getFlag( u32 flag ) const
        {
            return BitUtil::getFlagValue( static_cast<u32>( m_flags ), flag );
        }

        SmartPtr<Properties> CSceneNodeOgreNext::getProperties() const
        {
            auto properties = CSceneNode::getProperties();

            auto name = String( "null" );
            auto handle = getHandle();
            if( handle )
            {
                name = handle->getName();
            }

            properties->setProperty( "name", name );

            auto iNumAttachedObjects = getNumObjects();
            properties->setProperty( "NumObjects", iNumAttachedObjects );

            if( m_sceneNode )
            {
                auto sceneNodePosition = m_sceneNode->getPosition();
                auto sceneNodeScale = m_sceneNode->getScale();
                auto sceneNodeOrientation = OgreUtil::convert( m_sceneNode->getOrientation() );

                Vector3<real_Num> localRotation;
                //sceneNodeOrientation.toDegrees( localRotation );

                //sceneNodeOrientation.normalise().toDegrees( localRotation );

                Vector3<real_Num> localRotation2;
                //m_state->getOrientation().toDegrees( localRotation2 );

                properties->setProperty( "sceneNodePosition",
                                         Ogre::StringConverter::toString( sceneNodePosition ) );
                properties->setProperty( "sceneNodeScale",
                                         Ogre::StringConverter::toString( sceneNodeScale ) );
                properties->setProperty( "sceneNodeOrientation", localRotation );

                properties->setProperty( "stateOrientation", localRotation2 );
            }

            return properties;
        }

        void CSceneNodeOgreNext::SceneNodeStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            FB_ASSERT( message );
            if( auto owner = getOwner() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto factoryManager = applicationManager->getFactoryManager();
                auto renderTask = graphicsSystem->getRenderTask();
                auto currentTaskId = Thread::getCurrentTask();
                FB_ASSERT( currentTaskId == renderTask );

                ISharedObject::ScopedLock lock( graphicsSystem );

                if( currentTaskId == renderTask )
                {
                    if( !message )
                    {
                        return;
                    }

                    if( message->isExactly<StateMessageVector3>() )
                    {
                        auto positionMessage = fb::static_pointer_cast<StateMessageVector3>( message );
                        auto value = positionMessage->getValue();
                        auto type = message->getType();

                        if( type == STATE_MESSAGE_POSITION )
                        {
                            owner->setPosition( value );
                        }
                        else if( type == STATE_MESSAGE_SCALE )
                        {
                            owner->setScale( value );
                        }
                        else if( type == STATE_MESSAGE_LOOK_AT )
                        {
                            owner->lookAt( value );
                        }
                    }
                    else if( message->isExactly<StateMessageOrientation>() )
                    {
                        auto orientationMessage =
                            fb::static_pointer_cast<StateMessageOrientation>( message );
                        owner->setOrientation( orientationMessage->getOrientation() );
                    }
                    else if( message->isExactly<StateMessageObject>() )
                    {
                        auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                        auto object = objectMessage->getObject();
                        auto type = objectMessage->getType();
                        FB_ASSERT( objectMessage );

                        if( type == STATE_MESSAGE_ADD_CHILD )
                        {
                            owner->addChild( object );
                        }
                        else if( type == STATE_MESSAGE_REMOVE_CHILD )
                        {
                            owner->removeChild( object );
                        }
                        else if( type == STATE_MESSAGE_ATTACH_OBJECT )
                        {
                            owner->attachObject( object );
                        }
                        else if( type == STATE_MESSAGE_DETACH_OBJECT )
                        {
                            owner->detachObject( object );
                        }
                        else if( type == STATE_MESSAGE_DETACH_ALL_OBJECTS )
                        {
                            owner->detachAllObjects();
                        }
                        else
                        {
                            FB_ASSERT( false );
                        }
                    }
                    else if( message->isExactly<StateMessageType>() )
                    {
                        auto stateMessage = fb::static_pointer_cast<StateMessageType>( message );
                        if( stateMessage->getType() == STATE_MESSAGE_ADD )
                        {
                            owner->add();
                        }
                        else if( stateMessage->getType() == STATE_MESSAGE_REMOVE )
                        {
                            owner->remove();
                        }
                    }
                    else if( message->isExactly<StateMessageVisible>() )
                    {
                        auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                        FB_ASSERT( visibleMessage );

                        auto visible = visibleMessage->isVisible();
                        auto cascade = visibleMessage->getCascade();

                        owner->setVisible( visible, cascade );
                    }
                }
            }
        }

        void CSceneNodeOgreNext::SceneNodeStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ISharedObject::ScopedLock lock( graphicsSystem );

            if( auto owner = getOwner() )
            {
                Ogre::SceneNode *sceneNode = nullptr;
                owner->_getObject( (void **)&sceneNode );

                if( sceneNode )
                {
                    auto sceneNodeState = fb::static_pointer_cast<SceneNodeState>( state );
                    if( sceneNodeState )
                    {
                        auto position = sceneNodeState->getPosition();
                        auto scale = sceneNodeState->getScale();
                        auto orientation = sceneNodeState->getOrientation();

                        auto pos = OgreUtil::convertToOgreVector( position );
                        // if (!OgreUtil::equals(pos, sceneNode->getPosition()))
                        {
                            sceneNode->setPosition( pos );
                        }

                        auto orient = OgreUtil::convertToOgreQuaternion( orientation );
                        // if (!OgreUtil::equals(orient, sceneNode->getOrientation()))
                        {
                            orient.normalise();
                            sceneNode->setOrientation( orient );
                        }

                        auto vScale = OgreUtil::convertToOgreVector( scale );
                        // if (!OgreUtil::equals(vScale, sceneNode->getScale()))
                        {
                            sceneNode->setScale( vScale );
                        }

                        // sceneNode->_update( true, false );
                        state->setDirty( false );
                    }
                }
            }
        }

        void CSceneNodeOgreNext::SceneNodeStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
            // if(query->isExactly(StateQueryAABB3F::TYPE_INFO))
            //{
            //	u32 type = query->getType();
            //	if(type == ISceneNode::STATE_QUERY_TYPE_LOCAL_AABB)
            //	{
            //		Ogre::SceneNode* sceneNode = m_owner->m_sceneNode;
            //		Ogre::AxisAlignedBox localBox = sceneNode->_getWorldAABB();

            //		Ogre::Matrix4 worldTransform = sceneNode->_getFullTransform();
            //		Ogre::Matrix4 invWorldTransform = worldTransform.inverse();
            //		localBox.transform(invWorldTransform);

            //		Ogre::Vector3 minPoint = localBox.getMinimum();
            //		Ogre::Vector3 maxPoint = localBox.getMaximum();

            //		StateQueryAABB3FPtr aabbQuery = query;
            //		aabbQuery->setAABB(AABB3F(Vector3F(minPoint.x, minPoint.y, minPoint.z),
            //			Vector3F(maxPoint.x, maxPoint.y, maxPoint.z)));

            //		query->setResultReady(true);
            //	}
            //	else if(type == ISceneNode::STATE_QUERY_TYPE_WORLD_AABB)
            //	{
            //		Ogre::SceneNode* sceneNode = m_owner->m_sceneNode;
            //		sceneNode->_update(true, true);
            //		const Ogre::AxisAlignedBox& worldBox = sceneNode->_getWorldAABB();
            //		const Ogre::Vector3& minPoint = worldBox.getMinimum();
            //		const Ogre::Vector3& maxPoint = worldBox.getMaximum();

            //		StateQueryAABB3FPtr aabbQuery = query;
            //		aabbQuery->setAABB(AABB3F(Vector3F(minPoint.x, minPoint.y, minPoint.z),
            //			Vector3F(maxPoint.x, maxPoint.y, maxPoint.z)));

            //		query->setResultReady(true);
            //	}
            //}
        }

        SmartPtr<CSceneNodeOgreNext> CSceneNodeOgreNext::SceneNodeStateListener::getOwner() const
        {
            return m_owner;
        }

        void CSceneNodeOgreNext::SceneNodeStateListener::setOwner( SmartPtr<CSceneNodeOgreNext> owner )
        {
            m_owner = owner;
        }

        CSceneNodeOgreNext::SceneNodeStateListener::~SceneNodeStateListener()
        {
            m_owner = nullptr;
        }

        AABB3F CSceneNodeOgreNext::calculateAABB() const
        {
            Ogre::AxisAlignedBox box;

            // for (u32 i = 0; i < m_sceneNode->numAttachedObjects(); ++i)
            //{
            //	Ogre::MovableObject* object = m_sceneNode->getAttachedObject(i);
            //	if (object->getTypeFlags() & Ogre::SceneManager::ENTITY_TYPE_MASK)
            //	{
            //		Ogre::Entity* entity = (Ogre::Entity*)object;
            //		box.merge(entity->getBoundingBox());
            //	}
            // }

            const Ogre::Vector3 &minPoint = box.getMinimum();
            const Ogre::Vector3 &maxPoint = box.getMaximum();

            return AABB3F( minPoint.x, minPoint.y, minPoint.z, maxPoint.x, maxPoint.y, maxPoint.z );
        }

        bool CSceneNodeOgreNext::_getFlag( u32 flag ) const
        {
            return ( m_flags & flag ) != 0;
        }

        void *CSceneNodeOgreNext::_getRenderSystemTransform() const
        {
            return nullptr;
        }

        u32 CSceneNodeOgreNext::getObjectsBuffer( SmartPtr<IGraphicsObject> *buffer,
                                                  u32 bufferSize ) const
        {
            return 0;
        }

        void CSceneNodeOgreNext::removeChildren()
        {
        }

        u32 CSceneNodeOgreNext::getChildrenBuffer( SmartPtr<ISceneNode> *children, u32 bufferSize ) const
        {
            return 0;
        }

        CSceneNodeOgreNext::NodeListener::NodeListener( CSceneNodeOgreNext *owner ) : m_owner( owner )
        {
        }

        CSceneNodeOgreNext::NodeListener::~NodeListener()
        {
        }

        void CSceneNodeOgreNext::NodeListener::nodeUpdated( const Ogre::Node *node )
        {
        }

        void CSceneNodeOgreNext::NodeListener::nodeDestroyed( const Ogre::Node *node )
        {
        }

        void CSceneNodeOgreNext::NodeListener::nodeAttached( const Ogre::Node *node )
        {
            Ogre::SceneManager *pSceneManager = nullptr;

            SmartPtr<IGraphicsScene> smgr = m_owner->getCreator();
            smgr->_getObject( (void **)&pSceneManager );

            auto sceneNode = (Ogre::SceneNode *)node;
            if( pSceneManager->getRootSceneNode() == sceneNode->getParentSceneNode() )
            {
                // m_owner->registerForUpdates(true, false);
            }
        }

        void CSceneNodeOgreNext::NodeListener::nodeDetached( const Ogre::Node *node )
        {
            Ogre::SceneManager *pSceneManager = nullptr;

            SmartPtr<IGraphicsScene> smgr = m_owner->getCreator();
            smgr->_getObject( (void **)&pSceneManager );

            auto sceneNode = (Ogre::SceneNode *)node;
            if( pSceneManager->getRootSceneNode() == sceneNode->getParentSceneNode() )
            {
                // m_owner->registerForUpdates(false, false);
            }
        }
    }  // end namespace render
}  // end namespace fb
