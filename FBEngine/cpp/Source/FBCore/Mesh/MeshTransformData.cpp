#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/MeshTransformData.h>
#include <FBCore/Interface/Mesh/IMesh.h>

namespace fb
{
    MeshTransformData::MeshTransformData( const Vector3F &position, const Vector3F &scale,
                                          const QuaternionF &orientation, const SmartPtr<IMesh> &mesh ) :
        Position( position ),
        Scale( scale ),
        Orientation( orientation ),
        Mesh( mesh )
    {
        UVOffsets.resize( 2 );
        UVScaleData.resize( 2 );
        UVScaleData[0] = Vector2F::UNIT;
        UVScaleData[1] = Vector2F::UNIT;
    }

    MeshTransformData::MeshTransformData()
    {
        UVOffsets.resize( 2 );
        UVScaleData.resize( 2 );
        UVScaleData[0] = Vector2F::UNIT;
        UVScaleData[1] = Vector2F::UNIT;
    }
} // namespace fb
