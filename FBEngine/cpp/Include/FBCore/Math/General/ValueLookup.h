#ifndef FBMath_ValueLookup_h__
#define FBMath_ValueLookup_h__

#include <FBCore/Base/Array.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    class ValueLookupTypes
    {
    public:
        enum
        {
            VALUE_TYPE_FIXED,
            VALUE_TYPE_RANDOM,
        };
    };

    template <class T>
    class ValueLookup
    {
    protected:
        u32 m_type;
        u32 m_seed;
        Array<T> m_values;

    public:
        ValueLookup() : m_type( ValueLookupTypes::VALUE_TYPE_FIXED ), m_seed( 0 )
        {
        }

        ~ValueLookup()
        {
        }

        virtual T getValue() = 0;

        u32 getType() const
        {
            return m_type;
        }

        void setType( u32 val )
        {
            m_type = val;
        }

        void setMin( const T &minValue )
        {
            m_values[0] = minValue;
        }

        void setMax( const T &maxValue )
        {
            m_values[1] = maxValue;
        }

        u32 getSeed() const
        {
            return m_seed;
        }

        void setSeed( u32 val )
        {
            m_seed = val;
        }
    };

    template <class T>
    class ValueLookupNumber
    {
    protected:
        u32 m_type;
        u32 m_seed;
        Array<T> m_values;

    public:
        ValueLookupNumber()
        {
            m_values.set_used( 2 );
            m_values[0] = T( 0 );
            m_values[1] = T( 0 );
        }

        T getValue()
        {
            switch( m_type )
            {
            case ValueLookupTypes::VALUE_TYPE_FIXED:
            {
                return m_values[0];
            }
            break;
            case ValueLookupTypes::VALUE_TYPE_RANDOM:
            {
                const T &minVal = m_values[0];
                const T &maxVal = m_values[1];

                return Math<T>::RangedRandom( minVal, maxVal, m_seed++ );
            }
            break;
            default:
            {
            }
            }

            return 0;
        }

        u32 getType() const
        {
            return m_type;
        }

        void setType( u32 val )
        {
            m_type = val;
        }

        void setMin( const T &minValue )
        {
            m_values[0] = minValue;
        }

        void setMax( const T &maxValue )
        {
            m_values[1] = maxValue;
        }

        u32 getSeed() const
        {
            return m_seed;
        }

        void setSeed( u32 val )
        {
            m_seed = val;
        }
    };

    using ValueLookupNumberI = ValueLookupNumber<s32>;
    using ValueLookupNumberF = ValueLookupNumber<f32>;

    template <class T>
    class ValueLookupVector3
    {
    protected:
        u32 m_type;
        u32 m_seed;
        Array<T> m_values;

    public:
        ValueLookupVector3()
        {
            m_values.set_used( 2 );
            m_values[0] = Vector3<T>::ZERO;
            m_values[1] = Vector3<T>::ZERO;
        }

        ~ValueLookupVector3()
        {
        }

        Vector3<T> getValue()
        {
            switch( m_type )
            {
            case ValueLookupTypes::VALUE_TYPE_FIXED:
            {
                return m_values[0];
            }
            break;
            case ValueLookupTypes::VALUE_TYPE_RANDOM:
            {
                const Vector3<T> &minVal = m_values[0];
                const Vector3<T> &maxVal = m_values[1];

                return Vector3<T>( Math<T>::RangedRandom( minVal.X(), maxVal.X(), m_seed++ ),
                                   Math<T>::RangedRandom( minVal.Y(), maxVal.Y(), m_seed++ ),
                                   Math<T>::RangedRandom( minVal.Z(), maxVal.Z(), m_seed++ ) );
            }
            break;
            default:
            {
            }
            }

            return Vector3<T>::ZERO;
        }

        u32 getType() const
        {
            return m_type;
        }

        void setType( u32 val )
        {
            m_type = val;
        }

        void setMin( const T &minValue )
        {
            m_values[0] = minValue;
        }

        void setMax( const T &maxValue )
        {
            m_values[1] = maxValue;
        }

        u32 getSeed() const
        {
            return m_seed;
        }

        void setSeed( u32 val )
        {
            m_seed = val;
        }
    };

    using ValueLookupVector3I = ValueLookupVector3<s32>;
    using ValueLookupVector3F = ValueLookupVector3<f32>;
}  // end namespace fb

#endif  // ParticleParam_h__
