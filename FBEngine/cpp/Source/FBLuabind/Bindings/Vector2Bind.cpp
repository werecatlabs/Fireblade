#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/Vector2Bind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include "FBLuabind/FBLuabindTypes.h"
#include <FBCore/FBCore.h>
#include <FBLua/LuaManager.h>
#include "luabind/operator.hpp"

namespace fb
{

    s32 vector2i_getX( const Vector2I &vec )
    {
        return vec.X();
    }

    s32 vector2i_getY( const Vector2I &vec )
    {
        return vec.Y();
    }

    f32 vector2f_getX( const Vector2F &vec )
    {
        return vec.X();
    }

    f32 vector2f_getY( const Vector2F &vec )
    {
        return vec.Y();
    }

    f64 vector2d_getX( const Vector2D &vec )
    {
        return vec.X();
    }

    f64 vector2d_getY( const Vector2D &vec )
    {
        return vec.Y();
    }

    template <class T>
    int PointToWatch( const Vector2<T> &vec )
    {
        auto engine = core::IApplicationManager::instance();
        LuaManagerPtr luaMgr = engine->getScriptManager();
        lua_State *L = luaMgr->getLuaState();

        lua_newtable( L );
        int table = lua_gettop( L );

        lua_pushstring( L, "x" );
        lua_pushnumber( L, vec.X() );
        lua_settable( L, table );
        lua_pushstring( L, "y" );
        lua_pushnumber( L, vec.Y() );
        lua_settable( L, table );

        return 1;
    }

    void bindVector2( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<Vector2I>( "Vector2I" )
                        .def( constructor<>() )
                        .def( constructor<Vector2I &>() )
                        .def( constructor<lua_Integer, lua_Integer>() )
                        .def( "X", vector2i_getX )
                        .def( "Y", vector2i_getY )
                        .def( "__towatch", PointToWatch<s32> )

                        // Operators
                        .def( self == other<Vector2I>() )
                        .def( self + other<Vector2I>() )
                        .def( self - other<Vector2I>() )
                        .def( self * other<Vector2I>() )
                        .def( self * lua_Integer() )
                        .def( tostring( self ) )

        ];

        module( L )[class_<Vector2F>( "Vector2F" )
                        .def( constructor<>() )
                        .def( constructor<Vector2F &>() )
                        .def( constructor<lua_Number, lua_Number>() )
                        .def( "__towatch", PointToWatch<f32> )
                        .def( "X", vector2f_getX )
                        .def( "Y", vector2f_getY )

                        .def( "dotProduct", &Vector2F::dotProduct )
                        .def( "normalise", &Vector2F::normalise )
                        .def( "normaliseLength", &Vector2F::normaliseLength )
                        .def( "normaliseCopy", &Vector2F::normaliseCopy )
                        .def( "getLength", &Vector2F::length )
                        .def( "rotateBy", &Vector2F::rotateBy )

                        // Operators
                        .def( self == other<Vector2F>() )
                        .def( self + other<Vector2F>() )
                        .def( self - other<Vector2F>() )
                        .def( self * other<Vector2F>() )
                        .def( self * f32() )
                        .def( tostring( self ) )];

        //module(L)
        //	[
        //		class_<Vector2D>( "Vector2D" )
        //		.def(constructor<>())
        //		.def(constructor<Vector2D&>())
        //		.def(constructor<f64, f64>())
        //		.def("__towatch", PointToWatch<f64> )

        //		.def("X", vector2d_getX )
        //		.def("Y", vector2d_getY )

        //		.def("dotProduct", &Vector2D::dotProduct )
        //		.def("normalize", &Vector2D::normalize )
        //		.def("normalizeLength", &Vector2D::normalizeLength )
        //		.def("normaliseCopy", &Vector2D::normaliseCopy )
        //		.def("getLength", &Vector2D::getLength )
        //		.def("rotateBy", &Vector2D::rotateBy )

        //		// Operators
        //		.def( self == other<Vector2D>() )
        //		.def( self + other<Vector2D>() )
        //		.def( self - other<Vector2D>() )
        //		.def( self * other<Vector2D>() )
        //		.def( self * f64() )
        //		.def(tostring(self))
        //	];

        LUA_CONST_START( Vector2I )
        LUA_CONST( Vector2I, ZERO );
        LUA_CONST( Vector2I, UNIT_X );
        LUA_CONST( Vector2I, UNIT_Y );
        LUA_CONST( Vector2I, UNIT );
        LUA_CONST_END;

        LUA_CONST_START( Vector2F )
        LUA_CONST( Vector2F, ZERO );
        LUA_CONST( Vector2F, UNIT_X );
        LUA_CONST( Vector2F, UNIT_Y );
        LUA_CONST( Vector2F, UNIT );
        LUA_CONST_END;

        //LUA_CONST_START( Vector2D )
        //LUA_CONST( Vector2D, ZERO);
        //LUA_CONST( Vector2D, UNIT_X);
        //LUA_CONST( Vector2D, UNIT_Y);
        //LUA_CONST( Vector2D, UNIT);
        //LUA_CONST_END;
    }

}  // end namespace fb
