#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/FBImGui.h>
#include <FBImGui/ImGuiManager.h>
#include <FBImGui/ImGuiTreeNode.h>
#include <FBImGui/ImGuiTreeCtrl.h>
#include <FBImGui/ImGuiText.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        void FBImGui::load( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            FactoryUtil::addFactory<ImGuiTreeNode>();
            FactoryUtil::addFactory<ImGuiTreeCtrl>();
            FactoryUtil::addFactory<ImGuiText>();

            factoryManager->setPoolSizeByType<ImGuiTreeNode>( 128 );
            factoryManager->setPoolSizeByType<ImGuiTreeCtrl>( 12 );
            factoryManager->setPoolSizeByType<ImGuiText>( 128 );
        }

        void FBImGui::unload( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<IUIManager> FBImGui::createUI()
        {
            return fb::make_ptr<ImGuiManager>();
        }
    }  // end namespace ui
}  // end namespace fb
