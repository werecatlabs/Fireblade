#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CSceneNodeOgre.h>
#include <FBGraphicsOgre/Wrapper/CCameraOgre.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBGraphicsOgre/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        u32 CSceneNodeOgre::m_nameExt = 0;

        FB_CLASS_REGISTER_DERIVED( fb, CSceneNodeOgre, CSceneNode );

        CSceneNodeOgre::CSceneNodeOgre() :
            m_sceneNode( nullptr ),

            m_flags( 0 ),
            m_lastUpdate( 0 ),
            m_transformUpdate( 0 ),

            m_isCulled( true )
        {
            auto handle = getHandle();

            m_nodeListener = nullptr;

            static const auto SceneNodeStr = String( "SceneNode" );
            auto name = SceneNodeStr + StringUtil::toString( m_nameExt++ );
            handle->setName( name );

            auto applicationManager = core::IApplicationManager::instance();
            auto stateManager = applicationManager->getStateManager();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            auto stateObject = stateManager->addStateObject();

            auto sceneNodeStateListener = factoryManager->make_ptr<SceneNodeStateListener>();
            sceneNodeStateListener->setOwner( this );
            m_stateListener = sceneNodeStateListener;
            stateObject->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<SceneNodeState>();
            //state->setSceneNode( this );
            stateObject->setState( state );
            stateObject->setOwner( this );

            setStateObject( stateObject );

            m_state = state;

            auto stateTask = graphicsSystem->getStateTask();
            m_state->setTaskId( stateTask );
        }

        CSceneNodeOgre::CSceneNodeOgre( SmartPtr<IGraphicsScene> creator ) :
            m_sceneNode( nullptr ),

            m_flags( 0 ),
            m_lastUpdate( 0 ),
            m_transformUpdate( 0 ),

            m_isCulled( true )
        {
            auto handle = getHandle();

            m_nodeListener = nullptr;

            m_creator = creator;

            static const auto SceneNodeStr = String( "SceneNode" );
            auto name = SceneNodeStr + StringUtil::toString( m_nameExt++ );
            handle->setName( name );

            auto applicationManager = core::IApplicationManager::instance();
            auto stateManager = applicationManager->getStateManager();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            auto stateObject = stateManager->addStateObject();

            auto sceneNodeStateListener = factoryManager->make_ptr<SceneNodeStateListener>();
            sceneNodeStateListener->setOwner( this );
            m_stateListener = sceneNodeStateListener;
            stateObject->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<SceneNodeState>();
            stateObject->setState( state );
            stateObject->setOwner( this );

            setStateObject( stateObject );

            m_state = state;

            auto stateTask = graphicsSystem->getStateTask();
            m_state->setTaskId( stateTask );
        }

        CSceneNodeOgre::~CSceneNodeOgre()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }

            destroyStateContext();
        }

        void CSceneNodeOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                //#if _DEBUG
                //                auto task = Thread::getCurrentTask();
                //                auto renderTask =
                //                IApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
                //                FB_ASSERT( task == renderTask );
                //#endif

                setLoadingState( LoadingState::Loading );

                auto creator = getCreator();
                if( creator )
                {
                    Ogre::SceneManager *smgr = nullptr;
                    creator->_getObject( (void **)&smgr );

                    auto handle = getHandle();
                    FB_ASSERT( handle );

                    auto name = handle->getName();
                    if( StringUtil::isNullOrEmpty( name ) )
                    {
                        name = "SceneNode" + StringUtil::toString( m_nameExt++ );
                        handle->setName( name );
                    }

                    m_sceneNode = smgr->createSceneNode( name );

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
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &state = getLoadingState();
                if( state != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    if( graphicsSystem )
                    {
                        ScopedLock lock( graphicsSystem );

                        if( !m_graphicsObjects.empty() )
                        {
                            for( auto obj : m_graphicsObjects )
                            {
                                obj->setOwner( nullptr );
                            }

                            m_sceneNode->detachAllObjects();
                            m_graphicsObjects.clear();
                        }

                        destroyStateContext();

                        // remove all the children
                        removeAllChildren();

                        if( auto pParent = getParent() )
                        {
                            auto parent = fb::static_pointer_cast<CSceneNodeOgre>( pParent );
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

                        SmartPtr<CGraphicsSceneOgre> smgr = getCreator();
                        if( smgr )
                        {
                            // remove ogre scene node
                            if( m_sceneNode != nullptr )
                            {
                                m_sceneNode->detachAllObjects();
                                m_graphicsObjects.clear();

                                auto ogreSmgr = smgr->getSceneManager();
                                FB_ASSERT( ogreSmgr );

                                if( m_sceneNode != ogreSmgr->getRootSceneNode() )
                                {
                                    ogreSmgr->destroySceneNode( m_sceneNode );
                                }

                                m_sceneNode = nullptr;
                            }
                        }
                        else
                        {
                            m_sceneNode = nullptr;
                        }

                        auto &gc = GarbageCollector::instance();
                        gc.update();

                        setCreator( nullptr );

                        CSceneNode::unload( nullptr );

                        gc.update();
                    }

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgre::initialise( Ogre::SceneNode *sceneNode )
        {
            m_sceneNode = sceneNode;

            // m_nodeListener = new NodeListener(this);
            // m_sceneNode->setListener(m_nodeListener);

            setLoadingState( LoadingState::Loaded );
        }

        void CSceneNodeOgre::destroy()
        {
            // remove ogre scene node
            if( m_sceneNode != nullptr )
            {
                // remove all the objects from the scene node
                detachAllObjects();

                auto creator = getCreator();

                Ogre::SceneManager *ogreSmgr = nullptr;
                creator->_getObject( (void **)&ogreSmgr );

                ogreSmgr->destroySceneNode( m_sceneNode );
                m_sceneNode = nullptr;
            }
        }

        void CSceneNodeOgre::setMaterialName( const String &materialName, bool cascade )
        {
            for( u32 i = 0; i < m_graphicsObjects.size(); ++i )
            {
                // m_graphicsObjects[i]->setMaterialName(materialName);
            }

            if( cascade )
            {
                for( u32 i = 0; i < m_children.size(); ++i )
                {
                    SmartPtr<CSceneNodeOgre> child = m_children[i];
                    child->setMaterialName( materialName );
                }
            }
        }

        void CSceneNodeOgre::setPosition( const Vector3F &position )
        {
            m_state->setPosition( position );
        }

        Vector3F CSceneNodeOgre::getPosition() const
        {
            return m_state->getPosition();
        }

        Vector3F CSceneNodeOgre::getWorldPosition() const
        {
            return m_state->getAbsolutePosition();
        }

        void CSceneNodeOgre::setRotationFromDegrees( const Vector3F &degrees )
        {
            QuaternionF yaw;
            yaw.fromAngleAxis( MathF::DegToRad( ( degrees ).Y() ), Vector3F::UNIT_Y );

            QuaternionF pitch;
            pitch.fromAngleAxis( MathF::DegToRad( ( degrees ).X() ), Vector3F::UNIT_X );

            QuaternionF roll;
            roll.fromAngleAxis( MathF::DegToRad( ( degrees ).Z() ), Vector3F::UNIT_Z );

            // set the orientation
            setOrientation( yaw * pitch * roll );
        }

        void CSceneNodeOgre::setOrientation( const QuaternionF &orientation )
        {
            m_state->setOrientation( orientation );
        }

        QuaternionF CSceneNodeOgre::getOrientation() const
        {
            return m_state->getOrientation();
        }

        QuaternionF CSceneNodeOgre::getWorldOrientation() const
        {
            return m_state->getAbsoluteOrientation();
        }

        void CSceneNodeOgre::setScale( const Vector3F &scale )
        {
            m_state->setScale( scale );
        }

        Vector3F CSceneNodeOgre::getScale() const
        {
            return m_state->getScale();
        }

        Vector3F CSceneNodeOgre::getWorldScale() const
        {
            return m_state->getAbsoluteScale();
        }

        void CSceneNodeOgre::lookAt( const Vector3F &targetPoint )
        {
            auto direction = targetPoint - getPosition();
            auto orientation = MathUtilF::getOrientationFromDirection(
                direction, -Vector3F::unitZ(), true, Vector3<real_Num>::unitY() );
            setOrientation( orientation );
        }

        void CSceneNodeOgre::setFixedYawAxis( bool useFixed, const Vector3F &fixedAxis )
        {
            if( m_sceneNode )
            {
                Ogre::Vector3 axis( fixedAxis.X(), fixedAxis.Y(), fixedAxis.Z() );
                m_sceneNode->setFixedYawAxis( useFixed, axis );
            }
        }

        AABB3F CSceneNodeOgre::getLocalAABB() const
        {
            return calculateAABB();
        }

        AABB3F CSceneNodeOgre::getWorldAABB() const
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

        void CSceneNodeOgre::setVisible( bool visible, bool cascade /*= true*/ )
        {
            if( m_state )
            {
                m_state->setVisible( visible );
            }
        }

        bool CSceneNodeOgre::isVisible() const
        {
            if( m_state )
            {
                return m_state->isVisible();
            }

            return false;
        }

        void CSceneNodeOgre::setCulled( bool culled )
        {
            m_isCulled = culled;

            // m_sceneNode->setShouldCull(m_isCulled);
        }

        bool CSceneNodeOgre::isCulled() const
        {
            return m_isCulled;
        }

        void CSceneNodeOgre::attachObject( SmartPtr<IGraphicsObject> object )
        {
            try
            {
                FB_ASSERT( object );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                if( isThreadSafe() )
                {
                    FB_ASSERT( m_sceneNode );

                    const auto &objectLoadingState = object->getLoadingState();
                    if( objectLoadingState == LoadingState::Loaded )
                    {
                        ScopedLock lock( graphicsSystem );

                        if( object->isDerived<ICamera>() )
                        {
                            object->setOwner( this );

                            auto camera = fb::static_pointer_cast<CCameraOgre>( object );

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

                            object->setAttached( true );

                            m_graphicsObjects.push_back( object );
                        }
                        else
                        {
                            object->setOwner( this );

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
                                    FB_LOG_ERROR(
                                        "CSceneNodeOgre::attachObject Object already attached." );
                                }
                            }

                            object->setAttached( true );

                            m_graphicsObjects.push_back( object );
                        }
                    }
                    else
                    {
                        auto message = factoryManager->make_ptr<StateMessageObject>();
                        message->setType( STATE_MESSAGE_ATTACH_OBJECT );
                        message->setObject( object );
                        addMessage( message );
                    }

                    if( auto stateObject = getStateObject() )
                    {
                        stateObject->setDirty( true );
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_ATTACH_OBJECT );
                    message->setObject( object );
                    addMessage( message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgre::detachObject( SmartPtr<IGraphicsObject> object )
        {
            try
            {
                FB_ASSERT( object );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                if( isThreadSafe() )
                {
                    ScopedLock lock( graphicsSystem );

                    Ogre::MovableObject *moveable = nullptr;
                    object->_getObject( (void **)&moveable );

                    if( m_sceneNode )
                    {
                        if( moveable )
                        {
                            m_sceneNode->detachObject( moveable );
                        }
                    }

                    auto it = std::find( m_graphicsObjects.begin(), m_graphicsObjects.end(), object );
                    if( it != m_graphicsObjects.end() )
                    {
                        m_graphicsObjects.erase( it );
                    }

                    object->setAttached( false );
                    object->setOwner( nullptr );
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_DETACH_OBJECT );
                    message->setObject( object );
                    addMessage( message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CSceneNodeOgre::detachAllObjects()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( isThreadSafe() )
            {
                ScopedLock lock( graphicsSystem );

                for( auto obj : m_graphicsObjects )
                {
                    obj->setOwner( nullptr );
                }

                m_sceneNode->detachAllObjects();
                m_graphicsObjects.clear();
            }
            else
            {
                auto factoryManager = applicationManager->getFactoryManager();
                auto renderTask = graphicsSystem->getRenderTask();

                auto stateMessage = factoryManager->make_ptr<StateMessageType>();
                stateMessage->setType( STATE_MESSAGE_DETACH_ALL_OBJECTS );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( renderTask, stateMessage );
                }
            }
        }

        Array<SmartPtr<IGraphicsObject>> CSceneNodeOgre::getObjects() const
        {
            return m_graphicsObjects;
        }

        SmartPtr<ISceneNode> CSceneNodeOgre::addChildSceneNode( const String &name )
        {
            SmartPtr<ISceneNode> sceneNode;

            auto creator = getCreator();

            if( name.length() == 0 )
            {
                sceneNode = creator->addSceneNode();
            }
            else
            {
                sceneNode = creator->addSceneNode( name );
            }

            addChild( sceneNode );

            return sceneNode;
        }

        SmartPtr<ISceneNode> CSceneNodeOgre::addChildSceneNode( const Vector3F &position )
        {
            auto creator = getCreator();
            auto sceneNode = creator->addSceneNode();
            addChild( sceneNode );
            sceneNode->setPosition( position );
            return sceneNode;
        }

        u32 CSceneNodeOgre::getNumObjects() const
        {
            return (u32)m_graphicsObjects.size();
        }

        void CSceneNodeOgre::addChild( SmartPtr<ISceneNode> child )
        {
            if( isThreadSafe() && child->isLoaded() )
            {
                child->remove();  // remove from old parent

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
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto factoryManager = applicationManager->getFactoryManager();

                if( !child->isLoaded() )
                {
                    graphicsSystem->loadObject( child );
                }

                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_ADD_CHILD );
                message->setObject( child );
                addMessage( message );
            }
        }

        bool CSceneNodeOgre::removeChild( SmartPtr<ISceneNode> child )
        {
            if( isThreadSafe() )
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
            }
            else
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();

                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( STATE_MESSAGE_REMOVE_CHILD );
                message->setObject( child );
                addMessage( message );
            }

            return false;
        }

        void CSceneNodeOgre::add()
        {
            if( isThreadSafe() )
            {
                if( !m_sceneNode->isInSceneGraph() )
                {
                    auto creator = getCreator();
                    auto root = creator->getRootSceneNode();
                    root->addChild( this );
                }
            }
            else
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();

                auto message = factoryManager->make_ptr<StateMessageType>();
                message->setType( STATE_MESSAGE_ADD );
                addMessage( message );
            }
        }

        void CSceneNodeOgre::remove()
        {
            if( isThreadSafe() )
            {
                if( auto parent = getParent() )
                {
                    parent->removeChild( this );
                }
            }
            else
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();

                auto message = factoryManager->make_ptr<StateMessageType>();
                message->setType( STATE_MESSAGE_REMOVE );
                addMessage( message );
            }
        }

        void CSceneNodeOgre::removeAllChildren()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ISharedObject::ScopedLock lock( graphicsSystem );

            Array<SmartPtr<ISceneNode>> children;
            getChildren( children );

            for( u32 i = 0; i < children.size(); ++i )
            {
                SmartPtr<ISceneNode> &child = children[i];
                child->remove();
            }

            m_children.clear();
        }

        SmartPtr<ISceneNode> CSceneNodeOgre::findChild( const String &name )
        {
            Array<SmartPtr<ISceneNode>> children;
            getChildren( children );

            for( u32 i = 0; i < children.size(); ++i )
            {
                SmartPtr<ISceneNode> &sceneNode = children[i];
                if( name == ( sceneNode->getHandle()->getName() ) )
                {
                    return sceneNode;
                }
            }

            return nullptr;
        }

        void CSceneNodeOgre::getChildren( Array<SmartPtr<ISceneNode>> &children ) const
        {
            auto numChildren = m_children.size();
            children.reserve( numChildren );

            for( size_t i = 0; i < numChildren; ++i )
            {
                SmartPtr<ISceneNode> sceneNode( m_children[i] );
                children.push_back( sceneNode );
            }
        }

        Array<SmartPtr<ISceneNode>> CSceneNodeOgre::getChildren() const
        {
            return m_children;
        }

        u32 CSceneNodeOgre::getNumChildren() const
        {
            return static_cast<u32>( m_children.size() );
        }

        void CSceneNodeOgre::needUpdate( bool forceParentUpdate )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ISharedObject::ScopedLock lock( graphicsSystem );

            m_sceneNode->needUpdate( forceParentUpdate );
        }

        SmartPtr<ISceneNode> CSceneNodeOgre::clone( SmartPtr<ISceneNode> parent,
                                                    const String &name ) const
        {
            SmartPtr<CSceneNodeOgre> sceneNode = parent->addChildSceneNode();

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

        void CSceneNodeOgre::showBoundingBox( bool show )
        {
            if( m_sceneNode )
            {
                m_sceneNode->showBoundingBox( show );
            }
        }

        bool CSceneNodeOgre::getShowBoundingBox() const
        {
            if( m_sceneNode )
            {
                return m_sceneNode->getShowBoundingBox();
            }

            return false;
        }

        void CSceneNodeOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_sceneNode;
        }

        void CSceneNodeOgre::setTransformationDirty( u32 msgType )
        {
        }

        Ogre::SceneNode *CSceneNodeOgre::getSceneNode() const
        {
            return m_sceneNode;
        }

        void CSceneNodeOgre::_updateBoundingBox()
        {
        }

        void CSceneNodeOgre::_updateBounds()
        {
            if( m_sceneNode )
            {
                m_sceneNode->_updateBounds();
            }
        }

        void CSceneNodeOgre::setVisibilityFlags( u32 flags )
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

        u32 CSceneNodeOgre::getVisibilityFlags() const
        {
            return m_visibilityFlags;
        }

        void CSceneNodeOgre::setFlag( u32 flag, bool value )
        {
            m_flags = BitUtil::setFlagValue( static_cast<u32>( m_flags ), flag, value );
        }

        bool CSceneNodeOgre::getFlag( u32 flag ) const
        {
            return BitUtil::getFlagValue( static_cast<u32>( m_flags ), flag );
        }

        s32 CSceneNodeOgre::ScriptReceiver::setProperty( hash_type id, const Parameter &param )
        {
            // if(id == StringUtil::VISIBILITY_MASK_HASH)
            //{
            //	m_node->setVisibilityFlags(param.data.iData);
            // }
            // else if(id == StringUtil::VISIBLE_HASH)
            //{
            //	m_node->setVisible(param.data.bData ? true : false);
            // }

            return 0;
        }

        s32 CSceneNodeOgre::ScriptReceiver::setProperty( hash_type id, const Parameters &params )
        {
            // if(id == StringUtil::POSITION_3_HASH)
            //{
            //	Vector3F position;
            //	position[0] = params[0].data.fData;
            //	position[1] = params[1].data.fData;
            //	position[2] = params[2].data.fData;
            //	m_node->setPosition(position);

            //	return 0;
            //}
            // else if(id == StringUtil::ORIENTATION_HASH)
            //{
            //	QuaternionF orientation;
            //	orientation.W() = params[0].data.fData;
            //	orientation.X() = params[1].data.fData;
            //	orientation.Y() = params[2].data.fData;
            //	orientation.Z() = params[3].data.fData;
            //	m_node->setOrientation(orientation);

            //	return 0;
            //}

            return 0;
        }

        s32 CSceneNodeOgre::ScriptReceiver::setProperty( hash_type hash, void *param )
        {
            // if(hash == StringUtil::POSITION_3_HASH)
            //{
            //	Vector3F position = *static_cast<Vector3F*>(param);
            //	m_node->setPosition(position);
            //	return 0;
            // }
            // else if(hash == StringUtil::SCALE_3_HASH)
            //{
            //	Vector3F scale = *static_cast<Vector3F*>(param);
            //	m_node->setScale(scale);
            //	return 0;
            // }
            // else if(hash == StringUtil::ORIENTATION_HASH)
            //{
            //	QuaternionF orientation = *static_cast<QuaternionF*>(param);
            //	m_node->setOrientation(orientation);
            //	return 0;
            // }

            return 0;
        }

        s32 CSceneNodeOgre::ScriptReceiver::getProperty( hash_type hash, void *param ) const
        {
            // if(hash == StringUtil::POSITION_3_HASH)
            //{
            //	Vector3F& position = *static_cast<Vector3F*>(param);
            //	position = m_node->getPosition();
            //	return 0;
            // }
            // else if(hash == StringUtil::SCALE_3_HASH)
            //{
            //	Vector3F& scale = *static_cast<Vector3F*>(param);
            //	scale = m_node->getScale();
            //	return 0;
            // }
            // else if(hash == StringUtil::ORIENTATION_HASH)
            //{
            //	QuaternionF& orientation = *static_cast<QuaternionF*>(param);
            //	orientation = m_node->getOrientation();
            //	return 0;
            // }

            return 0;
        }

        s32 CSceneNodeOgre::ScriptReceiver::getProperty( hash_type id, Parameters &params ) const
        {
            return 0;
        }

        s32 CSceneNodeOgre::ScriptReceiver::getProperty( hash_type id, Parameter &param ) const
        {
            return 0;
        }

        s32 CSceneNodeOgre::ScriptReceiver::callFunction( hash_type hashId, const Parameters &params,
                                                          Parameters &results )
        {
            // if(hashId == StringUtil::ADD_HASH)
            //{
            //	m_node->add();
            //	return 0;
            // }
            // else if(hashId == StringUtil::REMOVE_HASH)
            //{
            //	m_node->remove();
            //	return 0;
            // }

            return 0;
        }

        CSceneNodeOgre::ScriptReceiver::ScriptReceiver( CSceneNodeOgre *node ) : m_node( node )
        {
        }

        void CSceneNodeOgre::setProperties( SmartPtr<Properties> properties )
        {
        }

        Array<SmartPtr<ISharedObject>> CSceneNodeOgre::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> childObjects;

            auto creator = getCreator();
            childObjects.push_back( creator );

            auto parent = getParent();
            childObjects.push_back( parent );

            // childObjects.push_back( m_stateObject );
            // childObjects.push_back( m_stateListener );
            childObjects.push_back( m_state );
            return childObjects;
        }

        SmartPtr<Properties> CSceneNodeOgre::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

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
                sceneNodeOrientation.toDegrees( localRotation );

                sceneNodeOrientation.normalise().toDegrees( localRotation );

                Vector3<real_Num> localRotation2;
                m_state->getOrientation().toDegrees( localRotation2 );

                properties->setProperty( "sceneNodePosition",
                                         Ogre::StringConverter::toString( sceneNodePosition ) );
                properties->setProperty( "sceneNodeScale",
                                         Ogre::StringConverter::toString( sceneNodeScale ) );
                properties->setProperty( "sceneNodeOrientation", localRotation );

                properties->setProperty( "stateOrientation", localRotation2 );
            }

            return properties;
        }

        void CSceneNodeOgre::SceneNodeStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            FB_ASSERT( message );
            FB_ASSERT( m_owner );

            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();

            auto currentTaskId = Thread::getCurrentTask();
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
                        m_owner->setPosition( value );
                    }
                    else if( type == STATE_MESSAGE_SCALE )
                    {
                        m_owner->setScale( value );
                    }
                    else if( type == STATE_MESSAGE_LOOK_AT )
                    {
                        m_owner->lookAt( value );
                    }
                }
                else if( message->isExactly<StateMessageOrientation>() )
                {
                    auto orientationMessage =
                        fb::static_pointer_cast<StateMessageOrientation>( message );
                    m_owner->setOrientation( orientationMessage->getOrientation() );
                }
                else if( message->isExactly<StateMessageObject>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                    FB_ASSERT( objectMessage );

                    if( objectMessage->getType() == STATE_MESSAGE_ADD_CHILD )
                    {
                        m_owner->addChild( objectMessage->getObject() );
                    }
                    else if( objectMessage->getType() == STATE_MESSAGE_ATTACH_OBJECT )
                    {
                        m_owner->attachObject( objectMessage->getObject() );
                    }
                    else if( objectMessage->getType() == STATE_MESSAGE_DETACH_OBJECT )
                    {
                        m_owner->detachObject( objectMessage->getObject() );
                    }
                    else if( objectMessage->getType() == STATE_MESSAGE_DETACH_ALL_OBJECTS )
                    {
                        m_owner->detachAllObjects();
                    }
                }
                else if( message->isExactly<StateMessageType>() )
                {
                    auto stateMessage = fb::static_pointer_cast<StateMessageType>( message );
                    if( stateMessage->getType() == STATE_MESSAGE_ADD )
                    {
                        m_owner->add();
                    }
                    else if( stateMessage->getType() == STATE_MESSAGE_REMOVE )
                    {
                        m_owner->remove();
                    }
                }
                else if( message->isExactly<StateMessageVisible>() )
                {
                    auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                    m_owner->setVisible( visibleMessage->isVisible(), visibleMessage->getCascade() );
                }
            }
        }

        void CSceneNodeOgre::SceneNodeStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            FB_ASSERT( m_owner );

            Ogre::SceneNode *sceneNode = nullptr;
            m_owner->_getObject( (void **)&sceneNode );

            if( sceneNode )
            {
                auto sceneNodeState = fb::static_pointer_cast<SceneNodeState>( state );
                if( sceneNodeState )
                {
                    state->setDirty( false );

                    auto position = sceneNodeState->getPosition();
                    auto scale = sceneNodeState->getScale();
                    auto orientation = sceneNodeState->getOrientation();

                    auto pos = OgreUtil::convertToOgreVector( position );
                    auto orient = OgreUtil::convertToOgreQuaternion( orientation );
                    auto vScale = OgreUtil::convertToOgreVector( scale );

                    //if( sceneNode->getName().find( "poly" ) != String::npos )
                    //{
                    //    int stop = 0;
                    //    stop = 0;
                    //}

                    auto visible = sceneNodeState->isVisible();
                    sceneNode->setVisible( visible );

                    if( ( !OgreUtil::equals( pos, sceneNode->getPosition() ) ) ||
                        ( !OgreUtil::equals( orient, sceneNode->getOrientation() ) ) ||
                        ( !OgreUtil::equals( vScale, sceneNode->getScale() ) ) )
                    {
                        sceneNode->setPosition( pos );

                        orient.normalise();
                        sceneNode->setOrientation( orient );

                        sceneNode->setScale( vScale );

                        sceneNode->_update( true, false );

                        auto worldPosition = sceneNode->_getDerivedPosition();
                        auto worldOrientation = sceneNode->_getDerivedOrientation();
                        auto worldScale = sceneNode->_getDerivedScale();

                        sceneNodeState->setAbsolutePosition(
                            Vector3<real_Num>( worldPosition.x, worldPosition.y, worldPosition.z ) );
                        sceneNodeState->setAbsoluteOrientation(
                            Quaternion<real_Num>( worldOrientation.w, worldOrientation.x,
                                                  worldOrientation.y, worldOrientation.z ) );
                        sceneNodeState->setAbsoluteScale(
                            Vector3<real_Num>( worldScale.x, worldScale.y, worldScale.z ) );
                    }
                }
            }
        }

        void CSceneNodeOgre::SceneNodeStateListener::handleQuery( SmartPtr<IStateQuery> &query )
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

        CSceneNodeOgre *CSceneNodeOgre::SceneNodeStateListener::getOwner() const
        {
            return m_owner;
        }

        void CSceneNodeOgre::SceneNodeStateListener::setOwner( CSceneNodeOgre *owner )
        {
            m_owner = owner;
        }

        CSceneNodeOgre::SceneNodeStateListener::~SceneNodeStateListener()
        {
            m_owner = nullptr;
        }

        AABB3F CSceneNodeOgre::calculateAABB() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::AxisAlignedBox box;

            for( u32 i = 0; i < m_sceneNode->numAttachedObjects(); ++i )
            {
                Ogre::MovableObject *object = m_sceneNode->getAttachedObject( i );
                if( object->getTypeFlags() & Ogre::SceneManager::ENTITY_TYPE_MASK )
                {
                    auto entity = static_cast<Ogre::Entity *>( object );
                    box.merge( entity->getBoundingBox() );
                }
            }

            const Ogre::Vector3 &minPoint = box.getMinimum();
            const Ogre::Vector3 &maxPoint = box.getMaximum();

            return AABB3F( minPoint.x, minPoint.y, minPoint.z, maxPoint.x, maxPoint.y, maxPoint.z );
        }

        bool CSceneNodeOgre::_getFlag( u32 flag ) const
        {
            return ( m_flags & flag ) != 0;
        }

        void *CSceneNodeOgre::_getRenderSystemTransform() const
        {
            return nullptr;
        }

        u32 CSceneNodeOgre::getObjectsBuffer( SmartPtr<IGraphicsObject> *buffer, u32 bufferSize ) const
        {
            return 0;
        }

        void CSceneNodeOgre::removeChildren()
        {
        }

        u32 CSceneNodeOgre::getChildrenBuffer( SmartPtr<ISceneNode> *children, u32 bufferSize ) const
        {
            return 0;
        }

        void CSceneNodeOgre::destroyStateContext()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();

            if( auto stateObject = getStateObject() )
            {
                if( auto stateListener = getStateListener() )
                {
                    stateObject->removeStateListener( stateListener );
                }

                if( stateManager )
                {
                    stateManager->removeStateObject( stateObject );
                }

                stateObject->unload( nullptr );
                setStateObject( nullptr );
            }

            if( auto stateListener = getStateListener() )
            {
                stateListener->unload( nullptr );
                setStateListener( nullptr );
            }

            if( m_state )
            {
                m_state->setOwner( nullptr );
                m_state->unload( nullptr );
                m_state = nullptr;
            }
        }

        CSceneNodeOgre::NodeListener::NodeListener( CSceneNodeOgre *owner ) : m_owner( owner )
        {
        }

        CSceneNodeOgre::NodeListener::~NodeListener()
        {
        }

        void CSceneNodeOgre::NodeListener::nodeUpdated( const Ogre::Node *node )
        {
        }

        void CSceneNodeOgre::NodeListener::nodeDestroyed( const Ogre::Node *node )
        {
        }

        void CSceneNodeOgre::NodeListener::nodeAttached( const Ogre::Node *node )
        {
            Ogre::SceneManager *pSceneManager = nullptr;

            auto smgr = m_owner->getCreator();
            smgr->_getObject( (void **)&pSceneManager );

            auto sceneNode = (Ogre::SceneNode *)node;
            if( pSceneManager->getRootSceneNode() == sceneNode->getParentSceneNode() )
            {
                // m_owner->registerForUpdates(true, false);
            }
        }

        void CSceneNodeOgre::NodeListener::nodeDetached( const Ogre::Node *node )
        {
            Ogre::SceneManager *pSceneManager = nullptr;

            auto smgr = m_owner->getCreator();
            smgr->_getObject( (void **)&pSceneManager );

            auto sceneNode = (Ogre::SceneNode *)node;
            if( pSceneManager->getRootSceneNode() == sceneNode->getParentSceneNode() )
            {
                // m_owner->registerForUpdates(false, false);
            }
        }
    }  // end namespace render
}  // end namespace fb
