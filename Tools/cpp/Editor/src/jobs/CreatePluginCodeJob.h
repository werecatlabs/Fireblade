#ifndef CreatePluginCodeJob_h__
#define CreatePluginCodeJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class CreatePluginCodeJob : public Job
        {
        public:
            CreatePluginCodeJob();
            ~CreatePluginCodeJob() override;

            void execute() override;
        };
    }  // end namespace editor
}  // end namespace fb

#endif // CreatePluginCodeJob_h__
