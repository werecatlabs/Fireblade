#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/TransformData.h"

namespace fb
{
    TransformData::TransformData() :
        position( VectorData( 0.0f, 0.0f, 0.0f, 0.0f ) ),
        orientation( VectorData( 0.0f, 0.0f, 0.0f, 1.0f ) ),
        scale( VectorData( 1.0f, 1.0f, 1.0f, 1.0f ) )
    {
    }

    TransformData::TransformData( VectorData pos, VectorData ori, VectorData scale ) :
        position( pos ),
        orientation( ori ),
        scale( scale )
    {
    }

    TransformData &TransformData::operator=( const TransformData &other )
    {
        position = other.position;
        orientation = other.orientation;
        scale = other.scale;
        return *this;
    }

    bool TransformData::isFinite() const
    {
        return MathF::isFinite( position.x ) && MathF::isFinite( position.y ) &&
               MathF::isFinite( position.z ) && MathF::isFinite( scale.x ) &&
               MathF::isFinite( scale.y ) && MathF::isFinite( scale.z ) &&
               MathF::isFinite( orientation.x ) && MathF::isFinite( orientation.y ) &&
               MathF::isFinite( orientation.z ) && MathF::isFinite( orientation.w );
    }

    bool TransformData::isValid() const
    {
        if( position.isValid() && scale.isValid() && orientation.isValid() )
        {
            return true;
        }

        return false;
    }

    TransformData MakeSaracenTransform()
    {
        return TransformData();
    }
}  // namespace fb
