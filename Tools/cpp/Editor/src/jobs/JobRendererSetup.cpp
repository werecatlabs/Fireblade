#include <GameEditorPCH.h>
#include <jobs/JobRendererSetup.h>
#include <FBCore/FBCore.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#    include <FBGraphicsOgreNext/FBGraphicsOgreNext.h>
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include <FBGraphicsOgre/FBGraphicsOgre.h>
#endif

namespace fb::editor
{

    JobRendererSetup::JobRendererSetup() = default;

    JobRendererSetup::~JobRendererSetup() = default;

    void JobRendererSetup::execute()
    {
        // auto applicationManager = core::ApplicationManager::instance();
        // FB_ASSERT(applicationManager);
        //
        // auto graphicsSystem = applicationManager->getGraphicsSystem();
        // FB_ASSERT(graphicsSystem);

        // graphicsSystem->load(nullptr);
        //
        // auto configuration = graphicsSystem->createConfiguration();
        // FB_ASSERT(configuration);

        ////setupResources();

        // configuration->setCreateWindow(false);
        // graphicsSystem->configure(configuration);

        ////graphicsSystem->loadResources();

        ////chooseSceneManager();

        ////Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();
        ////Ogre::MaterialManager::getSingletonPtr()->initialise();

        // static const auto RENDERER_READY_HASH = StringUtil::getHash("RendererReady");
        // auto stateManager = applicationManager->getStateManager();
        //
        // auto stateMessage = fb::make_ptr<StateMessage>();
        // stateMessage->setType(RENDERER_READY_HASH);

        // stateManager->sendMessage(Thread::Task::Render, stateMessage);
    }

    void JobRendererSetup::chooseSceneManager()
    {
        // try
        //{
        //	auto applicationManager = core::ApplicationManager::instance();
        //	auto& graphicsSystem = applicationManager->getGraphicsSystem();

        //	auto smgr = graphicsSystem->addSceneManager("DefaultSceneManager", "ViewSM");
        //	smgr->setAmbientLight(ColourF::White);

        //	//SmartPtr<IWindow> window = graphicsSystem->getDefaultWindow();
        //	//if ( window )
        //	//{
        //	//	SmartPtr<ICamera> camera = smgr->addCamera("DefaultWindowCamera");
        //	//	SmartPtr<ISceneNode> cameraNode = smgr->getRootSceneNode()->addChildSceneNode();
        //	//	cameraNode->attachObject(camera);
        //	//	cameraNode->setPosition(Vector3F::UNIT_Z * 250.0f);
        //	//	SmartPtr<IViewport> viewport = window->addViewport(0, camera);
        //	//	viewport->setBackgroundColour(ColourF(0.02, 0.85, 0.75, 1.0));
        //	//}

        //	//SmartPtr<render::IGraphicsSceneManager> particleSceneManager =
        // graphicsSystem->addSceneManager("DefaultSceneManager", "ParticleSceneManager");
        //}
        // catch (std::exception& e)
        //{
        //	FB_LOG_EXCEPTION(e);
        //	wxMessageBox(e.what());
        //}
    }

}  // namespace fb::editor
