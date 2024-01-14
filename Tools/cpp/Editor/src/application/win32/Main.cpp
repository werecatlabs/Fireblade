#include <GameEditorPCH.h>
#include <EditorApplication.h>
#include "FBCore/FBCore.h"

#if !FB_EDITOR_TESTS
//#ifdef FB_PLATFORM_WIN32
//int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
//{
//    fb::editor::Application app;
//    app.load( nullptr );
//    app.run();
//    return 0;
//}
//#else
auto main() -> int
{
    auto app = fb::make_ptr<fb::editor::EditorApplication>();

    const auto threads = fb::Thread::hardware_concurrency();
    app->setActiveThreads( threads );
    //app->setActiveThreads( 0 );

    app->load( nullptr );
    app->run();
    app->unload( nullptr );
    return 0;
}
//#endif
#endif
