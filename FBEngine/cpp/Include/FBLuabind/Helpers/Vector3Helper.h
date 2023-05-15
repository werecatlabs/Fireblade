#ifndef Vector3Helper_h__
#define Vector3Helper_h__

#include <FBCore/Math/Vector3.h>

namespace fb
{

    template <class T>
    class Vector3Helper
    {
    public:
        static void setX( Vector3<T> &vec, lua_Number num )
        {
            vec.X() = (T)num;
        }

        static void setY( Vector3<T> &vec, lua_Number num )
        {
            vec.Y() = (T)num;
        }

        static void setZ( Vector3<T> &vec, lua_Number num )
        {
            vec.Z() = (T)num;
        }

        static T getX( const Vector3<T> &vec )
        {
            return vec.X();
        }

        static T getY( const Vector3<T> &vec )
        {
            return vec.Y();
        }

        static T getZ( const Vector3<T> &vec )
        {
            return vec.Z();
        }
    };

}  // end namespace fb

#endif  // Vector3Helper_h__
