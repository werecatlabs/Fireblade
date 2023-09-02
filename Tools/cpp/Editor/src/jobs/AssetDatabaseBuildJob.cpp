#include <GameEditorPCH.h>
#include "jobs/AssetDatabaseBuildJob.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        AssetDatabaseBuildJob::AssetDatabaseBuildJob()
        {
        }

        AssetDatabaseBuildJob::~AssetDatabaseBuildJob()
        {
        }

        void AssetDatabaseBuildJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            resourceDatabase->build();
        }

    }  // namespace editor
}  // namespace fb
