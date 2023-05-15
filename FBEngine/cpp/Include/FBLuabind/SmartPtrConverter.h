#ifndef SmartPtrConverter_h__
#define SmartPtrConverter_h__

#include <FBLuaBind/ScriptObjectUtil.h>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <typeinfo>

namespace luabind
{

    template <typename B>
    struct default_converter<fb::SmartPtr<B>> : native_converter_base<fb::SmartPtr<B>>
    {
        static int compute_score( lua_State *L, int index )
        {
            auto type = lua_type( L, index );
            return type == LUA_TUSERDATA ? 0 : -1;
        }

        fb::SmartPtr<B> from( lua_State *L, int index )
        {
            if( lua_isnil( L, index ) )
            {
                return nullptr;
            }

            auto obj = luabind::detail::get_instance( L, index );
            if( !obj )
            {
                return nullptr;
            }

            auto s = obj->get_instance( luabind::detail::registered_class<B>::id );
            if( s.first )
            {
                return static_cast<B *>( s.first );
            }

            auto p = obj->get_instance_by_type<B>();
            if (p)
            {            
                return p;
            }

            return nullptr;
        }

        void to( lua_State *L, fb::SmartPtr<B> const &ptr )
        {
            if( !ptr )
            {
                luabind::detail::convert_to_lua( L, NULL );
                return;
            }

            auto scriptObj = static_cast<fb::ISharedObject *>( ptr.get() );
            bool hasCast = fb::ScriptObjectUtil::toLua( L, scriptObj );
            if( !hasCast )
            {
                luabind::detail::convert_to_lua( L, static_cast<B *>( scriptObj ) );
            }
        }
    };

    template <typename B>
    struct default_converter<fb::SmartPtr<B> const &> : default_converter<fb::SmartPtr<B>>
    {
    };

}  // namespace luabind

#endif  // SmartPtrConverter_h__
