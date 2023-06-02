#ifndef _IPrefabManager_H
#define _IPrefabManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResourceManager.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace scene
    {

        /**
         * Interface for managing prefabs, which are pre-built Actor objects.
         * Extends the IResourceManager interface.
         */
        class IPrefabManager : public IResourceManager
        {
        public:
            /** Virtual destructor. */
            ~IPrefabManager() override = default;

            /**
             * Creates an instance of an Actor prefab.
             * @param prefab The prefab to create an instance of.
             * @return A smart pointer to the instantiated Actor object.
             */
            virtual SmartPtr<IActor> createInstance( SmartPtr<IActor> prefab ) = 0;

            /**
             * Loads an Actor object from data.
             * @param data The data to load the Actor from.
             * @param parent A smart pointer to the parent Actor of the loaded Actor.
             * @return A smart pointer to the loaded Actor object.
             */
            virtual SmartPtr<IActor> loadActor( SmartPtr<Properties> data, SmartPtr<IActor> parent ) = 0;

            /**
             * Loads a prefab from a file path.
             * @param filePath The file path of the prefab to load.
             * @return A smart pointer to the loaded IPrefab object.
             */
            virtual SmartPtr<IPrefab> loadPrefab( const String &filePath ) = 0;

            /**
             * Saves a prefab to a file.
             * @param filePath The file path to save the prefab to.
             * @param prefab A smart pointer to the prefab to save.
             */
            virtual void savePrefab( const String &filePath, SmartPtr<IActor> prefab ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif
