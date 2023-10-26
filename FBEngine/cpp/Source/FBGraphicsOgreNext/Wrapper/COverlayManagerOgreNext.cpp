#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementOgre.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementContainer.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementText.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Core/LogManager.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb::render, COverlayManagerOgreNext, IOverlayManager );

        u32 COverlayManagerOgreNext::m_nameExt = 0;

        COverlayManagerOgreNext::COverlayManagerOgreNext()
        {
            m_overlayMgr = Ogre::v1::OverlayManager::getSingletonPtr();
        }

        COverlayManagerOgreNext::~COverlayManagerOgreNext()
        {
            unload( nullptr );
        }

        void COverlayManagerOgreNext::load( SmartPtr<ISharedObject> data )
        {
        }

        void COverlayManagerOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    for( auto overlayElement : m_overlayElements )
                    {
                        overlayElement->unload( nullptr );
                    }

                    m_overlayElements.clear();

                    for( auto overlay : m_overlays )
                    {
                        overlay->unload( nullptr );
                    }

                    m_overlays.clear();

                    m_overlayMgr = nullptr;

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayManagerOgreNext::update()
        {
            auto overlays = getOverlays();
            for(auto overlay : overlays)
            {
                overlay->update();
            }
        }

        SmartPtr<IOverlay> COverlayManagerOgreNext::addOverlay( const String &instanceName )
        {
            try
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlay = fb::make_ptr<COverlayOgreNext>();
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

        bool COverlayManagerOgreNext::removeOverlay( const String &instanceName )
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

            return false;
        }

        bool COverlayManagerOgreNext::removeOverlay( SmartPtr<IOverlay> overlay )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto it = std::find( m_overlays.begin(), m_overlays.end(), overlay );
            if( it != m_overlays.end() )
            {
                m_overlays.erase( it );
                return true;
            }

            return false;
        }

        SmartPtr<IOverlay> COverlayManagerOgreNext::findOverlay( const String &instanceName )
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

        bool COverlayManagerOgreNext::hasOverlay( const String &instanceName )
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

        SmartPtr<IOverlayElement> COverlayManagerOgreNext::addElement( const String &typeName,
                                                                   const String &instanceName )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            if( typeName == ( String( "Panel" ) ) )
            {
                SmartPtr<COverlayElementContainer> container( new COverlayElementContainer );
                container->setName( instanceName );
                m_overlayElements.push_back( container );
                graphicsSystem->loadObject( container );
                return container;
            }
            else if( typeName == ( String( "TextArea" ) ) )
            {
                SmartPtr<COverlayElementText> container( new COverlayElementText );
                container->setName( instanceName );
                m_overlayElements.push_back( container );
                graphicsSystem->loadObject( container );
                return container;
            }
            else if( typeName == ( String( "VectorImage" ) ) )
            {
                return nullptr;
            }
            else
            {
                FB_LOG_ERROR( "COverlayManager::addElement - unknown element type." );
            }

            return nullptr;
        }

        SmartPtr<IOverlayElement> COverlayManagerOgreNext::findElement( const String &instanceName )
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

        bool COverlayManagerOgreNext::removeElement( SmartPtr<IOverlayElement> element )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            graphicsSystem->unloadObject( element );

            auto it = std::remove( m_overlayElements.begin(), m_overlayElements.end(), element );
            if( it != m_overlayElements.end() )
            {
                m_overlayElements.erase(it, m_overlayElements.end());
                return true;
            }

            return false;
        }

        void COverlayManagerOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = m_overlayMgr;
        }

        Array<SmartPtr<IOverlay>> COverlayManagerOgreNext::getOverlays() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_overlays;
        }

    }  // end namespace render
}  // end namespace fb
