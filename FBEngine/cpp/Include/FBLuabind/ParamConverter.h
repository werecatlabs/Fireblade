#ifndef ParamConverter_h__
#define ParamConverter_h__

#include <FBCore/Core/Parameter.h>
#include <FBCore/Interface/Script/IScriptObject.h>
#include "FBLuabind/ScriptObjectUtil.h"
#include <luabind/operator.hpp>
#include <typeinfo>

namespace luabind
{

    template <>
    struct default_converter<fb::Parameter> : native_converter_base<fb::Parameter>
    {
        static int compute_score( lua_State *L, int index )
        {
            auto type = lua_type( L, index );
            switch( type )
            {
            case LUA_TSTRING:
            //case LUA_TINT:
            case LUA_TNIL:
            case LUA_TBOOLEAN:
            case LUA_TNUMBER:
            {
                return 0;
            }
            break;
            case LUA_TUSERDATA:
            {
                if( lua_isnil( L, index ) )
                {
                    return 0;
                }
                else
                {
                    auto obj = luabind::detail::get_instance( L, index );
                    if( obj == 0 )
                        return -1;

                    if( obj->is_const() )
                        return -1;

                    int score = -1;

                    std::pair<void *, int> scriptObjInstance =
                        obj->get_instance( luabind::detail::registered_class<fb::IScriptObject>::id );
                    if( scriptObjInstance.second >= 0 )
                        score = 0;

                    return score;
                }
            }
            };

            return -1;
        }

        static bool isInteger( lua_Number number )
        {
            return floor( number ) == number;
        }

        fb::Parameter from( lua_State *L, int index )
        {
            using namespace fb;

            Parameter param;

            switch( lua_type( L, index ) )
            {
            case LUA_TBOOLEAN:
                param.data.bData = lua_toboolean( L, index );
                param.type = Parameter::ParameterType::PARAM_TYPE_BOOL;
                break;
            case LUA_TNUMBER:
            {
                if( isInteger( luaL_checknumber( L, index ) ) )
                {
                    param.data.iData = (s32)lua_tointeger( L, index );
                    param.type = Parameter::ParameterType::PARAM_TYPE_S64;
                    return param;
                }

                param.data.fData = (f32)lua_tonumber( L, index );
                param.type = Parameter::ParameterType::PARAM_TYPE_F64;
            }
            break;
            case LUA_TSTRING:
                param.data.pData = (void *)lua_tostring( L, index );
                param.type = Parameter::ParameterType::PARAM_TYPE_STR;
                break;
            case LUA_TTABLE:
            {
                if( lua_isnil( L, index ) )
                {
                    param.data.pData = NULL;
                    param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                }
                else
                {
                    luabind::detail::object_rep *obj = luabind::detail::get_instance( L, index );
                    if( obj == 0 )
                    {
                        param.data.pData = NULL;
                        param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                        return param;
                    }

                    if( obj->is_const() )
                    {
                        param.data.pData = NULL;
                        param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                        return param;
                    }

                    std::pair<void *, int> s =
                        obj->get_instance( luabind::detail::registered_class<IScriptObject>::id );
                    param.data.pData = s.first;
                    param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                }
            }
            break;
            case LUA_TUSERDATA:
            {
                if( lua_isnil( L, index ) )
                {
                    param.data.pData = NULL;
                    param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                }
                else
                {
                    luabind::detail::object_rep *obj = luabind::detail::get_instance( L, index );
                    if( obj == 0 )
                    {
                        param.data.pData = NULL;
                        param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                        return param;
                    }

                    if( obj->is_const() )
                    {
                        param.data.pData = NULL;
                        param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                        return param;
                    }

                    auto classId = luabind::detail::registered_class<ISharedObject>::id;
                    auto s = obj->get_instance( classId );

                    if( !s.first )
                    {
                        classId = luabind::detail::registered_class<Parameters>::id;
                        s = obj->get_instance( classId );
                    }

                    if( s.first )
                    {
                        param.data.pData = s.first;
                        param.type = Parameter::ParameterType::PARAM_TYPE_PTR;
                    }
                }
            }
            break;
            };

            return param;
        }

        void to( lua_State *L, fb::Parameter const &param )
        {
            using namespace fb;

            switch( param.type )
            {
            case Parameter::ParameterType::PARAM_TYPE_BOOL:
            {
                lua_pushboolean( L, param.data.bData );
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_U8:
            case Parameter::ParameterType::PARAM_TYPE_U16:
            case Parameter::ParameterType::PARAM_TYPE_U32:
            case Parameter::ParameterType::PARAM_TYPE_S8:
            case Parameter::ParameterType::PARAM_TYPE_S16:
            case Parameter::ParameterType::PARAM_TYPE_S32:
            case Parameter::ParameterType::PARAM_TYPE_S64:
            {
                lua_pushinteger( L, (lua_Integer)param.data.iData );
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_F32:
            case Parameter::ParameterType::PARAM_TYPE_F64:
            {
                lua_pushnumber( L, (lua_Number)param.data.fData );
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_STR:
            {
                //lua_pushstring( L, static_cast<char *>( param.data.pData ) );

                auto &str = param.str;
                luabind::detail::convert_to_lua( L, str );
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_PTR:
            {
                if( !param.data.pData )
                {
                    lua_pushnil(L);
                    return;
                }

                auto scriptObj = static_cast<fb::ISharedObject *>( param.data.pData );
                bool hasCast = fb::ScriptObjectUtil::toLua( L, scriptObj );
                if( !hasCast )
                {
                    detail::convert_to_lua( L, static_cast<ISharedObject *>( param.data.pData ) );
                }
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_OBJECT:
            {
                if( !param.object )
                {
                    lua_pushnil(L);
                    return;
                }

                bool hasCast = ScriptObjectUtil::toLua( L, param.object.get() );
                if( !hasCast )
                {
                    detail::convert_to_lua( L, static_cast<ISharedObject *>( param.object.get() ) );
                }
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_ARRAY:
            {
                auto &scriptArray = param.array;
                luabind::detail::convert_to_lua( L, scriptArray );
            }
            break;
            default:
            {
                FB_ASSERT( false );  // unhandled case
            }
            break;
            };
        }
    };

    template <>
    struct default_converter<fb::Parameter const &> : default_converter<fb::Parameter>
    {
    };

    template <class T>
    struct default_converter<fb::WeakPtr<T> > : default_converter<T *>
    {
        typedef boost::mpl::false_ is_native;

        template <class U>
        int match( lua_State *L, U, int index )
        {
            return default_converter<T *>::match( L, LUABIND_DECORATE_TYPE( T * ), index );
        }

        template <class U>
        fb::WeakPtr<T> apply( lua_State *L, U, int index )
        {
            T *raw_ptr = default_converter<T *>::apply( L, LUABIND_DECORATE_TYPE( T * ), index );
            if( !raw_ptr )
            {
                return nullptr;
            }

            return fb::WeakPtr<T>( raw_ptr );
        }

        void apply( lua_State *L, fb::WeakPtr<T> const &p )
        {
            detail::value_converter().apply( L, p );
        }

        template <class U>
        void converter_postcall( lua_State *, U const &, int )
        {
        }
    };

    template <class T>
    struct default_converter<fb::WeakPtr<T> const &> : default_converter<fb::WeakPtr<T> >
    {
    };

}  // namespace luabind

#endif  // ParamConverter_h__
