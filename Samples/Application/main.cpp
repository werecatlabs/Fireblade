#include "Application.h"

//--------------------------------------------
int main( int argc, char *argv[] )
{
    // Create application object
    Application app;

#if 1
    try
    {
        app.load( nullptr );
        app.run();
    }
    catch(Exception &e)
    {
        // fb::MessageBox::show( e.what().c_str() );
    }

#else
    app.load();
    app.run();
#endif

    return 0;
}
