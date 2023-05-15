#ifndef CParticleRenderer_h__
#define CParticleRenderer_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Particle/IParticleRenderer.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>
#include <FBGraphicsOgre/Wrapper/CParticleNode.h>
#include <FBCore/Base/HashMap.h>

#if FB_OGRE_USE_PARTICLE_UNIVERSE
#    include "ParticleUniverseSystemManager.h"
#endif

namespace fb
{
    namespace render
    {

        class PUParticleRenderer : public CParticleNode<IParticleRenderer>
        {
        public:
            PUParticleRenderer();

            PUParticleRenderer( ParticleUniverse::ParticleRenderer *renderer );

            ~PUParticleRenderer();

        protected:
            ParticleUniverse::ParticleRenderer *m_renderer;
        };

    }  // namespace render
}  // namespace fb

#endif  // CParticleRenderer_h__
