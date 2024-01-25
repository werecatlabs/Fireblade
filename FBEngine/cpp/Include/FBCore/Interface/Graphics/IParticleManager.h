#ifndef IParticleManager_h__
#define IParticleManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/IObject.h>

namespace fb
{
    namespace render
    {
        class IParticleManager : public ISharedObject
        {
        public:
            ~IParticleManager() override = default;

            virtual SmartPtr<IParticleSystem> addParticleSystem( hash32 id ) = 0;
        };

    }  // namespace render
}  // namespace fb

#endif  // IParticleManager_h__
