#ifndef JobRestoreTree_h__
#define JobRestoreTree_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class JobRestoreTree : public Job
        {
        public:
            JobRestoreTree();
            ~JobRestoreTree();

            void execute();

        protected:
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // JobRestoreTree_h__
