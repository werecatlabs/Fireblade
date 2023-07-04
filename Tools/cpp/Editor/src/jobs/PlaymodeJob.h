#ifndef PlaymodeJob_h__
#define PlaymodeJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class PlaymodeJob : public Job
        {
        public:
            PlaymodeJob();
            virtual ~PlaymodeJob();

            virtual void execute();
        };

    }  // namespace editor
}  // namespace fb

#endif  // PlaymodeJob_h__
