#include <GameEditorPCH.h>
#include <jobs/OptimiseDatabasesJob.h>
#include <editor/EditorManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    OptimiseDatabasesJob::OptimiseDatabasesJob() = default;

    OptimiseDatabasesJob::~OptimiseDatabasesJob() = default;

    void OptimiseDatabasesJob::execute()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();

            if( auto resourceDatabase = applicationManager->getResourceDatabase() )
            {
                resourceDatabase->optimise();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // namespace fb::editor
