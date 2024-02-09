#include <GameEditorPCH.h>
#include <commands/AddActorCmd.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    using namespace fb::scene;

    void AddActorCmd::load( SmartPtr<ISharedObject> data )
    {
    }

    void AddActorCmd::unload( SmartPtr<ISharedObject> data )
    {
    }

    void AddActorCmd::undo()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto editorManager = EditorManager::getSingletonPtr();
        auto projectManager = editorManager->getProjectManager();
        auto uiManager = editorManager->getUI();

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = getActor();
        if( actor )
        {
            scene->unregisterAll( actor );
            scene->removeActor( actor );
        }

        uiManager->rebuildSceneTree();
    }

    void AddActorCmd::redo()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto editorManager = EditorManager::getSingletonPtr();
        auto projectManager = editorManager->getProjectManager();
        auto uiManager = editorManager->getUI();
        auto project = editorManager->getProject();

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = createActor();
        if( actor )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        setActor( actor );

        uiManager->rebuildSceneTree();
    }

    void AddActorCmd::execute()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto projectManager = editorManager->getProjectManager();
        FB_ASSERT( projectManager );

        auto uiManager = editorManager->getUI();
        FB_ASSERT( uiManager );

        auto project = editorManager->getProject();
        FB_ASSERT( project );

        auto selectionManager = applicationManager->getSelectionManager();
        FB_ASSERT( selectionManager );

        auto selection = selectionManager->getSelection();

        SmartPtr<scene::IActor> parent;

        if( !selection.empty() )
        {
            auto selected = selection.back();

            if( selected->isDerived<scene::IActor>() )
            {
                parent = fb::dynamic_pointer_cast<scene::IActor>( selected );
            }
        }

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = createActor();
        //FB_ASSERT( actor );

        if( actor )
        {
            if( parent )
            {
                parent->addChild( actor );
            }
            else
            {
                auto s = actor->getScene();
                if( !s )
                {
                    scene->addActor( actor );
                }
            }

            if( auto root = actor->getSceneRoot() )
            {
                root->hierarchyChanged();
            }

            setActor( actor );

            if( applicationManager->isPlaying() )
            {
                actor->setState( IActor::State::Play );
            }
            else
            {
                actor->setState( IActor::State::Edit );
            }
        }

        uiManager->rebuildSceneTree();
    }

    auto AddActorCmd::createActor() -> SmartPtr<IActor>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto application = applicationManager->getApplication();

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto projectManager = editorManager->getProjectManager();
        FB_ASSERT( projectManager );

        auto uiManager = editorManager->getUI();
        FB_ASSERT( uiManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        switch( auto actorType = getActorType() )
        {
        case ActorType::Actor:
        {
            auto sceneManager = applicationManager->getSceneManager();
            auto scene = sceneManager->getCurrentScene();

            auto filePath = getFilePath();
            auto fileExt = Path::getFileExtension( filePath );
            fileExt = StringUtil::make_lower( fileExt );

            if( ApplicationUtil::isSupportedMesh( filePath ) || fileExt == ".prefab" )
            {
                auto prefabManager = applicationManager->getPrefabManager();

                auto selectionManager = applicationManager->getSelectionManager();

                auto prefabResource = prefabManager->loadResource( filePath );
                if( prefabResource )
                {
                    auto prefab = fb::static_pointer_cast<IPrefab>( prefabResource );
                    if( prefab )
                    {
                        if( auto actor = prefab->createActor() )
                        {
                            auto name = Path::getFileNameWithoutExtension( filePath );

                            if( auto handle = actor->getHandle() )
                            {
                                handle->setName( name );
                            }

                            // auto selection = selectionManager->getSelection();
                            // if (!selection.empty())
                            //{
                            //	for (auto selected : selection)
                            //	{
                            //		if (selected->isDerived<IActor>())
                            //		{
                            //			auto selectedActor =
                            // fb::static_pointer_cast<IActor>(selected);
                            //			selectedActor->addChild(actor);
                            //		}
                            //	}
                            // }
                            // else
                            {
                                scene->addActor( actor );
                                // scene->registerAllUpdates(actor);
                            }

                            if( applicationManager->isPlaying() )
                            {
                                actor->setState( IActor::State::Play );
                            }
                            else
                            {
                                actor->setState( IActor::State::Edit );
                            }

                            uiManager->rebuildSceneTree();
                            return actor;
                        }
                    }
                }
            }
            else
            {
                auto actor = sceneManager->createActor();

                return actor;
            }
        }
        break;
        case ActorType::Button:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Button" );
            actor->setName( name );

            auto size = Vector2F( 300, 100 );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                canvasTransform->setSize( size );

                canvasTransform->setHorizontalAlignment(
                    scene::LayoutTransform::HorizontalAlignment::CENTER );
                canvasTransform->setVerticalAlignment(
                    scene::LayoutTransform::VerticalAlignment::CENTER );
            }

            //if( auto image = actor->addComponent<scene::Image>() )
            //{
            //    image->setTextureName( "panel.png" );
            //    //image->setTint(ColourF(0.0f, 0.5f, 0.0f, 1.0f));
            //}

            auto button = actor->addComponent<scene::Button>();
            button->setCascadeInput( false );

            auto material = actor->addComponent<scene::Material>();
            if( material )
            {
                // material->setMainTexturePath("Rounded Filled 256px.png");
                // material->setTint(ColourF(0.0f, 0.5f, 0.0f, 1.0f));
            }

            //auto actorText = sceneManager->createActor();
            //actor->addChild( actorText );

            //if( auto textCanvasTransform = actorText->addComponent<scene::LayoutTransform>() )
            //{
            //    auto textSize = Vector2F( 100, 20 );
            //    textCanvasTransform->setSize( textSize );

            //    textCanvasTransform->setHorizontalAlignment(
            //        scene::LayoutTransform::HorizontalAlignment::CENTER );
            //    textCanvasTransform->setVerticalAlignment(
            //        scene::LayoutTransform::VerticalAlignment::CENTER );
            //}

            //if( auto text = actorText->addComponent<scene::Text>() )
            //{
            //    text->setText( "Button" );

            //    auto textName = String( "Text" );
            //    actorText->setName( textName );
            //}

            return actor;
        }
        break;
        case ActorType::Camera:
        {
            auto actor = application->createDefaultCamera( false );
            return actor;
        }
        break;
        case ActorType::Car:
        {
            auto actor = application->createDefaultVehicle( false );
            return actor;
        }
        break;
        case ActorType::Canvas:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Canvas" );
            actor->setName( name );

            actor->addComponent<scene::Layout>();
            actor->addComponent<scene::LayoutTransform>();

            return actor;
        }
        break;
        case ActorType::Checkbox:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Checkbox" );
            actor->setName( name );

            auto size = Vector2F( 300, 100 );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                canvasTransform->setSize( size );
            }

            //actor->addComponent<scene::ImageComponent>();
            auto checkbox = actor->addComponent<scene::Toggle>();
            checkbox->setCascadeInput( false );
            //checkbox->setCaption( "Checkbox" );

            auto material = actor->addComponent<scene::Material>();
            if( material )
            {
                // material->setMainTexturePath("Rounded Filled 256px.png");
                // material->setTint(ColourF(0.0f, 0.5f, 0.0f, 1.0f));
            }

            return actor;
        }
        break;
        case ActorType::Cube:
        {
            auto actor = application->createDefaultCube( false );
            return actor;
        }
        break;
        case ActorType::CubeMesh:
        {
            auto actor = application->createDefaultCubeMesh( false );
            return actor;
        }
        break;
        case ActorType::Cubemap:
        {
            auto actor = application->createDefaultCubemap( false );
            return actor;
        }
        break;
        case ActorType::CubeGround:
        {
            auto actor = application->createDefaultCube( false );
            return actor;
        }
        break;
        case ActorType::DirectionalLight:
        {
            return application->createDirectionalLight( false );
        }
        break;
        case ActorType::Panel:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Panel" );
            actor->setName( name );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                auto size = Vector2F( 1920, 1080 );
                canvasTransform->setSize( size );
            }

            actor->addComponent<scene::Image>();

            auto material = actor->addComponent<scene::Material>();
            if( material )
            {
                // material->setMainTexturePath("BumpyMetal.jpg");
                // material->setTint(ColourF(0.5f, 0.5f, 0.5f, 1.0f));
            }

            return actor;
        }
        break;
        case ActorType::ParticleSystem:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Particle System" );
            actor->setName( name );

            auto ps = actor->addComponent<scene::ParticleSystem>();

            return actor;
        }
        break;
        case ActorType::ParticleSystemSmoke:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Particle System Smoke" );
            actor->setName( name );

            auto ps = actor->addComponent<scene::ParticleSystem>();

            return actor;
        }
        break;
        case ActorType::ParticleSystemSand:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Particle System Sand" );
            actor->setName( name );

            auto ps = actor->addComponent<scene::ParticleSystem>();

            return actor;
        }
        break;
        case ActorType::ProceduralScene:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "ProceduralScene" );
            actor->setName( name );

            //auto proceduralScene = actor->addComponent<scene::ProceduralScene>();
            //FB_ASSERT( proceduralScene );

            return actor;
        }
        break;
        case ActorType::PointLight:
        {
            return application->createPointLight( false );
        }
        break;
        case ActorType::PhysicsCube:
        {
            return application->createDefaultGround( false );
        }
        break;
        case ActorType::Skybox:
        {
            return application->createDefaultSky( false );
        }
        break;
        case ActorType::SimpleButton:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Button" );
            actor->setName( name );

            auto size = Vector2F( 300, 100 );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                canvasTransform->setSize( size );
            }

            //actor->addComponent<scene::ImageComponent>();
            auto button = actor->addComponent<scene::Button>();
            button->setCascadeInput( false );
            button->setSimpleButton( true );
            button->setCaption( "Button" );

            auto material = actor->addComponent<scene::Material>();
            if( material )
            {
                // material->setMainTexturePath("Rounded Filled 256px.png");
                // material->setTint(ColourF(0.0f, 0.5f, 0.0f, 1.0f));
            }

            return actor;
        }
        break;
        case AddActorCmd::ActorType::Slider:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Slider" );
            actor->setName( name );

            auto size = Vector2F( 300, 100 );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                canvasTransform->setSize( size );
            }

            auto slider = actor->addComponent<scene::Slider>();

            return actor;
        }
        break;
        case ActorType::Text:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Text" );
            actor->setName( name );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                auto size = Vector2F( 300, 100 );
                canvasTransform->setSize( size );
            }

            auto text = actor->addComponent<scene::Text>();
            if( text )
            {
                text->setText( "Text" );
            }

            return actor;
        }
        break;
        case ActorType::ToggleButton:
        case ActorType::ToggleWithText:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Toggle Button" );
            actor->setName( name );

            auto canvasTransform = actor->addComponent<scene::LayoutTransform>();
            if( canvasTransform )
            {
                auto pos = Vector2F( 0, 0 );
                canvasTransform->setPosition( pos );

                auto size = Vector2F( 100, 10 );
                canvasTransform->setSize( size );
            }

            auto bgImage = actor->addComponent<scene::Image>();
            bgImage->setTextureName( "rounded_filled_1024.png" );

            auto toggle = actor->addComponent<scene::Toggle>();

            auto toggleActor = sceneManager->createActor();
            actor->addChild( toggleActor );

            auto toggleActorName = String( "Toggle" );
            toggleActor->setName( toggleActorName );

            auto toggleImage = toggleActor->addComponent<scene::Image>();
            toggleImage->setTextureName( "circle_filled_1024.png" );

            auto toggleCanvasTransform = toggleActor->addComponent<scene::LayoutTransform>();
            if( toggleCanvasTransform )
            {
                auto size = Vector2F( 40 * ( 1920.0f / 1080.0f ), 40 );
                toggleCanvasTransform->setSize( size );
            }

            toggle->setToggleTransform( toggleCanvasTransform );

            if( actorType == ActorType::ToggleWithText )
            {
                auto textActor = sceneManager->createActor();
                actor->addChild( textActor );

                auto textActorName = String( "Text" );
                textActor->setName( textActorName );

                auto text = textActor->addComponent<scene::Text>();
                text->setText( "Toggle" );

                auto textCanvasTransform = textActor->addComponent<scene::LayoutTransform>();
                if( textCanvasTransform )
                {
                    auto pos = Vector2F( -150, 0 );
                    textCanvasTransform->setPosition( pos );

                    auto size = Vector2F( 120, 40 );
                    textCanvasTransform->setSize( size );
                }
            }

            toggleActor->addComponent<scene::Material>();

            return actor;
        }
        break;
        case ActorType::Terrain:
        {
            auto actor = sceneManager->createActor();
            actor->setStatic( true );

            auto name = String( "Terrain" );
            actor->setName( name );

            //auto terrainDirector = fb::make_ptr<Director>();
            //terrainDirector->saveToFile( "Terrain.resource" );

            auto terrain = actor->addComponent<scene::TerrainSystem>();
            FB_ASSERT( terrain );

            auto collisionTerrain = actor->addComponent<scene::CollisionTerrain>();
            FB_ASSERT( collisionTerrain );

            auto rigidbody = actor->addComponent<scene::Rigidbody>();
            FB_ASSERT( rigidbody );

            auto material = actor->addComponent<scene::Material>();
            if( material )
            {
                material->setMaterialPath( "DefaultTerrain.mat" );
            }

            return actor;
        }
        break;
        case ActorType::Vehicle:
        {
            auto actor = application->createDefaultVehicle( false );
            return actor;
        }
        break;
        default:
        {
            auto actor = sceneManager->createActor();

            auto name = String( "Actor" );
            actor->setName( name );
            return actor;
        }
        }

        return nullptr;
    }

    auto AddActorCmd::getActor() const -> SmartPtr<IActor>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_actor;
    }

    void AddActorCmd::setActor( SmartPtr<IActor> actor )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_actor = actor;
    }

    auto AddActorCmd::getParent() const -> SmartPtr<IActor>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_parent;
    }

    void AddActorCmd::setParent( SmartPtr<IActor> parent )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_parent = parent;
    }

    auto AddActorCmd::getActorType() const -> AddActorCmd::ActorType
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_actorType;
    }

    void AddActorCmd::setActorType( ActorType actorType )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_actorType = actorType;
    }

    auto AddActorCmd::getFilePath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_filePath;
    }

    void AddActorCmd::setFilePath( const String &filePath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        FB_ASSERT( !Path::isPathAbsolute( filePath ) );
        m_filePath = filePath;
    }

}  // namespace fb::editor
