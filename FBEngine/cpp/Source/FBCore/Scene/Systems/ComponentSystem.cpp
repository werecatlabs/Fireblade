#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Systems/ComponentSystem.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/System/IState.h>

namespace fb
{
    namespace scene
    {
        ComponentSystem::ComponentSystem()
        {
        }

        ComponentSystem::~ComponentSystem()
        {
        }

        void ComponentSystem::load( SmartPtr<ISharedObject> data )
        {
            reserve( 12 );
            SharedObject<ISystem>::setLoadingState( LoadingState::Loaded );
        }

        void ComponentSystem::unload( SmartPtr<ISharedObject> data )
        {
            SharedObject<ISystem>::setLoadingState( LoadingState::Loaded );
        }

        u32 ComponentSystem::addComponent( SmartPtr<IComponent> component )
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
                const auto currentSize = getSize();
                reserve( currentSize + 8 );

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

        size_t ComponentSystem::getSize() const
        {
            return m_size;
        }

        void ComponentSystem::setSize( size_t size )
        {
            m_size = size;
        }

        hash_type ComponentSystem::getStateType() const
        {
            return m_stateType;
        }

        void ComponentSystem::setStateType( hash_type type )
        {
            m_stateType = type;
        }

        SmartPtr<IState> ComponentSystem::getState( u32 id ) const
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

        void ComponentSystem::reserveData( size_t size )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            factoryManager->setPoolSize( m_stateType, size );
            m_data.resize( size );

            for( size_t i = m_size; i < size; ++i )
            {
                m_data[i] = factoryManager->createById( m_stateType );
            }
        }

        bool ComponentSystem::isFreeSlot( u32 slot )
        {
            const Atomic<LoadingState> unallocatedState = LoadingState::Unallocated;
            const auto &loadingState = getLoadingState( slot );

            if( loadingState == unallocatedState )
            {
                return true;
            }

            return false;
        }

        const Atomic<LoadingState> &ComponentSystem::getLoadingState( u32 id ) const
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

        SmartPtr<IComponent> ComponentSystem::getObject( u32 index ) const
        {
            FB_ASSERT( index < getSize() );
            return m_components[index];
        }
    }  // namespace scene
}  // namespace fb
