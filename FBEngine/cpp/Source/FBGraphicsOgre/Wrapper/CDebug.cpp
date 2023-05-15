#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CDebug.h>
#include <FBGraphicsOgre/DynamicLines.h>
#include <FBGraphicsOgre/Wrapper/CDebugLine.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreWireBoundingBox.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CDebug, CSharedObject<IDebug> );

        CDebug::CDebug()
        {
            auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
            pNewDebugLines->reserve( 256 );
            setDebugLines( pNewDebugLines );
        }

        CDebug::~CDebug()
        {
            unload( nullptr );
        }

        void CDebug::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );
                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDebug::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );
                clear();
                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDebug::preUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            switch( task )
            {
            case Thread::Task::Render:
            {
                if( auto pDebugLines = getDebugLines() )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto &debugLine : debugLines )
                    {
                        debugLine->preUpdate();
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }

        void CDebug::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            switch( task )
            {
            case Thread::Task::Render:
            {
                if( auto pDebugLines = getDebugLines() )
                {
                    auto &debugLines = *pDebugLines;
                    for( auto &debugLine : debugLines )
                    {
                        if( debugLine )
                        {
                            debugLine->update();
                        }
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }

        void CDebug::postUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            switch( task )
            {
            case Thread::Task::Render:
            {
                if( auto pDebugLines = getDebugLines() )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto &debugLine : debugLines )
                    {
                        if( debugLine )
                        {
                            debugLine->postUpdate();
                        }
                    }

                    bool removeLines = false;
                    auto newDebugLines =
                        Array<SmartPtr<IDebugLine>>( debugLines.begin(), debugLines.end() );
                    for( auto &debugLine : debugLines )
                    {
                        if( debugLine->getLifeTime() > debugLine->getMaxLifeTime() )
                        {
                            auto it = std::find( newDebugLines.begin(), newDebugLines.end(), debugLine );
                            if( it != newDebugLines.end() )
                            {
                                newDebugLines.erase( it );
                                m_removeQueue.push( debugLine );
                                removeLines = true;
                            }
                        }
                    }

                    if( removeLines )
                    {
                        auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                        *pNewDebugLines = ConcurrentArray<SmartPtr<IDebugLine>>( newDebugLines.begin(),
                                                                                 newDebugLines.end() );
                        setDebugLines( pNewDebugLines );
                    }

                    SmartPtr<IDebugLine> pQueuedDebugLine;
                    while( m_removeQueue.try_pop( pQueuedDebugLine ) )
                    {
                        pQueuedDebugLine->unload( nullptr );
                        pQueuedDebugLine = nullptr;
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }

        void CDebug::drawPoint( s32 id, const Vector3<real_Num> &p, u32 color )
        {
            auto size = static_cast<real_Num>( 0.1 );
            auto offset0 = Vector3<real_Num>::forward() * size;
            auto offset1 = Vector3<real_Num>::up() * size;
            auto offset2 = Vector3<real_Num>::right() * size;

            drawLine( id, p + offset0, p - offset0, color );
            drawLine( id + 1, p + offset1, p - offset1, color );
            drawLine( id + 2, p + offset2, p - offset2, color );
        }

        void CDebug::drawLine( s32 id, const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                               u32 colour )
        {
            auto line = getLine( id );
            if( !line )
            {
                line = addLine( id );
            }

            if( line )
            {
                line->setPosition( start );
                line->setVector( end - start );
            }
        }

        void CDebug::drawText( s32 id, const Vector2<real_Num> &position, const String &text, u32 color )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto overlayManager = graphicsSystem->getOverlayManager();
            FB_ASSERT( overlayManager );

            auto overlay = getOverlay();
            if( !overlay )
            {
                overlay = overlayManager->addOverlay( "debug_overlay" );
                setOverlay( overlay );

                overlay->setVisible( true );
            }

            if( overlay )
            {
                auto textElement = getElementById( id );
                if( !textElement )
                {
                    auto referenceScreenSize = Vector2F( 1920.0f, 1080.0f );

                    auto name = StringUtil::toString( id );
                    textElement = overlayManager->addElement( "TextArea", name );

                    if( auto handle = textElement->getHandle() )
                    {
                        handle->setId( id );
                    }

                    textElement->setPosition( position );
                    textElement->setSize( Vector2F( 100, 40 ) / referenceScreenSize );
                    textElement->setHorizontalAlignment( 0 );
                    textElement->setVerticalAlignment( 0 );
                    overlay->addElement( textElement );

                    addOverlayElement( textElement );
                }

                if( textElement )
                {
                    textElement->setCaption( text );
                }
            }
        }

        SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> CDebug::getOverlayElementsPtr() const
        {
            return m_overlayElements;
        }

        void CDebug::setOverlayElementsPtr(
            SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> elements )
        {
            m_overlayElements = elements;
        }

        void CDebug::addOverlayElement( SmartPtr<IOverlayElement> element )
        {
            auto p = getOverlayElementsPtr();
            if( !p )
            {
                p = fb::make_shared<ConcurrentArray<SmartPtr<IOverlayElement>>>();
                setOverlayElementsPtr( p );
            }

            if( p )
            {
                auto &elements = *p;
                elements.push_back( element );
            }
        }

        SmartPtr<IOverlayElement> CDebug::getElementById( s32 id ) const
        {
            if( auto p = getOverlayElementsPtr() )
            {
                auto &elements = *p;
                for( auto &element : elements )
                {
                    if( auto handle = element->getHandle() )
                    {
                        if( handle->getId() == id )
                        {
                            return element;
                        }
                    }
                }
            }

            return nullptr;
        }

        SmartPtr<IOverlay> CDebug::getOverlay() const
        {
            return m_overlay;
        }

        void CDebug::setOverlay( SmartPtr<IOverlay> overlay )
        {
            m_overlay = overlay;
        }

        SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> CDebug::getDebugLines() const
        {
            return m_debugLines;
        }

        void CDebug::setDebugLines( SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> debugLines )
        {
            m_debugLines = debugLines;
        }

        SmartPtr<IDebugLine> CDebug::addLine( s32 id )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto debugLine = factoryManager->make_ptr<CDebugLine>();
            debugLine->setId( id );

            auto pDebugLines = getDebugLines();
            pDebugLines->push_back( debugLine );

            return debugLine;
        }

        void CDebug::removeLine( s32 id )
        {
            auto pDebugLines = getDebugLines();
            auto &debugLines = *pDebugLines;

            auto newDebugLines = Array<SmartPtr<IDebugLine>>( debugLines.begin(), debugLines.end() );
            for( auto &debugLine : debugLines )
            {
                if( debugLine->getId() == id )
                {
                    auto it = std::find( newDebugLines.begin(), newDebugLines.end(), debugLine );
                    if( it != newDebugLines.end() )
                    {
                        newDebugLines.erase( it );

                        auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                        *pNewDebugLines = ConcurrentArray<SmartPtr<IDebugLine>>( newDebugLines.begin(),
                                                                                 newDebugLines.end() );
                        setDebugLines( pNewDebugLines );
                    }
                }
            }
        }

        void CDebug::removeLine( SmartPtr<IDebugLine> debugLine )
        {
            if( auto pDebugLines = getDebugLines() )
            {
                auto &debugLines = *pDebugLines;

                auto newDebugLines = Array<SmartPtr<IDebugLine>>( debugLines.begin(), debugLines.end() );

                auto it = std::find( newDebugLines.begin(), newDebugLines.end(), debugLine );
                if( it != newDebugLines.end() )
                {
                    newDebugLines.erase( it );

                    auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                    *pNewDebugLines = ConcurrentArray<SmartPtr<IDebugLine>>( newDebugLines.begin(),
                                                                             newDebugLines.end() );
                    setDebugLines( pNewDebugLines );
                }
            }
        }

        SmartPtr<IDebugLine> CDebug::getLine( s32 id ) const
        {
            if( auto pDebugLines = getDebugLines() )
            {
                auto &debugLines = *pDebugLines;

                for( auto debugLine : debugLines )
                {
                    if( debugLine )
                    {
                        if( debugLine->getId() == id )
                        {
                            return debugLine;
                        }
                    }
                }
            }

            return nullptr;
        }

        void CDebug::clear()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            if( auto pDebugLines = getDebugLines() )
            {
                auto &debugLines = *pDebugLines;

                for( auto debugLine : debugLines )
                {
                    graphicsSystem->unloadObject( debugLine );
                }
            }

            auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
            setDebugLines( pNewDebugLines );
        }
    }  // end namespace render
}  // end namespace fb
