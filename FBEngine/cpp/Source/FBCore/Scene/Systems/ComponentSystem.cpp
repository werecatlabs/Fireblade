#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Systems/ComponentSystem.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, ComponentSystem, IComponentSystem );

    ComponentSystem::ComponentSystem() = default;

    ComponentSystem::~ComponentSystem() = default;

    void ComponentSystem::load( SmartPtr<ISharedObject> data )
    {
        reserve( 12 );
        IComponentSystem::setLoadingState( LoadingState::Loaded );
    }

    void ComponentSystem::unload( SmartPtr<ISharedObject> data )
    {
        IComponentSystem::setLoadingState( LoadingState::Loaded );
    }

    auto ComponentSystem::addComponent( SmartPtr<IComponent> component ) -> u32
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

        const auto retries = 10;
        for( size_t i = 0; i < retries; ++i )
        {
            for( u32 i = m_lastFreeSlot.load() + 1; i < getSize(); ++i )
            {
                if( isFreeSlot( i ) )
                {
                    setLoadingState( i, LoadingState::Allocated );

                    FB_ASSERT( getObject( i ) == nullptr );
                    setObject( i, component );

                    auto componentState = m_data[i];
                    component->setComponentState( componentState );

                    m_lastFreeSlot = i;
                    return i;
                }
            }

            for( u32 i = 0; i < m_lastFreeSlot; ++i )
            {
                if( isFreeSlot( i ) )
                {
                    setLoadingState( i, LoadingState::Allocated );

                    FB_ASSERT( getObject( i ) == nullptr );
                    setObject( i, component );

                    auto componentState = m_data[i];
                    component->setComponentState( componentState );

                    m_lastFreeSlot = i;
                    return i;
                }
            }

            FB_ASSERT( isValid() );

            // grow the arrays
            auto growSize = getGrowSize();
            const auto currentSize = getSize();
            reserve( ( currentSize + growSize ) * 2 );

            FB_ASSERT( isValid() );
        }

        FB_ASSERT( isValid() );
        return std::numeric_limits<u32>::max();
    }

    void ComponentSystem::removeComponent( u32 id )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

        setLoadingState( id, LoadingState::Unallocated );
        setObject( id, nullptr );
    }

    void ComponentSystem::removeComponent( SmartPtr<IComponent> component )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto handle = component->getHandle();
        auto id = handle->getInstanceId();

        FB_ASSERT( isValid() );

        setLoadingState( id, LoadingState::Unallocated );
        setObject( id, nullptr );
    }

    void ComponentSystem::reserve( size_t size )
    {
        FB_ASSERT( isValid() );

        RecursiveMutex::ScopedLock lock( m_mutex );

        auto currentSize = getSize();
        if( currentSize != size )
        {
            m_components.resize( size );
            m_loadingStates.resize( size );

            for( size_t i = currentSize; i < size; ++i )
            {
                m_loadingStates[i] = LoadingState::Unallocated;
            }

            reserveData( size );

            setSize( size );
        }
    }

    auto ComponentSystem::getSize() const -> size_t
    {
        return m_size;
    }

    void ComponentSystem::setSize( size_t size )
    {
        m_size = size;
    }

    auto ComponentSystem::getStateType() const -> hash_type
    {
        return m_stateType;
    }

    void ComponentSystem::setStateType( hash_type type )
    {
        m_stateType = type;
    }

    auto ComponentSystem::getState( u32 id ) const -> SmartPtr<IState>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );
        FB_ASSERT( id < getSize() );

        return m_data[id];
    }

    void ComponentSystem::setState( u32 id, SmartPtr<IState> state )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );
        FB_ASSERT( id < getSize() );

        m_data[id] = state;
    }

    auto ComponentSystem::getGrowSize() const -> u32
    {
        return m_growSize;
    }

    void ComponentSystem::setGrowSize( u32 growSize )
    {
        m_growSize = growSize;
    }

    void ComponentSystem::reserveData( size_t size )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        factoryManager->setPoolSize( m_stateType, size );
        m_data.resize( size );

        for( size_t i = m_size; i < size; ++i )
        {
            m_data[i] = factoryManager->createById( m_stateType );
        }
    }

    auto ComponentSystem::isFreeSlot( u32 slot ) -> bool
    {
        const Atomic<LoadingState> unallocatedState = LoadingState::Unallocated;
        const auto &loadingState = getLoadingState( slot );

        if( loadingState == unallocatedState )
        {
            return true;
        }

        return false;
    }

    auto ComponentSystem::getLoadingState( u32 id ) const -> const Atomic<LoadingState> &
    {
        FB_ASSERT( id < getSize() );
        return m_loadingStates[id];
    }

    void ComponentSystem::setLoadingState( u32 id, LoadingState state )
    {
        FB_ASSERT( id < getSize() );
        m_loadingStates[id] = state;
    }

    void ComponentSystem::setObject( u32 index, SmartPtr<IComponent> component )
    {
        FB_ASSERT( index < getSize() );
        m_components[index] = component;
    }

    auto ComponentSystem::getObject( u32 index ) const -> SmartPtr<IComponent>
    {
        FB_ASSERT( index < getSize() );
        return m_components[index];
    }
}  // namespace fb::scene
