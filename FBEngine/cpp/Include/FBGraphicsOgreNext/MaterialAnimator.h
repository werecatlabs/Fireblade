#ifndef MaterialAnimator_h__
#define MaterialAnimator_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Animation/IAnimator.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class MaterialAnimator : public IAnimator
        {
        public:
            MaterialAnimator();
            ~MaterialAnimator();

            void setId( u32 id );
            u32 getId() const;

            void start();
            void stop();

            void setLoop( bool loop );
            bool isLoop() const;

            void setReverse( bool reverse );
            bool isReverse() const;
            bool isFinished() const;

            void setAnimationLength( f32 animationLength );

            IMaterial *getOwner() const;
            void setOwner( IMaterial *val );

        protected:
            IMaterial *m_owner = nullptr;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // MaterialAnimator_h__
