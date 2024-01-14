#include <GameEditorPCH.h>
#include <FBCore/FBCore.h>

#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include "commands/DragDropActorCmd.h"
#include "commands/AddActorCmd.h"
#include <EditorApplication.h>

#if FB_EDITOR_TESTS
#    include <boost/test/unit_test.hpp>

using namespace fb;
using namespace fb::editor;

BOOST_AUTO_TEST_CASE( play_mode )
{
    auto app = fb::make_ptr<fb::editor::EditorApplication>();

    try
    {
        app->load( nullptr );
        //app.run();

        app->enterPlayMode();

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        while( applicationManager->isRunning() )
        {
            try
            {
                if( taskManager )
                {
                    taskManager->update();
                }

                Thread::yield();

                auto timer = applicationManager->getTimer();
                auto now = timer->now();
                if( now > 10.0 )
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
    }
    catch( std::exception &e )
    {
        FB_LOG_EXCEPTION( e );
    }

    app->unload( nullptr );
    app = nullptr;
}

BOOST_AUTO_TEST_CASE( play_stop_mode )
{
    auto app = fb::make_ptr<fb::editor::EditorApplication>();

    try
    {
        app->load( nullptr );
        //app->run();

        auto isPlayMode = false;

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

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

                if( isPlayMode == false )
                {
                    app->enterPlayMode();
                    isPlayMode = true;
                }
                else
                {
                    app->stopPlayMode();
                    isPlayMode = false;

                    applicationManager->setQuit( true );
                    applicationManager->setRunning( false );
                }

                auto now = timer->now();
                if( now > endTime )
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
    }
    catch( std::exception &e )
    {
        FB_LOG_EXCEPTION( e );
    }

    app->unload( nullptr );
    app = nullptr;
}

#endif
