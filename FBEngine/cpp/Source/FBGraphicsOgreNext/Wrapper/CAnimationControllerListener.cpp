#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Wrapper/CAnimationControllerListener.h"

namespace fb::render
{

    //--------------------------------------------
    CAnimationControllerListener::CAnimationControllerListener() = default;

    //--------------------------------------------
    CAnimationControllerListener::~CAnimationControllerListener() = default;

    //--------------------------------------------
    void CAnimationControllerListener::update( const s32 &task, const time_interval &t,
                                               const time_interval &dt )
    {
    }

    //--------------------------------------------
    void CAnimationControllerListener::OnAnimationEnd( const String &name )
    {
    }

}  // namespace fb::render
