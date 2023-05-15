#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/NetBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    void bindNet( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IPacket>( "IPacket" )
                    //.def("read", IPacket::read )
        ];
    }

}  // end namespace fb
