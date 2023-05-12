#ifndef IScheduler_h__
#define IScheduler_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class IScheduler : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IScheduler() override = default;

        virtual void schedule( SmartPtr<ITask> task ) = 0;
        virtual void wait() = 0;

        virtual SmartPtr<ITask> getNext() = 0;
    };
}  // end namespace fb

#endif  // IScheduler_h__
