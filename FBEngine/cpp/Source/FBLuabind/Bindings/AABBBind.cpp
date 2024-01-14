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
    }

}  // end namespace fb
