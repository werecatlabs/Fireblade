#ifndef CreateCodeProjectJob_h__
#define CreateCodeProjectJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class CreateCodeProjectJob : public Job
        {
        public:
            CreateCodeProjectJob();
            ~CreateCodeProjectJob() override;

            void execute() override;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // CreateCodeProjectJob_h__
