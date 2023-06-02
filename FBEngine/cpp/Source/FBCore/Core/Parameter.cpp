#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Parameter.h>

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

    Vector2F Parameter::getVector2() const
    {
        FB_ASSERT( array.size() >= 2 );
        return Vector2F( array[0].data.fData, array[1].data.fData );
    }

    void Parameter::setVector2( const Vector2F &data )
    {
        array.resize( 2 );
        array[0].data.fData = data.x;
        array[1].data.fData = data.y;
    }

    Vector3F Parameter::getVector3() const
    {
        FB_ASSERT( array.size() >= 3 );
        return Vector3F( array[0].data.fData, array[1].data.fData, array[2].data.fData );
    }

    void Parameter::setVector3( const Vector3F &data )
    {
        array.resize( 3 );
        array[0].data.fData = data.x;
        array[1].data.fData = data.y;
        array[2].data.fData = data.z;
    }

    QuaternionF Parameter::getQuaternion() const
    {
        FB_ASSERT( array.size() >= 4 );
        return QuaternionF( array[0].data.fData, array[1].data.fData, array[2].data.fData,
                            array[3].data.fData );
    }

    void Parameter::setQuaternion( const QuaternionF &data )
    {
        array.resize( 4 );

        array[0].data.fData = data[0];
        array[1].data.fData = data[1];
        array[2].data.fData = data[2];
        array[3].data.fData = data[3];
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
