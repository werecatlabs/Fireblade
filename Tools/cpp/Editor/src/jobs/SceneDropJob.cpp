#include <GameEditorPCH.h>
#include <jobs/SceneDropJob.h>
#include "commands/AddActorCmd.h"
#include "commands/DragDropActorCmd.h"
#include <editor/EditorManager.h>
#include "ui/SceneWindow.h"
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{
    SceneDropJob::SceneDropJob() = default;

    SceneDropJob::~SceneDropJob() = default;

    void SceneDropJob::execute()
    {
        try
        {
            auto text = getData();
            auto sender = getSender();
            auto tree = getTree();
            auto owner = getOwner();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto commandManager = applicationManager->getCommandManager();
            FB_ASSERT( commandManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto editorManager = EditorManager::getSingletonPtr();

            auto uiManager = editorManager->getUI();

            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto sceneManager = applicationManager->getSceneManager();
            auto scene = sceneManager->getCurrentScene();

            auto properties = fb::make_ptr<Properties>();
            auto dataStr = String( text.c_str() );

            DataUtil::parse( dataStr, properties.get() );

            if( properties->hasProperty( "resourceUUID" ) )
            {
                auto resourceId = properties->getProperty( "resourceUUID" );
                auto iResourceId = StringUtil::parseInt( resourceId );

                SmartPtr<scene::IPrefab> prefab = resourceDatabase->loadResource( iResourceId );
                if( prefab )
                {
                    auto actor = prefab->createActor();
                    scene->addActor( actor );
                }

                uiManager->rebuildSceneTree();
            }
            else if( !StringUtil::isNullOrEmpty( dataStr ) )
            {
                auto filePath = properties->getProperty( "filePath" );

                auto dragSrc = tree->getDragSourceElement();
                auto dropDst = tree->getDropDestinationElement();

                if( sender && sender->isDerived<ui::IUIWindow>() )
                {
                    if( !StringUtil::isNullOrEmpty( filePath ) )
                    {
                        auto cmd = fb::make_ptr<AddActorCmd>();
                        cmd->setActorType( AddActorCmd::ActorType::Actor );
                        cmd->setFilePath( filePath );
                        commandManager->addCommand( cmd );
                    }
                    else
                    {
                        if( !owner->getDragDropActorCmd() )
                        {
                            auto cmd = fb::make_ptr<DragDropActorCmd>();
                            cmd->setPosition( Vector2I::zero() );
                            cmd->setSrc( sender );
                            cmd->setDst( nullptr );
                            cmd->setData( text );

                            commandManager->addCommand( cmd );
                        }

                        owner->setDragDropActorCmd( nullptr );
                    }
                }
                else if( dragSrc )
                {
                    if( !commandManager->hasCommand( owner->getDragDropActorCmd() ) )
                    {
                        owner->setDragDropActorCmd( nullptr );
                    }

                    if( !owner->getDragDropActorCmd() )
                    {
                        auto cmd = fb::make_ptr<DragDropActorCmd>();
                        cmd->setPosition( Vector2I::zero() );
                        cmd->setSrc( dragSrc );
                        cmd->setDst( dropDst );
                        cmd->setData( text );
                        cmd->setSiblingIndex( getSiblingIndex() );
                        owner->setDragDropActorCmd( cmd );

                        commandManager->addCommand( cmd );
                    }
                }
                else if( !StringUtil::isNullOrEmpty( filePath ) )
                {
                    auto cmd = fb::make_ptr<AddActorCmd>();
                    cmd->setActorType( AddActorCmd::ActorType::Actor );
                    cmd->setFilePath( filePath );
                    commandManager->addCommand( cmd );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto SceneDropJob::getData() const -> String
    {
        return m_data;
    }

    void SceneDropJob::setData( const String &data )
    {
        m_data = data;
    }

    auto SceneDropJob::getFilePath() const -> String
    {
        return m_filePath;
    }

    void SceneDropJob::setFilePath( const String &filePath )
    {
        m_filePath = filePath;
    }

    auto SceneDropJob::getSender() const -> SmartPtr<ui::IUIElement>
    {
        return m_sender;
    }

    void SceneDropJob::setSender( SmartPtr<ui::IUIElement> sender )
    {
        m_sender = sender;
    }

    auto SceneDropJob::getDragDropActorCmd() const -> SmartPtr<ICommand>
    {
        return m_dragDropActorCmd;
    }

    void SceneDropJob::setDragDropActorCmd( SmartPtr<ICommand> dragDropActorCmd )
    {
        m_dragDropActorCmd = dragDropActorCmd;
    }

    auto SceneDropJob::getTree() const -> SmartPtr<ui::IUITreeCtrl>
    {
        return m_tree;
    }

    void SceneDropJob::setTree( SmartPtr<ui::IUITreeCtrl> tree )
    {
        m_tree = tree;
    }

    auto SceneDropJob::getOwner() const -> SmartPtr<SceneWindow>
    {
        return m_owner;
    }

    void SceneDropJob::setOwner( SmartPtr<SceneWindow> owner )
    {
        m_owner = owner;
    }

    auto SceneDropJob::getSiblingIndex() const -> s32
    {
        return m_siblingIndex;
    }

    void SceneDropJob::setSiblingIndex( s32 siblingIndex )
    {
        m_siblingIndex = siblingIndex;
    }

}  // namespace fb::editor
