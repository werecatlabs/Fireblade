#ifndef _VectorUtil_H
#define _VectorUtil_H

#include <FBCore/Base/BitUtil.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    /** Provides functions to help deal with vectors. */
    class FBCore_API VectorUtil
    {
    public:
        union CompressedVector2
        {
            u32 val;
            u16 vals[2];
        };

        /** Compresses a 2d vector into an unsigned int. */
        template <class T>
        static u32 compressVector2( const Vector2<T> &vector );

        /** Decompresses a vector stored in a unsigned int. */
        template <class T>
        static Vector2<T> decompressVector2( u32 value );

        template <class T>
        static Vector2<T> toVector2( const Vector2D &vec );

        template <class T>
        static Vector2<T> toVector2( const Vector2F &vec );

        template <class T>
        static Vector3<T> toVector3( const Vector3D &vec );

        template <class T>
        static Vector3<T> toVector3( const Vector3F &vec );
    };

    template <class T>
    u32 VectorUtil::compressVector2( const Vector2<T> &vector )
    {
        CompressedVector2 val;
        val.vals[0] = BitUtil::floatToHalf( vector.X() );
        val.vals[1] = BitUtil::floatToHalf( vector.Y() );
        return val.val;
    }

    template <class T>
    Vector2<T> VectorUtil::decompressVector2( u32 value )
    {
        Vector2<T> vector;
        return vector;
    }

    template <class T>
    Vector2<T> VectorUtil::toVector2( const Vector2D &vec )
    {
        return Vector2<T>( vec.X(), vec.Y() );
    }

    template <class T>
    Vector2<T> VectorUtil::toVector2( const Vector2F &vec )
    {
        return Vector2<T>( vec.X(), vec.Y() );
    }

    template <class T>
    Vector3<T> VectorUtil::toVector3( const Vector3D &vec )
    {
        return Vector3<T>( vec.X(), vec.Y(), vec.Z() );
    }

    template <class T>
    Vector3<T> VectorUtil::toVector3( const Vector3F &vec )
    {
        return Vector3<T>( vec.X(), vec.Y(), vec.Z() );
    }

}  // end namespace fb

#endif
