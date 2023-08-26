#ifndef StateQueue_h__
#define StateQueue_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateQueue.h>

#include <deque>

namespace fb
{

    class StateQueue : public IStateQueue
    {
    public:
        StateQueue();
        StateQueue( const StateQueue &other );
        ~StateQueue() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void update() override;

        void queueMessage( const SmartPtr<IStateMessage> &message ) override;

        void clear() override;

        u32 getTaskId() const override;

        void setTaskId( u32 taskId ) override;

        bool isEmpty() const override;

        void addStateUpdate( SmartPtr<IState> state );

        SmartPtr<IStateContext> getOwner() const;
        void setOwner( SmartPtr<IStateContext> val );

        SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessages() const override;
        SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessagesAndClear() override;

        FB_CLASS_REGISTER_DECL;

    protected:
        void setMessageQueue( SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> messageQueue );

        SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessageQueue() const;

        /// The message queue.
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> m_messageQueue;

        /// The message queue.
        std::deque<SmartPtr<IState>> m_stateQueue;

        /// The state object that receives messages.
        SmartPtr<IStateContext> m_owner;

        /// The id this queue dispatches to.
        atomic_s32 m_taskId;
    };

}  // end namespace fb

#endif  // StateQueue_h__
