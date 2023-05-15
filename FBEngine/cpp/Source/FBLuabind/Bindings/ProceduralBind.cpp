#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ProceduralBind.h"
#include <FBCore/FBCore.h>

namespace fb
{

    void bindProcedural( lua_State *L )
    {
        using namespace luabind;
        using namespace procedural;

        module( L )[class_<IProceduralGenerator, ISharedObject, SmartPtr<IProceduralGenerator>>(
                        "IProceduralGenerator" )
                        .def( "getFilePath", &IProceduralGenerator::getFilePath )
                        .def( "setFilePath", &IProceduralGenerator::setFilePath )
                        .def( "generate", &IProceduralGenerator::generate )
                        .def( "getInput", &IProceduralGenerator::getInput )
                        .def( "setInput", &IProceduralGenerator::setInput )
                        .def( "getOutput", &IProceduralGenerator::getOutput )
                        .def( "setOutput", &IProceduralGenerator::setOutput )
                        .def( "getParent", &IProceduralGenerator::getParent )
                        .def( "setParent", &IProceduralGenerator::setParent )];

        module( L )[class_<ICityGenerator, IProceduralGenerator>( "ICityGenerator" )
                        .def( "generate", &ICityGenerator::generate )
                        .def( "isFinished", &ICityGenerator::isFinished )
                        .def( "getTerrainGenerator", &ICityGenerator::getTerrainGenerator )
                        .def( "setTerrainGenerator", &ICityGenerator::setTerrainGenerator )
                        .def( "getBlockGenerator", &ICityGenerator::getBlockGenerator )
                        .def( "setBlockGenerator", &ICityGenerator::setBlockGenerator )
                        .def( "getProceduralWorld", &ICityGenerator::getProceduralWorld )
                        .def( "setProceduralWorld", &ICityGenerator::setProceduralWorld )
                        .def( "getMeshGenerator", &ICityGenerator::getMeshGenerator )
                        .def( "setMeshGenerator", &ICityGenerator::setMeshGenerator )
                        .def( "getRoadGenerator", &ICityGenerator::getRoadGenerator )
                        .def( "setRoadGenerator", &ICityGenerator::setRoadGenerator )
                        .def( "getCities", &ICityGenerator::getCities )
                        .def( "removeCity", &ICityGenerator::removeCity )
                        .def( "addCity", &ICityGenerator::addCity )];

        module( L )[class_<IProceduralCollision, ISharedObject, no_bases>( "IProceduralCollision" )
                        .def( "rayTest", ( bool( IProceduralCollision::* )( const Vector3<real_Num> &,
                                                                            const Vector3<real_Num> &,
                                                                            Vector3<real_Num> & ) ) &
                                             IProceduralCollision::rayTest )
                        .def( "rayTest", ( bool( IProceduralCollision::* )(
                                             const Vector3<real_Num> &, const Vector3<real_Num> &,
                                             Vector3<real_Num> &, Triangle3<real_Num> & ) ) &
                                             IProceduralCollision::rayTest )
                        .def( "rayTest",
                              ( bool( IProceduralCollision::* )(
                                  const Vector3<real_Num> &, const Vector3<real_Num> &,
                                  Vector3<real_Num> &, Triangle3<real_Num> &, const Array<String> & ) ) &
                                  IProceduralCollision::rayTest )
                        .def( "intersects",
                              ( bool( IProceduralCollision::* )( const SmartPtr<IProceduralCity> &,
                                                                 const Sphere3<real_Num> & ) ) &
                                  IProceduralCollision::intersects )
                        .def( "intersects", ( bool( IProceduralCollision::* )(
                                                const SmartPtr<IRoad> &, const Sphere3<real_Num> & ) ) &
                                                IProceduralCollision::intersects )

        ];
    }

}  // end namespace fb
