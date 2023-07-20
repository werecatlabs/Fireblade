#ifndef ILightmapper_h__
#define ILightmapper_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class ILightmapper : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ILightmapper() override = default;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // ILightmapper_h__
