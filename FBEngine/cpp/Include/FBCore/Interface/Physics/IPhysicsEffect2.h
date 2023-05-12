#ifndef IPhysicsEffect2_h__
#define IPhysicsEffect2_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {
        class IPhysicsEffect2 : public ISharedObject
        {
        public:
            ~IPhysicsEffect2() override = default;

            virtual void handleEvent( const SmartPtr<IEvent> &event ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsEffect2_h__
