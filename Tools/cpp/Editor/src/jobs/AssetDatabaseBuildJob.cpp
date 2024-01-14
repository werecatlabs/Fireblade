#include <GameEditorPCH.h>
#include "jobs/AssetDatabaseBuildJob.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    AssetDatabaseBuildJob::AssetDatabaseBuildJob() = default;

    AssetDatabaseBuildJob::~AssetDatabaseBuildJob() = default;

    void AssetDatabaseBuildJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();
        FB_ASSERT( resourceDatabase );

        resourceDatabase->build();
    }

}  // namespace fb::editor
