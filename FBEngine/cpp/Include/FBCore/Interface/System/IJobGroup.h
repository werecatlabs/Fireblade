#ifndef IJobGroup_h__
#define IJobGroup_h__

#include <FBCore/Interface/System/IJob.h>

namespace fb
{
    
    /** An Interface for a job group class. Intended to control the execution of group of jobs. */
    class IJobGroup : public IJob
    {
    public:
        /** Virtual destructor.
         */
        ~IJobGroup() override = default;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IJobGroup_h__
