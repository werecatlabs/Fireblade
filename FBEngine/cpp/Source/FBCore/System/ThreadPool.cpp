#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ThreadPool.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/System/WorkerThread.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IWorkerThread.h>

#if defined( _MSC_VER ) && _MSC_VER > 1600  // (Visual Studio 2010)
#    include <thread>
#else
#    ifdef FB_USE_BOOST
#        include <boost/thread/thread.hpp>
#    endif
#endif

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ThreadPool, IThreadPool );

    ThreadPool::ThreadPool()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "ThreadPool" );
        }
    }

    ThreadPool::~ThreadPool()
    {
        unload( nullptr );
    }

    void ThreadPool::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto fsmManager = applicationManager->getFsmManager();
            FB_ASSERT( fsmManager );

            auto fsm = fsmManager->createFSM();

            auto fsmListener = fb::make_ptr<ThreadPoolFSMListener>();
            fsmListener->setOwner( this );
            fsm->addListener( fsmListener );

            m_fsmListener = fsmListener;
            m_fsm = fsm;

            for( auto &m_workerThread : m_workerThreads )
            {
                auto workerThread = factoryManager->make_object<IWorkerThread>();
                FB_ASSERT( workerThread );

                workerThread->load( data );
                m_workerThread = workerThread;
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ThreadPool::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fsmManager = applicationManager->getFsmManager();
                FB_ASSERT( fsmManager );

                if( auto fsm = getFSM() )
                {
                    if( auto fsmListener = getFSMListener() )
                    {
                        fsm->removeListener( fsmListener );
                        setFSMListener( nullptr );
                    }

                    fsmManager->destroyFSM( fsm );
                    setFSM( nullptr );
                }

                for( auto &workerThread : m_workerThreads )
                {
                    workerThread->unload( data );
                }

                m_workerThreads.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ThreadPool::addWorkerThread() -> SmartPtr<IWorkerThread>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        if( factoryManager )
        {
            auto workerThread = factoryManager->make_object<IWorkerThread>();
            FB_ASSERT( workerThread );

            m_workerThreads.push_back( workerThread );
            ++m_numThreads;

            return workerThread;
        }

        return nullptr;
    }

    auto ThreadPool::getThread( u32 index ) -> SmartPtr<IWorkerThread>
    {
        if( isLoaded() )
        {
            return m_workerThreads[index];
        }

        return nullptr;
    }

    auto ThreadPool::getNumThreads() const -> u32
    {
        return m_numThreads;
    }

    void ThreadPool::setNumThreads( u32 numThreads )
    {
        m_numThreads = numThreads;

        m_workerThreads.resize( numThreads );
        m_states.resize( numThreads );
        m_threads.resize( numThreads );
        m_targetFPS.resize( numThreads );
        m_threadId.resize( numThreads );
        m_queueLengthMilliseconds.resize( numThreads );
        m_reserveFlags.resize( numThreads );
    }

    auto ThreadPool::getState() const -> IThreadPool::State
    {
        if( auto fsm = getFSM() )
        {
            return fsm->getState<State>();
        }

        return State::None;
    }

    void ThreadPool::setState( State state )
    {
        auto fsm = getFSM();
        FB_ASSERT( fsm );

        if( fsm )
        {
            FB_ASSERT( fsm->isValid() );
            fsm->setState( state );
        }
    }

    void ThreadPool::stop()
    {
        for( auto &workerThread : m_workerThreads )
        {
            workerThread->stop();
        }
    }

    auto ThreadPool::isValid() const -> bool
    {
        const auto &loadingState = getLoadingState();

        switch( loadingState )
        {
        case LoadingState::Unloaded:
        {
        }
        break;
        case LoadingState::Loaded:
        {
            auto fsm = getFSM();
            return fsm && fsm->isValid();
        }
        break;
        }

        return true;
    }

    auto ThreadPool::getFSM() const -> SmartPtr<IFSM>
    {
        return m_fsm;
    }

    void ThreadPool::setFSM( SmartPtr<IFSM> fsm )
    {
        m_fsm = fsm;
    }

    auto ThreadPool::getFSMListener() const -> SmartPtr<IFSMListener>
    {
        return m_fsmListener;
    }

    void ThreadPool::setFSMListener( SmartPtr<IFSMListener> listener )
    {
        m_fsmListener = listener;
    }

    auto ThreadPool::handleEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        switch( eventType )
        {
        case IFSM::Event::Change:
        {
        }
        break;
        case IFSM::Event::Enter:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Start:
            {
                for( auto &workerThread : m_workerThreads )
                {
                    workerThread->setState( IWorkerThread::State::Start );
                }
            }
            break;
            case State::Stop:
            {
                for( auto &workerThread : m_workerThreads )
                {
                    workerThread->setState( IWorkerThread::State::Stop );
                }
            }
            break;
            }
        }
        break;
        case IFSM::Event::Leave:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Start:
            {
            }
            break;
            case State::Stop:
            {
            }
            break;
            }
        }
        break;
        case IFSM::Event::Pending:
        {
        }
        break;
        case IFSM::Event::Complete:
        {
        }
        break;
        case IFSM::Event::NewState:
        {
        }
        break;
        case IFSM::Event::WaitForChange:
        {
        }
        break;
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    auto ThreadPool::ThreadPoolFSMListener::handleEvent( u32 state, IFSM::Event eventType )
        -> IFSM::ReturnType
    {
        if( auto owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                return m_owner->handleEvent( state, eventType );
            }
        }

        return IFSM::ReturnType::NotLoaded;
    }

    auto ThreadPool::ThreadPoolFSMListener::getOwner() const -> ThreadPool *
    {
        return m_owner;
    }

    void ThreadPool::ThreadPoolFSMListener::setOwner( ThreadPool *owner )
    {
        m_owner = owner;
    }
}  // end namespace fb
