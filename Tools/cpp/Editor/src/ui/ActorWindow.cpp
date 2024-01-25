#include <GameEditorPCH.h>
#include <ui/ActorWindow.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <editor/EditorManager.h>
#include <ui/EventsWindow.h>
#include <ui/ProjectTreeData.h>
#include <ui/PropertiesWindow.h>
#include <ui/SceneWindow.h>
#include <ui/TransformWindow.h>
#include <ui/UIManager.h>
#include <ui/ObjectBrowserDialog.h>
#include <ui/MaterialWindow.h>
#include <ui/TerrainWindow.h>
#include <FBCore/FBCore.h>

enum
{
    TREE_ITEM_STATE_NOT_FOUND = 0,
    TREE_ITEM_STATE_EXPANDED = 1,
    TREE_ITEM_STATE_NOT_EXPANDED = 2
};

namespace fb::editor
{

    ActorWindow::ActorWindow( SmartPtr<ui::IUIWindow> parent )
    {
        setParent( parent );
    }

    void ActorWindow::setActorName( const String &textStr )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto ui = editorManager->getUI();
        FB_ASSERT( ui );

        auto selection = selectionManager->getSelection();
        for( auto selected : selection )
        {
            if( selected->isDerived<scene::IActor>() )
            {
                auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                if( actor )
                {
                    if( auto handle = actor->getHandle() )
                    {
                        auto name = handle->getName();
                        if( name != textStr )
                        {
                            handle->setName( textStr );

                            if( auto sceneWindow = ui->getSceneWindow() )
                            {
                                sceneWindow->buildTree();
                            }
                        }
                    }
                    else
                    {
                        FB_LOG_ERROR( "No handle" );
                    }
                }
            }
        }
    }

    void ActorWindow::updateObjectSelection( SmartPtr<ISharedObject> object )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto component = fb::dynamic_pointer_cast<scene::IComponent>( object );
        if( component )
        {
            selectionManager->clearSelection();

            selectionManager->addSelectedObject( component );

            if( component->isDerived<scene::Button>() )
            {
                m_objectType = ObjectType::Button;
            }
            else if( component->isDerived<scene::Material>() )
            {
                m_objectType = ObjectType::Material;
            }
            else if( component->isDerived<scene::TerrainSystem>() )
            {
                m_objectType = ObjectType::Terrain;
            }
            else
            {
                m_objectType = ObjectType::None;
            }

            switch( m_objectType )
            {
            case ObjectType::Button:
            {
                m_materialWindow->setWindowVisible( false );
                m_propertiesWindow->setWindowVisible( true );
                m_eventsWindow->setWindowVisible( true );
                m_terrainWindow->setWindowVisible( false );

                m_propertiesWindow->updateSelection();
                m_eventsWindow->updateSelection();
            }
            break;
            case ObjectType::Material:
            {
                m_materialWindow->setWindowVisible( true );
                m_propertiesWindow->setWindowVisible( false );
                m_eventsWindow->setWindowVisible( false );
                m_terrainWindow->setWindowVisible( false );

                m_materialWindow->updateSelection();
            }
            break;
            case ObjectType::Terrain:
            {
                m_materialWindow->setWindowVisible( false );
                m_propertiesWindow->setWindowVisible( true );
                m_eventsWindow->setWindowVisible( false );
                m_terrainWindow->setWindowVisible( true );

                m_terrainWindow->updateSelection();
            }
            break;
            default:
            {
                m_materialWindow->setWindowVisible( false );
                m_propertiesWindow->setWindowVisible( true );
                m_eventsWindow->setWindowVisible( false );
                m_terrainWindow->setWindowVisible( false );

                m_propertiesWindow->updateSelection();
                m_eventsWindow->updateSelection();
            }
            }
        }
        else
        {
            selectionManager->clearSelection();

            selectionManager->addSelectedObject( object );

            m_materialWindow->setWindowVisible( false );
            m_propertiesWindow->setWindowVisible( true );
            m_eventsWindow->setWindowVisible( false );
            m_terrainWindow->setWindowVisible( false );

            m_propertiesWindow->updateSelection();
        }
    }

    ActorWindow::~ActorWindow()
    {
        unload( nullptr );
    }

    void ActorWindow::load( SmartPtr<ISharedObject> data )
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
            parentWindow->setLabel( "ActorWindowChild" );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            auto actorWindow = ui->addElementByType<ui::IUIWindow>();
            actorWindow->setSize( Vector2F( 0.0f, 85.0f ) );
            actorWindow->setHasBorder( true );

            auto componentWindow = ui->addElementByType<ui::IUIWindow>();
            componentWindow->setSize( Vector2F( 0.0f, 120.0f ) );
            componentWindow->setHasBorder( true );

            auto uiListener = fb::make_ptr<UIElementListener>();
            uiListener->setOwner( this );
            m_uiListener = uiListener;

            m_actorEnabled = ui->addElementByType<ui::IUILabelTogglePair>();
            m_actorVisible = ui->addElementByType<ui::IUILabelTogglePair>();
            m_actorStatic = ui->addElementByType<ui::IUILabelTogglePair>();

            m_actorEnabled->setElementId( static_cast<s32>( WidgetId::Enabled ) );
            m_actorEnabled->addObjectListener( uiListener );
            m_actorEnabled->setLabel( "Enabled" );

            m_actorVisible->setSameLine( true );
            m_actorVisible->setElementId( static_cast<s32>( WidgetId::Visible ) );
            m_actorVisible->addObjectListener( uiListener );
            m_actorVisible->setLabel( "Show" );

            m_actorStatic->setSameLine( true );
            m_actorStatic->setElementId( static_cast<s32>( WidgetId::Static ) );
            m_actorStatic->addObjectListener( uiListener );
            m_actorStatic->setLabel( "Static" );

            actorWindow->addChild( m_actorEnabled );
            actorWindow->addChild( m_actorVisible );
            actorWindow->addChild( m_actorStatic );

            auto addComponentButton = ui->addElementByType<ui::IUIButton>();
            FB_ASSERT( addComponentButton );

            addComponentButton->setElementId( static_cast<s32>( WidgetId::AddComponent ) );
            addComponentButton->setLabel( "Add Component" );
            actorWindow->addChild( addComponentButton );
            m_addComponentButton = addComponentButton;
            m_addComponentButton->addObjectListener( uiListener );

            auto removeComponentButton = ui->addElementByType<ui::IUIButton>();
            FB_ASSERT( removeComponentButton );

            removeComponentButton->setSameLine( true );
            removeComponentButton->setElementId( static_cast<s32>( WidgetId::RemoveComponent ) );
            removeComponentButton->setLabel( "Remove Component" );
            actorWindow->addChild( removeComponentButton );
            m_removeComponentButton = removeComponentButton;
            m_removeComponentButton->addObjectListener( uiListener );

            m_actorNamePair = ui->addElementByType<ui::IUILabelTextInputPair>();
            m_actorNamePair->setLabel( "Name" );
            m_actorNamePair->setValue( "Untitled" );
            m_actorNamePair->setElementId( static_cast<s32>( WidgetId::Label ) );
            m_actorNamePair->addObjectListener( uiListener );
            actorWindow->addChild( m_actorNamePair );

            auto tree = ui->addElementByType<ui::IUITreeCtrl>();
            componentWindow->addChild( tree );
            tree->addObjectListener( uiListener );
            m_tree = tree;

            if( actorWindow )
            {
                parentWindow->addChild( actorWindow );
            }

            m_transformWindow = fb::make_ptr<TransformWindow>();
            m_transformWindow->setParent( parentWindow );
            m_transformWindow->load( data );

            if( componentWindow )
            {
                parentWindow->addChild( componentWindow );
            }

            m_propertiesWindow = fb::make_ptr<PropertiesWindow>();
            m_propertiesWindow->setParent( parentWindow );
            m_propertiesWindow->load( data );

            m_eventsWindow = fb::make_ptr<EventsWindow>();
            m_eventsWindow->setParent( parentWindow );
            m_eventsWindow->load( data );

            auto materialWindow = fb::make_ptr<MaterialWindow>( parentWindow );
            materialWindow->load( data );
            materialWindow->setWindowVisible( false );
            m_materialWindow = materialWindow;

            auto terrainWindow = fb::make_ptr<TerrainWindow>();
            terrainWindow->setParent( parentWindow );
            terrainWindow->load( data );
            m_terrainWindow = terrainWindow;

            m_actorWindow = actorWindow;
            m_componentWindow = componentWindow;

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ActorWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            if( m_actorNamePair )
            {
                m_actorNamePair->unload( data );
                m_actorNamePair = nullptr;

                ui->removeElement( m_actorNamePair );
            }

            m_selectedObject = nullptr;
            m_selectedEntity = nullptr;
            //m_parentFilter = nullptr;

            if( m_actorWindow )
            {
                ui->removeElement( m_actorWindow );
                m_actorWindow = nullptr;
            }

            if( m_componentWindow )
            {
                ui->removeElement( m_componentWindow );
                m_componentWindow = nullptr;
            }

            if( auto propertiesWindow = getPropertiesWindow() )
            {
                propertiesWindow->unload( nullptr );
                setPropertiesWindow( nullptr );
            }

            if( auto transformWindow = getTransformWindow() )
            {
                transformWindow->unload( nullptr );
                setTransformWindow( nullptr );
            }

            if( m_terrainWindow )
            {
                m_terrainWindow->unload( nullptr );
                m_terrainWindow = nullptr;
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

            EditorWindow::unload( nullptr );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ActorWindow::staticCheckboxState()
    {
        auto isStatic = false;  // m_staticCheckbox->GetValue();

        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        auto selection = selectionManager->getSelection();
        for( auto selected : selection )
        {
            auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
            if( actor )
            {
                actor->setStatic( isStatic );
            }
        }
    }

    void ActorWindow::visibleCheckboxState()
    {
        auto isVisible = false;  // m_visibleCheckbox->GetValue();

        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        auto selection = selectionManager->getSelection();
        for( auto selected : selection )
        {
            auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
            if( actor )
            {
                actor->setVisible( isVisible );
            }
        }
    }

    void ActorWindow::addComponent()
    {
        // auto window = getWindow();
        // ObjectBrowserDialog dlg(window);
        // auto returnValue = dlg.ShowModal();

        // if (returnValue == wxID_OK)
        //{
        //	auto selectedComponentType = dlg.getSelectedObject();

        //	auto applicationManager = core::ApplicationManager::instance();
        //	auto factoryManager = applicationManager->getFactoryManager();
        //	auto selectionManager = applicationManager->getSelectionManager();

        //	auto selection = selectionManager->getSelected();
        //	for (auto selected: selection)
        //	{
        //		auto actor = fb::dynamic_pointer_cast<IActor>(selected);
        //		if (actor)
        //		{
        //			auto component = factoryManager->createObjectFromType<scene::IComponent>(
        //					selectedComponentType);
        //			actor->addComponentInstance(component);
        //		}
        //	}

        //	buildTree();
        //}
    }

    void ActorWindow::removeComponent()
    {
        // auto selectedItem = m_tree->GetSelection();
        // auto data = (ProjectTreeData*)m_tree->GetItemData(selectedItem);
        // if (data)
        //{
        //	if (data->getObjectType() == "component")
        //	{
        //		auto applicationManager = core::ApplicationManager::instance();
        //		auto selectionManager = applicationManager->getSelectionManager();

        //		auto selection = selectionManager->getSelected();
        //		for (auto selected : selection)
        //		{
        //			auto component = fb::dynamic_pointer_cast<scene::IComponent>(selected);
        //			if (component)
        //			{
        //				//m_selectedObject->removeComponent(component);
        //			}
        //		}

        //		buildTree();
        //	}
        //}
    }

    void ActorWindow::buildTree()
    {
        try
        {
            if( !m_tree )
            {
                return;
            }

            if( m_tree )
            {
                m_tree->clear();
            }

            for( auto data : m_dataArray )
            {
                data->unload( nullptr );
            }

            m_dataArray.clear();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );
            FB_ASSERT( graphicsSystem->isValid() );

            auto materialManager = graphicsSystem->getMaterialManager();
            auto meshManager = applicationManager->getMeshManager();

            FB_ASSERT( materialManager );
            FB_ASSERT( meshManager );

            auto editorManager = EditorManager::getSingletonPtr();
            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto currentScene = sceneManager->getCurrentScene();
            FB_ASSERT( currentScene );

            if( currentScene )
            {
                auto project = editorManager->getProject();
                FB_ASSERT( project );

                auto sceneName = currentScene->getLabel();

                auto selection = selectionManager->getSelection();
                if( selection.size() == 1 )
                {
                    auto selectedObject = selection.front();
                    m_selectedObject = selectedObject;

                    if( selectedObject )
                    {
                        auto actor = SmartPtr<scene::IActor>();
                        if( selectedObject->isDerived<scene::IActor>() )
                        {
                            actor = fb::dynamic_pointer_cast<scene::IActor>( selectedObject );
                        }
                        else if( selectedObject->isDerived<scene::IComponent>() )
                        {
                            auto component =
                                fb::dynamic_pointer_cast<scene::IComponent>( selectedObject );
                            if( component )
                            {
                                actor = component->getActor();
                            }
                        }

                        if( actor )
                        {
                            auto actorName = actor->getName();
                            m_actorNamePair->setValue( actorName );

                            auto isEnabled = actor->isEnabled();
                            m_actorEnabled->setValue( isEnabled );

                            auto isStatic = actor->isStatic();
                            m_actorStatic->setValue( isStatic );

                            auto isVisible = actor->isVisible();
                            m_actorVisible->setValue( isVisible );

                            // ProjectTreeData data("project", "project", project, project);
                            // ProjectTreeData scriptsData(m_defaultMenu, "project", "filter",
                            // project, m_parentFilter);

                            auto rootNode = m_tree->addRoot();
                            FB_ASSERT( rootNode );
                            rootNode->setExpanded( true );

                            addActorToTree( actor, rootNode );
                        }
                    }
                }
                else
                {
                    for( auto selected : selection )
                    {
                        auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                        if( actor )
                        {
                            // addActorToTree(actor, rootId);
                        }
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ActorWindow::addTransformToTree( SmartPtr<scene::ITransform> component, String label,
                                          SmartPtr<ui::IUITreeNode> node )
    {
        // try
        //{
        //	if (component)
        //	{
        //		auto editorManager = EditorManager::getSingletonPtr();

        //		auto project = editorManager->getProject();
        //		FB_ASSERT(project);

        //		ProjectTreeData data("transform", "transform", component, component);

        //		auto treeId = m_tree->AppendItem(rootId, label, -1, -1, new ProjectTreeData(data));
        //	}
        //}
        // catch (std::exception& e)
        //{
        //	FB_LOG_EXCEPTION(e);
        //}
    }

    void ActorWindow::addComponentToTree( SmartPtr<scene::IComponent> component,
                                          SmartPtr<ui::IUITreeNode> node )
    {
        try
        {
            if( component )
            {
                auto editorManager = EditorManager::getSingletonPtr();

                auto project = editorManager->getProject();
                FB_ASSERT( project );

                auto data = SmartPtr<ProjectTreeData>(
                    new ProjectTreeData( "component", "component", component, component ) );

                auto typeinfo = component->getTypeInfo();
                FB_ASSERT( typeinfo );

                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto className = typeManager->getLabel( typeinfo );
                if( StringUtil::isNullOrEmpty( className ) )
                {
                    className = "Untitled";
                }

                auto treeNode = m_tree->addNode();

                FB_ASSERT( treeNode );
                Util::setText( treeNode, className );

                treeNode->setNodeUserData( data );

                if( node )
                {
                    node->addChild( treeNode );
                }

                m_dataArray.emplace_back( data );

                auto subComponentsTreeNode = m_tree->addNode();

                FB_ASSERT( subComponentsTreeNode );
                Util::setText( subComponentsTreeNode, "Sub Components" );

                auto subComponents = component->getSubComponents();
                if( !subComponents.empty() )
                {
                    if( treeNode )
                    {
                        treeNode->addChild( subComponentsTreeNode );
                    }

                    for( auto child : subComponents )
                    {
                        if( child )
                        {
                            addObjectToTree( child, subComponentsTreeNode );
                        }
                    }
                }

                auto childrenTreeNode = m_tree->addNode();

                FB_ASSERT( childrenTreeNode );
                Util::setText( childrenTreeNode, "Children" );

                if( treeNode )
                {
                    treeNode->addChild( childrenTreeNode );
                }

                auto children = component->getChildObjects();
                for( auto child : children )
                {
                    if( child )
                    {
                        addObjectToTree( child, childrenTreeNode );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ActorWindow::addObjectToTree( SmartPtr<ISharedObject> object, SmartPtr<ui::IUITreeNode> node )
    {
        try
        {
            if( object )
            {
                auto editorManager = EditorManager::getSingletonPtr();

                auto project = editorManager->getProject();
                FB_ASSERT( project );

                auto data = SmartPtr<ProjectTreeData>(
                    new ProjectTreeData( "object", "object", object, object ) );

                auto typeinfo = object->getTypeInfo();
                FB_ASSERT( typeinfo != 0 );

                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto className = String( typeManager->getName( typeinfo ) );
                if( StringUtil::isNullOrEmpty( className ) )
                {
                    className = "Untitled";
                }

                if( object->isDerived<physics::IPhysicsShape>() )
                {
                    className = String( "PhysicsShape" );
                }
                else if( object->isDerived<physics::IPhysicsBody3>() )
                {
                    className = String( "PhysicsBody3" );
                }
                else if( object->isDerived<IStateContext>() )
                {
                    className = String( "StateObject" );
                }
                else if( object->isDerived<IStateListener>() )
                {
                    className = String( "StateListener" );
                }
                else if( object->isDerived<render::IGraphicsScene>() )
                {
                    className = String( "SceneManager" );
                }
                else if( object->isDerived<render::ISceneNode>() )
                {
                    className = String( "SceneNode" );
                }
                else if( object->isDerived<render::IGraphicsMesh>() )
                {
                    className = String( "Mesh" );
                }
                else if( object->isDerived<render::IMaterial>() )
                {
                    className = String( "Material" );
                }
                else if( object->isDerived<render::IMaterialTechnique>() )
                {
                    className = String( "Technique" );
                }
                else if( object->isDerived<render::IMaterialPass>() )
                {
                    className = String( "Pass" );
                }
                else if( object->isDerived<render::IMaterialTexture>() )
                {
                    auto textureHandle = object->getHandle();
                    className = textureHandle->getName();
                }
                else if( object->isDerived<ui::IUIImage>() )
                {
                    className = String( "Image" );
                }

                auto treeNode = m_tree->addNode();

                FB_ASSERT( treeNode );
                Util::setText( treeNode, className );

                treeNode->setNodeUserData( data );

                if( node )
                {
                    node->addChild( treeNode );
                }

                auto children = object->getChildObjects();
                for( auto child : children )
                {
                    if( child )
                    {
                        addObjectToTree( child, treeNode );
                    }
                }

                // if (!children.empty())
                //{
                //	m_tree->Expand(treeId);
                // }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ActorWindow::addActorToTree( SmartPtr<scene::IActor> actor,
                                      SmartPtr<ui::IUITreeNode> parentNode )
    {
        try
        {
            // auto actorName = actor->getName();

            // auto node = m_tree->addNode();
            // FB_ASSERT(node);
            // Util::setText(node, actorName);

            // parentNode->addChild(node);

            // auto children = actor->getChildren();
            // for (auto child : children)
            //{
            //	addActorToTree(child, node);
            // }

            // auto editorManager = EditorManager::getSingletonPtr();

            // auto project = editorManager->getProject();
            // FB_ASSERT(project);

            // ProjectTreeData data("actor", "actor", actor, actor);

            // auto actorName = actor->getName();
            // if (StringUtil::isNullOrEmpty(actorName))
            //{
            //	//actorName = "Untitled";
            // }

            ////auto treeId = m_tree->AppendItem(rootId, actorName, -1, -1, new
            /// ProjectTreeData(data));

            // auto transform = actor->getTransform();
            // addTransformToTree(transform, "Transform", rootId);

            auto components = actor->getComponentsByType<scene::IComponent>();
            for( auto component : components )
            {
                addComponentToTree( component, parentNode );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ActorWindow::addResourceToTree( SmartPtr<IResource> resource, SmartPtr<ui::IUITreeNode> node )
    {
        // try
        //{
        //	auto editorManager = EditorManager::getSingletonPtr();

        //	auto project = editorManager->getProject();
        //	FB_ASSERT(project);

        //	ProjectTreeData data("resource", "resource", resource, resource);

        //	auto handle = resource->getHandle();
        //	auto actorName = handle->getName();
        //	if (StringUtil::isNullOrEmpty(actorName))
        //	{
        //		//actorName = "Untitled";
        //	}

        //	auto treeId = m_tree->AppendItem(rootId, actorName, -1, -1, new ProjectTreeData(data));

        //	auto children = resource->getChildObjects();
        //	for (auto child : children)
        //	{
        //		if (child)
        //		{
        //			addObjectToTree(child, treeId);
        //		}
        //	}
        //}
        // catch (std::exception& e)
        //{
        //	FB_LOG_EXCEPTION(e);
        //}
    }

    void ActorWindow::addMaterialToTree( SmartPtr<render::IMaterial> material,
                                         SmartPtr<ui::IUITreeNode> node )
    {
        // try
        //{
        //	auto editorManager = EditorManager::getSingletonPtr();

        //	auto project = editorManager->getProject();
        //	FB_ASSERT(project);

        //	ProjectTreeData data("material", "material", material, material);

        //	auto matreialHandle = material->getHandle();
        //	auto actorName = matreialHandle->getName();
        //	if (StringUtil::isNullOrEmpty(actorName))
        //	{
        //		//actorName = "Untitled";
        //	}

        //	auto treeId = m_tree->AppendItem(rootId, actorName, -1, -1, new ProjectTreeData(data));

        //	addObjectToTree(material, rootId);
        //}
        // catch (std::exception& e)
        //{
        //	FB_LOG_EXCEPTION(e);
        //}
    }

    //
    // void ActorWindow::handleTreeSelectionChanged()
    //{
    //	//try
    //	//{
    //	//	FB_ASSERT(m_tree);

    //	//	if (m_tree->HasFocus())
    //	//	{
    //	//		auto selectedId = event.GetItem();
    //	//		if (!selectedId)
    //	//		{
    //	//			return;
    //	//		}

    //	//		if (selectedId.IsOk())
    //	//		{
    //	//			m_lastSelectedItem = selectedId;
    //	//		}

    //	//		auto applicationManager = core::ApplicationManager::instance();
    //	//		auto selectionManager = applicationManager->getSelectionManager();

    //	//		auto editorManager = EditorManager::getSingletonPtr();
    //	//		auto ui = editorManager->getUI();
    //	//		ui->hideComponentEditWindows();

    //	//		auto data = (ProjectTreeData*)m_tree->GetItemData(selectedId);
    //	//		if (data)
    //	//		{
    //	//			auto pObject = data->getObjectData();
    //	//			auto ownerType = data->getOwnerType();

    //	//			if (ownerType == ("entity"))
    //	//			{
    //	//				SmartPtr<EntityTemplate> entityTemplate = data->getOwnerData();
    //	//				editorManager->getProject()->setSelectedEntityTemplate(entityTemplate);
    //	//			}
    //	//			else if (ownerType == ("mesh"))
    //	//			{
    //	//				SmartPtr<MeshTemplate> meshTemplate = data->getObjectData();

    //	//				SmartPtr<SceneViewManager> meshViewManager =
    // editorManager->getSceneViewManager();
    //	//				meshViewManager->setMeshTemplate(meshTemplate);
    //	//			}
    //	//			else if (ownerType == "actor")
    //	//			{
    //	//				auto object = data->getObjectData();
    //	//				auto pObject = object->getSharedFromThis<ISharedObject>();

    //	//				selectionManager->clearSelection();
    //	//				selectionManager->addSelectedObject(pObject);

    //	//				auto uiManager = editorManager->getUI();
    //	//				if (uiManager)
    //	//				{
    //	//					uiManager->updateActorSelection();
    //	//				}
    //	//			}
    //	//			else if (ownerType == "transform")
    //	//			{
    //	//				auto object = data->getObjectData();
    //	//				auto pObject = object->getSharedFromThis<ISharedObject>();

    //	//				selectionManager->clearSelection();
    //	//				selectionManager->addSelectedObject(pObject);

    //	//				auto uiManager = editorManager->getUI();
    //	//				if (uiManager)
    //	//				{
    //	//					uiManager->updateActorSelection();
    //	//				}
    //	//			}
    //	//			else if (ownerType == "component")
    //	//			{
    //	//				auto object = data->getObjectData();
    //	//				auto pObject = object->getSharedFromThis<ISharedObject>();

    //	//				selectionManager->clearSelection();
    //	//				selectionManager->addSelectedObject(pObject);

    //	//				auto uiManager = editorManager->getUI();
    //	//				if (uiManager)
    //	//				{
    //	//					uiManager->updateComponentSelection();
    //	//				}
    //	//			}
    //	//			else if (pObject->isDerived<render::IMaterial>())
    //	//			{
    //	//				auto object = data->getObjectData();
    //	//				auto pObject = object->getSharedFromThis<ISharedObject>();

    //	//				selectionManager->clearSelection();
    //	//				selectionManager->addSelectedObject(pObject);

    //	//				auto uiManager = editorManager->getUI();
    //	//				if (uiManager)
    //	//				{
    //	//					uiManager->updateActorSelection();
    //	//				}
    //	//			}
    //	//			else if (pObject->isDerived<render::IMaterialNode>())
    //	//			{
    //	//				auto object = data->getObjectData();
    //	//				auto pObject = object->getSharedFromThis<ISharedObject>();

    //	//				selectionManager->clearSelection();
    //	//				selectionManager->addSelectedObject(pObject);

    //	//				auto uiManager = editorManager->getUI();
    //	//				if (uiManager)
    //	//				{
    //	//					uiManager->updateComponentSelection();
    //	//				}
    //	//			}
    //	//			else if (ownerType == "object")
    //	//			{
    //	//				auto object = data->getObjectData();
    //	//				auto pObject = object->getSharedFromThis<ISharedObject>();

    //	//				selectionManager->clearSelection();
    //	//				selectionManager->addSelectedObject(pObject);

    //	//				auto propertiesWindow = ui->getPropertiesWindow();
    //	//				if (propertiesWindow)
    //	//				{
    //	//					propertiesWindow->updateSelection();
    //	//				}
    //	//			}
    //	//			else
    //	//			{
    //	//				auto project = editorManager->getProject();
    //	//				project->setSelectedEntityTemplate(nullptr);
    //	//			}
    //	//		}
    //	//	}
    //	//}
    //	//catch (std::exception& e)
    //	//{
    //	//	wxMessageBox(e.what());
    //	//}
    //}

    //
    // void ActorWindow::handleContextMenu()
    //{
    //	//try
    //	//{
    //	//	m_tree->PopupMenu(m_defaultMenu);
    //	//	event.Skip();
    //	//}
    //	//catch (std::exception& e)
    //	//{
    //	//	wxMessageBox(e.what());
    //	//}
    // }

    void ActorWindow::saveTreeState()
    {
        //// clear map
        // treeState.clear();

        //// reset selected item
        // m_newSelectedItem = NULL;

        // wxTreeItemId itemId = m_tree->GetRootItem();
        // String parent = "";
        // if (itemId)
        //{
        //	saveItemState(parent, itemId);
        // }
    }

    void ActorWindow::restoreTreeState()
    {
    }

    //
    // void ActorWindow::saveItemState(String parent, wxTreeItemId itemId)
    //{
    //	//// make item name
    //	//String itemName = String(m_tree->GetItemText(itemId));
    //	//if (parent != "")
    //	//	itemName = parent + "/" + itemName;

    //	//// get expanded state
    //	//bool isExpanded = false;
    //	//if (m_tree->ItemHasChildren(itemId))
    //	//	isExpanded = m_tree->IsExpanded(itemId);

    //	////get selected state
    //	//if (m_tree->IsSelected(itemId))
    //	//	m_newSelectedItem = itemId;

    //	//// add item to map
    //	//treeState.insert(std::map<String, bool>::value_type(itemName, isExpanded));

    //	//// parse childes
    //	//wxTreeItemIdValue cookie;
    //	//wxTreeItemId childrenItem = m_tree->GetFirstChild(itemId, cookie);
    //	//while (childrenItem.IsOk())
    //	//{
    //	//	saveItemState(itemName, childrenItem);
    //	//	childrenItem = m_tree->GetNextChild(itemId, cookie);
    //	//}
    //}

    //
    // void ActorWindow::restoreTreeState()
    //{
    //	//wxTreeItemId itemId = m_tree->GetRootItem();

    //	//// restore items
    //	//String parent = "";
    //	//if (itemId)
    //	//	restoreItemState(parent, itemId, false);

    //	//if (m_newSelectedItem)
    //	//{
    //	//	m_tree->EnsureVisible(m_newSelectedItem);
    //	//	m_tree->SelectItem(m_newSelectedItem);
    //	//}
    //}

    void ActorWindow::updateSelection()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto selectionManager = applicationManager->getSelectionManager();

            auto selection = selectionManager->getSelection();
            for( auto object : selection )
            {
                if( object )
                {
                    if( object->isDerived<scene::IActor>() )
                    {
                        auto actor = fb::dynamic_pointer_cast<scene::IActor>( object );
                        auto transform = actor->getTransform();

                        if( m_transformWindow )
                        {
                            m_transformWindow->setTransform( transform );
                            m_transformWindow->updateSelection();
                        }
                    }

                    // auto component = fb::dynamic_pointer_cast<scene::IComponent>(object);
                    // auto resource = fb::dynamic_pointer_cast<IResource>(object);
                    // auto fileSelection = fb::dynamic_pointer_cast<FileSelection>(object);

                    // ActorWindow::ObjectType objectType = ActorWindow::ObjectType::ACTOR;

                    // if (actor)
                    //{
                    //	objectType = ActorWindow::ObjectType::ACTOR;
                    // }
                    // else if (fileSelection)
                    //{
                    //	auto filePath = fileSelection->getFilePath();
                    //	auto ext = Path::getFileExtension(filePath);
                    //	ext = StringUtil::make_lower(ext);

                    //	if (ext == ".fbx")
                    //	{
                    //		objectType = ActorWindow::ObjectType::MESH;
                    //	}
                    //	else
                    //	{
                    //		objectType = ActorWindow::ObjectType::MATERIAL;
                    //	}
                    //}

                    // m_objectType = objectType;

                    m_eventsWindow->updateSelection();

                    m_terrainWindow->setWindowVisible( false );
                    m_eventsWindow->setWindowVisible( true );
                }
            }

            buildTree();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ActorWindow::getTransformWindow() const -> SmartPtr<TransformWindow>
    {
        return m_transformWindow;
    }

    void ActorWindow::setTransformWindow( SmartPtr<TransformWindow> transformWindow )
    {
        m_transformWindow = transformWindow;
    }

    auto ActorWindow::getPropertiesWindow() const -> SmartPtr<PropertiesWindow>
    {
        return m_propertiesWindow;
    }

    void ActorWindow::setPropertiesWindow( SmartPtr<PropertiesWindow> propertiesWindow )
    {
        m_propertiesWindow = propertiesWindow;
    }

    ActorWindow::UIElementListener::UIElementListener() = default;

    ActorWindow::UIElementListener::~UIElementListener() = default;

    auto ActorWindow::UIElementListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                      const Array<Parameter> &arguments,
                                                      SmartPtr<ISharedObject> sender,
                                                      SmartPtr<ISharedObject> object,
                                                      SmartPtr<IEvent> event ) -> Parameter
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto element = fb::dynamic_pointer_cast<ui::IUIElement>( sender );
        auto elementId = static_cast<WidgetId>( element->getElementId() );

        if( eventValue == IEvent::handleSelection )
        {
            switch( elementId )
            {
            case WidgetId::AddComponent:
            {
                auto editorManager = EditorManager::getSingletonPtr();
                auto ui = editorManager->getUI();

                auto objectBrowserDialog = ui->getObjectBrowserDialog();
                objectBrowserDialog->setWindowVisible( true );
            }
            break;
            case WidgetId::RemoveComponent:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                auto actor = SmartPtr<scene::IActor>();

                auto selection = selectionManager->getSelection();
                for( auto selected : selection )
                {
                    if( selected->isDerived<scene::IComponent>() )
                    {
                        auto component = fb::static_pointer_cast<scene::IComponent>( selected );
                        actor = component->getActor();

                        if( actor )
                        {
                            actor->removeComponentInstance( component );
                        }
                    }
                }

                selectionManager->clearSelection();
                selectionManager->addSelectedObject( actor );

                m_owner->buildTree();
            }
            break;
            }
        }
        else if( eventValue == IEvent::handleValueChanged )
        {
            switch( elementId )
            {
            case WidgetId::Label:
            {
                auto namePair = m_owner->m_actorNamePair;
                auto actorName = namePair->getValue();

                auto selection = selectionManager->getSelection();
                for( auto selected : selection )
                {
                    if( selected->isDerived<scene::IActor>() )
                    {
                        auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                        actor->setName( actorName );
                    }
                }

                auto editorManager = EditorManager::getSingletonPtr();
                auto ui = editorManager->getUI();
                ui->rebuildSceneTree();
            }
            break;
            case WidgetId::Enabled:
            {
                auto toggle = m_owner->m_actorEnabled;
                auto value = toggle->getValue();

                auto selection = selectionManager->getSelection();
                for( auto selected : selection )
                {
                    if( selected->isDerived<scene::IActor>() )
                    {
                        auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                        actor->setEnabled( value );
                    }
                }
            }
            break;
            case WidgetId::Visible:
            {
                auto toggle = m_owner->m_actorVisible;
                auto value = toggle->getValue();

                auto selection = selectionManager->getSelection();
                for( auto selected : selection )
                {
                    if( selected->isDerived<scene::IActor>() )
                    {
                        auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                        actor->setVisible( value );
                    }
                }
            }
            break;
            case WidgetId::Static:
            {
                auto toggle = m_owner->m_actorStatic;
                auto value = toggle->getValue();

                auto selection = selectionManager->getSelection();
                for( auto selected : selection )
                {
                    if( selected->isDerived<scene::IActor>() )
                    {
                        auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                        actor->setStatic( value );
                    }
                }
            }
            break;
            default:
            {
            }
            break;
            }
        }
        else if( eventValue == IEvent::handleTreeSelectionActivated )
        {
            auto node = fb::static_pointer_cast<ui::IUITreeNode>( arguments[0].object );
            auto data = fb::static_pointer_cast<ProjectTreeData>( node->getNodeUserData() );

            if( data )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                selectionManager->clearSelection();

                auto object = data->getObjectData();
                if( object )
                {
                    m_owner->updateObjectSelection( object );
                }
            }
        }

        return {};
    }

    auto ActorWindow::UIElementListener::getOwner() const -> ActorWindow *
    {
        return m_owner;
    }

    void ActorWindow::UIElementListener::setOwner( ActorWindow *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
