#include <FBCore/FBCorePCH.h>
#include <FBCore/ApplicationUtil.h>
#include <FBCore/FBCoreHeaders.h>

namespace fb
{

    auto ApplicationUtil::loadMesh( const String &filePath ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        if( prefabManager )
        {
            auto prefab = prefabManager->loadPrefab( filePath );
            if( prefab )
            {
                return prefab->createActor();
            }
        }

        return nullptr;
    }

    auto ApplicationUtil::addMenuItem( SmartPtr<ui::IUIMenu> menu, s32 itemid, const String &text,
                                       const String &help, ui::IUIMenuItem::Type type )
        -> SmartPtr<ui::IUIMenuItem>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        auto menuItem = ui->addElementByType<ui::IUIMenuItem>();
        FB_ASSERT( menuItem );

        menuItem->setElementId( itemid );
        menuItem->setText( text );
        menuItem->setHelp( help );
        menuItem->setMenuItemType( type );

        menu->addMenuItem( menuItem );

        return menuItem;
    }

    auto ApplicationUtil::addMenuSeparator( SmartPtr<ui::IUIMenu> menu ) -> SmartPtr<ui::IUIMenuItem>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        auto menuItem = ui->addElementByType<ui::IUIMenuItem>();
        FB_ASSERT( menuItem );

        auto type = ui::IUIMenuItem::Type::Separator;
        menuItem->setMenuItemType( type );

        menu->addMenuItem( menuItem );

        return menuItem;
    }

    auto ApplicationUtil::setText( SmartPtr<ui::IUITreeNode> node, const String &text )
        -> SmartPtr<ui::IUIElement>
    {
        if( node )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto ui = applicationManager->getUI() )
            {
                if( auto textElement = ui->addElementByType<ui::IUIText>() )
                {
                    textElement->setText( text );
                    node->addChild( textElement );

                    return textElement;
                }
            }
        }

        return nullptr;
    }

    auto ApplicationUtil::setImage( SmartPtr<ui::IUITreeNode> node, const String &imagePath )
        -> SmartPtr<ui::IUIElement>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto ui = applicationManager->getUI();
        FB_ASSERT( ui );

        auto imageElement = ui->addElementByType<ui::IUIImage>();
        FB_ASSERT( imageElement );

        //textElement->setText( text );
        node->setNodeData( imageElement );

        return imageElement;
    }

    auto ApplicationUtil::getFirstChild( SmartPtr<ui::IUIElement> element ) -> SmartPtr<ui::IUIElement>
    {
        if( element )
        {
            if( auto p = element->getChildren() )
            {
                auto &children = *p;
                if( !children.empty() )
                {
                    return children.front();
                }
            }
        }
        return nullptr;
    }

    auto ApplicationUtil::getText( SmartPtr<ui::IUITreeNode> node ) -> String
    {
        if( auto p = node->getChildren() )
        {
            auto &children = *p;
            for( auto &child : children )
            {
                if( child->isDerived<ui::IUIText>() )
                {
                    auto text = fb::static_pointer_cast<ui::IUIText>( child );
                    return text->getText();
                }
            }
        }

        return String( "" );
    }

    auto ApplicationUtil::getComponentFactoryType( const String &factoryType ) -> String
    {
        Array<String> ignoreList;
        ignoreList.emplace_back( "UnityEngine.Transform" );

        if( std::find( ignoreList.begin(), ignoreList.end(), factoryType ) != ignoreList.end() )
        {
            return String( "" );
        }

        Map<String, String> componentMap;
        componentMap["saracen.ApplicationManager"] = "SimulatorApplication";
        componentMap["FB.FBFiniteStateMachine"] = "FiniteStateMachine";
        componentMap["UnityEngine.Canvas"] = "CanvasComponent";

        componentMap["UnityEngine.RectTransform"] = "CanvasTransform";

        componentMap["UnityEngine.UI.Text"] = "TextComponent";
        componentMap["UnityEngine.UI.Image"] = "ImageComponent";
        componentMap["UnityEngine.UI.Button"] = "ButtonComponent";

        componentMap["Unitycoding.UIWidgets.TooltipTrigger"] = "Tooltip";
        componentMap["UI.Tables.TableLayout"] = "TableLayout";
        componentMap["UnityEngine.UI.VerticalLayoutGroup"] = "TableLayout";

        componentMap["saracen.StartMenu"] = "StartMenu";

        auto it = componentMap.find( factoryType );
        if( it != componentMap.end() )
        {
            return it->second;
        }

        if( factoryType.find( "saracen." ) != String::npos )
        {
            return StringUtil::replaceAll( factoryType, "saracen.", "" );
        }

        return String( "" );
    }

    void ApplicationUtil::convertCSharp( const String &srcPath, const String &dstPath )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        ScriptGenerator scriptGenerator;
        auto csharpPath = "F:/dev/FB_v3/Assets/_game/scripts";
        auto cppPath = "E:/dev/game_code";

        fileSystem->addFolder( csharpPath, true );

        // if (fileSystem->isExistingFolder(csharpPath))
        {
            if( !fileSystem->isExistingFolder( cppPath ) )
            {
                fileSystem->createDirectories( cppPath );
            }

            fileSystem->addFolder( cppPath, true );

            scriptGenerator.setProjectPath( "FBApplication" );
            scriptGenerator.setReplaceFileName( "" );
            scriptGenerator.setReplacementFileName( "" );

            auto namespaceNames = Array<String>();
            namespaceNames.emplace_back( "fb" );
            scriptGenerator.setNamespaceNames( namespaceNames );

            scriptGenerator.convertCSharp( csharpPath, cppPath );
        }
    }

    auto ApplicationUtil::getEventPriority( SmartPtr<scene::IComponent> component ) -> u32
    {
        if( component->isDerived<scene::Mesh>() )
        {
            return 500;
        }
        if( component->isDerived<scene::CollisionMesh>() )
        {
            return 400;
        }
        if( component->isDerived<scene::Rigidbody>() )
        {
            return 300;
        }
        if( component->isDerived<scene::MeshRenderer>() )
        {
            return 200;
        }
        if( component->isDerived<scene::Material>() )
        {
            return 100;
        }

        return 1000;
    }

    void ApplicationUtil::showAllOverlays()
    {
    }

    void ApplicationUtil::hideAllOverlays()
    {
    }

    /*
    void ApplicationUtil::addActor( data::actor_data &parent, const data::json_actor &a )
    {
        data::actor_data actor;
        actor.name = a.name;
        actor.label = a.name;

        for( auto child : a.children )
        {
            addActor( actor, child );
        }

        parent.children.push_back( actor );
    }
    */

    auto ApplicationUtil::importJsonScene( const String &filePath ) -> SmartPtr<IData>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto dataStr = fileSystem->readAllText( filePath );

        //data::json_scene scene;
        //DataUtil::parse( dataStr, &scene );

        //auto pScene = fb::make_ptr<Data<data::fb_scene>>();
        //auto rScene = pScene->getDataAsType<data::fb_scene>();

        //data::actor_data actor;
        //actor.name = "root";
        //actor.label = "root";

        //for( auto a : scene.hierarchy )
        //{
        //    addActor( actor, a );
        //}

        //rScene->actors.push_back( actor );

        //return pScene;

        return nullptr;
    }

    auto ApplicationUtil::importScene( const String &filePath ) -> SmartPtr<IData>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto dataStr = fileSystem->readAllText( filePath );

        //auto pScene = fb::make_ptr<Data<data::fb_scene>>();
        //auto rScene = pScene->getDataAsType<data::fb_scene>();

        //DataUtil::parse( dataStr, rScene.get() );

        //return pScene;

        return nullptr;
    }

    auto ApplicationUtil::createOverlayPanelTest() -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        scene->addActor( actor );
        scene->registerAllUpdates( actor );
        actor->setName( "Canvas" );

        actor->setLocalPosition( Vector3<real_Num>( 0, 0, 0 ) );
        actor->setPosition( Vector3<real_Num>( 0, 0, 0 ) );

        auto canvas = actor->addComponent<scene::Layout>();
        canvas->setReferenceSize( Vector2I( 1920, 1080 ) );

        auto actorImageParent = sceneManager->createActor();
        actor->addChild( actorImageParent );
        actorImageParent->setName( "Transform" );

        auto actorImageParentCanvasTransform = actorImageParent->addComponent<scene::LayoutTransform>();
        actorImageParentCanvasTransform->setSize( Vector2<real_Num>( 1920.0f, 1080.0f ) );

        auto actorImage = sceneManager->createActor();
        actorImageParent->addChild( actorImage );
        actorImage->setName( "Image" );

        auto actorImageCanvasTransform = actorImage->addComponent<scene::LayoutTransform>();
        actorImageCanvasTransform->setSize( Vector2<real_Num>( 1280.0f, 720.0f ) * 1.0f );

        auto image = actorImage->addComponent<scene::Image>();
        // image->setImagePath(mediaPath + "/checker.png");
        //image->setImagePath( "f40_hd.jpg" );

        auto materialComponent = actorImage->addComponent<scene::Material>();
        materialComponent->setMaterialPath( "DefaultUI.mat" );

        // auto material = materialComponent->getMaterial();
        // if (material)
        //{
        //	material->setMaterialType(render::IMaterial::MaterialType::UI);
        // }

        return actor;
    }

    auto ApplicationUtil::createOverlayTextTest() -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        scene->addActor( actor );
        scene->registerAllUpdates( actor );
        actor->setName( "Canvas" );

        actor->setLocalPosition( Vector3<real_Num>( 0, 0, 0 ) );
        actor->setPosition( Vector3<real_Num>( 0, 0, 0 ) );

        auto canvas = actor->addComponent<scene::Layout>();
        canvas->setReferenceSize( Vector2I( 1920, 1080 ) );

        auto actorImageParent = sceneManager->createActor();
        actor->addChild( actorImageParent );
        actorImageParent->setName( "Transform" );

        auto actorImageParentCanvasTransform = actorImageParent->addComponent<scene::LayoutTransform>();
        actorImageParentCanvasTransform->setSize( Vector2<real_Num>( 1920.0f, 1080.0f ) );

        auto actorImage = sceneManager->createActor();
        actorImageParent->addChild( actorImage );
        actorImage->setName( "Image" );

        auto actorImageCanvasTransform = actorImage->addComponent<scene::LayoutTransform>();
        actorImageCanvasTransform->setSize( Vector2<real_Num>( 1280.0f, 720.0f ) * 1.0f );

        auto actorText = sceneManager->createActor();
        actorImage->addChild( actorText );

        auto actorTextCanvasTransform = actorText->addComponent<scene::LayoutTransform>();
        if( actorTextCanvasTransform )
        {
            actorTextCanvasTransform->setSize( Vector2<real_Num>( 300, 200 ) );
        }

        auto textComponent = actorText->addComponent<scene::Text>();
        if( textComponent )
        {
            textComponent->setText( "test" );
        }

        // auto image = actorImage->addComponent<scene::ImageComponent>();
        ////image->setImagePath(mediaPath + "/checker.png");
        // image->setImagePath("f40_hd.jpg");

        auto materialComponent = actorImage->addComponent<scene::Material>();
        materialComponent->setMaterialPath( "DefaultUI.mat" );

        // auto material = materialComponent->getMaterial();
        // if (material)
        //{
        //	material->setMaterialType(render::IMaterial::MaterialType::UI);
        // }

        return actor;
    }

    auto ApplicationUtil::createOverlayButtonTest() -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = sceneManager->createActor();
        scene->addActor( actor );
        scene->registerAllUpdates( actor );
        actor->setName( "Canvas" );

        actor->setLocalPosition( Vector3<real_Num>( 0, 0, 0 ) );
        actor->setPosition( Vector3<real_Num>( 0, 0, 0 ) );

        auto canvas = actor->addComponent<scene::Layout>();
        canvas->setReferenceSize( Vector2I( 1920, 1080 ) );

        auto actorImageParent = sceneManager->createActor();
        actor->addChild( actorImageParent );
        actorImageParent->setName( "Transform" );

        auto actorImageParentCanvasTransform = actorImageParent->addComponent<scene::LayoutTransform>();
        actorImageParentCanvasTransform->setSize( Vector2<real_Num>( 1920.0f, 1080.0f ) );

        auto actorButton = sceneManager->createActor();
        actorImageParent->addChild( actorButton );
        actorButton->setName( "Button" );

        actorButton->setLocalPosition( Vector3<real_Num>( 300, 200, 0 ) );

        auto actorImageCanvasTransform = actorButton->addComponent<scene::LayoutTransform>();
        if( actorImageCanvasTransform )
        {
            actorImageCanvasTransform->setSize( Vector2<real_Num>( 300, 100 ) );
        }

        auto actorButtonImageComponent = actorButton->addComponent<scene::Image>();
        if( actorButtonImageComponent )
        {
            //actorButtonImageComponent->setImagePath( "checker.png" );
        }

        actorButton->addComponent<scene::Button>();

        auto material = actorButton->addComponent<scene::Material>();
        if( material )
        {
            // material->setMainTexturePath("Rounded Filled 256px.png");
            // material->setTint(ColourF(0.0f, 0.5f, 0.0f, 1.0f));
        }

        auto actorText = sceneManager->createActor();
        actorButton->addChild( actorText );

        actorText->addComponent<scene::LayoutTransform>();
        auto text = actorText->addComponent<scene::Text>();
        text->setText( "Button" );

        auto textName = String( "Text" );
        actorText->setName( textName );

        //auto image = actorImage->addComponent<scene::ImageComponent>();
        ////image->setImagePath(mediaPath + "/checker.png");
        // image->setImagePath("f40_hd.jpg");

        auto materialComponent = actorButton->addComponent<scene::Material>();
        materialComponent->setMaterialPath( "DefaultUI.mat" );

        // auto material = materialComponent->getMaterial();
        // if (material)
        //{
        //	material->setMaterialType(render::IMaterial::MaterialType::UI);
        // }

        return actor;
    }

    void ApplicationUtil::createRigidStaticMesh( SmartPtr<scene::IActor> actor, bool recursive )
    {
        try
        {
            actor->setStatic( true );

            auto meshComponent = actor->getComponent<scene::Mesh>();
            if( meshComponent )
            {
                auto collisionMesh = actor->getComponent<scene::CollisionMesh>();
                if( !collisionMesh )
                {
                    collisionMesh = actor->addComponent<scene::CollisionMesh>();
                }

                if( collisionMesh )
                {
                    auto meshPath = meshComponent->getMeshPath();
                    collisionMesh->setMeshPath( meshPath );
                }

                auto rigidbody = actor->getComponent<scene::Rigidbody>();
                if( !rigidbody )
                {
                    rigidbody = actor->addComponent<scene::Rigidbody>();
                }
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    createRigidStaticMesh( child, recursive );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ApplicationUtil::createRigidDynamicMesh( SmartPtr<scene::IActor> actor, bool recursive )
    {
        try
        {
            actor->setStatic( false );

            auto meshComponent = actor->getComponent<scene::Mesh>();
            if( meshComponent )
            {
                auto collisionMesh = actor->getComponent<scene::CollisionMesh>();
                if( !collisionMesh )
                {
                    collisionMesh = actor->addComponent<scene::CollisionMesh>();
                }

                if( collisionMesh )
                {
                    auto meshPath = meshComponent->getMeshPath();
                    collisionMesh->setMeshPath( meshPath );
                }

                auto rigidbody = actor->getComponent<scene::Rigidbody>();
                if( !rigidbody )
                {
                    rigidbody = actor->addComponent<scene::Rigidbody>();
                }
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    createRigidDynamicMesh( child, recursive );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ApplicationUtil::createFactories()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<Properties>();

        FactoryUtil::addFactory<scene::Actor>();
        FactoryUtil::addFactory<scene::Director>();

        FactoryUtil::addFactory<scene::SphericalCameraController>();
        FactoryUtil::addFactory<scene::ThirdPersonCameraController>();
        FactoryUtil::addFactory<scene::VehicleCameraController>();

        FactoryUtil::addFactory<scene::Light>();

        //FactoryUtil::addFactory<scene::Application>();
        //FactoryUtil::addFactory<scene::UI>();
        //FactoryUtil::addFactory<scene::SimulatorApplication>();
        //FactoryUtil::addFactory<scene::SimulatorUI>();
        //FactoryUtil::addFactory<scene::TruckSimulatorApplication>();

        FactoryUtil::addFactory<scene::Camera>();
        FactoryUtil::addFactory<scene::CarController>();
        FactoryUtil::addFactory<scene::Constraint>();

        FactoryUtil::addFactory<scene::CollisionBox>();
        FactoryUtil::addFactory<scene::CollisionMesh>();
        FactoryUtil::addFactory<scene::CollisionPlane>();
        FactoryUtil::addFactory<scene::CollisionSphere>();
        FactoryUtil::addFactory<scene::CollisionTerrain>();

        //FactoryUtil::addFactory<scene::DynamicMesh>();

        FactoryUtil::addFactory<scene::FiniteStateMachine>();

        //FactoryUtil::addFactory<scene::LSystem>();
        //FactoryUtil::addFactory<scene::LSystemTurtle>();

        FactoryUtil::addFactory<scene::Material>();
        FactoryUtil::addFactory<scene::Mesh>();
        FactoryUtil::addFactory<scene::MeshRenderer>();

        FactoryUtil::addFactory<scene::ParticleSystem>();

        FactoryUtil::addFactory<scene::Rigidbody>();
        FactoryUtil::addFactory<scene::Skybox>();
        //FactoryUtil::addFactory<scene::StemComponent>();

        FactoryUtil::addFactory<scene::TerrainBlendMap>();
        FactoryUtil::addFactory<scene::TerrainLayer>();
        FactoryUtil::addFactory<scene::TerrainSystem>();

        FactoryUtil::addFactory<scene::UserComponent>();

        FactoryUtil::addFactory<scene::WheelController>();

        //FactoryUtil::addFactory<ui::StartMenu>();
        //FactoryUtil::addFactory<ui::SystemSettings>();

        FactoryUtil::addFactory<scene::Button>();
        FactoryUtil::addFactory<scene::Layout>();
        FactoryUtil::addFactory<scene::LayoutTransform>();
        FactoryUtil::addFactory<scene::Image>();
        FactoryUtil::addFactory<scene::InputField>();
        FactoryUtil::addFactory<scene::Text>();

        FactoryUtil::addFactory<scene::Transform>();

        FactoryUtil::addFactory<StateMessageVector3>();
        FactoryUtil::addFactory<StateMessageVector4>();
        FactoryUtil::addFactory<StateMessageUIntValue>();
        FactoryUtil::addFactory<StateMessageIntValue>();
        FactoryUtil::addFactory<StateMessageVisible>();

        FactoryUtil::addFactory<MaterialPassState>();
        FactoryUtil::addFactory<PhysicsSceneState>();

        const auto size = 32;

        factoryManager->setPoolSizeByType<Properties>( size );

        factoryManager->setPoolSizeByType<scene::Actor>( size );
        factoryManager->setPoolSizeByType<scene::Transform>( size );

        factoryManager->setPoolSizeByType<scene::Material>( size );
        factoryManager->setPoolSizeByType<scene::Mesh>( size );
        factoryManager->setPoolSizeByType<scene::MeshRenderer>( size );
        factoryManager->setPoolSizeByType<scene::CollisionBox>( size );
        factoryManager->setPoolSizeByType<scene::Rigidbody>( size );

        const auto messagePoolSize = 12;

        factoryManager->setPoolSizeByType<StateMessageVector3>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageVector4>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageUIntValue>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageIntValue>( messagePoolSize );
        factoryManager->setPoolSizeByType<StateMessageVisible>( messagePoolSize );

        auto factories = factoryManager->getFactories();
        for( auto factory : factories )
        {
            factory->allocatePoolData();
        }
    }

    void ApplicationUtil::createDefaultMaterials()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto filePath = String( "default" );
        auto result = resourceDatabase->createOrRetrieveByType<render::IMaterial>( filePath );
        if( result.first && result.second )
        {
            auto material = result.first;
            graphicsSystem->loadObject( material );
        }
    }

    void ApplicationUtil::createDefaultFont()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto director = fb::make_ptr<scene::Director>();
        director->load( nullptr );

        auto fontFileName = "cuckoo.ttf";
        auto directorProperties = director->getProperties();
        directorProperties->setProperty( "font_type", String( "arial" ) );
        directorProperties->setProperty( "font_source", String( fontFileName ) );
        directorProperties->setProperty( "font_size", 12 );
        directorProperties->setProperty( "font_resolution", 96 );

        if( fileSystem->isExistingFile( fontFileName, true, true ) )
        {
            auto fontName = String( "default" );
            auto result =
                resourceDatabase->createOrRetrieveFromDirector<render::IFont>( fontName, director );
            if( result.first && result.second )
            {
                auto font = result.first;
                font->setProperties( directorProperties );
                font->load( nullptr );
            }
        }
    }

    auto ApplicationUtil::getResourceType( const String &resourceTypeName ) -> IResource::ResourceType
    {
        if( resourceTypeName == "None" )
        {
            return IResource::ResourceType::None;
        }
        if( resourceTypeName == "Material" )
        {
            return IResource::ResourceType::Material;
        }
        if( resourceTypeName == "Object" )
        {
            return IResource::ResourceType::Object;
        }
        if( resourceTypeName == "Actor" )
        {
            return IResource::ResourceType::Actor;
        }
        if( resourceTypeName == "Component" )
        {
            return IResource::ResourceType::Component;
        }
        if( resourceTypeName == "Texture" )
        {
            return IResource::ResourceType::Texture;
        }

        return IResource::ResourceType::None;
    }

    auto ApplicationUtil::getResourceTypeByName( IResource::ResourceType resourceType ) -> String
    {
        switch( resourceType )
        {
        case IResource::ResourceType::None:
            return "None";
        case IResource::ResourceType::Material:
            return "Material";
        case IResource::ResourceType::Object:
            return "Object";
        case IResource::ResourceType::Actor:
            return "Actor";
        case IResource::ResourceType::Component:
            return "Component";
        case IResource::ResourceType::Texture:
            return "Texture";
        }

        return "";
    }

    auto ApplicationUtil::isSupportedMesh( const String &filePath ) -> bool
    {
        auto fileTypes = Array<String>();
        fileTypes.reserve( 5 );

        fileTypes.emplace_back( ".fbx" );
        fileTypes.emplace_back( ".obj" );
        fileTypes.emplace_back( ".dae" );
        fileTypes.emplace_back( ".3ds" );
        fileTypes.emplace_back( ".fbmeshbin" );

        auto ext = Path::getFileExtension( filePath );
        ext = StringUtil::make_lower( ext );

        for( auto type : fileTypes )
        {
            if( ext == type )
            {
                return true;
            }
        }

        return false;
    }

    auto ApplicationUtil::isSupportedTexture( const String &filePath ) -> bool
    {
        auto fileTypes = Array<String>();
        fileTypes.reserve( 5 );

        fileTypes.emplace_back( ".png" );
        fileTypes.emplace_back( ".jpg" );
        fileTypes.emplace_back( ".jpeg" );
        fileTypes.emplace_back( ".tga" );
        fileTypes.emplace_back( ".bmp" );

        auto ext = Path::getFileExtension( filePath );
        ext = StringUtil::make_lower( ext );

        for( auto type : fileTypes )
        {
            if( ext == type )
            {
                return true;
            }
        }

        return false;
    }

    auto ApplicationUtil::isSupportedSound( const String &filePath ) -> bool
    {
        auto fileTypes = Array<String>();
        fileTypes.reserve( 5 );

        fileTypes.emplace_back( ".wav" );
        fileTypes.emplace_back( ".ogg" );
        fileTypes.emplace_back( ".mp3" );

        auto ext = Path::getFileExtension( filePath );
        ext = StringUtil::make_lower( ext );

        for( auto type : fileTypes )
        {
            if( ext == type )
            {
                return true;
            }
        }

        return false;
    }

    auto ApplicationUtil::createDefaultSky( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = sceneManager->createActor();

        auto name = String( "Skybox" );
        actor->setName( name );

        auto skybox = actor->addComponent<scene::Skybox>();

        auto material = actor->addComponent<scene::Material>();
        if( material )
        {
            material->setMaterialPath( "DefaultSkybox.mat" );
        }

        if( addToScene )
        {
            scene->addActor( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto ApplicationUtil::createCamera( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = sceneManager->createActor();

        auto name = String( "Camera" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::Camera>();
        FB_ASSERT( c );

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    void ApplicationUtil::createRigidStaticMesh()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                    createRigidStaticMesh( actor, true );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ApplicationUtil::createRigidDynamicMesh()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                    createRigidDynamicMesh( actor, true );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ApplicationUtil::createDefaultCubemap( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );
            FB_ASSERT( scene->isValid() );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            auto name = String( "Cubemap" );
            actor->setName( name );

            auto c = actor->addComponent<scene::CollisionBox>();
            FB_ASSERT( c );

            auto cubemap = actor->addComponent<scene::Cubemap>();
            FB_ASSERT( cubemap );

            auto meshComponent = actor->addComponent<scene::Mesh>();
            FB_ASSERT( meshComponent );
            meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

            auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = actor->addComponent<scene::Material>();
            FB_ASSERT( material );
            FB_ASSERT( material->isValid() );

            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            if( addToScene )
            {
                scene->addActor( actor );
                FB_ASSERT( scene->isValid() );
            }

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto ApplicationUtil::createDefaultCube( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );
            FB_ASSERT( scene->isValid() );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            const auto name = String( "Cube" );
            actor->setName( name );

            auto c = actor->addComponent<scene::CollisionBox>();
            FB_ASSERT( c );

            auto rigidbody = actor->addComponent<scene::Rigidbody>();
            FB_ASSERT( rigidbody );

            auto meshComponent = actor->addComponent<scene::Mesh>();
            FB_ASSERT( meshComponent );
            meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

            auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = actor->addComponent<scene::Material>();
            FB_ASSERT( material );
            FB_ASSERT( material->isValid() );

            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            if( addToScene )
            {
                scene->addActor( actor );
                FB_ASSERT( scene->isValid() );
            }

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto ApplicationUtil::createDefaultCubeMesh( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );
            FB_ASSERT( scene->isValid() );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            const auto name = String( "Cube Mesh" );
            actor->setName( name );

            auto c = actor->addComponent<scene::CollisionMesh>();
            FB_ASSERT( c );

            auto rigidbody = actor->addComponent<scene::Rigidbody>();
            FB_ASSERT( rigidbody );

            auto meshComponent = actor->addComponent<scene::Mesh>();
            FB_ASSERT( meshComponent );
            meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

            auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = actor->addComponent<scene::Material>();
            FB_ASSERT( material );
            FB_ASSERT( material->isValid() );

            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            if( addToScene )
            {
                scene->addActor( actor );
                FB_ASSERT( scene->isValid() );
            }

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto ApplicationUtil::createDefaultGround( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        actor->setStatic( true );

        auto name = String( "Ground" );
        actor->setName( name );

        auto collisionBox = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( collisionBox );
        if( collisionBox )
        {
            collisionBox->setExtents( Vector3<real_Num>::unit() * 1.0f );
        }

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        //auto material = actor->addComponent<scene::Material>();
        //FB_ASSERT( material );

        //if( material )
        //{
        //    material->setMaterialPath( "default" );
        //    material->updateMaterial();
        //}

        auto groundPosition = Vector3<real_Num>::unitY() * static_cast<real_Num>( -250.0 );
        // groundPosition += Vector3<real_Num>::unitZ() * -250.0f;

        auto groundScale = Vector3<real_Num>::unit() * static_cast<real_Num>( 500.0 );

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        if( addToScene )
        {
            scene->addActor( actor );
            FB_ASSERT( scene->isValid() );
        }

        return actor;
    }

    auto ApplicationUtil::createDefaultTerrain( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        actor->setStatic( true );

        auto name = String( "Terrain" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        // todo remove
        // for test the renderer
        auto terrain = actor->addComponent<scene::TerrainSystem>();

        auto terrainCollision = actor->addComponent<scene::CollisionTerrain>();

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto ApplicationUtil::createDefaultConstraint() -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        actor->setStatic( true );

        auto name = String( "Constraint" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto terrain = actor->addComponent<scene::Constraint>();

        scene->addActor( actor );
        scene->registerAllUpdates( actor );

        return actor;
    }

    auto ApplicationUtil::createDirectionalLight( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Directional Light" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        // todo remove
        // for test the renderer
        auto light = actor->addComponent<scene::Light>();
        if( light )
        {
            light->setLightType( render::ILight::LightTypes::LT_DIRECTIONAL );
        }

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto ApplicationUtil::createPointLight( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Main Light" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        // todo remove
        // for test the renderer
        auto light = actor->addComponent<scene::Light>();
        if( light )
        {
            light->setLightType( render::ILight::LightTypes::LT_POINT );
        }

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto ApplicationUtil::createDefaultPlane( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Plane" );
        actor->setName( name );

        auto localOrientation = Quaternion<real_Num>::angleAxis(
            Math<real_Num>::DegToRad( static_cast<real_Num>( -90.0 ) ), Vector3<real_Num>::unitX() );
        actor->setLocalOrientation( localOrientation );
        actor->setLocalScale( Vector3<real_Num>::unit() * static_cast<real_Num>( 100.0 ) );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( "plane.fbmeshbin" );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto ApplicationUtil::createDefaultVehicle( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        f32 wheelBase = 2.530f;
        f32 length = 4.405f;
        f32 width = 1.810f;
        f32 height = 1.170f;
        f32 mass = 1370.0f;

        auto actor = sceneManager->createActor();

        auto name = String( "Vehicle" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto bodyDimensions = Vector3<real_Num>( length, height, width ) * 0.5f;
        c->setExtents( bodyDimensions );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto vehicle = actor->addComponent<scene::CarController>();
        FB_ASSERT( vehicle );

        auto cubeMeshPath = String( "cube_internal.fbmeshbin" );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( cubeMeshPath );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto meshActor = sceneManager->createActor();

        auto meshActorName = String( "mesh" );
        meshActor->setName( meshActorName );

        actor->addChild( meshActor );

        Array<Vector3<real_Num>> wheelPositions;
        wheelPositions.resize( 4 );

        auto wheelOffset = -1.0f;
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::FRONT_LEFT )] =
            Vector3<real_Num>( -width / 2.0f, wheelOffset, wheelBase / 2.0f );
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::FRONT_RIGHT )] =
            Vector3<real_Num>( width / 2.0f, wheelOffset, wheelBase / 2.0f );
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::REAR_LEFT )] =
            Vector3<real_Num>( -width / 2.0f, wheelOffset, -wheelBase / 2.0f );
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::REAR_RIGHT )] =
            Vector3<real_Num>( width / 2.0f, wheelOffset, -wheelBase / 2.0f );

        auto wheelMeshSize = Vector3<real_Num>( 0.1f, 0.3f, 0.3f );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelMeshActor = sceneManager->createActor();

            auto wheelMeshActorName = String( "wheel_" ) + StringUtil::toString( i );
            wheelMeshActor->setName( wheelMeshActorName );

            auto wheelMeshComponent = wheelMeshActor->addComponent<scene::Mesh>();
            FB_ASSERT( wheelMeshComponent );
            wheelMeshComponent->setMeshPath( cubeMeshPath );

            auto meshRenderer = wheelMeshActor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = wheelMeshActor->addComponent<scene::Material>();
            FB_ASSERT( material );

            meshActor->addChild( wheelMeshActor );

            wheelMeshActor->setLocalPosition( wheelPositions[i] );
            wheelMeshActor->setLocalScale( wheelMeshSize );
        }

        auto dynamicsActor = sceneManager->createActor();

        auto dynamicsName = String( "dynamics" );
        dynamicsActor->setName( dynamicsName );

        // auto dynamicsId = StringUtil::getHash(dynamicsName);
        // dynamicsActor->setId(dynamicsId);

        // auto dynamicsUuid = StringUtil::getUUID();
        // dynamicsActor->setUUID(dynamicsUuid);

        actor->addChild( dynamicsActor );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelActor = sceneManager->createActor();
            dynamicsActor->addChild( wheelActor );

            auto wheelName = String( "Wheel" );
            wheelActor->setName( wheelName );

            auto wheel = wheelActor->addComponent<scene::WheelController>();
            FB_ASSERT( wheel );
        }

        // auto prefab = prefabManager->loadPrefab("f40.fbx");
        // if (prefab)
        //{
        //	auto vehicleMesh = prefab->createActor();
        //
        //	//auto vehicleMeshTransform = vehicleMesh->getLocalTransform();
        //	//if (vehicleMeshTransform)
        //	//{
        //	//	vehicleMeshTransform->setScale(Vector3<real_Num>::unit() * 0.0254f);
        //	//}

        //	vehicleMesh->setName("F40");
        //	actor->addChild(vehicleMesh);
        //}

        auto groundPosition = Vector3<real_Num>::unitY() * static_cast<real_Num>( 5.0 );
        auto groundScale = Vector3<real_Num>::unit() * static_cast<real_Num>( 1.0 );

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        actor->updateTransform();

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto ApplicationUtil::createDefaultCar( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Car" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto vehicle = actor->addComponent<scene::CarController>();
        FB_ASSERT( vehicle );

        auto cubeMeshPath = String( "cube_internal.fbmeshbin" );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( cubeMeshPath );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto dynamicsActor = sceneManager->createActor();

        auto dynamicsName = String( "dynamics" );
        dynamicsActor->setName( dynamicsName );

        // auto dynamicsId = StringUtil::getHash(dynamicsName);
        // dynamicsActor->setId(dynamicsId);

        // auto dynamicsUuid = StringUtil::getUUID();
        // dynamicsActor->setUUID(dynamicsUuid);

        actor->addChild( dynamicsActor );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelActor = sceneManager->createActor();
            dynamicsActor->addChild( wheelActor );

            auto wheelName = String( "Wheel" );
            wheelActor->setName( wheelName );

            auto wheel = wheelActor->addComponent<scene::WheelController>();
            FB_ASSERT( wheel );
        }

        auto rigActor = sceneManager->createActor();

        auto rigName = String( "rig" );
        rigActor->setName( rigName );
        actor->addChild( rigActor );

        //auto prefab = prefabManager->loadPrefab( "f40.fbx" );
        //if( prefab )
        //{
        //    auto vehicleMesh = prefab->createActor();

        //    // auto vehicleMeshTransform = vehicleMesh->getLocalTransform();
        //    // if (vehicleMeshTransform)
        //    //{
        //    //	vehicleMeshTransform->setScale(Vector3<real_Num>::unit() * 0.0254f);
        //    // }

        //    vehicleMesh->setName( "F40" );
        //    rigActor->addChild( vehicleMesh );
        //}

        auto groundPosition = Vector3<real_Num>::unitY() * 5.0f;
        auto groundScale = Vector3<real_Num>::unit() * 1.0f;

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        actor->updateTransform();

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto ApplicationUtil::createDefaultTruck( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Truck" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto vehicle = actor->addComponent<scene::CarController>();
        FB_ASSERT( vehicle );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto dynamicsActor = sceneManager->createActor();

        auto dynamicsName = String( "dynamics" );
        dynamicsActor->setName( dynamicsName );

        // auto dynamicsId = StringUtil::getHash(dynamicsName);
        // dynamicsActor->setId(dynamicsId);

        // auto dynamicsUuid = StringUtil::getUUID();
        // dynamicsActor->setUUID(dynamicsUuid);

        actor->addChild( dynamicsActor );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelActor = sceneManager->createActor();
            dynamicsActor->addChild( wheelActor );

            auto wheelName = String( "Wheel" );
            wheelActor->setName( wheelName );

            auto wheel = wheelActor->addComponent<scene::WheelController>();
            FB_ASSERT( wheel );
        }

        auto rigActor = sceneManager->createActor();

        auto rigName = String( "rig" );
        rigActor->setName( rigName );
        actor->addChild( rigActor );

        auto prefab = prefabManager->loadPrefab( "f40.fbx" );
        if( prefab )
        {
            auto vehicleMesh = prefab->createActor();

            // auto vehicleMeshTransform = vehicleMesh->getLocalTransform();
            // if (vehicleMeshTransform)
            //{
            //	vehicleMeshTransform->setScale(Vector3<real_Num>::unit() * 0.0254f);
            // }

            vehicleMesh->setName( "F40" );
            rigActor->addChild( vehicleMesh );
        }

        auto groundPosition = Vector3<real_Num>::unitY() * 5.0f;
        auto groundScale = Vector3<real_Num>::unit() * 1.0f;

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        actor->updateTransform();

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto ApplicationUtil::createDefaultParticleSystem( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        FB_ASSERT( actor );

        auto particleSystem = actor->addComponent<scene::ParticleSystem>();
        FB_ASSERT( particleSystem );

        return actor;
    }

    auto ApplicationUtil::loadVehicleDataFromDatabase( s32 id ) -> SmartPtr<IData>
    {
        return nullptr;
    }

    auto ApplicationUtil::createDefaultMaterialUI() -> SmartPtr<render::IMaterial>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto materialManager = graphicsSystem->getMaterialManager();
        FB_ASSERT( materialManager );

        auto materialName = String( "DefaultUI" );
        SmartPtr<render::IMaterial> defaultMat = materialManager->loadFromFile( materialName + ".mat" );
        if( !defaultMat )
        {
            static const auto uuid =
                String( "2f261ebe-5db8-11ed-9b6a-0242ac120002" );  //StringUtil::getUUID();

            auto resource = materialManager->createOrRetrieve( uuid, materialName, "material" );
            auto material = fb::static_pointer_cast<render::IMaterial>( resource.first );
            if( material )
            {
                material->setMaterialType( render::IMaterial::MaterialType::UI );
                graphicsSystem->loadObject( material );

                auto techniques = material->getTechniques();
                SmartPtr<render::IMaterialTechnique> technique;

                if( !techniques.empty() )
                {
                    technique = techniques[0];
                }

                if( !technique )
                {
                    technique = material->createTechnique();
                }

                if( technique )
                {
                    auto passes = technique->getPasses();
                    SmartPtr<render::IMaterialPass> pass;

                    if( !passes.empty() )
                    {
                        pass = passes[0];
                    }

                    if( !pass )
                    {
                        pass = technique->createPass();
                    }

                    if( pass )
                    {
                        auto textures = pass->getTextureUnits();
                        if( !textures.empty() )
                        {
                            //textures[0];
                        }
                    }
                }
            }

            return material;
        }

        return defaultMat;
    }

    auto ApplicationUtil::createDefaultMaterial() -> SmartPtr<render::IMaterial>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto materialManager = graphicsSystem->getMaterialManager();
        FB_ASSERT( materialManager );

        auto resource = materialManager->create( StringUtil::getUUID() );
        auto material = fb::static_pointer_cast<render::IMaterial>( resource );
        if( material )
        {
            auto techniques = material->getTechniques();
            SmartPtr<render::IMaterialTechnique> technique;

            if( !techniques.empty() )
            {
                technique = techniques[0];
            }

            if( !technique )
            {
                technique = material->createTechnique();
            }

            if( technique )
            {
                auto passes = technique->getPasses();
                SmartPtr<render::IMaterialPass> pass;

                if( !passes.empty() )
                {
                    pass = passes[0];
                }

                if( !pass )
                {
                    pass = technique->createPass();
                }

                if( pass )
                {
                }
            }
        }

        return material;
    }

    auto ApplicationUtil::createProceduralTest() -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "ProceduralScene" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        //auto proceduralScene = actor->addComponent<scene::ProceduralScene>();
        //if( proceduralScene )
        //{
        //    proceduralScene->loadMapData( "bullsmoor_small.osm" );
        //}

        scene->addActor( actor );
        scene->registerUpdate( Thread::Task::Primary, Thread::UpdateState::Update, actor );

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto ApplicationUtil::loadVehicleFromDB( s32 id ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actorDatabase = sceneManager->createActor();
        FB_ASSERT( actorDatabase );

        //auto vehicleDatabaseManager = actorDatabase->addComponent<VehicleDatabaseManager>();
        //scene->addActor( actorDatabase );

        auto actorSystemSettings = sceneManager->createActor();
        FB_ASSERT( actorSystemSettings );

        //auto systemSettings = actorSystemSettings->addComponent<scene::SystemSettings>();

        scene->addActor( actorSystemSettings );

        auto actorPilot = sceneManager->createActor();
        FB_ASSERT( actorPilot );

        //auto player = actorSystemSettings->addComponent<scene::Player>();

        scene->addActor( actorPilot );

        auto actor = sceneManager->createActor();
        FB_ASSERT( actor );

        //auto vehicle = actor->addComponent<scene::VehicleCar>();

        scene->addActor( actor );

        //player->setVehicle( vehicle );
        ////applicationManager->setPlayer( player );
        //applicationManager->setDatabase( vehicleDatabaseManager );

        //auto job = fb::make_ptr<VehicleLoadJob>();
        //job->setVehicle( vehicle );
        //job->setModelId( id );
        //job->execute();

        return actor;
    }

    auto ApplicationUtil::getRelativeMousePos( Vector2<real_Num> relativeMousePosition )
        -> Vector2<real_Num>
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto ui = applicationManager->getUI();
        if( ui )
        {
            if( auto uiWindow = ui->getMainWindow() )
            {
                if( auto mainWindow = applicationManager->getWindow() )
                {
                    auto mainWindowSize = mainWindow->getSize();
                    auto mainWindowSizeF = Vector2<real_Num>( static_cast<f32>( mainWindowSize.x ),
                                                              static_cast<f32>( mainWindowSize.y ) );

                    auto pos = uiWindow->getPosition() / mainWindowSizeF;
                    auto size = uiWindow->getSize() / mainWindowSizeF;

                    auto aabb = AABB2<real_Num>( pos, size, true );
                    if( aabb.isInside( relativeMousePosition ) )
                    {
                        return ( relativeMousePosition - pos ) / size;
                    }
                }
            }
        }

        return Vector2<real_Num>::zero();
    }

}  // end namespace fb
