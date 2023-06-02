#ifndef ComponentSystem_h__
#define ComponentSystem_h__

#include <FBCore/Interface/Scene/ISystem.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace scene
    {

        class ComponentSystem : public CSharedObject<ISystem>
        {
        public:
            ComponentSystem();
            virtual ~ComponentSystem();

            u32 addComponent( SmartPtr<IComponent> component );

            void removeComponent( SmartPtr<IComponent> component );

            void removeComponent( u32 id );

            void reserve( size_t size );

            size_t getSize() const;

        protected:
            bool isFreeSlot( u32 slot );

            const Atomic<LoadingState> &getLoadingState( u32 id ) const;

            void setLoadingState( u32 id, LoadingState state );

            void setObject( u32 index, SmartPtr<IComponent> component );

            SmartPtr<IComponent> getObject( u32 index ) const;

            size_t m_size = 0;

            // The index of the last freed object slot.
            atomic_u32 m_lastFreeSlot = 0;

            Array<SmartPtr<IComponent>> m_components;
            Array<Atomic<LoadingState>> m_loadingStates;
            mutable RecursiveMutex m_mutex;
        };

    }  // namespace scene
}  // namespace fb

#endif  // ComponentSystem_h__
