#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTreeCtrl.h>
#include "FBImGui/ImGuiTreeNode.h"
#include <FBCore/FBCore.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiTreeCtrl, CImGuiElement<IUITreeCtrl> );

    u32 ImGuiTreeCtrl::m_nodeIdExt = 0;

    void DrawRowsBackground( int row_count, float line_height, float x1, float x2, float y_offset,
                             ImU32 col_even, ImU32 col_odd )
    {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        float y0 = ImGui::GetCursorScreenPos().y + static_cast<float>( static_cast<int>( y_offset ) );

        int row_display_start;
        int row_display_end;
        ImGui::CalcListClipping( row_count, line_height, &row_display_start, &row_display_end );
        for( int row_n = row_display_start; row_n < row_display_end; row_n++ )
        {
            ImU32 col = ( row_n & 1 ) ? col_odd : col_even;
            if( ( col & IM_COL32_A_MASK ) == 0 )
            {
                continue;
            }
            float y1 = y0 + ( line_height * row_n );
            float y2 = y1 + line_height;
            draw_list->AddRectFilled( ImVec2( x1, y1 ), ImVec2( x2, y2 ), col );
        }
    }

    ImGuiTreeCtrl::ImGuiTreeCtrl()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();

        auto stateListener = factoryManager->make_ptr<StateListener>();
        stateListener->setOwner( this );
        stateContext->addStateListener( stateListener );

        setStateContext( stateContext );
        setStateListener( stateListener );
    }

    ImGuiTreeCtrl::~ImGuiTreeCtrl()
    {
        unload( nullptr );
    }

    void ImGuiTreeCtrl::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loaded );
    }

    void ImGuiTreeCtrl::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                m_treeSelectedNodes.clear();

                if( auto p = getTreeNodesPtr() )
                {
                    auto &treeNodes = *p;
                    for( auto treeNode : treeNodes )
                    {
                        if( treeNode )
                        {
                            treeNode->unload( data );
                        }
                    }

                    setTreeNodesPtr( nullptr );
                }

                m_dragSourceElement = nullptr;
                m_dropDestinationElement = nullptr;
                m_root = nullptr;
                m_treeNodes = nullptr;
                m_selectedTreeNode = nullptr;

                CImGuiElement<IUITreeCtrl>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ImGuiTreeCtrl::addRoot() -> SmartPtr<IUITreeNode>
    {
        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto node = ui->addElementByType<IUITreeNode>();
            m_root = node;

            auto handle = m_root->getHandle();
            if( handle )
            {
                handle->setInstanceId( m_nodeIdExt );
            }

            m_nodeIdExt++;
            return m_root;
        }

        return nullptr;
    }

    auto ImGuiTreeCtrl::addNode() -> SmartPtr<IUITreeNode>
    {
        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto ui = applicationManager->getUI() )
            {
                if( auto node = ui->addElementByType<IUITreeNode>() )
                {
                    node->setOwnerTree( this );
                    addTreeNode( node );

                    if( auto handle = node->getHandle() )
                    {
                        handle->setInstanceId( m_nodeIdExt );
                    }

                    m_nodeIdExt++;
                    node->setTreeNodeId( m_nodeIdExt );

                    return node;
                }

                return nullptr;
            }
        }

        return nullptr;
    }

    void ImGuiTreeCtrl::expand( SmartPtr<IUITreeNode> node )
    {
    }

    void ImGuiTreeCtrl::clear()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        //if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            if( auto root = getRoot() )
            {
                root->removeAllChildren();
                setRoot( nullptr );
            }

            if( auto p = getTreeNodesPtr() )
            {
                auto &treeNodes = *p;
                for( auto &treeNode : treeNodes )
                {
                    if( treeNode )
                    {
                        //treeNode->unload( nullptr );
                        ui->removeElement( treeNode );
                    }
                }

                setTreeNodesPtr( nullptr );
            }
        }
        //else
        //{
        //    auto message = fb::make_ptr<StateMessage>();
        //    message->setType( IUITreeCtrl::clearHash );

        //    if( auto stateContext = getStateContext() )
        //    {
        //        stateContext->addMessage( renderTask, message );
        //    }
        //}
    }

    auto ImGuiTreeCtrl::getTreeNodes() const -> Array<SmartPtr<IUITreeNode>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto p = getTreeNodesPtr() )
        {
            auto &nodes = *p;
            return Array<SmartPtr<IUITreeNode>>( nodes.begin(), nodes.end() );
        }

        return {};
    }

    void ImGuiTreeCtrl::setTreeNodes( Array<SmartPtr<IUITreeNode>> treeNodes )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto p = getTreeNodesPtr() )
        {
            auto &nodes = *p;
            nodes = ConcurrentArray<SmartPtr<IUITreeNode>>( treeNodes.begin(), treeNodes.end() );
        }
        else
        {
            auto nodes = fb::make_shared<ConcurrentArray<SmartPtr<IUITreeNode>>>();
            *nodes = ConcurrentArray<SmartPtr<IUITreeNode>>( treeNodes.begin(), treeNodes.end() );

            setTreeNodesPtr( nodes );
        }
    }

    auto ImGuiTreeCtrl::getSelectTreeNodes() const -> Array<SmartPtr<IUITreeNode>>
    {
        return m_treeSelectedNodes;
    }

    void ImGuiTreeCtrl::setSelectTreeNodes( Array<SmartPtr<IUITreeNode>> treeNodes )
    {
        m_treeSelectedNodes = treeNodes;
    }

    void ImGuiTreeCtrl::addSelectTreeNode( SmartPtr<IUITreeNode> treeNode )
    {
        if( treeNode )
        {
            m_treeSelectedNodes.push_back( treeNode );

            treeNode->setSelected( true );
        }
    }

    void ImGuiTreeCtrl::clearSelectTreeNodes()
    {
        for( auto treeSelectedNode : m_treeSelectedNodes )
        {
            treeSelectedNode->setSelected( false );
        }

        m_treeSelectedNodes.clear();
    }

    auto ImGuiTreeCtrl::getSelectTreeNode() const -> SmartPtr<IUITreeNode>
    {
        return m_selectedTreeNode;
    }

    void ImGuiTreeCtrl::setSelectTreeNode( SmartPtr<IUITreeNode> treeNode )
    {
        if( m_selectedTreeNode != treeNode )
        {
            if( m_selectedTreeNode )
            {
                m_selectedTreeNode->setSelected( false );
            }

            m_selectedTreeNode = treeNode;

            if( m_selectedTreeNode )
            {
                m_selectedTreeNode->setSelected( true );
            }
        }
    }

    auto ImGuiTreeCtrl::getRoot() const -> SmartPtr<IUITreeNode>
    {
        return m_root;
    }

    void ImGuiTreeCtrl::setRoot( SmartPtr<IUITreeNode> val )
    {
        m_root = val;
    }

    auto ImGuiTreeCtrl::isMultiSelect() const -> bool
    {
        return m_multiSelect;
    }

    void ImGuiTreeCtrl::setMultiSelect( bool multiSelect )
    {
        m_multiSelect = multiSelect;
    }

    auto ImGuiTreeCtrl::getDragSourceElement() const -> SmartPtr<IUIElement>
    {
        return m_dragSourceElement;
    }

    void ImGuiTreeCtrl::setDragSourceElement( SmartPtr<IUIElement> dragSource )
    {
        m_dragSourceElement = dragSource;
    }

    auto ImGuiTreeCtrl::getDropDestinationElement() const -> SmartPtr<IUIElement>
    {
        return m_dropDestinationElement;
    }

    void ImGuiTreeCtrl::setDropDestinationElement( SmartPtr<IUIElement> dropDestination )
    {
        m_dropDestinationElement = dropDestination;
    }

    auto ImGuiTreeCtrl::getSelectedSiblingIndex() const -> s32
    {
        return m_siblingIndex;
    }

    void ImGuiTreeCtrl::setSelectedSiblingIndex( s32 siblingIndex )
    {
        m_siblingIndex = siblingIndex;
    }

    void ImGuiTreeCtrl::createElement( SmartPtr<IUIElement> element )
    {
        auto treeCtrl = fb::static_pointer_cast<ImGuiTreeCtrl>( element );
        //treeCtrl->setSiblingIndex( -1 );

        auto root = treeCtrl->getRoot();
        if( root )
        {
            auto label = Util::getText( root );
            if( StringUtil::isNullOrEmpty( label ) )
            {
                label = "Untitled";
            }

            auto nodeFlags = static_cast<s32>( ImGuiTreeNodeFlags_None );
            if( root->isExpanded() )
            {
                nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
            }

            auto handle = root->getHandle();
            auto id = handle->getInstanceId();

            if( ImGui::TreeNodeEx( (void *)static_cast<intptr_t>( id ), nodeFlags, label.c_str() ) )
            {
                if( auto dragSource = treeCtrl->getDragSource() )
                {
                    if( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
                    {
                        auto args = Array<Parameter>();
                        args.resize( 1 );

                        args[0].object = treeCtrl;

                        auto retValue = dragSource->handleEvent( IEvent::Type::UI, IEvent::handleDrag,
                                                                 args, treeCtrl, treeCtrl, nullptr );

                        auto &dataStr = retValue.str;

                        ImGui::SetDragDropPayload( "_TREENODE", dataStr.c_str(), dataStr.size() );
                        ImGui::Text( label.c_str() );
                        ImGui::EndDragDropSource();
                    }
                }

                if( auto dropTarget = treeCtrl->getDropTarget() )
                {
                    if( ImGui::BeginDragDropTarget() )
                    {
                        auto payload = ImGui::AcceptDragDropPayload( "_TREENODE" );
                        if( payload )
                        {
                            auto data =
                                String( static_cast<const char *>( payload->Data ), payload->DataSize );

                            auto args = Array<Parameter>();
                            args.resize( 2 );

                            args[0].object = treeCtrl;
                            args[1].str = data;

                            dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args,
                                                     treeCtrl, treeCtrl, nullptr );
                        }

                        ImGui::EndDragDropTarget();
                    }
                }

                if( ImGui::IsMouseDragging( 0 ) )
                {
                    // Render a preview at the mouse cursor
                }

                const bool is_hovered = ImGui::IsItemHovered();  // Hovered
                const bool is_active = ImGui::IsItemActive();    // Held

                if( is_hovered && ImGui::IsMouseClicked( 0 ) )
                {
                    auto listeners = treeCtrl->getObjectListeners();
                    for( auto listener : listeners )
                    {
                        auto args = Array<Parameter>();
                        args.resize( 1 );

                        args[0].object = root;

                        listener->handleEvent( IEvent::Type::UI, IEvent::handleTreeSelectionActivated,
                                               args, treeCtrl, treeCtrl, nullptr );
                    }
                }

                if( is_hovered && ImGui::IsMouseReleased( 0 ) )
                {
                    auto listeners = treeCtrl->getObjectListeners();
                    for( auto listener : listeners )
                    {
                        auto args = Array<Parameter>();
                        args.resize( 1 );

                        args[0].object = root;

                        listener->handleEvent( IEvent::Type::UI, IEvent::handleTreeSelectionRelease,
                                               args, treeCtrl, treeCtrl, nullptr );
                    }
                }

                float x1 = ImGui::GetCurrentWindow()->WorkRect.Min.x;
                float x2 = ImGui::GetCurrentWindow()->WorkRect.Max.x;
                float item_spacing_y = ImGui::GetStyle().ItemSpacing.y;
                float item_offset_y = -item_spacing_y * 0.5f;
                float line_height = ImGui::GetTextLineHeight() + item_spacing_y;

                auto evenColor = 0;
                auto oddColor = ImGui::GetColorU32( ImVec4( 0.3f, 0.3f, 0.3f, 0.5f ) );
                DrawRowsBackground( treeCtrl->numNodesDisplayed, line_height, x1, x2, item_offset_y,
                                    evenColor, oddColor );

                treeCtrl->numNodesDisplayed = 0;

                auto tree = fb::static_pointer_cast<IUITreeCtrl>( treeCtrl );
                if( auto p = root->getChildren() )
                {
                    auto &children = *p;

                    for( auto &child : children )
                    {
                        if( child->isDerived<IUITreeNode>() )
                        {
                            auto &treeNode = fb::reinterpret_pointer_cast<IUITreeNode>( child );

                            ImGuiTreeNode::createTreeNode( tree, treeNode, treeCtrl->numNodesDisplayed );
                        }
                    }
                }

                ImGui::TreePop();
            }
        }
    }

    void ImGuiTreeCtrl::addTreeNode( SmartPtr<IUITreeNode> node )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto p = getTreeNodesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IUITreeNode>>>();
            p->reserve( 1024 );
            setTreeNodesPtr( p );
        }

        if( p )
        {
            auto &nodes = *p;
            nodes.push_back( node );
        }
    }

    void ImGuiTreeCtrl::removeTreeNode( SmartPtr<IUITreeNode> node )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto p = getTreeNodesPtr() )
        {
            auto &treeNodes = *p;

            auto nodes = Array<SmartPtr<IUITreeNode>>( treeNodes.begin(), treeNodes.end() );
            auto it = std::find( nodes.begin(), nodes.end(), node );
            if( it != nodes.end() )
            {
                nodes.erase( it );
            }

            auto newPtr =
                fb::make_shared<ConcurrentArray<SmartPtr<IUITreeNode>>>( nodes.begin(), nodes.end() );
            setTreeNodesPtr( newPtr );
        }
    }

    auto ImGuiTreeCtrl::getTreeNodesPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IUITreeNode>>>
    {
        return m_treeNodes;
    }

    void ImGuiTreeCtrl::setTreeNodesPtr( SharedPtr<ConcurrentArray<SmartPtr<IUITreeNode>>> ptr )
    {
        m_treeNodes = ptr;
    }

    void ImGuiTreeCtrl::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
        if( message->getType() == IUITreeCtrl::clearHash )
        {
            if( auto owner = (ImGuiTreeCtrl *)getOwner() )
            {
                owner->clear();
            }
        }
    }

    // Callback function for when a drag operation starts
    auto dragStartCallback( ImGuiPayload *payload ) -> bool
    {
        // Payload contains data about the drag operation, such as the node index being dragged
        int nodeIndex = *static_cast<int *>( payload->Data );

        // Here we could do some additional checks to see if we allow the drag operation
        // For example, we could prevent the root node from being moved

        // Store the index of the node being dragged in ImGui's data context
        ImGui::SetDragDropPayload( "Node", &nodeIndex, sizeof( nodeIndex ) );

        return true;
    }

    // Callback function for when a drag operation is released
    auto dragEndCallback( ImGuiPayload *payload ) -> bool
    {
        // Payload contains data about the drag operation, such as the node index being dragged
        int nodeIndex = *static_cast<int *>( payload->Data );

        // Here we could do some additional checks to see if we allow the drop operation
        // For example, we could prevent a node from being dropped onto its own children

        // Get the index of the node being dropped onto, which is stored in ImGui's data context
        int targetIndex = *static_cast<int *>( ImGui::GetDragDropPayload()->Data );

        //// Move the dragged node to the new position by updating its parent and sibling indices
        //Node &node = nodes[nodeIndex];
        //Node &target = nodes[targetIndex];

        //if( node.parent != target.parent )
        //{
        //    // Move the node to a new parent
        //    node.parent = target.parent;
        //    target.parent = nodeIndex;

        //    // Add the node to the target's children
        //    target.children.push_back( nodeIndex );

        //    // Remove the node from its old parent's children
        //    auto it = std::find( nodes[node.parent].children.begin(),
        //                         nodes[node.parent].children.end(), nodeIndex );
        //    if( it != nodes[node.parent].children.end() )
        //    {
        //        nodes[node.parent].children.erase( it );
        //    }
        //}
        //else
        //{
        //    // Move the node within its current parent's children
        //    auto it = std::find( target.children.begin(), target.children.end(), nodeIndex );
        //    IM_ASSERT( it != target.children.end() );
        //    target.children.erase( it );
        //    target.children.insert( it, nodeIndex );
        //}

        return true;
    }

}  // namespace fb::ui
