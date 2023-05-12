#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/Data.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{

    template <class T>
    String Transform3<T>::toJson() const
    {
        FB_ASSERT( isSane() );

        auto data = toData();
        auto v = data->template getDataAsType<data::transform_data>();
        return DataUtil::toString( v );
    }

    template <class T>
    SmartPtr<IData> Transform3<T>::toData() const
    {
        FB_ASSERT( isSane() );

        auto data = fb::make_ptr<Data<data::transform_data>>();
        auto t = data->getDataAsType<data::transform_data>();

        auto pPos = m_position.toData();
        auto pRot = m_orientation.toData();
        auto pScale = m_scale.toData();

        t->position = *pPos->template getDataAsType<data::vec4>();
        t->orientation = *pRot->template getDataAsType<data::vec4>();
        t->scale = *pScale->template getDataAsType<data::vec4>();

        return data;
    }

    template <class T>
    void Transform3<T>::fromData( SmartPtr<IData> data )
    {
        FB_ASSERT( isSane() );

        auto t = data->getDataAsType<data::transform_data>();

        auto p = t->position;
        auto q = t->orientation;
        auto s = t->scale;

        m_position = Vector3<T>( static_cast<T>( p.x ), static_cast<T>( p.y ), static_cast<T>( p.z ) );
        m_orientation = Quaternion<T>( static_cast<T>( q.w ), static_cast<T>( q.x ),
                                       static_cast<T>( q.y ), static_cast<T>( q.z ) );
        m_scale = Vector3<T>( static_cast<T>( s.x ), static_cast<T>( s.y ), static_cast<T>( s.z ) );
    }

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
        properties->getPropertyValue( "Position", m_position );

        Vector3<T> rotation;
        properties->getPropertyValue( "Rotation", rotation );

        m_orientation.fromDegrees( rotation );

        properties->getPropertyValue( "Scale", m_scale );
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
