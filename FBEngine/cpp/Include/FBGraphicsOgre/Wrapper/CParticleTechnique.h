#ifndef CParticleTechnique_h__
#define CParticleTechnique_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Particle/IParticleTechnique.h>
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

        class PUParticleTechnique : public CParticleNode<IParticleTechnique>
        {
        public:
            PUParticleTechnique();

            PUParticleTechnique( ParticleUniverse::ParticleTechnique *puTech );

            ~PUParticleTechnique() override;

            ParticleUniverse::ParticleTechnique *getTechnique() const;

            void setTechnique( ParticleUniverse::ParticleTechnique *val );

            SmartPtr<IParticleEmitter> getEmitterByName( const String &name ) const override;

            SmartPtr<IParticleAffector> getAffectorByName( const String &name ) const override;

            SmartPtr<IParticleRenderer> getRendererByName( const String &name ) const override;

            s32 getObject( u32 hash, SmartPtr<IObject> &object ) const;

            //
            // IScriptObjects
            //

            /** Gets an object call script functions. */
            virtual SmartPtr<IScriptInvoker> &getInvoker();

            /** Gets an object call script functions. */
            virtual const SmartPtr<IScriptInvoker> &getInvoker() const;

            /** Sets an object call script functions. */
            virtual void setInvoker( SmartPtr<IScriptInvoker> invoker );

            /** Gets an object to receive script calls. */
            virtual SmartPtr<IScriptReceiver> &getReceiver();

            /** Gets an object to receive script calls. */
            virtual const SmartPtr<IScriptReceiver> &getReceiver() const;

            /** Sets an object to receive script calls. */
            virtual void setReceiver( SmartPtr<IScriptReceiver> receiver );

            void addEmitter( SmartPtr<IParticleEmitter> emitter ) override;

            void removeEmitter( SmartPtr<IParticleEmitter> emitter ) override;

            Array<SmartPtr<IParticleEmitter>> getParticleEmitters() const override;

            void addAffector( SmartPtr<IParticleAffector> affector ) override;

            void removeAffector( SmartPtr<IParticleAffector> affector ) override;

            Array<SmartPtr<IParticleAffector>> getParticleAffectors() const override;

            void addRenderer( SmartPtr<IParticleRenderer> renderer ) override;

            void removeRenderer( SmartPtr<IParticleRenderer> renderer ) override;

            Array<SmartPtr<IParticleRenderer>> getParticleRenderers() const override;

            SmartPtr<IParticleEmitter> getEmitter( hash32 hash ) const override;

            SmartPtr<IParticleAffector> getAffector( hash32 hash ) const override;

            SmartPtr<IParticleRenderer> getRenderer( hash32 hash ) const override;

            Array<SmartPtr<IParticle>> getParticles() const override;

            void setParticles( const Array<SmartPtr<IParticle>> &val ) override;

            void addParticle( SmartPtr<IParticle> val ) override;

            void removeParticle( SmartPtr<IParticle> val ) override;

            void clearParticles() override;

            virtual void calculateState( SmartPtr<IParticle> particle, u32 stateIndex,
                                         void *data = nullptr );

            SmartPtr<IParticleSystem> getParticleSystem() const override;

            void setParticleSystem( SmartPtr<IParticleSystem> val ) override;

        protected:
            SmartPtr<IScriptInvoker> m_scriptInvoker;
            SmartPtr<IScriptReceiver> m_scriptReceiver;

            ParticleUniverse::ParticleTechnique *m_particleTechnique;

            using ParticleEmitters = HashMap<u32, SmartPtr<IParticleEmitter>>;
            ParticleEmitters m_particleEmitters;
        };


    }  // namespace render
}  // namespace fb

#endif  // CParticleTechnique_h__
