#include <GameEditorPCH.h>
#include <commands/DragDropActorCmd.h>
#include <editor/EditorManager.h>
#include <ui/ProjectTreeData.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    void DragDropActorCmd::undo()
    {
    }

    void DragDropActorCmd::redo()
    {
    }

    void DragDropActorCmd::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto dragSrc = getSrc();
        auto dropDst = getDst();

        auto isSrcTreeNode = false;
        auto isDstTreeNode = false;

        if( dragSrc )
        {
            isSrcTreeNode = dragSrc->isDerived<ui::IUITreeNode>();
        }

        if( dropDst )
        {
            isDstTreeNode = dropDst->isDerived<ui::IUITreeNode>();
        }

        if( isSrcTreeNode && isDstTreeNode )
        {
            auto dragSrcNode = fb::static_pointer_cast<ui::IUITreeNode>( dragSrc );
            auto dropDstNode = fb::static_pointer_cast<ui::IUITreeNode>( dropDst );

            auto dragSrcData = dragSrcNode->getNodeUserData();
            auto dropDstData = dropDstNode->getNodeUserData();

            if( dragSrcData && dropDstData )
            {
                auto dragSrcProjectData = fb::static_pointer_cast<ProjectTreeData>( dragSrcData );

                auto dropDstProjectData = fb::static_pointer_cast<ProjectTreeData>( dropDstData );

                auto dragSrcObject = dragSrcProjectData->getObjectData();
                auto dropDstObject = dropDstProjectData->getObjectData();

                if( dragSrcObject && dropDstObject )
                {
                    if( dragSrcObject->isDerived<scene::IActor>() &&
                        dropDstObject->isDerived<scene::IActor>() )
                    {
                        auto dragSrcActor = fb::static_pointer_cast<scene::IActor>( dragSrcObject );
                        auto dropDstActor = fb::static_pointer_cast<scene::IActor>( dropDstObject );

                        auto parent = dragSrcActor->getParent();
                        if( parent != dropDstActor )
                        {
                            if( parent )
                            {
                                parent->removeChild( dragSrcActor );
                            }

                            scene->removeActor( dragSrcActor );
                            if( dropDstActor->getParent() == dragSrcActor )
                            {
                                auto srcParent = dropDstActor->getParent();
                                if( srcParent )
                                {
                                    srcParent->removeChild( dropDstActor );
                                }

                                auto dstParent = dropDstActor->getParent();
                                if( dstParent )
                                {
                                    dstParent->removeChild( dropDstActor );
                                }

                                auto parent = dragSrcActor->getParent();
                                if( !parent )
                                {
                                    FB_ASSERT( dropDstActor->getParent() == nullptr );
                                    scene->addActor( dropDstActor );
                                }
                                else
                                {
                                    parent->addChild( dropDstActor );
                                }
                            }

                            dropDstActor->addChild( dragSrcActor );
                        }
                    }
                }
            }
        }
        else if( dragSrc->isDerived<ui::IUIWindow>() )
        {
            auto selectionManager = applicationManager->getSelectionManager();
            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                    auto parent = actor->getParent();
                    if( parent )
                    {
                        parent->removeChild( actor );

                        scene->addActor( actor );
                    }
                }
            }
        }

        if( isSrcTreeNode )
        {
            auto dragSrcNode = fb::static_pointer_cast<ui::IUITreeNode>( dragSrc );

            auto dragSrcData = dragSrcNode->getNodeUserData();
            if( dragSrcData )
            {
                auto dragSrcProjectData = fb::static_pointer_cast<ProjectTreeData>( dragSrcData );

                auto dragSrcObject = dragSrcProjectData->getObjectData();
                if( dragSrcObject )
                {
                    if( dragSrcObject->isDerived<scene::IActor>() )
                    {
                        auto dragSrcActor = fb::static_pointer_cast<scene::IActor>( dragSrcObject );

                        auto siblingIndex = getSiblingIndex();
                        if( siblingIndex >= 0 )
                        {
                            dragSrcActor->setSiblingIndex( siblingIndex );
                        }
                    }
                }
            }
        }

        sceneManager->makeActorTransformsDirty();

        auto editorManager = EditorManager::getSingletonPtr();
        auto ui = editorManager->getUI();
        ui->rebuildSceneTree();
    }

    auto DragDropActorCmd::getPosition() const -> Vector2I
    {
        return m_position;
    }

    void DragDropActorCmd::setPosition( const Vector2I &position )
    {
        m_position = position;
    }

    auto DragDropActorCmd::getSrc() const -> SmartPtr<ui::IUIElement>
    {
        return m_src;
    }

    void DragDropActorCmd::setSrc( SmartPtr<ui::IUIElement> src )
    {
        m_src = src;
    }

    auto DragDropActorCmd::getDst() const -> SmartPtr<ui::IUIElement>
    {
        return m_dst;
    }

    void DragDropActorCmd::setDst( SmartPtr<ui::IUIElement> dst )
    {
        m_dst = dst;
    }

    auto DragDropActorCmd::getData() const -> String
    {
        return m_data;
    }

    void DragDropActorCmd::setData( const String &data )
    {
        m_data = data;
    }

    auto DragDropActorCmd::getSiblingIndex() const -> s32
    {
        return m_siblingIndex;
    }

    void DragDropActorCmd::setSiblingIndex( s32 siblingIndex )
    {
        m_siblingIndex = siblingIndex;
    }

}  // namespace fb::editor
