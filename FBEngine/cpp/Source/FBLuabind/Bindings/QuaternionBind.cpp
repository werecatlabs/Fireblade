#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/QuaternionBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "luabind/operator.hpp"

namespace fb
{

    /*f32 quaternion_getW(const Quaternion& vec)
    {
        return vec.W();
    }


    f32 quaternion_getX(const Quaternion& vec)
    {
        return vec.X();
    }

    f32 quaternion_getY(const Quaternion& vec)
    {
        return vec.Y();
    }

    f32 quaternion_getZ(const Quaternion& vec)
    {
        return vec.Z();
    }*/

    f32 quaternion_getW( const QuaternionF &vec )
    {
        return vec.W();
    }

    f32 quaternion_getX( const QuaternionF &vec )
    {
        return vec.X();
    }

    f32 quaternion_getY( const QuaternionF &vec )
    {
        return vec.Y();
    }

    f32 quaternion_getZ( const QuaternionF &vec )
    {
        return vec.Z();
    }

    void bindQuaternion( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<QuaternionF>( "Quaternion" )
                        .def( constructor<>() )
                        .def( constructor<QuaternionF &>() )
                        .def( constructor<f32, f32, f32, f32>() )
                        .def( "W", quaternion_getW )
                        .def( "X", quaternion_getX )
                        .def( "Y", quaternion_getY )
                        .def( "Z", quaternion_getZ )

                        // Operators
                        .def( self + other<QuaternionF>() )
                        .def( self - other<QuaternionF>() )
                        .def( self * other<QuaternionF>() )
                        .def( self * f32() )
                        .def( tostring( self ) )];
    }

}  // end namespace fb
