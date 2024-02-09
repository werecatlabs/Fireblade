#include <GameEditorPCH.h>
#include <ui/PropertiesWindow.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <ui/ActorWindow.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{
    PropertiesWindow::PropertiesWindow() = default;

    PropertiesWindow::~PropertiesWindow()
    {
        unload( nullptr );
    }

    void PropertiesWindow::load( SmartPtr<ISharedObject> data )
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
            parentWindow->setLabel( "PropertiesWindowChild" );
            parentWindow->setSize( Vector2F( 0.0f, 300.0f ) );
            parentWindow->setHasBorder( true );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            m_propertyGrid = ui->addElementByType<ui::IUIPropertyGrid>();
            parentWindow->addChild( m_propertyGrid );

            auto propertiesListener = fb::make_ptr<PropertiesListener>();
            propertiesListener->setOwner( this );
            m_propertyGrid->addObjectListener( propertiesListener );

            auto dropTarget = fb::make_ptr<PropertiesDropTarget>();
            dropTarget->setOwner( this );
            m_propertyGrid->setDropTarget( dropTarget );
            m_dropTarget = dropTarget;

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PropertiesWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            if( m_propertyGrid )
            {
                ui->removeElement( m_propertyGrid );
                m_propertyGrid = nullptr;
            }

            if( auto parentWindow = getParentWindow() )
            {
                ui->removeElement( parentWindow );
                setParentWindow( nullptr );
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PropertiesWindow::update()
    {
        if( isDirty() )
        {
            updateSelection();
            setDirty( false );
        }
    }

    void PropertiesWindow::updateSelection()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto selectionManager = applicationManager->getSelectionManager();
            auto jobQueue = applicationManager->getJobQueue();

            if( auto selected = getSelected() )
            {
                auto job = fb::make_ptr<UpdateSelectionJob>();
                job->setOwner( this );
                job->setObject( selected );
                jobQueue->addJob( job );
            }
            else
            {
                auto selection = selectionManager->getSelection();
                for( auto object : selection )
                {
                    auto job = fb::make_ptr<UpdateSelectionJob>();
                    job->setOwner( this );
                    job->setObject( object );
                    jobQueue->addJob( job );
                }

                setSelection( selection );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PropertiesWindow::updateSelection( SmartPtr<ISharedObject> object )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto materialManager = graphicsSystem->getMaterialManager();

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto meshManager = applicationManager->getMeshManager();

        if( object )
        {
            if( object->isDerived<scene::IActor>() )
            {
                auto actor = fb::dynamic_pointer_cast<scene::IActor>( object );

                auto properties = actor->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<physics::IPhysicsMaterial3>() )
            {
                auto physicsMaterial = fb::dynamic_pointer_cast<physics::IPhysicsMaterial3>( object );

                auto properties = physicsMaterial->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<physics::IPhysicsShape>() )
            {
                auto physicsShape = fb::dynamic_pointer_cast<physics::IPhysicsShape>( object );

                auto properties = physicsShape->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<physics::IPhysicsBody3>() )
            {
                auto physicsBody = fb::dynamic_pointer_cast<physics::IPhysicsBody3>( object );

                auto properties = physicsBody->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<scene::ITransform>() )
            {
                auto transform = fb::dynamic_pointer_cast<scene::ITransform>( object );

                auto properties = transform->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<render::ISceneNode>() )
            {
                auto sceneNode = fb::dynamic_pointer_cast<render::ISceneNode>( object );

                auto properties = sceneNode->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IGraphicsObject>() )
            {
                auto graphicsObject = fb::dynamic_pointer_cast<render::IGraphicsObject>( object );

                auto properties = graphicsObject->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<IStateContext>() )
            {
                auto stateContext = fb::dynamic_pointer_cast<IStateContext>( object );

                auto properties = stateContext->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<IState>() )
            {
                auto state = fb::dynamic_pointer_cast<IState>( object );

                auto properties = state->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<IMeshResource>() )
            {
                auto meshResource = fb::static_pointer_cast<IMeshResource>( object );

                auto properties = meshResource->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IMaterial>() )
            {
                auto material = fb::static_pointer_cast<render::IMaterial>( object );

                auto properties = material->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IMaterialNode>() )
            {
                auto materialNode = fb::static_pointer_cast<render::IMaterialNode>( object );

                auto properties = materialNode->getProperties();
                if( properties )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<scene::IComponent>() )
            {
                auto derived = fb::dynamic_pointer_cast<scene::IComponent>( object );

                if( auto properties = derived->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<scene::IComponentEventListener>() )
            {
                auto derived = fb::dynamic_pointer_cast<scene::IComponentEventListener>( object );

                if( auto properties = derived->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<scene::IDirector>() )
            {
                auto derived = fb::dynamic_pointer_cast<scene::IDirector>( object );

                if( auto properties = derived->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<ui::IUIElement>() )
            {
                auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );

                if( auto properties = element->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IOverlay>() )
            {
                auto element = fb::dynamic_pointer_cast<render::IOverlay>( object );
                if( auto properties = element->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IOverlayElement>() )
            {
                auto element = fb::dynamic_pointer_cast<render::IOverlayElement>( object );
                if( auto properties = element->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<core::IPrototype>() )
            {
                auto prototype = fb::dynamic_pointer_cast<core::IPrototype>( object );
                if( auto properties = prototype->getProperties() )
                {
                    m_propertyGrid->setProperties( properties );
                }
            }
            else if( object->isDerived<FileSelection>() )
            {
                auto fileSelection = fb::dynamic_pointer_cast<FileSelection>( object );

                auto filePath = fileSelection->getFilePath();
                auto ext = Path::getFileExtension( filePath );
                ext = StringUtil::make_lower( ext );

                static const auto materialExt = String( ".mat" );
                static const auto fbxExt = String( ".fbx" );
                static const auto resourceExt = String( ".resource" );

                if( ext == materialExt )
                {
                    auto material = materialManager->loadFromFile( filePath );
                    if( material )
                    {
                        auto properties = material->getProperties();
                        if( properties )
                        {
                            m_propertyGrid->setProperties( properties );
                        }
                    }
                }
                else if( ApplicationUtil::isSupportedMesh( filePath ) )
                {
                    auto mesh = meshManager->loadFromFile( filePath );
                    if( mesh )
                    {
                        auto properties = mesh->getProperties();
                        if( properties )
                        {
                            m_propertyGrid->setProperties( properties );
                        }
                    }
                }
                else if( ext == resourceExt )
                {
                    auto resource = resourceDatabase->loadResource( filePath );
                    if( resource )
                    {
                        auto properties = resource->getProperties();
                        if( properties )
                        {
                            m_propertyGrid->setProperties( properties );
                        }
                    }
                }
            }
        }
    }

    void PropertiesWindow::updateSelectionMT( SmartPtr<ISharedObject> object )
    {
        if( auto propertyGrid = getPropertyGrid() )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto materialManager = graphicsSystem->getMaterialManager();

            auto resourceDatabase = applicationManager->getResourceDatabase();

            auto meshManager = applicationManager->getMeshManager();

            if( object )
            {
                if( object->isDerived<scene::IActor>() )
                {
                    auto actor = fb::dynamic_pointer_cast<scene::IActor>( object );

                    auto properties = actor->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<physics::IPhysicsMaterial3>() )
                {
                    auto physicsMaterial =
                        fb::dynamic_pointer_cast<physics::IPhysicsMaterial3>( object );

                    auto properties = physicsMaterial->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<physics::IPhysicsShape>() )
                {
                    auto physicsShape = fb::dynamic_pointer_cast<physics::IPhysicsShape>( object );

                    auto properties = physicsShape->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<physics::IPhysicsBody3>() )
                {
                    auto physicsBody = fb::dynamic_pointer_cast<physics::IPhysicsBody3>( object );

                    auto properties = physicsBody->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<scene::ITransform>() )
                {
                    auto transform = fb::dynamic_pointer_cast<scene::ITransform>( object );

                    auto properties = transform->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<render::ISceneNode>() )
                {
                    auto sceneNode = fb::dynamic_pointer_cast<render::ISceneNode>( object );

                    auto properties = sceneNode->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<render::IGraphicsObject>() )
                {
                    auto graphicsObject = fb::dynamic_pointer_cast<render::IGraphicsObject>( object );

                    auto properties = graphicsObject->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<IStateContext>() )
                {
                    auto stateContext = fb::dynamic_pointer_cast<IStateContext>( object );

                    auto properties = stateContext->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<IState>() )
                {
                    auto state = fb::dynamic_pointer_cast<IState>( object );

                    auto properties = state->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<IMeshResource>() )
                {
                    auto meshResource = fb::static_pointer_cast<IMeshResource>( object );

                    auto properties = meshResource->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<render::IMaterial>() )
                {
                    auto material = fb::static_pointer_cast<render::IMaterial>( object );

                    auto properties = material->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<render::IMaterialNode>() )
                {
                    auto materialNode = fb::static_pointer_cast<render::IMaterialNode>( object );

                    auto properties = materialNode->getProperties();
                    if( properties )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<scene::IComponent>() )
                {
                    auto derived = fb::dynamic_pointer_cast<scene::IComponent>( object );

                    if( auto properties = derived->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<scene::IComponentEventListener>() )
                {
                    auto derived = fb::dynamic_pointer_cast<scene::IComponentEventListener>( object );

                    if( auto properties = derived->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<scene::IDirector>() )
                {
                    auto derived = fb::dynamic_pointer_cast<scene::IDirector>( object );

                    if( auto properties = derived->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<ui::IUIElement>() )
                {
                    auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );

                    if( auto properties = element->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<render::IOverlay>() )
                {
                    auto element = fb::dynamic_pointer_cast<render::IOverlay>( object );
                    if( auto properties = element->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<render::IOverlayElement>() )
                {
                    auto element = fb::dynamic_pointer_cast<render::IOverlayElement>( object );
                    if( auto properties = element->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<core::IPrototype>() )
                {
                    auto prototype = fb::dynamic_pointer_cast<core::IPrototype>( object );
                    if( auto properties = prototype->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
                else if( object->isDerived<FileSelection>() )
                {
                    auto fileSelection = fb::dynamic_pointer_cast<FileSelection>( object );

                    auto filePath = fileSelection->getFilePath();
                    auto ext = Path::getFileExtension( filePath );
                    ext = StringUtil::make_lower( ext );

                    static const auto materialExt = String( ".mat" );
                    static const auto fbxExt = String( ".fbx" );
                    static const auto resourceExt = String( ".resource" );

                    if( ext == materialExt )
                    {
                        auto material = materialManager->loadFromFile( filePath );
                        if( material )
                        {
                            auto properties = material->getProperties();
                            if( properties )
                            {
                                propertyGrid->setProperties( properties );
                            }
                        }
                    }
                    else if( ApplicationUtil::isSupportedMesh( filePath ) )
                    {
                        auto director = resourceDatabase->loadDirectorFromResourcePath(
                            filePath, scene::MeshResourceDirector::typeInfo() );
                        if( director )
                        {
                            auto properties = director->getProperties();
                            if( properties )
                            {
                                propertyGrid->setProperties( properties );
                            }
                        }
                    }
                    else if( ApplicationUtil::isSupportedTexture( filePath ) )
                    {
                        auto director = resourceDatabase->loadDirectorFromResourcePath(
                            filePath, scene::TextureResourceDirector::typeInfo() );
                        if( director )
                        {
                            auto properties = director->getProperties();
                            if( properties )
                            {
                                propertyGrid->setProperties( properties );
                            }
                        }
                    }
                    else if( ApplicationUtil::isSupportedSound( filePath ) )
                    {
                        auto director = resourceDatabase->loadDirectorFromResourcePath(
                            filePath, scene::SoundResourceDirector::typeInfo() );
                        if( director )
                        {
                            auto properties = director->getProperties();
                            if( properties )
                            {
                                propertyGrid->setProperties( properties );
                            }
                        }
                    }
                    else
                    {
                        auto resource = resourceDatabase->loadResource( filePath );
                        if( resource )
                        {
                            auto properties = resource->getProperties();
                            if( properties )
                            {
                                propertyGrid->setProperties( properties );
                            }
                        }
                    }
                }
                else
                {
                    if( auto properties = object->getProperties() )
                    {
                        propertyGrid->setProperties( properties );
                    }
                }
            }
        }
    }

    auto PropertiesWindow::isDirty() const -> bool
    {
        return m_isDirty;
    }

    void PropertiesWindow::setDirty( bool dirty )
    {
        if( m_isDirty != dirty )
        {
            m_isDirty = dirty;

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto jobQueue = applicationManager->getJobQueue();
            FB_ASSERT( jobQueue );

            auto job = fb::make_ptr<ObjectUpdateJob>();
            job->setOwner( this );
            jobQueue->addJob( job );
        }
    }

    auto PropertiesWindow::getPropertyGrid() const -> SmartPtr<ui::IUIPropertyGrid>
    {
        return m_propertyGrid;
    }

    void PropertiesWindow::setPropertyGrid( SmartPtr<ui::IUIPropertyGrid> val )
    {
        m_propertyGrid = val;
    }

    auto PropertiesWindow::getSelected() const -> SmartPtr<ISharedObject>
    {
        return m_selected;
    }

    void PropertiesWindow::setSelected( SmartPtr<ISharedObject> selected )
    {
        m_selected = selected;
    }

    auto PropertiesWindow::getSelection() const -> Array<SmartPtr<ISharedObject>>
    {
        return m_selection;
    }

    void PropertiesWindow::setSelection( Array<SmartPtr<ISharedObject>> selection )
    {
        m_selection = selection;
    }

    void PropertiesWindow::propertyChange( const String &name, const String &value, bool isButton )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto ui = editorManager->getUI();
            FB_ASSERT( ui );

            auto resourceType = IResource::ResourceType::None;

            if( auto selected = getSelected() )
            {
                resourceType = propertyChange( selected, name, value, isButton );
            }
            else
            {
                auto selection = getSelection();
                for( auto object : selection )
                {
                    resourceType = propertyChange( object, name, value, isButton );
                }
            }

            //if( isButton )
            //{
            //    if( ui )
            //    {
            //        if( auto resourceDatabaseDialog = ui->getResourceDatabaseDialog() )
            //        {
            //            resourceDatabaseDialog->setResourceType( resourceType );

            //            if( auto selected = getSelected() )
            //            {
            //                resourceDatabaseDialog->setCurrentObject( selected );
            //            }

            //            resourceDatabaseDialog->setPropertyName( name );
            //            resourceDatabaseDialog->setWindowVisible( true );
            //            resourceDatabaseDialog->populate();
            //        }
            //    }
            //}

            setDirty( true );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto PropertiesWindow::propertyChange( SmartPtr<ISharedObject> object, const String &name,
                                           const String &value, bool isButton )
        -> IResource::ResourceType
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto application = applicationManager->getApplication();

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto ui = editorManager->getUI();
        FB_ASSERT( ui );

        auto resourceType = IResource::ResourceType::None;

        if( object )
        {
            auto transform = fb::dynamic_pointer_cast<scene::ITransform>( object );
            auto component = fb::dynamic_pointer_cast<scene::IComponent>( object );
            auto sceneNode = fb::dynamic_pointer_cast<render::ISceneNode>( object );
            auto graphicsObject = fb::dynamic_pointer_cast<render::IGraphicsObject>( object );
            auto stateContext = fb::dynamic_pointer_cast<IStateContext>( object );
            auto state = fb::dynamic_pointer_cast<IState>( object );

            if( object->isDerived<scene::IActor>() )
            {
                auto actor = fb::dynamic_pointer_cast<scene::IActor>( object );

                if( auto properties = actor->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    actor->setProperties( properties );
                }
            }
            else if( transform )
            {
                auto properties = transform->getProperties();
                if( properties )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    transform->setProperties( properties );
                }
            }
            else if( component )
            {
                if( auto properties = component->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    component->setProperties( properties );
                }
            }
            else if( sceneNode )
            {
                auto properties = sceneNode->getProperties();
                if( properties )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    sceneNode->setProperties( properties );
                }
            }
            else if( graphicsObject )
            {
                auto properties = graphicsObject->getProperties();
                if( properties )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    graphicsObject->setProperties( properties );
                }
            }
            else if( stateContext )
            {
                auto properties = stateContext->getProperties();
                if( properties )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    stateContext->setProperties( properties );
                }
            }
            else if( state )
            {
                auto properties = state->getProperties();
                if( properties )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    state->setProperties( properties );
                }
            }
            else if( object->isDerived<scene::IComponentEventListener>() )
            {
                auto component = fb::dynamic_pointer_cast<scene::IComponentEventListener>( object );

                if( auto properties = component->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    component->setProperties( properties );
                }
            }
            else if( object->isDerived<physics::IPhysicsBody3>() )
            {
                auto physicsBody = fb::static_pointer_cast<physics::IPhysicsBody3>( object );
                if( physicsBody )
                {
                    auto properties = physicsBody->getProperties();
                    if( properties )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = application->getResourceType( resourceTypeName );
                        }

                        physicsBody->setProperties( properties );
                    }
                }
            }
            else if( object->isDerived<IMeshResource>() )
            {
                auto meshResource = fb::static_pointer_cast<IMeshResource>( object );
                if( meshResource )
                {
                    auto properties = meshResource->getProperties();
                    if( properties )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = application->getResourceType( resourceTypeName );
                        }

                        meshResource->setProperties( properties );
                    }

                    meshResource->save();
                }
            }
            else if( object->isDerived<render::IMaterial>() )
            {
                auto material = fb::static_pointer_cast<render::IMaterial>( object );
                if( material )
                {
                    auto properties = material->getProperties();
                    if( properties )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = application->getResourceType( resourceTypeName );
                        }

                        material->setProperties( properties );
                    }

                    material->save();

                    ui->rebuildActorTree();
                }
            }
            else if( object->isDerived<render::IMaterialPass>() )
            {
                auto materialPass = fb::static_pointer_cast<render::IMaterialPass>( object );
                if( materialPass )
                {
                    auto properties = materialPass->getProperties();
                    if( properties )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = application->getResourceType( resourceTypeName );
                        }

                        materialPass->setProperties( properties );
                    }
                }

                auto technique =
                    fb::static_pointer_cast<render::IMaterialTechnique>( materialPass->getParent() );
                if( technique )
                {
                    auto material = technique->getMaterial();
                    if( material )
                    {
                        material->save();
                    }
                }
            }
            else if( object->isDerived<render::IMaterialTexture>() )
            {
                auto materialTexture = fb::static_pointer_cast<render::IMaterialTexture>( object );
                if( materialTexture )
                {
                    auto properties = materialTexture->getProperties();
                    if( properties )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = application->getResourceType( resourceTypeName );
                        }

                        materialTexture->setProperties( properties );
                    }
                }

                auto materialPass =
                    fb::static_pointer_cast<render::IMaterialTechnique>( materialTexture->getParent() );
                if( materialPass )
                {
                    auto technique =
                        fb::static_pointer_cast<render::IMaterialTechnique>( materialPass->getParent() );
                    if( technique )
                    {
                        auto material = technique->getMaterial();
                        if( material )
                        {
                            material->save();
                        }
                    }
                }
            }
            else if( object->isDerived<FileSelection>() )
            {
                auto fileSelection = fb::static_pointer_cast<FileSelection>( object );
                if( fileSelection )
                {
                    auto filePath = fileSelection->getFilePath();
                    auto fileExt = Path::getFileExtension( filePath );
                    fileExt = StringUtil::make_lower( fileExt );

                    if( fileExt == ".mat" )
                    {
                        auto graphicsSystem = applicationManager->getGraphicsSystem();
                        FB_ASSERT( graphicsSystem );

                        auto materialManager = graphicsSystem->getMaterialManager();
                        FB_ASSERT( materialManager );

                        auto material = materialManager->loadFromFile( filePath );
                        if( material )
                        {
                            auto properties = material->getProperties();
                            if( properties )
                            {
                                properties->setProperty( name, value );

                                if( isButton )
                                {
                                    auto &property = properties->getPropertyObject( name );
                                    property.setAttribute( "click", "true" );

                                    auto resourceTypeName = property.getAttribute( "resourceType" );
                                    resourceType = application->getResourceType( resourceTypeName );
                                }

                                material->setProperties( properties );
                            }

                            material->save();
                        }
                    }
                    else if( ApplicationUtil::isSupportedMesh( filePath ) )
                    {
                        auto meshResource = resourceDatabase->loadDirectorFromResourcePath(
                            filePath, scene::MeshResourceDirector::typeInfo() );
                        if( meshResource )
                        {
                            auto properties = meshResource->getProperties();
                            if( properties )
                            {
                                properties->setProperty( name, value );

                                if( isButton )
                                {
                                    auto &property = properties->getPropertyObject( name );
                                    property.setAttribute( "click", "true" );

                                    auto resourceTypeName = property.getAttribute( "resourceType" );
                                    resourceType = application->getResourceType( resourceTypeName );
                                }

                                meshResource->setProperties( properties );
                                meshResource->save();
                            }
                        }
                    }
                    else if( ApplicationUtil::isSupportedTexture( filePath ) )
                    {
                        auto textureResource = resourceDatabase->loadDirectorFromResourcePath(
                            filePath, scene::TextureResourceDirector::typeInfo() );
                        if( textureResource )
                        {
                            auto properties = textureResource->getProperties();
                            if( properties )
                            {
                                properties->setProperty( name, value );

                                if( isButton )
                                {
                                    auto &property = properties->getPropertyObject( name );
                                    property.setAttribute( "click", "true" );

                                    auto resourceTypeName = property.getAttribute( "resourceType" );
                                    resourceType = application->getResourceType( resourceTypeName );
                                }

                                textureResource->setProperties( properties );
                                textureResource->save();
                            }
                        }
                    }
                    else
                    {
                        auto resource = resourceDatabase->loadResource( filePath );
                        if( resource )
                        {
                            auto properties = resource->getProperties();
                            if( properties )
                            {
                                properties->setProperty( name, value );
                                resource->setProperties( properties );
                            }

                            resource->save();
                        }
                    }
                }
            }
            else if( object->isDerived<core::IPrototype>() )
            {
                auto prototype = fb::dynamic_pointer_cast<core::IPrototype>( object );
                if( auto properties = prototype->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    prototype->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IOverlay>() )
            {
                auto overlay = fb::dynamic_pointer_cast<render::IOverlay>( object );
                if( auto properties = overlay->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    overlay->setProperties( properties );
                }
            }
            else if( object->isDerived<render::IOverlayElement>() )
            {
                auto overlayElement = fb::dynamic_pointer_cast<render::IOverlayElement>( object );
                if( auto properties = overlayElement->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    overlayElement->setProperties( properties );
                }
            }
            else if( object->isDerived<ui::IUIElement>() )
            {
                auto element = fb::dynamic_pointer_cast<ui::IUIElement>( object );
                if( auto properties = element->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    element->setProperties( properties );
                }
            }
            else
            {
                if( auto properties = object->getProperties() )
                {
                    properties->setProperty( name, value );

                    if( isButton )
                    {
                        auto &property = properties->getPropertyObject( name );
                        property.setAttribute( "click", "true" );

                        auto resourceTypeName = property.getAttribute( "resourceType" );
                        resourceType = application->getResourceType( resourceTypeName );
                    }

                    object->setProperties( properties );
                }
            }
        }

        return resourceType;
    }

    auto PropertiesWindow::PropertiesListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                            const Array<Parameter> &arguments,
                                                            SmartPtr<ISharedObject> sender,
                                                            SmartPtr<ISharedObject> object,
                                                            SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handlePropertyButtonClick )
        {
            FB_ASSERT( m_owner );

            auto name = arguments[0].str;
            auto value = arguments[1].str;

            m_owner->propertyChange( name, value, true );
        }
        else if( eventValue == IEvent::handlePropertyChanged )
        {
            FB_ASSERT( m_owner );

            auto name = arguments[0].str;
            auto value = arguments[1].str;

            m_owner->propertyChange( name, value, false );
        }

        return {};
    }

    auto PropertiesWindow::PropertiesListener::getOwner() const -> PropertiesWindow *
    {
        return m_owner;
    }

    void PropertiesWindow::PropertiesListener::setOwner( PropertiesWindow *owner )
    {
        m_owner = owner;
    }

    PropertiesWindow::UpdateSelectionJob::UpdateSelectionJob() = default;

    PropertiesWindow::UpdateSelectionJob::~UpdateSelectionJob() = default;

    void PropertiesWindow::UpdateSelectionJob::execute()
    {
        auto owner = getOwner();
        FB_ASSERT( owner );

        auto object = getObject();
        owner->updateSelectionMT( object );
    }

    auto PropertiesWindow::UpdateSelectionJob::getOwner() const -> SmartPtr<PropertiesWindow>
    {
        auto p = m_owner.load();
        return p.lock();
    }

    void PropertiesWindow::UpdateSelectionJob::setOwner( SmartPtr<PropertiesWindow> owner )
    {
        m_owner = owner;
    }

    auto PropertiesWindow::UpdateSelectionJob::getObject() const -> SmartPtr<ISharedObject>
    {
        auto p = m_object.load();
        return p.lock();
    }

    void PropertiesWindow::UpdateSelectionJob::setObject( SmartPtr<ISharedObject> object )
    {
        m_object = object;
    }

    PropertiesWindow::PropertiesDropTarget::PropertiesDropTarget() = default;

    PropertiesWindow::PropertiesDropTarget::~PropertiesDropTarget() = default;

    auto PropertiesWindow::PropertiesDropTarget::handleEvent( IEvent::Type eventType,
                                                              hash_type eventValue,
                                                              const Array<Parameter> &arguments,
                                                              SmartPtr<ISharedObject> sender,
                                                              SmartPtr<ISharedObject> object,
                                                              SmartPtr<IEvent> event ) -> Parameter
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        if( eventValue == IEvent::handleDrop )
        {
            auto properties = fb::make_ptr<Properties>();
            auto dataStr = arguments[0].getStr();
            auto name = arguments[1].getStr();
            auto value = arguments[2].getStr();

            DataUtil::parse( dataStr, properties.get() );

            auto filePath = properties->getProperty( "filePath" );
            if( !StringUtil::isNullOrEmpty( filePath ) )
            {
                if( auto resource = resourceDatabase->loadResource( filePath ) )
                {
                    auto handle = resource->getHandle();
                    auto uuid = handle->getUUID();

                    m_owner->propertyChange( name, uuid, false );
                }
            }
            else
            {
                auto uuid = properties->getProperty( "actorUUID" );
                m_owner->propertyChange( name, uuid, false );
            }
        }

        return {};
    }

    auto PropertiesWindow::PropertiesDropTarget::getOwner() const -> PropertiesWindow *
    {
        return m_owner;
    }

    void PropertiesWindow::PropertiesDropTarget::setOwner( PropertiesWindow *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
