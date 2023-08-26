#ifndef _PositionAnimator2_H
#define _PositionAnimator2_H

#include "FBCore/Animation/BaseAnimator.h"
#include "FBCore/Math/Vector2.h"

namespace fb
{

    template <class T>
    class PositionAnimator2 : public BaseAnimator
    {
    public:
        PositionAnimator2() :
            m_start( Vector2F::UNIT ),
            m_end( Vector2F::UNIT ),
            m_object( NULL ),
            m_animationLength( 1.0f ),
            m_animationTime( 0.0f )
        {
        }

        PositionAnimator2( T *object, f32 animationLength, const Vector2F &start = Vector2F::UNIT,
                           const Vector2F &end = Vector2F::UNIT ) :
            m_start( start ),
            m_end( end ),
            m_object( object )
        {
            setAnimationTime( 0.f );
            setAnimationLength( animationLength );
        }

        ~PositionAnimator2()
        {
        }

        /** */
        void update()
        {
            if( m_isPlaying )
            {
                m_animationTime += dt * ( 1.0f / m_animationLength );
                m_animationTime = MathF::clamp( m_animationTime, 0.0f, 1.0f );
                if( m_loop && m_animationTime == 1.0f )
                    m_animationTime = 0.0f;

                Vector2F position;
                if( !m_reverse )
                    position = m_start + ( m_end - m_start ) * m_animationTime;
                else
                    position = m_end + ( m_start - m_end ) * m_animationTime;

                m_object->setPosition( position );
            }
        }

        /** */
        void start()
        {
            BaseAnimator::start();
            m_animationTime = 0.0f;
        }

        /** */
        bool isFinished() const
        {
            return m_animationTime >= ( 1.0 - MathF::epsilon() );
        }

        /** */
        void setStart( const Vector2F &start )
        {
            m_start = start;
        }

        /** */
        Vector2F getStart() const
        {
            return m_start;
        }

        /** */
        void setEnd( const Vector2F &end )
        {
            m_end = end;
        }

        /** */
        Vector2F getEnd() const
        {
            return m_end;
        }

    protected:
        ///
        Vector2F m_start;

        ///
        Vector2F m_end;

        ///
        T *m_object;
    };

}  // end namespace fb

#endif
