#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/Parameter.h>

namespace fb
{
    void Parameter::setObject( SmartPtr<ISharedObject> data )
    {
        type = ParameterType::PARAM_TYPE_OBJECT;
        object = data;
    }

    void Parameter::setArray( const Array<Parameter> &data )
    {
        type = ParameterType::PARAM_TYPE_ARRAY;
        array = data;
    }

    void Parameter::setStr( const String &data )
    {
        type = ParameterType::PARAM_TYPE_STR;
        str = data;
    }

    SmartPtr<ISharedObject> Parameter::getObject() const
    {
        return object;
    }

    const Array<Parameter> &Parameter::getArray() const
    {
        return array;
    }

    String Parameter::getStr() const
    {
        return str;
    }

    const Parameter Parameter::VOID_PARAM = Parameter();

    Parameter::Parameter( void *data )
    {
        setPtr( data );
    }

    Parameter::Parameter( f64 data )
    {
        setF64( data );
    }

    Parameter::Parameter( s64 data )
    {
        setS64( data );
    }

    Parameter::Parameter( f32 data )
    {
        setF32( data );
    }

    Parameter::Parameter( u32 data )
    {
        setU32( data );
    }

    Parameter::Parameter( s32 data )
    {
        setS32( data );
    }

    Parameter::Parameter( u16 data )
    {
        setU16( data );
    }

    Parameter::Parameter( u8 data )
    {
        setU8( data );
    }

    Parameter::Parameter( const c8 *data )
    {
        setCharPtr( (c8 *)data );
    }

    Parameter::Parameter( bool data )
    {
        setBool( data );
    }

    Parameter::Parameter() : type( ParameterType::PARAM_TYPE_NULL )
    {
        data.pData = nullptr;
    }

    Parameter::Parameter( const Array<Parameter> &data )
    {
        type = ParameterType::PARAM_TYPE_ARRAY;
        array = data;
    }

    Parameter::Parameter( SmartPtr<ISharedObject> data )
    {
        type = ParameterType::PARAM_TYPE_OBJECT;
        object = data;
    }

    Parameter::Parameter( const String &data )
    {
        type = ParameterType::PARAM_TYPE_STR;
        str = data;
    }

    Parameter::~Parameter()
    {
    }

    bool Parameter::operator==( const Parameter &other ) const
    {
        return type == other.type && data.iData == other.data.iData;
    }
}  // end namespace fb
