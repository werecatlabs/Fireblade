#ifndef AssetDatabaseBuildJob_h__
#define AssetDatabaseBuildJob_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class AssetDatabaseBuildJob : public Job
        {
        public:
            AssetDatabaseBuildJob();
            ~AssetDatabaseBuildJob();

            void execute() override;
        };

    }  // namespace editor
}  // namespace fb

#endif  // AssetDatabaseBuildJob_h__
