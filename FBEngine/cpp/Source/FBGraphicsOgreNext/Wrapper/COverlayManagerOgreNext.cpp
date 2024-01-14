#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementOgre.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementContainer.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementText.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Core/LogManager.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>

namespace fb::render
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
        for( auto overlay : overlays )
        {
            overlay->update();
        }
    }

    auto COverlayManagerOgreNext::addOverlay( const String &instanceName ) -> SmartPtr<IOverlay>
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto overlay = fb::make_ptr<COverlayOgreNext>();
            overlay->setName( instanceName );
            m_overlays.emplace_back( overlay );
            graphicsSystem->loadObject( overlay );
            return overlay;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto COverlayManagerOgreNext::removeOverlay( const String &instanceName ) -> bool
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

    auto COverlayManagerOgreNext::removeOverlay( SmartPtr<IOverlay> overlay ) -> bool
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

    auto COverlayManagerOgreNext::findOverlay( const String &instanceName ) -> SmartPtr<IOverlay>
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

    auto COverlayManagerOgreNext::hasOverlay( const String &instanceName ) -> bool
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

    auto COverlayManagerOgreNext::addElement( const String &typeName, const String &instanceName )
        -> SmartPtr<IOverlayElement>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        if( typeName == ( String( "Panel" ) ) )
        {
            SmartPtr<COverlayElementContainer> container( new COverlayElementContainer );
            container->setName( instanceName );
            m_overlayElements.emplace_back( container );
            graphicsSystem->loadObject( container );
            return container;
        }
        else if( typeName == ( String( "TextArea" ) ) )
        {
            SmartPtr<COverlayElementText> container( new COverlayElementText );
            container->setName( instanceName );
            m_overlayElements.emplace_back( container );
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

    auto COverlayManagerOgreNext::findElement( const String &instanceName ) -> SmartPtr<IOverlayElement>
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

    auto COverlayManagerOgreNext::removeElement( SmartPtr<IOverlayElement> element ) -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        graphicsSystem->unloadObject( element );

        auto it = std::remove( m_overlayElements.begin(), m_overlayElements.end(), element );
        if( it != m_overlayElements.end() )
        {
            m_overlayElements.erase( it, m_overlayElements.end() );
            return true;
        }

        return false;
    }

    void COverlayManagerOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_overlayMgr;
    }

    auto COverlayManagerOgreNext::getOverlays() const -> Array<SmartPtr<IOverlay>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_overlays;
    }

}  // namespace fb::render
