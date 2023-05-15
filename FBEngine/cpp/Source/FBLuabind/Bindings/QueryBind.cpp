#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/QueryBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "luabind/operator.hpp"

namespace fb
{

    //QueryClosestEntityPtr _createQueryClosestEntity()
    //{
    //    return nullptr;  // QueryClosestEntityPtr(new QueryClosestEntity, true);
    //}

    //QueryPtr _up_cast( QueryClosestEntityPtr query )
    //{
    //    return query;
    //}

    //lua_Integer IQuery_getGameWorldMask( IQuery *query )
    //{
    //    u32 mask = query->getGameWorldMask();
    //    lua_Integer iMask = *reinterpret_cast<lua_Integer *>( &mask );
    //    return iMask;
    //}

    //void IQuery_setGameWorldMask( IQuery *query, lua_Integer mask )
    //{
    //    u32 uiMask = *reinterpret_cast<u32 *>( &mask );
    //    query->setGameWorldMask( uiMask );
    //}

    void bindQuery( lua_State *L )
    {
        using namespace luabind;

        //module( L )[class_<IQuery, IScriptObject, QueryPtr>( "IQuery" )
        //                .def( "getQueryType", &IQuery::getQueryType )
        //                .def( "setQueryType", &IQuery::setQueryType )

        //                .def( "getGameWorldMask", IQuery_getGameWorldMask )
        //                .def( "setGameWorldMask", IQuery_setGameWorldMask )];

        //module( L )[class_<QueryClosestEntity, IQuery, QueryPtr>( "QueryClosestEntity" )
        //                .def( "getState", &QueryClosestEntity::getState )
        //                .def( "setState", &QueryClosestEntity::setState )
        //                .def( "getEntityType", &QueryClosestEntity::getEntityType )
        //                .def( "setEntityType", &QueryClosestEntity::setEntityType )
        //                .def( "getEntityId", &QueryClosestEntity::getEntityId )
        //                .def( "setEntityId", &QueryClosestEntity::setEntityId )
        //                .def( "getClosestEntity", &QueryClosestEntity::getClosestEntity )

        //                .def( "getPosition", &QueryClosestEntity::getPosition )
        //                .def( "setPosition", &QueryClosestEntity::setPosition )];

        //module( L )[def( "createQueryClosestEntity", _createQueryClosestEntity )];
    }

}  // end namespace fb
