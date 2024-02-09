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

    auto Parameter::getVector2() const -> Vector2F
    {
        FB_ASSERT( array.size() >= 2 );
        return { array[0].data.fData, array[1].data.fData };
    }

    void Parameter::setVector2( const Vector2F &data )
    {
        array.resize( 2 );
        array[0].data.fData = data.x;
        array[1].data.fData = data.y;
    }

    auto Parameter::getVector3() const -> Vector3<real_Num>
    {
        FB_ASSERT( array.size() >= 3 );
        return { array[0].data.fData, array[1].data.fData, array[2].data.fData };
    }

    void Parameter::setVector3( const Vector3<real_Num> &data )
    {
        array.resize( 3 );
        array[0].data.fData = data.x;
        array[1].data.fData = data.y;
        array[2].data.fData = data.z;
    }

    auto Parameter::getQuaternion() const -> Quaternion<real_Num>
    {
        FB_ASSERT( array.size() >= 4 );
        return { array[0].data.fData, array[1].data.fData, array[2].data.fData, array[3].data.fData };
    }

    void Parameter::setQuaternion( const Quaternion<real_Num> &data )
    {
        array.resize( 4 );

        array[0].data.fData = data[0];
        array[1].data.fData = data[1];
        array[2].data.fData = data[2];
        array[3].data.fData = data[3];
    }

    auto Parameter::getObject() const -> SmartPtr<ISharedObject>
    {
        return object;
    }

    auto Parameter::getArray() const -> const Array<Parameter> &
    {
        return array;
    }

    auto Parameter::getStr() const -> String
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
        setCharPtr( const_cast<c8 *>( data ) );
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

    Parameter::~Parameter() = default;

    auto Parameter::operator==( const Parameter &other ) const -> bool
    {
        return type == other.type && data.iData == other.data.iData;
    }
}  // end namespace fb
