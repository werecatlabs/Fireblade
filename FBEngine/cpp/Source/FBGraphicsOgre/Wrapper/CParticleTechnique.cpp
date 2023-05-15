#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CParticleTechnique.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        PUParticleTechnique::PUParticleTechnique( ParticleUniverse::ParticleTechnique *puTech ) :
            m_particleTechnique( puTech )
        {
            // setInvoker(SmartPtr<IScriptInvoker>(new ScriptInvokerStandard, true));
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this), true));

            // u32 numEmitters = m_particleTechnique->getNumEmitters();
            // for(u32 i=0; i<numEmitters; ++i)
            //{
            //	ParticleUniverse::ParticleEmitter* puEmitter = m_particleTechnique->getEmitter(i);

            //	SmartPtr<IParticleEmitter> emitterPtr(new PUParticleEmitter(puEmitter), true);
            //	m_particleEmitters[StringUtil::getHash(puEmitter->getName().c_str())] = emitterPtr;
            //}
        }

        PUParticleTechnique::PUParticleTechnique()
        {
            // setInvoker(SmartPtr<IScriptInvoker>(new ScriptInvokerStandard, true));
            // setReceiver(SmartPtr<IScriptReceiver>(new ScriptReceiver(this), true));
        }

        PUParticleTechnique::~PUParticleTechnique()
        {
        }

        ParticleUniverse::ParticleTechnique *PUParticleTechnique::getTechnique() const
        {
            return m_particleTechnique;
        }

        void PUParticleTechnique::setTechnique( ParticleUniverse::ParticleTechnique *val )
        {
            m_particleTechnique = val;
        }

        fb::SmartPtr<fb::render::IParticleEmitter> PUParticleTechnique::getEmitterByName(
            const String &name ) const
        {
            // ParticleUniverse::ParticleEmitter* puEmitter =
            // m_particleTechnique->getEmitter(name.c_str()); SmartPtr<IParticleEmitter> emitter(new
            // PUParticleEmitter(puEmitter), true); return emitter;

            return nullptr;
        }

        fb::SmartPtr<fb::render::IParticleAffector> PUParticleTechnique::getAffectorByName(
            const String &name ) const
        {
            // ParticleUniverse::ParticleAffector* puAffector =
            // m_particleTechnique->getAffector(name.c_str()); SmartPtr<IParticleAffector>
            // affector(new PUParticleAffector(puAffector), true); return affector;

            return nullptr;
        }

        fb::SmartPtr<fb::render::IParticleRenderer> PUParticleTechnique::getRendererByName(
            const String &name ) const
        {
            return SmartPtr<IParticleRenderer>();
        }

        s32 PUParticleTechnique::getObject( u32 hash, SmartPtr<IObject> &object ) const
        {
            auto it = m_particleEmitters.find( hash );
            if( it != m_particleEmitters.end() )
            {
                // object = it->second;
                return 0;
            }

            return 0;
        }

        const fb::SmartPtr<fb::IScriptInvoker> &PUParticleTechnique::getInvoker() const
        {
            return m_scriptInvoker;
        }

        fb::SmartPtr<fb::IScriptInvoker> &PUParticleTechnique::getInvoker()
        {
            return m_scriptInvoker;
        }

        void PUParticleTechnique::setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_scriptInvoker = invoker;
        }

        const fb::SmartPtr<fb::IScriptReceiver> &PUParticleTechnique::getReceiver() const
        {
            return m_scriptReceiver;
        }

        fb::SmartPtr<fb::IScriptReceiver> &PUParticleTechnique::getReceiver()
        {
            return m_scriptReceiver;
        }

        void PUParticleTechnique::setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_scriptReceiver = receiver;
        }

        void PUParticleTechnique::addEmitter( SmartPtr<IParticleEmitter> emitter )
        {
        }

        void PUParticleTechnique::removeEmitter( SmartPtr<IParticleEmitter> emitter )
        {
        }

        Array<SmartPtr<IParticleEmitter>> PUParticleTechnique::getParticleEmitters() const
        {
            return Array<SmartPtr<IParticleEmitter>>();
        }

        void PUParticleTechnique::addAffector( SmartPtr<IParticleAffector> affector )
        {
        }

        void PUParticleTechnique::removeAffector( SmartPtr<IParticleAffector> affector )
        {
        }

        Array<SmartPtr<IParticleAffector>> PUParticleTechnique::getParticleAffectors() const
        {
            return Array<SmartPtr<IParticleAffector>>();
        }

        void PUParticleTechnique::addRenderer( SmartPtr<IParticleRenderer> renderer )
        {
        }

        void PUParticleTechnique::removeRenderer( SmartPtr<IParticleRenderer> renderer )
        {
        }

        Array<SmartPtr<IParticleRenderer>> PUParticleTechnique::getParticleRenderers() const
        {
            return Array<SmartPtr<IParticleRenderer>>();
        }

        SmartPtr<IParticleEmitter> PUParticleTechnique::getEmitter( hash32 hash ) const
        {
            return nullptr;
        }

        SmartPtr<IParticleAffector> PUParticleTechnique::getAffector( hash32 hash ) const
        {
            return nullptr;
        }

        SmartPtr<IParticleRenderer> PUParticleTechnique::getRenderer( hash32 hash ) const
        {
            return nullptr;
        }

        Array<SmartPtr<IParticle>> PUParticleTechnique::getParticles() const
        {
            return Array<SmartPtr<IParticle>>();
        }

        void PUParticleTechnique::setParticles( const Array<SmartPtr<IParticle>> &val )
        {
        }

        void PUParticleTechnique::addParticle( SmartPtr<IParticle> val )
        {
        }

        void PUParticleTechnique::removeParticle( SmartPtr<IParticle> val )
        {
        }

        void PUParticleTechnique::clearParticles()
        {
        }

        void PUParticleTechnique::calculateState( SmartPtr<IParticle> particle, u32 stateIndex,
                                                  void *data /*= nullptr*/ )
        {
        }

        SmartPtr<IParticleSystem> PUParticleTechnique::getParticleSystem() const
        {
            return nullptr;
        }

        void PUParticleTechnique::setParticleSystem( SmartPtr<IParticleSystem> val )
        {
        }

    }  // namespace render
}  // namespace fb
