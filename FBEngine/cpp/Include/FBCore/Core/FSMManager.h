#ifndef FSMManager_h__
#define FSMManager_h__

#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** Data oriented implementation of the FSMManager class is responsible for managing FSMs.
    */
    class FSMManager : public SharedObject<IFSMManager>
    {
    public:
        /** Constructor. */
        FSMManager();

        /** Destructor. */
        ~FSMManager() override;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::update */
        void update() override;

        /** @copydoc IFSMManager::createFSM */
        SmartPtr<IFSM> createFSM() override;

        /** @copydoc IFSMManager::destroyFSM */
        void destroyFSM( SmartPtr<IFSM> fsm ) override;

        /** @copydoc IFSMManager::getStateChangeTime */
        f64 getStateChangeTime( u32 id ) const override;

        /** @copydoc IFSMManager::setStateChangeTime */
        void setStateChangeTime( u32 id, const f64 &stateChangeTime ) override;

        /** @copydoc IFSMManager::getPreviousState */
        u8 getPreviousState( u32 id ) const override;

        /** @copydoc IFSMManager::getCurrentState */
        u8 getCurrentState( u32 id ) const override;

        /** @copydoc IFSMManager::getNewState */
        u8 getNewState( u32 id ) const override;

        /** @copydoc IFSMManager::setNewState */
        void setNewState( u32 id, s32 state, bool changeNow = false ) override;

        void stateOverride( u32 id, s32 state );

        bool isPending( u32 id ) const;
        bool isStateChangeComplete( u32 id ) const;

        void setStateChangeComplete( u32 id, bool val );

        void addListener( u32 id, SmartPtr<IFSMListener> val ) override;
        void removeListener( u32 id, SmartPtr<IFSMListener> val ) override;
        void removeListeners( u32 id ) override;

        bool getAutoChangeState( u32 id ) const;
        void setAutoChangeState( u32 id, bool val );

        bool getAllowStateChange( u32 id ) const;
        void setAllowStateChange( u32 id, bool val );

        bool isReady( u32 id ) const;

        void setReady( u32 id, bool ready );

        bool getAutoTriggerEnterStateComplete( u32 id ) const;

        void setAutoTriggerEnterStateComplete( u32 id, bool val );

        s32 getStateTicks( u32 id, int task ) const;

        s32 getStateTicks( u32 id ) const;

        void setStateTicks( u32 id, s32 val );

        u32 getListenerPriority( u32 id ) override;

        void setListenerPriority( u32 id, u32 priority ) override;

        u32 *getFlagsPtr( u32 id ) const;

        u32 getFlags( u32 id );
        void setFlags( u32 id, u32 flags );

        void setListeners( u32 id, SharedPtr<Array<SmartPtr<IFSMListener>>> listeners );
        SharedPtr<Array<SmartPtr<IFSMListener>>> getListeners( u32 id ) const;

        time_interval getStateTime( u32 id ) const;

        void setStateTime( u32 id, time_interval stateTime );

        bool isValid() const;

        size_t getSize() const;

        void setSize( size_t size );

        size_t getGrowSize() const;

        void setGrowSize( size_t growSize );

        FB_CLASS_REGISTER_DECL;

    protected:
        void resize( size_t size );

        u32 createNewId();

        void changeState( u32 id );

        Array<u32> m_flags;

        /// The previous state of the fsm.
        Array<u8> m_previousStates;

        /// The current state of the fsm.
        Array<u8> m_currentStates;

        /// The new state of the fsm.
        Array<u8> m_newStates;

        Array<time_interval> m_stateChangeTimes;
        Array<time_interval> m_stateTimes;
        Array<atomic_bool> m_ready;

        Array<SharedPtr<Array<SmartPtr<IFSMListener>>>> m_listeners;

        Array<SmartPtr<IFSM>> m_fsms;

        u32 m_idCount = 0;

        size_t m_size = 0;
        size_t m_growSize = 128;

        mutable RecursiveMutex m_mutex;

        static u32 m_idExt;
    };
}  // end namespace fb

#endif  // FSMManager_h__
