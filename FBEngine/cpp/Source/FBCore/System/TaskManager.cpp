#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TaskManager.h>
#include <FBCore/Base/BitUtil.h>
#include <FBCore/Base/DebugTrace.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/IProfile.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/Task.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb::render, TaskManager, CSharedObject<ITaskManager> );
    u32 TaskManager::m_idExt = 0;

    TaskManager::TaskManager()
    {
        //setGarbageCollected( false );
    }

    TaskManager::~TaskManager()
    {
        TaskManager::unload( nullptr );
    }

    void TaskManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto count = 0;
            for( auto &task : m_tasks )
            {
                auto eTask = static_cast<Thread::Task>( count );
                task.setTask( eTask );
                task.load( nullptr );
                task.addReference();

                count++;
            }

            for( auto &nextUpdateTime : m_nextUpdateTimes )
            {
                nextUpdateTime = 0.0;
            }

            for( auto &nextUpdateTime : m_nextUpdateTimes )
            {
                nextUpdateTime = 0.0;
            }

            for( size_t i = 0; i < static_cast<size_t>( Thread::Task::Count ); ++i )
            {
                m_taskIds[i] = static_cast<Thread::Task>( i );
            }

            for( u32 i = 0; i < static_cast<u32>( Thread::Task::Count ); ++i )
            {
                setFlags( i, Task::enabled_flag, true );
                setFlags( i, Task::executing_flag, false );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TaskManager::reload( SmartPtr<ISharedObject> data )
    {
    }

    void TaskManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            for( auto &task : m_tasks )
            {
                task.unload( nullptr );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TaskManager::update()
    {
        FB_DEBUG_TRACE;

        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto threadPool = applicationManager->getThreadPool();
            FB_ASSERT( threadPool );

            auto timer = applicationManager->getTimer();
            auto t = timer->now();

            auto taskCount = 0;
            const auto numThreads = threadPool->getNumThreads();
            for( size_t i = 0; i < m_threadHint.size(); ++i )
            {
                auto &hint = m_threadHint[i];

                auto id = i;
                if( const auto enabled = getFlags( static_cast<u32>( id ), Task::enabled_flag ) )
                {
                    hint = MathI::Mod( taskCount, numThreads );
                    taskCount++;
                }
            }

            auto threadId = Thread::getCurrentThreadId();

            auto taskManagerState = getState();
            switch( taskManagerState )
            {
            case State::FreeStep:
            {
                auto threadPool = applicationManager->getThreadPool();
                if( threadPool )
                {
                    switch( threadId )
                    {
                    case Thread::ThreadId::Primary:
                    {
                        auto numThreads = threadPool->getNumThreads();
                        if( numThreads == 0 )
                        {
                            auto numTasks = static_cast<size_t>( Thread::Task::Count );
                            for( size_t i = 0; i < numTasks; ++i )
                            {
                                auto id = i;

                                auto enabled = getFlags( static_cast<u32>( id ), Task::enabled_flag );
                                if( enabled )
                                {
                                    auto &pTask = m_tasks[i];
                                    // if( m_nextUpdateTimes[i] < t )
                                    {
                                        auto paused = pTask.isPaused();
                                        if( !paused )
                                        {
                                            auto state = pTask.getState();
                                            if( state == ITask::State::Idle ||
                                                state == ITask::State::None )
                                            {
                                                pTask.setState( ITask::State::Executing );
                                                pTask.update();
                                                pTask.setState( ITask::State::Idle );
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            auto numTasks = static_cast<size_t>( Thread::Task::Count );
                            for( size_t i = 0; i < numTasks; ++i )
                            {
                                auto id = i;

                                auto primary = getFlags( static_cast<u32>( id ), Task::primary_flag );
                                if( primary )
                                {
                                    auto enabled =
                                        getFlags( static_cast<u32>( id ), Task::enabled_flag );
                                    if( enabled )
                                    {
                                        auto &pTask = m_tasks[i];
                                        auto paused = pTask.isPaused();
                                        if( !paused )
                                        {
                                            if( m_nextUpdateTimes[i] < t )
                                            {
                                                auto state = pTask.getState();
                                                if( state == ITask::State::Idle ||
                                                    state == ITask::State::None )
                                                {
                                                    pTask.setState( ITask::State::Executing );
                                                    pTask.update();
                                                    pTask.setState( ITask::State::Idle );
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                    default:
                    {
                        auto numTasks = static_cast<size_t>( Thread::Task::Count );
                        for( size_t i = 0; i < numTasks; ++i )
                        {
                            if( m_threadHint[i] == static_cast<u32>( threadId ) )
                            {
                                auto id = i;

                                auto enabled = getFlags( static_cast<u32>( id ), Task::enabled_flag );
                                if( enabled )
                                {
                                    auto primary =
                                        getFlags( static_cast<u32>( id ), Task::primary_flag );
                                    if( !primary )
                                    {
                                        auto &pTask = m_tasks[i];
                                        auto paused = pTask.isPaused();
                                        if( !paused )
                                        {
                                            if( m_nextUpdateTimes[i] < t )
                                            {
                                                auto state = pTask.getState();
                                                if( state == ITask::State::Idle ||
                                                    state == ITask::State::None )
                                                {
                                                    pTask.setState( ITask::State::Executing );
                                                    pTask.update();
                                                    pTask.setState( ITask::State::Idle );
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    }
                }
                else
                {
                    auto numTasks = static_cast<size_t>( Thread::Task::Count );
                    for( size_t i = 0; i < numTasks; ++i )
                    {
                        if( m_threadHint[i] == static_cast<u32>( threadId ) )
                        {
                            auto id = i;

                            auto enabled = getFlags( static_cast<u32>( id ), Task::enabled_flag );
                            if( enabled )
                            {
                                auto &pTask = m_tasks[i];
                                auto paused = pTask.isPaused();
                                if( !paused )
                                {
                                    if( m_nextUpdateTimes[i] < t )
                                    {
                                        auto state = pTask.getState();
                                        if( state == ITask::State::Idle || state == ITask::State::None )
                                        {
                                            pTask.setState( ITask::State::Executing );
                                            pTask.update();
                                            pTask.setState( ITask::State::Idle );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
            default:
            {
            }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TaskManager::setAffinity( u32 id, u32 affinity )
    {
        FB_ASSERT( !MathI::isFinite( id ) );
        FB_ASSERT( id < m_affinity.size() );
        m_affinity[id] = affinity;
    }

    u32 TaskManager::getAffinity( u32 id ) const
    {
        FB_ASSERT( !MathI::isFinite( id ) );
        FB_ASSERT( id < m_affinity.size() );
        return m_affinity[id];
    }

    SmartPtr<ITask> TaskManager::getTask( Thread::Task taskId ) const
    {
        if( isLoaded() )
        {
            auto iTaskIndex = static_cast<size_t>( taskId );
            auto task = (ITask *)&m_tasks[iTaskIndex];
            return SmartPtr<ITask>( task );
        }

        return nullptr;
    }

    void TaskManager::setState( State state )
    {
        m_state = state;
    }

    ITaskManager::State TaskManager::getState() const
    {
        return m_state;
    }

    u32 TaskManager::getTaskFlags( u32 id ) const
    {
        FB_ASSERT( id < m_taskFlags.size() );
        return m_taskFlags[id];
    }

    void TaskManager::setTaskFlags( u32 id, u32 flags )
    {
        FB_ASSERT( id < m_taskFlags.size() );
        m_taskFlags[id] = flags;
    }

    Thread::Task TaskManager::getTaskId( u32 id ) const
    {
        FB_ASSERT( id < m_taskIds.size() );
        return m_taskIds[id];
    }

    void TaskManager::setTaskId( u32 id, Thread::Task task )
    {
        FB_ASSERT( id < m_taskIds.size() );
        m_taskIds[id] = task;
    }

    void TaskManager::setFlags( u32 id, u32 flag, bool value )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        FB_ASSERT( id < m_taskFlags.size() );
        const auto &flags = m_taskFlags[id].load();
        m_taskFlags[id] = BitUtil::setFlagValue( flags, flag, value );
    }

    bool TaskManager::getFlags( u32 id, u32 flag ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );

        FB_ASSERT( id < m_taskFlags.size() );
        const auto &flags = m_taskFlags[id].load();
        return BitUtil::getFlagValue( flags, flag );
    }

    f64 TaskManager::getTargetFPS( u32 id ) const
    {
        FB_ASSERT( id < m_targetfps.size() );
        return m_targetfps[id];
    }

    void TaskManager::setTargetFPS( u32 id, f64 targetfps )
    {
        FB_ASSERT( id < m_targetfps.size() );
        m_targetfps[id] = targetfps;
    }

    f64 TaskManager::getNextUpdateTime( u32 id ) const
    {
        return m_nextUpdateTimes[id];
    }

    void TaskManager::setNextUpdateTime( u32 id, f64 nextUpdateTime )
    {
        m_nextUpdateTimes[id] = nextUpdateTime;
    }

    Thread::Task TaskManager::getTaskIds( u32 id ) const
    {
        FB_ASSERT( id < m_taskIds.size() );
        return m_taskIds[id];
    }

    void TaskManager::setTaskIds( u32 id, Thread::Task taskId )
    {
        FB_ASSERT( id < m_taskIds.size() );
        m_taskIds[id] = taskId;
    }

    ITask::State TaskManager::getTaskState( u32 id ) const
    {
        FB_ASSERT( id < m_states.size() );
        return m_states[id];
    }

    void TaskManager::setTaskState( u32 id, ITask::State state )
    {
        FB_ASSERT( id < m_states.size() );
        m_states[id] = state;
    }

    SmartPtr<ISharedObject> TaskManager::getOwner( u32 id ) const
    {
        FB_ASSERT( id < m_owners.size() );
        return m_owners[id];
    }

    void TaskManager::setOwner( u32 id, SmartPtr<ISharedObject> owner )
    {
        FB_ASSERT( id < m_owners.size() );
        m_owners[id] = owner;
    }

    bool TaskManager::isValid() const
    {
        return true;
    }

    void TaskManager::setState( u32 id, ITask::State state )
    {
        FB_ASSERT( id < m_states.size() );
        m_states[id] = state;
    }

    ITask::State TaskManager::getState( u32 id ) const
    {
        FB_ASSERT( id < m_states.size() );
        return m_states[id];
    }

    void TaskManager::queueTask( SmartPtr<ITask> task )
    {
    }

    void TaskManager::updateParallelTasks()
    {
    }

    Array<SmartPtr<ITask>> TaskManager::getTasks() const
    {
        return Array<SmartPtr<ITask>>();
    }

    void TaskManager::wait()
    {
        for( auto &t : m_tasks )
        {
            while( !( t.getState() == ITask::State::None || t.getState() == ITask::State::Idle ) )
            {
                Thread::yield();
            }
        }
    }

    void TaskManager::stop()
    {
        for( auto &t : m_tasks )
        {
            t.stop();
        }
    }

    void TaskManager::reset()
    {
        for( auto &t : m_tasks )
        {
            t.reset();
        }
    }

    SmartPtr<ITaskLock> TaskManager::lockTask( Thread::Task taskId )
    {
        if( auto task = getTask( taskId ) )
        {
            return fb::make_ptr<Task::Lock>( task );
        }

        return nullptr;
    }

    u32 TaskManager::getNumTasks() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );

        auto count = 0;

        for( auto &flags : m_taskFlags )
        {
            if( ( flags & Task::enabled_flag ) != 0 )
            {
                ++count;
            }
        }

        return count;
    }

    SmartPtr<ITask> TaskManager::addTask( u32 id, f64 updateFrequency )
    {
        // TaskMinimalPtr task(new TaskMinimal(id, updateFrequency, this), true);
        // m_tasks.push_back(task);
        return SmartPtr<ITask>();
    }

    void TaskManager::removeTask( u32 id )
    {
        FB_ASSERT_TRUE( true );  // not implemented
    }

    void TaskManager::destroyAllTasks()
    {
    }

    void TaskManager::OnQueueTask( SmartPtr<ITask> &task )
    {
        // task->setQueueState(ITask::TQS_QUEUED);
        // task->setState(ITask::TPS_QUEUED);
    }

    void TaskManager::OnTaskRemoveFromQueue( SmartPtr<ITask> &task )
    {
        // auto engine = core::IApplicationManager::instance();
        // SmartPtr<ITimer>& timer = engine->getMainTimer();
        // u32 currentTime = timer->getTimeMilliseconds();

        // SmartPtr<TaskDataStandard> data;// = task->getTaskData();
        ////task->setState(ITask::TQS_READY);
        ////task->setState(ITask::TPS_READY);
        // data->setLastUpdateTime(currentTime);
    }

    void TaskManager::OnTaskUpdateStart( SmartPtr<ITask> &task )
    {
        // task->setState(ITask::TPS_EXECUTING);
    }

    void TaskManager::OnTaskUpdateEnd( SmartPtr<ITask> &task )
    {
        // task->setState(ITask::TPS_COMPLETED);
    }

    TaskManager::Lock::Lock( SmartPtr<ITaskManager> taskManager ) : m_taskManager( taskManager )
    {
        auto tasks = m_taskManager->getTasks();
        for( auto task : tasks )
        {
            if( task )
            {
                task->stop();
            }
        }
    }

    TaskManager::Lock::Lock()
    {
    }

    TaskManager::Lock::~Lock()
    {
        auto tasks = m_taskManager->getTasks();
        for( auto task : tasks )
        {
            if( task )
            {
                task->start();
            }
        }
    }
}  // end namespace fb
