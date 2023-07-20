#ifndef IMaterialManager_h__
#define IMaterialManager_h__

#include <FBCore/Interface/Resource/IResourceManager.h>

namespace fb
{
    namespace render
    {

        /** Interface for a material manager. */
        class IMaterialManager : public IResourceManager
        {
        public:
            /** Virtual destructor. */
            ~IMaterialManager() override = default;

            /** Function used to clone a material.
            @param material The existing material.
            @param clonedMaterialName The name of the cloned material.
            */
            virtual SmartPtr<IMaterial> cloneMaterial( SmartPtr<IMaterial> material,
                                                       const String &clonedMaterialName ) = 0;

            /** Function used to clone a material.
            @param name The name of the existing material.
            @param clonedMaterialName The name of the cloned material.
            */
            virtual SmartPtr<IMaterial> cloneMaterial( const String &name,
                                                       const String &clonedMaterialName ) = 0;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialManager_h__
