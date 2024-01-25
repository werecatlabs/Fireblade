#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

namespace fb::render
{
    u32 CSceneNodeOgreNext::m_nameExt = 0;

    FB_CLASS_REGISTER_DERIVED( fb, CSceneNodeOgreNext, SceneNode );

    CSceneNodeOgreNext::CSceneNodeOgreNext()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "CSceneNodeOgreNext" );
        }

        m_nodeListener = nullptr;

        static const auto SceneNodeStr = String( "SceneNode" );
        auto name = SceneNodeStr + StringUtil::toString( m_nameExt++ );

        auto handle = getHandle();
        FB_ASSERT( handle );

        handle->setName( name );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        FB_ASSERT( stateManager );
        FB_ASSERT( graphicsSystem );
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();
        FB_ASSERT( stateContext );

        stateContext->setOwner( this );

        setStateContext( stateContext );

        auto stateListener = factoryManager->make_ptr<SceneNodeStateListener>();
        stateListener->setOwner( this );
        m_stateListener = stateListener;
        stateContext->addStateListener( m_stateListener );

        auto transformState = factoryManager->make_ptr<TransformState>();
        stateContext->addState( transformState );

        auto sceneNodeState = factoryManager->make_ptr<SceneNodeState>();
        //sceneNodeState->setSceneNode( this );
        stateContext->addState( sceneNodeState );

        auto renderTask = graphicsSystem->getStateTask();
        sceneNodeState->setTaskId( renderTask );
        transformState->setTaskId( renderTask );

        constexpr auto size = sizeof( CSceneNodeOgreNext );
    }

    CSceneNodeOgreNext::CSceneNodeOgreNext( SmartPtr<IGraphicsScene> creator )
    {
        m_nodeListener = nullptr;

        m_creator = creator;

        static const auto SceneNodeStr = String( "SceneNode" );
        auto name = SceneNodeStr + StringUtil::toString( m_nameExt++ );

        auto handle = getHandle();
        handle->setName( name );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();
        FB_ASSERT( stateContext );

        stateContext->setOwner( this );

        setStateContext( stateContext );

        auto sceneNodeStateListener = factoryManager->make_ptr<SceneNodeStateListener>();
        sceneNodeStateListener->setOwner( this );
        m_stateListener = sceneNodeStateListener;
        stateContext->addStateListener( m_stateListener );

        auto transformState = factoryManager->make_ptr<TransformState>();
        stateContext->addState( transformState );

        auto sceneNodeState = factoryManager->make_ptr<SceneNodeState>();
        //sceneNodeState->setSceneNode( this );
        stateContext->addState( sceneNodeState );

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
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ScopedLock lock( graphicsSystem );

            auto task = Thread::getCurrentTask();
            auto renderTask = graphicsSystem->getRenderTask();
            FB_ASSERT( Thread::getTaskFlag( Thread::Render_Flag ) );

            setLoadingState( LoadingState::Loading );

            Ogre::SceneManager *smgr = nullptr;

            if( auto creator = getCreator() )
            {
                creator->_getObject( reinterpret_cast<void **>( &smgr ) );
            }

            if( auto handle = getHandle() )
            {
                auto name = handle->getName();
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );
            }

            m_sceneNode = smgr->createSceneNode();

            auto graphicsObjects = getObjects();
            for( auto graphicsObject : graphicsObjects )
            {
                if( !graphicsObject->isLoaded() )
                {
                    graphicsObject->load( nullptr );
                }

                Ogre::MovableObject *moveable = nullptr;
                graphicsObject->_getObject( reinterpret_cast<void **>( &moveable ) );

                if( moveable )
                {
                    if( !moveable->isAttached() )
                    {
                        m_sceneNode->attachObject( moveable );
                    }
                }
            }

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

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ScopedLock lock( graphicsSystem );

            if( auto stateManager = applicationManager->getStateManager() )
            {
                auto stateContext = getStateContext();
                if( stateContext )
                {
                    auto stateListener = getStateListener();
                    if( stateListener )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateContext );

                    stateContext->unload( nullptr );
                    setStateContext( nullptr );
                }

                auto stateListener = getStateListener();
                if( stateListener )
                {
                    stateListener->unload( nullptr );
                    setStateListener( nullptr );
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

            SceneNode::unload( data );
            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CSceneNodeOgreNext::setupNode( Ogre::SceneNode *sceneNode )
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
                creator->_getObject( reinterpret_cast<void **>( &ogreSmgr ) );
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
            for( const auto &i : m_children )
            {
                SmartPtr<CSceneNodeOgreNext> child = i;
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

    auto CSceneNodeOgreNext::getLocalAABB() const -> AABB3F
    {
        return calculateAABB();
    }

    auto CSceneNodeOgreNext::getWorldAABB() const -> AABB3F
    {
        // auto applicationManager = core::ApplicationManager::instance();
        // auto graphicsSystem = applicationManager->getGraphicsSystem();
        // auto factoryManager = applicationManager->getFactoryManager();
        // auto renderTask = graphicsSystem->getRenderTask();

        // SmartPtr<SceneNodeState> state = m_stateContext->getState();
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

        return { minPoint.x, minPoint.y, minPoint.z, maxPoint.x, maxPoint.y, maxPoint.z };
    }

    void CSceneNodeOgreNext::attachObject( SmartPtr<IGraphicsObject> object )
    {
        FB_ASSERT( object );

        auto applicationManager = core::ApplicationManager::instance();
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

                    Ogre::MovableObject *movable = nullptr;
                    object->_getObject( reinterpret_cast<void **>( &movable ) );

                    if( movable )
                    {
                        if( !movable->isAttached() )
                        {
                            m_sceneNode->attachObject( movable );
                        }
                        else
                        {
                            movable->detachFromParent();
                            m_sceneNode->attachObject( movable );
                        }
                    }

                    object->setOwner( this );
                    m_graphicsObjects.push_back( object );
                }
                else
                {
                    Ogre::MovableObject *movable = nullptr;
                    object->_getObject( reinterpret_cast<void **>( &movable ) );

                    if( movable )
                    {
                        if( !movable->isAttached() )
                        {
                            m_sceneNode->attachObject( movable );
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

                if( auto stateContext = getStateContext() )
                {
                    stateContext->addMessage( stateTask, message );
                }
            }
        }
        else
        {
            auto message = factoryManager->make_ptr<StateMessageObject>();
            message->setType( STATE_MESSAGE_ATTACH_OBJECT );
            message->setObject( object );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( stateTask, message );
            }
        }
    }

    void CSceneNodeOgreNext::detachObject( SmartPtr<IGraphicsObject> object )
    {
        FB_ASSERT( object );

        auto applicationManager = core::ApplicationManager::instance();
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
            object->_getObject( reinterpret_cast<void **>( &moveable ) );

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

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( stateTask, message );
            }
        }
    }

    void CSceneNodeOgreNext::detachAllObjects()
    {
        auto applicationManager = core::ApplicationManager::instance();
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
            ISharedObject::ScopedLock lock( graphicsSystem );
            m_sceneNode->detachAllObjects();
            m_graphicsObjects.clear();
        }
        else
        {
            auto stateMessage = factoryManager->make_ptr<StateMessageType>();
            stateMessage->setType( STATE_MESSAGE_DETACH_ALL_OBJECTS );

            auto stateContext = getStateContext();
            if( stateContext )
            {
                stateContext->addMessage( stateTask, stateMessage );
            }
        }
    }

    auto CSceneNodeOgreNext::getObjects() const -> Array<SmartPtr<IGraphicsObject>>
    {
        return m_graphicsObjects;
    }

    auto CSceneNodeOgreNext::addChildSceneNode( const String &name ) -> SmartPtr<ISceneNode>
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

    auto CSceneNodeOgreNext::addChildSceneNode( const Vector3F &position ) -> SmartPtr<ISceneNode>
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

    auto CSceneNodeOgreNext::getNumObjects() const -> u32
    {
        return static_cast<u32>( m_graphicsObjects.size() );
    }

    void CSceneNodeOgreNext::addChild( SmartPtr<ISceneNode> child )
    {
        auto applicationManager = core::ApplicationManager::instance();
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
            ISharedObject::ScopedLock lock( graphicsSystem );

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
            child->_getObject( reinterpret_cast<void **>( &sceneNode ) );

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

            auto stateContext = getStateContext();
            if( stateContext )
            {
                stateContext->addMessage( stateTask, message );
            }
        }
    }

    auto CSceneNodeOgreNext::removeChild( SmartPtr<ISceneNode> child ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
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
                    ISharedObject::ScopedLock lock( graphicsSystem );

                    auto it = std::find( m_children.begin(), m_children.end(), child );
                    if( it != m_children.end() )
                    {
                        m_children.erase( it );

                        child->setParent( nullptr );

                        Ogre::SceneNode *sceneNode = nullptr;
                        child->_getObject( reinterpret_cast<void **>( &sceneNode ) );

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

                auto stateContext = getStateContext();
                stateContext->addMessage( stateTask, message );
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_REMOVE_CHILD );
                message->setObject( child );

                auto stateContext = getStateContext();
                if( stateContext )
                {
                    stateContext->addMessage( stateTask, message );
                }
            }

            return true;
        }

        return false;
    }

    void CSceneNodeOgreNext::remove()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
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
                ISharedObject::ScopedLock lock( graphicsSystem );

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

                auto stateContext = getStateContext();
                stateContext->addMessage( stateTask, stateMessage );
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
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ISharedObject::ScopedLock lock( graphicsSystem );

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

    auto CSceneNodeOgreNext::findChild( const String &name ) -> SmartPtr<ISceneNode>
    {
        Array<SmartPtr<ISceneNode>> children;
        getChildren( children );

        for( auto &sceneNode : children )
        {
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

    auto CSceneNodeOgreNext::getChildren() const -> Array<SmartPtr<ISceneNode>>
    {
        return m_children;
    }

    auto CSceneNodeOgreNext::getNumChildren() const -> u32
    {
        return static_cast<u32>( m_children.size() );
    }

    void CSceneNodeOgreNext::needUpdate( bool forceParentUpdate )
    {
        //
        // m_sceneNode->needUpdate(forceParentUpdate);
    }

    auto CSceneNodeOgreNext::clone( SmartPtr<ISceneNode> parent, const String &name ) const
        -> SmartPtr<ISceneNode>
    {
        SmartPtr<CSceneNodeOgreNext> sceneNode = parent->addChildSceneNode();

        // set properties
        sceneNode->setPosition( getPosition() );
        sceneNode->setOrientation( getOrientation() );
        sceneNode->setScale( getScale() );

        // clone attached objects
        for( const auto &graphicsObject : m_graphicsObjects )
        {
            SmartPtr<IGraphicsObject> cloneGraphicsObject = graphicsObject->clone();
            sceneNode->attachObject( cloneGraphicsObject );
        }

        return sceneNode;
    }

    void CSceneNodeOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_sceneNode;
    }

    auto CSceneNodeOgreNext::getSceneNode() const -> Ogre::SceneNode *
    {
        return m_sceneNode;
    }

    void CSceneNodeOgreNext::updateBounds()
    {
    }

    auto CSceneNodeOgreNext::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = SceneNode::getProperties();

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
        if( auto &owner = getOwner() )
        {
            auto applicationManager = core::ApplicationManager::instance();
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
                    if( stateMessage->getType() == STATE_MESSAGE_REMOVE )
                    {
                        owner->remove();
                    }
                }
            }
        }
    }

    void CSceneNodeOgreNext::SceneNodeStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ISharedObject::ScopedLock lock( graphicsSystem );

        if( auto &owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                Ogre::SceneNode *sceneNode = nullptr;
                owner->_getObject( reinterpret_cast<void **>( &sceneNode ) );

                if( sceneNode )
                {
                    if( state->isDerived<TransformState>() )
                    {
                        auto &transformState = fb::reinterpret_pointer_cast<TransformState>( state );

                        auto transform = transformState->getLocalTransform();
                        auto position = transform.getPosition();
                        auto orientation = transform.getOrientation();
                        auto scale = transform.getScale();

                        auto pos = OgreUtil::convertToOgreVector( position );
                        if( !OgreUtil::equals( pos, sceneNode->getPosition() ) )
                        {
                            sceneNode->setPosition( pos );
                        }

                        auto orient = OgreUtil::convertToOgreQuaternion( orientation );
                        if( !OgreUtil::equals( orient, sceneNode->getOrientation() ) )
                        {
                            orient.normalise();
                            sceneNode->setOrientation( orient );
                        }

                        auto vScale = OgreUtil::convertToOgreVector( scale );
                        if( !OgreUtil::equals( vScale, sceneNode->getScale() ) )
                        {
                            sceneNode->setScale( vScale );
                        }

                        transformState->setActualTransform( transform );
                        state->setDirty( false );
                    }
                    else if( state->isDerived<SceneNodeState>() )
                    {
                        auto &sceneNodeState = fb::reinterpret_pointer_cast<SceneNodeState>( state );

                        state->setDirty( false );
                    }
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

    auto CSceneNodeOgreNext::SceneNodeStateListener::getOwner() -> SmartPtr<CSceneNodeOgreNext> &
    {
        return m_owner;
    }

    auto CSceneNodeOgreNext::SceneNodeStateListener::getOwner() const
        -> const SmartPtr<CSceneNodeOgreNext> &
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

    auto CSceneNodeOgreNext::calculateAABB() const -> AABB3F
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

        return { minPoint.x, minPoint.y, minPoint.z, maxPoint.x, maxPoint.y, maxPoint.z };
    }

    auto CSceneNodeOgreNext::_getRenderSystemTransform() const -> void *
    {
        return nullptr;
    }

    void CSceneNodeOgreNext::removeChildren()
    {
    }

    CSceneNodeOgreNext::NodeListener::NodeListener( CSceneNodeOgreNext *owner ) : m_owner( owner )
    {
    }

    CSceneNodeOgreNext::NodeListener::~NodeListener() = default;

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
        smgr->_getObject( reinterpret_cast<void **>( &pSceneManager ) );

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
        smgr->_getObject( reinterpret_cast<void **>( &pSceneManager ) );

        auto sceneNode = (Ogre::SceneNode *)node;
        if( pSceneManager->getRootSceneNode() == sceneNode->getParentSceneNode() )
        {
            // m_owner->registerForUpdates(false, false);
        }
    }
}  // namespace fb::render
