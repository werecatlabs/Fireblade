#ifndef CMaterialManager_h__
#define CMaterialManager_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterialManager.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/UtilityTypes.h>

namespace fb
{
    namespace render
    {

        class CMaterialManager : public SharedObject<IMaterialManager>
        {
        public:
            CMaterialManager();
            ~CMaterialManager() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IMaterialManager::cloneMaterial */
            SmartPtr<IMaterial> cloneMaterial( SmartPtr<IMaterial> material,
                                               const String &clonedMaterialName ) override;

            /** @copydoc IMaterialManager::cloneMaterial */
            SmartPtr<IMaterial> cloneMaterial( const String &name,
                                               const String &clonedMaterialName ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &name ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid, const String &name ) override;

            /** @copydoc IResourceManager::createOrRetrieve */
            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                              const String &type ) override;

            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path );

            void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

            /** @copydoc IResourceManager::loadFromFile */
            SmartPtr<IResource> loadFromFile( const String &filePath ) override;

            /** @copydoc IResourceManager::load */
            SmartPtr<IResource> load( const String &name ) override;

            /** @copydoc IResourceManager::getByName */
            SmartPtr<IResource> getByName( const String &name ) override;

            /** @copydoc IResourceManager::getById */
            SmartPtr<IResource> getById( const String &uuid ) override;

            /** Get the graphics system object.
            @return Will be null in this implementation.
            */
            void _getObject( void **ppObject ) const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<SmartPtr<IMaterial>> m_materials;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialManager_h__
