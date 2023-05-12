#include <FBCore/FBCorePCH.h>
#include <FBCore/FSM/FSMManager.h>
#include <FBCore/FSM/FSM.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FSMManager, IFSMManager );

    constexpr auto size = 32768;
    u32 FSMManager::m_idExt = 0;

    FSMManager::FSMManager()
    {
    }

    FSMManager::~FSMManager()
    {
        unload( nullptr );
    }

    SmartPtr<IFSM> FSMManager::createFSM()
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( isLoaded() );

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto fsm = factoryManager->make_ptr<FSM>();
        FB_ASSERT( fsm );

        fsm->setFsmManager( this );

        auto handle = fsm->getHandle();
        FB_ASSERT( handle );

        auto id = createNewId();
        handle->setInstanceId( id );

        m_fsms[id] = fsm;

        fsm->load( nullptr );

        return fsm;
    }

    void FSMManager::destroyFSM( SmartPtr<IFSM> fsm )
    {
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

    u32 FSMManager::createNewId()
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
            setLoadingState( LoadingState::Loading );

            m_fsms.resize( size );
            m_previousStates.resize( size );
            m_currentStates.resize( size );
            m_newStates.resize( size );
            m_listeners.resize( size );

            m_stateChangeTimes.resize( size );
            m_stateTimes.resize( size );
            m_flags.resize( size );
            m_ready.resize( size );

            for( auto &value : m_previousStates )
            {
                value = 0;
            }

            for( auto &value : m_currentStates )
            {
                value = 0;
            }

            for( auto &value : m_newStates )
            {
                value = 0;
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FSMManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
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

                for( auto pListeners : m_listeners )
                {
                    if( pListeners )
                    {
                        auto &listeners = *pListeners;
                        for( auto listener : listeners )
                        {
                            if( listener )
                            {
                                listener->unload( nullptr );
                            }
                        }
                    }
                }

                m_listeners.clear();
                m_fsms.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FSMManager::changeState( u32 i )
    {
        auto &previousState = m_previousStates[i];
        auto &currentState = m_currentStates[i];
        auto &newState = m_newStates[i];

        if( currentState != newState )
        {
            auto pListeners = getListeners( i );
            if( pListeners )
            {
                const auto &fsmListeners = *pListeners;
                for( auto listener : fsmListeners )
                {
                    if( listener )
                    {
                        listener->handleEvent( currentState, IFSM::Event::Leave );
                    }

                    currentState = newState;

                    if( listener )
                    {
                        listener->handleEvent( currentState, IFSM::Event::Enter );
                    }

                    if( listener )
                    {
                        const auto result = listener->handleEvent( currentState, IFSM::Event::Complete );
                        if( result != IFSM::ReturnType::Ok )
                        {
                            auto stateStr = StringUtil::toString( currentState );
                            FB_LOG( "State change complete not ok: " + stateStr );
                        }
                    }
                }
            }

            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();
            auto t = timer->getTime();
            setStateChangeTime( i, t );
        }
    }

    void FSMManager::update()
    {
        FB_ASSERT( isLoaded() );

        auto applicationManager = core::IApplicationManager::instance();
        auto timer = applicationManager->getTimer();
        if( timer )
        {
            auto dt = timer->getDeltaTime();

            for( size_t i = 0; i < m_idExt; ++i )
            {
                auto &stateTime = m_stateTimes[i];
                stateTime += dt;
            }
        }

        for( size_t i = 0; i < m_idExt; ++i )
        {
            changeState( (u32)i );
        }
    }

    f64 FSMManager::getStateChangeTime( u32 id ) const
    {
        return m_stateChangeTimes[id];
    }

    void FSMManager::setStateChangeTime( u32 id, const f64 &stateChangeTime )
    {
        m_stateChangeTimes[id] = stateChangeTime;
    }

    u8 FSMManager::getPreviousState( u32 id ) const
    {
        FB_ASSERT( id < m_previousStates.size() );
        return m_previousStates[id];
    }

    u8 FSMManager::getCurrentState( u32 id ) const
    {
        FB_ASSERT( id < m_currentStates.size() );
        return m_currentStates[id];
    }

    u8 FSMManager::getNewState( u32 id ) const
    {
        FB_ASSERT( id < m_newStates.size() );
        return m_newStates[id];
    }

    void FSMManager::setNewState( u32 id, s32 state, bool changeNow )
    {
        FB_ASSERT( id < m_newStates.size() );
        m_newStates[id] = state;

        if( changeNow )
        {
            changeState( id );
        }
    }

    void FSMManager::stateOverride( u32 id, s32 state )
    {
    }

    bool FSMManager::isPending( u32 id ) const
    {
        return false;
    }

    bool FSMManager::isStateChangeComplete( u32 id ) const
    {
        return false;
    }

    void FSMManager::setStateChangeComplete( u32 id, bool val )
    {
    }

    void FSMManager::addListener( u32 id, SmartPtr<IFSMListener> listener )
    {
        if( const auto pOldListeners = getListeners( id ) )
        {
            auto &oldListeners = *pOldListeners;

            auto pNewListeners = fb::make_shared<Array<SmartPtr<IFSMListener>>>();
            auto &listeners = *pNewListeners;
            listeners.reserve( oldListeners.size() + 1 );

            listeners = Array<SmartPtr<IFSMListener>>( oldListeners.begin(), oldListeners.end() );
            listeners.push_back( listener );

            setListeners( id, pNewListeners );
        }
        else
        {
            auto pNewListeners = fb::make_shared<Array<SmartPtr<IFSMListener>>>();
            auto &listeners = *pNewListeners;
            listeners.reserve( 1 );
            listeners.push_back( listener );

            setListeners( id, pNewListeners );
        }
    }

    void FSMManager::removeListener( u32 id, SmartPtr<IFSMListener> listener )
    {
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

    bool FSMManager::getAutoChangeState( u32 id ) const
    {
        return false;
    }

    void FSMManager::setAutoChangeState( u32 id, bool val )
    {
    }

    bool FSMManager::getAllowStateChange( u32 id ) const
    {
        return false;
    }

    void FSMManager::setAllowStateChange( u32 id, bool val )
    {
    }

    bool FSMManager::isReady( u32 id ) const
    {
        FB_ASSERT( id < m_ready.size() );
        return m_ready[id];
    }

    void FSMManager::setReady( u32 id, bool ready )
    {
        FB_ASSERT( id < m_ready.size() );
        m_ready[id] = ready;
    }

    bool FSMManager::getAutoTriggerEnterStateComplete( u32 id ) const
    {
        return false;
    }

    void FSMManager::setAutoTriggerEnterStateComplete( u32 id, bool val )
    {
    }

    s32 FSMManager::getStateTicks( u32 id, int task ) const
    {
        return 0;
    }

    s32 FSMManager::getStateTicks( u32 id ) const
    {
        return 0;
    }

    void FSMManager::setStateTicks( u32 id, s32 val )
    {
    }

    u32 FSMManager::getListenerPriority( u32 id )
    {
        return 0;
    }

    void FSMManager::setListenerPriority( u32 id, u32 priority )
    {
    }

    fb::u32 *FSMManager::getFlagsPtr( u32 id ) const
    {
        return (u32 *)&m_flags[id];
    }

    u32 FSMManager::getFlags( u32 id )
    {
        return m_flags[id];
    }

    void FSMManager::setFlags( u32 id, u32 flags )
    {
        m_flags[id] = flags;
    }

    void FSMManager::setListeners( u32 id, SharedPtr<Array<SmartPtr<IFSMListener>>> listeners )
    {
        FB_ASSERT( id < m_listeners.size() );
        m_listeners[id] = listeners;
    }

    SharedPtr<Array<SmartPtr<IFSMListener>>> FSMManager::getListeners( u32 id ) const
    {
        FB_ASSERT( id < m_listeners.size() );
        return m_listeners[id];
    }

    time_interval FSMManager::getStateTime( u32 id ) const
    {
        FB_ASSERT( id < m_stateTimes.size() );
        return m_stateTimes[id];
    }

    void FSMManager::setStateTime( u32 id, time_interval stateTime )
    {
        FB_ASSERT( id < m_stateTimes.size() );
        FB_ASSERT( Math<time_interval>::isFinite( stateTime ) );
        m_stateTimes[id] = stateTime;
    }

    bool FSMManager::isValid() const
    {
        if( getLoadingState() == LoadingState::Loaded )
        {
            const auto statesAllocated = m_previousStates.size() == size &&
                                         m_currentStates.size() == size && m_newStates.size() == size;
            const auto timesAllocated = m_stateChangeTimes.size() == size && m_stateTimes.size() == size;
            const auto fsmsAllocated =
                m_ready.size() == size && m_listeners.size() == size && m_fsms.size() == size;

            return statesAllocated && timesAllocated && fsmsAllocated;
        }

        return true;
    }

}  // end namespace fb
