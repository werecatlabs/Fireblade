#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TaskManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/IProfile.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/System/Task.h>
#include <FBCore/Core/FSMManager.h>
#include <FBCore/System/TaskLock.h>

#include <utility>

#include "FBCore/Memory/PointerUtil.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TaskManager, ITaskManager );
    u32 TaskManager::m_idExt = 0;

    TaskManager::TaskManager()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "TaskManager" );
        }
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

            auto numTasks = static_cast<u32>( Thread::Task::Count );

            m_tasks.resize( numTasks );
            m_affinity.resize( numTasks );
            m_taskFlags.resize( numTasks );
            m_targetfps.resize( numTasks );
            m_nextUpdateTimes.resize( numTasks );
            m_taskIds.resize( numTasks );
            m_states.resize( numTasks );
            m_owners.resize( numTasks );
            m_threadHint.resize( numTasks );

            auto fsmManager = fb::make_ptr<FSMManager>();
            fsmManager->setGrowSize( numTasks );
            fsmManager->load( nullptr );
            setFSMManager( fsmManager );

            auto count = 0;
            for( auto &task : m_tasks )
            {
                task = fb::make_ptr<Task>();
                auto eTask = static_cast<Thread::Task>( count );
                task->setTask( eTask );
                task->load( nullptr );

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

            for( size_t i = 0; i < numTasks; ++i )
            {
                auto eTask = static_cast<Thread::Task>( i );
                m_taskIds[i] = eTask;
            }

            for( u32 i = 0; i < numTasks; ++i )
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
        unload( data );
        load( data );
    }

    void TaskManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            for( auto &task : m_tasks )
            {
                task->unload( nullptr );
            }

            setLoadingState( LoadingState::Unloaded );
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
            auto &applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto &threadPool = applicationManager->getThreadPool();

            auto &timer = applicationManager->getTimer();
            auto t = timer->now();

            if( threadPool )
            {
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
            }

            auto threadId = Thread::getCurrentThreadId();

            switch( threadId )
            {
            case Thread::ThreadId::Primary:
            {
                if( auto fsmManager = getFSMManager() )
                {
                    fsmManager->update();
                }
            }
            break;
            default:
                break;
            }

            auto taskManagerState = getState();
            switch( taskManagerState )
            {
            case State::FreeStep:
            {
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
                                        pTask->update();
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

                                        if( m_nextUpdateTimes[i] < t )
                                        {
                                            pTask->update();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                    default:
                    {
                        if( !m_threadHint.empty() )
                        {
                            auto numTasks = static_cast<size_t>( Thread::Task::Count );
                            for( size_t i = 0; i < numTasks; ++i )
                            {
                                if( m_threadHint[i] == static_cast<u32>( threadId ) )
                                {
                                    auto id = i;

                                    auto enabled =
                                        getFlags( static_cast<u32>( id ), Task::enabled_flag );
                                    if( enabled )
                                    {
                                        auto primary =
                                            getFlags( static_cast<u32>( id ), Task::primary_flag );
                                        if( !primary )
                                        {
                                            auto &pTask = m_tasks[i];

                                            if( m_nextUpdateTimes[i] < t )
                                            {
                                                pTask->update();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    };
                }
                else
                {
                    if( !m_threadHint.empty() )
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

                                    if( m_nextUpdateTimes[i] < t )
                                    {
                                        pTask->update();
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

    auto TaskManager::getAffinity( u32 id ) const -> u32
    {
        FB_ASSERT( !MathI::isFinite( id ) );
        FB_ASSERT( id < m_affinity.size() );
        return m_affinity[id];
    }

    auto TaskManager::getTask( Thread::Task taskId ) const -> SmartPtr<ITask>
    {
        if( isLoaded() )
        {
            auto iTaskIndex = static_cast<size_t>( taskId );
            return m_tasks[iTaskIndex];
        }

        return nullptr;
    }

    void TaskManager::setState( State state )
    {
        m_state = state;
    }

    auto TaskManager::getState() const -> ITaskManager::State
    {
        return m_state;
    }

    auto TaskManager::getTaskFlags( u32 id ) const -> u32
    {
        FB_ASSERT( id < m_taskFlags.size() );
        return m_taskFlags[id];
    }

    void TaskManager::setTaskFlags( u32 id, u32 flags )
    {
        FB_ASSERT( id < m_taskFlags.size() );
        m_taskFlags[id] = flags;
    }

    auto TaskManager::getTaskId( u32 id ) const -> Thread::Task
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

    auto TaskManager::getFlagsPtr( u32 id ) const -> atomic_u32 *
    {
        return const_cast<atomic_u32 *>( &m_taskFlags[id] );
    }

    auto TaskManager::getTargetFPS( u32 id ) const -> f64
    {
        FB_ASSERT( id < m_targetfps.size() );
        return m_targetfps[id];
    }

    void TaskManager::setTargetFPS( u32 id, f64 targetfps )
    {
        FB_ASSERT( id < m_targetfps.size() );
        m_targetfps[id] = targetfps;
    }

    auto TaskManager::getNextUpdateTime( u32 id ) const -> f64
    {
        return m_nextUpdateTimes[id];
    }

    void TaskManager::setNextUpdateTime( u32 id, f64 nextUpdateTime )
    {
        m_nextUpdateTimes[id] = nextUpdateTime;
    }

    auto TaskManager::getTaskIds( u32 id ) const -> Thread::Task
    {
        FB_ASSERT( id < m_taskIds.size() );
        return m_taskIds[id];
    }

    void TaskManager::setTaskIds( u32 id, Thread::Task taskId )
    {
        FB_ASSERT( id < m_taskIds.size() );
        m_taskIds[id] = taskId;
    }

    auto TaskManager::getTaskState( u32 id ) const -> ITask::State
    {
        FB_ASSERT( id < m_states.size() );
        return m_states[id];
    }

    void TaskManager::setTaskState( u32 id, ITask::State state )
    {
        FB_ASSERT( id < m_states.size() );
        m_states[id] = state;
    }

    auto TaskManager::getOwner( u32 id ) const -> SmartPtr<ISharedObject>
    {
        FB_ASSERT( id < m_owners.size() );
        return m_owners[id];
    }

    void TaskManager::setOwner( u32 id, SmartPtr<ISharedObject> owner )
    {
        FB_ASSERT( id < m_owners.size() );
        m_owners[id] = owner;
    }

    auto TaskManager::isValid() const -> bool
    {
        return true;
    }

    void TaskManager::setFSMManager( SmartPtr<IFSMManager> fsmManager )
    {
        m_fsmManager = fsmManager;
    }

    void TaskManager::setState( u32 id, ITask::State state )
    {
        FB_ASSERT( id < m_states.size() );
        m_states[id] = state;
    }

    auto TaskManager::getState( u32 id ) const -> ITask::State
    {
        FB_ASSERT( id < m_states.size() );
        return m_states[id];
    }

    auto TaskManager::getTasks() const -> Array<SmartPtr<ITask>>
    {
        return Array<SmartPtr<ITask>>( m_tasks.begin(), m_tasks.end() );
    }

    void TaskManager::wait()
    {
        for( auto &t : m_tasks )
        {
            while( !( t->getState() == ITask::State::None || t->getState() == ITask::State::Idle ) )
            {
                Thread::yield();
            }
        }
    }

    void TaskManager::stop()
    {
        auto applicationManager = core::ApplicationManager::instance();
        if( applicationManager->isRunning() )
        {
            for( auto &t : m_tasks )
            {
                t->stop();
            }
        }
    }

    void TaskManager::reset()
    {
        for( auto &t : m_tasks )
        {
            t->reset();
        }
    }

    auto TaskManager::lockTask( Thread::Task taskId ) -> TaskLock
    {
        if( auto task = getTask( taskId ) )
        {
            return TaskLock( task );
        }

        return {};
    }

    auto TaskManager::getNumTasks() const -> u32
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

    TaskManager::Lock::Lock( SmartPtr<ITaskManager> taskManager ) :
        m_taskManager( std::move( taskManager ) )
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

    TaskManager::Lock::Lock() = default;

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
