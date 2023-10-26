#ifndef _CAnimationStateController_H_
#define _CAnimationStateController_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include "FBCore/Interface/Graphics/IGraphicsScene.h"
#include "FBCore/Interface/Graphics/IGraphicsObject.h"
#include "FBCore/Interface/Graphics/IAnimationStateController.h"
#include "FBCore/Interface/Script/IScriptReceiver.h"
#include "FBCore/Interface/System/IStateListener.h"
#include "FBCore/Interface/System/IState.h"
#include "FBCore/Interface/System/IStateQuery.h"
#include <FBCore/Interface/Memory/ISharedObject.h>
#include "FBCore/Core/HashMap.h"

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        class CAnimationStateController : public IAnimationStateController
        {
        public:
            CAnimationStateController( SmartPtr<render::IGraphicsScene> creator );
            ~CAnimationStateController();

            void update( const s32 &task, const time_interval &t, const time_interval &dt );

            bool setAnimationEnabled( const String &animationName, bool enabled );
            bool setAnimationEnabled( const String &animationName, bool enabled, f32 timePosition );
            bool isAnimationEnabled( const String &animationName );
            void stopAllAnimations();

            bool hasAnimationEnded( const String &animationName );

            bool hasAnimation( const String &animationName ) const;

            void setAnimationLoop( const String &animationName, bool loop );
            bool isAnimationLooping( const String &animationName );

            void setAnimationReversed( const String &animationName, bool reversed );
            bool isAnimationReversed( const String &animationName );

            bool setTimePosition( const String &animationName, f32 timePosition );
            f32 getTimePosition( const String &animationName );

        private:

            class Animation : public ISharedObject
            {
            public:
                Animation( CAnimationStateController *controller );

                CAnimationStateController *m_controller;
            };

            SmartPtr<render::IGraphicsScene> m_creator;
            Array<Ogre::v1::AnimationState *> m_animationStates;
            Array<Ogre::v1::AnimationState *> m_reversedAnimationStates;

            typedef HashMap<hash32, SmartPtr<IAnimation>> Animations;
            Animations m_animations;
        };

        typedef SmartPtr<CAnimationStateController> CAnimationStateControllerPtr;

    }  // namespace render
}  // end namespace fb

#endif
