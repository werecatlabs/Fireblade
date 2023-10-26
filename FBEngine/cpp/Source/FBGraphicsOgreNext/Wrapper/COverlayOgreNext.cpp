#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb::render, COverlayOgreNext, IOverlay );

        COverlayOgreNext::COverlayOgreNext()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto stateObject = stateManager->addStateObject();

                auto stateListener = factoryManager->make_ptr<OverlayStateListener>();
                stateListener->setOwner( this );
                setStateListener( stateListener );
                stateObject->addStateListener( m_stateListener );

                auto state = factoryManager->make_ptr<OverlayState>();
                stateObject->setState( state );
                stateObject->setOwner( this );

                setStateObject( stateObject );

                m_state = state;

                auto stateTask = graphicsSystem->getStateTask();
                m_state->setTaskId( stateTask );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        COverlayOgreNext::~COverlayOgreNext()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void COverlayOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto overlayManager = Ogre::v1::OverlayManager::getSingletonPtr();
                FB_ASSERT( overlayManager );

                auto name = getName();

                m_overlay = overlayManager->create( name );
                FB_ASSERT( m_overlay );

                auto panel = static_cast<Ogre::v1::OverlayContainer *>(
                    overlayManager->createOverlayElement( "Panel", name ) );
                FB_ASSERT( panel );

                panel->setMetricsMode( Ogre::v1::GuiMetricsMode::GMM_PIXELS );
                panel->initialise();

                m_overlay->add2D( panel );
                m_container = panel;

                if( auto stateObject = getStateObject() )
                {
                    stateObject->setDirty( true );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                if( auto stateObject = getStateObject() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                        setStateListener( nullptr );
                    }

                    stateManager->removeStateObject( m_stateObject );

                    stateObject->unload( nullptr );
                    setStateObject( nullptr );
                }

                for( auto element : m_elements )
                {
                    Ogre::v1::OverlayContainer *ogreElement;
                    element->_getObject( (void **)&ogreElement );

                    if( ogreElement )
                    {
                        m_container->_notifyParent( m_container->getParent(), nullptr );
                        m_container->removeChild( ogreElement->getName() );
                    }
                }

                m_elements.clear();

                if( m_overlay )
                {
                    auto overlayManager = Ogre::v1::OverlayManager::getSingletonPtr();
                    FB_ASSERT( overlayManager );

                    if( m_container )
                    {
                        m_overlay->remove2D( m_container );

                        auto it = m_container->getChildIterator();
                        while( it.hasMoreElements() )
                        {
                            auto element = it.getNext();
                            m_container->removeChild( element->getName() );
                        }

                        overlayManager->destroyOverlayElement( m_container );
                        m_container = nullptr;
                    }

                    overlayManager->destroy( m_overlay );
                    m_overlay = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayOgreNext::update()
        {
            auto container = m_container;
            auto overlay = m_overlay;

            if( container )
            {
                container->_notifyViewport();
                container->_notifyParent( nullptr, overlay );
            }
        }

        String COverlayOgreNext::getName() const
        {
            auto handle = getHandle();
            return handle->getName();
        }

        void COverlayOgreNext::setName( const String &name )
        {
            auto handle = getHandle();
            handle->setName( name );
        }

        void COverlayOgreNext::addElement( SmartPtr<IOverlayElement> element )
        {
            try
            {
                FB_ASSERT( element );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                if( isThreadSafe() )
                {
                    if( element->isLoaded() )
                    {
                        Ogre::v1::OverlayContainer *ogreElement;
                        element->_getObject( (void **)&ogreElement );

                        element->setOverlay( this );

                        m_elements.push_back( element );

                        if( m_overlay )
                        {
                            //m_overlay->add2D( ogreElement );

                            if( m_container )
                            {
                                if( ogreElement->getParent() != m_container )
                                {
                                    m_container->addChild( ogreElement );
                                }
                            }
                        }

                        if( auto stateContext = getStateObject() )
                        {
                            stateContext->setDirty( true );
                        }
                    }
                    else
                    {
                        auto message = factoryManager->make_ptr<StateMessageObject>();
                        message->setType( IOverlayElement::STATE_MESSAGE_ATTACH_OBJECT );
                        message->setObject( element );
                        addMessage( message );
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_ATTACH_OBJECT );
                    message->setObject( element );
                    addMessage( message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool COverlayOgreNext::removeElement( SmartPtr<IOverlayElement> element )
        {
            try
            {
                FB_ASSERT( element );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                if( isThreadSafe() )
                {
                    Ogre::v1::OverlayContainer *ogreElement;
                    element->_getObject( (void **)&ogreElement );

                    auto it = std::find( m_elements.begin(), m_elements.end(), element );
                    if( it != m_elements.end() )
                    {
                        m_elements.erase( it );
                    }

                    if( m_overlay )
                    {
                        if( m_container )
                        {
                            if( ogreElement->getParent() == m_container )
                            {
                                m_container->removeChild( ogreElement->getName() );
                            }
                        }
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageObject>();
                    message->setType( STATE_MESSAGE_DETACH_OBJECT );
                    message->setObject( element );
                    addMessage( message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return false;
        }

        void COverlayOgreNext::addSceneNode( SmartPtr<ISceneNode> sceneNode )
        {
            FB_ASSERT( sceneNode );

            Ogre::SceneNode *ogreSceneNode;
            sceneNode->_getObject( (void **)&ogreSceneNode );

            if( m_overlay )
            {
                if( ogreSceneNode )
                {
                    // m_overlay->add3D(ogreSceneNode);
                }
            }
        }

        bool COverlayOgreNext::removeSceneNode( SmartPtr<ISceneNode> sceneNode )
        {
            FB_ASSERT( sceneNode );

            Ogre::SceneNode *ogreSceneNode;
            sceneNode->_getObject( (void **)&ogreSceneNode );

            if( m_overlay )
            {
                if( ogreSceneNode )
                {
                    // m_overlay->remove3D(ogreSceneNode);
                }
            }

            return false;
        }

        void COverlayOgreNext::setVisible( bool visible )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<OverlayState>() )
                    {
                        state->setVisible( visible );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool COverlayOgreNext::isVisible() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<OverlayState>() )
                {
                    return state->isVisible();
                }
            }

            return false;
        }

        void COverlayOgreNext::setZOrder( u32 zorder )
        {
            if( m_overlay )
            {
                m_overlay->setZOrder( (u16)zorder );
            }
        }

        u32 COverlayOgreNext::getZOrder( void ) const
        {
            if( m_overlay )
            {
                return (u32)m_overlay->getZOrder();
            }

            return 0;
        }

        void COverlayOgreNext::updateZOrder()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ISharedObject::ScopedLock lock( graphicsSystem );

            for( auto element : m_elements )
            {
                Ogre::v1::OverlayContainer *ogreElement;
                element->_getObject( (void **)&ogreElement );

                if( ogreElement )
                {
                    if( m_container )
                    {
                        m_container->removeChild( ogreElement->getName() );
                    }
                }
            }

            std::sort( m_elements.begin(), m_elements.end(),
                       []( auto a, auto b ) { return a->getZOrder() < b->getZOrder(); } );

            for( auto element : m_elements )
            {
                Ogre::v1::OverlayContainer *ogreElement;
                element->_getObject( (void **)&ogreElement );

                if( ogreElement )
                {
                    if( m_container )
                    {
                        m_container->addChild( ogreElement );
                    }
                }
            }
        }

        void COverlayOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = m_overlay;
        }

        Array<SmartPtr<IOverlayElement>> COverlayOgreNext::getElements() const
        {
            return m_elements;
        }

        Vector2I COverlayOgreNext::getAbsoluteResolution() const
        {
            return Vector2I();
        }

        void COverlayOgreNext::setAbsoluteResolution( const Vector2I &val )
        {
        }

        bool COverlayOgreNext::isValid() const
        {
            auto stateObject = getStateObject();
            auto stateListener = getStateListener();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                if( m_overlay && stateObject && stateListener && m_state )
                {
                    if( !( stateObject->isValid() && stateListener->isValid() && m_state->isValid() ) )
                    {
                        return false;
                    }

                    for( auto element : m_elements )
                    {
                        if( !element->isValid() )
                        {
                            return false;
                        }
                    }

                    return true;
                }
            }
            else if( loadingState == LoadingState::Unloaded )
            {
                if( !( m_overlay && stateObject && stateListener && m_state ) )
                {
                    return false;
                }

                return true;
            }

            return false;
        }

        Array<SmartPtr<ISharedObject>> COverlayOgreNext::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( m_elements.size() + 12 );

            for( auto element : m_elements )
            {
                if( element )
                {
                    //objects.push_back( element );
                }
            }

            return objects;
        }

        SmartPtr<Properties> COverlayOgreNext::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

            if( m_overlay )
            {
                auto visible = m_overlay->isVisible();
                properties->setProperty( "visible", visible );

                properties->setProperty( "name", m_overlay->getName() );
                properties->setProperty( "zorder", m_overlay->getZOrder() );
                properties->setProperty( "origin", m_overlay->getOrigin() );
                properties->setProperty( "zOrder", m_overlay->getZOrder() );
                properties->setProperty( "scrollX", m_overlay->getScrollX() );
                properties->setProperty( "scrollY", m_overlay->getScrollY() );
                properties->setProperty( "scaleX", m_overlay->getScaleX() );
                properties->setProperty( "scaleY", m_overlay->getScaleY() );
            }

            return properties;
        }

        void COverlayOgreNext::setProperties( SmartPtr<Properties> properties )
        {
            auto visible = m_overlay->isVisible();
            auto zorder = (u32)m_overlay->getZOrder();

            properties->getPropertyValue( "visible", visible );
            properties->getPropertyValue( "zorder", zorder );

            if( m_overlay->isVisible() != visible )
            {
                m_overlay->setVisible( visible );
            }

            m_overlay->setZOrder( zorder );
        }

        COverlayOgreNext::OverlayStateListener::~OverlayStateListener()
        {
        }

        void COverlayOgreNext::OverlayStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                if( owner->isLoaded() )
                {
                    auto container = owner->m_container;
                    auto overlay = owner->m_overlay;

                    auto overlayState = fb::static_pointer_cast<OverlayState>( state );

                    container->_notifyViewport();
                    container->_notifyParent( nullptr, overlay );

                    auto visible = overlayState->isVisible();

                    overlay->hide();

                    if( visible )
                    {
                        overlay->show();
                    }
                    else
                    {
                        overlay->hide();
                    }

                    state->setDirty( false );
                }
            }
        }

        void COverlayOgreNext::OverlayStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();
            auto currentTaskId = Thread::getCurrentTask();

            FB_ASSERT( currentTaskId == renderTask );

            if( currentTaskId == renderTask )
            {
                if( message->isExactly<StateMessageVisible>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                    m_owner->setVisible( objectMessage->isVisible() );
                }
                else if( message->isExactly<StateMessageObject>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                    FB_ASSERT( objectMessage );

                    auto type = objectMessage->getType();
                    auto object = objectMessage->getObject();

                    if( type == STATE_MESSAGE_ATTACH_OBJECT )
                    {
                        m_owner->addElement( object );
                    }
                    else if( type == STATE_MESSAGE_DETACH_OBJECT )
                    {
                        m_owner->removeElement( object );
                    }
                }
            }
        }

        void COverlayOgreNext::OverlayStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        COverlayOgreNext *COverlayOgreNext::OverlayStateListener::getOwner() const
        {
            return m_owner;
        }

        void COverlayOgreNext::OverlayStateListener::setOwner( COverlayOgreNext *val )
        {
            m_owner = val;
        }

    }  // end namespace render
}  // end namespace fb
