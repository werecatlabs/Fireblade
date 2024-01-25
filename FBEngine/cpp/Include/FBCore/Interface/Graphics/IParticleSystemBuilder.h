#ifndef IParticleSystemBuilder_h__
#define IParticleSystemBuilder_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class IParticleSystemBuilder : public ISharedObject
        {
        public:
            ~IParticleSystemBuilder() override = default;

            virtual void loadFromFile( const String &fileName ) = 0;
        };

    }  // namespace render
}  // namespace fb

#endif  // IParticleSystemBuilder_h__
