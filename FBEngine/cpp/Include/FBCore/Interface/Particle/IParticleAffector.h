#ifndef IParticleAffector_h__
#define IParticleAffector_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Particle/IParticleNode.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Parameter.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for particle affectors.
         */
        class IParticleAffector : public IParticleNode
        {
        public:
            /** Destructor. */
            ~IParticleAffector() override = default;

            /**
             * @brief Calculates the state of a particle using the given index.
             *
             * @param particle The particle whose state to calculate.
             * @param stateIndex The index of the state to calculate.
             * @param data Optional data to use in the calculation.
             */
            virtual void calculateState( SmartPtr<IParticle> particle, u32 stateIndex,
                                         void *data = nullptr ) = 0;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IParticleAffector_h__
