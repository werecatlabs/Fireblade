#ifndef EventJob_h__
#define EventJob_h__


#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/System/Job.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{
    /** A job that is used to dispatch events. */
	class EventJob : public Job
    {
    public:
        /** Constructor */
        EventJob() = default;

        /** Destructor */
        ~EventJob() override = default;

        /** @copydoc IJob::execute */
        void execute() override;

        SmartPtr<ISharedObject> getObject() const;

        void setObject( SmartPtr<ISharedObject> object );

        SmartPtr<ISharedObject> getSender() const;

        void setSender( SmartPtr<ISharedObject> sender );

        IStateContext *owner = nullptr;
        IEvent::Type eventType = IEvent::Type::Object;
        hash_type eventValue = 0;
        Array<Parameter> arguments;

        SmartPtr<IEvent> event;

    private:
        AtomicSmartPtr<ISharedObject> m_object;
        AtomicSmartPtr<ISharedObject> m_sender;
    };

}  // namespace fb

#endif  // EventJob_h__
