#ifndef SetupMaterialJob_h__
#define SetupMaterialJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class SetupMaterialJob : public Job
        {
        public:
            SetupMaterialJob();
            ~SetupMaterialJob() override;

            void execute() override;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // SetupMaterialJob_h__
