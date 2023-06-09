#include <GameEditorPCH.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include "commands/DragDropActorCmd.h"
#include "commands/AddActorCmd.h"
#include <Application.h>

#if FB_EDITOR_TESTS
#    include <boost/test/unit_test.hpp>

using namespace fb;
using namespace fb::editor;

BOOST_AUTO_TEST_CASE( ui_test )
{
    fb::editor::Application app;

    try
    {        
        app.load( nullptr );
        //app.run();

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto timer = applicationManager->getTimer();
        auto endTime = timer->now() + 10.0;

        while( applicationManager->isRunning() )
        {
            try
            {
                if( taskManager )
                {
                    taskManager->update();
                }

                Thread::yield();

                if( timer->now() > endTime )
                {
                    break;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        if( auto actor = sceneManager->createActor() )
        {
            auto canvas = actor->addComponent<scene::Layout>();
        }

        endTime = timer->now() + 10.0;

        while( applicationManager->isRunning() )
        {
            try
            {
                if( taskManager )
                {
                    taskManager->update();
                }

                Thread::yield();

                if( timer->now() > endTime )
                {
                    break;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        applicationManager->setQuit( true );
        applicationManager->setRunning( false );

        app.unload( nullptr );
    }
    catch( std::exception &e )
    {
        FB_LOG_EXCEPTION( e );
    }
}

#endif
