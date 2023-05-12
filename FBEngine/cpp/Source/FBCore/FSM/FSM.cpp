#include <FBCore/FBCorePCH.h>
#include <FBCore/FSM/FSM.h>
#include <FBCore/Base/BitUtil.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FSM, IFSM );

    FSM::FSM()
    {
    }

    FSM::~FSM()
    {
        unload( nullptr );
    }

    void FSM::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fsmManager = getFsmManager();
            FB_ASSERT( fsmManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            m_flags = fsmManager->getFlagsPtr( id );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void FSM::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto fsmManager = getFsmManager();
                FB_ASSERT( fsmManager );

                const auto handle = getHandle();
                FB_ASSERT( handle );

                const auto id = handle->getInstanceId();

                fsmManager->removeListeners( id );
                setFsmManager( nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SmartPtr<IFSMManager> FSM::getFsmManager() const
    {
        const auto fsmManager = m_fsmManager.load();
        return fsmManager.lock();
    }

    void FSM::setFsmManager( SmartPtr<IFSMManager> fsmManager )
    {
        m_fsmManager = fsmManager;
    }

    time_interval FSM::getStateTime() const
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();

        return fsmManager->getStateTime( id );
    }

    void FSM::setStateTime( time_interval stateTime )
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();

        return fsmManager->setStateTime( id, stateTime );
    }

    time_interval FSM::getStateTimeElapsed() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto timer = applicationManager->getTimer();

        return timer->getTime() - getStateTime();
    }

    u8 FSM::getPreviousState() const
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();

        return fsmManager->getPreviousState( id );
    }

    u8 FSM::getCurrentState() const
    {
        const auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();
        return fsmManager->getCurrentState( id );
    }

    u8 FSM::getNewState() const
    {
        const auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();
        return fsmManager->getNewState( id );
    }

    void FSM::setNewState( s32 val, bool changeNow /*= false*/ )
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();
        fsmManager->setNewState( id, val, changeNow );
    }

    void FSM::stateOverride( s32 state )
    {
    }

    bool FSM::isPending() const
    {
        return BitUtil::getFlagValue( *m_flags, IFSM::isPendingFlag );
    }

    void FSM::triggerStateChangeComplete()
    {
        BitUtil::setFlagValue( *m_flags, IFSM::isPendingFlag, false );
        BitUtil::setFlagValue( *m_flags, IFSM::isStateChangeCompleteFlag, true );

        auto pListeners = getListeners();
        auto &listeners = *pListeners;

        for( auto listener : listeners )
        {
            try
            {
                if( listener )
                {
                    const auto state = getCurrentState();
                    const auto result = listener->handleEvent( state, IFSM::Event::Complete );
                    if( result == IFSM::ReturnType::Ok )
                    {
                        auto stateStr = StringUtil::toString( state );
                        FB_LOG( "State change complete ok: " + stateStr );
                    }
                    else
                    {
                        auto stateStr = StringUtil::toString( state );
                        FB_LOG( "State change complete not ok: " + stateStr );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    }

    bool FSM::isStateChangeComplete() const
    {
        return BitUtil::getFlagValue( *m_flags, IFSM::isStateChangeCompleteFlag );
    }

    void FSM::setStateChangeComplete( bool stateChangeComplete )
    {
        BitUtil::setFlagValue( *m_flags, IFSM::isStateChangeCompleteFlag, stateChangeComplete );
    }

    void FSM::addListener( SmartPtr<IFSMListener> listener )
    {
        if( listener )
        {
            listener->setFSM( this );
        }

        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        auto handle = getHandle();
        FB_ASSERT( handle );

        auto id = handle->getInstanceId();
        FB_ASSERT( id < std::numeric_limits<u32>::max() );

        fsmManager->addListener( id, listener );
    }

    void FSM::removeListener( SmartPtr<IFSMListener> listener )
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        auto handle = getHandle();
        FB_ASSERT( handle );

        if( handle )
        {
            auto id = handle->getInstanceId();
            FB_ASSERT( id < std::numeric_limits<u32>::max() );

            fsmManager->removeListener( id, listener );
        }
    }

    bool FSM::getAutoChangeState() const
    {
        return BitUtil::getFlagValue( *m_flags, IFSM::autoChangeStateFlag );
    }

    void FSM::setAutoChangeState( bool autoChangeState )
    {
        BitUtil::setFlagValue( *m_flags, IFSM::autoChangeStateFlag, autoChangeState );
    }

    bool FSM::getAllowStateChange() const
    {
        return BitUtil::getFlagValue( *m_flags, IFSM::allowStateChangeFlag );
    }

    void FSM::setAllowStateChange( bool allowStateChange )
    {
        BitUtil::setFlagValue( *m_flags, IFSM::allowStateChangeFlag, allowStateChange );
    }

    bool FSM::isReady() const
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        auto handle = getHandle();
        FB_ASSERT( handle );

        auto id = handle->getInstanceId();

        return fsmManager->isReady( id );
    }

    void FSM::setReady( bool ready )
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        auto handle = getHandle();
        FB_ASSERT( handle );

        auto id = handle->getInstanceId();

        return fsmManager->setReady( id, ready );
    }

    bool FSM::getAutoTriggerEnterStateComplete() const
    {
        return false;
    }

    void FSM::setAutoTriggerEnterStateComplete( bool autoTriggerEnterStateComplete )
    {
    }

    s32 FSM::getStateTicks( Thread::Task task ) const
    {
        return m_stateTicks;
    }

    s32 FSM::getStateTicks() const
    {
        return m_stateTicks;
    }

    void FSM::setStateTicks( s32 val )
    {
        m_stateTicks = val;
    }

    SharedPtr<Array<SmartPtr<IFSMListener>>> FSM::getListeners() const
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        auto handle = getHandle();
        FB_ASSERT( handle );

        auto id = handle->getInstanceId();
        return fsmManager->getListeners( id );
    }

}  // end namespace fb
