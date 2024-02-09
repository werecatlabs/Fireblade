#ifndef _PositionAnimator3_H
#define _PositionAnimator3_H

#include "FBCore/Animation/BaseAnimator.h"
#include <FBCore/Math/Vector3.h>

namespace fb
{

    template <class T>
    class PositionAnimator3 : public BaseAnimator
    {
    public:
        PositionAnimator3();

        PositionAnimator3( T *object, f32 animationLength, const Vector3<real_Num> &startScale = Vector3<real_Num>::unit(),
                           const Vector3<real_Num> &endScale = Vector3<real_Num>::unit() );

        ~PositionAnimator3();

        void update();

        void start();

        bool isFinished() const;

    protected:
        Vector3<real_Num> m_start;
        Vector3<real_Num> m_end;

        T *m_object;
    };

    template <class T>
    PositionAnimator3<T>::PositionAnimator3() :
        m_start( Vector3<real_Num>::unit() ),
        m_end( Vector3<real_Num>::unit() ),
        m_object( NULL )
    {
    }

    template <class T>
    PositionAnimator3<T>::PositionAnimator3( T *object, f32 animationLength,
                                             const Vector3<real_Num> &startScale = Vector3<real_Num>::unit(),
                                             const Vector3<real_Num> &endScale = Vector3<real_Num>::unit() ) :
        m_start( startScale ),
        m_end( endScale ),
        m_object( object ),
        m_animationLength( animationLength ),
        m_animationTime( 0.0f )
    {
    }

    template <class T>
    PositionAnimator3<T>::~PositionAnimator3()
    {
    }

    template <class T>
    void PositionAnimator3<T>::update()
    {
        if( m_isPlaying )
        {
            m_animationTime += dt * ( 1.0f / m_animationLength );
            m_animationTime = MathF::clamp( m_animationTime, 0.0f, 1.0f );
            Vector3<real_Num> position = m_start + ( m_end - m_start ) * m_animationTime;
            m_object->setPosition( position );
        }
    }

    template <class T>
    void PositionAnimator3<T>::start()
    {
        BaseAnimator::start();
        m_animationTime = 0.0f;
    }

    template <class T>
    bool PositionAnimator3<T>::isFinished() const
    {
        return m_animationTime >= ( 1.0 - MathF::epsilon() );
    }

}  // end namespace fb

#endif
