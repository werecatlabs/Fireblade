#ifndef ISystem_h__
#define ISystem_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace scene
    {
        /** System base class for the entity component system. */
        class ISystem : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ISystem() override = default;

            void update() override = 0;

            FB_CLASS_REGISTER_DECL;
        };
    } // namespace scene
}     // namespace fb

#endif  // ISystem_h__
