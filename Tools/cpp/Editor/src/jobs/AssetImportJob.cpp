#include <GameEditorPCH.h>
#include "jobs/AssetImportJob.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    AssetImportJob::AssetImportJob() = default;

    AssetImportJob::~AssetImportJob() = default;

    void AssetImportJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();
        FB_ASSERT( resourceDatabase );

        resourceDatabase->importAssets();
    }

}  // namespace fb::editor
