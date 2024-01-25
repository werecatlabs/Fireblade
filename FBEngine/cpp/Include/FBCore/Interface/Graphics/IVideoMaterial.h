#ifndef IVideoMaterial_h__
#define IVideoMaterial_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class IVideoMaterial : public ISharedObject
        {
        public:
            ~IVideoMaterial() override = default;

            virtual void _getObject( void **object ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // end namespace fb

#endif  // IVideoMaterial_h__
