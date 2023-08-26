#include <FBLuabind/FBLuabindPCH.h>
#include <FBLuabind/Bindings/MeshBind.h>
#include <FBLuabind/SmartPtrConverter.h>
#include <FBLuabind/ParamConverter.h>
#include <FBCore/FBCore.h>
#include <luabind/luabind.hpp>
#include <luabind/class.hpp>

namespace fb
{

    void bindMesh( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<ISubMesh, ISharedObject>( "ISubMesh" )
                        .def( "setMaterialName", &ISubMesh::setMaterialName )
                        .def( "getMaterialName", &ISubMesh::getMaterialName )
                        .def( "setVertexBuffer", &ISubMesh::setVertexBuffer )
                        .def( "getVertexBuffer", &ISubMesh::getVertexBuffer )
                        .def( "setIndexBuffer", &ISubMesh::setIndexBuffer )
                        .def( "getIndexBuffer", &ISubMesh::getIndexBuffer )
                        .def( "updateAABB", &ISubMesh::updateAABB )
                        .def( "getAABB", &ISubMesh::getAABB )
                        .def( "setAABB", &ISubMesh::setAABB )
                        .def( "clone", &ISubMesh::clone )
                        .def( "getUseSharedVertices", &ISubMesh::getUseSharedVertices )
                        .def( "setUseSharedVertices", &ISubMesh::setUseSharedVertices )
                        .def( "compare", &ISubMesh::compare )];

        module( L )[class_<IMesh, ISharedObject>( "IMesh" )
                        .def( "addSubMesh", &IMesh::addSubMesh )
                        .def( "removeSubMesh", &IMesh::removeSubMesh )
                        .def( "removeAllSubMeshes", &IMesh::removeAllSubMeshes )
                        .def( "getSubMeshes", &IMesh::getSubMeshes )
                        .def( "setName", &IMesh::setName )
                        .def( "getName", &IMesh::getName )
                        .def( "updateAABB", &IMesh::updateAABB )
                        .def( "getAABB", &IMesh::getAABB )
                        .def( "setAABB", &IMesh::setAABB )
                        .def( "clone", &IMesh::clone )
                        .def( "getAnimationInterface", &IMesh::getAnimationInterface )
                        .def( "setAnimationInterface", &IMesh::setAnimationInterface )
                        .def( "getHasSharedVertexData", &IMesh::getHasSharedVertexData )
                        .def( "setHasSharedVertexData", &IMesh::setHasSharedVertexData )
                        .def( "hasSkeleton", &IMesh::hasSkeleton )
                        .def( "getSkeletonName", &IMesh::getSkeletonName )
                        .def( "getNumLodLevels", &IMesh::getNumLodLevels )
                        .def( "isEdgeListBuilt", &IMesh::isEdgeListBuilt )
                        .def( "hasVertexAnimation", &IMesh::hasVertexAnimation )
                        .def( "compare", &IMesh::compare )];
    }

}  // end namespace fb
