#ifndef _IParticleSystem_H
#define _IParticleSystem_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for particle systems in a 3D scene.
         *
         * A particle system emits a group of particles that simulate various physical effects such as fire,
         * smoke, sparks, and dust. This interface defines methods to start, stop, pause, and resume the particle
         * system, as well as methods to set and get its properties and techniques. The particle system can be
         * scaled independently from the scene node to which it is attached.
         */
        class IParticleSystem : public IGraphicsObject
        {
        public:
            /**
             * @brief Virtual destructor.
             */
            ~IParticleSystem() override = default;

            /**
             * Interface for particle systems in a 3D scene.
             *
             * A particle system emits a group of particles that simulate various physical effects such as fire,
             * smoke, sparks, and dust. This interface defines methods to start, stop, pause, and resume the particle
             * system, as well as methods to set and get its properties and techniques. The particle system can be
             * scaled independently from the scene node to which it is attached.
             */
            virtual void reload() = 0;

            /**
             * Interface for particle systems in a 3D scene.
             *
             * A particle system emits a group of particles that simulate various physical effects such as fire,
             * smoke, sparks, and dust. This interface defines methods to start, stop, pause, and resume the particle
             * system, as well as methods to set and get its properties and techniques. The particle system can be
             * scaled independently from the scene node to which it is attached.
             */
            virtual void prepare() = 0;

            /**
             * Starts the particle system.
             *
             * This method starts the particle system. Only if a particle system has been attached to a scene node,
             * it can be started.
             */
            virtual void start() = 0;

            /**
             * Stops the particle system.
             *
             * This method stops the particle system. Only if a particle system has been attached to a scene node and
             * started, it can be stopped.
             */
            virtual void stop() = 0;

            /**
             * Stops emission of all particle and really stops the particle system when all particles are expired.
             *
             * This method stops the emission of all particles and stops the particle system when all particles are
             * expired.
             */
            virtual void stopFade() = 0;

            /**
             * Pauses the particle system.
             *
             * This method pauses the particle system.
             */
            virtual void pause() = 0;

            /**
             * Pauses the particle system for a period of time. After this time, the particle system automatically
             * resumes.
             *
             * This method pauses the particle system for a specified period of time. After this time, the particle
             * system automatically resumes.
             *
             * \param pauseTime The time in seconds to pause the particle system.
             */
            virtual void pause( f32 pauseTime ) = 0;

            /**
             * Resumes the particle system.
             *
             * This method resumes the particle system after it has been paused.
             */
            virtual void resume() = 0;

            /**
             * Sets the fast forward time and interval.
             *
             * This method sets the fast forward time and interval for the particle system.
             *
             * \param time The time to fast forward in seconds.
             * \param interval The interval between updates in seconds.
             */
            virtual void setFastForward( f32 time, f32 interval ) = 0;

            /**
             * Gets the fast forward time.
             *
             * This method returns the fast forward time for the particle system in seconds.
             */
            virtual f32 getFastForwardTime() const = 0;

            /**
             * Gets the fast forward interval.
             *
             * This method returns the interval between updates during fast forwarding in seconds.
             */
            virtual f32 getFastForwardInterval() const = 0;

            /**
             * @brief Sets the name of the template which acts as a blueprint for creation of this Particle System.
             *
             * @param templateName The name of the template.
             */
            virtual void setTemplateName( const String &templateName ) = 0;

            /**
             * @brief Returns the name of the template which acted as a blueprint for creation of this Particle System.
             *
             * @return The name of the template.
             */
            virtual String getTemplateName() const = 0;

            /**
             * @brief Set the scale independent from the node to which it is attached.
             *
             * @param scale The new scale.
             */
            virtual void setScale( const Vector3F &scale ) = 0;

            /**
             * @brief Returns the scale.
             *
             * @return The scale.
             */
            virtual Vector3F getScale() const = 0;

            /**
             * @brief Returns the state of the particle system.
             *
             * @return The state of the particle system.
             */
            virtual u32 getState() const = 0;

            /**
             * @brief Returns a smart pointer to a particle technique with the given name.
             *
             * @param name The name of the particle technique.
             * @return A smart pointer to the particle technique.
             */
            virtual SmartPtr<IParticleTechnique> getTechnique( const String &name ) const = 0;

            /**
             * @brief Returns the scene manager.
             *
             * @return The scene manager.
             */
            virtual SmartPtr<IGraphicsScene> getSceneManager() const = 0;

            /**
             * @brief Sets the scene manager.
             *
             * @param val The new scene manager.
             */
            virtual void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) = 0;

            /**
             * @brief Adds a listener.
             *
             * @param listener The listener to add.
             */
            virtual void addListener( SmartPtr<IParticleSystemListener> listener ) = 0;

            /**
             * @brief Removes a listener.
             *
             * @param listener The listener to remove.
             */
            virtual void removeListener( SmartPtr<IParticleSystemListener> listener ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace render
}  // end namespace fb

#endif
