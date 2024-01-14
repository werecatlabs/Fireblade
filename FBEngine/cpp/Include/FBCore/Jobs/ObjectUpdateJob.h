#ifndef ObjectUpdateJob_h__
#define ObjectUpdateJob_h__

#include <FBCore/System/Job.h>

namespace fb
{

    /** A job that is used to update objects. */
    class ObjectUpdateJob : public Job
    {
    public:
        ObjectUpdateJob();
        ~ObjectUpdateJob() override;

        /** @copydoc Job::execute */
        void execute() override;

        SmartPtr<ISharedObject> getOwner() const;
        void setOwner( SmartPtr<ISharedObject> owner );

    protected:
        SmartPtr<ISharedObject> m_owner;
    };

}  // namespace fb

#endif  // ObjectUpdateJob_h__
