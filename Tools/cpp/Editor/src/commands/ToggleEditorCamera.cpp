#include <GameEditorPCH.h>
#include "ToggleEditorCamera.h"
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    ToggleEditorCamera::ToggleEditorCamera() = default;

    ToggleEditorCamera::~ToggleEditorCamera() = default;

    void ToggleEditorCamera::undo()
    {
    }

    void ToggleEditorCamera::redo()
    {
    }

    void ToggleEditorCamera::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto jobQueue = applicationManager->getJobQueue();
        auto taskManager = applicationManager->getTaskManager();
        auto applicationTask = taskManager->getTask( Thread::Task::Application );

        auto editorCamera = getToggleValue();
        applicationManager->setEditorCamera( editorCamera );

        auto job = fb::make_ptr<CameraManagerReset>();
        applicationTask->addJob( job );
    }

    auto ToggleEditorCamera::getToggleValue() const -> bool
    {
        return m_toggleValue;
    }

    void ToggleEditorCamera::setToggleValue( bool toggleValue )
    {
        m_toggleValue = toggleValue;
    }

}  // namespace fb::editor
