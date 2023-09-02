#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTreeNode.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiTreeNode, CImGuiElement<IUITreeNode> );

        u32 ImGuiTreeNode::m_idExt = 0;

        ImGuiTreeNode::ImGuiTreeNode()
        {
        }

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

        SmartPtr<IUIElement> ImGuiTreeNode::getNodeData() const
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

        SmartPtr<ISharedObject> ImGuiTreeNode::getNodeUserData() const
        {
            return m_nodeUserData;
        }

        void ImGuiTreeNode::setNodeUserData( SmartPtr<ISharedObject> nodeUserData )
        {
            m_nodeUserData = nodeUserData;
        }

        IUITreeNode::Type ImGuiTreeNode::getNodeType() const
        {
            return m_nodeType;
        }

        void ImGuiTreeNode::setNodeType( Type nodeType )
        {
            m_nodeType = nodeType;
        }

        SmartPtr<IUITreeCtrl> ImGuiTreeNode::getOwnerTree() const
        {
            return m_ownerTree.lock();
        }

        void ImGuiTreeNode::setOwnerTree( SmartPtr<IUITreeCtrl> owner )
        {
            m_ownerTree = owner;
        }

        u32 ImGuiTreeNode::getTreeNodeId() const
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

        void ImGuiTreeNode::createTreeNode( SmartPtr<IUITreeCtrl> tree, SmartPtr<IUIElement> element,
                                            s32 &numNodesDisplayed )
        {
            try
            {
                if( element && element->isDerived<IUITreeNode>() )
                {
                    auto treeNode = fb::static_pointer_cast<IUITreeNode>( element );
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

                    auto children = element->getChildrenByType<IUITreeNode>();

                    if( children.empty() )
                    {
                        node_flags |= ImGuiTreeNodeFlags_Leaf;
                    }

                    node_flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
                    node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;

                    auto handle = treeNode->getHandle();
                    auto id = handle->getInstanceId();

                    auto opened = ImGui::TreeNodeEx( (void *)static_cast<intptr_t>( id ), node_flags,
                                                     label.c_str() );

                    treeNode->setExpanded( opened );
                    numNodesDisplayed++;

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

                            auto retValue =
                                dragSource->handleEvent( IEvent::Type::UI, IEvent::handleDrag, args,
                                                         treeNode, treeNode, nullptr );

                            auto &dragData = retValue.str;

                            if( !StringUtil::isNullOrEmpty( dragData ) )
                            {
                                ImGui::SetDragDropPayload( "_TREENODE", dragData.c_str(),
                                                           dragData.size() );
                                ImGui::Text( label.c_str() );
                            }

                            ImGui::EndDragDropSource();
                        }
                    }

                    if( auto dropTarget = tree->getDropTarget() )
                    {
                        if( ImGui::BeginDragDropTarget() )
                        {
                            if( ImGui::IsMouseReleased( 0 ) )
                            {
                                tree->setDropDestinationElement( treeNode );

                                auto payload = ImGui::GetDragDropPayload();
                                if( payload )
                                {
                                    auto data = String( static_cast<const char *>( payload->Data ),
                                                        payload->DataSize );
                                    if( !StringUtil::isNullOrEmpty( data ) )
                                    {
                                        auto src = tree->getDragSourceElement();
                                        auto dst = tree->getDropDestinationElement();

                                        auto args = Array<Parameter>();
                                        args.reserve( 1 );

                                        args.push_back( Parameter( data ) );

                                        dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop,
                                                                 args, src, dst, nullptr );
                                    }
                                }
                            }

                            ImGui::EndDragDropTarget();
                        }
                    }

                    if( ImGui::IsMouseReleased( 0 ) && ImGui::IsItemHovered( ImGuiHoveredFlags_None ) )
                    {
                        if( tree->isMultiSelect() )
                        {
                            tree->addSelectTreeNode( treeNode );
                        }
                        else
                        {
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
                                                           IEvent::handleTreeNodeDoubleClicked, args,
                                                           tree, treeNode, nullptr );
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

                                listener->handleEvent( IEvent::Type::Object,
                                                       IEvent::handleTreeSelectionRelease, args, tree,
                                                       treeNode, nullptr );
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
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // end namespace ui
}  // end namespace fb
