#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreViewport.h>

namespace fb
{
    namespace render
    {

        CViewportOgreNext::CViewportOgreNext()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto stateManager = applicationManager->getStateManager();
                auto threadPool = applicationManager->getThreadPool();
                auto factoryManager = applicationManager->getFactoryManager();

                auto state = factoryManager->make_ptr<ViewportState>();

                auto stateTask = graphicsSystem->getStateTask();
                state->setTaskId( stateTask );

                auto stateObject = stateManager->addStateObject();

                auto viewportStateListener = factoryManager->make_ptr<ViewportStateListener>();
                viewportStateListener->setOwner( this );
                setStateListener( viewportStateListener );

                stateObject->setState( state );
                stateObject->addStateListener( viewportStateListener );
                setStateObject( stateObject );
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

        void CViewportOgreNext::update()
        {
            // m_viewport->update();
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
        }

        void CViewportOgreNext::ViewportStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CViewportOgreNext *CViewportOgreNext::ViewportStateListener::getOwner() const
        {
            return m_owner;
        }

        void CViewportOgreNext::ViewportStateListener::setOwner( CViewportOgreNext *owner )
        {
            m_owner = owner;
        }

    }  // end namespace render
}  // end namespace fb
