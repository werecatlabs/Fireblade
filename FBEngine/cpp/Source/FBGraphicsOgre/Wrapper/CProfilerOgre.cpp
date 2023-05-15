#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CProfilerOgre.h>
#include <FBGraphicsOgre/Wrapper/CProfileOgre.h>
#include <FBGraphicsOgre/Ogre/Plot.hpp>
#include <FBGraphicsOgre/Ogre/COgreProfiler.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        CProfilerOgre::CProfilerOgre() : m_nextUpdate( 0.f )
        {
            m_profiler = new Ogre::Profiler();

            m_profiler->setTimer( Ogre::Root::getSingletonPtr()->getTimer() );
        }

        CProfilerOgre::~CProfilerOgre()
        {
            delete m_profiler;
        }

        void CProfilerOgre::update( float t, float dt )
        {
        }

        bool CProfilerOgre::isVisible() const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_overlay->isVisible();
        }

        void CProfilerOgre::setVisible( bool val )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_profiler->setEnabled( val );
        }

        SmartPtr<IProfile> CProfilerOgre::addProfile()
        {
            // SmartPtr<IProfile> profile(new CProfileOgre, true);
            // m_profiles.push_back(profile);
            // return profile;

            return nullptr;
        }

        SmartPtr<IProfile> CProfilerOgre::addProfile( hash32 id )
        {
            return nullptr;
        }

        void CProfilerOgre::removeProfile( SmartPtr<IProfile> profile )
        {
            // m_profiles.erase_element(profile);
        }

        SmartPtr<IProfile> CProfilerOgre::getProfile( hash32 id )
        {
            return nullptr;
        }

        void CProfilerOgre::start( const String &name )
        {
            m_profiler->beginProfile( name.c_str() );
        }

        void CProfilerOgre::end( const String &name )
        {
            m_profiler->endProfile( name.c_str() );
        }

        void CProfilerOgre::logResults()
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_profiler->logResults();
        }
    }  // namespace render
}  // namespace fb
