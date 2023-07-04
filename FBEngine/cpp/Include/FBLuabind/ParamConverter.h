#ifndef ParamConverter_h__
#define ParamConverter_h__

#include <FBCore/Core/Parameter.h>
#include <FBCore/Interface/Script/IScriptObject.h>
#include "FBLuabind/ScriptObjectUtil.h"
#include <luabind/operator.hpp>
#include <typeinfo>

/*
#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <boost/ref.hpp>

#include <boost/mpl/apply_wrap.hpp>

*/

/*
#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8
*/
/*
namespace luabind { namespace detail
{
template<class T>
T convert_to_cpp(lua_State* L, const T& v)
{
typedef typename mpl::apply_wrap1<
unwrap_ref<boost::is_reference_wrapper<T>::value>
, T
>::type value_type;

typename mpl::apply_wrap2<default_policy,value_type,lua_to_cpp>::type converter;

converter.apply(L, unwrap_ref<boost::is_reference_wrapper<T>::value>::get(v));
}
}}*/

/*
namespace luabind {

    template <>
    struct default_converter<__int64>
        : native_converter_base<__int64>
    {
        static int compute_score(lua_State* L, int index)
        {
            return -1;//lua_isinteger(L, index) ? 0 : -1;
        }

        __int64 from(lua_State* L, int index)
        {
            return (__int64)lua_tointeger(L, index);
        }

        void to(lua_State* L, __int64 value)
        {
            lua_pushinteger(L, value);
        }
    };

} // namespace luabind
*/

// String converter
namespace luabind
{

    //template <>
    //struct default_converter<fb::String> : native_converter_base<fb::String>
    //{
    //	static int compute_score(lua_State* L, int index)
    //	{
    //		return lua_isstring(L, index) ? 0 : -1;
    //	}

    //	fb::String from(lua_State* L, int index)
    //	{
    //		return fb::String(lua_tostring(L, index));
    //	}

    //	void to(lua_State* L, const fb::String& value)
    //	{
    //		lua_pushstring(L, value.c_str());
    //	}
    //};

    //template <>
    //struct default_converter< const fb::String > : default_converter< fb::String >
    //{ };

    //template <>
    //struct default_converter< const fb::String & > : default_converter< fb::String >
    //{ };

}  // namespace luabind

/*
namespace luabind
{
    using namespace fb;

    template <>
    struct default_converter<IScriptObject* >
        : default_converter<IScriptObject>
    {
        typedef boost::mpl::false_ is_native;

        template <class U>
        int match(lua_State* L, U, int index)
        {
            return -1;
        }

        template <class U>
        IScriptObject* apply(lua_State* L, U, int index)
        {
            IScriptObject* raw_ptr = default_converter<IScriptObject*>::apply(L, LUABIND_DECORATE_TYPE(IScriptObject*), index);
            return (IScriptObject*)(raw_ptr);
        }

        void apply(lua_State* L, IScriptObject* const& p)
        {
            detail::value_converter().apply(L, p);
        }

        template <class U>
        void converter_postcall(lua_State*, U const&, int)
        {
        }
    };
}*/

namespace luabind
{

    template <>
    struct default_converter<fb::Parameter> : native_converter_base<fb::Parameter>
    {
        static int compute_score( lua_State *L, int index )
        {
            int type = lua_type( L, index );
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
                    luabind::detail::object_rep *obj = luabind::detail::get_instance( L, index );
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

                auto& str = param.str;
                luabind::detail::convert_to_lua( L, str );
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_PTR:
            {
                if( !param.data.pData )
                {
                    luabind::detail::convert_to_lua( L, NULL );
                    return;
                }

                fb::IScriptObject *scriptObj = static_cast<fb::IScriptObject *>( param.data.pData );
                fb::ScriptObjectUtil::toLua( L, scriptObj );
            }
            break;
            case Parameter::ParameterType::PARAM_TYPE_ARRAY:
            {
                auto &scriptArray = param.array;
                luabind::detail::convert_to_lua( L, scriptArray );
            }
            break;
            };
        }
    };

    template <>
    struct default_converter<fb::Parameter const &> : default_converter<fb::Parameter>
    {
    };

}  // namespace luabind

namespace luabind
{

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
