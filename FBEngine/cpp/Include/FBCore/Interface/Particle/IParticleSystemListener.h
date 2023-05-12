#ifndef IParticleSystemListener_h__
#define IParticleSystemListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for classes that want to listen for events from a particle system.
         */
        class IParticleSystemListener : public ISharedObject
        {
        public:
            ~IParticleSystemListener() override = default;

            /**
             * @brief Called when the particle system is started.
             */
            virtual void start() = 0;

            /**
             * @brief Called when the particle system is stopped.
             */
            virtual void stop() = 0;
        };

    }  // namespace render
}  // namespace fb

#endif  // IParticleSystemListener_h__
