#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IFSM, ISharedObject );

    const u32 IFSM::isStateChangeCompleteFlag = ( 1 << 0 );
    const u32 IFSM::autoChangeStateFlag = ( 1 << 1 );
    const u32 IFSM::isPendingFlag = ( 1 << 2 );
    const u32 IFSM::isReadyFlag = ( 1 << 3 );
    const u32 IFSM::isLockedFlag = ( 1 << 4 );
    const u32 IFSM::allowStateChangeFlag = ( 1 << 5 );

}  // end namespace fb
