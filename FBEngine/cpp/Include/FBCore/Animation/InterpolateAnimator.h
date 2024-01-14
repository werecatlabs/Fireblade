#ifndef _InterpolateAnimator_H
#define _InterpolateAnimator_H

#include "FBCore/Animation/BaseAnimator.h"
#include "FBCore/Math/Vector2.h"
#include <FBCore/Math/Vector3.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{

    /** An animator to interpolate a value.
     */
    template <class T>
    class InterpolateAnimator : public BaseAnimator
    {
    public:
        /** Default Constructor. */
        InterpolateAnimator() : BaseAnimator()
        {
        }

        /** Constructor.
        @param
            animationLength The animation length in seconds.
        @param
            start The start value.
        @param
            end The end value.
        */
        InterpolateAnimator( f32 animationLength, const T &start, const T &end ) :
            m_start( start ),
            m_end( end )
        {
            setAnimationLength( animationLength );
            setAnimationTime( 0.0f );
        }

        /** Destructor. */
        ~InterpolateAnimator()
        {
        }

        /** */
        void update()
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            if( m_isPlaying )
            {
                m_animationTime += (f32)dt * ( 1.0f / m_animationLength );
                m_animationTime = MathF::clamp( m_animationTime, 0.0f, 1.0f );

                if( !m_reverse )
                    m_value = m_start + ( m_end - m_start ) * m_animationTime;
                else
                    m_value = m_end + ( m_start - m_end ) * m_animationTime;
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

        void setStartValue( const T &start )
        {
            m_start = start;
        }

        T getStartValue() const
        {
            return m_start;
        }

        void setEndValue( const T &end )
        {
            m_end = end;
        }

        T getEndValue() const
        {
            return m_end;
        }

        T getValue() const
        {
            return m_value;
        }

        void setReverse( bool reverse )
        {
            m_reverse = reverse;
            m_animationTime = 0.0;
        }

    protected:
        T m_start;
        T m_end;
        T m_value;
    };

    typedef InterpolateAnimator<f32> InterpolateFloat;
    typedef InterpolateAnimator<Vector2F> InterpolateVector2f;
    typedef InterpolateAnimator<Vector3F> InterpolateVector3f;

    typedef SmartPtr<InterpolateFloat> InterpolateFloatPtr;
    typedef SmartPtr<InterpolateVector2f> InterpolateVector2fPtr;
    typedef SmartPtr<InterpolateVector3f> InterpolateVector3fPtr;

}  // end namespace fb

#endif
