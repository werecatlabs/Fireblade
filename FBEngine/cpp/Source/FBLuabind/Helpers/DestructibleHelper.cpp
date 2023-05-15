#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuaBind/Helpers/DestructibleHelper.h"

namespace fb
{

    lua_Integer DestructibleHelper::getCollisionType( scene::Destructible *destructible )
    {
        hash32 hash = destructible->getCollisionType();
        return *reinterpret_cast<lua_Integer *>( &hash );
    }

    void DestructibleHelper::setCollisionType( scene::Destructible *destructible, lua_Integer val )
    {
        hash32 hash = *reinterpret_cast<hash32 *>( &val );
        destructible->setCollisionType( hash );
    }

    lua_Integer DestructibleHelper::getCollisionMask( scene::Destructible *destructible )
    {
        hash32 hash = destructible->getCollisionMask();
        return *reinterpret_cast<lua_Integer *>( &hash );
    }

    void DestructibleHelper::setCollisionMask( scene::Destructible *destructible, lua_Integer val )
    {
        hash32 hash = *reinterpret_cast<hash32 *>( &val );
        destructible->setCollisionMask( hash );
    }

}  // end namespace fb
