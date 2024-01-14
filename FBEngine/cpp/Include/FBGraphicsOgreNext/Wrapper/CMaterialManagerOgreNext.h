#ifndef CMaterialManagerOgreNext_h__
#define CMaterialManagerOgreNext_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/MaterialManager.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/UtilityTypes.h>

namespace fb
{
    namespace render
    {

        /** Class used to manage materials. */
        class CMaterialManagerOgreNext : public MaterialManager
        {
        public:
            /** Constructor. */
            CMaterialManagerOgreNext();

            /** Destructor. */
            ~CMaterialManagerOgreNext() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IMaterialManager::cloneMaterial */
            SmartPtr<IMaterial> cloneMaterial( SmartPtr<IMaterial> material,
                                               const String &clonedMaterialName );

            /** @copydoc IMaterialManager::cloneMaterial */
            SmartPtr<IMaterial> cloneMaterial( const String &name,
                                               const String &clonedMaterialName ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &name ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid, const String &name ) override;

            /** Get the graphics system object.
            @return Will be null in this implementation.
            */
            void _getObject( void **ppObject ) const override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CMaterialManagerOgreNext_h__
