#ifndef Smooth_h__
#define Smooth_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <deque>

namespace fb
{
    template <class T>
    struct SmoothValue
    {
        SmoothValue( time_interval time, T value ) : time( time ), value( value )
        {
        }

        time_interval time;
        T value;
    };

    template <class T>
    class Smooth : public CSharedObject<ISharedObject>
    {
    public:
        Smooth() : m_interval( static_cast<time_interval>( 1.0 ) )
        {
        }

        ~Smooth() override
        {
        }

        T getValue( const SmoothValue<T> &value )
        {
            m_values.push_back( value );

            if( m_values.size() == 1 )
            {
                return T();
            }

            typename std::deque<SmoothValue<T>>::iterator it = m_values.begin(),
                                                          end = m_values.end() - 2;
            while( it != end )
            {
                if( value.time - ( *it ).time > m_interval )
                    ++it;
                else
                    break;
            }

            m_values.erase( m_values.begin(), it );

            return ( m_values.back().value - m_values.front().value ) / ( m_values.size() - 1 );
        }

        time_interval getInterval() const
        {
            return m_interval;
        }

        void setInterval( time_interval val )
        {
            m_interval = val;
        }

    protected:
        time_interval m_interval;
        std::deque<SmoothValue<T>> m_values;
    };
}  // end namespace fb

#endif  // Smooth_h__
