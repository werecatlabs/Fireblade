#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreViewport.h>

namespace fb::render
{

    CViewportOgreNext::CViewportOgreNext()
    {
        try
        {
            if( auto handle = getHandle() )
            {
                handle->setName( "CViewportOgreNext" );
            }

            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto stateManager = applicationManager->getStateManager();
            auto threadPool = applicationManager->getThreadPool();
            auto factoryManager = applicationManager->getFactoryManager();

            auto state = factoryManager->make_ptr<ViewportState>();

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );

            auto stateContext = stateManager->addStateObject();

            auto viewportStateListener = factoryManager->make_ptr<ViewportStateListener>();
            viewportStateListener->setOwner( this );
            setStateListener( viewportStateListener );

            auto mask = 0xFFFFFFFF & ( ~Ogre::VisibilityFlags::LAYER_VISIBILITY );
            state->setVisibilityMask( mask );

            stateContext->addState( state );
            stateContext->addStateListener( viewportStateListener );
            setStateContext( stateContext );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    CViewportOgreNext::~CViewportOgreNext()
    {
        unload( nullptr );
    }

    void CViewportOgreNext::initialise( Ogre::Viewport *viewport )
    {
        m_viewport = viewport;
    }

    void CViewportOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_viewport;
    }

    void CViewportOgreNext::ViewportStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
    }

    void CViewportOgreNext::ViewportStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        state->setDirty( false );
    }

    void CViewportOgreNext::ViewportStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto CViewportOgreNext::ViewportStateListener::getOwner() const -> CViewportOgreNext *
    {
        return m_owner;
    }

    void CViewportOgreNext::ViewportStateListener::setOwner( CViewportOgreNext *owner )
    {
        m_owner = owner;
    }

}  // namespace fb::render
