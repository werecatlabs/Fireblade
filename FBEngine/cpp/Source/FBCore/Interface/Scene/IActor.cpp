#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, IActor, ISharedObject );

    const u32 IActor::ActorFlagReserved = ( 1 << 0 );
    const u32 IActor::ActorFlagStatic = ( 1 << 1 );
    const u32 IActor::ActorFlagVisible = ( 1 << 2 );
    const u32 IActor::ActorFlagEnabled = ( 1 << 3 );
    const u32 IActor::ActorFlagMine = ( 1 << 4 );
    const u32 IActor::ActorFlagPerpetual = ( 1 << 5 );
    const u32 IActor::ActorFlagDirty = ( 1 << 6 );
    const u32 IActor::ActorFlagAwake = ( 1 << 7 );
    const u32 IActor::ActorFlagStarted = ( 1 << 8 );
    const u32 IActor::ActorFlagDummy = ( 1 << 9 );
    const u32 IActor::ActorFlagInScene = ( 1 << 10 );
    const u32 IActor::ActorFlagEnabledInScene = ( 1 << 11 );
    const u32 IActor::ActorFlagIsEditor = ( 1 << 12 );

}  // namespace fb::scene
