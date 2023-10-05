#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Task.h>
#include <FBCore/System/TaskManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/IProfile.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Core/FSMManager.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Task, ITask );

    Task::Task( const Task &other )
    {
    }

    void Task::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );

        auto applicationManager = core::IApplicationManager::instance();
        auto taskManager = fb::static_pointer_cast<TaskManager>( applicationManager->getTaskManager() );

        auto taskId = getTask();

        auto fsmManager = fb::make_ptr<FSMManager>();
        fsmManager->load( data );
        applicationManager->setFsmManagerByTask( taskId, fsmManager );
        m_fsm = fsmManager->createFSM();

        m_taskFlags = taskManager->getFlagsPtr( (u32)taskId );
        setLoadingState( LoadingState::Loaded );
    }

    void Task::reload( SmartPtr<ISharedObject> data )
    {
        unload( data );
        load( data );
    }

    void Task::unload( SmartPtr<ISharedObject> data )
    {
        if( isLoaded() )
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::IApplicationManager::instance();
            auto taskManager =
                fb::static_pointer_cast<TaskManager>( applicationManager->getTaskManager() );

            if( taskManager )
            {
                if( auto fsmManager = applicationManager->getFsmManagerByTask( getTask() ) )
                {
                    fsmManager->destroyFSM( m_fsm );
                }
            }

            m_fsm = nullptr;

            m_profile = nullptr;
            m_jobs.clear();

            setLoadingState( LoadingState::Unloaded );
        }
    }

    void Task::update()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto taskManager = applicationManager->getTaskManager();

            auto eTask = getTask();
            if( auto fsmManager = applicationManager->getFsmManagerByTask( eTask ) )
            {
                fsmManager->update();
            }

            if( auto fsm = getFSM() )
            {
                Flags flags( this );

                auto eState = static_cast<State>( fsm->getCurrentState() );
                switch( eState )
                {
                case State::None:
                {
                }
                break;
                case State::Idle:
                {
                }
                break;
                case State::Stopped:
                {
                }
                break;
                case State::Executing:
                {
                    auto timer = applicationManager->getTimer();

                    auto profile = getProfile();
                    if( profile )
                    {
                        profile->start();
                    }

                    auto nextUpdateTime = getNextUpdateTime();
                    if( nextUpdateTime < timer->now() )
                    {
                        auto eCurrentTask = Thread::getCurrentTask();

                        Thread::setCurrentTask( eTask );

                        auto taskFlags = getThreadTaskFlags();

                        if( eTask != Thread::Task::Primary )
                        {
                            if( isPrimary() )
                            {
                                taskFlags = taskFlags | Thread::Primary_Flag;
                            }
                        }
                        else
                        {
                            auto tasks = taskManager->getTasks();
                            for(auto task : tasks)
                            {
                                if( task->isPrimary() )
                                {
                                    taskFlags = taskFlags | task->getThreadTaskFlags();
                                }
                            }
                        }

                        Thread::setTaskFlags( taskFlags );

                        timer->update();

                        if( !m_jobs.empty() )
                        {
                            SmartPtr<IJob> job;
                            while( m_jobs.try_pop( job ) )
                            {
                                job->execute();
                            }
                        }

                        if( auto owner = getOwner() )
                        {
                            try
                            {
                                owner->preUpdate();
                                owner->update();
                                owner->postUpdate();
                            }
                            catch( std::exception &e )
                            {
                                FB_LOG_EXCEPTION( e );
                            }
                        }

                        auto nextUpdateTime = getNextUpdateTime();
                        auto targetFPS = getTargetFPS();
                        if( targetFPS > std::numeric_limits<time_interval>::epsilon() )
                        {
                            auto rate = 1.0 / targetFPS;
                            nextUpdateTime = nextUpdateTime + rate;
                            setNextUpdateTime( nextUpdateTime );
                        }

                        Thread::setCurrentTask( eCurrentTask );
                    }

                    if( profile )
                    {
                        profile->end();
                    }
                }
                break;
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Task::reset()
    {
        m_tickCount = 0;
    }

    void Task::queueJob( SmartPtr<IJob> job )
    {
        if( job )
        {
            m_jobs.push( job );
        }
    }

    void Task::setPrimary( bool usePrimary )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( usePrimary )
        {
            ( *m_taskFlags ) = *m_taskFlags | ITask::primary_flag;
        }
        else
        {
            ( *m_taskFlags ) = *m_taskFlags & ~primary_flag;
        }
    }

    bool Task::isPrimary() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return ( *m_taskFlags & primary_flag ) != 0;
    }

    void Task::setRecycle( bool recycle )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( recycle )
        {
            ( *m_taskFlags ) = *m_taskFlags | ITask::recycle_flag;
        }
        else
        {
            ( *m_taskFlags ) = *m_taskFlags & ~recycle_flag;
        }
    }

    bool Task::getRecycle() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return ( *m_taskFlags & recycle_flag ) != 0;
    }

    void Task::setAffinity( s32 id )
    {
        m_affinity = id;
    }

    s32 Task::getAffinity() const
    {
        return m_affinity;
    }

    bool Task::isParallel() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return ( *m_taskFlags & parallel_flag ) != 0;
    }

    void Task::setParallel( bool parallel )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( parallel )
        {
            ( *m_taskFlags ) = *m_taskFlags | ITask::parallel_flag;
        }
        else
        {
            ( *m_taskFlags ) = *m_taskFlags & ~parallel_flag;
        }
    }

    bool Task::isEnabled() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return ( *m_taskFlags & enabled_flag ) != 0;
    }

    void Task::setEnabled( bool enabled )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( enabled )
        {
            ( *m_taskFlags ) = *m_taskFlags | ITask::enabled_flag;
        }
        else
        {
            ( *m_taskFlags ) = *m_taskFlags & ~enabled_flag;
        }
    }

    Thread::Task Task::getTask() const
    {
        return m_taskId;
    }

    void Task::setTask( Thread::Task task )
    {
        m_taskId = task;
    }

    u32 Task::getThreadTaskFlags() const
    {
        return m_threadTaskFlags;
    }

    void Task::setThreadTaskFlags( u32 threadTaskFlags )
    {
        m_threadTaskFlags = threadTaskFlags;
    }

    void Task::stop()
    {
        if( !isLoaded() )
        {
            return;
        }

        FB_DEBUG_TRACE;

        --m_stopped;

        if( isUpdating() )
        {
            const auto task = getTask();
            const auto currentTask = Thread::getCurrentTask();

            if( task != currentTask )
            {
                if( auto fsm = getFSM() )
                {
                    if( fsm->isLoaded() )
                    {
                        while( fsm->getCurrentState() != static_cast<u8>( State::Stopped ) )
                        {
                            setState( State::Stopped );
                            Thread::yield();
                        }
                    }
                }
            }

            FB_ASSERT( isStopped() == true );
        }
    }

    void Task::start()
    {
        FB_DEBUG_TRACE;

        ++m_stopped;
    }

    bool Task::isStopped() const
    {
        return m_stopped < 0;
    }

    bool Task::isUpdating() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return ( *m_taskFlags & updating_flag ) != 0;
    }

    void Task::setUpdating( bool updating )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( updating )
        {
            ( *m_taskFlags ) = *m_taskFlags | ITask::updating_flag;
        }
        else
        {
            ( *m_taskFlags ) = *m_taskFlags & ~updating_flag;
        }
    }

    bool Task::isExecuting() const
    {
        return getState() == State::Executing;
    }

    f64 Task::getTargetFPS() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getTargetFPS( static_cast<u32>( getTask() ) );
        }

        return 0.0;
    }

    void Task::setTargetFPS( f64 targetFPS )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setTargetFPS( static_cast<u32>( getTask() ), targetFPS );
        }
    }

    bool Task::getUseFixedTime() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return ( *m_taskFlags & fixed_time_flag ) != 0;
    }

    void Task::setUseFixedTime( bool useFixedTime )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );

        if( useFixedTime )
        {
            ( *m_taskFlags ) = *m_taskFlags | ITask::fixed_time_flag;
        }
        else
        {
            ( *m_taskFlags ) = *m_taskFlags & ~fixed_time_flag;
        }
    }

    time_interval Task::getNextUpdateTime() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getNextUpdateTime( static_cast<u32>( getTask() ) );
        }

        return 0.0;
    }

    void Task::setNextUpdateTime( time_interval nextUpdateTime )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setNextUpdateTime( static_cast<u32>( getTask() ), nextUpdateTime );
        }
    }

    void Task::calculateAutoFPS()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto fps = 1.0 / timer->getSmoothDeltaTime();
        if( fps > 0.0 )
        {
            auto targetFPS = (f64)( ( (s32)fps - 50 ) / 100 * 100 );
            targetFPS = Math<f64>::clamp( targetFPS, 30.0, 60.0 );

            setAutoFPS( targetFPS );
        }
        else
        {
            setAutoFPS( 60.0 );
        }
    }

    f64 Task::getAutoFPS() const
    {
        return m_autoFPS;
    }

    void Task::setAutoFPS( f64 autoFPS )
    {
        m_autoFPS = autoFPS;
    }

    u32 Task::getTicks() const
    {
        return m_tickCount;
    }

    SmartPtr<IProfile> Task::getProfile() const
    {
        return m_profile;
    }

    void Task::setProfile( SmartPtr<IProfile> profile )
    {
        m_profile = profile;
    }

    SmartPtr<IFSM> Task::getFSM() const
    {
        return m_fsm;
    }

    void Task::setState( State state )
    {
        if( auto fsm = getFSM() )
        {
            auto iState = static_cast<u8>( state );
            fsm->setNewState( iState );
        }
    }

    ITask::State Task::getState() const
    {
        if( auto fsm = getFSM() )
        {
            auto iState = fsm->getCurrentState();
            return static_cast<State>( iState );
        }

        return State::None;
    }

    SmartPtr<ISharedObject> Task::getOwner() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getOwner( static_cast<u32>( getTask() ) );
        }

        return nullptr;
    }

    void Task::setOwner( SmartPtr<ISharedObject> owner )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setOwner( static_cast<u32>( getTask() ), owner );
        }
    }

    Task::Flags::Flags( SmartPtr<Task> task )
    {
        setTask( task );

        if( task )
        {
            task->setUpdating( true );

            if( auto fsm = task->getFSM() )
            {
                if( !fsm->isPending() )
                {
                    if( task->isStopped() )
                    {
                        fsm->setNewState( static_cast<u32>( ITask::State::Stopped ), true );
                    }
                    else if( !task->isStopped() )
                    {
                        if( fsm->getCurrentState() == static_cast<u32>( ITask::State::Idle ) )
                        {
                            fsm->setNewState( static_cast<u32>( ITask::State::Executing ), true );
                        }
                        else
                        {
                            fsm->setNewState( static_cast<u32>( ITask::State::Idle ), true );
                        }
                    }
                }
            }
        }
    }

    Task::Flags::~Flags()
    {
        if( auto task = getTask() )
        {
            task->setUpdating( false );

            if( auto fsm = task->getFSM() )
            {
                auto currentState = static_cast<Task::State>( fsm->getCurrentState() );
                if( currentState == State::Executing )
                {
                    fsm->setNewState( static_cast<u32>( Task::State::Idle ), true );
                }
            }
        }
    }

    SmartPtr<Task> Task::Flags::getTask() const
    {
        return m_task;
    }

    void Task::Flags::setTask( SmartPtr<Task> task )
    {
        m_task = task;
    }
}  // end namespace fb
