#ifndef _CAnimationController_H_
#define _CAnimationController_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include "FBCore/Interface/Graphics/IAnimationController.h"
#include "FBCore/Interface/Script/IScriptReceiver.h"
#include "FBCore/Interface/System/IStateListener.h"
#include "FBCore/Interface/System/IState.h"
#include "FBCore/Interface/System/IStateQuery.h"
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/HashMap.h>

namespace fb
{
    namespace render
    {

        class CAnimationController : public IAnimationController
        {
        public:
            static const u32 SET_ANIMATION_ENABLED_HASH;
            static const u32 SET_ANIMATION_ENABLED_TIME_HASH;
            static const u32 IS_ANIMATION_ENABLED_HASH;
            static const u32 SET_ANIMATION_LOOP_HASH;

            static const u32 STOP_ALL_ANIMATIONS_HASH;

            CAnimationController();
            ~CAnimationController();

            void Initialize( Ogre::v1::Entity *entity );

            void update( const s32 &task, const time_interval &t, const time_interval &dt );

            bool setAnimationEnabled( const String &animationName, bool enabled );
            bool setAnimationEnabled( const String &animationName, bool enabled, f32 timePosition );

            bool _setAnimationEnabled( const String &animationName, bool enabled, f32 timePosition );

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

            f32 getAnimationLength( const String &animationName ) const;

            void addListener( IAnimationControllerListener *listener );
            void removeListener( IAnimationControllerListener *listener );

            SmartPtr<IStateContext> &getStateContext();
            const SmartPtr<IStateContext> &getStateContext() const;
            void setStateContext( SmartPtr<IStateContext> subject );

        private:
            class ScriptReceiver : public IScriptReceiver
            {
            public:
                ScriptReceiver( IAnimationController *animCtrl );

                s32 callFunction( u32 hashId, const Parameters &params, Parameters &results );

            protected:
                IAnimationController *m_animCtrl;
            };

            class AnimationControllerStateListener : public IStateListener
            {
            public:
                AnimationControllerStateListener( CAnimationController *owner );
                ~AnimationControllerStateListener();

                void handleStateChanged( const SmartPtr<IStateMessage> &message );
                void handleStateChanged( SmartPtr<IState> &state );
                void handleQuery( SmartPtr<IStateQuery> &query );

            protected:
                CAnimationController *m_owner;
            };

            class Animation : public ISharedObject
            {
            public:
                Animation( CAnimationController *controller );

                Ogre::AnimationState *getAnimationState() const;
                void setAnimationState( Ogre::AnimationState *val );

                f32 getLength() const;
                void setLength( f32 val );

                bool isLooping() const;
                void setLooping( bool val );

                bool isEnabled() const;
                void setEnabled( bool val );

            protected:
                CAnimationController *m_controller;
                Ogre::AnimationState *m_animationState;
                f32 m_length;
                bool m_isLooping;
                bool m_isEnabled;
            };

            void _update( f64 dt );
            bool _setAnimationEnabled( const String &animationName, bool enabled );

            void addAnimation( Ogre::v1::AnimationState *pAnimState );
            SmartPtr<IAnimation> findAnimation( const String &animationName ) const;

            SmartPtr<IStateContext> m_stateContext;
            SmartPtr<IStateListener> m_stateListener;
            Ogre::v1::Entity *m_entity;

            typedef HashMap<hash32, SmartPtr<IAnimation>> Animations;
            Animations m_animations;

            Array<IAnimationControllerListener *> m_listeners;

            mutable SpinRWMutex Mutex;
        };

        typedef SmartPtr<CAnimationController> CAnimationControllerPtr;

    }  // namespace render
}  // end namespace fb

#endif
