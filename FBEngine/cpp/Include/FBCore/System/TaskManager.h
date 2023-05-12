#ifndef TaskManagerMinimal_h__
#define TaskManagerMinimal_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/FixedArray.h>
#include <FBCore/System/Task.h>

namespace fb
{

    /** Default task manager implementation.  */
    class TaskManager : public CSharedObject<ITaskManager>
    {
    public:
        /* Used to lock all the tasks. */
        class Lock : public CSharedObject<ISharedObject>
        {
        public:
            Lock();
            Lock( SmartPtr<ITaskManager> taskManager );
            ~Lock() override;

            SmartPtr<ITaskManager> m_taskManager;
        };

        /** Constructor */
        TaskManager();

        /** Destructor */
        ~TaskManager() override;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::reload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::update */
        void update() override;

        void OnQueueTask( SmartPtr<ITask> &task );
        void OnTaskRemoveFromQueue( SmartPtr<ITask> &task );
        void OnTaskUpdateStart( SmartPtr<ITask> &task );
        void OnTaskUpdateEnd( SmartPtr<ITask> &task );

        SmartPtr<ITask> addTask( u32 id, f64 updateFrequency );
        void removeTask( u32 id );
        void destroyAllTasks();

        void queueTask( SmartPtr<ITask> task );

        void updateParallelTasks();

        Array<SmartPtr<ITask>> getTasks() const override;

        void wait();
        void stop();
        void reset();

        SmartPtr<ITaskLock> lockTask( Thread::Task taskId );

        u32 getNumTasks() const override;

        SmartPtr<ITask> getTask( Thread::Task taskId ) const override;

        void setState( State state ) override;
        State getState() const override;

        /** @copydoc ITask::setPrimary */
        void setFlags( u32 id, u32 flag, bool value );

        /** @copydoc ITask::isPrimary */
        bool getFlags( u32 id, u32 flag ) const;

        /** @copydoc ITask::setState */
        void setState( u32 id, ITask::State state );

        /** @copydoc ITask::getState */
        ITask::State getState( u32 id ) const;

        /** @copydoc TaskManager::getTaskId */
        Thread::Task getTaskId( u32 id ) const;

        /** @copydoc TaskManager::setTaskId */
        void setTaskId( u32 id, Thread::Task task );

        Task *getTask( u32 id );

        u32 getAffinity( u32 id ) const;

        void setAffinity( u32 id, u32 affinity );

        u32 getTaskFlags( u32 id ) const;

        void setTaskFlags( u32 id, u32 taskFlags );

        f64 getTargetFPS( u32 id ) const;

        void setTargetFPS( u32 id, f64 targetfps );

        f64 getNextUpdateTime( u32 id ) const;

        void setNextUpdateTime( u32 id, f64 nextUpdateTime );

        Thread::Task getTaskIds( u32 id ) const;

        void setTaskIds( u32 id, Thread::Task taskId );

        ITask::State getTaskState( u32 id ) const;

        void setTaskState( u32 id, ITask::State state );

        SmartPtr<ISharedObject> getOwner( u32 id ) const;

        void setOwner( u32 id, SmartPtr<ISharedObject> object );

        bool isValid() const override;

        FB_CLASS_REGISTER_DECL;

    private:
        mutable SpinRWMutex m_mutex;

        Atomic<State> m_state = State::None;

        FixedArray<Task, static_cast<u32>( Thread::Task::Count )> m_tasks;
        FixedArray<atomic_u32, static_cast<u32>( Thread::Task::Count )> m_affinity;
        FixedArray<atomic_u32, static_cast<u32>( Thread::Task::Count )> m_taskFlags;
        FixedArray<atomic_f64, static_cast<u32>( Thread::Task::Count )> m_targetfps;
        FixedArray<atomic_f64, static_cast<u32>( Thread::Task::Count )> m_nextUpdateTimes;
        FixedArray<Thread::Task, static_cast<u32>( Thread::Task::Count )> m_taskIds;
        FixedArray<ITask::State, static_cast<u32>( Thread::Task::Count )> m_states;
        FixedArray<SmartPtr<ISharedObject>, static_cast<u32>( Thread::Task::Count )> m_owners;
        FixedArray<atomic_u32, static_cast<u32>( Thread::Task::Count )> m_threadHint;

        u32 m_idCount = 0;

        static u32 m_idExt;
    };
}  // end namespace fb

#endif  // TaskManager_h__
