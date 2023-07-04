#ifndef JobSaveTree_h__
#define JobSaveTree_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class JobSaveTree : public Job
        {
        public:
            JobSaveTree();
            ~JobSaveTree();

            void execute();

        protected:
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // JobSaveTree_h__
