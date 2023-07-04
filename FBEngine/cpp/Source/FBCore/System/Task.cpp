#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Task.h>
#include <FBCore/System/TaskManager.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Interface/System/IProfile.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Task, SharedObject<ITask> );

    void Task::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );

        setLoadingState( LoadingState::Loaded );
    }

    void Task::reload( SmartPtr<ISharedObject> data )
    {
    }

    void Task::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );
        m_profile = nullptr;
        m_jobs.clear();
        setLoadingState( LoadingState::Unloaded );
    }

    void Task::update()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto profile = getProfile();
            if( profile )
            {
                profile->start();
            }

            auto nextUpdateTime = getNextUpdateTime();
            if( nextUpdateTime < timer->now() )
            {
                auto task = getTask();
                auto prevTask = Thread::getCurrentTask();
                Thread::setCurrentTask( task );

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

                auto &gc = GarbageCollector::instance();
                gc.update();

                auto nextUpdateTime = getNextUpdateTime();
                auto targetFPS = getTargetFPS();
                if( targetFPS > std::numeric_limits<time_interval>::epsilon() )
                {
                    auto rate = 1.0 / targetFPS;
                    nextUpdateTime = nextUpdateTime + rate;
                    setNextUpdateTime( nextUpdateTime );
                }

                Thread::setCurrentTask( prevTask );
            }

            if( profile )
            {
                profile->end();
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
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            auto taskId = getTask();
            taskManager->setFlags( static_cast<u32>( taskId ), primary_flag, usePrimary );
        }
    }

    bool Task::isPrimary() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            auto taskId = getTask();
            return taskManager->getFlags( static_cast<u32>( taskId ), primary_flag );
        }

        return false;
    }

    void Task::setRecycle( bool recycle )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setFlags( static_cast<u32>( getTask() ), recycle_flag, recycle );
        }
    }

    bool Task::getRecycle() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getFlags( static_cast<u32>( getTask() ), recycle_flag );
        }

        return false;
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
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getFlags( static_cast<u32>( getTask() ), parallel_flag );
        }

        return false;
    }

    void Task::setParallel( bool parallel )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setFlags( static_cast<u32>( getTask() ), parallel_flag, parallel );
        }
    }

    bool Task::isEnabled() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getFlags( static_cast<u32>( getTask() ), enabled_flag );
        }

        return false;
    }

    void Task::setEnabled( bool enabled )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setFlags( static_cast<u32>( getTask() ), enabled_flag, enabled );
        }
    }

    bool Task::isPaused() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            return taskManager->getFlags( static_cast<u32>( getTask() ), paused_flag );
        }

        return false;
    }

    void Task::setPaused( bool paused )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setFlags( static_cast<u32>( getTask() ), paused_flag, paused );
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

    void Task::stop()
    {
        FB_DEBUG_TRACE;

#if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        std::cout << boost::stacktrace::stacktrace();
#endif

        --m_stopped;

        if( isExecuting() )
        {
            auto task = getTask();
            auto currentTask = Thread::getCurrentTask();

            if( task != currentTask )
            {
                setPaused( true );

                while( isExecuting() )
                {
                    Thread::yield();
                }

                FB_ASSERT( isExecuting() == false );
            }
            else
            {
                setPaused( true );
            }

            FB_ASSERT( isStopped() == true );
        }
        else
        {
            setPaused( true );
        }
    }

    void Task::start()
    {
        FB_DEBUG_TRACE;

#if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        std::cout << boost::stacktrace::stacktrace();
#endif

        ++m_stopped;

        setPaused( false );
    }

    bool Task::isStopped() const
    {
        return m_stopped < 0;
    }

    bool Task::isUpdating() const
    {
        return m_isUpdating;
    }

    void Task::setUpdating( bool updating )
    {
        m_isUpdating = updating;
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
        return m_useFixedTime;
    }

    void Task::setUseFixedTime( bool useFixedTime )
    {
        m_useFixedTime = useFixedTime;
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

        return static_cast<time_interval>( 0.0 );
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

    void Task::setState( State state )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            taskManager->setTaskState( static_cast<u32>( getTask() ), state );
        }
    }

    ITask::State Task::getState() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto pTaskManager = applicationManager->getTaskManager();
        auto taskManager = fb::static_pointer_cast<TaskManager>( pTaskManager );
        if( taskManager )
        {
            auto eTask = getTask();
            return taskManager->getTaskState( static_cast<u32>( eTask ) );
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

    Task::Lock::Lock( SmartPtr<ITask> task )
    {
        FB_DEBUG_TRACE;

        if( task )
        {
            setTask( task );

#if !FB_FINAL
            FB_LOG( "Task id: " + Thread::getTaskName( task->getTask() ) );
#endif

            task->stop();
        }
    }

    Task::Lock::Lock()
    {
        FB_DEBUG_TRACE;
    }

    Task::Lock::~Lock()
    {
        FB_DEBUG_TRACE;

        if( auto task = getTask() )
        {
            task->start();
        }
    }

    SmartPtr<ITask> Task::Lock::getTask() const
    {
        auto p = m_task.load();
        return p.lock();
    }

    void Task::Lock::setTask( SmartPtr<ITask> task )
    {
        m_task = task;
    }
}  // end namespace fb
