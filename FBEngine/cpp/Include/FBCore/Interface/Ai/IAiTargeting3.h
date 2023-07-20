#ifndef IAiTargeting3_h__
#define IAiTargeting3_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{

    class IAiTargeting3 : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAiTargeting3() override = default;

        virtual Vector3F getTargetPosition() const = 0;
        virtual void setTargetPosition( const Vector3F &targetPosition ) = 0;

        virtual SmartPtr<scene::IActor> getOwner() const = 0;
        virtual void setOwner( SmartPtr<scene::IActor> owner ) = 0;

        virtual SmartPtr<scene::IActor> getTarget() const = 0;
        virtual void setTarget( SmartPtr<scene::IActor> target ) = 0;
    };
}  // end namespace fb

#endif  // IAiTargeting3_h__
