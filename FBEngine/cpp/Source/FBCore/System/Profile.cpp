#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Profile.h>
#include <FBCore/Core/Util.h>
#include <chrono>

namespace fb
{
    const s32 Profile::DEFAULT_NUM_SAMPLES = 10;
    u32 Profile::m_idExt = 0;

    Profile::Profile()
    {
        auto id = m_idExt++;

        if( auto handle = getHandle() )
        {
            handle->setId( id );
        }
    }

    Profile::~Profile() = default;

    void Profile::start()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        m_start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }

    void Profile::end()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        m_end = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        auto interval = m_end - m_start;
        m_timeTaken =
            static_cast<float>( static_cast<f64>( interval ) / static_cast<f64>( 1000000000LL ) );

        m_averageTimeTaken.push_back( m_timeTaken );

        if( m_averageTimeTaken.size() > DEFAULT_NUM_SAMPLES )
        {
            m_averageTimeTaken.erase(
                m_averageTimeTaken.begin(),
                m_averageTimeTaken.begin() + ( m_averageTimeTaken.size() - DEFAULT_NUM_SAMPLES ) );
        }
    }

    auto Profile::getAverageTimeTaken() const -> f64
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( m_averageTimeTaken.size() > 0.0f )
        {
            return Util::average( m_averageTimeTaken );
        }

        return 0.0f;
    }

    void Profile::setTimeTaken( float timeTaken )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        m_timeTaken = timeTaken;

        m_averageTimeTaken.push_back( m_timeTaken );

        if( m_averageTimeTaken.size() > DEFAULT_NUM_SAMPLES )
        {
            m_averageTimeTaken.erase(
                m_averageTimeTaken.begin(),
                m_averageTimeTaken.begin() + ( m_averageTimeTaken.size() - DEFAULT_NUM_SAMPLES ) );
        }
    }

    void Profile::clear()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        m_timeTaken = 0.0f;
        m_averageTimeTaken.clear();
    }

    void Profile::setLabel( const String &label )
    {
        m_label = label;
    }

    auto Profile::getLabel() const -> String
    {
        return m_label;
    }

    void Profile::setUserData( void *userData )
    {
        m_userData = userData;
    }

    auto Profile::getUserData() const -> void *
    {
        return m_userData;
    }

    auto Profile::getTimeTaken() const -> float
    {
        return m_timeTaken;
    }

    auto Profile::getDescription() const -> String
    {
        return m_description;
    }

    void Profile::setDescription( const String &description )
    {
        m_description = description;
    }

    auto Profile::getTotal() const -> f64
    {
        return m_total;
    }

    void Profile::setTotal( f64 total )
    {
        m_total = total;
    }

}  // end namespace fb
