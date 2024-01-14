#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/ParameterUtil.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{

    auto ParameterUtil::setVector2( const Vector2F &vec ) -> Parameters
    {
        Parameters params;
        params[0].data.fData = vec.X();
        params[1].data.fData = vec.Y();
        return params;
    }

    auto ParameterUtil::getVector2( const Parameters &params ) -> Vector2F
    {
        return { params[0].data.fData, params[1].data.fData };
    }

    auto ParameterUtil::getVector3( const Parameters &params ) -> Vector3F
    {
        return { params[0].data.fData, params[1].data.fData, params[2].data.fData };
    }

    auto ParameterUtil::getAABB2( const Parameters &params ) -> AABB2F
    {
        return { params[0].data.fData, params[1].data.fData, params[2].data.fData,
                 params[3].data.fData };
    }

    auto ParameterUtil::getParamTypeAsString( u32 paramType ) -> String
    {
        auto eParamType = static_cast<Parameter::ParameterType>( paramType );
        switch( eParamType )
        {
        case Parameter::ParameterType::PARAM_TYPE_NULL:
            return "null";
        case Parameter::ParameterType::PARAM_TYPE_BOOL:
            return "bool";
        case Parameter::ParameterType::PARAM_TYPE_U8:
            return "u8";
        case Parameter::ParameterType::PARAM_TYPE_U16:
            return "u16";
        case Parameter::ParameterType::PARAM_TYPE_U32:
            return "u32";
        case Parameter::ParameterType::PARAM_TYPE_S8:
            return "s8";
        case Parameter::ParameterType::PARAM_TYPE_S16:
            return "s16";
        case Parameter::ParameterType::PARAM_TYPE_S32:
            return "s32";
        case Parameter::ParameterType::PARAM_TYPE_F32:
            return "f32";
        case Parameter::ParameterType::PARAM_TYPE_S64:
            return "s64";
        case Parameter::ParameterType::PARAM_TYPE_F64:
            return "f64";
        case Parameter::ParameterType::PARAM_TYPE_CHAR_PTR:
            return "charptr";
        case Parameter::ParameterType::PARAM_TYPE_PTR:
            return "ptr";
        case Parameter::ParameterType::PARAM_TYPE_STR:
            return "str";
        case Parameter::ParameterType::PARAM_TYPE_VEC2I:
            return "vec2i";
        case Parameter::ParameterType::PARAM_TYPE_VEC2F:
            return "vec2f";
        case Parameter::ParameterType::PARAM_TYPE_VEC3I:
            return "vec3i";
        case Parameter::ParameterType::PARAM_TYPE_VEC3F:
            return "vec3f";
        default:
        {
        }
        }

        return {};
    }

    auto ParameterUtil::getParamTypeFromString( const String &paramTypeStr ) -> u32
    {
        if( paramTypeStr == String( "null" ) )
        {
            return static_cast<u32>( Parameter::ParameterType::PARAM_TYPE_NULL );
        }

        return static_cast<u32>( Parameter::ParameterType::PARAM_TYPE_NULL );
    }
}  // namespace fb
