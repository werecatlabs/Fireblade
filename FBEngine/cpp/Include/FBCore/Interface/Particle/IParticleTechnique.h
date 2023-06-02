#ifndef IParticleTechnique_h__
#define IParticleTechnique_h__

#include <FBCore/Interface/Particle/IParticleNode.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for a particle technique.
         */
        class IParticleTechnique : public IParticleNode
        {
        public:
            /** */
            ~IParticleTechnique() override = default;

            /**
             * @brief Adds an emitter.
             */
            virtual void addEmitter( SmartPtr<IParticleEmitter> emitter ) = 0;

            /**
             * @brief Removes an emitter.
             */
            virtual void removeEmitter( SmartPtr<IParticleEmitter> emitter ) = 0;

            /**
             * @brief Returns an array of all the emitters in this technique.
             */
            virtual Array<SmartPtr<IParticleEmitter>> getParticleEmitters() const = 0;

            /**
             * @brief Adds an affector.
             */
            virtual void addAffector( SmartPtr<IParticleAffector> affector ) = 0;

            /**
             * @brief Removes an affector.
             */
            virtual void removeAffector( SmartPtr<IParticleAffector> affector ) = 0;

            /**
             * @brief Returns an array of all the affectors in this technique.
             */
            virtual Array<SmartPtr<IParticleAffector>> getParticleAffectors() const = 0;

            /**
             * @brief Adds a renderer.
             */
            virtual void addRenderer( SmartPtr<IParticleRenderer> renderer ) = 0;

            /**
             * @brief Removes a renderer.
             */
            virtual void removeRenderer( SmartPtr<IParticleRenderer> renderer ) = 0;

            /**
             * @brief Returns an array of all the renderers in this technique.
             */
            virtual Array<SmartPtr<IParticleRenderer>> getParticleRenderers() const = 0;

            /**
             * @brief Returns an emitter with the given hash.
             */
            virtual SmartPtr<IParticleEmitter> getEmitter( hash32 hash ) const = 0;

            /**
             * @brief Returns an affector with the given hash.
             */
            virtual SmartPtr<IParticleAffector> getAffector( hash32 hash ) const = 0;

            /**
             * @brief Returns a renderer with the given hash.
             */
            virtual SmartPtr<IParticleRenderer> getRenderer( hash32 hash ) const = 0;

            /**
             * @brief Returns an emitter with the given name.
             */
            virtual SmartPtr<IParticleEmitter> getEmitterByName( const String &name ) const = 0;

            /**
             * @brief Returns an affector with the given name.
             */
            virtual SmartPtr<IParticleAffector> getAffectorByName( const String &name ) const = 0;

            /**
             * @brief Returns a renderer with the given name.
             */
            virtual SmartPtr<IParticleRenderer> getRendererByName( const String &name ) const = 0;

            /**
             * @brief Returns an array of all the particles in this technique.
             */
            virtual Array<SmartPtr<IParticle>> getParticles() const = 0;

            /**
             * @brief Sets the particles to the given array.
             */
            virtual void setParticles( const Array<SmartPtr<IParticle>> &particles ) = 0;

            /**
             * @brief Adds a particle.
             */
            virtual void addParticle( SmartPtr<IParticle> particle ) = 0;

            /**
             * @brief Removes a particle.
             */
            virtual void removeParticle( SmartPtr<IParticle> particle ) = 0;

            /**
             * @brief Clears all particles from this technique.
             */
            virtual void clearParticles() = 0;
        };
    }  // namespace render
}  // end namespace fb

#endif  // IParticleTechnique_h__
