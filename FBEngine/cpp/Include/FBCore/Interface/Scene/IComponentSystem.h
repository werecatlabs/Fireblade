#ifndef ISystem_h__
#define ISystem_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace scene
    {
        /** System abstract interface for the entity component system. */
        class IComponentSystem : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IComponentSystem() override = default;

            void update() override = 0;

            virtual u32 addComponent( SmartPtr<IComponent> component ) = 0;

            virtual void removeComponent( SmartPtr<IComponent> component ) = 0;

            virtual void removeComponent( u32 id ) = 0;

            virtual void reserve( size_t size ) = 0;

            virtual size_t getSize() const = 0;
            virtual void setSize( size_t size ) = 0;

            virtual hash_type getStateType() const = 0;

            virtual void setStateType( hash_type type ) = 0;

            virtual SmartPtr<IState> getState( u32 id ) const = 0;
            virtual void setState( u32 id, SmartPtr<IState> state ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // namespace fb

#endif  // ISystem_h__
