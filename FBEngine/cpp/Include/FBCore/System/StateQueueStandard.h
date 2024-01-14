#ifndef StateQueueStandard_h__
#define StateQueueStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateQueue.h>
#include <deque>

namespace fb
{

    /** Standard implementation of IStateQueue. */
    class StateQueueStandard : public IStateQueue
    {
    public:
        /** Constructor */
        StateQueueStandard();

        /** Destructor */
        StateQueueStandard( const StateQueueStandard &other );

        /** Destructor */
        ~StateQueueStandard() override;

        /** @copydoc IStateQueue::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IStateQueue::update */
        void update() override;

        /** @copydoc IStateQueue::queueMessage */
        void queueMessage( const SmartPtr<IStateMessage> &message ) override;

        /** @copydoc IStateQueue::queueState */
        void clear() override;

        /** @copydoc IStateQueue::getTaskId */
        u32 getTaskId() const override;

        /** @copydoc IStateQueue::setTaskId */
        void setTaskId( u32 taskId ) override;

        /** @copydoc IStateQueue::isEmpty */
        bool isEmpty() const override;

        /** @copydoc IStateQueue::addStateUpdate */
        void addStateUpdate( SmartPtr<IState> state );

        /** @copydoc IStateQueue::getOwner */
        SmartPtr<IStateContext> getOwner() const;

        /** @copydoc IStateQueue::setOwner */
        void setOwner( SmartPtr<IStateContext> owner );

        /** @copydoc IStateQueue::getMessages */
        SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessages() const override;

        /** @copydoc IStateQueue::getMessagesAndClear */
        SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessagesAndClear() override;

        FB_CLASS_REGISTER_DECL;

    protected:
        /** Sets the message queue. */
        void setMessageQueue( SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> messageQueue );

        /** Gets the message queue. */
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

#endif  // StateQueueStandard_h__
