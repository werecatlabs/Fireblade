#ifndef Scheduler_h__
#define Scheduler_h__

#include <FBCore/FBCorePrerequisites.h>

#include <FBCore/Core/Array.h>
#include <FBCore/Interface/System/IScheduler.h>

namespace fb
{
    class Scheduler : public IScheduler
    {
    public:
        Scheduler();
        ~Scheduler();

        void update();
    };

}  // namespace fb

#endif  // Scheduler_h__
