#ifndef ComponentSystem_h__
#define ComponentSystem_h__

#include <FBCore/Interface/Scene/IComponentSystem.h>

namespace fb
{
    namespace scene
    {

        class ComponentSystem : public IComponentSystem
        {
        public:
            ComponentSystem();
            virtual ~ComponentSystem();

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            u32 addComponent( SmartPtr<IComponent> component );

            void removeComponent( SmartPtr<IComponent> component );

            void removeComponent( u32 id );

            void reserve( size_t size );

            size_t getSize() const;
            void setSize( size_t size );

            hash_type getStateType() const;

            void setStateType( hash_type type );

            SmartPtr<IState> getState( u32 id ) const;
            void setState( u32 id, SmartPtr<IState> state );

            u32 getGrowSize() const;

            void setGrowSize( u32 growSize );

            FB_CLASS_REGISTER_DECL;

        protected:
            virtual void reserveData( size_t size );

            bool isFreeSlot( u32 slot );

            const Atomic<LoadingState> &getLoadingState( u32 id ) const;

            void setLoadingState( u32 id, LoadingState state );

            void setObject( u32 index, SmartPtr<IComponent> component );

            SmartPtr<IComponent> getObject( u32 index ) const;

            size_t m_size = 0;

            // The index of the last freed object slot.
            atomic_u32 m_lastFreeSlot = 0;
            u32 m_growSize = 128;

            hash_type m_stateType = 0;
            Array<SmartPtr<IState>> m_data;
            Array<SmartPtr<IComponent>> m_components;
            Array<Atomic<LoadingState>> m_loadingStates;
            mutable RecursiveMutex m_mutex;
        };

    }  // namespace scene
}  // namespace fb

#endif  // ComponentSystem_h__
