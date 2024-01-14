#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/FSMManager.h>
#include <FBCore/Core/FSM.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FSMManager, IFSMManager );

    u32 FSMManager::m_idExt = 0;

    FSMManager::FSMManager() = default;

    FSMManager::~FSMManager()
    {
        unload( nullptr );
    }

    auto FSMManager::createFSM() -> SmartPtr<IFSM>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );
        FB_ASSERT( isLoaded() );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto fsm = factoryManager->make_ptr<FSM>();
        FB_ASSERT( fsm );

        fsm->setFsmManager( this );

        auto handle = fsm->getHandle();
        FB_ASSERT( handle );

        auto id = createNewId();
        while( id >= getSize() )
        {
            auto growSize = getGrowSize();
            auto size = getSize() + growSize;
            resize( size );
        }

        FB_ASSERT( id < m_fsms.size() );

        handle->setInstanceId( id );

        m_fsms[id] = fsm;

        fsm->load( nullptr );

        return fsm;
    }

    void FSMManager::destroyFSM( SmartPtr<IFSM> fsm )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( fsm );
        FB_ASSERT( isValid() );
        FB_ASSERT( isLoaded() );

        fsm->unload( nullptr );

        fsm->setFsmManager( nullptr );

        auto it = std::find( m_fsms.begin(), m_fsms.end(), fsm );
        if( it != m_fsms.end() )
        {
            auto handle = fsm->getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            m_fsms[id] = nullptr;
        }
    }

    auto FSMManager::createNewId() -> u32
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( isLoaded() );

        m_idCount = m_idExt++;
        return m_idCount;
    }

    void FSMManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            setLoadingState( LoadingState::Loading );

            auto growSize = getGrowSize();
            auto size = getSize() + growSize;
            resize( size );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FSMManager::resize( size_t size )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto currentSize = getSize();
        if( currentSize != size )
        {
            m_fsms.resize( size );

            auto pNewPreviousStates = fb::make_shared_array<atomic_u8>( size, getPreviousStates(), 0 );
            setPreviousStates( pNewPreviousStates );

            auto pNewCurrentStates = fb::make_shared_array<atomic_u8>( size, getCurrentStates(), 0 );
            setCurrentStates( pNewCurrentStates );

            auto pNewNewStates = fb::make_shared_array<atomic_u8>( size, getNewStates(), 0 );
            setNewStates( pNewNewStates );

            auto pNewListeners = fb::make_shared_array<SharedPtr<Array<SmartPtr<IFSMListener>>>>(
                size, m_listeners, nullptr );
            m_listeners = pNewListeners;

            m_stateChangeTimes.resize( size );
            m_stateTimes.resize( size );
            m_flags.resize( size );
            m_ready.resize( size );

            setSize( size );
        }
    }

    void FSMManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                for( auto fsm : m_fsms )
                {
                    if( fsm )
                    {
                        fsm->unload( nullptr );
                    }
                }

                if( auto p = getListenersPtr() )
                {
                    auto &pListeners = *p;
                    for( auto listeners : pListeners )
                    {
                        if( listeners )
                        {
                            for( auto listener : *listeners )
                            {
                                listener->unload( nullptr );
                            }
                        }
                    }
                }

                m_listeners = nullptr;
                m_fsms.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FSMManager::changeState()
    {
        auto currentStates = getCurrentStates();
        auto newStates = getNewStates();

        auto size = getSize();
        for( size_t i = 0; i < size; ++i )
        {
            if( m_fsms[i] != nullptr )
            {
                const auto &currentState = ( *currentStates )[i];
                const auto &newState = ( *newStates )[i];

                if( currentState != newState )
                {
                    auto pListeners = getListeners( static_cast<u32>( i ) );
                    if( pListeners )
                    {
                        const auto &fsmListeners = *pListeners;
                        if( !fsmListeners.empty() )
                        {
                            for( auto listener : fsmListeners )
                            {
                                if( listener )
                                {
                                    auto leaveReturn =
                                        listener->handleEvent( currentState, IFSM::Event::Leave );
                                    if( leaveReturn != IFSM::ReturnType::Ok )
                                    {
                                        continue;
                                    }

                                    auto enterReturn =
                                        listener->handleEvent( newState, IFSM::Event::Enter );
                                    if( enterReturn != IFSM::ReturnType::Ok )
                                    {
                                        continue;
                                    }

                                    setPreviousState( static_cast<u32>( i ), (s32)currentState );
                                    setCurrentState( static_cast<u32>( i ), (s32)newState );

                                    const auto result =
                                        listener->handleEvent( newState, IFSM::Event::Complete );
                                    if( result != IFSM::ReturnType::Ok )
                                    {
                                        auto stateStr = StringUtil::toString( newState );
                                        FB_LOG( "State change complete not ok: " + stateStr );
                                    }
                                }
                            }
                        }
                        else
                        {
                            setCurrentState( static_cast<u32>( i ), (s32)newState );
                        }
                    }
                    else
                    {
                        setCurrentState( static_cast<u32>( i ), (s32)newState );
                    }

                    auto applicationManager = core::ApplicationManager::instance();
                    auto timer = applicationManager->getTimer();
                    auto t = timer->getTime();
                    setStateChangeTime( static_cast<u32>( i ), t );
                }
            }
        }
    }

    void FSMManager::changeState( u32 i )
    {
        if( m_fsms[i] != nullptr )
        {
            auto currentStates = getCurrentStates();
            auto newStates = getNewStates();

            auto currentState = ( *currentStates )[i];
            auto newState = ( *newStates )[i];

            if( currentState != newState )
            {
                auto pListeners = getListeners( i );
                if( pListeners )
                {
                    const auto &fsmListeners = *pListeners;
                    if( !fsmListeners.empty() )
                    {
                        for( auto listener : fsmListeners )
                        {
                            if( listener )
                            {
                                listener->handleEvent( currentState, IFSM::Event::Leave );
                            }

                            setCurrentState( i, newState );

                            if( listener )
                            {
                                listener->handleEvent( newState, IFSM::Event::Enter );
                            }

                            if( listener )
                            {
                                const auto result =
                                    listener->handleEvent( newState, IFSM::Event::Complete );
                                if( result != IFSM::ReturnType::Ok )
                                {
                                    auto stateStr = StringUtil::toString( newState );
                                    FB_LOG( "State change complete not ok: " + stateStr );
                                }
                            }
                        }
                    }
                    else
                    {
                        setCurrentState( i, newState );
                    }
                }
                else
                {
                    setCurrentState( i, newState );
                }

                auto applicationManager = core::ApplicationManager::instance();
                auto timer = applicationManager->getTimer();
                auto t = timer->getTime();
                setStateChangeTime( i, t );
            }
        }
    }

    auto FSMManager::getListenersPtr() const
        -> SharedPtr<Array<SharedPtr<Array<SmartPtr<IFSMListener>>>>>
    {
        return m_listeners;
    }

    void FSMManager::setListenersPtr(
        SharedPtr<Array<SharedPtr<Array<SmartPtr<IFSMListener>>>>> listeners )
    {
        m_listeners = listeners;
    }

    void FSMManager::update()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isLoaded() );

        auto size = getSize();

        auto &applicationManager = core::ApplicationManager::instance();
        auto &timer = applicationManager->getTimer();
        if( timer )
        {
            auto dt = timer->getDeltaTime();

            for( size_t i = 0; i < size; ++i )
            {
                if( m_fsms[i] != nullptr )
                {
                    addStateTime( static_cast<u32>( i ), dt );
                }
            }
        }

        changeState();
    }

    auto FSMManager::getStateChangeTime( u32 id ) const -> f64
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_stateChangeTimes[id];
    }

    void FSMManager::setStateChangeTime( u32 id, const f64 &stateChangeTime )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_stateChangeTimes[id] = stateChangeTime;
    }

    auto FSMManager::getPreviousState( u32 id ) const -> u8
    {
        if( auto p = getPreviousStates() )
        {
            auto &previousStates = *p;
            if( id < previousStates.size() )
            {
                return previousStates[id];
            }
        }

        return 0;
    }

    void FSMManager::setPreviousState( u32 id, s32 state )
    {
        if( auto p = getPreviousStates() )
        {
            auto &previousStates = *p;
            if( id < previousStates.size() )
            {
                previousStates[id] = state;
            }
        }
    }

    auto FSMManager::getCurrentState( u32 id ) const -> u8
    {
        if( auto p = getCurrentStates() )
        {
            auto &currentStates = *p;
            if( id < currentStates.size() )
            {
                return currentStates[id];
            }
        }

        return 0;
    }

    void FSMManager::setCurrentState( u32 id, s32 state )
    {
        if( auto p = getCurrentStates() )
        {
            auto &currentStates = *p;
            if( id < currentStates.size() )
            {
                currentStates[id] = state;
            }
        }
    }

    auto FSMManager::getNewState( u32 id ) const -> u8
    {
        if( auto p = getNewStates() )
        {
            auto &newStates = *p;
            if( id < newStates.size() )
            {
                return newStates[id];
            }
        }

        return 0;
    }

    void FSMManager::setNewState( u32 id, s32 state, bool changeNow )
    {
        if( auto p = getNewStates() )
        {
            auto &newStates = *p;
            if( id < newStates.size() )
            {
                newStates[id] = state;
            }
        }

        if( changeNow )
        {
            changeState( id );
        }
    }

    void FSMManager::stateOverride( u32 id, s32 state )
    {
    }

    auto FSMManager::isPending( u32 id ) const -> bool
    {
        return false;
    }

    auto FSMManager::isStateChangeComplete( u32 id ) const -> bool
    {
        return false;
    }

    void FSMManager::setStateChangeComplete( u32 id, bool val )
    {
    }

    void FSMManager::addListener( u32 id, SmartPtr<IFSMListener> listener )
    {
        auto p = getListeners( id );
        if( !p )
        {
            p = fb::make_shared<Array<SmartPtr<IFSMListener>>>();
            setListeners( id, p );
        }

        if( p )
        {
            auto &listeners = *p;
            listeners.reserve( 1 );
            listeners.push_back( listener );
        }
    }

    void FSMManager::removeListener( u32 id, SmartPtr<IFSMListener> listener )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto pListeners = getListeners( id );
        if( pListeners )
        {
            auto &listeners = *pListeners;
            auto it = std::find( listeners.begin(), listeners.end(), listener );
            if( it != listeners.end() )
            {
                auto listener = *it;
                if( listener )
                {
                    listener->unload( nullptr );
                }

                listeners.erase( it );
            }
        }
    }

    void FSMManager::removeListeners( u32 id )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto pListeners = getListeners( id ) )
        {
            auto &listeners = *pListeners;
            for( auto listener : listeners )
            {
                listener->unload( nullptr );
            }

            listeners.clear();
        }
    }

    auto FSMManager::getAutoChangeState( u32 id ) const -> bool
    {
        return false;
    }

    void FSMManager::setAutoChangeState( u32 id, bool val )
    {
    }

    auto FSMManager::getAllowStateChange( u32 id ) const -> bool
    {
        return false;
    }

    void FSMManager::setAllowStateChange( u32 id, bool val )
    {
    }

    auto FSMManager::isReady( u32 id ) const -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        FB_ASSERT( id < m_ready.size() );
        return m_ready[id];
    }

    void FSMManager::setReady( u32 id, bool ready )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        FB_ASSERT( id < m_ready.size() );
        m_ready[id] = ready;
    }

    auto FSMManager::getAutoTriggerEnterStateComplete( u32 id ) const -> bool
    {
        return false;
    }

    void FSMManager::setAutoTriggerEnterStateComplete( u32 id, bool val )
    {
    }

    auto FSMManager::getStateTicks( u32 id, int task ) const -> s32
    {
        return 0;
    }

    auto FSMManager::getStateTicks( u32 id ) const -> s32
    {
        return 0;
    }

    void FSMManager::setStateTicks( u32 id, s32 val )
    {
    }

    auto FSMManager::getListenerPriority( u32 id ) -> u32
    {
        return 0;
    }

    void FSMManager::setListenerPriority( u32 id, u32 priority )
    {
    }

    auto FSMManager::getFlagsPtr( u32 id ) const -> u32 *
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return const_cast<u32 *>( &m_flags[id] );
    }

    auto FSMManager::getFlags( u32 id ) -> u32
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_flags[id];
    }

    void FSMManager::setFlags( u32 id, u32 flags )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_flags[id] = flags;
    }

    void FSMManager::setListeners( u32 id, SharedPtr<Array<SmartPtr<IFSMListener>>> listeners )
    {
        auto p = getListenersPtr();
        if( !p )
        {
            p = fb::make_shared<Array<SharedPtr<Array<SmartPtr<IFSMListener>>>>>();
            setListenersPtr( p );
        }

        if( p )
        {
            auto &listenerArray = *p;
            if( id < listenerArray.size() )
            {
                listenerArray[id] = listeners;
            }
        }
    }

    auto FSMManager::getListeners( u32 id ) const -> SharedPtr<Array<SmartPtr<IFSMListener>>>
    {
        if( auto p = getListenersPtr() )
        {
            auto &listeners = *p;
            if( id < listeners.size() )
            {
                return listeners[id];
            }
        }

        return nullptr;
    }

    auto FSMManager::getStateTime( u32 id ) const -> time_interval
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        FB_ASSERT( id < m_stateTimes.size() );
        return m_stateTimes[id];
    }

    void FSMManager::setStateTime( u32 id, time_interval stateTime )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        FB_ASSERT( id < m_stateTimes.size() );
        FB_ASSERT( Math<time_interval>::isFinite( stateTime ) );
        m_stateTimes[id] = stateTime;
    }

    void FSMManager::addStateTime( u32 id, time_interval stateTime )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( id < m_stateTimes.size() )
        {
            m_stateTimes[id] += stateTime;
        }
    }

    auto FSMManager::isValid() const -> bool
    {
        //RecursiveMutex::ScopedLock lock( m_mutex );
        //if( getLoadingState() == LoadingState::Loaded )
        //{
        //    auto size = getSize();

        //    const auto statesAllocated = m_previousStates.size() == size &&
        //                                 m_currentStates.size() == size && m_newStates.size() == size;
        //    const auto timesAllocated = m_stateChangeTimes.size() == size && m_stateTimes.size() == size;
        //    const auto fsmsAllocated =
        //        m_ready.size() == size && m_listeners.size() == size && m_fsms.size() == size;

        //    return statesAllocated && timesAllocated && fsmsAllocated;
        //}

        return true;
    }

    auto FSMManager::getSize() const -> size_t
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_size;
    }

    void FSMManager::setSize( size_t size )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_size = size;
    }

    auto FSMManager::getGrowSize() const -> size_t
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_growSize;
    }

    void FSMManager::setGrowSize( size_t growSize )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_growSize = growSize;
    }

    auto FSMManager::getPreviousStates() const -> SharedPtr<Array<atomic_u8>>
    {
        return m_previousStates;
    }

    void FSMManager::setPreviousStates( SharedPtr<Array<atomic_u8>> previousStates )
    {
        m_previousStates = previousStates;
    }

    auto FSMManager::getCurrentStates() const -> SharedPtr<Array<atomic_u8>>
    {
        return m_currentStates;
    }

    void FSMManager::setCurrentStates( SharedPtr<Array<atomic_u8>> currentStates )
    {
        m_currentStates = currentStates;
    }

    auto FSMManager::getNewStates() const -> SharedPtr<Array<atomic_u8>>
    {
        return m_newStates;
    }

    void FSMManager::setNewStates( SharedPtr<Array<atomic_u8>> newStates )
    {
        m_newStates = newStates;
    }
}  // end namespace fb
