#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Systems/ComponentSystem.h>
#include <FBCore/Interface/Scene/IComponent.h>

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

                        m_lastFreeSlot = i;
                        return i;
                    }
                }

                FB_ASSERT( isValid() );

                // grow the arrays
                const auto currentSize = getSize();
                reserve( currentSize + currentSize );

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
        }

        void ComponentSystem::reserve( size_t size )
        {
            FB_ASSERT( isValid() );

            RecursiveMutex::ScopedLock lock( m_mutex );

            auto currentSize = getSize();
            if( currentSize != size )
            {
                m_loadingStates.resize( size );
            }
        }

        size_t ComponentSystem::getSize() const
        {
            return m_size;
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

        const fb::Atomic<fb::LoadingState> &ComponentSystem::getLoadingState( u32 id ) const
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

        fb::SmartPtr<fb::scene::IComponent> ComponentSystem::getObject( u32 index ) const
        {
            FB_ASSERT( index < getSize() );
            return m_components[index];
        }

    }  // namespace scene
}  // namespace fb
