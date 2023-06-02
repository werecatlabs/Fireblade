#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/FactoryUtil.h>
#include <FBCore/System/Job.h>
#include <FBCore/System/JobQueue.h>
#include <FBCore/System/ProcessManager.h>
#include <FBCore/System/StateContext.h>
#include <FBCore/System/StateContextStandard.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/System/StateContextStandard.h>
#include <FBCore/System/StateQueueStandard.h>

namespace fb
{

    void FactoryUtil::createFactories()
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<StateQueueStandard>();
        FactoryUtil::addFactory<Job>();
        FactoryUtil::addFactory<JobQueue>();
        FactoryUtil::addFactory<ProcessManager>();
        FactoryUtil::addFactory<StateContext>();
        FactoryUtil::addFactory<StateContextStandard>();
        FactoryUtil::addFactory<StateQueueStandard>();
    }

}  // namespace fb
