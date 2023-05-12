#ifndef _IIKGOAL_H
#define _IIKGOAL_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    
    class IIKGoal : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IIKGoal() override = default;

        virtual ISharedObject *GetTarget() = 0;
        virtual ISharedObject *GetEffector() = 0;

        virtual Vector3F GetTargetPosition() const = 0;
        virtual Vector3F GetEffectorPosition() const = 0;

        virtual void SetWeight( f32 weight ) = 0;
        virtual f32 GetWeight() const = 0;
    };

}  // end namespace fb

#endif
