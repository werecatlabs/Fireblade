#ifndef BaseStateTest_h__
#define BaseStateTest_h__

#include <FBCore/Atomics/AtomicFloat.h>

#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Properties.h>

namespace fb
{
    template <class T>
    class BaseStateT : public T
    {
    public:
        BaseStateT();
        ~BaseStateT() override;

        /** The time of this state. */
        time_interval getTime() const;

        /** The time of this state. */
        void setTime( time_interval time );

        /** To know if the state object needs updating. */
        bool isDirty() const;

        /** Lets the state object know it needs updating. */
        void setDirty( bool dirty );

        SmartPtr<IStateContext> getStateContext() const;
        void setStateContext( SmartPtr<IStateContext> context );

        Thread::Task getTaskId() const;
        void setTaskId( Thread::Task task );

        /** Gets the data as a properties object.
        @return The data as a properties object.
        */
        SmartPtr<Properties> getProperties() const;

        /** Sets the data as a properties object.
        @param properties The properties object.
        */
        void setProperties( SmartPtr<Properties> properties );

        SmartPtr<ISharedObject> getOwner() const;
        void setOwner( SmartPtr<ISharedObject> owner );

        SmartPtr<IState> clone() const;

        void assign( SmartPtr<IState> state );

        FB_CLASS_REGISTER_TEMPLATE_DECL( BaseStateT, T );

    protected:
        WeakPtr<ISharedObject> m_owner;
        WeakPtr<IStateContext> m_context;
        Thread::Task m_taskId = Thread::Task::Primary;
        atomic_f64 m_time = 0.0;
        atomic_bool m_dirty = false;
    };
}  // end namespace fb

#include <FBCore/State/States/BaseStateT.inl>

#endif  // BaseState_h__
