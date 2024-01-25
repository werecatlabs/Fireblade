#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include <FBCore/Interface/Graphics/IParticleSystem.h>
#include <FBCore/Graphics/GraphicsObject.h>

namespace fb
{
    namespace render
    {

        class ParticleSystem : public GraphicsObject<IParticleSystem>
        {
        public:
            ParticleSystem();
            ~ParticleSystem() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void reload() override;

            void prepare() override;

            void start() override;

            void stop() override;

            void stopFade() override;

            void pause() override;

            void pause( f32 pauseTime ) override;

            void resume() override;

            void setFastForward( f32 time, f32 interval ) override;

            f32 getFastForwardTime() const override;

            f32 getFastForwardInterval() const override;

            void setTemplateName( const String &templateName ) override;

            String getTemplateName() const override;

            void setScale( const Vector3F &scale ) override;

            Vector3F getScale() const override;

            u32 getState() const override;

            SmartPtr<IParticleTechnique> getTechnique( const String &name ) const override;

            SmartPtr<IGraphicsScene> getSceneManager() const override;

            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) override;

            void addListener( SmartPtr<IParticleSystemListener> listener ) override;

            void removeListener( SmartPtr<IParticleSystemListener> listener ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // ParticleSystem_h__
