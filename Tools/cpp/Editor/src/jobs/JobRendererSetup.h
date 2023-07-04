#ifndef JobRendererSetup_h__
#define JobRendererSetup_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class JobRendererSetup : public Job
        {
        public:
            JobRendererSetup();
            ~JobRendererSetup();

            void execute();

        protected:
            void chooseSceneManager();
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // JobRendererSetup_h__
