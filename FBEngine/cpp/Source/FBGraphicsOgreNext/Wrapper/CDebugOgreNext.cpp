#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CDebugOgreNext.h>
#include <FBGraphicsOgreNext/DynamicLines.h>
#include <FBGraphicsOgreNext/Wrapper/CDebugLine.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreWireBoundingBox.h>
#include <Ogre.h>
#include <OgreHlmsUnlit.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreManualObject2.h>

namespace fb
{
    namespace render
    {

        CDebugOgreNext::~CDebugOgreNext()
        {
            unload( nullptr );
        }

        void CDebugOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                // auto id = Ogre::Id::generateNewId<Ogre::v1::WireBoundingBox>();
                // auto objectMemoryManager = new Ogre::ObjectMemoryManager;

                // Ogre::SceneManager *manager = nullptr;
                // m_wireBoundingBox = new Ogre::v1::WireBoundingBox( id, objectMemoryManager, manager );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto stateObject = stateManager->addStateObject();
                FB_ASSERT( stateObject );

                setStateObject( stateObject );

                auto sceneNodeStateListener = factoryManager->make_ptr<StateListener>();
                sceneNodeStateListener->setOwner( this );
                m_stateListener = sceneNodeStateListener;
                stateObject->addStateListener( m_stateListener );

                auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                pNewDebugLines->reserve( 256 );
                setDebugLines( pNewDebugLines );

                m_memoryManager = new Ogre::ObjectMemoryManager();

                FB_ASSERT( applicationManager->isValid() );
                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDebugOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    clear();

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CDebugOgreNext::preUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto stateTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            if( task == stateTask )
            {
                if( auto pDebugLines = getDebugLines() )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto debugLine : debugLines )
                    {
                        if( debugLine )
                        {
                            debugLine->preUpdate();
                        }
                    }
                }
            }
        }

        void CDebugOgreNext::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto stateTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            if( task == stateTask )
            {
                if( auto pDebugLines = getDebugLines() )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto debugLine : debugLines )
                    {
                        if( debugLine )
                        {
                            debugLine->update();
                        }
                    }
                }
            }
        }

        void CDebugOgreNext::postUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto stateTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            if( task == stateTask )
            {
                if( auto pDebugLines = getDebugLines() )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto debugLine : debugLines )
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
                        auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>(
                            newDebugLines.begin(), newDebugLines.end() );
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
        }

        void CDebugOgreNext::drawPoint( s32 id, const Vector3<real_Num> &p, u32 color )
        {
            auto size = static_cast<real_Num>( 0.1 );
            auto offset0 = Vector3<real_Num>::forward() * size;
            auto offset1 = Vector3<real_Num>::up() * size;
            auto offset2 = Vector3<real_Num>::right() * size;

            drawLine( id, p + offset0, p - offset0, color );
            drawLine( id + 1, p + offset1, p - offset1, color );
            drawLine( id + 2, p + offset2, p - offset2, color );
        }

        SmartPtr<IDebugLine> CDebugOgreNext::drawLine( s32 id, const Vector3<real_Num> &start,
                                               const Vector3<real_Num> &end, u32 colour )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto line = getLine( id );
            if( !line )
            {
                line = addLine( id );
            }

            if( line )
            {
                line->setColour( colour );
                line->setPosition( start );
                line->setVector( end - start );
            }

            if( !line->isLoaded() )
            {
                graphicsSystem->loadObject( line );
            }

            return line;
        }

        Ogre::HlmsUnlitDatablock *CDebugOgreNext::getDatablock() const
        {
            return m_datablock;
        }

        void CDebugOgreNext::setDatablock( Ogre::HlmsUnlitDatablock *datablock )
        {
            m_datablock = datablock;
        }

        void CDebugOgreNext::drawText( s32 id, const Vector2<real_Num> &position, const String &text, u32 color )
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

                overlay->setVisible( true );
            }
        }

        SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> CDebugOgreNext::getOverlayElementsPtr() const
        {
            return m_overlayElements;
        }

        void CDebugOgreNext::setOverlayElementsPtr(
            SharedPtr<ConcurrentArray<SmartPtr<IOverlayElement>>> elements )
        {
            m_overlayElements = elements;
        }

        void CDebugOgreNext::addOverlayElement( SmartPtr<IOverlayElement> element )
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

        SmartPtr<IOverlayElement> CDebugOgreNext::getElementById( s32 id ) const
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

        SmartPtr<IOverlay> CDebugOgreNext::getOverlay() const
        {
            return m_overlay;
        }

        void CDebugOgreNext::setOverlay( SmartPtr<IOverlay> overlay )
        {
            m_overlay = overlay;
        }

        void CDebugOgreNext::createLineMaterial()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT( applicationManager );

            // auto graphicsSystem = applicationManager->getGraphicsSystem();
            // FB_ASSERT( graphicsSystem );

            // auto smgr = graphicsSystem->getSceneManager();
            // FB_ASSERT( smgr );

            // Ogre::SceneManager *sceneManager = nullptr;
            // smgr->_getObject( (void **)&sceneManager );

            auto root = Ogre::Root::getSingletonPtr();

            auto hlmsManager = root->getHlmsManager();

            auto hlmsUnlitMaterials =
                static_cast<Ogre::HlmsUnlit *>( hlmsManager->getHlms( Ogre::HLMS_UNLIT ) );
            Ogre::HlmsUnlitDatablock *UnlitDatablock;
            Ogre::String datablockUnlitName = "UnlitColour";

            UnlitDatablock =
                static_cast<Ogre::HlmsUnlitDatablock *>( hlmsUnlitMaterials->createDatablock(
                    datablockUnlitName, datablockUnlitName, Ogre::HlmsMacroblock(),
                    Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );

            UnlitDatablock->setUseColour( true );
            UnlitDatablock->setColour( Ogre::ColourValue( 1, 1, 1 ) );

            m_datablock = UnlitDatablock;
        }

        SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> CDebugOgreNext::getDebugLines() const
        {
            return m_debugLines;
        }

        void CDebugOgreNext::setDebugLines( SharedPtr<ConcurrentArray<SmartPtr<IDebugLine>>> debugLines )
        {
            m_debugLines = debugLines;
        }

        SmartPtr<IDebugLine> CDebugOgreNext::addLine( s32 id )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            if( m_datablock == nullptr )
            {
                createLineMaterial();
            }

            auto debugLine = factoryManager->make_ptr<CDebugLine>();
            debugLine->setMemoryManager( m_memoryManager );
            debugLine->setId( id );
            debugLine->setDatablock( m_datablock );

            if( auto pDebugLines = getDebugLines() )
            {
                pDebugLines->push_back( debugLine );
            }

            return debugLine;
        }

        void CDebugOgreNext::removeLine( s32 id )
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

        void CDebugOgreNext::removeLine( SmartPtr<IDebugLine> debugLine )
        {
            auto pDebugLines = getDebugLines();
            auto &debugLines = *pDebugLines;

            auto newDebugLines = Array<SmartPtr<IDebugLine>>( debugLines.begin(), debugLines.end() );

            auto it = std::find( newDebugLines.begin(), newDebugLines.end(), debugLine );
            if( it != newDebugLines.end() )
            {
                newDebugLines.erase( it );

                auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                *pNewDebugLines =
                    ConcurrentArray<SmartPtr<IDebugLine>>( newDebugLines.begin(), newDebugLines.end() );
                setDebugLines( pNewDebugLines );
            }
        }

        SmartPtr<IDebugLine> CDebugOgreNext::getLine( s32 id ) const
        {
            auto pDebugLines = getDebugLines();
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

            return nullptr;
        }

        void CDebugOgreNext::clear()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto renderTask = graphicsSystem->getRenderTask();
            auto stateTask = graphicsSystem->getStateTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                auto pDebugLines = getDebugLines();
                if( pDebugLines )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto debugLine : debugLines )
                    {
                        if( debugLine )
                        {
                            debugLine->unload( nullptr );
                        }
                    }
                }

                auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                setDebugLines( pNewDebugLines );
            }
            else
            {
                auto pDebugLines = getDebugLines();
                if( pDebugLines )
                {
                    auto &debugLines = *pDebugLines;

                    for( auto debugLine : debugLines )
                    {
                        if( debugLine )
                        {
                            graphicsSystem->unloadObject( debugLine );
                        }
                    }
                }

                auto pNewDebugLines = fb::make_shared<ConcurrentArray<SmartPtr<IDebugLine>>>();
                setDebugLines( pNewDebugLines );
            }
        }

        void CDebugOgreNext::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CDebugOgreNext::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
        {
            if( message->isExactly<StateMessageDrawLine>() )
            {
                auto stateMessageDrawLine = fb::static_pointer_cast<StateMessageDrawLine>( message );

                auto colour = stateMessageDrawLine->getColour();
                auto lineId = stateMessageDrawLine->getLineId();
                auto start = stateMessageDrawLine->getStart();
                auto end = stateMessageDrawLine->getEnd();

                m_owner->drawLine( lineId, start, end, colour );
            }
        }

        void CDebugOgreNext::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CDebugOgreNext *CDebugOgreNext::StateListener::getOwner() const
        {
            return m_owner;
        }

        void CDebugOgreNext::StateListener::setOwner( CDebugOgreNext *owner )
        {
            m_owner = owner;
        }
    }  // end namespace render
}  // end namespace fb
