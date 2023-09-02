#ifndef LeavePlaymodeJob_h__
#define LeavePlaymodeJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class LeavePlaymodeJob : public Job
        {
        public:
            LeavePlaymodeJob();
            ~LeavePlaymodeJob() override;

            void execute() override;
        };
    }  // namespace editor
}  // namespace fb

#endif  // LeavePlaymodeJob_h__
