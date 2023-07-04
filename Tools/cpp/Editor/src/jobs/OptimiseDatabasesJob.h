#ifndef OptimiseDatabasesJob_h__
#define OptimiseDatabasesJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class OptimiseDatabasesJob : public Job
        {
        public:
            OptimiseDatabasesJob();
            ~OptimiseDatabasesJob() override;

            void execute() override;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // OptimiseDatabasesJob_h__
