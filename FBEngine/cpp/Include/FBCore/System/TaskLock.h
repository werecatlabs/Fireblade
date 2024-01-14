#ifndef __FB_TaskLock_h__
#define __FB_TaskLock_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{

    class TaskLock
    {
    public:
        TaskLock();
        explicit TaskLock( SmartPtr<ITask> task );
        ~TaskLock();

        SmartPtr<ITask> getTask() const;
        void setTask( SmartPtr<ITask> task );

    private:
        SmartPtr<ITask> m_task;
    };

}  // namespace fb

#endif  // TaskLock_h__
