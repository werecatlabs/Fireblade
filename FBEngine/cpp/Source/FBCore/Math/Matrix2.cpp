#include <FBCore/FBCorePCH.h>
#include "FBCore/Math/Matrix2.h"

namespace fb
{
    template <class T>
    Matrix2<T>::Matrix2( T angle )
    {
        fromAngle( angle );
    }

    // explicit instantiation
    template class Matrix2<s32>;
    template class Matrix2<f32>;
    template class Matrix2<f64>;
}  // end namespace fb
