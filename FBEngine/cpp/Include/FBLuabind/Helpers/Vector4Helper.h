#ifndef Vector4Helper_h__
#define Vector4Helper_h__

#include <FBCore/Math/Vector4.h>

namespace fb
{

    template <class T>
    class Vector4Helper
    {
    public:
        static void setX( Vector4<T> &vec, lua_Number num )
        {
            vec.X() = (T)num;
        }

        static void setY( Vector4<T> &vec, lua_Number num )
        {
            vec.Y() = (T)num;
        }

        static void setZ( Vector4<T> &vec, lua_Number num )
        {
            vec.Z() = (T)num;
        }

        static void setW( Vector4<T> &vec, lua_Number num )
        {
            vec.W() = (T)num;
        }

        static T getX( const Vector4<T> &vec )
        {
            return vec.X();
        }

        static T getY( const Vector4<T> &vec )
        {
            return vec.Y();
        }

        static T getZ( const Vector4<T> &vec )
        {
            return vec.Z();
        }

        static T getW( const Vector4<T> &vec )
        {
            return vec.W();
        }
    };

}  // end namespace fb

#endif  // Vector4Helper_h__
