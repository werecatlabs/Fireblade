#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/COverlayManagerOgre.h>
#include <FBGraphicsOgre/Wrapper/COverlayOgre.h>
#include <FBGraphicsOgre/Wrapper/COverlayElementOgre.h>
#include <FBGraphicsOgre/Wrapper/COverlayElementContainer.h>
#include <FBGraphicsOgre/Wrapper/COverlayElementText.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>
#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, COverlayManagerOgre, CSharedObject<IOverlayManager> );

        u32 COverlayManagerOgre::m_nameExt = 0;

        COverlayManagerOgre::COverlayManagerOgre()
        {
            
        }

        COverlayManagerOgre::~COverlayManagerOgre()
        {
            unload( nullptr );
        }

        void COverlayManagerOgre::load( SmartPtr<ISharedObject> data )
        {
        }

        void COverlayManagerOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    for( auto overlayElement : m_overlayElements )
                    {
                        overlayElement->unload( nullptr );
                    }

                    m_overlayElements.clear();

                    auto &gc = GarbageCollector::instance();
                    gc.update();

                    for( auto overlay : m_overlays )
                    {
                        overlay->unload( nullptr );
                    }

                    m_overlays.clear();

                    gc.update();

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IOverlay> COverlayManagerOgre::addOverlay( const String &instanceName )
        {
            try
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

	            auto applicationManager = core::IApplicationManager::instance();
	            FB_ASSERT( applicationManager );
	
	            auto graphicsSystem = applicationManager->getGraphicsSystem();
	            FB_ASSERT( graphicsSystem );
	
	            auto overlay = fb::make_ptr<COverlayOgre>();
	            overlay->setName( instanceName );
	            m_overlays.push_back( overlay );
	            graphicsSystem->loadObject( overlay );
	            return overlay;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        bool COverlayManagerOgre::removeOverlay( const String &instanceName )
        {
            try
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

	            auto count = 0;
	            for( auto e : m_overlays )
	            {
	                if( instanceName == e->getName() )
	                {
	                    auto it = m_overlays.begin();
	                    std::advance( it, count );
	                    m_overlays.erase( it );
	                    return true;
	                }
	
	                count++;
	            }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return false;
        }

        bool COverlayManagerOgre::removeOverlay( SmartPtr<IOverlay> overlay )
        {
            try
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

	            auto it = std::find( m_overlays.begin(), m_overlays.end(), overlay );
	            if( it != m_overlays.end() )
	            {
	                m_overlays.erase( it );
	                return true;
	            }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return false;
        }

        SmartPtr<IOverlay> COverlayManagerOgre::findOverlay( const String &instanceName )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            for( auto e : m_overlays )
            {
                if( instanceName == e->getName() )
                {
                    return e;
                }
            }

            return nullptr;
        }

        bool COverlayManagerOgre::hasOverlay( const String &instanceName )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            for( auto e : m_overlays )
            {
                if( instanceName == e->getName() )
                {
                    return true;
                }
            }

            return false;
        }

        SmartPtr<IOverlayElement> COverlayManagerOgre::addElement( const String &typeName,
                                                                   const String &instanceName )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            if( typeName == ( String( "Panel" ) ) )
            {
                auto container = fb::make_ptr<COverlayElementContainer>();
                container->setName( instanceName );
                m_overlayElements.push_back( container );
                graphicsSystem->loadObject( container );
                return container;
            }
            if( typeName == ( String( "TextArea" ) ) )
            {
                auto container = fb::make_ptr<COverlayElementText>();
                container->setName( instanceName );
                m_overlayElements.push_back( container );
                graphicsSystem->loadObject( container );
                return container;
            }
            if( typeName == ( String( "VectorImage" ) ) )
            {
                return nullptr;
            }

            FB_LOG_ERROR( "COverlayManager::addElement - unknown element type." );

            return nullptr;
        }

        SmartPtr<IOverlayElement> COverlayManagerOgre::findElement( const String &instanceName )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            for( auto e : m_overlayElements )
            {
                if( instanceName == e->getName() )
                {
                    return e;
                }
            }

            return nullptr;
        }

        bool COverlayManagerOgre::removeElement( SmartPtr<IOverlayElement> element )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            graphicsSystem->unloadObject( element );

            auto it = std::find( m_overlayElements.begin(), m_overlayElements.end(), element );
            if( it != m_overlayElements.end() )
            {
                m_overlayElements.erase( it );
                return true;
            }

            return false;
        }

        void COverlayManagerOgre::_getObject( void **ppObject ) const
        {
            auto overlayMgr = Ogre::OverlayManager::getSingletonPtr();
            *ppObject = overlayMgr;
        }
        
        Array<SmartPtr<IOverlay>> COverlayManagerOgre::getOverlays() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_overlays;
        }

    }  // end namespace render
}  // end namespace fb
