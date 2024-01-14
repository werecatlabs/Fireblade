#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementOgre.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>

namespace fb::render
{

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::render, COverlayElementOgre, T, T );

    template <class T>
    void COverlayElementOgre<T>::createStateContext()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();

        auto listener = factoryManager->make_ptr<StateListenerOgre>();
        listener->setOwner( this );
        stateContext->addStateListener( listener );

        auto state = factoryManager->make_ptr<OverlayElementState>();
        stateContext->addState( state );

        stateContext->setOwner( this );
        OverlayElement<T>::setStateContext( stateContext );

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

        auto pOwner = OverlayElement<T>::ElementStateListener::getOwner();
        if( auto owner = fb::static_pointer_cast<COverlayElementOgre<T>>( pOwner ) )
        {
            if( auto element = owner->getElement() )
            {
                auto gva = overlayElementState->getVerticalAlignment();
                element->setVerticalAlignment( static_cast<Ogre::v1::GuiVerticalAlignment>( gva ) );

                auto gha = overlayElementState->getHorizontalAlignment();
                element->setHorizontalAlignment( static_cast<Ogre::v1::GuiHorizontalAlignment>( gha ) );

                auto metricsMode = overlayElementState->getMetricsMode();
                element->setMetricsMode( static_cast<Ogre::v1::GuiMetricsMode>( metricsMode ) );

                element->_setLeft( position.X() );
                element->_setTop( position.Y() );

                element->_setWidth( size.X() );
                element->_setHeight( size.Y() );

                auto visible = overlayElementState->isVisible();
                if( visible != element->isVisible() )
                {
                    if( visible )
                    {
                        if( auto overlay =
                                fb::static_pointer_cast<COverlayOgreNext>( owner->getOverlay() ) )
                        {
                            Ogre::v1::Overlay *ogreOverlay;
                            overlay->_getObject( reinterpret_cast<void **>( &ogreOverlay ) );

                            element->_notifyParent( element->getParent(), ogreOverlay );
                        }

                        element->show();
                    }
                    else
                    {
                        element->hide();
                    }
                }

                auto text = overlayElementState->getCaption();
                element->setCaption( text );

                auto colour = overlayElementState->getColour();
                element->setColour( Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a ) );

                auto material = overlayElementState->getMaterial();
                owner->setupMaterial( material );

                auto zOrder = overlayElementState->getZOrder();
                auto ratio = static_cast<f32>( zOrder ) / 6.0f;
                ratio = MathF::max( ratio, 1.0f );

                constexpr int SubRqIdBits = 3;
                constexpr auto max_value = ( 1u << SubRqIdBits ) - 1u;
                //element->_notifyZOrder( zOrder );

                auto order = static_cast<u32>( ratio * static_cast<f32>( max_value ) );
                element->setRenderQueueSubGroup( zOrder );

                //element->_positionsOutOfDate();
                //element->_notifyViewport();
                //element->_update();

                //element->updatePositionGeometry();
                //element->updateTextureGeometry();

                state->setDirty( false );
            }
        }
    }

    template <class T>
    void COverlayElementOgre<T>::StateListenerOgre::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    template <class T>
    auto COverlayElementOgre<T>::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = OverlayElement<T>::getProperties();
        if( auto element = getElement() )
        {
            auto text = element->getCaption();
            if( !text.empty() )
            {
                properties->setProperty( "Text", text );
            }

            auto colour = element->getColour();
            properties->setProperty( "Colour", ColourF( colour.r, colour.g, colour.b, colour.a ) );

            auto visible = element->isVisible();
            properties->setProperty( "visible", visible );
        }

        return properties;
    }

    template <class T>
    void COverlayElementOgre<T>::setProperties( SmartPtr<Properties> properties )
    {
        OverlayElement<T>::setProperties( properties );

        if( auto element = getElement() )
        {
            auto text = properties->getProperty( "Text" );
            element->setCaption( text );

            ColourF colour;
            properties->getPropertyValue( "Colour", colour );
            auto colourValue = Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a );
            element->setColour( colourValue );

            //auto materialName = properties->getProperty( "Material" );
            //if( material )
            //{
            //    setupMaterial( materialName );
            //}

            //auto zOrder = properties->getProperty( "ZOrder" );
            //if( zOrder )
            //{
            //    auto value = zOrder->getValue();
            //    auto ratio = (f32)value / 6.0f;
            //    ratio = MathF::max( ratio, 1.0f );

            //    constexpr int SubRqIdBits = 3;
            //    constexpr auto max_value = ( 1u << SubRqIdBits ) - 1u;
            //    //element->_notifyZOrder( zOrder );

            //    auto order = (u32)( ratio * (f32)max_value );
            //    element->setRenderQueueSubGroup( order );
            //}

            auto visible = element->isVisible();

            properties->getPropertyValue( "visible", visible );

            if( element->isVisible() != visible )
            {
                if( visible )
                {
                    element->show();
                }
                else
                {
                    element->hide();
                }
            }
        }
    }

    template class COverlayElementOgre<IOverlayElementText>;
    template class COverlayElementOgre<IOverlayElementContainer>;

}  // namespace fb::render
