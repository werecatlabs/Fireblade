#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CInstancedObject.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        static int nameExt = 0;

        CInstancedObject::CInstancedObject( SmartPtr<IGraphicsScene> smgr,
                                            const String &materialName,
                                            const String &instanceManagerName ) :
            m_smgr( smgr ),
            m_instancedEntity( nullptr ),
            m_materialName( materialName ),
            m_instanceManagerName( instanceManagerName )
        {
            auto engine = core::IApplicationManager::instance();

            // m_stateObject = platformMgr->createStateObject();

            m_stateListener = SmartPtr<IStateListener>( new InstancedObjectStateListener( this ) );
            m_stateObject->addStateListener( m_stateListener );

            // m_state = InstancedObjectStatePtr(new InstancedObjectState);

            m_name = String( "CInstancedObject" ) + StringUtil::toString( nameExt++ );
        }

        CInstancedObject::~CInstancedObject()
        {
        }

        void CInstancedObject::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();

            switch( task )
            {
            case Thread::Task::Render:
            {
                updateState();
            }
            break;
            default:
            {
            }
            }
        }

        void CInstancedObject::setPosition( const Vector3F &position )
        {
        }

        Vector3F CInstancedObject::getPosition() const
        {
            return Vector3F();
        }

        void CInstancedObject::setOrientation( const QuaternionF &orientation )
        {
        }

        QuaternionF CInstancedObject::getOrientation() const
        {
            return QuaternionF();
        }

        void CInstancedObject::setScale( const Vector3F &scale )
        {
        }

        Vector3F CInstancedObject::getScale() const
        {
            return Vector3F();
        }

        String CInstancedObject::getName() const
        {
            return m_name;
        }

        u32 CInstancedObject::getId() const
        {
            return StringUtil::getHash( m_name );
        }

        void CInstancedObject::detachFromParent()
        {
        }

        void CInstancedObject::_attachToParent( SmartPtr<ISceneNode> parent )
        {
        }

        void CInstancedObject::setMaterialName( const String &materialName, s32 index /*= -1 */ )
        {
        }

        String CInstancedObject::getMaterialName( s32 index /*= -1 */ ) const
        {
            return StringUtil::EmptyString;
        }

        void CInstancedObject::setCastShadows( bool castShadows )
        {
        }

        bool CInstancedObject::getCastShadows() const
        {
            return false;
        }

        void CInstancedObject::setRecieveShadows( bool recieveShadows )
        {
        }

        bool CInstancedObject::getRecieveShadows() const
        {
            return false;
        }

        void CInstancedObject::setVisible( bool isVisible )
        {
            auto currentTaskId = Thread::getCurrentTask();

            if( currentTaskId != Thread::Task::Render )
            {
                SmartPtr<StateMessageVisible> message( new StateMessageVisible );
                message->setVisible( isVisible );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                m_instancedEntity->setVisible( isVisible );
            }

            SpinRWMutex::ScopedLock lock( m_state->Mutex );
            m_state->m_isVisible = isVisible;
        }

        bool CInstancedObject::isVisible() const
        {
            //SpinRWMutex::ScopedLock lock( m_state->Mutex );
            return m_state->m_isVisible;
        }

        void CInstancedObject::setRenderQueueGroup( u8 renderQueueGroup )
        {
            auto currentTaskId = Thread::getCurrentTask();

            if( currentTaskId != Thread::Task::Render )
            {
                SmartPtr<StateMessageUIntValue> message( new StateMessageUIntValue );
                message->setType( RENDER_QUEUE_HASH );
                message->setValue( renderQueueGroup );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                m_instancedEntity->setRenderQueueGroup( renderQueueGroup );
            }

            SpinRWMutex::ScopedLock lock( m_state->Mutex );
            m_state->m_renderQueueGroup = renderQueueGroup;
        }

        void CInstancedObject::setVisibilityFlags( u32 flags )
        {
            auto currentTaskId = Thread::getCurrentTask();

            if( currentTaskId != Thread::Task::Render )
            {
                SmartPtr<StateMessageUIntValue> message( new StateMessageUIntValue );
                message->setType( VISIBILITY_FLAGS_HASH );
                message->setValue( flags );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                m_instancedEntity->setVisibilityFlags( flags );
            }

            SpinRWMutex::ScopedLock lock( m_state->Mutex );
            m_state->m_visibilityMask = flags;
        }

        u32 CInstancedObject::getVisibilityFlags() const
        {
            //SpinRWMutex::ScopedLock lock( m_state->Mutex );
            return m_state->m_visibilityMask;
        }

        SmartPtr<IGraphicsObject> CInstancedObject::clone(
            const String &name /*= StringUtil::EmptyString */ ) const
        {
            return nullptr;
        }

        void CInstancedObject::_getObject( void **ppObject ) const
        {
            *ppObject = m_instancedEntity;
        }

        void CInstancedObject::setPropertyValue( const String &name, const String &value )
        {
        }

        bool CInstancedObject::getPropertyValue( const String &name, String &value )
        {
            return false;
        }

        void CInstancedObject::setProperties( const Properties &propertyGroup )
        {
        }

        bool CInstancedObject::getProperties( Properties &propertyGroup,
                                              u32 flags /*= AllProperties */ ) const
        {
            return false;
        }

        void CInstancedObject::setFlag( u32 flag, bool value )
        {
        }

        bool CInstancedObject::getFlag( u32 flag ) const
        {
            return false;
        }

        AABB3F CInstancedObject::getLocalAABB() const
        {
            return m_localAABB;
        }

        void CInstancedObject::setLocalAABB( const AABB3F &val )
        {
            m_localAABB = val;
        }

        SmartPtr<IStateContext> &CInstancedObject::getStateObject()
        {
            return m_stateObject;
        }

        const SmartPtr<IStateContext> &CInstancedObject::getStateObject() const
        {
            return m_stateObject;
        }

        void CInstancedObject::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void CInstancedObject::setCustomParam( unsigned char idx, const Vector4F &newParam )
        {
            auto currentTaskId = Thread::getCurrentTask();

            if( currentTaskId != Thread::Task::Render )
            {
                SmartPtr<StateMessageParamVector4> message( new StateMessageParamVector4 );
                message->setType( CUSTOM_PARAMETER_HASH );
                message->setId( idx );
                message->setValue( newParam );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                if( !m_instancedEntity )
                {
                    updateState();

                    if( m_instancedEntity )
                        m_instancedEntity->setCustomParam(
                            idx,
                            Ogre::Vector4( newParam.X(), newParam.Y(), newParam.Z(), newParam.W() ) );
                }
                else
                {
                    m_instancedEntity->setCustomParam(
                        idx, Ogre::Vector4( newParam.X(), newParam.Y(), newParam.Z(), newParam.W() ) );
                }
            }

            if( idx < MAX_CUSTOM_PARAMS )
            {
                SpinRWMutex::ScopedLock lock( m_state->Mutex );
                m_state->m_customParams[idx] = newParam;
            }
        }

        Vector4F CInstancedObject::getCustomParam( unsigned char idx )
        {
            if( idx < MAX_CUSTOM_PARAMS )
            {
                SpinRWMutex::ScopedLock lock( m_state->Mutex );
                return m_state->m_customParams[idx];
            }

            return Vector4F::ZERO;
        }

        bool CInstancedObject::isAttached() const
        {
            if( m_instancedEntity )
                return m_instancedEntity->isAttached();

            return false;
        }
        
        void CInstancedObject::setState( SmartPtr<IState> state )
        {
        }

        SmartPtr<IState> CInstancedObject::getState() const
        {
            return nullptr;
        }

        void CInstancedObject::updateState()
        {
            if( !m_instancedEntity )
            {
                Ogre::SceneManager *pSceneManager = nullptr;
                m_smgr->_getObject( (void **)&pSceneManager );
                FB_ASSERT( pSceneManager );

                m_instancedEntity = pSceneManager->createInstancedEntity(
                    m_materialName.c_str(), m_instanceManagerName.c_str() );
            }
            else
            {
                if( m_owner )
                {
                    if( !m_instancedEntity->isAttached() )
                    {
                        Ogre::SceneNode *sceneNode = nullptr;
                        m_owner->_getObject( (void **)&sceneNode );

                        sceneNode->attachObject( m_instancedEntity );
                    }
                }
            }
        }

        void CInstancedObject::setInstancedEntity( Ogre::InstancedEntity *val )
        {
            m_instancedEntity = val;
        }

        Ogre::InstancedEntity *CInstancedObject::getInstancedEntity() const
        {
            return m_instancedEntity;
        }

        CInstancedObject::InstancedObjectStateListener::InstancedObjectStateListener(
            CInstancedObject *instancedObject ) :
            m_owner( instancedObject )
        {
        }

        CInstancedObject::InstancedObjectStateListener::~InstancedObjectStateListener()
        {
            m_owner = nullptr;
        }

        void CInstancedObject::InstancedObjectStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            /*		if(task == Thread::Task::Render)
                        m_owner->updateState();

                    if(message->isExactly(StateMessageVisible::TYPE_INFO))
                    {
                        StateMessageVisiblePtr visibleMessage = message;
                        m_owner->setVisible(visibleMessage->isVisible());
                    }
                    else if(message->isExactly(StateMessageUIntValue::TYPE_INFO))
                    {
                        SmartPtr<StateMessageUIntValue> intValueMessage = message;
                        u32 type = intValueMessage->getType();

                        if(type == CInstancedObject::RENDER_QUEUE_HASH)
                        {
                            m_owner->setRenderQueueGroup(intValueMessage->getValue());
                        }
                        else if(type == CInstancedObject::VISIBILITY_FLAGS_HASH)
                        {
                            m_owner->setVisibilityFlags(intValueMessage->getValue());
                        }
                    }
                    else if(message->isExactly(StateMessageParamVector4::TYPE_INFO))
                    {
                        SmartPtr<StateMessageParamVector4> stateMessage = message;
                        if(stateMessage->getType() == CInstancedObject::CUSTOM_PARAMETER_HASH)
                        {
                            m_owner->setCustomParam(stateMessage->getId(), stateMessage->getValue());
                        }
                    }	*/
        }

        void CInstancedObject::InstancedObjectStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CInstancedObject::InstancedObjectStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CInstancedObject::InstancedObjectState::InstancedObjectState()
        {
            m_visibilityMask = 0;
            m_renderQueueGroup = 0;
            m_isVisible = true;
            m_isRegistered = false;
        }

        CInstancedObject::InstancedObjectState::~InstancedObjectState()
        {
        }

        u32 CInstancedObject::InstancedObjectState::getId() const
        {
            return 0;
        }

        void CInstancedObject::InstancedObjectState::setId( u32 val )
        {
        }

        bool CInstancedObject::InstancedObjectState::isModified() const
        {
            return false;
        }

        void CInstancedObject::InstancedObjectState::setModified( bool modified )
        {
        }

        u32 CInstancedObject::InstancedObjectState::getModifiedFlags() const
        {
            return 0;
        }

        void CInstancedObject::InstancedObjectState::setModifiedFlags( u32 val )
        {
        }

        bool CInstancedObject::InstancedObjectState::isDirty() const
        {
            return false;
            //return m_updateCount.isDirty();
        }

        void CInstancedObject::InstancedObjectState::setDirty( bool dirty )
        {
            //m_updateCount.setDirty( dirty );
        }

        void CInstancedObject::InstancedObjectState::update()
        {
        }

        bool CInstancedObject::InstancedObjectState::isRegistered() const
        {
            return m_isRegistered;
        }

        void CInstancedObject::InstancedObjectState::setRegistered( bool val )
        {
            m_isRegistered = val;
        }
    }  // namespace render
}  // end namespace fb
