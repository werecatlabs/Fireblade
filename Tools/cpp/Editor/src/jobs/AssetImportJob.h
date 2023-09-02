#ifndef AssetImportJob_h__
#define AssetImportJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {

        class AssetImportJob : public Job
        {
        public:
            AssetImportJob();
            ~AssetImportJob();

            void execute() override;
        };

    }  // namespace editor
}  // namespace fb



#endif // AssetImportJob_h__
