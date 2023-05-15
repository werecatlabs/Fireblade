#ifndef CParticleAffector_h__
#define CParticleAffector_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Particle/IParticleAffector.h>
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

        class PUParticleAffector : public CParticleNode<IParticleAffector>
        {
        public:
            PUParticleAffector();

            PUParticleAffector( ParticleUniverse::ParticleAffector *affector );

            ~PUParticleAffector();

            void calculateState( SmartPtr<IParticle> particle, u32 stateIndex,
                                 void *data = nullptr ) override;

            SmartPtr<IParticleSystem> getParticleSystem() const override;

            void setParticleSystem( SmartPtr<IParticleSystem> val ) override;

        protected:
            ParticleUniverse::ParticleAffector *m_affector;
        };

    }  // namespace render
}  // namespace fb

#endif  // CParticleAffector_h__
