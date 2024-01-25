#ifndef _PositionAnimator2_H
#define _PositionAnimator2_H

#include <FBCore/Animation/BaseAnimator.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    template <class T>
    class PositionAnimator2 : public BaseAnimator
    {
    public:
        PositionAnimator2();

        PositionAnimator2( T *object, f32 animationLength, const Vector2F &start = Vector2F::UNIT,
                           const Vector2F &end = Vector2F::UNIT );

        ~PositionAnimator2();

        /** */
        void update();

        /** */
        void start();

        /** */
        bool isFinished() const;

        /** */
        void setStart( const Vector2F &start );

        /** */
        Vector2F getStart() const;

        /** */
        void setEnd( const Vector2F &end );

        /** */
        Vector2F getEnd() const;

    protected:
        ///
        Vector2F m_start;

        ///
        Vector2F m_end;

        ///
        T *m_object;
    };

    template <class T>
    PositionAnimator2<T>::PositionAnimator2() :
        m_start( Vector2F::UNIT ),
        m_end( Vector2F::UNIT ),
        m_object( NULL ),
        m_animationLength( 1.0f ),
        m_animationTime( 0.0f )
    {
    }

    template <class T>
    PositionAnimator2<T>::PositionAnimator2( T *object, f32 animationLength,
                                             const Vector2F &start = Vector2F::UNIT,
                                             const Vector2F &end = Vector2F::UNIT ) :
        m_start( start ),
        m_end( end ),
        m_object( object )
    {
        setAnimationTime( 0.f );
        setAnimationLength( animationLength );
    }

    template <class T>
    PositionAnimator2<T>::~PositionAnimator2()
    {
    }

    template <class T>
    void PositionAnimator2<T>::update()
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

    template <class T>
    void PositionAnimator2<T>::start()
    {
        BaseAnimator::start();
        m_animationTime = 0.0f;
    }

    template <class T>
    bool PositionAnimator2<T>::isFinished() const
    {
        return m_animationTime >= ( 1.0 - MathF::epsilon() );
    }

    template <class T>
    void PositionAnimator2<T>::setStart( const Vector2F &start )
    {
        m_start = start;
    }
    template <class T> /** */
    Vector2F PositionAnimator2<T>::getStart() const
    {
        return m_start;
    }

    template <class T>
    void PositionAnimator2<T>::setEnd( const Vector2F &end )
    {
        m_end = end;
    }

    template <class T>
    Vector2F PositionAnimator2<T>::getEnd() const
    {
        return m_end;
    }

}  // end namespace fb

#endif
