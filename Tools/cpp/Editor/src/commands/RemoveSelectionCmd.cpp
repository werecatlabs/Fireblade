#include <GameEditorPCH.h>
#include "RemoveSelectionCmd.h"
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    RemoveSelectionCmd::RemoveSelectionCmd() = default;

    RemoveSelectionCmd::~RemoveSelectionCmd() = default;

    void RemoveSelectionCmd::undo()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        for( auto selectionData : m_actorData )
        {
            auto actor = sceneManager->createActor();

            auto data = selectionData->getActorData();
            actor->fromData( data );

            auto parent = selectionData->getParent();
            if( parent )
            {
                parent->addChild( actor );
            }
            else
            {
                scene->addActor( actor );
                scene->registerAllUpdates( actor );
            }

            selectionData->setActor( actor );
        }

        auto editorManager = EditorManager::getSingletonPtr();
        auto uiManager = editorManager->getUI();
        uiManager->rebuildSceneTree();
    }

    void RemoveSelectionCmd::redo()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        for( auto selectionData : m_actorData )
        {
            auto actor = selectionData->getActor();
            if( actor )
            {
                scene->removeActor( actor );

                auto parent = actor->getParent();
                if( parent )
                {
                    parent->removeChild( actor );
                    selectionData->setParent( parent );
                }

                auto data = actor->toData();
                if( data )
                {
                    selectionData->setActorData( data );
                }

                actor->unload( nullptr );
            }
        }

        auto editorManager = EditorManager::getSingletonPtr();
        auto uiManager = editorManager->getUI();
        uiManager->rebuildSceneTree();
    }

    void RemoveSelectionCmd::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto selection = selectionManager->getSelection();
        for( auto selected : selection )
        {
            auto selectionData = fb::make_ptr<ActorData>();

            auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
            if( actor )
            {
                scene->removeActor( actor );

                auto parent = actor->getParent();
                if( parent )
                {
                    parent->removeChild( actor );
                    selectionData->setParent( parent );
                }

                auto data = actor->toData();
                if( data )
                {
                    selectionData->setActorData( data );
                }

                sceneManager->destroyActor( actor );

                m_actorData.push_back( selectionData );
            }
        }

        auto editorManager = EditorManager::getSingletonPtr();
        auto uiManager = editorManager->getUI();
        uiManager->rebuildSceneTree();
    }

    RemoveSelectionCmd::ActorData::ActorData() = default;

    RemoveSelectionCmd::ActorData::~ActorData() = default;

    auto RemoveSelectionCmd::ActorData::getParent() const -> SmartPtr<scene::IActor>
    {
        return m_parent;
    }

    void RemoveSelectionCmd::ActorData::setParent( SmartPtr<scene::IActor> val )
    {
        m_parent = val;
    }

    auto RemoveSelectionCmd::ActorData::getActor() const -> SmartPtr<scene::IActor>
    {
        return m_actor;
    }

    void RemoveSelectionCmd::ActorData::setActor( SmartPtr<scene::IActor> val )
    {
        m_actor = val;
    }

    auto RemoveSelectionCmd::ActorData::getActorData() const -> SmartPtr<ISharedObject>
    {
        return m_actorData;
    }

    void RemoveSelectionCmd::ActorData::setActorData( SmartPtr<ISharedObject> val )
    {
        m_actorData = val;
    }

}  // namespace fb::editor
