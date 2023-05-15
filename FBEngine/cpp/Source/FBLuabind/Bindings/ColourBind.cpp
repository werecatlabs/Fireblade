#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ColourBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    void bindSColorf( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<ColourF>( "ColourF" )
                        .def( constructor<>() )
                        .def( constructor<ColourF &>() )
                        .def( constructor<f32, f32, f32, f32>() )];
    }

}  // end namespace fb
