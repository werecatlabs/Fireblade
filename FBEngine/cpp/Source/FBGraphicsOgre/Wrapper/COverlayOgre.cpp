#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/COverlayOgre.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, COverlayOgre, IOverlay );

        COverlayOgre::COverlayOgre()
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

        COverlayOgre::~COverlayOgre()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void COverlayOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto overlayManager = Ogre::OverlayManager::getSingletonPtr();
                FB_ASSERT( overlayManager );

                m_overlay = overlayManager->create( m_name );
                FB_ASSERT( m_overlay );

                auto panel = static_cast<Ogre::OverlayContainer *>(
                    overlayManager->createOverlayElement( "Panel", m_name ) );
                FB_ASSERT( panel );

                panel->setMetricsMode( Ogre::GuiMetricsMode::GMM_PIXELS );
                panel->initialise();

                m_overlay->add2D( panel );
                m_container = panel;

                m_overlay->initialise();
                m_overlay->show();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void COverlayOgre::unload( SmartPtr<ISharedObject> data )
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

                        stateListener->unload( nullptr );
                        setStateListener( nullptr );
                    }

                    stateManager->removeStateObject( stateObject );
                    setStateObject( nullptr );
                }

                if( m_overlay )
                {
                    auto overlayManager = Ogre::OverlayManager::getSingletonPtr();
                    FB_ASSERT( overlayManager );

                    if( m_container )
                    {
                        m_overlay->remove2D( m_container );

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

        String COverlayOgre::getName() const
        {
            return m_name;
        }

        void COverlayOgre::setName( const String &name )
        {
            m_name = name;
        }

        void COverlayOgre::addElement( SmartPtr<IOverlayElement> element )
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

                if( isThreadSafe() )
                {
                    if( element->isLoaded() )
                    {
                        Ogre::OverlayElement *ogreElement;
                        element->_getObject( (void **)&ogreElement );

                        m_elements.push_back( element );

                        if( m_overlay )
                        {
                            if( m_container )
                            {
                                if( ogreElement->getParent() != m_container )
                                {
                                    m_container->addChild( ogreElement );
                                }
                            }
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
                    message->setType( IOverlayElement::STATE_MESSAGE_ATTACH_OBJECT );
                    message->setObject( element );
                    addMessage( message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool COverlayOgre::removeElement( SmartPtr<IOverlayElement> element )
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

                if( isThreadSafe() )
                {
                    Ogre::OverlayContainer *ogreElement;
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
                    message->setType( IOverlayElement::STATE_MESSAGE_DETACH_OBJECT );
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

        void COverlayOgre::addSceneNode( SmartPtr<ISceneNode> sceneNode )
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

        bool COverlayOgre::removeSceneNode( SmartPtr<ISceneNode> sceneNode )
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

        void COverlayOgre::setVisible( bool visible )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( m_overlay )
                    {
                        m_overlay->setVisible( visible );

                        if( visible )
                        {
                            m_overlay->show();
                        }
                        else
                        {
                            m_overlay->hide();
                        }
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageVisible>();
                    message->setVisible( visible );

                    if( auto stateContext = getStateObject() )
                    {
                        stateContext->addMessage( stateTask, message );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool COverlayOgre::isVisible() const
        {
            if( m_overlay )
            {
                return m_overlay->isVisible();
            }

            return false;
        }

        void COverlayOgre::setZOrder( u32 zorder )
        {
            if( m_overlay )
            {
                m_overlay->setZOrder( static_cast<u16>( zorder ) );
            }
        }

        u32 COverlayOgre::getZOrder( void ) const
        {
            if( m_overlay )
            {
                return m_overlay->getZOrder();
            }

            return 0;
        }

        void COverlayOgre::updateZOrder()
        {
            if( m_overlay )
            {
                m_overlay->assignZOrders();
            }
        }

        void COverlayOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_overlay;
        }

        Array<SmartPtr<IOverlayElement>> COverlayOgre::getElements() const
        {
            return m_elements;
        }

        Vector2I COverlayOgre::getAbsoluteResolution() const
        {
            return Vector2I();
        }

        void COverlayOgre::setAbsoluteResolution( const Vector2I &val )
        {
        }

        bool COverlayOgre::isValid() const
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

        Array<SmartPtr<ISharedObject>> COverlayOgre::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( m_elements.size() + 12 );

            for( auto element : m_elements )
            {
                if( element )
                {
                    objects.push_back( element );
                }
            }

            return objects;
        }

        SmartPtr<Properties> COverlayOgre::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

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

            return properties;
        }

        void COverlayOgre::setProperties( SmartPtr<Properties> properties )
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

        COverlayOgre::OverlayStateListener::~OverlayStateListener()
        {
        }

        void COverlayOgre::OverlayStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void COverlayOgre::OverlayStateListener::handleStateChanged(
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
                    auto visible = objectMessage->isVisible();

                    m_owner->setVisible( visible );
                }
                else if( message->isExactly<StateMessageObject>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                    FB_ASSERT( objectMessage );

                    auto type = objectMessage->getType();
                    auto object = objectMessage->getObject();

                    if( type == IOverlayElement::STATE_MESSAGE_ATTACH_OBJECT )
                    {
                        m_owner->addElement( object );
                    }
                    else if( type == IOverlayElement::STATE_MESSAGE_DETACH_OBJECT )
                    {
                        m_owner->removeElement( object );
                    }
                }
            }
        }

        void COverlayOgre::OverlayStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        COverlayOgre *COverlayOgre::OverlayStateListener::getOwner() const
        {
            return m_owner;
        }

        void COverlayOgre::OverlayStateListener::setOwner( COverlayOgre *val )
        {
            m_owner = val;
        }
    }  // end namespace render
}  // end namespace fb
