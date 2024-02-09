#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/UIComponent.h>
#include "FBCore/Jobs/EventJob.h"
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/UI/IUILayout.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
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
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto renderUI = applicationManager->getRenderUI();
            FB_ASSERT( renderUI );

            if( auto element = getElement() )
            {
                renderUI->removeElement( element );
                setElement( nullptr );
            }

            setCanvas( nullptr );

            Component::unload( data );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto UIComponent::getElementListener() const -> SmartPtr<IEventListener>
    {
        return m_elementListener;
    }

    void UIComponent::setElementListener( SmartPtr<IEventListener> elementListener )
    {
        m_elementListener = elementListener;
    }

    auto UIComponent::getElement() const -> SmartPtr<ui::IUIElement>
    {
        return m_element;
    }

    void UIComponent::setElement( SmartPtr<ui::IUIElement> element )
    {
#if !FB_FINAL
        if( ( m_element && element ) == true )
        {
            FB_LOG( "Element already set." );
        }
#endif

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

    auto UIComponent::getCanvas() const -> SmartPtr<UIComponent>
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

    auto UIComponent::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Component::getChildObjects();

        objects.emplace_back( m_element );
        objects.emplace_back( m_canvas );
        return objects;
    }

    auto UIComponent::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = Component::getProperties() )
        {
            properties->setProperty( "colour", m_colour );
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
        Component::setProperties( properties );

        auto applicationManager = core::ApplicationManager::instance();

        auto sceneManager =
            fb::static_pointer_cast<SceneManager>( applicationManager->getSceneManager() );
        FB_ASSERT( sceneManager );

        properties->getPropertyValue( "colour", m_colour );

        properties->getPropertyValue( "order", m_zOrder );
        properties->getPropertyValue( "handleInputEvents", m_cascadeInput );

        static const auto updateTransformStr = String( "Update Transform" );
        if( properties->hasProperty( updateTransformStr ) )
        {
            auto &resetButton = properties->getPropertyObject( updateTransformStr );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
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

        if( auto actor = getActor() )
        {
            auto enabled = isEnabled() && actor->isEnabledInScene();
            auto visible = applicationManager->isPlaying() ? enabled : enabled && actor->isVisible();
            if( visible )
            {
                createUI();
            }
        }

        updateVisibility();
        updateColour();
    }

    void UIComponent::updateFlags( u32 flags, u32 oldFlags )
    {
        auto applicationManager = core::ApplicationManager::instance();

        if( auto actor = getActor() )
        {
            switch( auto eState = getState() )
            {
            case State::Edit:
            case State::Play:
            {
                if( auto element = getElement() )
                {
                    if( auto stateContext = element->getStateContext() )
                    {
                        stateContext->setDirty( true );
                    }
                }

                auto components = actor->getAllComponentsInChildren<UIComponent>();
                for( auto &component : components )
                {
                    if( auto element = component->getElement() )
                    {
                        if( auto stateContext = element->getStateContext() )
                        {
                            stateContext->setDirty( true );
                        }
                    }
                }

                updateElementState();
                updateOrder();
                updateVisibility();

                if( auto root = actor->getSceneRoot() )
                {
                    root->updateTransform();
                }
            }
            break;
            }
        }
    }

    void UIComponent::updateTransform()
    {
        auto applicationManager = core::ApplicationManager::instance();
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
                    //        Vector2<real_Num>( size.X() / static_cast<f32>( referenceSize.X() ),
                    //                  size.Y() / static_cast<f32>( referenceSize.Y() ) );

                    //    position = Vector2<real_Num>::zero();

                    //    element->setPosition( position );
                    //    element->setSize( size );
                    //}

                    canvasTransform->updateTransform();
                }
            }
        }
    }

    auto UIComponent::getCascadeInput() const -> bool
    {
        return m_cascadeInput;
    }

    void UIComponent::setCascadeInput( bool cascadeInput )
    {
        m_cascadeInput = cascadeInput;
    }

    auto UIComponent::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == visibilityChanged )
        {
            if( auto actor = getActor() )
            {
                auto visible = actor->isVisible();
                if( visible )
                {
                    auto element = getElement();
                    if( !element )
                    {
                        createUI();
                    }
                }

                if( auto element = getElement() )
                {
                    element->setVisible( visible );
                }
            }
        }
        else if( eventValue == hierarchyChanged )
        {
            if( auto element = getElement() )
            {
                if( auto stateContext = element->getStateContext() )
                {
                    stateContext->setDirty( true );
                }
            }

            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();
                if( visible )
                {
                    auto element = getElement();
                    if( !element )
                    {
                        createUI();
                    }
                }

                actor->updateTransform();
            }
        }
        else if( eventValue == parentChanged )
        {
            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();
                if( visible )
                {
                    auto element = getElement();
                    if( !element )
                    {
                        createUI();
                    }
                }

                actor->updateTransform();
            }
        }
        else if( eventValue == childAddedInHierarchy )
        {
            if( auto element = getElement() )
            {
                if( auto stateContext = element->getStateContext() )
                {
                    stateContext->setDirty( true );
                }
            }

            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();
                if( visible )
                {
                    auto element = getElement();
                    if( !element )
                    {
                        createUI();
                    }
                }

                actor->updateTransform();
            }
        }
        else if( eventValue == childRemovedInHierarchy )
        {
            if( auto element = getElement() )
            {
                if( auto stateContext = element->getStateContext() )
                {
                    stateContext->setDirty( true );
                }
            }

            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();
                if( visible )
                {
                    auto element = getElement();
                    if( !element )
                    {
                        createUI();
                    }
                }

                actor->updateTransform();
            }
        }
        else if( eventValue == actorReset )
        {
            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();
                if( visible )
                {
                    auto element = getElement();
                    if( !element )
                    {
                        createUI();
                    }
                }
            }
        }

        return Component::handleEvent( eventType, eventValue, arguments, sender, object, event );
    }

    auto UIComponent::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
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
            case State::Edit:
            case State::Play:
            {
                if( auto element = getElement() )
                {
                    if( auto canvas = fb::static_pointer_cast<Layout>( getCanvas() ) )
                    {
                        if( auto layout = canvas->getLayout() )
                        {
                            layout->removeChild( element );
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

    auto UIComponent::getZOrder() const -> u32
    {
        return m_zOrder;
    }

    void UIComponent::setZOrder( u32 zOrder )
    {
        m_zOrder = zOrder;
    }

    auto UIComponent::getActorListeners() const -> Array<SmartPtr<IActor>>
    {
        return m_actorListeners;
    }

    void UIComponent::removeListener( SmartPtr<IComponent> component )
    {
        m_componentListeners.erase(
            std::remove( m_componentListeners.begin(), m_componentListeners.end(), component ),
            m_componentListeners.end() );
    }

    void UIComponent::removeActorListener( SmartPtr<IActor> actor )
    {
        m_actorListeners.erase( std::remove( m_actorListeners.begin(), m_actorListeners.end(), actor ),
                                m_actorListeners.end() );
    }

    auto UIComponent::getComponentListeners() const -> Array<SmartPtr<IComponent>>
    {
        return m_componentListeners;
    }

    void UIComponent::addListener( SmartPtr<IComponent> component )
    {
        m_componentListeners.push_back( component );
    }

    void UIComponent::addActorListener( SmartPtr<IActor> actor )
    {
        m_actorListeners.push_back( actor );
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
        auto applicationManager = core::ApplicationManager::instance();

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

        //if( auto canvas = getCanvas() )
        //{
        //    canvas->updateVisibility();
        //}
    }

    void UIComponent::updateElementState()
    {
    }

    void UIComponent::updateColour()
    {
        if( m_element )
        {
            m_element->setColour( m_colour );
        }
    }

    void UIComponent::setColour( const ColourF &colour )
    {
        m_colour = colour;
    }

    ColourF UIComponent::getColour() const
    {
        return m_colour;
    }

    auto UIComponent::UIElementListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                      const Array<Parameter> &arguments,
                                                      SmartPtr<ISharedObject> sender,
                                                      SmartPtr<ISharedObject> object,
                                                      SmartPtr<IEvent> event ) -> Parameter
    {
        auto &applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto taskManager = applicationManager->getTaskManager();
        auto applicationTask = taskManager->getTask( Thread::Task::Application );
        auto task = Thread::getCurrentTask();

        if( task == Thread::Task::Application )
        {
            //if( sender )
            //{
            //    if( sender->isDerived<ui::IUIElement>() )
            //    {
            //        auto element = fb::static_pointer_cast<ui::IUIElement>( sender );

            //        if( auto pOwner = element->getOwner() )
            //        {
            //            if( auto owner = fb::dynamic_pointer_cast<UIComponent>( pOwner ) )
            //            {
            //                owner->handleEvent( eventType, eventValue, arguments, sender, object,
            //                                    event );
            //            }
            //        }
            //    }
            //}

            //if( object )
            //{
            //    if( object->isDerived<ui::IUIElement>() )
            //    {
            //        auto element = fb::static_pointer_cast<ui::IUIElement>( object );

            //        if( auto pOwner = element->getOwner() )
            //        {
            //            if( auto owner = fb::dynamic_pointer_cast<UIComponent>( pOwner ) )
            //            {
            //                owner->handleEvent( eventType, eventValue, arguments, sender, object,
            //                                    event );
            //            }
            //        }
            //    }
            //}
            if( eventValue == IEvent::CLICK_HASH )
            {
                int stop = 0;
                stop = 0;

                auto actor = m_owner->getActor();

                //if( auto element = fb::static_pointer_cast<ui::IUIElement>( sender ) )
                //{
                //    if( auto pOwner = element->getOwner() )
                //    {
                //        if( auto owner = fb::dynamic_pointer_cast<UIComponent>( pOwner ) )
                //        {
                //            owner->handleEvent( eventType, eventValue, arguments, sender, object,
                //                                event );
                //        }
                //    }
                //}
            }

            //FB_ASSERT( task == Thread::Task::Application );

            auto owner = getOwner();
            auto actors = owner->getActorListeners();
            for( auto actor : actors )
            {
                actor->handleEvent( eventType, eventValue, arguments, sender, getOwner(), event );
            }

            if( auto actor = owner->getActor() )
            {
                if( auto p = actor->getComponentsPtr() )
                {
                    auto &components = *p;
                    for( auto component : components )
                    {
                        component->handleEvent( eventType, eventValue, arguments, sender, object,
                                                event );
                    }
                }
            }

            auto components = owner->getComponentListeners();
            for( auto component : components )
            {
                component->handleEvent( eventType, eventValue, arguments, sender, getOwner(), event );
            }
        }

        return {};
    }

    auto UIComponent::UIElementListener::getOwner() const -> SmartPtr<UIComponent>
    {
        return m_owner;
    }

    void UIComponent::UIElementListener::setOwner( SmartPtr<UIComponent> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::scene
