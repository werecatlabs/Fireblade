#ifndef IStateQueue_h__
#define IStateQueue_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{
    /** A class used to queue state messages. */
    class IStateQueue : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IStateQueue() override = default;

        /** Queues a message. */
        virtual void queueMessage( const SmartPtr<IStateMessage> &message ) = 0;

        /** Clears the message queue. */
        virtual void clear() = 0;

        /** Get the task id that processes the stored messages. */
        virtual u32 getTaskId() const = 0;

        /** Set the task id that processes the stored messages. */
        virtual void setTaskId( u32 taskId ) = 0;

        /** Gets boolean indicating whether the queue is empty. */
        virtual bool isEmpty() const = 0;

        /** Gets an array of the messages.
        @ return an array of the messages.
        */
        virtual SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessages() const = 0;

        /** Gets an array of the messages.
        @ return an array of the messages.
        */
        virtual SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> getMessagesAndClear() = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IStateQueue_h__
