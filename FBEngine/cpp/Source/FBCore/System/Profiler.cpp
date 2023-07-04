#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Profiler.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/Timer.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IProfile.h>

#include "FBCore/System/Profile.h"

namespace fb
{
    Profiler::Profiler() : m_nextUpdate( 0.f ), m_nextUpdateTime( 0.0 )
    {
    }

    Profiler::~Profiler()
    {
    }

    void Profiler::update()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();
            auto task = Thread::getCurrentTask();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SmartPtr<IProfile> Profiler::addProfile()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        auto profile = fb::make_ptr<Profile>();
        m_profiles.push_back( profile );

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

    SmartPtr<IProfile> Profiler::getProfile( hash64 id ) const
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

    Array<SmartPtr<IProfile>> Profiler::getProfiles() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_profiles;
    }

    void Profiler::logResults()
    {
    }
}  // end namespace fb
