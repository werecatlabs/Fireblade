#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/Vector3Bind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include "FBLuabind/FBLuabindTypes.h"
#include <FBCore/FBCore.h>
#include <luabind/operator.hpp>
#include "FBLuabind/Helpers/Vector3Helper.h"

namespace fb
{

    void bindVector3( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<Vector3I>( "Vector3I" )
                        .def( constructor<>() )
                        .def( constructor<Vector3I &>() )
                        .def( constructor<s32, s32, s32>() )

                        .def( "X", Vector3Helper<s32>::setX )
                        .def( "Y", Vector3Helper<s32>::setY )
                        .def( "Z", Vector3Helper<s32>::setZ )

                        .def( "X", Vector3Helper<s32>::getX )
                        .def( "Y", Vector3Helper<s32>::getY )
                        .def( "Z", Vector3Helper<s32>::getZ )

                        // Operators
                        .def( self + other<Vector3I>() )
                        .def( self - other<Vector3I>() )
                        .def( self * other<Vector3I>() )
                        .def( self * f32() )
                        .def( tostring( self ) )];

        module( L )[class_<Vector3F>( "Vector3F" )
                        .def( constructor<>() )
                        .def( constructor<f32, f32, f32>() )
                        .def( tostring( const_self ) )
                        .def_readwrite( "x", &Vector3F::x )
                        .def_readwrite( "y", &Vector3F::y )
                        .def_readwrite( "z", &Vector3F::z )
                        .def( self + other<Vector3F>() )
                        .def( self - other<Vector3F>() )
                        .def( self * other<f32>() )
                        .def( self / other<f32>() )
                        .def( self == other<Vector3F>() )
                        //.def( self != other<Vector3F>() )
                        .def( self < other<Vector3F>() )
                        .def( self <= other<Vector3F>() )
                        //.def( self > other<Vector3F>() )
                        //.def( self >= other<Vector3F>() )
                        .def( "makeFloor", &Vector3F::makeFloor )
                        .def( "makeCeil", &Vector3F::makeCeil )
                        .def( "perpendicular", &Vector3F::perpendicular )
                        .def( "crossProduct", &Vector3F::crossProduct )
                        //.def( "randomDeviant", &Vector3F::randomDeviant )
                        //.def( "getRotationTo", &Vector3F::getRotationTo )
                        //.def( "angleBetween", &Vector3F::angleBetween )
                        //.def( "distance", &Vector3F::distance )
                        //.def( "squaredDistance", &Vector3F::squaredDistance )
                        .def( "dotProduct", &Vector3F::dotProduct )
                        .def( "normalise", &Vector3F::normalise )
                        .def( "midPoint", &Vector3F::midPoint )
                        .def( "perpendicular", &Vector3F::perpendicular )
                        //.def( "reflect", &Vector3F::reflect )
                        .def( "isZeroLength", &Vector3F::isZeroLength )
                        .def( "normalisedCopy", &Vector3F::normaliseCopy )
                        .def( "crossProduct", &Vector3F::crossProduct )
                        //.def( "getRotationTo", &Vector3F::getRotationTo )
                        //.def( "getRotationTo", &Vector3F::getRotationTo )
                        //.def( "getRotationTo", &Vector3F::getRotationTo )
                        ];

        module( L )[class_<Vector3D>( "Vector3D" )
                        .def( constructor<>() )
                        .def( constructor<Vector3D &>() )
                        .def( constructor<f64, f64, f64>() )

                        .def( "normalise", &Vector3D::normalise )

                        .def( "X", Vector3Helper<f64>::setX )
                        .def( "Y", Vector3Helper<f64>::setY )
                        .def( "Z", Vector3Helper<f64>::setZ )

                        .def( "X", Vector3Helper<f64>::getX )
                        .def( "Y", Vector3Helper<f64>::getY )
                        .def( "Z", Vector3Helper<f64>::getZ )

                        // Operators
                        .def( self + other<Vector3D>() )
                        .def( self - other<Vector3D>() )
                        .def( self * other<Vector3D>() )
                        .def( self * f64() )
                        .def( tostring( self ) )];

        LUA_CONST_START( Vector3I )
        LUA_CONST( Vector3I, ZERO );
        LUA_CONST( Vector3I, UNIT_X );
        LUA_CONST( Vector3I, UNIT_Y );
        LUA_CONST( Vector3I, UNIT );
        LUA_CONST_END;

        LUA_CONST_START( Vector3F )
        LUA_CONST( Vector3F, ZERO );
        LUA_CONST( Vector3F, UNIT_X );
        LUA_CONST( Vector3F, UNIT_Y );
        LUA_CONST( Vector3F, UNIT );
        LUA_CONST_END;

        LUA_CONST_START( Vector3D )
        LUA_CONST( Vector3D, ZERO );
        LUA_CONST( Vector3D, UNIT_X );
        LUA_CONST( Vector3D, UNIT_Y );
        LUA_CONST( Vector3D, UNIT );
        LUA_CONST_END;
    }

}  // end namespace fb
