#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/ParticleSystem.h>
#include <FBCore/Interface/Graphics/IParticleTechnique.h>
#include <FBCore/Interface/Graphics/IParticleSystem.h>
#include <FBCore/Interface/Graphics/IParticleSystemListener.h>

namespace fb
{
    namespace render
    {
        ParticleSystem::ParticleSystem()
        {
        }

        ParticleSystem::~ParticleSystem()
        {
        }

        void ParticleSystem::load( SmartPtr<ISharedObject> data )
        {
        }

        void ParticleSystem::unload( SmartPtr<ISharedObject> data )
        {
        }

        void ParticleSystem::reload()
        {
        }

        void ParticleSystem::prepare()
        {
        }

        void ParticleSystem::start()
        {
        }

        void ParticleSystem::stop()
        {
        }

        void ParticleSystem::stopFade()
        {
        }

        void ParticleSystem::pause()
        {
        }

        void ParticleSystem::resume()
        {
        }

        void ParticleSystem::setFastForward( f32 time, f32 interval )
        {
        }

        f32 ParticleSystem::getFastForwardTime() const
        {
            return 0.0f;
        }

        f32 ParticleSystem::getFastForwardInterval() const
        {
            return 0.0f;
        }

        void ParticleSystem::setTemplateName( const String &templateName )
        {
        }

        String ParticleSystem::getTemplateName() const
        {
            return String();
        }

        void ParticleSystem::setScale( const Vector3F &scale )
        {
        }

        Vector3F ParticleSystem::getScale() const
        {
            return Vector3F();
        }

        u32 ParticleSystem::getState() const
        {
            return 0;
        }

        SmartPtr<IParticleTechnique> ParticleSystem::getTechnique( const String &name ) const
        {
            return nullptr;
        }

        SmartPtr<IGraphicsScene> ParticleSystem::getSceneManager() const
        {
            return nullptr;
        }

        void ParticleSystem::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
        {
        }

        void ParticleSystem::addListener( SmartPtr<IParticleSystemListener> listener )
        {
        }

        void ParticleSystem::removeListener( SmartPtr<IParticleSystemListener> listener )
        {
        }

        void ParticleSystem::pause( f32 pauseTime )
        {
        }
    }  // namespace render
}  // namespace fb
