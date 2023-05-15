#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CParticleAffector.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        PUParticleAffector::PUParticleAffector( ParticleUniverse::ParticleAffector *affector ) :
            m_affector( affector )
        {
        }

        PUParticleAffector::PUParticleAffector()
        {
        }

        PUParticleAffector::~PUParticleAffector()
        {
        }

        void PUParticleAffector::calculateState( SmartPtr<IParticle> particle, u32 stateIndex,
                                                 void *data /*= nullptr*/ )
        {
        }

        SmartPtr<IParticleSystem> PUParticleAffector::getParticleSystem() const
        {
            return nullptr;
        }

        void PUParticleAffector::setParticleSystem( SmartPtr<IParticleSystem> val )
        {
        }

    }  // namespace render
}  // namespace fb
