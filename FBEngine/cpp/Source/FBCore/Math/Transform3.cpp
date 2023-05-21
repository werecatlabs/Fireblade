#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{

    template <class T>
    AABB3<T> Transform3<T>::transformAABB( const AABB3<T> &aabb )
    {
        FB_ASSERT( isSane() );

        // Do nothing if current null or infinite
        if( !aabb.isFinite() )
        {
            return AABB3<T>();
        }

        AABB3<T> box;

        Vector3<T> currentCorner = Vector3<T>::zero();

        auto oldMin = aabb.getMinimum();
        auto oldMax = aabb.getMaximum();

        // We sequentially compute the corners in the following order :
        // 0, 6, 5, 1, 2, 4 ,7 , 3
        // This sequence allows us to only change one member at a time to get at all corners.

        // For each one, we transform it using the matrix
        // Which gives the resulting point and merge the resulting point.

        // First corner
        // min min min
        currentCorner = oldMin;
        box.merge( transformPoint( currentCorner ) );

        // min,min,max
        currentCorner.Z() = oldMax.Z();
        box.merge( transformPoint( currentCorner ) );

        // min max max
        currentCorner.Y() = oldMax.Y();
        box.merge( transformPoint( currentCorner ) );

        // min max min
        currentCorner.Z() = oldMin.Z();
        box.merge( transformPoint( currentCorner ) );

        // max max min
        currentCorner.X() = oldMax.X();
        box.merge( transformPoint( currentCorner ) );

        // max max max
        currentCorner.Z() = oldMax.Z();
        box.merge( transformPoint( currentCorner ) );

        // max min max
        currentCorner.Y() = oldMin.Y();
        box.merge( transformPoint( currentCorner ) );

        // max min min
        currentCorner.Z() = oldMin.Z();
        box.merge( transformPoint( currentCorner ) );

        return box;
    }

    template <class T>
    SmartPtr<Properties> Transform3<T>::getProperties() const
    {
        auto properties = fb::make_ptr<Properties>();

        properties->setProperty( "Position", m_position );

        Vector3<T> rotation;
        m_orientation.toDegrees( rotation );

        properties->setProperty( "Rotation", rotation );
        properties->setProperty( "Scale", m_scale );

        return properties;
    }

    template <class T>
    void Transform3<T>::setProperties( SmartPtr<Properties> properties )
    {
        if( auto child = properties->getChild( "position" ) )
        {
            child->getPropertyValue( "x", m_position.x );
            child->getPropertyValue( "y", m_position.y );
            child->getPropertyValue( "z", m_position.z );
        }
        else
        {
            properties->getPropertyValue( "Position", m_position );
        }

        Vector3<T> rotation;

        if( auto child = properties->getChild( "rotation" ) )
        {
            child->getPropertyValue( "x", rotation.x );
            child->getPropertyValue( "y", rotation.y );
            child->getPropertyValue( "z", rotation.z );
        }
        else
        {
            properties->getPropertyValue( "Rotation", rotation );
        }

        m_orientation.fromDegrees( rotation );

        if( auto child = properties->getChild( "scale" ) )
        {
            child->getPropertyValue( "x", m_scale.x );
            child->getPropertyValue( "y", m_scale.y );
            child->getPropertyValue( "z", m_scale.z );
        }
        else
        {
            properties->getPropertyValue( "Scale", m_scale );
        }
    }

    template <class T>
    void Transform3<T>::fromWorldToLocal( Transform3<T> &parentTransform, Transform3<T> &worldTransform )
    {
        m_position = parentTransform.convertWorldToLocalPosition( worldTransform.getPosition() );
        m_orientation =
            parentTransform.convertWorldToLocalOrientation( worldTransform.getOrientation() );
        m_scale = worldTransform.getScale();
    }

    template class Transform3<s32>;
    template class Transform3<f32>;
    template class Transform3<f64>;
}  // end namespace fb
