#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/MeshTransformData.h>
#include <FBCore/Interface/Mesh/IMesh.h>

namespace fb
{
    MeshTransformData::MeshTransformData( const Vector3<real_Num> &position,
                                          const Vector3<real_Num> &scale,
                                          const Quaternion<real_Num> &orientation,
                                          const SmartPtr<IMesh> &mesh ) :
        Position( position ),
        Scale( scale ),
        Orientation( orientation ),
        Mesh( mesh )
    {
        UVOffsets.resize( 2 );
        UVScaleData.resize( 2 );
        UVScaleData[0] = Vector2<real_Num>::UNIT;
        UVScaleData[1] = Vector2<real_Num>::UNIT;
    }

    MeshTransformData::MeshTransformData()
    {
        UVOffsets.resize( 2 );
        UVScaleData.resize( 2 );
        UVScaleData[0] = Vector2<real_Num>::UNIT;
        UVScaleData[1] = Vector2<real_Num>::UNIT;
    }
}  // namespace fb
