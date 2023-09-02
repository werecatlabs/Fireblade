#include <GameEditorPCH.h>
#include "jobs/AssetImportJob.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        AssetImportJob::AssetImportJob()
        {
        }

        AssetImportJob::~AssetImportJob()
        {
        }

        void AssetImportJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            resourceDatabase->importAssets();
        }

    }  // namespace editor
}  // namespace fb
