#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Actor.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Transform.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Util.h>
#include <FBCore/Core/Properties.h>
#include <rttr/registration>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Actor, Resource<IActor> );
    u32 Actor::m_idExt = 0;

    Actor::Actor()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "Actor" );
        }

        setResourceType( ResourceType::Actor );

        m_children = fb::make_shared<ConcurrentArray<SmartPtr<IActor>>>();
        m_components = fb::make_shared<ConcurrentArray<SmartPtr<IComponent>>>();
    }

    Actor::~Actor()
    {
        unload( nullptr );
    }

    auto Actor::getName() const -> String
    {
        auto handle = getHandle();
        FB_ASSERT( handle );

        return handle->getName();
    }

    void Actor::setName( const String &name )
    {
        auto handle = getHandle();
        FB_ASSERT( handle );

        handle->setName( name );
    }

    auto Actor::getWorldTransform( time_interval t ) const -> Transform3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getWorldTransform();
        }

        return {};
    }

    auto Actor::getWorldTransform() const -> Transform3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getWorldTransform();
        }

        return {};
    }

    auto Actor::getLocalTransform( time_interval t ) const -> Transform3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getLocalTransform();
        }

        return {};
    }

    auto Actor::getLocalTransform() const -> Transform3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getLocalTransform();
        }

        return {};
    }

    auto Actor::getLocalPosition() const -> Vector3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getLocalPosition();
        }

        return Vector3<real_Num>::zero();
    }

    void Actor::setLocalPosition( const Vector3<real_Num> &localPosition )
    {
        if( auto &transform = getTransform() )
        {
            transform->setLocalPosition( localPosition );
            transform->setDirty( true );
        }

        updateTransform();
    }

    auto Actor::getLocalScale() const -> Vector3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getLocalScale();
        }

        return Vector3<real_Num>::zero();
    }

    void Actor::setLocalScale( const Vector3<real_Num> &localScale )
    {
        if( auto &transform = getTransform() )
        {
            transform->setLocalScale( localScale );
            transform->setDirty( true );
        }

        updateTransform();
    }

    auto Actor::getLocalOrientation() const -> Quaternion<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getLocalOrientation();
        }

        return Quaternion<real_Num>::identity();
    }

    void Actor::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
    {
        if( auto &transform = getTransform() )
        {
            transform->setLocalOrientation( localOrientation );
            transform->setDirty( true );
        }

        updateTransform();
    }

    auto Actor::getPosition() const -> Vector3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getPosition();
        }

        return Vector3<real_Num>::zero();
    }

    void Actor::lookAt( const Vector3<real_Num> &position )
    {
        auto vec = position - getPosition();
        auto rot = MathUtil<real_Num>::getRotationTo( -Vector3<real_Num>::unitZ(), vec );
        setOrientation( rot );
    }

    void Actor::lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis )
    {
        auto vec = position - getPosition();
        auto rot = MathUtil<real_Num>::getOrientationFromDirection( vec, -Vector3<real_Num>::unitZ(),
                                                                    true, yawAxis );
        setOrientation( rot );
    }

    void Actor::setPosition( const Vector3<real_Num> &position )
    {
        if( auto &transform = getTransform() )
        {
            transform->setPosition( position );
            transform->setLocalDirty( true );
        }

        updateTransform();
    }

    auto Actor::getScale() const -> Vector3<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getScale();
        }

        return Vector3<real_Num>::zero();
    }

    void Actor::setScale( const Vector3<real_Num> &scale )
    {
        if( auto &transform = getTransform() )
        {
            transform->setScale( scale );
            transform->setLocalDirty( true );
        }

        updateTransform();
    }

    auto Actor::getOrientation() const -> Quaternion<real_Num>
    {
        if( auto &transform = getTransform() )
        {
            return transform->getOrientation();
        }

        return Quaternion<real_Num>::identity();
    }

    void Actor::setOrientation( const Quaternion<real_Num> &orientation )
    {
        if( auto &transform = getTransform() )
        {
            transform->setOrientation( orientation );
            transform->setLocalDirty( true );
        }

        updateTransform();
    }

    void Actor::levelWasLoaded( SmartPtr<IScene> scene )
    {
        try
        {
            auto components = getComponents();
            for( auto component : components )
            {
                if( component )
                {
                    component->handleEvent( IEvent::Type::Actor, IComponent::sceneWasLoaded,
                                            Array<Parameter>(), this, component, nullptr );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->levelWasLoaded( scene );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Actor::hierarchyChanged()
    {
        try
        {
            auto components = getComponents();
            for( auto component : components )
            {
                if( component )
                {
                    component->handleEvent( IEvent::Type::Actor, IComponent::hierarchyChanged,
                                            Array<Parameter>(), this, component, nullptr );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->hierarchyChanged();
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Actor::childAdded( SmartPtr<IActor> child )
    {
        try
        {
            auto components = getComponents();
            for( auto component : components )
            {
                if( component )
                {
                    component->handleEvent( IEvent::Type::Actor, IComponent::childAdded,
                                            Array<Parameter>(), this, child, nullptr );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->childAdded( child );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Actor::childRemoved( SmartPtr<IActor> child )
    {
        try
        {
            auto components = getComponents();
            for( auto component : components )
            {
                if( component )
                {
                    component->handleEvent( IEvent::Type::Actor, IComponent::childRemoved,
                                            Array<Parameter>(), this, child, nullptr );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->childRemoved( child );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Actor::preUpdate()
    {
        auto components = getComponents();
        for( auto &c : components )
        {
            if( c )
            {
                c->preUpdate();
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto children = *p;
            for( auto child : children )
            {
                if( child )
                {
                    child->preUpdate();
                }
            }
        }
    }

    auto Actor::getComponentsPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IComponent>>>
    {
        return m_components;
    }

    void Actor::updateDirty( u32 flags, u32 oldFlags )
    {
        auto components = getComponents();
        for( auto &component : components )
        {
            if( component )
            {
                component->updateFlags( flags, oldFlags );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                if( child )
                {
                    child->updateDirty( flags, oldFlags );
                }
            }
        }
    }

    void Actor::update()
    {
        auto components = getComponents();
        for( auto &c : components )
        {
            if( c )
            {
                c->update();
            }
        }
    }

    void Actor::postUpdate()
    {
        auto components = getComponents();
        for( auto &c : components )
        {
            if( c )
            {
                c->postUpdate();
            }
        }
    }

    void Actor::addComponentInstance( SmartPtr<IComponent> component )
    {
        try
        {
            FB_ASSERT( component );

            component->setActor( this );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            sceneManager->addComponent( component );

            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                components.push_back( component );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Actor::removeComponentInstance( SmartPtr<IComponent> component )
    {
        FB_ASSERT( component );

        if( auto p = getComponentsPtr() )
        {
            auto components = Array<SmartPtr<IComponent>>( p->begin(), p->end() );
            auto it = std::find( components.begin(), components.end(), component );
            if( it != components.end() )
            {
                components.erase( it );
            }

            auto newComponents = fb::make_shared<ConcurrentArray<SmartPtr<IComponent>>>(
                components.begin(), components.end() );
            m_components = newComponents;
        }

        component->unload( nullptr );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        sceneManager->removeComponent( component );
    }

    auto Actor::hasComponent( int handle ) -> bool
    {
        auto components = getComponents();
        for( auto component : components )
        {
            auto pHandle = component->getHandle();
            auto componentId = pHandle->getId();

            if( componentId == handle )
            {
                return true;
            }
        }

        return false;
    }

    auto Actor::getComponent( int handle ) const -> SmartPtr<IComponent>
    {
        auto components = getComponents();
        for( auto component : components )
        {
            auto pHandle = component->getHandle();
            auto componentId = pHandle->getId();

            if( componentId == handle )
            {
                return component;
            }
        }

        return nullptr;
    }

    auto Actor::isMine() const -> bool
    {
        auto flags = getFlags();
        return BitUtil::getFlagValue( flags, ActorFlagMine );
    }

    void Actor::setMine( bool mine )
    {
        auto flags = getFlags();
        auto newFlags = BitUtil::setFlagValue( flags, ActorFlagMine, mine );
        setFlags( newFlags );
    }

    auto Actor::isStatic() const -> bool
    {
        auto flags = getFlags();
        return BitUtil::getFlagValue( flags, ActorFlagStatic );
    }

    void Actor::setStatic( bool isstatic )
    {
        if( isStatic() != isstatic )
        {
            auto flags = getFlags();
            auto newFlags = BitUtil::setFlagValue( flags, ActorFlagStatic, isstatic );
            setFlags( newFlags );

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->setStatic( isstatic );
                }
            }
        }
    }

    auto Actor::isEnabledInScene() const -> bool
    {
        auto enabledInScene = isEnabled();
        if( enabledInScene )
        {
            auto count = 0;
            auto parent = getParent();
            while( parent && count++ < 1000 )
            {
                if( !parent->isEnabled() )
                {
                    return false;
                }

                parent = parent->getParent();
            }
        }

        return enabledInScene;
    }

    auto Actor::isEnabled() const -> bool
    {
        auto flags = getFlags();
        return BitUtil::getFlagValue( flags, ActorFlagEnabled );
    }

    void Actor::setEnabled( bool enabled )
    {
        if( isEnabled() != enabled )
        {
            auto flags = getFlags();
            auto newFlags = BitUtil::setFlagValue( flags, ActorFlagEnabled, enabled );
            setFlags( newFlags );

            updateVisibility();
        }
    }

    auto Actor::isVisible() const -> bool
    {
        auto flags = getFlags();
        return BitUtil::getFlagValue( flags, ActorFlagVisible );
    }

    void Actor::setVisible( bool visible, bool cacade )
    {
        auto flags = getFlags();
        auto newFlags = BitUtil::setFlagValue( flags, ActorFlagVisible, visible );
        setFlags( newFlags );

        if( cacade )
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->setVisible( visible, cacade );
                }
            }
        }
    }

    auto Actor::isDirty() const -> bool
    {
        auto flags = getFlags();
        return BitUtil::getFlagValue( flags, ActorFlagDirty );
    }

    void Actor::setDirty( bool dirty )
    {
        if( isDirty() != dirty )
        {
            auto flags = getFlags();
            auto newFlags = BitUtil::setFlagValue( flags, ActorFlagDirty, dirty );
            setFlags( newFlags );
        }
    }

    auto Actor::isValid() const -> bool
    {
        return true;
    }

    void Actor::updateTransform()
    {
        if( auto transform = getTransform() )
        {
            transform->updateTransform();
        }

        auto components = getComponents();
        for( auto component : components )
        {
            component->updateTransform();
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto child : children )
            {
                child->updateTransform();
            }
        }
    }

    auto Actor::getComponents() const -> Array<SmartPtr<IComponent>>
    {
        if( auto p = getComponentsPtr() )
        {
            auto components = *p;
            return Array<SmartPtr<IComponent>>( components.begin(), components.end() );
        }

        return {};
    }

    auto Actor::handleActorEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        switch( eventType )
        {
        case IFSM::Event::Change:
            break;
        case IFSM::Event::Enter:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Create:
            case State::Destroyed:
            case State::Edit:
            {
                bool cascade = getParent() == nullptr;
                auto transform = getTransform();
                if( transform )
                {
                    transform->setDirty( true, cascade );
                }

                auto components = getComponents();
                for( auto component : components )
                {
                    component->setState( IComponent::State::Edit );
                }
            }
            break;
            case State::Play:
            {
                bool cascade = getParent() == nullptr;
                auto transform = getTransform();
                if( transform )
                {
                    transform->setDirty( true, cascade );
                }

                auto components = getComponents();
                for( auto component : components )
                {
                    component->setState( IComponent::State::Play );
                }
            }
            break;
            default:
            {
            }
            break;
            }
        }
        break;
        case IFSM::Event::Leave:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Create:
            case State::Destroyed:
            case State::Edit:
            case State::Play:
            {
            }
            break;
            default:
            {
            }
            break;
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
        case IFSM::Event::Count:
            break;
        default:
            break;
        }

        return IFSM::ReturnType::Ok;
    }

    auto Actor::handleActorGameEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        switch( eventType )
        {
        case IFSM::Event::Change:
            break;
        case IFSM::Event::Enter:
            break;
        case IFSM::Event::Leave:
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
            break;
        }

        return IFSM::ReturnType::Ok;
    }

    auto Actor::compareTag( const String &tag ) const -> bool
    {
        auto it = std::find( m_tags.begin(), m_tags.end(), tag );
        if( it != m_tags.end() )
        {
            return true;
        }

        return false;
    }

    auto Actor::getSceneRoot() const -> SmartPtr<IActor>
    {
        auto parent = getParent();
        while( parent )
        {
            auto root = parent->getSceneRoot();
            if( root == nullptr )
            {
                return parent;
            }

            parent = root;
        }

        return nullptr;
    }

    auto Actor::getSceneLevel() const -> u32
    {
        auto sceneLevel = 0;
        auto parent = getParent();
        while( parent )
        {
            auto root = parent->getSceneRoot();
            if( root == nullptr )
            {
                return sceneLevel;
            }

            ++sceneLevel;
            parent = root;
        }

        return sceneLevel;
    }

    auto Actor::getTransform() -> SmartPtr<ITransform> &
    {
        return m_transform;
    }

    auto Actor::getTransform() const -> const SmartPtr<ITransform> &
    {
        return m_transform;
    }

    void Actor::setTransform( SmartPtr<ITransform> transform )
    {
        m_transform = transform;
    }

    void Actor::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            FB_ASSERT( pSceneManager );

            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            m_flags = sceneManager->getFlagsPtr( id );

            FB_ASSERT( sceneManager->isLoaded() );

            auto transform = sceneManager->createTransformComponent();
            setTransform( transform );

            if( transform )
            {
                transform->setActor( this );
            }

            setEnabled( true );
            setVisible( true );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Actor::reload( SmartPtr<ISharedObject> data )
    {
        unload( data );
        load( data );
    }

    void Actor::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto isDummy = getFlag( ActorFlagDummy );
                if( !isDummy )
                {
                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto pSceneManager = applicationManager->getSceneManager();
                    auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

                    if( auto transform = getTransform() )
                    {
                        transform->unload( nullptr );
                    }

                    if( auto p = getChildrenPtr() )
                    {
                        auto &children = *p;
                        for( auto child : children )
                        {
                            if( child )
                            {
                                child->unload( data );
                            }
                        }
                    }

                    auto components = getComponents();
                    for( auto component : components )
                    {
                        try
                        {
                            if( component )
                            {
                                component->unload( nullptr );
                            }
                        }
                        catch( std::exception &e )
                        {
                            FB_LOG_EXCEPTION( e );
                        }
                    }

                    m_scene = nullptr;
                    m_parent = nullptr;
                    m_transform = nullptr;

                    if( auto p = getChildrenPtr() )
                    {
                        auto &children = *p;
                        children.clear();
                    }

                    if( auto p = getComponentsPtr() )
                    {
                        auto &components = *p;
                        components.clear();
                    }

                    // for (u32 x = 0; x < int(Thread::UpdateState::Count); ++x)
                    //{
                    //	for (u32 y = 0; y < int(Thread::Task::Count); ++y)
                    //	{
                    //		m_updateComponents[x][y] = nullptr;
                    //	}
                    // }

                    if( auto scene = getScene() )
                    {
                        auto pThis = getSharedFromThis<Actor>();
                        scene->unregisterAll( pThis );
                    }

                    setScene( nullptr );

                    if( sceneManager )
                    {
                        auto transform = getTransform();
                        sceneManager->destroyTransformComponent( transform );
                        setTransform( nullptr );
                    }

                    // m_componentCache = nullptr;
                    // m_parent = nullptr;

                    for( auto component : components )
                    {
                        if( component )
                        {
                            component->handleEvent( IEvent::Type::Actor, IComponent::actorUnload,
                                                    Array<Parameter>(), this, component, nullptr );
                        }
                    }
                }

                setStateContext( nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Actor::getPerpetual() const -> bool
    {
        return m_perpetual;
    }

    void Actor::setPerpetual( bool perpetual )
    {
        m_perpetual = perpetual;
    }

    auto Actor::getAutoUpdateComponents() const -> bool
    {
        return m_autoUpdateComponents;
    }

    void Actor::setAutoUpdateComponents( bool autoUpdateComponents )
    {
        m_autoUpdateComponents = autoUpdateComponents;
    }

    auto Actor::getScene() const -> SmartPtr<IScene>
    {
        auto p = m_scene.load();
        return p.lock();
    }

    void Actor::setScene( SmartPtr<IScene> scene )
    {
        m_scene = scene;
    }

    void Actor::triggerEnter( SmartPtr<IComponent> collision )
    {
        auto components = getComponents();
        for( auto component : components )
        {
            component->handleEvent( IEvent::Type::Actor, IComponent::triggerCollisionEnter,
                                    Array<Parameter>(), this, collision, nullptr );
        }
    }

    void Actor::triggerLeave( SmartPtr<IComponent> collision )
    {
        auto components = getComponents();
        for( auto component : components )
        {
            component->handleEvent( IEvent::Type::Actor, IComponent::triggerCollisionLeave,
                                    Array<Parameter>(), this, collision, nullptr );
        }
    }

    void Actor::componentLoaded( SmartPtr<IComponent> loadedComponent )
    {
        auto components = getComponents();
        for( auto component : components )
        {
            component->handleEvent( IEvent::Type::Actor, IComponent::componentLoaded, Array<Parameter>(),
                                    this, loadedComponent, nullptr );
        }
    }

    auto Actor::getChildByIndex( u32 index ) const -> SmartPtr<IActor>
    {
        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            return children[index];
        }

        return nullptr;
    }

    auto Actor::getNumChildren() const -> u32
    {
        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            return static_cast<u32>( children.size() );
        }

        return 0;
    }

    auto Actor::getSiblingIndex() const -> s32
    {
        auto parentTransform = getParent();
        if( parentTransform == nullptr )
        {
            return 0;
        }

        for( u32 i = 0; i < parentTransform->getNumChildren(); i++ )
        {
            const auto child = parentTransform->getChildByIndex( i );
            if( child == getSharedFromThis<IActor>() )
            {
                return i;
            }
        }

        return -1;
    }

    void Actor::addChild( SmartPtr<IActor> child )
    {
        FB_ASSERT( child );

        if( auto parent = child->getParent() )
        {
            parent->removeChild( child );
        }

        auto handle = child->getHandle();
        auto id = handle->getInstanceId();

        FB_ASSERT( id != std::numeric_limits<u32>::max() );
        FB_ASSERT( id != getHandle()->getInstanceId() );  // child id matches parent

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            children.push_back( child );

            //FB_ASSERT( CoreUtil::hasDuplicates( children ) == false );
        }

        auto pThis = getSharedFromThis<IActor>();
        child->setParent( pThis );

        if( auto p = getComponentsPtr() )
        {
            auto components = *p;
            for( auto &component : components )
            {
                component->handleEvent( IEvent::Type::Actor, IComponent::childAdded, Array<Parameter>(),
                                        this, child, nullptr );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto children = *p;
            for( auto &c : children )
            {
                c->childAddedInHierarchy( child );
            }
        }
    }

    void Actor::removeChild( SmartPtr<IActor> child )
    {
        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;

            auto childrenArray = Array<SmartPtr<IActor>>( children.begin(), children.end() );
            const auto it = std::find( childrenArray.begin(), childrenArray.end(), child );
            if( it != childrenArray.end() )
            {
                childrenArray.erase( it );
            }

            m_children = fb::make_shared<ConcurrentArray<SmartPtr<IActor>>>( childrenArray.begin(),
                                                                             childrenArray.end() );
            child->setParent( nullptr );
        }

        if( auto p = getComponentsPtr() )
        {
            auto &components = *p;
            for( auto &component : components )
            {
                component->handleEvent( IEvent::Type::Actor, IComponent::childRemoved,
                                        Array<Parameter>(), this, child, nullptr );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &c : children )
            {
                c->childRemovedInHierarchy( child );
            }
        }
    }

    void Actor::childAddedInHierarchy( SmartPtr<IActor> child )
    {
        if( auto p = getComponentsPtr() )
        {
            auto components = *p;
            for( auto &component : components )
            {
                component->handleEvent( IEvent::Type::Actor, IComponent::childAddedInHierarchy,
                                        Array<Parameter>(), this, child, nullptr );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto children = *p;
            for( auto &c : children )
            {
                c->childAddedInHierarchy( child );
            }
        }
    }

    void Actor::childRemovedInHierarchy( SmartPtr<IActor> child )
    {
        if( auto p = getComponentsPtr() )
        {
            auto &components = *p;
            for( auto &component : components )
            {
                component->handleEvent( IEvent::Type::Actor, IComponent::childRemovedInHierarchy,
                                        Array<Parameter>(), this, child, nullptr );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &c : children )
            {
                c->childRemovedInHierarchy( child );
            }
        }
    }

    void Actor::removeChildren()
    {
        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                auto pChild = fb::static_pointer_cast<Actor>( child );
                pChild->setParent( nullptr );
            }
        }
    }

    auto Actor::findChild( const String &name ) -> SmartPtr<IActor>
    {
        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                if( child->getName() == name )
                {
                    return child;
                }
            }
        }

        return nullptr;
    }

    auto Actor::getChildren() const -> Array<SmartPtr<IActor>>
    {
        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            return Array<SmartPtr<IActor>>( children.begin(), children.end() );
        }

        return {};
    }

    auto Actor::getChildrenPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IActor>>>
    {
        return m_children;
    }

    auto Actor::getAllChildren( SmartPtr<IActor> parent ) const -> Array<SmartPtr<IActor>>
    {
        Array<SmartPtr<IActor>> children;
        children.reserve( 128 );

        auto childCount = parent->getNumChildren();

        for( u32 i = 0; i < childCount; i++ )
        {
            auto child = parent->getChildByIndex( i );
            children.push_back( child );

            auto childChildren = getAllChildren( child );

            children.insert( children.end(), childChildren.begin(), childChildren.end() );
        }

        return children;
    }

    auto Actor::getAllChildren() const -> Array<SmartPtr<IActor>>
    {
        auto thisActor = getSharedFromThis<IActor>();
        return getAllChildren( thisActor );
    }

    void Actor::setSiblingIndex( s32 index )
    {
        auto parent = getParent();
        if( parent != nullptr )
        {
            parent->setChildSiblingIndex( this, index );
        }
    }

    void Actor::setChildSiblingIndex( SmartPtr<IActor> child, s32 index )
    {
        auto p = getChildrenPtr();
        if( p != nullptr )
        {
            auto &children = *p;
            if( index < children.size() )
            {
                auto array = Array<SmartPtr<IActor>>( children.begin(), children.end() );
                array.erase( std::remove( array.begin(), array.end(), child ), array.end() );
                array.insert( array.begin() + index, child );
                m_children =
                    fb::make_shared<ConcurrentArray<SmartPtr<IActor>>>( array.begin(), array.end() );
            }
        }
    }

    auto Actor::getParent() const -> SmartPtr<IActor>
    {
        auto p = m_parent.load();
        return p.lock();
    }

    void Actor::setParent( SmartPtr<IActor> parent )
    {
        auto p = getParent();
        if( p != parent )
        {
            m_parent = parent;

            if( auto transform = getTransform() )
            {
                transform->parentChanged( parent, p );
            }

            auto components = getComponents();
            for( auto component : components )
            {
                Array<Parameter> arguments;
                arguments.resize( 2 );

                arguments[0].object = parent;
                arguments[1].object = p;

                component->handleEvent( IEvent::Type::Actor, IComponent::parentChanged, arguments, this,
                                        component, nullptr );
            }
        }
    }

    auto Actor::toData() const -> SmartPtr<ISharedObject>
    {
        auto typeManager = TypeManager::instance();

        //FB_ASSERT( getLoadingState() == LoadingState::Unloaded );
        //FB_ASSERT( getHandle()->getInstanceId() != std::numeric_limits<u32>::max() );

        auto actorData = fb::make_ptr<Properties>();

        auto label = getName();
        auto bStatic = isStatic();
        auto visible = isVisible();
        auto enabled = isEnabled();

        actorData->setProperty( "label", label );
        actorData->setProperty( "static", bStatic );
        actorData->setProperty( "visible", visible );
        actorData->setProperty( "enabled", enabled );

        if( auto handle = getHandle() )
        {
            auto uuid = handle->getUUID();
            if( StringUtil::isNullOrEmpty( uuid ) )
            {
                uuid = StringUtil::getUUID();
            }

            actorData->setProperty( "uuid", uuid );
        }

        if( auto transform = getTransform() )
        {
            auto localTransform = transform->getLocalTransform();
            auto worldTransform = transform->getWorldTransform();

            auto localTransformProperties = localTransform.getProperties();
            localTransformProperties->setName( "localTransform" );
            actorData->addChild( localTransformProperties );

            auto worldTransformProperties = worldTransform.getProperties();
            worldTransformProperties->setName( "worldTransform" );
            actorData->addChild( worldTransformProperties );
        }

        if( auto p = this->getChildrenPtr() )
        {
            auto &children = *p;
            for( auto child : children )
            {
                if( child )
                {
                    FB_ASSERT( child->getHandle()->getInstanceId() != std::numeric_limits<u32>::max() );

                    auto pChildActorData = fb::static_pointer_cast<Properties>( child->toData() );
                    pChildActorData->setName( "child" );
                    actorData->addChild( pChildActorData );
                }
            }
        }

        auto components = getComponents();
        for( auto component : components )
        {
            auto componentData = fb::static_pointer_cast<Properties>( component->toData() );
            if( componentData )
            {
                componentData->setName( "component" );

                auto typeinfo = component->getTypeInfo();
                auto className = typeManager->getName( typeinfo );

                auto componentType = StringUtil::replaceAll( className, "class ", "" );
                componentData->setProperty( "componentType", componentType );

                actorData->addChild( componentData );
            }
        }

        return actorData;
    }

    void Actor::fromData( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( data );

            if( !data )
            {
                FB_LOG_ERROR( "CActor::fromData data null." );
                return;
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto actorData = fb::static_pointer_cast<Properties>( data );

            auto label = String();
            auto enabled = true;
            auto visible = true;
            auto bStatic = false;

            actorData->getPropertyValue( "label", label );
            actorData->getPropertyValue( "enabled", enabled );
            actorData->getPropertyValue( "visible", visible );
            actorData->getPropertyValue( "static", bStatic );

            setName( label );
            setEnabled( enabled );
            setVisible( visible );
            setStatic( bStatic );

            if( auto handle = getHandle() )
            {
                auto uuid = String();
                actorData->getPropertyValue( "uuid", uuid );

                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                handle->setUUID( uuid );
            }

            if( auto transform = getTransform() )
            {
                if( auto localTransformChild = actorData->getChild( "localTransform" ) )
                {
                    auto localTransform = transform->getLocalTransform();
                    localTransform.setProperties( localTransformChild );

                    transform->setLocalTransform( localTransform );
                }

                if( auto worldTransformChild = actorData->getChild( "worldTransform" ) )
                {
                    auto worldTransform = transform->getWorldTransform();
                    worldTransform.setProperties( worldTransformChild );

                    //transform->setWorldTransform( worldTransform );
                }
            }

            auto componentsData = actorData->getChildrenByName( "component" );

            auto components = Array<SmartPtr<IComponent>>();
            components.reserve( componentsData.size() );

            for( auto &component : componentsData )
            {
                auto componentType = String();
                component->getPropertyValue( "componentType", componentType );

                auto pComponent = factoryManager->createObjectFromType<IComponent>( componentType );
                if( !pComponent )
                {
                    auto componentTypeClean = StringUtil::replaceAll( componentType, "fb::", "" );
                    pComponent = factoryManager->createObjectFromType<IComponent>( componentTypeClean );
                }

                if( !pComponent )
                {
                    auto componentTypeClean = sceneManager->getComponentFactoryType( componentType );
                    pComponent = factoryManager->createObjectFromType<IComponent>( componentTypeClean );
                }

                if( pComponent )
                {
                    components.push_back( pComponent );
                }
            }

            for( auto &pComponent : components )
            {
                try
                {
                    addComponentInstance( pComponent );
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            for( size_t i = 0; i < components.size(); ++i )
            {
                try
                {
                    auto &pComponent = components[i];
                    auto &pComponentData = componentsData[i];
                    // FB_ASSERT(pComponent);

                    if( pComponent )
                    {
                        pComponent->setActor( this );

                        pComponent->fromData( pComponentData );

                        pComponent->load( nullptr );
                        componentLoaded( pComponent );
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            auto childrenData = actorData->getChildrenByName( "child" );
            for( auto &child : childrenData )
            {
                auto childActor = sceneManager->createActor();
                FB_ASSERT( childActor );

                childActor->fromData( child );
                addChild( childActor );

                //FB_ASSERT( getChildrenPtr()->size() == childrenData.size() );
                // FB_ASSERT(childActor->getComponents<IComponent>().size() ==
                // child.components.size());
            }

            updateTransform();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Actor::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Resource<IActor>::getProperties();

        properties->setProperty( "name", getName() );

        bool bIsStatic = isStatic();
        properties->setProperty( "static", bIsStatic );

        auto enabled = isEnabled();
        properties->setProperty( "enabled", enabled );

        return properties;
    }

    void Actor::setProperties( SmartPtr<Properties> properties )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        auto sceneManager = applicationManager->getSceneManager();

        auto name = String();
        properties->getPropertyValue( "label", name );
        setName( name );

        bool bIsStatic = false;
        properties->getPropertyValue( "static", bIsStatic );
        setStatic( bIsStatic );

        auto enabled = isEnabled();
        properties->getPropertyValue( "enabled", enabled );
        setEnabled( enabled );

        if( auto handle = getHandle() )
        {
            auto uuid = String();
            if( properties->getPropertyValue( "uuid", uuid ) )
            {
                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                handle->setUUID( uuid );
            }
        }

        if( auto transform = getTransform() )
        {
            if( auto localTransformChild = properties->getChild( "localTransform" ) )
            {
                auto localTransform = transform->getLocalTransform();
                localTransform.setProperties( localTransformChild );

                transform->setLocalTransform( localTransform );
                transform->setDirty( true );
            }

            if( auto worldTransformChild = properties->getChild( "worldTransform" ) )
            {
                auto worldTransform = transform->getWorldTransform();
                worldTransform.setProperties( worldTransformChild );

                //transform->setWorldTransform( worldTransform );
            }
        }

        auto componentsData = properties->getChildrenByName( "components" );
        auto componentsDataAlt = properties->getChildrenByName( "component" );
        componentsData.insert( componentsData.end(), componentsDataAlt.begin(),
                               componentsDataAlt.end() );

        auto components = Array<SmartPtr<IComponent>>();
        components.reserve( componentsData.size() );

        for( auto componentData : componentsData )
        {
            auto componentType = String();
            componentData->getPropertyValue( "componentType", componentType );

            auto pComponent = factoryManager->createObjectFromType<Component>( componentType );
            if( !pComponent )
            {
                auto nameSplit = StringUtil::split( componentType, "::" );
                std::reverse( nameSplit.begin(), nameSplit.end() );

                for( auto componentTypeName : nameSplit )
                {
                    pComponent = factoryManager->createObjectFromType<Component>( componentTypeName );
                    if( pComponent )
                    {
                        break;
                    }
                }
            }

            if( !pComponent )
            {
                componentType = sceneManager->getComponentFactoryType( componentType );
                pComponent = factoryManager->createObjectFromType<IComponent>( componentType );
            }

            if( pComponent )
            {
                components.emplace_back( pComponent );
            }
        }

        for( auto &pComponent : components )
        {
            try
            {
                addComponentInstance( pComponent );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        //FB_ASSERT( components.size() == componentsData.size() );

        for( size_t i = 0; i < components.size(); ++i )
        {
            try
            {
                auto &pComponent = components[i];
                auto &componentData = componentsData[i];

                if( pComponent )
                {
                    pComponent->setActor( this );
                    pComponent->fromData( componentData );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        for( size_t i = 0; i < components.size(); ++i )
        {
            try
            {
                auto &pComponent = components[i];
                auto &componentData = componentsData[i];

                if( pComponent )
                {
                    pComponent->load( nullptr );
                    componentLoaded( pComponent );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        auto childrenData = properties->getChildrenByName( "children" );
        auto childrenDataAlt = properties->getChildrenByName( "child" );
        childrenData.insert( childrenData.end(), childrenDataAlt.begin(), childrenDataAlt.end() );

        for( auto &childData : childrenData )
        {
            auto childActor = sceneManager->createActor();
            FB_ASSERT( childActor );

            childActor->setProperties( childData );
            addChild( childActor );

            //FB_ASSERT( childActor->getChildren()->size() == childrenData.size() );
            // FB_ASSERT(childActor->getComponents<IComponent>().size() ==
            // child.components.size());
        }

        updateTransform();
    }

    void Actor::setState( State state )
    {
        if( getState() != state )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            auto fsm = sceneManager->getFSM( id );
            if( fsm )
            {
                fsm->setState<State>( state );
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->setState( state );
                }
            }
        }
    }

    auto Actor::getState() const -> IActor::State
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pSceneManager = applicationManager->getSceneManager();
        auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
        FB_ASSERT( sceneManager );

        auto handle = getHandle();
        auto id = handle->getInstanceId();
        auto fsm = sceneManager->getFSM( id );
        if( fsm )
        {
            return fsm->getState<State>();
        }

        return static_cast<State>( 0 );
    }

    void Actor::setGameState( GameState state )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pSceneManager = applicationManager->getSceneManager();
        auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
        FB_ASSERT( sceneManager );

        auto handle = getHandle();
        auto id = handle->getInstanceId();
        auto fsm = sceneManager->getFSM( id );
        if( fsm )
        {
            fsm->setState<GameState>( state );
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                child->setGameState( state );
            }
        }
    }

    auto Actor::getGameState() const -> IActor::GameState
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pSceneManager = applicationManager->getSceneManager();
        auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
        FB_ASSERT( sceneManager );

        auto handle = getHandle();
        auto id = handle->getInstanceId();
        auto fsm = sceneManager->getFSM( id );
        if( fsm )
        {
            return fsm->getState<GameState>();
        }

        return static_cast<GameState>( 0 );
    }

    auto Actor::getFlags() const -> u32
    {
        return *m_flags;
    }

    void Actor::setFlags( u32 flags )
    {
        const auto currentFlags = getFlags();
        if( currentFlags != flags )
        {
            Array<Parameter> arguments;
            arguments.resize( 2 );

            arguments[0].setU32( currentFlags );
            arguments[1].setU32( flags );

            *m_flags = flags;

            sendEvent( IEvent::Type::Actor, IComponent::actorFlagsChanged, arguments, this, nullptr,
                       nullptr );
        }
    }

    auto Actor::getFlag( u32 flag ) const -> bool
    {
        auto flags = getFlags();
        return ( flags & flag ) != 0;
    }

    void Actor::setFlag( u32 flag, bool value )
    {
        auto flags = getFlags();
        if( value )
        {
            flags |= flag;
        }
        else
        {
            flags &= ~flag;
        }

        setFlags( flags );
    }

    void Actor::updateVisibility()
    {
        auto enabledInScene = isEnabledInScene();
        setFlag( ActorFlagEnabledInScene, enabledInScene );

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                child->updateVisibility();
            }
        }
    }

    auto Actor::handleEvent( IEvent::Type eventType, hash_type eventValue,
                             const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                             SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( auto p = getComponentsPtr() )
        {
            auto &components = *p;
            for( auto component : components )
            {
                component->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                child->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }
        }

        return {};
    }

    auto Actor::sendEvent( IEvent::Type eventType, hash_type eventValue,
                           const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                           SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( auto p = getComponentsPtr() )
        {
            auto &components = *p;
            for( auto component : components )
            {
                component->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }
        }

        if( auto p = getChildrenPtr() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                child->sendEvent( eventType, eventValue, arguments, sender, object, event );
            }
        }

        return {};
    }

    void Actor::ActorFsmListener::unload( SmartPtr<ISharedObject> data )
    {
        setOwner( nullptr );
    }

    auto Actor::ActorFsmListener::handleEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        if( auto owner = getOwner() )
        {
            return owner->handleActorEvent( state, eventType );
        }

        return IFSM::ReturnType::NotHandled;
    }

    auto Actor::ActorFsmListener::getOwner() const -> SmartPtr<Actor>
    {
        return m_owner;
    }

    void Actor::ActorFsmListener::setOwner( SmartPtr<Actor> owner )
    {
        m_owner = owner;
    }

    void Actor::ActorGameFsmListener::unload( SmartPtr<ISharedObject> data )
    {
        setOwner( nullptr );
    }

    auto Actor::ActorGameFsmListener::handleEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        if( auto owner = getOwner() )
        {
            return owner->handleActorGameEvent( state, eventType );
        }

        return IFSM::ReturnType::NotHandled;
    }

    auto Actor::ActorGameFsmListener::getOwner() const -> SmartPtr<Actor>
    {
        return m_owner;
    }

    void Actor::ActorGameFsmListener::setOwner( SmartPtr<Actor> owner )
    {
        m_owner = owner;
    }

    void Actor::registerClass()
    {
        using namespace fb;
        using namespace rttr;

        //registration::class_<Actor>( "Actor" ).property( "transform", &Actor::m_transform );
    }

}  // namespace fb::scene
