#ifndef _CPrefabManager_H
#define _CPrefabManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResourceGroupManager.h>
#include <FBCore/Interface/Scene/IPrefabManager.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace scene
    {
        /** Prefab manager implementation. */
        class PrefabManager : public SharedObject<IPrefabManager>
        {
        public:
            PrefabManager() = default;
            ~PrefabManager() override = default;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResourceManager::createInstance */
            SmartPtr<scene::IActor> createInstance( SmartPtr<scene::IActor> prefab ) override;

            /** @copydoc IResourceManager::loadActor */
            SmartPtr<scene::IActor> loadActor( SmartPtr<Properties> data,
                                               SmartPtr<scene::IActor> parent ) override;

            /** @copydoc IResourceManager::loadActor */
            //SmartPtr<scene::IActor> loadActor( data::actor_data &data, SmartPtr<scene::IActor> parent );

            /** @copydoc IResourceManager::loadPrefab */
            SmartPtr<IPrefab> loadPrefab( const String &filePath ) override;

            /** @copydoc IResourceManager::savePrefab */
            void savePrefab( const String &filePath, SmartPtr<scene::IActor> prefab ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid, const String &name ) override;

            /** @copydoc IResourceManager::saveToFile */
            void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

            /** @copydoc IResourceManager::loadFromFile */
            SmartPtr<IResource> loadFromFile( const String &filePath ) override;

            /** @copydoc IResourceManager::load */
            SmartPtr<IResource> load( const String &name ) override;

            /** @copydoc IResourceManager::getByName */
            SmartPtr<IResource> getByName( const String &name ) override;

            /** @copydoc IResourceManager::getById */
            SmartPtr<IResource> getById( const String &uuid ) override;

            /** @copydoc IResourceManager::_getObject */
            void _getObject( void **ppObject ) const override;

            /** @copydoc IResourceManager::createOrRetrieve */
            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                              const String &type ) override;

            /** @copydoc IResourceManager::createOrRetrieve */
            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path );

        private:
            Array<SmartPtr<IResource>> m_prefabs;
        };

    }  // namespace scene
}  // end namespace fb

#endif
