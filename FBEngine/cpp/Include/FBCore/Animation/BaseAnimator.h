#ifndef _BaseAnimator_H
#define _BaseAnimator_H

#include <FBCore/Interface/Animation/IAnimator.h>

namespace fb
{
    
    class BaseAnimator : public IAnimator
    {
    public:
        static const hash_type START_HASH;
        static const hash_type STOP_HASH;

        BaseAnimator();
        virtual ~BaseAnimator();

        virtual void update();

        virtual void setId( u32 id );
        virtual u32 getId() const;

        virtual void start();
        virtual void stop();

        bool isPlaying() const;

        virtual void setLoop( bool loop );
        virtual bool isLoop() const;

        virtual void setReverse( bool reverse );
        virtual bool isReverse() const;

        virtual bool isFinished() const;

        virtual void setAnimationLength( f32 animationLength );

        virtual f32 getAnimationLength() const;

        virtual f32 getAnimationTime() const;

        virtual void setAnimationTime( f32 animationTime );

    protected:
        u32 m_id;

        f32 m_animationLength;
        f32 m_animationTime;

        bool m_isPlaying;
        bool m_loop;
        bool m_reverse;

        static u32 m_idExt;
    };

}  // end namespace fb

#endif
