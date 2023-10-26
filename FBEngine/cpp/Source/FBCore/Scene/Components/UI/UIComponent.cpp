#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/UI/IUILayout.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, UIComponent, Component );

        UIComponent::UIComponent()
        {
            auto elementListener = fb::make_ptr<UIElementListener>();
            elementListener->setOwner( this );
            setElementListener( elementListener );
        }

        UIComponent::~UIComponent()
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void UIComponent::load( SmartPtr<ISharedObject> data )
        {
            Component::load( data );
        }

        void UIComponent::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                if( renderUI )
                {
                    // auto parentContainer = getParentContainer();
                    // if (parentContainer)
                    //{
                    //	parentContainer->removeAllChildren();

                    //	renderUI->removeElement(parentContainer);

                    //	parentContainer->unload(nullptr);
                    //	parentContainer = nullptr;
                    //}
                }

                setElement( nullptr );
                setCanvas( nullptr );

                Component::unload( data );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IEventListener> UIComponent::getElementListener() const
        {
            return m_elementListener;
        }

        void UIComponent::setElementListener( SmartPtr<IEventListener> elementListener )
        {
            m_elementListener = elementListener;
        }

        SmartPtr<ui::IUIElement> UIComponent::getElement() const
        {
            return m_element;
        }

        void UIComponent::setElement( SmartPtr<ui::IUIElement> element )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto stateContext = applicationManager->getStateObject();

            if( m_element )
            {
                m_element->removeObjectListener( m_elementListener );
            }

            m_element = element;

            if( m_element )
            {
                m_element->addObjectListener( m_elementListener );
                m_element->setOwner( this );
            }
        }

        SmartPtr<UIComponent> UIComponent::getCanvas() const
        {
            return m_canvas;
        }

        void UIComponent::setCanvas( SmartPtr<UIComponent> canvas )
        {
            m_canvas = canvas;
        }

        void UIComponent::updateDimensions()
        {
        }

        void UIComponent::updateMaterials()
        {
        }

        void UIComponent::visibilityChanged()
        {
            if( auto actor = getActor() )
            {
                auto visible = actor->isVisible();

                if( auto element = getElement() )
                {
                    element->setVisible( visible );
                }
            }
        }

        void UIComponent::hierarchyChanged()
        {
            if( auto element = getElement() )
            {
                if( auto stateContext = element->getStateObject() )
                {
                    stateContext->setDirty( true );
                }
            }

            getActor()->updateTransform();
        }

        void UIComponent::childAddedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto element = getElement() )
            {
                if( auto stateContext = element->getStateObject() )
                {
                    stateContext->setDirty( true );
                }
            }

            getActor()->updateTransform();
        }

        void UIComponent::childRemovedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto element = getElement() )
            {
                if( auto stateContext = element->getStateObject() )
                {
                    stateContext->setDirty( true );
                }
            }

            getActor()->updateTransform();
        }

        Array<SmartPtr<ISharedObject>> UIComponent::getChildObjects() const
        {
            auto objects = Component::getChildObjects();

            objects.push_back( m_element );
            objects.push_back( m_canvas );
            return objects;
        }

        SmartPtr<Properties> UIComponent::getProperties() const
        {
            if( auto properties = Component::getProperties() )
            {
                properties->setProperty( "order", m_zOrder );
                properties->setProperty( "handleInputEvents", m_cascadeInput );
                properties->setProperty( "Reset", "ResetButton", "button", false );
                properties->setProperty( "Update Transform", "UpdateButton", "button", false );

                return properties;
            }

            return nullptr;
        }

        void UIComponent::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "order", m_zOrder );
            properties->getPropertyValue( "handleInputEvents", m_cascadeInput );

            static const auto updateTransformStr = String( "Update Transform" );
            if( properties->hasProperty( updateTransformStr ) )
            {
                auto &resetButton = properties->getPropertyObject( updateTransformStr );
                if( resetButton.getAttribute( "click" ) == "true" )
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto sceneManager =
                        fb::static_pointer_cast<SceneManager>( applicationManager->getSceneManager() );
                    FB_ASSERT( sceneManager );

                    auto actor = getActor();
                    // actor->updateTransform();

                    sceneManager->addDirtyTransform( actor );
                    // actor->setNewFlag( IActor::ActorFlagDirty, true );
                }
            }

            if( auto element = getElement() )
            {
                element->setHandleInputEvents( m_cascadeInput );
            }

            updateTransform();
            updateElementState();
        }

        void UIComponent::updateDirty( u32 flags, u32 oldFlags )
        {
            auto applicationManager = core::IApplicationManager::instance();

            if( auto actor = getActor() )
            {
                switch( auto eState = getState() )
                {
                case State::Edit:
                case State::Play:
                {
                    if( auto element = getElement() )
                    {
                        if( auto stateContext = element->getStateObject() )
                        {
                            stateContext->setDirty( true );
                        }
                    }

                    auto components = actor->getAllComponentsInChildren<UIComponent>();
                    for( auto &component : components )
                    {
                        if( auto element = component->getElement() )
                        {
                            if( auto stateContext = element->getStateObject() )
                            {
                                stateContext->setDirty( true );
                            }
                        }
                    }

                    updateElementState();
                    updateOrder();
                    updateVisibility();
                }
                }
            }
        }

        void UIComponent::updateTransform()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto applicationTask = applicationManager->getApplicationTask();

            auto task = Thread::getCurrentTask();

            if( applicationTask == task )
            {
                if( auto actor = getActor() )
                {
                    auto canvas = fb::static_pointer_cast<Layout>( getCanvas() );
                    auto referenceSize = Vector2I( 1920, 1080 );
                    if( canvas )
                    {
                        referenceSize = canvas->getReferenceSize();
                    }

                    auto canvasTransform = actor->getComponent<LayoutTransform>();
                    if( canvasTransform )
                    {
                        //auto element = getElement();
                        //if( element )
                        //{
                        //    auto position = canvasTransform->getAbsolutePosition();
                        //    auto size = canvasTransform->getSize();
                        //    auto relativeSize =
                        //        Vector2F( size.X() / static_cast<f32>( referenceSize.X() ),
                        //                  size.Y() / static_cast<f32>( referenceSize.Y() ) );

                        //    position = Vector2F::zero();

                        //    element->setPosition( position );
                        //    element->setSize( size );
                        //}

                        canvasTransform->updateTransform();
                    }
                }
            }
        }

        bool UIComponent::getCascadeInput() const
        {
            return m_cascadeInput;
        }

        void UIComponent::setCascadeInput( bool cascadeInput )
        {
            m_cascadeInput = cascadeInput;
        }

        Parameter UIComponent::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                            const Array<Parameter> &arguments,
                                            SmartPtr<ISharedObject> sender,
                                            SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto task = Thread::getCurrentTask();
            switch( task )
            {
            case Thread::Task::Application:
            {
                if( eventValue == IEvent::loadingStateChanged )
                {
                    auto newState = static_cast<LoadingState>( arguments[1].getS32() );
                    if( newState == LoadingState::Loaded )
                    {
                        auto eState = getState();
                        switch( eState )
                        {
                        case State::Edit:
                        case State::Play:
                        {
                        }
                        }
                    }
                }
            }
            break;
            }

            return Parameter();
        }

        IFSM::ReturnType UIComponent::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            Component::handleComponentEvent( state, eventType );

            switch( eventType )
            {
            case IFSM::Event::Change:
            {
            }
            break;
            case IFSM::Event::Enter:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Destroyed:
                {
                }
                break;
                case State::Edit:
                case State::Play:
                {
                    setupCanvas();

                    if( !getElement() )
                    {
                        createUI();
                    }

                    updateOrder();
                    updateTransform();
                    updateVisibility();
                    updateElementState();
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Play:
                {
                    // if (!m_meshObject)
                    //{
                    //	updateMesh();
                    //	updateMaterials();
                    //	updateTransform();
                    // }
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Pending:
            {
            }
            break;
            case IFSM::Event::Complete:
            {
            }
            break;
            case IFSM::Event::NewState:
            {
            }
            break;
            case IFSM::Event::WaitForChange:
            {
            }
            break;
            default:
            {
            }
            break;
            }

            return IFSM::ReturnType::Ok;
        }

        void UIComponent::updateOrder()
        {
            if( auto pCanvas = getCanvas() )
            {
                auto canvas = fb::static_pointer_cast<Layout>( pCanvas );
                if( canvas )
                {
                    if( auto layout = canvas->getLayout() )
                    {
                        if( auto element = getElement() )
                        {
                            element->setLayout( layout );

                            if( element->getParent() != layout )
                            {
                                layout->addChild( element );
                            }

                            auto order = canvas->getZOrder( getActor() );
                            element->setOrder( order );

                            setZOrder( order );
                        }
                        else
                        {
                            FB_LOG_ERROR( "Element null." );
                        }
                    }
                }
            }
        }

        u32 UIComponent::getZOrder() const
        {
            return m_zOrder;
        }

        void UIComponent::setZOrder( u32 zOrder )
        {
            m_zOrder = zOrder;
        }

        void UIComponent::childAdded( SmartPtr<IActor> child )
        {
            updateTransform();
        }

        void UIComponent::childRemoved( SmartPtr<IActor> child )
        {
            updateTransform();
        }

        void UIComponent::setupCanvas()
        {
            if( auto actor = getActor() )
            {
                if( auto rootActor = actor->getSceneRoot() )
                {
                    auto canvas = rootActor->getComponentAndInChildren<Layout>();
                    setCanvas( canvas );
                }
            }
        }

        void UIComponent::createUI()
        {
        }

        void UIComponent::updateVisibility()
        {
            auto applicationManager = core::IApplicationManager::instance();

            if( auto actor = getActor() )
            {
                auto enabled = isEnabled() && actor->isEnabledInScene();
                auto visible = applicationManager->isPlaying() ? enabled : enabled && actor->isVisible();

                if( auto element = getElement() )
                {
                    element->setEnabled( enabled, false );
                    element->setVisible( visible, false );
                }
            }

            if( auto canvas = getCanvas() )
            {
                canvas->updateVisibility();
            }
        }

        void UIComponent::updateElementState()
        {
        }

        Parameter UIComponent::UIElementListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( sender )
            {
                if( sender->isDerived<ui::IUIElement>() )
                {
                    auto element = fb::static_pointer_cast<ui::IUIElement>( sender );

                    if( auto pOwner = element->getOwner() )
                    {
                        if( auto owner = fb::dynamic_pointer_cast<UIComponent>( pOwner ) )
                        {
                            owner->handleEvent( eventType, eventValue, arguments, sender, object,
                                                event );
                        }
                    }
                }
            }

            if( object )
            {
                if( object->isDerived<ui::IUIElement>() )
                {
                    auto element = fb::static_pointer_cast<ui::IUIElement>( object );

                    if( auto pOwner = element->getOwner() )
                    {
                        if( auto owner = fb::dynamic_pointer_cast<UIComponent>( pOwner ) )
                        {
                            owner->handleEvent( eventType, eventValue, arguments, sender, object,
                                                event );
                        }
                    }
                }
            }

            return Parameter();
        }

        UIComponent *UIComponent::UIElementListener::getOwner() const
        {
            return m_owner;
        }

        void UIComponent::UIElementListener::setOwner( UIComponent *owner )
        {
            m_owner = owner;
        }
    }  // namespace scene
}  // namespace fb
