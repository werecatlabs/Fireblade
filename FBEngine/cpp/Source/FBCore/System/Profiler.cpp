#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Profiler.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/System/Timer.h>
#include <FBCore/System/Profile.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IProfile.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    Profiler::Profiler() = default;

    Profiler::~Profiler() = default;

    void Profiler::update()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto timer = applicationManager->getTimer();
            auto task = Thread::getCurrentTask();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Profiler::addProfile() -> SmartPtr<IProfile>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        auto profile = fb::make_ptr<Profile>();
        m_profiles.emplace_back( profile );

        return profile;
    }

    void Profiler::removeProfile( SmartPtr<IProfile> profile )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        auto it = std::find( m_profiles.begin(), m_profiles.end(), profile );
        if( it != m_profiles.end() )
        {
            m_profiles.erase( it );
        }
    }

    auto Profiler::getProfile( hash64 id ) const -> SmartPtr<IProfile>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        for( auto &p : m_profiles )
        {
            if( auto handle = p->getHandle() )
            {
                if( handle->getId() == id )
                {
                    return p;
                }
            }
        }

        return nullptr;
    }

    auto Profiler::getProfiles() const -> Array<SmartPtr<IProfile>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_profiles;
    }

    void Profiler::logResults()
    {
    }
}  // end namespace fb
