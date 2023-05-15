#ifndef _CAnimationController_H_
#define _CAnimationController_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IAnimationController.h>
#include <FBCore/Interface/Script/IScriptReceiver.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Base/HashMap.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {

        class CAnimationController : public CSharedObject<IAnimationController>
        {
        public:
            static const hash_type SET_ANIMATION_ENABLED_HASH;
            static const hash_type SET_ANIMATION_ENABLED_TIME_HASH;
            static const hash_type IS_ANIMATION_ENABLED_HASH;
            static const hash_type SET_ANIMATION_LOOP_HASH;
            static const hash_type STOP_ALL_ANIMATIONS_HASH;
            static const hash_type FRAME_EVENT_RENDER_QUEUED;

            CAnimationController();
            ~CAnimationController() override;

            void Initialize( Ogre::Entity *entity );

            void update() override;

            bool setAnimationEnabled( const String &animationName, bool enabled ) override;
            bool setAnimationEnabled( const String &animationName, bool enabled,
                                      f32 timePosition ) override;

            bool _setAnimationEnabled( const String &animationName, bool enabled, f32 timePosition );

            bool isAnimationEnabled( const String &animationName ) override;
            void stopAllAnimations() override;

            bool hasAnimationEnded( const String &animationName ) override;

            bool hasAnimation( const String &animationName ) const override;

            void setAnimationLoop( const String &animationName, bool loop ) override;
            bool isAnimationLooping( const String &animationName ) override;

            void setAnimationReversed( const String &animationName, bool reversed ) override;
            bool isAnimationReversed( const String &animationName ) override;

            bool setTimePosition( const String &animationName, f32 timePosition ) override;
            f32 getTimePosition( const String &animationName ) override;

            f32 getAnimationLength( const String &animationName ) const override;

            void addListener( IAnimationControllerListener *listener ) ;
            void removeListener( IAnimationControllerListener *listener ) ;

            SmartPtr<IStateContext> &getStateObject();
            const SmartPtr<IStateContext> &getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> subject );

        private:
            class ScriptReceiver : public CSharedObject<IScriptReceiver>
            {
            public:
                ScriptReceiver( IAnimationController *animCtrl );

                s32 callFunction( hash_type hashId, const Parameters &params, Parameters &results ) override;

            protected:
                IAnimationController *m_animCtrl;
            };

            class AnimationControllerStateListener : public CSharedObject<IStateListener>
            {
            public:
                AnimationControllerStateListener( CAnimationController *owner );
                ~AnimationControllerStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

            protected:
                CAnimationController *m_owner;
            };

            class Animation : public CSharedObject<ISharedObject>
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
                CAnimationController *m_controller = nullptr;
                Ogre::AnimationState *m_animationState = nullptr;
                f32 m_length;
                bool m_isLooping;
                bool m_isEnabled;
            };

            void _update( f64 dt );
            bool _setAnimationEnabled( const String &animationName, bool enabled );

            void addAnimation( Ogre::AnimationState *pAnimState );
            SmartPtr<IAnimation> findAnimation( const String &animationName ) const;

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;
            Ogre::Entity *m_entity;

            using Animations = HashMap<hash32, SmartPtr<IAnimation>>;
            Animations m_animations;

            Array<IAnimationControllerListener *> m_listeners;

            SpinRWMutex Mutex;
        };
    }  // end namespace render
}  // end namespace fb

#endif
