#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/VectorData.h"
#include "FBCore/Data/DefineDataStructures.h"

namespace fb
{
    VectorData::VectorData() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
    {
    }

    VectorData::VectorData( f32 fX, f32 fY, f32 fZ, f32 fW ) : x( fX ), y( fY ), z( fZ ), w( fW )
    {
        FB_ASSERT( x == x );
        FB_ASSERT( y == y );
        FB_ASSERT( z == z );
        FB_ASSERT( w == w );
    }

    VectorData::VectorData( const VectorData &other )
    {
        *this = other;
    }

    VectorData &VectorData::operator=( const VectorData &other )
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    Vector3<real_Num> VectorData::toVector()
    {
        return Vector3<real_Num>( x, y, z );
    }

    Quaternion<real_Num> VectorData::toRotation()
    {
        return Quaternion<real_Num>( w, x, y, z );
    }

    Vector3<real_Num> VectorData::toCoordsVector()
    {
        return Vector3<real_Num>( x, y, -z );
    }

    Quaternion<real_Num> VectorData::toCoordsRotation()
    {
        return Quaternion<real_Num>( w, -x, -y, z );
    }

    bool VectorData::isValid() const
    {
        if( MathF::isFinite( x ) && MathF::isFinite( y ) && MathF::isFinite( z ) &&
            MathF::isFinite( w ) )
        {
            return true;
        }

        return false;
    }

    VectorData MakeSaracenVec()
    {
        return VectorData();
    }

    VectorData MakeSaracenVec( f32 fX, f32 fY, f32 fZ, f32 fW )
    {
        return VectorData( fX, fY, fZ, fW );
    }

    VectorData getSaracenVec( const Vector3<real_Num> &vec )
    {
        VectorData saracenVec;
        saracenVec.x = vec.X();
        saracenVec.y = vec.Y();
        saracenVec.z = vec.Z();
        saracenVec.w = 0.0f;
        return saracenVec;
    }

    VectorData getSaracenQuat( const Quaternion<real_Num> &vec )
    {
        VectorData saracenVec;
        saracenVec.x = vec.X();
        saracenVec.y = vec.Y();
        saracenVec.z = vec.Z();
        saracenVec.w = vec.W();
        return saracenVec;
    }

    void getSaracenVec4( const Vector3<real_Num> &vec, data::vec4 &saracenVec )
    {
        saracenVec.x = vec.X();
        saracenVec.y = vec.Y();
        saracenVec.z = vec.Z();
        saracenVec.w = 0.0f;
    }

    void getSaracenQuatVec4( const Quaternion<real_Num> &vec, data::vec4 &saracenVec )
    {
        saracenVec.x = vec.X();
        saracenVec.y = vec.Y();
        saracenVec.z = vec.Z();
        saracenVec.w = vec.W();
    }
}  // namespace fb
