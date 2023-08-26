#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TaskLock.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/System/ITask.h>

namespace fb
{

    TaskLock::TaskLock( SmartPtr<ITask> task )
    {
        FB_DEBUG_TRACE;

        if( task )
        {
            setTask( task );

#if !FB_FINAL
            FB_LOG( "Task stop id: " + Thread::getTaskName( task->getTask() ) );
#endif

            task->stop();
        }
    }

    TaskLock::TaskLock()
    {
        FB_DEBUG_TRACE;
    }

    TaskLock::~TaskLock()
    {
        FB_DEBUG_TRACE;

        if( auto task = getTask() )
        {
#if !FB_FINAL
            FB_LOG( "Task start id: " + Thread::getTaskName( task->getTask() ) );
#endif

            task->start();
        }
    }

    SmartPtr<ITask> TaskLock::getTask() const
    {
        auto p = m_task.load();
        return p.lock();
    }

    void TaskLock::setTask( SmartPtr<ITask> task )
    {
        m_task = task;
    }

}  // namespace fb
