#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTreeNode.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiTreeNode, CImGuiElement<IUITreeNode> );

    u32 ImGuiTreeNode::m_idExt = 0;

    ImGuiTreeNode::ImGuiTreeNode() = default;

    ImGuiTreeNode::~ImGuiTreeNode()
    {
        unload( nullptr );
    }

    void ImGuiTreeNode::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loaded );
    }

    void ImGuiTreeNode::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                if( auto parent = getParent() )
                {
                    parent->removeChild( this );
                }

                m_ownerTree = nullptr;
                m_nodeUserData = nullptr;
                m_nodeData = nullptr;

                CImGuiElement<IUITreeNode>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ImGuiTreeNode::getNodeData() const -> SmartPtr<IUIElement>
    {
        return m_nodeData;
    }

    void ImGuiTreeNode::setNodeData( SmartPtr<IUIElement> nodeData )
    {
        if( nodeData->isDerived<IUIText>() )
        {
            int stop = 0;
            stop = 0;

            FB_LOG_ERROR( "data error" );
        }

        m_nodeData = nodeData;
    }

    auto ImGuiTreeNode::getNodeUserData() const -> SmartPtr<ISharedObject>
    {
        return m_nodeUserData;
    }

    void ImGuiTreeNode::setNodeUserData( SmartPtr<ISharedObject> nodeUserData )
    {
        m_nodeUserData = nodeUserData;
    }

    auto ImGuiTreeNode::getNodeType() const -> IUITreeNode::Type
    {
        return m_nodeType;
    }

    void ImGuiTreeNode::setNodeType( Type nodeType )
    {
        m_nodeType = nodeType;
    }

    auto ImGuiTreeNode::getOwnerTree() const -> SmartPtr<IUITreeCtrl>
    {
        return m_ownerTree.lock();
    }

    void ImGuiTreeNode::setOwnerTree( SmartPtr<IUITreeCtrl> owner )
    {
        m_ownerTree = owner;
    }

    auto ImGuiTreeNode::getTreeNodeId() const -> u32
    {
        return m_treeNodeId;
    }

    void ImGuiTreeNode::setTreeNodeId( u32 treeNodeId )
    {
        m_treeNodeId = treeNodeId;
    }

    void ImGuiTreeNode::setExpanded( bool expanded )
    {
        m_expanded = expanded;
    }

    void ImGuiTreeNode::setSelected( bool selected )
    {
        m_selected = selected;
    }

    void ImGuiTreeNode::createTreeNode( SmartPtr<IUITreeCtrl> &tree, SmartPtr<IUITreeNode> &treeNode,
                                        s32 &numNodesDisplayed )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto inputManager = applicationManager->getInputDeviceManager();

            auto label = Util::getText( treeNode );

            if( StringUtil::isNullOrEmpty( label ) )
            {
                label = "Untitled";
            }

            auto node_flags = static_cast<int>( ImGuiTreeNodeFlags_None );
            if( treeNode->isExpanded() )
            {
                node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
            }

            if( treeNode->isSelected() )
            {
                node_flags |= ImGuiTreeNodeFlags_Selected;
            }

            auto children = treeNode->getChildrenByType<IUITreeNode>();

            if( children.empty() )
            {
                node_flags |= ImGuiTreeNodeFlags_Leaf;
            }

            node_flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
            node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;

            auto handle = treeNode->getHandle();
            auto id = handle->getInstanceId();

            auto opened =
                ImGui::TreeNodeEx( (void *)static_cast<intptr_t>( id ), node_flags, label.c_str() );

            auto siblingIndex = treeNode->getSiblingIndexByType<IUITreeNode>();
            auto siblingCount = treeNode->getSiblingCountByType<IUITreeNode>();

            if( ImGui::IsMouseDragging( 0 ) )
            {
                ImVec2 rectMin = ImGui::GetItemRectMin();
                ImVec2 rectMax = ImGui::GetItemRectMax();

                ImVec2 lineRectMin = ImGui::GetItemRectMin();
                ImVec2 lineRectMax = ImGui::GetItemRectMax();
                lineRectMin.y -= ( lineRectMax.y - lineRectMin.y ) * 0.1f;

                ImVec2 lastLineRectMin = ImGui::GetItemRectMin();
                ImVec2 lastLineRectMax = ImGui::GetItemRectMax();
                lastLineRectMax.y += ( lastLineRectMax.y - lastLineRectMin.y ) * 0.1f;

                if( ImGui::IsMouseHoveringRect( rectMin, rectMax, true ) )
                {
                    ImGui::GetWindowDrawList()->AddRect( rectMin, rectMax, IM_COL32( 255, 0, 0, 255 ) );

                    tree->setSelectedSiblingIndex( -1 );

                    tree->setDropDestinationElement( treeNode );
                }
                else if( ImGui::IsMouseHoveringRect( lineRectMin, lineRectMax, true ) )
                {
                    // Get the position of the hovered node
                    auto startPos = ImGui::GetCursorScreenPos();
                    startPos.y = lineRectMin.y;

                    auto endPos = ImVec2( startPos.x + 100, startPos.y );

                    // Draw a line
                    ImGui::GetWindowDrawList()->AddLine( startPos, endPos, IM_COL32( 255, 0, 0, 255 ),
                                                         2.0f );

                    tree->setSelectedSiblingIndex( siblingIndex );

                    tree->setDropDestinationElement( treeNode->getParent() );
                }
                else if( siblingIndex >= siblingCount - 1 &&
                         ImGui::IsMouseHoveringRect( lastLineRectMin, lastLineRectMax, true ) )
                {
                    // Get the position of the hovered node
                    auto startPos = ImGui::GetCursorScreenPos();
                    auto endPos = ImVec2( startPos.x + 100, startPos.y );

                    // Draw a line
                    ImGui::GetWindowDrawList()->AddLine( startPos, endPos, IM_COL32( 255, 0, 0, 255 ),
                                                         2.0f );

                    tree->setSelectedSiblingIndex( siblingIndex );

                    tree->setDropDestinationElement( treeNode->getParent() );
                }
            }

            treeNode->setExpanded( opened );
            numNodesDisplayed++;

            bool isNodeHovered = ImGui::IsItemHovered();
            treeNode->setHovered( isNodeHovered );

            if( auto dragSource = tree->getDragSource() )
            {
                if( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
                {
                    tree->setDragSourceElement( treeNode );

                    auto args = Array<Parameter>();
                    args.resize( 3 );

                    args[0].object = treeNode;
                    args[1].object = nullptr;
                    args[2].object = nullptr;

                    auto retValue = dragSource->handleEvent( IEvent::Type::UI, IEvent::handleDrag, args,
                                                             treeNode, treeNode, nullptr );

                    auto &dragData = retValue.str;

                    if( !StringUtil::isNullOrEmpty( dragData ) )
                    {
                        ImGui::SetDragDropPayload( "_TREENODE", dragData.c_str(), dragData.size() );
                        ImGui::Text( label.c_str() );
                    }

                    ImGui::EndDragDropSource();
                }
            }

            if( auto dropTarget = tree->getDropTarget() )
            {
                if( ImGui::IsMouseReleased( 0 ) )
                {
                    bool isNode = false;
                    if( ImGui::BeginDragDropTarget() )
                    {
                        isNode = true;
                    }

                    if( isNode || tree->getSelectedSiblingIndex() != -1 )
                    {
                        auto payload = ImGui::GetDragDropPayload();
                        if( payload )
                        {
                            auto data =
                                String( static_cast<const char *>( payload->Data ), payload->DataSize );
                            if( !StringUtil::isNullOrEmpty( data ) )
                            {
                                auto src = tree->getDragSourceElement();
                                auto dst = tree->getDropDestinationElement();

                                auto args = Array<Parameter>();
                                args.reserve( 2 );

                                args.emplace_back( data );
                                args.emplace_back( isNode ? -1 : tree->getSelectedSiblingIndex() );

                                dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args, src,
                                                         dst, nullptr );
                            }
                        }
                    }

                    if( isNode )
                    {
                        ImGui::EndDragDropTarget();
                    }
                }
            }

            if( ImGui::IsMouseReleased( 0 ) && ImGui::IsItemHovered( ImGuiHoveredFlags_None ) )
            {
                if( tree->isMultiSelect() )
                {
                    if( inputManager->isKeyPressed( KeyCodes::KEY_LCONTROL ) )
                    {
                        tree->addSelectTreeNode( treeNode );
                    }
                    else
                    {
                        tree->clearSelectTreeNodes();
                        tree->setSelectTreeNode( treeNode );
                    }
                }
                else
                {
                    tree->clearSelectTreeNodes();
                    tree->setSelectTreeNode( treeNode );
                }
            }

            if( ImGui::IsMouseClicked( 0 ) && ImGui::IsItemHovered( ImGuiHoveredFlags_None ) )
            {
                auto ownerTree = treeNode->getOwnerTree();
                if( ownerTree )
                {
                    auto listeners = ownerTree->getObjectListeners();
                    for( auto listener : listeners )
                    {
                        auto args = Array<Parameter>();
                        args.resize( 2 );

                        args[0].object = treeNode;

                        listener->handleEvent( IEvent::Type::Object,
                                               IEvent::handleTreeSelectionActivated, args, tree,
                                               treeNode, nullptr );
                    }
                }

                if( ImGui::IsMouseDoubleClicked( 0 ) )
                {
                    if( ownerTree )
                    {
                        auto listeners = ownerTree->getObjectListeners();
                        for( auto listener : listeners )
                        {
                            auto args = Array<Parameter>();
                            args.resize( 2 );

                            args[0].object = treeNode;

                            listener->handleEvent( IEvent::Type::Object,
                                                   IEvent::handleTreeNodeDoubleClicked, args, tree,
                                                   treeNode, nullptr );
                        }
                    }
                }
            }

            if( ImGui::IsMouseReleased( 0 ) && ImGui::IsItemHovered( ImGuiHoveredFlags_None ) )
            {
                auto ownerTree = treeNode->getOwnerTree();
                if( ownerTree )
                {
                    auto listeners = ownerTree->getObjectListeners();
                    for( auto listener : listeners )
                    {
                        auto args = Array<Parameter>();
                        args.resize( 2 );

                        args[0].object = treeNode;

                        listener->handleEvent( IEvent::Type::Object, IEvent::handleTreeSelectionRelease,
                                               args, tree, treeNode, nullptr );
                    }
                }
            }

            if( opened )
            {
                try
                {
                    for( auto &child : children )
                    {
                        createTreeNode( tree, child, numNodesDisplayed );
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }

                // if(tree->getRoot() == treeNode)
                //{
                //     if(( node_flags & ImGuiTreeNodeFlags_Leaf ) != 0)
                //     {
                //         ImGui::TreePop();
                //     }
                // }
                // else

                ImGui::TreePop();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // namespace fb::ui
