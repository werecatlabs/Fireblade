#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Base/Data.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{

    template <typename T>
    String Quaternion<T>::toJson() const
    {
        auto pData = toData();
        auto v = pData->template getDataAsType<data::vec4>();
        return DataUtil::toString( v );
    }

    template <typename T>
    SmartPtr<IData> Quaternion<T>::toData() const
    {
        auto pData = fb::make_ptr<Data<data::vec4>>();
        auto v = pData->getDataAsType<data::vec4>();

        v->x = static_cast<f32>( X() );
        v->y = static_cast<f32>( Y() );
        v->z = static_cast<f32>( Z() );
        v->w = static_cast<f32>( W() );
        return pData;
    }

    template <class T>
    void Quaternion<T>::fromData( SmartPtr<IData> data )
    {
        auto v = data->getDataAsType<data::vec4>();

        X() = static_cast<T>( v->x );
        Y() = static_cast<T>( v->y );
        Z() = static_cast<T>( v->z );
        W() = static_cast<T>( v->w );
    }

    template <typename T>
    T Quaternion<T>::getYaw( bool reprojectAxis /*= true*/ ) const
    {
        if( reprojectAxis )
        {
            // yaw = atan2(localz.x, localz.z)
            // pick parts of zAxis() implementation that we need
            T fTx = T( 2.0 ) * X();
            T fTy = T( 2.0 ) * Y();
            T fTz = T( 2.0 ) * Z();
            T fTwy = fTy * W();
            T fTxx = fTx * X();
            T fTxz = fTz * X();
            T fTyy = fTy * Y();

            return Math<T>::ATan2( fTxz + fTwy, T( 1.0 ) - ( fTxx + fTyy ) );
        }

        // internal version
        return Math<T>::ASin( T( -2.0 ) * ( X() * Z() - W() * Y() ) );
    }

    // explicit instantiation
    template class Quaternion<s32>;
    template class Quaternion<f32>;
    template class Quaternion<f64>;
}  // namespace fb
