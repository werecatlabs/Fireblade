#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{

    template <typename T>
    T Quaternion<T>::getYaw( bool reprojectAxis /*= true*/ ) const
    {
        if( reprojectAxis )
        {
            // yaw = atan2(localz.x, localz.z)
            // pick parts of zAxis() implementation that we need
            auto fTx = T( 2.0 ) * X();
            auto fTy = T( 2.0 ) * Y();
            auto fTz = T( 2.0 ) * Z();
            auto fTwy = fTy * W();
            auto fTxx = fTx * X();
            auto fTxz = fTz * X();
            auto fTyy = fTy * Y();

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
