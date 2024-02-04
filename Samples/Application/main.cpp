#include "Application.h"

int main( int argc, char *argv[] )
{
    using namespace fb;

    // Create application object
    Application app;

    try
    {
        app.load( nullptr );
        app.run();
    }
    catch( Exception &e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
