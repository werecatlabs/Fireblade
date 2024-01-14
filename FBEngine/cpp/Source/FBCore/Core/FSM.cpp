#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/FSM.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FSM, IFSM );

    FSM::FSM() = default;

    FSM::~FSM()
    {
        unload( nullptr );
    }

    void FSM::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
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

    auto FSM::getFsmManager() const -> SmartPtr<IFSMManager>
    {
        const auto fsmManager = m_fsmManager.load();
        return fsmManager.lock();
    }

    void FSM::setFsmManager( SmartPtr<IFSMManager> fsmManager )
    {
        m_fsmManager = fsmManager;
    }

    auto FSM::getStateTime() const -> time_interval
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

    auto FSM::getStateTimeElapsed() const -> time_interval
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto timer = applicationManager->getTimer();

        return timer->getTime() - getStateTime();
    }

    auto FSM::getPreviousState() const -> u8
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();

        return fsmManager->getPreviousState( id );
    }

    auto FSM::getCurrentState() const -> u8
    {
        const auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        const auto handle = getHandle();
        FB_ASSERT( handle );

        const auto id = handle->getInstanceId();
        return fsmManager->getCurrentState( id );
    }

    auto FSM::getNewState() const -> u8
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

    auto FSM::isPending() const -> bool
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

    auto FSM::isStateChangeComplete() const -> bool
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

    auto FSM::getAutoChangeState() const -> bool
    {
        return BitUtil::getFlagValue( *m_flags, IFSM::autoChangeStateFlag );
    }

    void FSM::setAutoChangeState( bool autoChangeState )
    {
        BitUtil::setFlagValue( *m_flags, IFSM::autoChangeStateFlag, autoChangeState );
    }

    auto FSM::getAllowStateChange() const -> bool
    {
        return BitUtil::getFlagValue( *m_flags, IFSM::allowStateChangeFlag );
    }

    void FSM::setAllowStateChange( bool allowStateChange )
    {
        BitUtil::setFlagValue( *m_flags, IFSM::allowStateChangeFlag, allowStateChange );
    }

    auto FSM::getAutoTriggerEnterStateComplete() const -> bool
    {
        return false;
    }

    void FSM::setAutoTriggerEnterStateComplete( bool autoTriggerEnterStateComplete )
    {
    }

    auto FSM::getStateTicks( Thread::Task task ) const -> s32
    {
        return m_stateTicks;
    }

    auto FSM::getStateTicks() const -> s32
    {
        return m_stateTicks;
    }

    void FSM::setStateTicks( s32 val )
    {
        m_stateTicks = val;
    }

    auto FSM::getListeners() const -> SharedPtr<Array<SmartPtr<IFSMListener>>>
    {
        auto fsmManager = getFsmManager();
        FB_ASSERT( fsmManager );

        auto handle = getHandle();
        FB_ASSERT( handle );

        auto id = handle->getInstanceId();
        return fsmManager->getListeners( id );
    }

}  // end namespace fb
