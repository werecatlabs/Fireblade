#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/Vector4Bind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include "FBLuabind/FBLuabindTypes.h"
#include <FBCore/FBCore.h>
#include <luabind/operator.hpp>
#include "FBLuabind/Helpers/Vector4Helper.h"

namespace fb
{

    void bindVector4( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<Vector4I>( "Vector4I" )
                        .def( constructor<>() )
                        .def( constructor<Vector4I &>() )
                        .def( constructor<s32, s32, s32, s32>() )

                        .def( "X", Vector4Helper<s32>::setX )
                        .def( "Y", Vector4Helper<s32>::setY )
                        .def( "Z", Vector4Helper<s32>::setZ )

                        .def( "X", Vector4Helper<s32>::getX )
                        .def( "Y", Vector4Helper<s32>::getY )
                        .def( "Z", Vector4Helper<s32>::getZ )

                        // Operators
                        .def( self + other<Vector4I>() )
                        .def( self - other<Vector4I>() )
                        .def( self * other<Vector4I>() )
                        .def( self * f32() )];

        module( L )[class_<Vector4F>( "Vector4F" )
                        .def( constructor<>() )
                        .def( constructor<Vector4F &>() )
                        .def( constructor<f32, f32, f32, f32>() )

                        //.def("normalise", &Vector4F::normalise)
                        //.def("getLength", &Vector4F::getLength)
                        //.def("getLengthSquared", &Vector4F::getLengthSQ)

                        .def( "X", Vector4Helper<f32>::setX )
                        .def( "Y", Vector4Helper<f32>::setY )
                        .def( "Z", Vector4Helper<f32>::setZ )

                        .def( "X", Vector4Helper<f32>::getX )
                        .def( "Y", Vector4Helper<f32>::getY )
                        .def( "Z", Vector4Helper<f32>::getZ )

                        // Operators
                        .def( self + other<Vector4F>() )
                        .def( self - other<Vector4F>() )
                        .def( self * other<Vector4F>() )
                        .def( self * f32() )];

        module( L )[class_<Vector4D>( "Vector4D" )
                        .def( constructor<>() )
                        .def( constructor<Vector4D &>() )
                        .def( constructor<f64, f64, f64, f64>() )

                        //.def("normalise", &Vector4D::normalise)

                        .def( "X", Vector4Helper<f64>::setX )
                        .def( "Y", Vector4Helper<f64>::setY )
                        .def( "Z", Vector4Helper<f64>::setZ )

                        .def( "X", Vector4Helper<f64>::getX )
                        .def( "Y", Vector4Helper<f64>::getY )
                        .def( "Z", Vector4Helper<f64>::getZ )

                        // Operators
                        .def( self + other<Vector4D>() )
                        .def( self - other<Vector4D>() )
                        .def( self * other<Vector4D>() )
                        .def( self * f64() )];

        LUA_CONST_START( Vector4I )
        LUA_CONST( Vector4I, ZERO );
        LUA_CONST( Vector4I, UNIT_X );
        LUA_CONST( Vector4I, UNIT_Y );
        LUA_CONST( Vector4I, UNIT );
        LUA_CONST_END;

        LUA_CONST_START( Vector4F )
        LUA_CONST( Vector4F, ZERO );
        LUA_CONST( Vector4F, UNIT_X );
        LUA_CONST( Vector4F, UNIT_Y );
        LUA_CONST( Vector4F, UNIT );
        LUA_CONST_END;

        LUA_CONST_START( Vector4D )
        LUA_CONST( Vector4D, ZERO );
        LUA_CONST( Vector4D, UNIT_X );
        LUA_CONST( Vector4D, UNIT_Y );
        LUA_CONST( Vector4D, UNIT );
        LUA_CONST_END;
    }

}  // end namespace fb
