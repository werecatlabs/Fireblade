#ifndef ImportUnityYaml_h__
#define ImportUnityYaml_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class ImportUnityYaml : public Job
        {
        public:
            ImportUnityYaml();
            virtual ~ImportUnityYaml();

            virtual void execute();
        };

    }  // namespace editor
}  // namespace fb

#endif  // ImportUnityYaml_h__
