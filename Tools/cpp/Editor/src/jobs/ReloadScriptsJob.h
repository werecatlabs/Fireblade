#ifndef ReloadScriptsJob_h__
#define ReloadScriptsJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class ReloadScriptsJob : public Job
        {
        public:
            ReloadScriptsJob();
            virtual ~ReloadScriptsJob();

            virtual void execute();
        };

    }  // namespace editor
}  // namespace fb

#endif  // ReloadScriptsJob_h__
