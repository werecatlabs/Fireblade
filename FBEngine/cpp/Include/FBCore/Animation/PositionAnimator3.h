#ifndef _PositionAnimator3_H
#define _PositionAnimator3_H

#include "FBCore/Animation/BaseAnimator.h"
#include <FBCore/Math/Vector3.h>

namespace fb
{

    //---------------------------------------------
    template <class T>
    class PositionAnimator3 : public BaseAnimator
    {
    public:
        PositionAnimator3() : m_start( Vector3F::unit() ), m_end( Vector3F::unit() ), m_object( NULL )
        {
        }

        PositionAnimator3( T *object, f32 animationLength, const Vector3F &startScale = Vector3F::unit(),
                           const Vector3F &endScale = Vector3F::unit() ) :
            m_start( startScale ),
            m_end( endScale ),
            m_object( object ),
            m_animationLength( animationLength ),
            m_animationTime( 0.0f )
        {
            setAnimationTime( 0.f );
            setAnimationLength( animationLength );
        }

        ~PositionAnimator3()
        {
        }

        void update()
        {
            if( m_isPlaying )
            {
                m_animationTime += dt * ( 1.0f / m_animationLength );
                m_animationTime = MathF::clamp( m_animationTime, 0.0f, 1.0f );
                Vector3F position = m_start + ( m_end - m_start ) * m_animationTime;
                m_object->setPosition( position );
            }
        }

        void start()
        {
            BaseAnimator::start();
            m_animationTime = 0.0f;
        }

        bool isFinished() const
        {
            return m_animationTime >= ( 1.0 - MathF::epsilon() );
        }

    protected:
        Vector3F m_start;
        Vector3F m_end;

        T *m_object;
    };

}  // end namespace fb

#endif
