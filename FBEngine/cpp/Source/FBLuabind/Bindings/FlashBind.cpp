#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/FlashBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    //IFlashMovie *addFlashMovie( IFlashPlayer *player, const char *fileName )
    //{
    //    FlashMoviePtr flMovie = player->addMovie( fileName );
    //    return flMovie.get();
    //}

    //IFlashMovieClip *_getMovieClip( IFlashMovie *movie )
    //{
    //    return movie->getMovieClip().get();
    //}

    //IFlashInstance *_getInstanceFromMovie( IFlashMovie *movie, const char *name )
    //{
    //    return movie->getInstance( name ).get();
    //}

    //IFlashInstance *_getInstance( IFlashMovieClip *movieClip, const char *name )
    //{
    //    return movieClip->getInstance( name ).get();
    //}

    ////
    //// FlashControlManager
    ////
    //FlashControlPtr _createFlashOverlay( IFlashControlManager *mgr, const char *name, IViewport *vp,
    //                                     const Vector2I &size, const Vector2I &position )
    //{
    //    return nullptr;  // mgr->createFlashOverlay(name, vp, size, position);
    //}

    //Parameter _callFunction( IFlashControl *ctrl, const char *functionName )
    //{
    //    Parameters params;
    //    return ctrl->_callFunction( functionName, params );
    //}

    //Parameter _callFunction1( IFlashControl *ctrl, const char *functionName, Parameter param0 )
    //{
    //    Parameters params;
    //    params.resize( 1 );
    //    params[0] = param0;
    //    return ctrl->_callFunction( functionName, params );
    //}

    //Parameter _callFunction2( IFlashControl *ctrl, const char *functionName, Parameter param0,
    //                          Parameter param1 )
    //{
    //    Parameters params;
    //    params.resize( 2 );
    //    params[0] = param0;
    //    params[1] = param1;
    //    return ctrl->_callFunction( functionName, params );
    //}

    //Parameter _callFunction3( IFlashControl *ctrl, const char *functionName, Parameter param0,
    //                          Parameter param1, Parameter param2 )
    //{
    //    Parameters params;
    //    params.resize( 2 );
    //    params[0] = param0;
    //    params[1] = param1;
    //    params[2] = param2;
    //    return ctrl->_callFunction( functionName, params );
    //}

    void bindFlash( lua_State *L )
    {
        //using namespace luabind;

        //module( L )[class_<IFlashControl, IScriptObject, FlashControlPtr>( "FlashControl" )
        //                .def( "load", &IFlashControl::load )
        //                .def( "setTransparent", &IFlashControl::setTransparent )
        //                .def( "isVisible", &IFlashControl::isVisible )
        //                .def( "setVisible", &IFlashControl::setVisible )
        //                .def( "_callFunction", _callFunction )
        //                .def( "_callFunction", _callFunction1 )
        //                .def( "_callFunction", _callFunction2 )
        //                .def( "_callFunction", _callFunction3 )
        //            //.def("play", &IFlashControl::play )
        //            //.def("stop", &IFlashControl::stop )
        //];

        //module( L )[class_<IFlashControlManager, IScriptObject, FlashControlManagerPtr>(
        //                "FlashControlManager" )
        //                .def( "createFlashOverlay", _createFlashOverlay )
        //                .def( "findFlashControl", &IFlashControlManager::findFlashControl )];

        //module( L )[class_<IFlashMovie, IScriptObject>( "FlashMovie" )
        //                .def( "play", &IFlashMovie::play )
        //                .def( "stop", &IFlashMovie::stop )
        //                .def( "getMainMovieClip", _getMovieClip )
        //                .def( "getInstance", _getInstanceFromMovie )];

        //module( L )[class_<IFlashInstance, IScriptObject>( "FlashInstance" )];

        //module( L )[class_<IFlashText, IScriptObject>( "FlashText" )];

        //module( L )[class_<IFlashMovieClip, IScriptObject>( "FlashMovieClip" )
        //                .def( "getInstance", _getInstance )];

        //module(
        //    L )[class_<IFlashPlayer, IScriptObject>( "FlashPlayer" ).def( "addMovie", addFlashMovie )];
    }

}  // end namespace fb
