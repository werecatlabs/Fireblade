#ifndef __PrefabManager_H
#define __PrefabManager_H

#include <FBCore/Interface/Scene/IPrefabManager.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace scene
    {
        /** Prefab manager implementation. */
        class PrefabManager : public IPrefabManager
        {
        public:
            PrefabManager();
            ~PrefabManager() override;

            /** @copydoc IPrefabManager::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IPrefabManager::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IPrefabManager::createInstance */
            SmartPtr<IActor> createInstance( SmartPtr<IActor> prefab ) override;

            /** @copydoc IPrefabManager::loadActor */
            SmartPtr<IActor> loadActor( SmartPtr<Properties> data,
                                        SmartPtr<IActor> parent ) override;

            /** @copydoc IPrefabManager::loadActor */
            //SmartPtr<scene::IActor> loadActor( data::actor_data &data, SmartPtr<scene::IActor> parent );

            /** @copydoc IPrefabManager::loadPrefab */
            SmartPtr<IPrefab> loadPrefab( const String &filePath ) override;

            /** @copydoc IPrefabManager::savePrefab */
            void savePrefab( const String &filePath, SmartPtr<IActor> prefab ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid ) override;

            /** @copydoc IResourceManager::create */
            SmartPtr<IResource> create( const String &uuid, const String &name ) override;

            /** @copydoc IResourceManager::saveToFile */
            void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

            /** @copydoc IResourceManager::loadFromFile */
            SmartPtr<IResource> loadFromFile( const String &filePath ) override;

            /** @copydoc IResourceManager::load */
            SmartPtr<IResource> loadResource( const String &name ) override;

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
            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path ) override;

            FB_CLASS_REGISTER_DECL;

        private:
            Array<SmartPtr<IResource>> m_prefabs;
        };
    } // namespace scene
}     // end namespace fb

#endif
