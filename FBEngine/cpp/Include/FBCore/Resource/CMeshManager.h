#ifndef __FBMeshManager__H
#define __FBMeshManager__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IMeshManager.h>
#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>

namespace fb
{

    class CMeshManager : public CSharedObject<IMeshManager>
    {
    public:
        CMeshManager();
        ~CMeshManager() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void addMesh( SmartPtr<IMesh> mesh );
        SmartPtr<IMesh> findMesh( const String &name );
        SmartPtr<IMesh> loadMesh( const String &filePath ) override;
        void saveMesh( SmartPtr<IMesh> mesh, const String &filePath );

        void saveMeshToFile( SmartPtr<IMesh> mesh, const String &filePath );

        SmartPtr<render::ISceneNode> loadMeshes( const String &fileName ) override;

        SmartPtr<IResource> create( const String &uuid ) override;
        SmartPtr<IResource> create( const String &uuid, const String &name ) override;

        Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                          const String &type ) override;

        /** @copydoc IResourceManager::createOrRetrieve */
        Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path );

        void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

        /** @copydoc IResourceManager::loadFromFile */
        SmartPtr<IResource> loadFromFile( const String &filePath ) override;

        SmartPtr<IResource> load( const String &name ) override;

        SmartPtr<IResource> getByName( const String &name ) override;

        SmartPtr<IResource> getById( const String &uuid ) override;

        void _getObject( void **ppObject ) const override;

    private:
        Array<SmartPtr<IMesh>> m_meshes;
        Array<SmartPtr<IMeshResource>> m_meshResources;
    };
}  // end namespace fb

#endif
