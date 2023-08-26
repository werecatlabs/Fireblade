#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessagePlay.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessagePlay, StateMessage );

    //---------------------------------------------
    StateMessagePlay::StateMessagePlay()
    {
    }

    //---------------------------------------------
    StateMessagePlay::~StateMessagePlay()
    {
    }
}  // end namespace fb
