#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/AABBBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    bool box2IntersectsBox( const AABB2F &boxA, const AABB2F &boxB )
    {
        return boxA.intersects( boxB );
    }

    void bindAABB( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<AABB2F>( "AABB2F" )
                        .def( constructor<>() )
                        .def( constructor<AABB2F &>() )
                        .def( constructor<f32, f32, f32, f32>() )
                        .def( constructor<Vector2F, Vector2F>() )

                        //.def("addInternalPoint", &rectf::addInternalPoint)
                        .def( "clipAgainst", &AABB2F::clipAgainst )
                        .def( "constrainTo", &AABB2F::constrainTo )
                        .def( "getArea", &AABB2F::getArea )
                        .def( "getCenter", &AABB2F::getCenter )
                        .def( "getHeight", &AABB2F::getHeight )
                        .def( "getSize", &AABB2F::getSize )
                        .def( "getWidth", &AABB2F::getWidth )
                        .def( "isPointInside", &AABB2F::isInside )
                        .def( "intersects", box2IntersectsBox )
                        .def( "isValid", &AABB2F::isValid )
                        .def( "repair", &AABB2F::repair )
                        .def( "getMin", &AABB2F::getMin )
                        .def( "setMin", &AABB2F::setMin )

                        .def( "getMax", &AABB2F::getMax )
                        .def( "setMax", &AABB2F::setMax )];

        //module(L)
        //	[
        //		class_<AABB2D>("aabbox2d")
        //		.def(constructor<>())
        //              .def(constructor<AABB2D&>())
        //              .def(constructor<f64, f64, f64, f64>())
        //		.def(constructor<Vector2D, Vector2D>())

        //		//.def("addInternalPoint", &rectf::addInternalPoint)
        //		.def("clipAgainst", &AABB2D::clipAgainst)
        //		.def("constrainTo", &AABB2D::constrainTo)
        //		.def("getArea", &AABB2D::getArea)
        //		.def("getCenter", &AABB2D::getCenter)
        //		.def("getHeight", &AABB2D::getHeight)
        //		.def("getSize", &AABB2D::getSize)
        //		.def("getWidth", &AABB2D::getWidth)
        //		.def("isPointInside", &AABB2D::isInside)
        //		.def("isRectCollided", &AABB2D::intersects)
        //		.def("isValid", &AABB2D::isValid)
        //		.def("repair", &AABB2D::repair)
        //		.def("getMin",&AABB2D::getMin)
        //		.def("setMin",&AABB2D::setMin)

        //		.def("getMax",&AABB2D::getMax)
        //		.def("setMax",&AABB2D::setMax)
        //	];

        module( L )[class_<AABB3F>( "AABB3F" )
                        //.def("addInternalBox", &AABB3F::merge)
                        //.def("addInternalPoint", &aabbox3f::addInternalPoint)
                        .def( "classifyPlaneRelation", &AABB3F::classifyPlaneRelation )
                        //.def("getArea", &aabbox3f::getArea)
                        .def( "getCenter", &AABB3F::getCenter )
                        .def( "getEdges", &AABB3F::getEdges )
                        .def( "getExtent", &AABB3F::getExtent )
                        .def( "getInterpolated", &AABB3F::getInterpolated )
                        //.def("getVolume", &aabbox3f::getVolume)
                        .def( "intersects", &AABB3F::intersects )
                        //.def("intersectsWithLine", &aabbox3f::intersectsWithLine)
                        .def( "isEmpty", &AABB3F::isEmpty )
                        .def( "isFullInside", &AABB3F::isFullInside )
                        .def( "isPointInside", &AABB3F::isPointInside )
                        .def( "isPointTotalInside", &AABB3F::isPointTotalInside )
                        .def( "repair", &AABB3F::repair )
                    //.def("reset", &aabbox3f::reset)
                    //.def_readwrite("MaxEdge",&aabbox3f::MaxEdge)
                    //.def_readwrite("MinEdge",&aabbox3f::MinEdge),
        ];

        //module(L)
        //	[
        //		class_<AABB3D>("aabbox3d")
        //		//.def("addInternalBox", &AABB3D::merge)
        //		//.def("addInternalPoint", &aabbox3d::addInternalPoint)
        //		.def("classifyPlaneRelation", &AABB3D::classifyPlaneRelation)
        //		//.def("getArea", &aabbox3d::getArea)
        //		.def("getCenter", &AABB3D::getCenter)
        //		.def("getEdges", &AABB3D::getEdges)
        //		.def("getExtent", &AABB3D::getExtent)
        //		.def("getInterpolated", &AABB3D::getInterpolated)
        //		//.def("getVolume", &aabbox3d::getVolume)
        //		.def("intersects", &AABB3D::intersects)
        //		//.def("intersectsWithLine", &aabbox3d::intersectsWithLine)
        //		.def("isEmpty", &AABB3D::isEmpty)
        //		.def("isFullInside", &AABB3D::isFullInside)
        //		.def("isPointInside", &AABB3D::isPointInside)
        //		.def("isPointTotalInside", &AABB3D::isPointTotalInside)
        //		.def("repair", &AABB3D::repair)
        //		//.def("reset", &aabbox3d::reset)
        //		//.def_readwrite("MaxEdge",&aabbox3d::MaxEdge)
        //		//.def_readwrite("MinEdge",&aabbox3d::MinEdge),
        //	];
    }

}  // end namespace fb
