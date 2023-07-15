#ifndef TaskLock_h__
#define TaskLock_h__

#include <FBCore/FBCorePrerequisites.h>

namespace fb
{

    class TaskLock 
    {
    public:
        TaskLock();
        TaskLock( SmartPtr<ITask> task );
        ~TaskLock();

        SmartPtr<ITask> getTask() const;
        void setTask( SmartPtr<ITask> task );

    private:
        AtomicWeakPtr<ITask> m_task;
    };

}  // namespace fb

#endif  // TaskLock_h__
