#ifndef CopyEngineFilesJob_h__
#define CopyEngineFilesJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class CopyEngineFilesJob : public Job
        {
        public:
            CopyEngineFilesJob();
            ~CopyEngineFilesJob() override;

            void execute() override;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // CreateCodeProjectJob_h__
