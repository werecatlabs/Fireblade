#ifndef ITaskLock_h__
#define ITaskLock_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for a task lock. */
    class ITaskLock : public ISharedObject
    {
    public:
        /** Destructor. */
        ~ITaskLock() override = default;

        /** Returns the task. */
        virtual SmartPtr<ITask> getTask() const = 0;

        /** Sets the task. */
        virtual void setTask( SmartPtr<ITask> task ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // ITaskLock_h__
