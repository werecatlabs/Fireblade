#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Particle/IParticleSystem.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IParticleSystem, ISharedObject );
    }  // end namespace render
}  // end namespace fb