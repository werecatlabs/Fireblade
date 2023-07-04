#ifndef ProjectCleanJob_h__
#define ProjectCleanJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class ProjectCleanJob : public Job
        {
        public:
            ProjectCleanJob();
            ~ProjectCleanJob() override;

            void execute() override;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // ProjectCleanJob_h__
