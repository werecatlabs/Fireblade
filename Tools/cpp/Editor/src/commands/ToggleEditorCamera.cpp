#include <GameEditorPCH.h>
#include "ToggleEditorCamera.h"
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

namespace fb
{
    namespace editor
    {

        ToggleEditorCamera::ToggleEditorCamera()
        {
        }

        ToggleEditorCamera::~ToggleEditorCamera()
        {
        }

        void ToggleEditorCamera::undo()
        {
        }

        void ToggleEditorCamera::redo()
        {
        }

        void ToggleEditorCamera::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            applicationManager->setEditorCamera( !applicationManager->isEditorCamera() );

            auto cameraManager = applicationManager->getCameraManager();

            cameraManager->reset();
        }

    }  // namespace editor
}  // namespace fb
