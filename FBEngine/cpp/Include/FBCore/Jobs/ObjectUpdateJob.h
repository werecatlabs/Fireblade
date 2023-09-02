#ifndef ObjectUpdateJob_h__
#define ObjectUpdateJob_h__

#include <FBCore/System/Job.h>

namespace fb
{

    class ObjectUpdateJob : public Job
    {
    public:
        ObjectUpdateJob();
        ~ObjectUpdateJob() override;

        void execute() override;

        SmartPtr<ISharedObject> getOwner() const;
        void setOwner( SmartPtr<ISharedObject> owner );

    protected:
        SmartPtr<ISharedObject> m_owner;
    };

}  // namespace fb

#endif  // ObjectUpdateJob_h__
