#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Core/LookupTable.h>
#include <FBCore/Math/Math.h>

namespace fb
{
    LookupTable::LookupTable()
    {
    }

    LookupTable::~LookupTable()
    {
    }

    void LookupTable::setPoints( const Array<std::pair<real_Num, real_Num>> &points )
    {
        m_points = points;
    }

    void LookupTable::setPoints( const Array<real_Num> &keys, const Array<real_Num> &points )
    {
    }

    void LookupTable::setPoints( const Array<real_Num> &points )
    {
    }

    real_Num LookupTable::interpolate( real_Num t )
    {
        size_t p0_ = 0;
        size_t p1_ = 0;

        const auto numServoPoints = m_points.size();
        for( size_t pointIdx = 0; pointIdx < numServoPoints; ++pointIdx )
        {
            auto &p = m_points[pointIdx];

            if( t < p.first )
            {
                p1_ = pointIdx;
                p0_ = pointIdx - 1;
                break;
            }
        }

        auto p0 = static_cast<size_t>( Math<s32>::clamp( (s32)p0_, 0, (s32)m_points.size() ) );
        auto p1 = static_cast<size_t>( Math<s32>::clamp( (s32)p1_, 0, (s32)m_points.size() ) );

        auto &f0 = m_points[p0];
        auto &f1 = m_points[p1];

        if( p0 != p1 )
        {
            auto delta = ( t - f0.first ) / ( f1.first - f0.first );
            return f0.second + ( f1.second - f0.second ) * delta;
        }

        return f0.second;
    }
}  // namespace fb
