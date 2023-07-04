#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CMeshManager.h>
#include <FBMesh/FBMesh.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        CMeshManager::CMeshManager()
        {
        }

        CMeshManager::~CMeshManager()
        {
        }

        SmartPtr<IMesh> CMeshManager::loadMesh( const String &meshName )
        {
            String fileExt = Path::getFileExtension( meshName );
            if( fileExt == ( ".mesh" ) )
            {
                auto engine = core::IApplicationManager::instance();
                SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();

                String fileName = Path::getFileName( meshName );
                SmartPtr<IStream> stream = fileSystem->open( fileName );
                if( stream )
                {
                    auto mesh = fb::make_ptr<Mesh>();

                    MeshSerializer meshSerializer;
                    meshSerializer.importMesh( stream, static_cast<Mesh *>( mesh.get() ) );

                    mesh->updateAABB( true );

                    return mesh;
                }
            }

            return nullptr;
        }

        SmartPtr<ISceneNode> CMeshManager::loadMeshes( const String &fileName )
        {
            return nullptr;
        }

        SmartPtr<IResource> CMeshManager::create( const String &name )
        {
            return nullptr;
        }

        SmartPtr<IResource> CMeshManager::create( const String &uuid, const String &name )
        {
            return nullptr;
        }

        SmartPtr<IResource> CMeshManager::load( const String &name )
        {
            return nullptr;
        }

        SmartPtr<IResource> CMeshManager::getByName( const String &name )
        {
            return nullptr;
        }

        SmartPtr<IResource> CMeshManager::getById( const String &uuid )
        {
            return nullptr;
        }

        void CMeshManager::_getObject( void **ppObject ) const
        {
        }

        Pair<SmartPtr<IResource>, bool> CMeshManager::createOrRetrieve( const String &uuid,
                                                                        const String &path,
                                                                        const String &type )
        {
            return Pair<SmartPtr<IResource>, bool>();
        }

        Pair<SmartPtr<IResource>, bool> CMeshManager::createOrRetrieve( const String &path )
        {
            return Pair<SmartPtr<IResource>, bool>();
        }

        void CMeshManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
        {
        }

        SmartPtr<IResource> CMeshManager::loadFromFile( const String &filePath )
        {
            return nullptr;
        }
    }  // end namespace render
}  // end namespace fb
