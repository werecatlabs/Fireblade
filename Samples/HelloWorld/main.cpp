#include "HelloWorld.h"

int main( int argc, char *argv[] )
{
    using namespace fb;

    // Create application object
    HelloWorld app;

#if 1
    try
    {
        app.setActiveThreads( 2 );
        app.load( nullptr );
        app.run();
        app.unload( nullptr );
    }
    catch( Exception &e )
    {
        // fb::MessageBox::show( e.what().c_str() );
    }
#else
    app.load( nullptr );
    app.run();
    app.unload( nullptr );
#endif

    return 0;
}
