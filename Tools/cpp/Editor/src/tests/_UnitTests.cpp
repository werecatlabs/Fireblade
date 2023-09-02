#include <GameEditorPCH.h>
#include <FBCore/FBCore.h>
#include <EditorApplication.h>

#if FB_EDITOR_TESTS

#    define BOOST_TEST_MAIN
#    define BOOST_TEST_MODULE FB_Editor_Tests
#    include <boost/test/included/unit_test.hpp>

using namespace fb;

BOOST_AUTO_TEST_CASE( editor_start )
{
    fb::editor::EditorApplication app;
    app.load( nullptr );
    //app.run();

    auto applicationManager = core::IApplicationManager::instance();
    applicationManager->setQuit( true );
    applicationManager->setRunning( false );

    app.unload( nullptr );
}

BOOST_AUTO_TEST_CASE( editor_run )
{
    try
    {
        fb::editor::EditorApplication app;

        const auto threads = fb::Thread::hardware_concurrency();
        app.setActiveThreads( threads );

        app.load( nullptr );
        //app.run();

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto timer = applicationManager->getTimer();
        auto endTime = timer->now() + 15.0;

        while( applicationManager->isRunning() )
        {
            try
            {
                if( taskManager )
                {
                    taskManager->update();
                }

                Thread::yield();

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

        app.unload( nullptr );
    }
    catch( std::exception &e )
    {
        FB_LOG_EXCEPTION( e );
    }
}

BOOST_AUTO_TEST_CASE( editor_run_single_threaded )
{
    try
    {
        fb::editor::EditorApplication app;
        app.setActiveThreads( 0 );
        app.load( nullptr );
        //app.run();

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto threadPool = applicationManager->getThreadPool();
        FB_ASSERT( threadPool );

        BOOST_CHECK( threadPool->getNumThreads() == 0 );

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

        app.unload( nullptr );
    }
    catch( std::exception &e )
    {
        FB_LOG_EXCEPTION( e );
    }
}

#endif
