#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/Data.h>

namespace fb
{
    template <>
    const Vector4<s32> Vector4<s32>::ZERO( 0, 0, 0, 0 );
    template <>
    const Vector4<s32> Vector4<s32>::UNIT_X( 1, 0, 0, 0 );
    template <>
    const Vector4<s32> Vector4<s32>::UNIT_Y( 0, 1, 0, 0 );
    template <>
    const Vector4<s32> Vector4<s32>::UNIT_Z( 0, 0, 1, 0 );
    template <>
    const Vector4<s32> Vector4<s32>::UNIT( 1, 1, 1, 1 );

    template <>
    const Vector4<f32> Vector4<f32>::ZERO( 0.0f, 0.0f, 0.0f, 0.0f );
    template <>
    const Vector4<f32> Vector4<f32>::UNIT_X( 1.0f, 0.0f, 0.0f, 0.0f );
    template <>
    const Vector4<f32> Vector4<f32>::UNIT_Y( 0.0f, 1.0f, 0.0f, 0.0f );
    template <>
    const Vector4<f32> Vector4<f32>::UNIT_Z( 0.0f, 0.0f, 1.0f, 0.0f );
    template <>
    const Vector4<f32> Vector4<f32>::UNIT( 1.0f, 1.0f, 1.0f, 1.0f );

    template <>
    const Vector4<f64> Vector4<f64>::ZERO( 0.0, 0.0, 0.0, 0.0 );
    template <>
    const Vector4<f64> Vector4<f64>::UNIT_X( 1.0, 0.0, 0.0, 0.0 );
    template <>
    const Vector4<f64> Vector4<f64>::UNIT_Y( 0.0, 1.0, 0.0, 0.0 );
    template <>
    const Vector4<f64> Vector4<f64>::UNIT_Z( 0.0, 0.0, 1.0, 0.0 );
    template <>
    const Vector4<f64> Vector4<f64>::UNIT( 1.0, 1.0, 1.0, 1.0 );

    // explicit instantiation
    template class Vector4<f32>;
    template class Vector4<f64>;
}  // end namespace fb
