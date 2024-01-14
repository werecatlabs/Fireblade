#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/UI/IUIContainer.h>
#include <FBCore/Interface/UI/IUILayout.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIImage.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Layout, IComponent );

    Layout::Layout()
    {
        m_referenceSize = Vector2I( 1920, 1080 );
    }

    Layout::~Layout()
    {
        unload( nullptr );
    }

    void Layout::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            UIComponent::load( data );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto renderUI = applicationManager->getRenderUI();
            FB_ASSERT( renderUI );

            auto layout = renderUI->addElementByType<ui::IUILayout>();
            FB_ASSERT( layout );

            setLayout( layout );
            setElement( layout );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Layout::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                FB_ASSERT( renderUI );

                if( auto layout = getLayout() )
                {
                    layout->removeAllChildren();
                    layout->setContainer( nullptr );

                    // auto container = getContainer();
                    // if (container)
                    //{
                    //	layout->removeChild(container);
                    // }

                    renderUI->removeElement( layout );
                    setLayout( nullptr );
                }

                UIComponent::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Layout::getLayout() const -> SmartPtr<ui::IUILayout>
    {
        return m_layout;
    }

    void Layout::setLayout( SmartPtr<ui::IUILayout> layout )
    {
        m_layout = layout;
    }

    auto Layout::getReferenceSize() const -> Vector2I
    {
        return m_referenceSize;
    }

    void Layout::setReferenceSize( const Vector2I &referenceSize )
    {
        m_referenceSize = referenceSize;
    }

    auto Layout::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = UIComponent::getChildObjects();
        objects.emplace_back( m_layout );
        return objects;
    }

    auto Layout::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = UIComponent::getProperties();
        properties->setProperty( "referenceSize", m_referenceSize );
        return properties;
    }

    void Layout::setProperties( SmartPtr<Properties> properties )
    {
        UIComponent::setProperties( properties );

        properties->getPropertyValue( "referenceSize", m_referenceSize );

        if( auto actor = getActor() )
        {
            if( auto transform = actor->getComponent<LayoutTransform>() )
            {
                auto sz = Vector2F( static_cast<f32>( m_referenceSize.X() ),
                                    static_cast<f32>( m_referenceSize.Y() ) );
                transform->setSize( sz );
            }
        }
    }

    void Layout::updateFlags( u32 flags, u32 oldFlags )
    {
        if( auto actor = getActor() )
        {
            if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();

                if( auto layout = getLayout() )
                {
                    layout->setVisible( visible );
                }

                auto canvasTransforms = actor->getComponentsAndInChildren<LayoutTransform>();
                for( auto canvasTransform : canvasTransforms )
                {
                    canvasTransform->updateTransform();
                }
            }
            else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                     BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();

                if( auto layout = getLayout() )
                {
                    layout->setVisible( visible );
                }

                auto canvasTransforms = actor->getComponentsAndInChildren<LayoutTransform>();
                for( auto canvasTransform : canvasTransforms )
                {
                    canvasTransform->updateTransform();
                }
            }
            else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabledInScene ) !=
                     BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabledInScene ) )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();

                if( auto layout = getLayout() )
                {
                    layout->setVisible( visible );
                }

                auto canvasTransforms = actor->getComponentsAndInChildren<LayoutTransform>();
                for( auto canvasTransform : canvasTransforms )
                {
                    canvasTransform->updateTransform();
                }
            }

            auto eState = getState();
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                auto children = actor->getAllComponentsInChildren<UIComponent>();
                for( auto child : children )
                {
                    child->updateOrder();
                }

                if( auto element = getElement() )
                {
                    element->updateZOrder();
                }

                for( auto child : children )
                {
                    if( auto element = child->getElement() )
                    {
                        child->updateOrder();
                    }
                }

                if( auto element = getElement() )
                {
                    if( auto stateContext = element->getStateContext() )
                    {
                        stateContext->setDirty( true );
                    }
                }

                for( auto child : children )
                {
                    if( auto element = child->getElement() )
                    {
                        if( auto stateContext = element->getStateContext() )
                        {
                            stateContext->setDirty( true );
                        }
                    }
                }
            }
            };
        }
    }

    auto Layout::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        Component::handleComponentEvent( state, eventType );

        switch( eventType )
        {
        case IFSM::Event::Change:
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
                auto actor = getActor();
                if( actor )
                {
                    auto canvasTransforms = actor->getComponentsAndInChildren<LayoutTransform>();
                    for( auto canvasTransform : canvasTransforms )
                    {
                        canvasTransform->updateTransform();
                    }

                    auto visible = isEnabled() && actor->isEnabledInScene();
                    if( auto layout = getLayout() )
                    {
                        layout->setVisible( visible );
                    }
                }

                updateOrder();

                if( auto element = getElement() )
                {
                    if( auto stateContext = element->getStateContext() )
                    {
                        stateContext->setDirty( true );
                    }
                }

                if( actor )
                {
                    auto children = actor->getComponentsInChildren<UIComponent>();
                    for( auto child : children )
                    {
                        if( auto element = child->getElement() )
                        {
                            if( auto stateContext = element->getStateContext() )
                            {
                                stateContext->setDirty( true );
                            }
                        }
                    }

                    if( auto transform = actor->getComponent<LayoutTransform>() )
                    {
                        transform->setSize( Vector2F( 1920, 1080 ) );
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
            break;
        case IFSM::Event::Complete:
        {
        }
        break;
        case IFSM::Event::NewState:
            break;
        case IFSM::Event::WaitForChange:
            break;
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    void Layout::updateVisibility()
    {
        if( auto actor = getActor() )
        {
            auto visible = isEnabled() && actor->isEnabledInScene();
            if( auto layout = getLayout() )
            {
                layout->setVisible( visible );
            }
        }
    }

    auto Layout::getElementOrder( SmartPtr<UIComponent> component ) const -> u32
    {
        if( auto actor = getActor() )
        {
            auto elements = actor->getAllComponentsInChildren<UIComponent>();
            auto it = std::find( elements.begin(), elements.end(), component );
            if( it != elements.end() )
            {
                return static_cast<u32>( std::distance( elements.begin(), it ) );
            }
        }

        return 0;
    }

    auto Layout::getElementOrderReversed( SmartPtr<UIComponent> component ) const -> u32
    {
        if( auto actor = getActor() )
        {
            auto elements = actor->getAllComponentsInChildren<UIComponent>();
            auto it = std::find( elements.begin(), elements.end(), component );
            if( it != elements.end() )
            {
                return static_cast<u32>( elements.size() - std::distance( elements.begin(), it ) );
            }
        }

        return 0;
    }

    auto Layout::getZOrder( SmartPtr<IActor> obj ) -> s32
    {
        if( auto actor = getActor() )
        {
            auto children = actor->getAllComponentsInChildren<UIComponent>();

            auto count = 0;
            for( auto &child : children )
            {
                if( child->getActor() == obj )
                {
                    return count;
                }

                ++count;
            }
        }

        return 0;
    }

    auto Layout::handleEvent( IEvent::Type eventType, hash_type eventValue,
                              const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                              SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IComponent::childAdded )
        {
            auto eState = getState();
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                updateOrder();

                if( auto actor = getActor() )
                {
                    auto children = actor->getComponentsInChildren<UIComponent>();
                    for( auto child : children )
                    {
                        child->updateOrder();
                    }

                    if( auto element = getElement() )
                    {
                        element->updateZOrder();
                    }
                }
            }
            };
        }
        else if( eventValue == IComponent::childRemoved )
        {
            auto eState = getState();
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                updateOrder();

                if( auto actor = getActor() )
                {
                    auto children = actor->getComponentsInChildren<UIComponent>();
                    for( auto child : children )
                    {
                        child->updateOrder();
                    }

                    if( auto element = getElement() )
                    {
                        element->updateZOrder();
                    }
                }
            }
            };
        }
        else if( eventValue == IComponent::childAddedInHierarchy )
        {
            auto eState = getState();
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                updateOrder();

                if( auto actor = getActor() )
                {
                    auto children = actor->getComponentsInChildren<UIComponent>();
                    for( auto child : children )
                    {
                        child->updateOrder();
                    }

                    if( auto element = getElement() )
                    {
                        element->updateZOrder();
                    }
                }
            }
            };
        }
        else if( eventValue == IComponent::childRemovedInHierarchy )
        {
            auto eState = getState();
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                updateOrder();

                if( auto actor = getActor() )
                {
                    auto children = actor->getComponentsInChildren<UIComponent>();
                    for( auto child : children )
                    {
                        child->updateOrder();
                    }

                    if( auto element = getElement() )
                    {
                        element->updateZOrder();
                    }
                }
            }
            };
        }
        else if( eventValue == IComponent::sceneWasLoaded )
        {
            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();

                if( auto layout = getLayout() )
                {
                    layout->setVisible( visible );

                    if( auto stateContext = layout->getStateContext() )
                    {
                        stateContext->setDirty( true );
                    }
                }

                if( auto element = getElement() )
                {
                    if( auto stateContext = element->getStateContext() )
                    {
                        stateContext->setDirty( true );
                    }
                }

                auto children = actor->getAllComponentsInChildren<UIComponent>();
                for( auto child : children )
                {
                    if( auto element = child->getElement() )
                    {
                        if( auto stateContext = element->getStateContext() )
                        {
                            stateContext->setDirty( true );
                        }
                    }
                }
            }
        }

        return {};
    }

}  // namespace fb::scene
