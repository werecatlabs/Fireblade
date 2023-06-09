#include <GameEditorPCH.h>
#include <ui/PropertiesWindow.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <ui/ActorWindow.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <ui/ResourceDatabaseDialog.h>

namespace fb
{
    namespace editor
    {

        PropertiesWindow::PropertiesWindow()
        {
        }

        PropertiesWindow::~PropertiesWindow()
        {
            unload( nullptr );
        }

        void PropertiesWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
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

                auto applicationManager = core::IApplicationManager::instance();
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
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                if( auto selected = getSelected() )
                {
                    updateSelection( selected );
                }
                else
                {
                    auto selection = selectionManager->getSelection();
                    for( auto object : selection )
                    {
                        updateSelection( object );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PropertiesWindow::updateSelection( SmartPtr<ISharedObject> object )
        {
            auto applicationManager = core::IApplicationManager::instance();
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
                    auto physicsMaterial =
                        fb::dynamic_pointer_cast<physics::IPhysicsMaterial3>( object );

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
                    auto stateObject = fb::dynamic_pointer_cast<IStateContext>( object );

                    auto properties = stateObject->getProperties();
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
                else if( object->isDerived<render::IOverlayElement>() )
                {
                    auto element = fb::dynamic_pointer_cast<render::IOverlayElement>( object );
                    if( auto properties = element->getProperties() )
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
                    else if( ext == fbxExt )
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

        bool PropertiesWindow::isDirty() const
        {
            return m_isDirty;
        }

        void PropertiesWindow::setDirty( bool dirty )
        {
            if( m_isDirty != dirty )
            {
                m_isDirty = dirty;

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto jobQueue = applicationManager->getJobQueue();
                FB_ASSERT( jobQueue );

                auto job = fb::make_ptr<ObjectUpdateJob>();
                job->setOwner( this );
                jobQueue->queueJob( job );
            }
        }

        SmartPtr<ui::IUIPropertyGrid> PropertiesWindow::getPropertyGrid() const
        {
            return m_propertyGrid;
        }

        void PropertiesWindow::setPropertyGrid( SmartPtr<ui::IUIPropertyGrid> val )
        {
            m_propertyGrid = val;
        }

        SmartPtr<ISharedObject> PropertiesWindow::getSelected() const
        {
            return m_selected;
        }

        void PropertiesWindow::setSelected( SmartPtr<ISharedObject> selected )
        {
            m_selected = selected;
        }

        void PropertiesWindow::propertyChange( const String &name, const String &value, bool isButton )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
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
                    auto selection = selectionManager->getSelection();
                    for( auto object : selection )
                    {
                        resourceType = propertyChange( object, name, value, isButton );
                    }
                }

                if( isButton )
                {
                    if( ui )
                    {
                        if( auto resourceDatabaseDialog = ui->getResourceDatabaseDialog() )
                        {
                            resourceDatabaseDialog->setResourceType( resourceType );

                            if( auto selected = getSelected() )
                            {
                                resourceDatabaseDialog->setCurrentObject( selected );
                            }

                            resourceDatabaseDialog->setPropertyName( name );
                            resourceDatabaseDialog->setWindowVisible( true );
                            resourceDatabaseDialog->populate();
                        }
                    }
                }

                setDirty( true );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        IResource::ResourceType PropertiesWindow::propertyChange( SmartPtr<ISharedObject> object,
                                                                  const String &name,
                                                                  const String &value, bool isButton )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

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
                auto stateObject = fb::dynamic_pointer_cast<IStateContext>( object );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
                        }

                        graphicsObject->setProperties( properties );
                    }
                }
                else if( stateObject )
                {
                    auto properties = stateObject->getProperties();
                    if( properties )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
                        }

                        stateObject->setProperties( properties );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                                resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                                resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                                resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                                resourceType = ApplicationUtil::getResourceType( resourceTypeName );
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
                                resourceType = ApplicationUtil::getResourceType( resourceTypeName );
                            }

                            materialTexture->setProperties( properties );
                        }
                    }

                    auto materialPass = fb::static_pointer_cast<render::IMaterialTechnique>(
                        materialTexture->getParent() );
                    if( materialPass )
                    {
                        auto technique = fb::static_pointer_cast<render::IMaterialTechnique>(
                            materialPass->getParent() );
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
                                        resourceType =
                                            ApplicationUtil::getResourceType( resourceTypeName );
                                    }

                                    material->setProperties( properties );
                                }

                                material->save();
                            }
                        }
                        else if( fileExt == ".fbx" )
                        {
                            auto graphicsSystem = applicationManager->getGraphicsSystem();
                            FB_ASSERT( graphicsSystem );

                            auto meshManager = applicationManager->getMeshManager();
                            FB_ASSERT( meshManager );

                            auto meshResource = meshManager->loadFromFile( filePath );
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
                                        resourceType =
                                            ApplicationUtil::getResourceType( resourceTypeName );
                                    }

                                    meshResource->setProperties( properties );
                                }
                            }
                        }
                    }
                }
                else if( object->isDerived<core::IPrototype>() )
                {
                    if( auto properties = state->getProperties() )
                    {
                        properties->setProperty( name, value );

                        if( isButton )
                        {
                            auto &property = properties->getPropertyObject( name );
                            property.setAttribute( "click", "true" );

                            auto resourceTypeName = property.getAttribute( "resourceType" );
                            resourceType = ApplicationUtil::getResourceType( resourceTypeName );
                        }

                        state->setProperties( properties );
                    }
                }
            }

            return resourceType;
        }

        fb::Parameter PropertiesWindow::PropertiesListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
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

            return Parameter();
        }

        PropertiesWindow *PropertiesWindow::PropertiesListener::getOwner() const
        {
            return m_owner;
        }

        void PropertiesWindow::PropertiesListener::setOwner( PropertiesWindow *owner )
        {
            m_owner = owner;
        }

    }  // end namespace editor
}  // end namespace fb
