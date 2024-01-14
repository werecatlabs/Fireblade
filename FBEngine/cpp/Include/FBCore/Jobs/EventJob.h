#ifndef EventJob_h__
#define EventJob_h__

#include <FBCore/System/Job.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Core/Parameter.h>

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

        /** @copydoc Job::execute */
        void execute() override;

        SmartPtr<ISharedObject> getObject() const;

        void setObject( SmartPtr<ISharedObject> object );

        SmartPtr<ISharedObject> getSender() const;

        void setSender( SmartPtr<ISharedObject> sender );

        IStateContext *getOwner() const;

        void setOwner( IStateContext *owner );

        IEvent::Type getEventType() const;

        void setEventType( IEvent::Type eventType );

        hash_type getEventValue() const;

        void setEventValue( hash_type eventValue );

        Array<Parameter> getArguments() const;

        void setArguments( const Array<Parameter> &arguments );

        SmartPtr<IEvent> getEvent() const;

        void setEvent( SmartPtr<IEvent> event );

    private:
        AtomicSmartPtr<ISharedObject> m_object;
        AtomicSmartPtr<ISharedObject> m_sender;
        IStateContext *m_owner = nullptr;
        IEvent::Type m_eventType = IEvent::Type::Object;
        hash_type m_eventValue = 0;
        Array<Parameter> m_arguments;
        SmartPtr<IEvent> m_event;
    };

}  // namespace fb

#endif  // EventJob_h__
