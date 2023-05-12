#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/CanvasTransform.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Layout, IComponent );

        Layout::Layout()
        {
            m_referenceSize = Vector2I( 1920, 1080 );
        }

        Layout::~Layout()
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void Layout::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                UIComponent::load( data );

                auto applicationManager = core::IApplicationManager::instance();
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

                    auto applicationManager = core::IApplicationManager::instance();
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

        void Layout::update()
        {
        }

        void Layout::awake()
        {
        }

        void Layout::play()
        {
            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();

                auto layout = getLayout();
                if( layout )
                {
                    layout->setVisible( visible );
                }
            }
        }

        void Layout::edit()
        {
            if( auto actor = getActor() )
            {
                auto visible = isEnabled() && actor->isEnabledInScene();

                auto layout = getLayout();
                if( layout )
                {
                    layout->setVisible( visible );
                }
            }
        }

        SmartPtr<ui::IUILayout> Layout::getLayout() const
        {
            return m_layout;
        }

        void Layout::setLayout( SmartPtr<ui::IUILayout> val )
        {
            m_layout = val;
        }

        Vector2I Layout::getReferenceSize() const
        {
            return m_referenceSize;
        }

        void Layout::setReferenceSize( const Vector2I &val )
        {
            m_referenceSize = val;
        }

        Array<SmartPtr<ISharedObject>> Layout::getChildObjects() const
        {
            auto objects = UIComponent::getChildObjects();
            objects.push_back( m_layout );
            return objects;
        }

        SmartPtr<Properties> Layout::getProperties() const
        {
            auto properties = UIComponent::getProperties();
            properties->setProperty( "referenceSize", m_referenceSize );
            return properties;
        }

        void Layout::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "referenceSize", m_referenceSize );
        }

        void Layout::updateDirty( u32 flags, u32 oldFlags )
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

                    auto canvasTransforms = actor->getComponentsAndInChildren<CanvasTransform>();
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

                    auto canvasTransforms = actor->getComponentsAndInChildren<CanvasTransform>();
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

                    auto canvasTransforms = actor->getComponentsAndInChildren<CanvasTransform>();
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
                        if( auto stateContext = element->getStateObject() )
                        {
                            stateContext->setDirty( true );
                        }
                    }

                    for( auto child : children )
                    {
                        if( auto element = child->getElement() )
                        {
                            if( auto stateContext = element->getStateObject() )
                            {
                                stateContext->setDirty( true );
                            }
                        }
                    }
                }
                };
            }
        }

        IFSM::ReturnType Layout::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            BaseComponent::handleComponentEvent( state, eventType );

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
                    if( auto actor = getActor() )
                    {
                        auto canvasTransforms = actor->getComponentsAndInChildren<CanvasTransform>();
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
                        if( auto stateContext = element->getStateObject() )
                        {
                            stateContext->setDirty( true );
                        }
                    }

                    if( auto actor = getActor() )
                    {
                        auto children = actor->getComponentsInChildren<UIComponent>();
                        for( auto child : children )
                        {
                            if( auto element = child->getElement() )
                            {
                                if( auto stateContext = element->getStateObject() )
                                {
                                    stateContext->setDirty( true );
                                }
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

        u32 Layout::getElementOrder( SmartPtr<UIComponent> component ) const
        {
            if( auto actor = getActor() )
            {
                auto elements = actor->getAllComponentsInChildren<UIComponent>();
                auto it = std::find( elements.begin(), elements.end(), component );
                if( it != elements.end() )
                {
                    return std::distance( elements.begin(), it );
                }
            }

            return 0;
        }

        u32 Layout::getElementOrderReversed( SmartPtr<UIComponent> component ) const
        {
            if( auto actor = getActor() )
            {
                auto elements = actor->getAllComponentsInChildren<UIComponent>();
                auto it = std::find( elements.begin(), elements.end(), component );
                if( it != elements.end() )
                {
                    return elements.size() - std::distance( elements.begin(), it );
                }
            }

            return 0;
        }

        void Layout::childAdded( SmartPtr<IActor> child )
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

        void Layout::childRemoved( SmartPtr<IActor> child )
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

        void Layout::childAddedInHierarchy( SmartPtr<IActor> child )
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

        void Layout::childRemovedInHierarchy( SmartPtr<IActor> child )
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

        s32 Layout::getZOrder( SmartPtr<IActor> obj )
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

        u32 Layout::getZOrder() const
        {
            return m_zOrder;
        }

        void Layout::levelWasLoaded( s32 level )
        {
        }

        void Layout::setZOrder( u32 zOrder )
        {
            m_zOrder = zOrder;
        }

    }  // namespace scene
}  // end namespace fb
