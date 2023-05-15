#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/COverlayElementOgre.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreOverlayElement.h>
#include <OgreOverlaySystem.h>

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::render, COverlayElementOgre, T, T );

        template <class T>
        void COverlayElementOgre<T>::createStateContext()
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

            auto listener = factoryManager->make_ptr<StateListenerOgre>();
            listener->setOwner( this );
            stateObject->addStateListener( listener );

            auto state = factoryManager->make_ptr<OverlayElementState>();
            stateObject->setState( state );

            stateObject->setOwner( this );
            COverlayElement<T>::setStateObject( stateObject );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );
        }

        template <class T>
        void COverlayElementOgre<T>::StateListenerOgre::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        template <class T>
        void COverlayElementOgre<T>::StateListenerOgre::handleStateChanged( SmartPtr<IState> &state )
        {
            auto overlayElementState = fb::static_pointer_cast<OverlayElementState>( state );

            auto position = overlayElementState->getPosition();
            auto size = overlayElementState->getSize();

            auto pOwner = COverlayElement<T>::ElementStateListener::getOwner();
            if( auto owner = fb::static_pointer_cast<COverlayElementOgre<T>>( pOwner ) )
            {
                if( auto element = owner->getElement() )
                {
                    auto gva = overlayElementState->getVerticalAlignment();
                    element->setVerticalAlignment( static_cast<Ogre::GuiVerticalAlignment>( gva ) );

                    auto gha = overlayElementState->getHorizontalAlignment();
                    element->setHorizontalAlignment( static_cast<Ogre::GuiHorizontalAlignment>( gha ) );

                    auto metricsMode = overlayElementState->getMetricsMode();
                    element->setMetricsMode( static_cast<Ogre::GuiMetricsMode>( metricsMode ) );

                    element->_setLeft( position.X() );
                    element->_setTop( position.Y() );

                    element->_setWidth( size.X() );
                    element->_setHeight( size.Y() );

                    if( overlayElementState->isVisible() )
                    {
                        element->show();
                    }
                    else
                    {
                        element->hide();
                    }

                    auto text = overlayElementState->getCaption();
                    element->setCaption( text );

                    auto colour = overlayElementState->getColour();
                    element->setColour( Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a ) );

                    auto material = overlayElementState->getMaterial();
                    owner->setupMaterial( material );

                    auto zOrder = overlayElementState->getZOrder();
                    element->_notifyZOrder( zOrder );

                    state->setDirty( false );
                }
            }
        }

        template <class T>
        void COverlayElementOgre<T>::StateListenerOgre::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        template class COverlayElementOgre<IOverlayElementText>;
        template class COverlayElementOgre<IOverlayElementContainer>;
    }  // end namespace render
}  // end namespace fb
