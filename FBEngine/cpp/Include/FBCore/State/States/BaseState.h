#ifndef BaseState_h__
#define BaseState_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Atomics/AtomicFloat.h>

namespace fb
{
    class BaseState : public IState
    {
    public:
        BaseState();
        ~BaseState() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        /** The time of this state. */
        time_interval getTime() const override;

        /** The time of this state. */
        void setTime( time_interval time ) override;

        /** To know if the state object needs updating. */
        bool isDirty() const override;

        /** Lets the state object know it needs updating. */
        void setDirty( bool dirty ) override;

        bool isRegistered() const;

        void setRegistered( bool registered );

        SmartPtr<IStateContext> &getStateContext() override;
        const SmartPtr<IStateContext> &getStateContext() const override;
        void setStateContext( SmartPtr<IStateContext> stateContext ) override;

        Thread::Task getTaskId() const override;
        void setTaskId( Thread::Task task ) override;

        /** Gets the data as a properties object.
        @return The data as a properties object.
        */
        SmartPtr<Properties> getProperties() const override;

        /** Sets the data as a properties object.
        @param properties The properties object.
        */
        void setProperties( SmartPtr<Properties> properties ) override;

        SmartPtr<ISharedObject> getOwner() const;
        void setOwner( SmartPtr<ISharedObject> owner );

        SmartPtr<IState> clone() const override;

        void assign( SmartPtr<IState> state ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        void makeClone( SmartPtr<BaseState> state ) const;

        mutable SpinRWMutex m_mutex;
        SmartPtr<ISharedObject> m_owner;
        SmartPtr<IStateContext> m_stateContext;
        Atomic<Thread::Task> m_taskId = Thread::Task::Primary;
        atomic_f64 m_time = 0.0;
        atomic_s32 m_dirty = 0;
        atomic_bool m_isRegistered = false;
    };
}  // end namespace fb

#endif  // BaseState_h__
