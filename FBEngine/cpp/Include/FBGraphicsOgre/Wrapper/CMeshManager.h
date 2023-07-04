#ifndef CMeshManager_h__
#define CMeshManager_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Resource/IMeshManager.h>
#include <FBCore/Core/HashMap.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace render
    {

        class CMeshManager : public SharedObject<IMeshManager>
        {
        public:
            CMeshManager();
            ~CMeshManager() override;

            SmartPtr<IMesh> loadMesh( const String &fileName ) override;
            SmartPtr<ISceneNode> loadMeshes( const String &fileName ) override;

            SmartPtr<IResource> create( const String &name ) override;

            SmartPtr<IResource> create( const String &uuid, const String &name ) override;

            SmartPtr<IResource> load( const String &name ) override;

            SmartPtr<IResource> getByName( const String &name ) override;

            SmartPtr<IResource> getById( const String &uuid ) override;

            void _getObject( void **ppObject ) const override;

            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                              const String &type ) override;

            Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path );

            void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

            SmartPtr<IResource> loadFromFile( const String &filePath ) override;

            // SmartPtr<IResource> createOrRetrieve(const String& name, const String& resourceGroup =
            // IResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) override;

        protected:
            using MeshResources = HashMap<u32, SmartPtr<IMeshResource>>;
            MeshResources m_meshResources;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CMeshManager_h__
