#ifndef ITaskLock_h__
#define ITaskLock_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class ITaskLock : public ISharedObject
    {
    public:
        ~ITaskLock() override = default;

        virtual SmartPtr<ITask> getTask() const = 0;

        virtual void setTask( SmartPtr<ITask> task ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ITaskLock_h__
