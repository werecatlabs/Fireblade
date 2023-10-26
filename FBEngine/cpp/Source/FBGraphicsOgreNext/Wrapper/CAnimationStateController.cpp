#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Wrapper/CAnimationStateController.h"
#include <FBCore/FBCoreHeaders.h>
#include "FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h"
#include <OgreAnimationState.h>
#include <OgreSceneManager.h>

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        CAnimationStateController::CAnimationStateController( SmartPtr<render::IGraphicsScene> creator ) :
            m_creator( creator )
        {
        }

        //--------------------------------------------
        CAnimationStateController::~CAnimationStateController()
        {
            Ogre::SceneManager *ogreSceneMgr = NULL;
            m_creator->_getObject( (void **)&ogreSceneMgr );

            for( u32 i = 0; i < m_animationStates.size(); ++i )
            {
                Ogre::v1::AnimationState *animState = m_animationStates[i];
                animState->setEnabled( false );

                ogreSceneMgr->destroyAnimationState( animState->getAnimationName() );
            }

            //auto engine = core::IApplicationManager::instance();
            //SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
            //graphicsSystem->removeFrameListener( m_frameListener );
        }

        //--------------------------------------------
        void CAnimationStateController::update( const s32 &task, const time_interval &t,
                                                const time_interval &dt )
        {
            if( Thread::getCurrentTask() != Thread::Task::Render )
                return;

            for( u32 i = 0; i < m_animationStates.size(); ++i )
            {
                Ogre::v1::AnimationState *animState = m_animationStates[i];

                bool isReversed =
                    false;  // m_reversedAnimationStates.find_element_index(animState) != -1;

                if( animState->getEnabled() && !animState->getLoop() )
                {
                    if( isReversed && animState->getTimePosition() == 0.0f )
                    {
                        animState->setEnabled( false );
                    }
                    else if( animState->hasEnded() )
                    {
                        animState->setEnabled( false );
                    }
                }

                if( animState->getEnabled() )
                {
                    if( !isReversed )
                        animState->addTime( dt );
                    else
                        animState->addTime( -dt );
                }
            }
        }

        //--------------------------------------------
        bool CAnimationStateController::setAnimationEnabled( const String &animationName, bool enabled )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::setAnimationEnabled: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* animState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( animState )
            //{
            //	if(m_animationStates.linear_search(animState) == -1)
            //	{
            //		m_animationStates.push_back(animState);
            //	}

            //	animState->setEnabled(enabled);
            //	return true;
            //}
            //}

            return false;
        }

        //--------------------------------------------
        bool CAnimationStateController::setAnimationEnabled( const String &animationName, bool enabled,
                                                             f32 timePosition )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::hasAnimationEnded: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* animState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( animState )
            //{
            //	if(m_animationStates.linear_search(animState) == -1)
            //	{
            //		m_animationStates.push_back(animState);
            //	}

            //	animState->setEnabled(enabled);
            //	animState->setTimePosition(timePosition);
            //	return true;
            //}

            return false;
        }

        //--------------------------------------------
        bool CAnimationStateController::isAnimationEnabled( const String &animationName )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::setAnimationEnabled: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* animState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( animState )
            //{
            //	return animState->getEnabled();
            // }

            return false;
        }

        //--------------------------------------------
        void CAnimationStateController::stopAllAnimations()
        {
            //

            // for(u32 i=0; i<m_animationStates.size(); ++i)
            //{
            //	Ogre::AnimationState* animState = m_animationStates[i];
            //	animState->setEnabled(false);
            // }

            // m_animationStates.clear();
        }

        //--------------------------------------------
        bool CAnimationStateController::hasAnimationEnded( const String &animationName )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::hasAnimationEnded: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* pAnimState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( pAnimState && pAnimState->hasEnded() )
            //{
            //	return true;
            // }

            return false;
        }

        //--------------------------------------------
        bool CAnimationStateController::hasAnimation( const String &animationName ) const
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // return ogreSceneMgr->hasAnimationState(animationName.c_str());

            return false;
        }

        //--------------------------------------------
        void CAnimationStateController::setAnimationLoop( const String &animationName, bool loop )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::setAnimationLoop: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return;
            // }

            // Ogre::AnimationState* pAnimState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( pAnimState )
            //{
            //	pAnimState->setLoop(loop);
            // }
        }

        //--------------------------------------------
        bool CAnimationStateController::isAnimationLooping( const String &animationName )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::getAnimationLoop: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* pAnimState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( pAnimState && pAnimState->getLoop() )
            //{
            //	return true;
            // }

            return false;
        }

        //--------------------------------------------
        void CAnimationStateController::setAnimationReversed( const String &animationName,
                                                              bool reversed )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::getAnimationLoop: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return;
            // }

            // Ogre::AnimationState* animState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if(reversed)
            //{
            //	m_reversedAnimationStates.push_back(animState);
            // }
            // else
            //{
            //	m_reversedAnimationStates.erase_element(animState);
            // }
        }

        //--------------------------------------------
        bool CAnimationStateController::isAnimationReversed( const String &animationName )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::getAnimationLoop: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* animState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // return m_reversedAnimationStates.linear_search(animState) != -1;

            return false;
        }

        //--------------------------------------------
        bool CAnimationStateController::setTimePosition( const String &animationName, f32 timePosition )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::setTimePosition: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* pAnimState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( pAnimState )
            //{
            //	pAnimState->setTimePosition(timePosition);
            //	return true;
            // }

            return false;
        }

        //--------------------------------------------
        f32 CAnimationStateController::getTimePosition( const String &animationName )
        {
            //

            // Ogre::SceneManager* ogreSceneMgr = NULL;
            // m_creator->_getObject((void**)&ogreSceneMgr);

            // if(!ogreSceneMgr->hasAnimationState(animationName.c_str()))
            //{
            //	String msg = String("CAnimationStateController::setTimePosition: Animation not found.
            // animation: ") + animationName; 	LOG_MESSAGE("Graphics", msg.c_str()); 	return false;
            // }

            // Ogre::AnimationState* pAnimState = ogreSceneMgr->getAnimationState(animationName.c_str());
            // if( pAnimState )
            //{
            //	return pAnimState->getTimePosition();
            // }

            return 0.0f;
        }

        //--------------------------------------------
        CAnimationStateController::Animation::Animation( CAnimationStateController *controller ) :
            m_controller( controller )
        {
        }

    }  // namespace render
}  // end namespace fb
