#ifndef _CAnimationStateController_H_
#define _CAnimationStateController_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Interface/Graphics/IAnimationStateController.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/HashMap.h>

namespace fb
{
    namespace render
    {

        class CAnimationStateController : public CSharedObject<IAnimationStateController>
        {
        public:
            CAnimationStateController( SmartPtr<IGraphicsScene> creator );
            ~CAnimationStateController() override;

            void update() override;

            bool setAnimationEnabled( const String &animationName, bool enabled ) override;
            bool setAnimationEnabled( const String &animationName, bool enabled,
                                      f32 timePosition ) override;
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

        private:
            class Animation : public CSharedObject<ISharedObject>
            {
            public:
                Animation( CAnimationStateController *controller );

                CAnimationStateController *m_controller = nullptr;
            };

            SmartPtr<IGraphicsScene> m_creator;
            Array<Ogre::AnimationState *> m_animationStates;
            Array<Ogre::AnimationState *> m_reversedAnimationStates;

            using Animations = HashMap<hash32, SmartPtr<IAnimation>>;
            Animations m_animations;
        };

        using CAnimationStateControllerPtr = SmartPtr<CAnimationStateController>;
    }  // end namespace render
}  // end namespace fb

#endif
