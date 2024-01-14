#include <GameEditorPCH.h>
#include <ui/ResourceDatabaseDialog.h>
#include <ui/ProjectTreeData.h>
#include <editor/EditorManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{
    ResourceDatabaseDialog::ResourceDatabaseDialog() = default;

    ResourceDatabaseDialog::~ResourceDatabaseDialog()
    {
        unload( nullptr );
    }

    void ResourceDatabaseDialog::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto parent = getParent();

            auto parentWindow = ui->addElementByType<ui::IUIWindow>();
            FB_ASSERT( parentWindow );

            setParentWindow( parentWindow );
            parentWindow->setLabel( "ResourceDatabaseDialog" );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            m_buttonWindow = ui->addElementByType<ui::IUIWindow>();
            parentWindow->addChild( m_buttonWindow );

            m_buttonWindow->setSize( Vector2F( 0.0f, 50.0f ) );

            m_treeWindow = ui->addElementByType<ui::IUIWindow>();
            parentWindow->addChild( m_treeWindow );

            auto uiListener = fb::make_ptr<UIElementListener>();
            uiListener->setOwner( this );
            m_uiListener = uiListener;

            auto tree = ui->addElementByType<ui::IUITreeCtrl>();
            m_treeWindow->addChild( tree );
            tree->setMultiSelect( false );
            tree->addObjectListener( uiListener );
            tree->setElementId( Tree );
            m_tree = tree;

            auto addComponentButton = ui->addElementByType<ui::IUIButton>();
            FB_ASSERT( addComponentButton );

            addComponentButton->setLabel( "Refresh" );
            m_buttonWindow->addChild( addComponentButton );
            addComponentButton->addObjectListener( uiListener );
            addComponentButton->setElementId( AddComponent );
            m_addComponentButton = addComponentButton;

            auto selectButton = ui->addElementByType<ui::IUIButton>();
            FB_ASSERT( selectButton );

            selectButton->setSameLine( true );
            selectButton->setLabel( "Select" );
            m_buttonWindow->addChild( selectButton );
            selectButton->addObjectListener( uiListener );
            selectButton->setElementId( Select );
            m_selectButton = selectButton;

            auto dragSource = fb::make_ptr<DragSource>();
            dragSource->setOwner( this );
            m_tree->setDragSource( dragSource );

            populate();

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ResourceDatabaseDialog::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            if( m_tree )
            {
                if( m_uiListener )
                {
                    m_tree->removeObjectListener( m_uiListener );
                    m_uiListener = nullptr;
                }

                m_tree->clear();
                m_treeNodes.clear();

                ui->removeElement( m_tree );
                m_tree = nullptr;
            }

            if( m_addComponentButton )
            {
                ui->removeElement( m_addComponentButton );
                m_addComponentButton = nullptr;
            }

            if( m_selectButton )
            {
                ui->removeElement( m_selectButton );
                m_selectButton = nullptr;
            }

            if( m_buttonWindow )
            {
                ui->removeElement( m_buttonWindow );
                m_buttonWindow = nullptr;
            }

            if( m_uiListener )
            {
                m_uiListener->unload( nullptr );
                m_uiListener = nullptr;
            }

            if( m_treeWindow )
            {
                ui->removeElement( m_treeWindow );
                m_treeWindow = nullptr;
            }

            if( auto parentWindow = getParentWindow() )
            {
                ui->removeElement( parentWindow );
                setParentWindow( nullptr );
            }

            for( auto data : m_dataArray )
            {
                data->unload( nullptr );
            }

            m_dataArray.clear();

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ResourceDatabaseDialog::addResourceToTree( SmartPtr<ui::IUITreeNode> rootNode,
                                                    SmartPtr<scene::IDirector> resource )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        auto resourceType = getResourceType();

        FB_ASSERT( resource->isDerived<scene::IDirector>() );

        auto handle = resource->getHandle();
        auto name = handle->getName();

        auto uuid = handle->getUUID();
        //FB_ASSERT( !StringUtil::isNullOrEmpty( uuid ) );

        auto label = Path::getFileName( name );

        auto treeNode = m_tree->addNode();
        FB_ASSERT( treeNode );

        switch( resourceType )
        {
        case IResource::ResourceType::None:
        {
            Util::setText( treeNode, label );

            auto data =
                factoryManager->make_ptr<ProjectTreeData>( "resource", "resource", resource, resource );
            treeNode->setNodeUserData( data );

            rootNode->addChild( treeNode );

            m_dataArray.emplace_back( data );
        }
        break;
        case IResource::ResourceType::Material:
        {
            if( auto resourceProperties = resource->getProperties() )
            {
                String value;
                resourceProperties->getPropertyValue( "type", value );

                if( value == "Material" )
                {
                    Util::setText( treeNode, label );

                    auto data = factoryManager->make_ptr<ProjectTreeData>( "resource", "resource",
                                                                           resource, resource );
                    treeNode->setNodeUserData( data );

                    rootNode->addChild( treeNode );

                    m_dataArray.emplace_back( data );
                }
            }
        }
        break;
        case IResource::ResourceType::Object:
        {
        }
        break;
        case IResource::ResourceType::Actor:
        {
            if( resource->isDerived<scene::IActor>() )
            {
                Util::setText( treeNode, name );

                auto data = factoryManager->make_ptr<ProjectTreeData>( "resource", "resource", resource,
                                                                       resource );
                treeNode->setNodeUserData( data );

                rootNode->addChild( treeNode );

                m_dataArray.emplace_back( data );
            }
        }
        break;
        case IResource::ResourceType::Component:
        {
            if( resource->isDerived<scene::IComponent>() )
            {
                Util::setText( treeNode, name );

                auto data = factoryManager->make_ptr<ProjectTreeData>( "resource", "resource", resource,
                                                                       resource );
                treeNode->setNodeUserData( data );

                rootNode->addChild( treeNode );

                m_dataArray.emplace_back( data );
            }

            if( auto resourceProperties = resource->getProperties() )
            {
                String value;
                resourceProperties->getPropertyValue( "type", value );

                if( value == "Component" )
                {
                    Util::setText( treeNode, name );

                    auto data = factoryManager->make_ptr<ProjectTreeData>( "resource", "resource",
                                                                           resource, resource );
                    treeNode->setNodeUserData( data );

                    rootNode->addChild( treeNode );

                    m_dataArray.emplace_back( data );
                }
            }
        }
        break;
        case IResource::ResourceType::Texture:
        {
            if( auto resourceProperties = resource->getProperties() )
            {
                String value;
                resourceProperties->getPropertyValue( "type", value );

                if( value == "Texture" )
                {
                    Util::setText( treeNode, label );

                    auto imageElement = ui->addElementByType<ui::IUIImage>();
                    FB_ASSERT( imageElement );

                    auto data = factoryManager->make_ptr<ProjectTreeData>( "resource", "resource",
                                                                           resource, resource );
                    treeNode->setNodeUserData( data );

                    rootNode->addChild( treeNode );

                    m_dataArray.emplace_back( data );
                }
            }
        }
        break;
        default:
        {
        }
        }
    }

    void ResourceDatabaseDialog::populate()
    {
        if( !m_tree )
        {
            return;
        }

        if( m_tree )
        {
            m_tree->clear();
        }

        m_treeNodes.clear();

        for( auto data : m_dataArray )
        {
            data->unload( nullptr );
        }

        m_dataArray.clear();

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto rootNode = m_tree->addRoot();
        FB_ASSERT( rootNode );
        rootNode->setExpanded( true );

        auto resourceType = getResourceType();

        auto resources = resourceDatabase->getResourceData();
        for( auto resource : resources )
        {
            auto parent = rootNode;

            auto handle = resource->getHandle();
            auto name = handle->getName();

            auto path = Path::getFilePath( name );
            if( !StringUtil::isNullOrEmpty( path ) )
            {
                auto it = m_treeNodes.find( path );
                if( it != m_treeNodes.end() )
                {
                    parent = it->second;
                }
                else
                {
                    //parent = m_tree->addNode();
                    //FB_ASSERT( parent );

                    //Util::setText( parent, path );

                    //auto data = factoryManager->make_ptr<ProjectTreeData>( "resource", "resource",
                    //                                                       nullptr, nullptr );
                    //parent->setNodeUserData( data );

                    //rootNode->addChild( parent );

                    //m_treeNodes[path] = parent;

                    auto splitPaths = StringUtil::split( path, "/" );

                    auto previousNode = SmartPtr<ui::IUITreeNode>();
                    auto currentPath = String();
                    for( auto &splitPath : splitPaths )
                    {
                        if( splitPath == "." || splitPath == ".." )
                        {
                            continue;
                        }

                        if( !currentPath.empty() )
                        {
                            currentPath += "/";
                        }

                        currentPath += splitPath;

                        auto it = m_treeNodes.find( currentPath );
                        if( it == m_treeNodes.end() )
                        {
                            auto splitNode = m_tree->addNode();
                            FB_ASSERT( splitNode );

                            Util::setText( splitNode, splitPath );

                            auto data = factoryManager->make_ptr<ProjectTreeData>(
                                "resource", "resource", nullptr, nullptr );
                            splitNode->setNodeUserData( data );

                            if( !previousNode )
                            {
                                rootNode->addChild( splitNode );
                            }
                            else
                            {
                                previousNode->addChild( splitNode );
                            }

                            m_treeNodes[currentPath] = splitNode;

                            previousNode = splitNode;
                        }
                        else
                        {
                            previousNode = it->second;
                        }
                    }

                    auto it = m_treeNodes.find( currentPath );
                    if( it != m_treeNodes.end() )
                    {
                        parent = it->second;
                    }
                }
            }

            addResourceToTree( parent, resource );
        }
    }

    void ResourceDatabaseDialog::select()
    {
        FB_ASSERT( m_tree );

        auto treeNode = m_tree->getSelectTreeNode();
        if( treeNode )
        {
            auto data = treeNode->getNodeUserData();
            if( data )
            {
                auto uuid = String();

                auto projectData = fb::static_pointer_cast<ProjectTreeData>( data );
                if( auto resourceObject = projectData->getObjectData() )
                {
                    if( auto handle = resourceObject->getHandle() )
                    {
                        uuid = handle->getUUID();
                    }

                    if( StringUtil::isNullOrEmpty( uuid ) )
                    {
                        if( resourceObject->isDerived<scene::IDirector>() )
                        {
                            auto director = fb::static_pointer_cast<scene::IDirector>( resourceObject );
                            auto directorProperties = director->getProperties();
                            directorProperties->getPropertyValue( "uuid", uuid );
                        }
                    }
                }

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                if( auto selected = getCurrentObject() )
                {
                    selectObject( selected, uuid );
                }
                else
                {
                    auto selection = selectionManager->getSelection();
                    for( auto selected : selection )
                    {
                        selectObject( selected, uuid );
                    }
                }
            }
        }
    }

    void ResourceDatabaseDialog::selectObject( SmartPtr<ISharedObject> selected, const String &uuid )
    {
        //FB_ASSERT(selected->isDerived<core::IPrototype>());

        if( selected->isDerived<scene::Material>() )
        {
            auto node = fb::static_pointer_cast<scene::Material>( selected );

            auto properties = node->getProperties();
            if( properties )
            {
                properties->setProperty( m_propertyName, uuid );
                node->setProperties( properties );
            }
        }
        else if( selected->isDerived<render::IMaterialNode>() )
        {
            auto node = fb::static_pointer_cast<render::IMaterialNode>( selected );

            auto properties = node->getProperties();
            if( properties )
            {
                properties->setProperty( m_propertyName, uuid );
                node->setProperties( properties );
            }

            auto material = node->getMaterial();
            if( material )
            {
                material->save();
            }
        }
        else if( selected->isDerived<IResource>() )
        {
            auto resource = fb::static_pointer_cast<IResource>( selected );

            auto properties = resource->getProperties();
            if( properties )
            {
                properties->setProperty( m_propertyName, uuid );
                resource->setProperties( properties );
            }
        }
        else if( selected->isDerived<core::IPrototype>() )
        {
            auto prototype = fb::static_pointer_cast<core::IPrototype>( selected );

            auto properties = prototype->getProperties();
            if( properties )
            {
                properties->setProperty( m_propertyName, uuid );
                prototype->setProperties( properties );
            }
        }
        else if( selected->isDerived<scene::IComponentEventListener>() )
        {
            auto prototype = fb::static_pointer_cast<scene::IComponentEventListener>( selected );

            auto properties = prototype->getProperties();
            if( properties )
            {
                properties->setProperty( m_propertyName, uuid );
                prototype->setProperties( properties );
            }
        }
    }

    auto ResourceDatabaseDialog::getSelectedObject() const -> String
    {
        return m_selectedObject;
    }

    void ResourceDatabaseDialog::setSelectedObject( const String &selectedObject )
    {
        m_selectedObject = selectedObject;
    }

    auto ResourceDatabaseDialog::getTree() const -> SmartPtr<ui::IUITreeCtrl>
    {
        return m_tree;
    }

    void ResourceDatabaseDialog::setTree( SmartPtr<ui::IUITreeCtrl> tree )
    {
        m_tree = tree;
    }

    auto ResourceDatabaseDialog::getCurrentObject() const -> SmartPtr<ISharedObject>
    {
        return m_currentObject;
    }

    void ResourceDatabaseDialog::setCurrentObject( SmartPtr<ISharedObject> currentObject )
    {
        m_currentObject = currentObject;
    }

    auto ResourceDatabaseDialog::getPropertyName() const -> String
    {
        return m_propertyName;
    }

    void ResourceDatabaseDialog::setPropertyName( const String &propertyName )
    {
        m_propertyName = propertyName;
    }

    auto ResourceDatabaseDialog::getResourceType() const -> IResource::ResourceType
    {
        return m_resourceType;
    }

    void ResourceDatabaseDialog::setResourceType( IResource::ResourceType resourceType )
    {
        m_resourceType = resourceType;
    }

    void ResourceDatabaseDialog::handleTreeSelectionChanged()
    {
        // auto selectedId = event.GetItem();
        // auto data = (ProjectTreeData*)m_tree->GetItemData(selectedId);
        // if (data)
        //{
        //	auto factory = fb::static_pointer_cast<IFactory>(data->getOwnerData());
        //	auto selectedObject = factory->getObjectType();
        //	setSelectedObject(selectedObject);
        // }
    }

    void ResourceDatabaseDialog::handleTreeSelectionActivated()
    {
        // auto selectedId = event.GetItem();
        // auto data = (ProjectTreeData*)m_tree->GetItemData(selectedId);
        // if (data)
        //{
        //	auto factory = fb::static_pointer_cast<IFactory>(data->getOwnerData());
        //	auto selectedObject = factory->getObjectType();
        //	setSelectedObject(selectedObject);
        // }

        // EndModal(wxID_OK);
    }

    auto ResourceDatabaseDialog::UIElementListener::handleEvent( IEvent::Type eventType,
                                                                 hash_type eventValue,
                                                                 const Array<Parameter> &arguments,
                                                                 SmartPtr<ISharedObject> sender,
                                                                 SmartPtr<ISharedObject> object,
                                                                 SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleSelection )
        {
            auto owner = getOwner();
            auto tree = owner->getTree();
            auto selectedNode = tree->getSelectTreeNode();

            auto element = fb::dynamic_pointer_cast<ui::IUIElement>( sender );
            auto elementId = static_cast<WidgetId>( element->getElementId() );
            switch( elementId )
            {
            case AddComponent:
            {
                owner->populate();
            }
            break;
            case Select:
            {
                owner->select();
            }
            break;
            }
        }

        return {};
    }

    auto ResourceDatabaseDialog::UIElementListener::getOwner() const -> ResourceDatabaseDialog *
    {
        return m_owner;
    }

    void ResourceDatabaseDialog::UIElementListener::setOwner( ResourceDatabaseDialog *owner )
    {
        m_owner = owner;
    }

    auto ResourceDatabaseDialog::DragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                          const Array<Parameter> &arguments,
                                                          SmartPtr<ISharedObject> sender,
                                                          SmartPtr<ISharedObject> object,
                                                          SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleDrag )
        {
            auto dataStr = handleDrag( Vector2I::zero(), sender );
            return Parameter( dataStr );
        }

        return {};
    }

    auto ResourceDatabaseDialog::DragSource::handleDrag( const Vector2I &position,
                                                         SmartPtr<ui::IUIElement> element ) -> String
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        if( element->isDerived<ui::IUITreeNode>() )
        {
            auto treeNode = fb::static_pointer_cast<ui::IUITreeNode>( element );
            auto text = Util::getText( treeNode );

            auto userData = treeNode->getNodeUserData();
            if( userData )
            {
                auto projectTreeData = fb::static_pointer_cast<ProjectTreeData>( userData );
                FB_ASSERT( projectTreeData );

                auto data = fb::make_ptr<Properties>();

                data->setProperty( "sourceId", treeNode->getTreeNodeId() );

                auto treeData = fb::static_pointer_cast<ProjectTreeData>( treeNode->getNodeUserData() );
                auto actor = treeData->getObjectData();

                if( actor )
                {
                    if( auto handle = actor->getHandle() )
                    {
                        auto uuid = handle->getUUID();
                        data->setProperty( "resourceId", handle->getInstanceId() );
                        data->setProperty( "resourceUUID", uuid );
                    }
                }

                return DataUtil::toString( data.get(), true );
            }
        }

        return {};
    }

    auto ResourceDatabaseDialog::DragSource::getOwner() const -> ResourceDatabaseDialog *
    {
        return m_owner;
    }

    void ResourceDatabaseDialog::DragSource::setOwner( ResourceDatabaseDialog *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
