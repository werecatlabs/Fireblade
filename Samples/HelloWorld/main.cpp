#include "HelloWorld.h"

int main( int argc, char *argv[] )
{
    using namespace fb;

    // Create application object
    HelloWorld app;

    try
    {
        app.setActiveThreads( 4 );
        app.load( nullptr );
        app.run();
        app.unload( nullptr );
    }
    catch( Exception &e )
    {
        fb::MessageBoxUtil::show( e.what() );
    }

    return 0;
}
