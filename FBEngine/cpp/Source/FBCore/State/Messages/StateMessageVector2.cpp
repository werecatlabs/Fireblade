#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageVector2.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    //FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, StateMessageVector2, f32,  StateMessage);
    //FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, StateMessageVector2, f64, StateMessage );

    // explicit instantiation
    template class StateMessageVector2<s32>;
    template class StateMessageVector2<f32>;
    template class StateMessageVector2<f64>;
}  // end namespace fb
