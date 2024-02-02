#ifndef TaskManagerMinimal_h__
#define TaskManagerMinimal_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/System/Task.h>

namespace fb
{

    /** Default task manager implementation.  */
    class TaskManager : public ITaskManager
    {
    public:
        /* Used to lock all the tasks. */
        class Lock : public ISharedObject
        {
        public:
            Lock();
            explicit Lock( SmartPtr<ITaskManager> taskManager );
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

        void addJobAllTasks( SmartPtr<IJob> job );

        Array<SmartPtr<ITask>> getTasks() const override;

        void wait() override;
        void stop() override;
        void reset() override;

        TaskLock lockTask( Thread::Task taskId ) override;

        u32 getNumTasks() const override;

        SmartPtr<ITask> getTask( Thread::Task taskId ) const override;

        void setState( State state ) override;
        State getState() const override;

        /** @copydoc ITask::setFlags */
        void setFlags( u32 id, u32 flag, bool value );

        /** @copydoc ITask::getFlags */
        bool getFlags( u32 id, u32 flag ) const;

        atomic_u32 *getFlagsPtr( u32 id ) const;

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

        SmartPtr<IFSMManager> &getFSMManager();
        const SmartPtr<IFSMManager> &getFSMManager() const;
        void setFSMManager( SmartPtr<IFSMManager> fsmManager );

        FB_CLASS_REGISTER_DECL;

    private:
        mutable SpinRWMutex m_mutex;

        SmartPtr<IFSMManager> m_fsmManager;

        Atomic<State> m_state = State::None;

        Array<SmartPtr<Task>> m_tasks;
        Array<atomic_u32> m_affinity;
        Array<atomic_u32> m_taskFlags;
        Array<atomic_f64> m_targetfps;
        Array<atomic_f64> m_nextUpdateTimes;
        Array<Thread::Task> m_taskIds;
        Array<ITask::State> m_states;
        Array<SmartPtr<ISharedObject>> m_owners;
        Array<atomic_u32> m_threadHint;

        u32 m_idCount = 0;

        static u32 m_idExt;
    };

    inline bool TaskManager::getFlags( u32 id, u32 flag ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );

        FB_ASSERT( id < m_taskFlags.size() );
        const auto &flags = m_taskFlags[id].load();
        return ( flags & flag ) != 0;
    }

    inline SmartPtr<IFSMManager> &TaskManager::getFSMManager()
    {
        return m_fsmManager;
    }

    inline const SmartPtr<IFSMManager> &TaskManager::getFSMManager() const
    {
        return m_fsmManager;
    }

}  // end namespace fb

#endif  // TaskManager_h__
