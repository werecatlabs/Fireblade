#ifndef IParticleEmitter_h__
#define IParticleEmitter_h__

#include <FBCore/Interface/Graphics/IParticleNode.h>

namespace fb
{
    namespace render
    {
        class IParticleEmitter : public IParticleNode
        {
        public:
            ~IParticleEmitter() override = default;

            virtual Vector3F getParticleSize() const = 0;
            virtual void setParticleSize( const Vector3F &val ) = 0;

            virtual Vector3F getParticleSizeMin() const = 0;
            virtual void setParticleSizeMin( const Vector3F &val ) = 0;

            virtual Vector3F getParticleSizeMax() const = 0;
            virtual void setParticleSizeMax( const Vector3F &val ) = 0;

            virtual Vector3F getDirection() const = 0;
            virtual void setDirection( const Vector3F &val ) = 0;

            virtual f32 getEmissionRate() const = 0;
            virtual void setEmissionRate( f32 val ) = 0;

            virtual f32 getEmissionRateMin() const = 0;
            virtual void setEmissionRateMin( f32 val ) = 0;

            virtual f32 getEmissionRateMax() const = 0;
            virtual void setEmissionRateMax( f32 val ) = 0;

            virtual f32 getTimeToLive() const = 0;
            virtual void setTimeToLive( f32 val ) = 0;

            virtual f32 getTimeToLiveMin() const = 0;
            virtual void setTimeToLiveMin( f32 val ) = 0;

            virtual f32 getTimeToLiveMax() const = 0;
            virtual void setTimeToLiveMax( f32 val ) = 0;

            virtual f32 getVelocity() const = 0;
            virtual void setVelocity( f32 val ) = 0;

            virtual f32 getVelocityMin() const = 0;
            virtual void setVelocityMin( f32 val ) = 0;

            virtual f32 getVelocityMax() const = 0;
            virtual void setVelocityMax( f32 val ) = 0;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IParticleEmitter_h__
