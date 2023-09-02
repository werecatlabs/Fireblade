#include <GameEditorPCH.h>
#include <jobs/SceneDropJob.h>
#include "commands/AddActorCmd.h"
#include "commands/DragDropActorCmd.h"
#include "ui/SceneWindow.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {
        SceneDropJob::SceneDropJob()
        {
        }

        SceneDropJob::~SceneDropJob()
        {
        }

        void SceneDropJob::execute()
        {
            try
            {
                auto text = getData();
                auto sender = getSender();
                auto tree = getTree();
                auto owner = getOwner();

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto commandManager = applicationManager->getCommandManager();
                FB_ASSERT( commandManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto properties = fb::make_ptr<Properties>();
                auto dataStr = String( text.c_str() );

                if( !StringUtil::isNullOrEmpty( dataStr ) )
                {
                    DataUtil::parse( dataStr, properties.get() );

                    auto filePath = properties->getProperty( "filePath" );

                    auto dragSrc = tree->getDragSourceElement();
                    auto dropDst = tree->getDropDestinationElement();

                    if( sender->isDerived<ui::IUIWindow>() )
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

        String SceneDropJob::getData() const
        {
            return m_data;
        }

        void SceneDropJob::setData( const String &data )
        {
            m_data = data;
        }

        String SceneDropJob::getFilePath() const
        {
            return m_filePath;
        }

        void SceneDropJob::setFilePath( const String &filePath )
        {
            m_filePath = filePath;
        }

        SmartPtr<ui::IUIElement> SceneDropJob::getSender() const
        {
            return m_sender;
        }

        void SceneDropJob::setSender( SmartPtr<ui::IUIElement> sender )
        {
            m_sender = sender;
        }

        SmartPtr<ICommand> SceneDropJob::getDragDropActorCmd() const
        {
            return m_dragDropActorCmd;
        }

        void SceneDropJob::setDragDropActorCmd( SmartPtr<ICommand> dragDropActorCmd )
        {
            m_dragDropActorCmd = dragDropActorCmd;
        }

        SmartPtr<ui::IUITreeCtrl> SceneDropJob::getTree() const
        {
            return m_tree;
        }

        void SceneDropJob::setTree( SmartPtr<ui::IUITreeCtrl> tree )
        {
            m_tree = tree;
        }

        SmartPtr<SceneWindow> SceneDropJob::getOwner() const
        {
            return m_owner;
        }

        void SceneDropJob::setOwner( SmartPtr<SceneWindow> owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
