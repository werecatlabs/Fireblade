#ifndef __DataInterfaceUtil_h__
#define __DataInterfaceUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Data/C/VectorData.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace plugin
    {

        class DataInterfaceUtil
        {
        public:
            template <class T>
            static VectorData toData( const Vector3<T> &v )
            {
                return MakeSaracenVec( v.x, v.y, v.z, T( 0.0 ) );
            }

            template <class T>
            static VectorData toData( const Quaternion<T> &v )
            {
                return MakeSaracenVec( v.x, v.y, v.z, v.w );
            }
        };

    }  // namespace plugin
}  // namespace fb

#endif  // __DataInterfaceUtil_h__
