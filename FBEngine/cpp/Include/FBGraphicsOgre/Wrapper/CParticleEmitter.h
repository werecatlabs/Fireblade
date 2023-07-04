#ifndef CParticleEmitter_h__
#define CParticleEmitter_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Particle/IParticleEmitter.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>
#include <FBGraphicsOgre/Wrapper/CParticleNode.h>
#include <FBCore/Core/HashMap.h>

#if FB_OGRE_USE_PARTICLE_UNIVERSE
#    include "ParticleUniverseSystemManager.h"
#endif

namespace fb
{
    namespace render
    {

        class PUParticleEmitter : public CParticleNode<IParticleEmitter>
        {
        public:
            PUParticleEmitter();

            PUParticleEmitter( ParticleUniverse::ParticleEmitter *emitter );

            ~PUParticleEmitter();

            ParticleUniverse::ParticleEmitter *getEmitter() const;

            void setEmitter( ParticleUniverse::ParticleEmitter *val );

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

            Vector3F getParticleSize() const override;

            void setParticleSize( const Vector3F &val ) override;

            Vector3F getParticleSizeMin() const override;

            void setParticleSizeMin( const Vector3F &val ) override;

            Vector3F getParticleSizeMax() const override;

            void setParticleSizeMax( const Vector3F &val ) override;

            Vector3F getDirection() const override;

            void setDirection( const Vector3F &val ) override;

            f32 getEmissionRate() const override;

            void setEmissionRate( f32 val ) override;

            f32 getEmissionRateMin() const override;

            void setEmissionRateMin( f32 val ) override;

            f32 getEmissionRateMax() const override;

            void setEmissionRateMax( f32 val ) override;

            f32 getTimeToLive() const override;

            void setTimeToLive( f32 val ) override;

            f32 getTimeToLiveMin() const override;

            void setTimeToLiveMin( f32 val ) override;

            f32 getTimeToLiveMax() const override;

            void setTimeToLiveMax( f32 val ) override;

            f32 getVelocity() const override;

            void setVelocity( f32 val ) override;

            f32 getVelocityMin() const override;

            void setVelocityMin( f32 val ) override;

            f32 getVelocityMax() const override;

            void setVelocityMax( f32 val ) override;

            virtual void calculateState( SmartPtr<IParticle> particle, u32 stateIndex,
                                         void *data = nullptr );

            SmartPtr<IParticleSystem> getParticleSystem() const override;
            void setParticleSystem( SmartPtr<IParticleSystem> val ) override;

        protected:
            SmartPtr<IScriptInvoker> m_scriptInvoker;
            SmartPtr<IScriptReceiver> m_scriptReceiver;
            ParticleUniverse::ParticleEmitter *m_emitter;
        };

    }  // namespace render
}  // namespace fb

#endif  // CParticleEmitter_h__
