#ifndef __FSM_h__
#define __FSM_h__

#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    /**
     * Data oriented implementation.
     */
    class FSM : public CSharedObject<IFSM>
    {
    public:
        FSM();
        FSM( const FSM &other ) = delete;
        ~FSM() override;

        /** @copydoc IObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /**
         * @copydoc IFSM::getFsmManager
         */
        SmartPtr<IFSMManager> getFsmManager() const override;

        /**
         * @copydoc IFSM::setFsmManager
         */
        void setFsmManager( SmartPtr<IFSMManager> fsmManager ) override;

        /**
         * @copydoc IFSM::getStateTime
         */
        time_interval getStateTime() const override;

        /**
         * @copydoc IFSM::setStateTime
         */
        void setStateTime( time_interval stateTime ) override;

        /**
         * @copydoc IFSM::getStateTimeElapsed
         */
        time_interval getStateTimeElapsed() const override;

        /**
         * @copydoc IFSM::getPreviousState
         */
        u8 getPreviousState() const override;

        /**
         * @copydoc IFSM::getCurrentState
         */
        u8 getCurrentState() const override;

        /**
         * @copydoc IFSM::getNewState
         */
        u8 getNewState() const override;

        /**
         * @copydoc IFSM::setNewState
         */
        void setNewState( s32 val, bool changeNow = false ) override;

        /**
         * @copydoc IFSM::stateOverride
         */
        void stateOverride( s32 state );

        /**
         * @copydoc IFSM::isPending
         */
        bool isPending() const override;

        /**
         * @copydoc IFSM::isStateChangeComplete
         */
        bool isStateChangeComplete() const;

        /**
         * @copydoc IFSM::setStateChangeComplete
         */
        void setStateChangeComplete( bool stateChangeComplete );

        /**
         * @copydoc IFSM::addListener
         */
        void addListener( SmartPtr<IFSMListener> listener ) override;

        /**
         * @copydoc IFSM::removeListener
         */
        void removeListener( SmartPtr<IFSMListener> listener ) override;

        /**
         * @copydoc IFSM::getAutoChangeState
         */
        bool getAutoChangeState() const;

        /**
         * @copydoc IFSM::setAutoChangeState
         */
        void setAutoChangeState( bool val );

        /**
         * @copydoc IFSM::getAllowStateChange
         */
        bool getAllowStateChange() const;

        /**
         * @copydoc IFSM::setAllowStateChange
         */
        void setAllowStateChange( bool allowStateChange );

        /**
         * @copydoc IFSM::isReady
         */
        bool isReady() const;

        /**
         * @copydoc IFSM::setReady
         */
        void setReady( bool ready );

        /**
         * @copydoc IFSM::getAutoTriggerEnterStateComplete
         */
        bool getAutoTriggerEnterStateComplete() const;

        /** @copydoc IFSM::setAutoTriggerEnterStateComplete */
        void setAutoTriggerEnterStateComplete( bool autoTriggerEnterStateComplete );

        /** @copydoc IFSM::getStateTicks */
        s32 getStateTicks( Thread::Task task ) const override;

        /** @copydoc IFSM::getStateTicks */
        s32 getStateTicks() const override;

        /** @copydoc IFSM::setStateTicks */
        void setStateTicks( s32 val );

        /** @copydoc IFSM::getListeners */
        SharedPtr<Array<SmartPtr<IFSMListener>>> getListeners() const;

        FB_CLASS_REGISTER_DECL;

    protected:
        void triggerStateChangeComplete();

        AtomicWeakPtr<IFSMManager> m_fsmManager;

        atomic_u32 m_threadId = 0;
        atomic_u32 m_stateTicks = 0;

        u32 *m_flags = nullptr;
    };
}  // end namespace fb

#endif  // __FSM_h__
