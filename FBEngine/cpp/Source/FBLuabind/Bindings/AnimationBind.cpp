#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/AnimationBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include <FBCore/FBCore.h>


namespace fb
{

    void bindAnimation( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IAnimator, IScriptObject, SmartPtr<IAnimator>>( "Animator" )

                        .def( "setId", &IAnimator::setId )
                        .def( "getId", &IAnimator::getId )

                        .def( "start", &IAnimator::start )
                        .def( "stop", &IAnimator::stop )

                        .def( "setLoop", &IAnimator::setLoop )
                        .def( "isLoop", &IAnimator::isLoop )

                        .def( "setReverse", &IAnimator::setReverse )
                        .def( "isReverse", &IAnimator::isReverse )
                        .def( "isFinished", &IAnimator::isFinished )
                        .def( "setAnimationLength", &IAnimator::setAnimationLength )];

        module(
            L )[class_<InterpolateVector2f, IAnimator, InterpolateVector2fPtr>( "InterpolateVector2f" )
                    .def( "getStartValue", &InterpolateVector2f::getStartValue )
                    .def( "setStartValue", &InterpolateVector2f::setStartValue )

                    .def( "getEndValue", &InterpolateVector2f::getEndValue )
                    .def( "setEndValue", &InterpolateVector2f::setEndValue )

                    .def( "getValue", &InterpolateVector2f::getValue )];
    }

}  // end namespace fb
