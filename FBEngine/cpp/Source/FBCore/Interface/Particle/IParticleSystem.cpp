#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Particle/IParticleSystem.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IParticleSystem, ISharedObject );
}  // namespace fb::render
