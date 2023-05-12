#ifndef IParticle_h__
#define IParticle_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class IParticle : public ISharedObject
        {
        public:
            ~IParticle() override = default;

            virtual void *getData() const = 0;
            virtual void setData( void *data ) = 0;
        };

    }  // namespace render
}  // namespace fb

#endif  // IParticle_h__
