#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ObjectTemplatesBind.h"
#include "FBLuabind/SmartPtrConverter.h"
#include <luabind/luabind.hpp>

namespace fb
{

    //class GenericFireArm2TemplateHelper
    //{
    //public:
    //    static lua_Integer getProjectileCollisionMask( GenericFireArm2TemplatePtr objectTemplate )
    //    {
    //        u32 mask = objectTemplate->getProjectileCollisionMask();
    //        lua_Integer maskValue = *reinterpret_cast<lua_Integer *>( &mask );
    //        return maskValue;
    //    }

    //    static void setProjectileCollisionMask( GenericFireArm2TemplatePtr objectTemplate,
    //                                            lua_Integer mask )
    //    {
    //        u32 maskValue = *reinterpret_cast<u32 *>( &mask );
    //        objectTemplate->setProjectileCollisionMask( maskValue );
    //    }

    //    static lua_Integer getProjectileCollisionType( GenericFireArm2TemplatePtr objectTemplate )
    //    {
    //        u32 mask = objectTemplate->getProjectileCollisionType();
    //        lua_Integer maskValue = *reinterpret_cast<lua_Integer *>( &mask );
    //        return maskValue;
    //    }

    //    static void setProjectileCollisionType( GenericFireArm2TemplatePtr objectTemplate,
    //                                            lua_Integer mask )
    //    {
    //        u32 maskValue = *reinterpret_cast<u32 *>( &mask );
    //        objectTemplate->setProjectileCollisionType( maskValue );
    //    }
    //};

    void bindObjectTemplates( lua_State *L )
    {
        using namespace luabind;

        //module( L )[class_<ITemplate, IScriptObject, TemplatePtr>( "ITemplate" )];

        //module( L )[class_<Box3Template, ITemplate, TemplatePtr>( "Box3Template" )];

        //module( L )[class_<TerrainTemplate, ITemplate, TemplatePtr>( "TerrainTemplate" )
        //                .def( "getHeightMap", &TerrainTemplate::getHeightMap )
        //                .def( "setHeightMap", &TerrainTemplate::setHeightMap )

        //                .def( "setHeightData", &TerrainTemplate::setHeightData )
        //                .def( "getHeightData", &TerrainTemplate::getHeightData )

        //                .def( "getWorldSize", &TerrainTemplate::getWorldSize )
        //                .def( "setWorldSize", &TerrainTemplate::setWorldSize )

        //                .def( "getTerrainSize", &TerrainTemplate::getTerrainSize )
        //                .def( "setTerrainSize", &TerrainTemplate::setTerrainSize )];

        //module( L )[class_<CarTemplate, ITemplate, CarTemplatePtr>( "CarTemplate" )
        //                .def( "getSceneFileName", &CarTemplate::getSceneFileName )
        //                .def( "setSceneFileName", &CarTemplate::setSceneFileName )];

        //module( L )[class_<VehicleTemplate, ITemplate, VehicleTemplatePtr>( "VehicleTemplate" )];

        //module( L )[class_<WaterTemplate, ITemplate, WaterTemplatePtr>( "WaterTemplate" )];

        //module( L )[class_<GenericFireArm2Template, ITemplate, GenericFireArm2TemplatePtr>(
        //                "GenericFireArm2Template" )
        //                .def( "getProjectileFactory", &GenericFireArm2Template::getProjectileFactory )
        //                .def( "setProjectileFactory", &GenericFireArm2Template::setProjectileFactory )

        //                .def( "getProjectilePoolSize", &GenericFireArm2Template::getProjectilePoolSize )
        //                .def( "setProjectilePoolSize", &GenericFireArm2Template::setProjectilePoolSize )

        //                .def( "getProjectileDamage", &GenericFireArm2Template::getProjectileDamage )
        //                .def( "setProjectileDamage", &GenericFireArm2Template::setProjectileDamage )

        //                .def( "getProjectileCollisionType",
        //                      GenericFireArm2TemplateHelper::getProjectileCollisionType )
        //                .def( "setProjectileCollisionType",
        //                      GenericFireArm2TemplateHelper::setProjectileCollisionType )

        //                .def( "getProjectileCollisionMask",
        //                      GenericFireArm2TemplateHelper::getProjectileCollisionMask )
        //                .def( "setProjectileCollisionMask",
        //                      GenericFireArm2TemplateHelper::setProjectileCollisionMask )];

        //module( L )[class_<ParticleProjectile2Template, ITemplate, ParticleProjectile2TemplatePtr>(
        //    "ParticleProjectile2Template" )];

        //module( L )[class_<ParticleSpecialEffect2Template, ITemplate, ParticleSpecialEffect2TemplatePtr>(
        //                "ParticleSpecialEffect2Template" )
        //                .def( "getTemplateName", &ParticleSpecialEffect2Template::getTemplateName )
        //                .def( "setTemplateName", &ParticleSpecialEffect2Template::setTemplateName )

        //                .def( "getPosition", &ParticleSpecialEffect2Template::getPosition )
        //                .def( "setPosition", &ParticleSpecialEffect2Template::setPosition )];
    }

}  // end namespace fb
