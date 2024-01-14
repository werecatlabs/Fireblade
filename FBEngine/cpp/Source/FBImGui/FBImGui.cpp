#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/FBImGui.h>
#include <FBImGui/ImGuiManager.h>
#include <FBImGui/ImGuiTreeNode.h>
#include <FBImGui/ImGuiTreeCtrl.h>
#include <FBImGui/ImGuiText.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{
    void FBImGui::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<ImGuiTreeNode>();
        FactoryUtil::addFactory<ImGuiTreeCtrl>();
        FactoryUtil::addFactory<ImGuiText>();

        factoryManager->setPoolSizeByType<ImGuiTreeNode>( 1024 );
        factoryManager->setPoolSizeByType<ImGuiTreeCtrl>( 12 );
        factoryManager->setPoolSizeByType<ImGuiText>( 1024 );
    }

    void FBImGui::unload( SmartPtr<ISharedObject> data )
    {
    }

    auto FBImGui::createUI() -> SmartPtr<IUIManager>
    {
        return fb::make_ptr<ImGuiManager>();
    }
}  // namespace fb::ui
