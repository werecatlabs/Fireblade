#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CAnimationControllerOgre.h>
#include <FBCore/FBCore.h>
#include "FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h"
#include <OgreAnimationState.h>
#include <OgreEntity.h>

namespace fb
{
    namespace render
    {

        const u32 CAnimationController::SET_ANIMATION_ENABLED_HASH =
            StringUtil::getHash( "setAnimationEnabled" );
        const u32 CAnimationController::SET_ANIMATION_ENABLED_TIME_HASH =
            StringUtil::getHash( "setAnimationEnabledTime" );
        const u32 CAnimationController::IS_ANIMATION_ENABLED_HASH =
            StringUtil::getHash( "isAnimationEnabled" );
        const u32 CAnimationController::SET_ANIMATION_LOOP_HASH =
            StringUtil::getHash( "setAnimationLoop" );

        const u32 CAnimationController::STOP_ALL_ANIMATIONS_HASH =
            StringUtil::getHash( "stopAllAnimations" );

        //--------------------------------------------
        CAnimationController::CAnimationController()
        {
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this)));

            auto applicationManager = core::IApplicationManager::instance();

            auto factoryManager = applicationManager->getFactoryManager();

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            //m_stateObject = stateManager->addStateObject();

            m_stateListener = SmartPtr<IStateListener>( new AnimationControllerStateListener( this ) );
            m_stateObject->addStateListener( m_stateListener );
        }

        //--------------------------------------------
        CAnimationController::~CAnimationController()
        {
            auto engine = core::IApplicationManager::instance();
            SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
            //graphicsSystem->removeFrameListener( m_frameListener );
        }

        //--------------------------------------------
        void CAnimationController::Initialize( Ogre::v1::Entity *entity )
        {
            m_entity = entity;
        }

        //--------------------------------------------
        void CAnimationController::update( const s32 &task, const time_interval &t,
                                           const time_interval &dt )
        {
            if( Thread::getCurrentTask() == Thread::Task::Render )
            {
                _update( dt );
            }
        }

        //--------------------------------------------
        bool CAnimationController::setAnimationEnabled( const String &animationName, bool enabled )
        {
            if( Thread::getCurrentTask() != Thread::Task::Render )
            {
                SmartPtr<StateMessageAnimationEnable> message( new StateMessageAnimationEnable );
                message->setType( CAnimationController::SET_ANIMATION_ENABLED_HASH );
                message->setName( animationName );
                message->setEnabled( enabled );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                return _setAnimationEnabled( animationName, enabled );
            }

            return false;
        }

        //--------------------------------------------
        bool CAnimationController::setAnimationEnabled( const String &animationName, bool enabled,
                                                        f32 timePosition )
        {
            if( Thread::getCurrentTask() != Thread::Task::Render )
            {
                SmartPtr<StateMessageAnimationEnable> message( new StateMessageAnimationEnable );
                message->setType( CAnimationController::SET_ANIMATION_ENABLED_TIME_HASH );
                message->setName( animationName );
                message->setEnabled( enabled );
                message->setTime( timePosition );
                m_stateObject->addMessage( Thread::Task::Render, message );
            }
            else
            {
                return _setAnimationEnabled( animationName, enabled, timePosition );
            }

            return false;
        }

        //--------------------------------------------
        bool CAnimationController::isAnimationEnabled( const String &animationName )
        {
            auto animation = findAnimation( animationName );
            if( animation )
            {
                //			return animation->isEnabled();
            }

            return false;
        }

        //--------------------------------------------
        void CAnimationController::stopAllAnimations()
        {
            // if(Thread::getCurrentTask() != Thread::Task::TASK_ID_RENDER)
            //{
            //	SmartPtr<StateMessageStop> message(new StateMessageStop);
            //	message->setType(CAnimationController::STOP_ALL_ANIMATIONS_HASH);
            //	m_stateObject->addMessage(Thread::Task::TASK_ID_RENDER, message);
            // }
            // else
            //{
            //	Ogre::AnimationStateSet* animationStateSet = m_entity->getAllAnimationStates();
            //	Ogre::AnimationStateIterator it=animationStateSet->getAnimationStateIterator();
            //	Ogre::AnimationState* pAnimState = NULL;
            //	while(it.hasMoreElements())
            //	{
            //		pAnimState = it.getNext();
            //		pAnimState->setEnabled(false);
            //	}
            // }
        }

        //--------------------------------------------
        bool CAnimationController::hasAnimationEnded( const String &animationName )
        {
            auto animationStateSet = m_entity->getAllAnimationStates();
            if( !animationStateSet->hasAnimationState( animationName.c_str() ) )
            {
                String msg =
                    String(
                        "CAnimationController::hasAnimationEnded: Animation not found. animation: " ) +
                    animationName;
                FB_LOG_MESSAGE( "AnimationController", msg.c_str() );
                return false;
            }

            auto pAnimState = animationStateSet->getAnimationState( animationName.c_str() );
            if( pAnimState && pAnimState->hasEnded() )
            {
                return true;
            }

            return false;
        }

        //--------------------------------------------
        bool CAnimationController::hasAnimation( const String &animationName ) const
        {
            // Ogre::AnimationStateSet* animationStateSet = m_entity->getAllAnimationStates();
            // if(!animationStateSet)
            //{
            //	return false;
            // }

            // if(!animationStateSet->hasAnimationState(animationName.c_str()))
            //{
            //	return false;
            // }

            return true;
        }

        //--------------------------------------------
        void CAnimationController::setAnimationLoop( const String &animationName, bool loop )
        {
            auto animationStateSet = m_entity->getAllAnimationStates();
            if( !animationStateSet->hasAnimationState( animationName.c_str() ) )
            {
                String msg =
                    String(
                        "CAnimationController::setAnimationLoop: Animation not found. animation: " ) +
                    animationName;
                FB_LOG_MESSAGE( "AnimationController", msg.c_str() );
                return;
            }

            auto pAnimState = animationStateSet->getAnimationState( animationName.c_str() );
            if( pAnimState )
            {
                pAnimState->setLoop( loop );
            }
        }

        //--------------------------------------------
        bool CAnimationController::isAnimationLooping( const String &animationName )
        {
            auto animation = findAnimation( animationName );
            if( animation )
            {
                // animation->isLooping();
            }

            return false;
        }

        //--------------------------------------------
        void CAnimationController::setAnimationReversed( const String &animationName, bool reversed )
        {
        }

        //--------------------------------------------
        bool CAnimationController::isAnimationReversed( const String &animationName )
        {
            return false;
        }

        //--------------------------------------------
        bool CAnimationController::setTimePosition( const String &animationName, f32 timePosition )
        {
            auto animationStateSet = m_entity->getAllAnimationStates();
            if( !animationStateSet->hasAnimationState( animationName.c_str() ) )
            {
                String msg =
                    String( "CAnimationController::setTimePosition: Animation not found. animation: " ) +
                    animationName;
                FB_LOG_MESSAGE( "AnimationController", msg.c_str() );
                return false;
            }

            auto pAnimState = animationStateSet->getAnimationState( animationName.c_str() );
            if( pAnimState )
            {
                pAnimState->setTimePosition( timePosition );
                return true;
            }

            return false;
        }

        //--------------------------------------------
        f32 CAnimationController::getTimePosition( const String &animationName )
        {
            auto animationStateSet = m_entity->getAllAnimationStates();
            if( !animationStateSet->hasAnimationState( animationName.c_str() ) )
            {
                String msg =
                    String( "CAnimationController::setTimePosition: Animation not found. animation: " ) +
                    animationName;
                FB_LOG_MESSAGE( "AnimationController", msg.c_str() );
                return false;
            }

            auto pAnimState = animationStateSet->getAnimationState( animationName.c_str() );
            if( pAnimState )
            {
                return pAnimState->getTimePosition();
            }

            return 0.0f;
        }

        //--------------------------------------------
        f32 CAnimationController::getAnimationLength( const String &animationName ) const
        {
            SmartPtr<IAnimation> animation = findAnimation( animationName );
            if( animation )
                animation->getLength();

            return 0.0f;
        }

        //--------------------------------------------
        SmartPtr<IStateContext> &CAnimationController::getStateObject()
        {
            return m_stateObject;
        }

        //--------------------------------------------
        const SmartPtr<IStateContext> &CAnimationController::getStateObject() const
        {
            return m_stateObject;
        }

        //--------------------------------------------
        void CAnimationController::setStateObject( SmartPtr<IStateContext> subject )
        {
            m_stateObject = subject;
        }

        //--------------------------------------------
        void CAnimationController::addListener( IAnimationControllerListener *listener )
        {
            m_listeners.push_back( listener );
        }

        //--------------------------------------------
        void CAnimationController::removeListener( IAnimationControllerListener *listener )
        {
            // m_listeners.erase_element(listener);
        }

        //--------------------------------------------
        void CAnimationController::_update( f64 dt )
        {
            // Ogre::AnimationStateSet* animationStateSet = m_entity->getAllAnimationStates();
            // if(!animationStateSet)
            //{
            //	return;
            // }

            // Ogre::AnimationStateIterator it = animationStateSet->getAnimationStateIterator();
            // Ogre::AnimationState* pAnimState = NULL;
            // while(it.hasMoreElements())
            //{
            //	pAnimState = it.getNext();

            //	if(pAnimState->getEnabled() && pAnimState->hasEnded() && !pAnimState->getLoop())
            //	{
            //		/*StateManagerPtr& stateMgr = IApplicationManager::instance()->getStateManager();

            //		static const u32 STATE_ON_ANIMATION_END =
            // StringUtil::getHashMakeLower("OnAnimationEnd");

            //		String animationName = pAnimState->getAnimationName().c_str();
            //		u32 animationHash = StringUtil::getHashMakeLower("animationName");

            //		StateMessageDefaultPtr stateMessage(new StateMessageDefault, true);
            //		stateMessage->setSubjectId(m_stateObject->getSubjectId());
            //		stateMessage->setType(STATE_ON_ANIMATION_END);
            //		stateMessage->setProperty(animationHash, animationName);
            //		stateMgr->notifyStateChanged(stateMessage);
            //		*/

            //		pAnimState->setEnabled(false);
            //	}

            //	if(pAnimState->getEnabled())
            //	{
            //		pAnimState->addTime( dt );
            //	}
            //}

            // Array<SmartPtr<IAnimation>> animations;
            // animations.reserve(m_animations.size());

            // Animations::iterator animationsIt = m_animations.begin();
            // for(; animationsIt != m_animations.end(); ++animationsIt)
            //{
            //	animations.push_back(animationsIt->second);
            // }

            // for(u32 i=0; i<animations.size(); ++i)
            //{
            //	SmartPtr<IAnimation> animation = animations[i];
            //	Ogre::AnimationState* animationState = animation->getAnimationState();

            //	animation->setEnabled(animationState->getEnabled());
            //	animation->setLength(animationState->getLength());
            //	animation->setLooping(animationState->getLoop());
            //}
        }

        //--------------------------------------------
        bool CAnimationController::_setAnimationEnabled( const String &animationName, bool enabled )
        {
            if( !m_entity->hasSkeleton() && !m_entity->hasVertexAnimation() )
                return false;

            auto animationStateSet = m_entity->getAllAnimationStates();
            if( !animationStateSet->hasAnimationState( animationName.c_str() ) )
            {
                String msg =
                    String(
                        "CAnimationController::setAnimationEnabled: Animation not found. animation: " ) +
                    animationName;
                FB_LOG_MESSAGE( "AnimationController", msg.c_str() );
                return false;
            }

            auto pAnimState = animationStateSet->getAnimationState( animationName.c_str() );
            if( pAnimState )
            {
                pAnimState->setEnabled( enabled );

                // addAnimation(pAnimState);

                return true;
            }

            return false;
        }

        //--------------------------------------------
        bool CAnimationController::_setAnimationEnabled( const String &animationName, bool enabled,
                                                         f32 timePosition )
        {
            if( !m_entity->hasSkeleton() && !m_entity->hasVertexAnimation() )
                return false;

            auto animationStateSet = m_entity->getAllAnimationStates();
            if( !animationStateSet->hasAnimationState( animationName.c_str() ) )
            {
                String msg =
                    String(
                        "CAnimationController::setAnimationEnabled: Animation not found. animation: " ) +
                    animationName;
                FB_LOG_MESSAGE( "AnimationController", msg.c_str() );
                return false;
            }

            auto pAnimState = animationStateSet->getAnimationState( animationName.c_str() );
            if( pAnimState )
            {
                pAnimState->setEnabled( enabled );
                pAnimState->setTimePosition( timePosition );

                // addAnimation(pAnimState);

                return true;
            }

            return false;
        }

        void CAnimationController::addAnimation( Ogre::v1::AnimationState *pAnimState )
        {
            SpinRWMutex::ScopedLock lock( Mutex );

            // SmartPtr<IAnimation> animation(new Animation(this));
            // animation->setAnimationState(pAnimState);
            // animation->setEnabled(pAnimState->getEnabled());
            // animation->setLength(pAnimState->getLength());
            // animation->setLooping(pAnimState->getLoop());

            // hash32 hash = StringUtil::getHash(pAnimState->getAnimationName().c_str());
            // m_animations[hash] = animation;

            // if(!m_frameListener)
            //{
            //	auto engine = core::IApplicationManager::instance();
            //	SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();

            //	m_frameListener = SmartPtr<IFrameListener>(new AnimationControllerFrameListener(this));
            //	graphicsSystem->addFrameListener(m_frameListener);
            //}
        }

        SmartPtr<IAnimation> CAnimationController::findAnimation( const String &animationName ) const
        {
            SpinRWMutex::ScopedLock lock( Mutex );

            hash32 hash = StringUtil::getHash( animationName );
            Animations::const_iterator it = m_animations.find( hash );
            if( it != m_animations.end() )
            {
                SmartPtr<IAnimation> animation = it->second;
                return animation;
            }

            return nullptr;
        }

        CAnimationController::ScriptReceiver::ScriptReceiver( IAnimationController *animCtrl ) :
            m_animCtrl( animCtrl )
        {
        }

        //--------------------------------------------
        s32 CAnimationController::ScriptReceiver::callFunction( u32 hashId, const Parameters &params,
                                                                Parameters &results )
        {
            if( hashId == SET_ANIMATION_ENABLED_HASH )
            {
                m_animCtrl->setAnimationEnabled( (char *)params[0].data.pData, params[1].data.bData );
            }
            else if( hashId == IS_ANIMATION_ENABLED_HASH )
            {
                bool value = m_animCtrl->isAnimationEnabled( (char *)params[0].data.pData );
                // results.(1);
                results[0].setBool( value );
            }
            else if( hashId == SET_ANIMATION_LOOP_HASH )
            {
                m_animCtrl->setAnimationLoop( (char *)params[0].data.pData, params[1].data.bData );
            }
            // else if( hashId == StringUtil::UPDATE_HASH )
            //{
            //	SmartPtr<ITimer>& timer = IApplicationManager::instance()->getTimer();
            //	f64 t = (f64)timer->getTimeMilliseconds() / 1000.0;
            //	f64 dt = (f64)timer->getTimeIntervalMilliseconds() / 1000.0;
            //	m_animCtrl->update(0, t, dt);
            // }
            else if( hashId == StringUtil::getHash( "stopAllAnimations" ) )
            {
                m_animCtrl->stopAllAnimations();
            }

            return 0;
        }

        CAnimationController::AnimationControllerStateListener::AnimationControllerStateListener(
            CAnimationController *owner ) :
            m_owner( owner )
        {
        }

        CAnimationController::AnimationControllerStateListener::~AnimationControllerStateListener()
        {
        }

        //--------------------------------------------
        void CAnimationController::AnimationControllerStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            // if(message->isExactly(StateMessageStop::TYPE_INFO))
            //{
            //	StateMessageStopPtr messageStop = message;
            //	u32 type = messageStop->getType();

            //	if(type == CAnimationController::STOP_ALL_ANIMATIONS_HASH)
            //	{
            //		m_owner->stopAllAnimations();
            //	}
            //}
            // else if(message->isExactly(StateMessageAnimationEnable::TYPE_INFO))
            //{
            //	StateMessageAnimationEnablePtr animationMessage = message;
            //	u32 type = animationMessage->getType();

            //	if(type == CAnimationController::SET_ANIMATION_ENABLED_HASH)
            //	{
            //		m_owner->setAnimationEnabled(animationMessage->getName(),
            // animationMessage->getEnabled());
            //	}
            //	else if(type == CAnimationController::SET_ANIMATION_ENABLED_TIME_HASH)
            //	{
            //		m_owner->setAnimationEnabled(animationMessage->getName(),
            // animationMessage->getEnabled(), animationMessage->getTime());
            //	}
            //}
        }

        void CAnimationController::AnimationControllerStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        //--------------------------------------------
        void CAnimationController::AnimationControllerStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }

        CAnimationController::Animation::Animation( CAnimationController *controller ) :
            m_controller( controller )
        {
        }

        Ogre::AnimationState *CAnimationController::Animation::getAnimationState() const
        {
            return m_animationState;
        }

        void CAnimationController::Animation::setAnimationState( Ogre::AnimationState *val )
        {
            m_animationState = val;
        }

        fb::f32 CAnimationController::Animation::getLength() const
        {
            return m_length;
        }

        void CAnimationController::Animation::setLength( f32 val )
        {
            m_length = val;
        }

        bool CAnimationController::Animation::isLooping() const
        {
            return m_isLooping;
        }

        void CAnimationController::Animation::setLooping( bool val )
        {
            m_isLooping = val;
        }

        bool CAnimationController::Animation::isEnabled() const
        {
            return m_isEnabled;
        }

        void CAnimationController::Animation::setEnabled( bool val )
        {
            m_isEnabled = val;
        }

    }  // namespace render
}  // end namespace fb
