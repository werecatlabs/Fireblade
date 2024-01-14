#include <GameEditorPCH.h>
#include <commands/FileMenuCmd.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <jobs/OpenSceneJob.h>
#include <jobs/CreateCodeProjectJob.h>
#include "jobs/CompileProjectJob.h"
#include <ui/UIManager.h>
#include <ui/ProjectWindow.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{
    FileMenuCmd::FileMenuCmd() = default;

    FileMenuCmd::~FileMenuCmd() = default;

    void FileMenuCmd::undo()
    {
    }

    void FileMenuCmd::redo()
    {
    }

    void FileMenuCmd::execute()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto uiManager = editorManager->getUI();
        FB_ASSERT( uiManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto jobQueue = applicationManager->getJobQueue();
        auto selectionManager = applicationManager->getSelectionManager();

        switch( auto eventId = getItemId() )
        {
        case UIManager::WidgetId::None:
        {
        }
        break;
        case UIManager::WidgetId::Exit:
        {
            applicationManager->setQuit( true );
        }
        break;
        case UIManager::WidgetId::About:
        {
        }
        break;
        case UIManager::WidgetId::NewProjectId:
        {
            auto project = editorManager->getProject();

            if( auto fileDialog = fileSystem->openFileDialog() )
            {
                auto projectPath = Path::getWorkingDirectory();
                if( !fileSystem->isExistingFolder( projectPath ) )
                {
                    projectPath = "";
                }

                fileDialog->setDialogMode( INativeFileDialog::DialogMode::Select );
                fileDialog->setFileExtension( ".fbproject" );
                fileDialog->setFilePath( projectPath );

                auto result = fileDialog->openDialog();
                if( result == INativeFileDialog::Result::Dialog_Okay )
                {
                    auto filePath = fileDialog->getFilePath();
                    if( !StringUtil::isNullOrEmpty( filePath ) )
                    {
                        project->create( filePath );

                        auto projectFilePath = filePath + "/project.fbproject";
                        editorManager->loadProject( projectFilePath );
                    }
                }
            }
        }
        break;
        case UIManager::WidgetId::OpenProjectId:
        {
            if( auto fileDialog = fileSystem->openFileDialog() )
            {
                auto projectPath = applicationManager->getProjectPath();
                if( !fileSystem->isExistingFolder( projectPath ) )
                {
                    projectPath = "";
                }

                fileDialog->setDialogMode( INativeFileDialog::DialogMode::Open );
                fileDialog->setFileExtension( ".fbproject" );
                fileDialog->setFilePath( projectPath );

                auto result = fileDialog->openDialog();
                if( result == INativeFileDialog::Result::Dialog_Okay )
                {
                    auto filePath = fileDialog->getFilePath();
                    if( !StringUtil::isNullOrEmpty( filePath ) )
                    {
                        editorManager->loadProject( filePath );
                    }
                }
            }
        }
        break;
        case UIManager::WidgetId::NewSceneDialog:
        {
        }
        break;
        case UIManager::WidgetId::OpenSceneId:
        {
            if( auto fileDialog = fileSystem->openFileDialog() )
            {
                auto projectPath = applicationManager->getProjectPath();
                if( !fileSystem->isExistingFolder( projectPath ) )
                {
                    projectPath = "";
                }

                fileDialog->setDialogMode( INativeFileDialog::DialogMode::Open );
                fileDialog->setFileExtension( ".fbscene" );
                fileDialog->setFilePath( projectPath );

                auto result = fileDialog->openDialog();
                if( result == INativeFileDialog::Result::Dialog_Okay )
                {
                    auto filePath = fileDialog->getFilePath();
                    if( !StringUtil::isNullOrEmpty( filePath ) )
                    {
                        auto job = fb::make_ptr<OpenSceneJob>();
                        job->execute();
                    }
                }
            }
        }
        break;
        case UIManager::WidgetId::BatchAllBtnId:
        {
        }
        break;
        case UIManager::WidgetId::AppPropertiesId:
        {
        }
        break;
        case UIManager::WidgetId::CreateOverlayTestId:
        {
            //ApplicationUtil::createOverlayPanelTest();

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            ui->rebuildSceneTree();
        }
        break;
        case UIManager::WidgetId::CreateOverlayTextTestId:
        {
            //ApplicationUtil::createOverlayTextTest();

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            ui->rebuildSceneTree();
        }
        break;
        case UIManager::WidgetId::CreateOverlayButtonTestId:
        {
            //ApplicationUtil::createOverlayButtonTest();

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            ui->rebuildSceneTree();
        }
        break;
        case UIManager::WidgetId::CreateProceduralTestId:
        {
            //auto applicationManager = core::ApplicationManager::instance();
            //FB_ASSERT( applicationManager );

            //auto actor = ApplicationUtil::createProceduralTest();
            //FB_ASSERT( actor );

            //if( applicationManager->isPlaying() )
            //{
            //    actor->setState( scene::IActor::State::Play );
            //}
            //else
            //{
            //    actor->setState( scene::IActor::State::Edit );
            //}

            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            ui->rebuildSceneTree();
        }
        break;
        case UIManager::WidgetId::GenerateCMakeProjectId:
        {
            auto job = fb::make_ptr<CreateCodeProjectJob>();
            jobQueue->addJob( job );
        }
        break;
        case UIManager::WidgetId::CompileId:
        {
            auto job = fb::make_ptr<CompileProjectJob>();
            jobQueue->addJob( job );
        }
        break;
        case UIManager::WidgetId::CreatePackageId:
            break;
        case UIManager::WidgetId::ProjectSettingsId:
            break;
        case UIManager::WidgetId::LoadProceduralSceneId:
        {
            //auto editorManager = EditorManager::getSingletonPtr();
            //FB_ASSERT( editorManager );

            //auto uiManager = editorManager->getUI();
            //FB_ASSERT( uiManager );

            //auto fileBrowser = uiManager->getFileBrowser();
            //FB_ASSERT( fileBrowser );

            //fileBrowser->setElementId( (s32)UIManager::WidgetId::LoadProceduralSceneDialog );
            //fileBrowser->setFileExtension( ".osm" );
            //fileBrowser->show();

            if( auto fileDialog = fileSystem->openFileDialog() )
            {
                auto projectPath = applicationManager->getProjectPath();
                if( !fileSystem->isExistingFolder( projectPath ) )
                {
                    projectPath = "";
                }

                fileDialog->setDialogMode( INativeFileDialog::DialogMode::Open );
                fileDialog->setFileExtension( ".osm" );
                fileDialog->setFilePath( projectPath );

                auto result = fileDialog->openDialog();
                if( result == INativeFileDialog::Result::Dialog_Okay )
                {
                }
            }
        }
        break;
        case UIManager::WidgetId::SaveProceduralSceneId:
        {
            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto uiManager = editorManager->getUI();
            FB_ASSERT( uiManager );

            //auto fileBrowser = uiManager->getFileBrowser();
            //FB_ASSERT( fileBrowser );

            //fileBrowser->setElementId( (s32)UIManager::WidgetId::SaveProceduralSceneDialog );
            //fileBrowser->setFileExtension( ".osm" );
            //fileBrowser->show();
        }
        break;
        case UIManager::WidgetId::LuaEditConfigDialogId:
            break;

        case UIManager::WidgetId::GotoId:
        {
        }
        break;
        case UIManager::WidgetId::ShowAllOverlaysId:
            break;
        case UIManager::WidgetId::HideAllOverlaysId:
            break;
        case UIManager::WidgetId::CreateRigidBodies:
        {
            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );

                    auto job = fb::make_ptr<CreateRigidBodies>();
                    job->setActor( actor );
                    job->setMakeStatic( true );
                    job->setConvex( false );
                    job->setCascade( true );

                    jobQueue->addJob( job );
                }
            }
        }
        break;
        case UIManager::WidgetId::CreateRigidStaticMeshId:
        {
            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );

                    auto job = fb::make_ptr<CreateRigidBodies>();
                    job->setActor( actor );
                    job->setMakeStatic( true );
                    job->setConvex( false );
                    job->setCascade( true );

                    jobQueue->addJob( job );
                }
            }
        }
        break;
        case UIManager::WidgetId::CreateRigidDynamicMeshId:
        {
            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );

                    auto job = fb::make_ptr<CreateRigidBodies>();
                    job->setActor( actor );
                    job->setMakeStatic( false );
                    job->setConvex( true );
                    job->setCascade( true );

                    jobQueue->addJob( job );
                }
            }
        }
        break;
        case UIManager::WidgetId::CreateConstraintId:
        {
        }
        break;
        case UIManager::WidgetId::CreateDefaultCarId:
            break;
        case UIManager::WidgetId::CreateDefaultTruckId:
            break;
        case UIManager::WidgetId::ConvertCSharpId:
            break;
        case UIManager::WidgetId::PhysicsEnableId:
            break;
        case UIManager::WidgetId::ID_CustomizeToolbar:
            break;
        case UIManager::WidgetId::RunId:
            break;
        case UIManager::WidgetId::StopId:
            break;
        case UIManager::WidgetId::FileBrowserId:
            break;
        case UIManager::WidgetId::ID_SampleItem:
            break;
        default:
        {
        }
        break;
        };
    }

    auto FileMenuCmd::getItemId() const -> UIManager::WidgetId
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_itemId;
    }

    void FileMenuCmd::setItemId( UIManager::WidgetId itemId )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_itemId = itemId;
    }

}  // namespace fb::editor
