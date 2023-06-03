#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/MeshResource.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MeshResource, IMeshResource );

    MeshResource::MeshResource()
    {
    }

    MeshResource::~MeshResource()
    {
    }

    bool MeshResource::hasSkeleton() const
    {
        return false;
    }

    String MeshResource::getSkeletonName() const
    {
        return "";
    }

    u32 MeshResource::getNumLodLevels() const
    {
        return 0;
    }

    bool MeshResource::isEdgeListBuilt() const
    {
        return false;
    }

    bool MeshResource::hasVertexAnimation() const
    {
        return false;
    }

    void MeshResource::save()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = toData();
            FB_ASSERT( data );

            //auto materialData = data->getDataAsType<data::mesh_importer_data>();

            //auto dataStr = DataUtil::toString( materialData, true );
            //FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            //auto fileId = getSettingsFileSystemId();
            //FB_ASSERT( fileId != 0 );

            //FileInfo fileInfo;
            //if(fileSystem->findFileInfo( fileId, fileInfo ))
            //{
            //    fileSystem->writeAllText( fileInfo.filePath.c_str(), dataStr );
            //}
        }
        catch(std::exception &e)
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::import()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto meshPath = getMeshPath();
            FB_ASSERT( !StringUtil::isNullOrEmpty( meshPath ) );

            if(!StringUtil::isNullOrEmpty( meshPath ))
            {
                resourceDatabase->importFile( meshPath );
            }
        }
        catch(std::exception &e)
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto meshManager = applicationManager->getMeshManager();
            FB_ASSERT( meshManager );

            auto meshPath = getMeshPath();
            if(!StringUtil::isNullOrEmpty( meshPath ))
            {
                if(fileSystem->isExistingFile( meshPath ))
                {
                    auto meshLoader = applicationManager->getMeshLoader();
                    FB_ASSERT( meshLoader );

                    if(meshLoader)
                    {
                        m_mesh = meshLoader->loadMesh( this );

                        if(m_mesh)
                        {
                            m_mesh->setName( meshPath );
                        }
                    }
                }
                else
                {
                    FB_LOG_ERROR( "File not found: " + meshPath );
                }
            }
        }
        catch(std::exception &e)
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            unload( nullptr );
            load( nullptr );
        }
        catch(std::exception &e)
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshResource::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if(m_mesh)
            {
                m_mesh->unload( nullptr );
                m_mesh = nullptr;
            }
        }
        catch(std::exception &e)
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SmartPtr<Properties> MeshResource::getProperties() const
    {
        auto properties = fb::make_ptr<Properties>();
        properties->setProperty( "scale", m_scale );
        properties->setProperty( "constraints", m_constraints );
        properties->setProperty( "animation", m_animation );
        properties->setProperty( "visibility", m_visibility );
        properties->setProperty( "cameras", m_cameras );
        properties->setProperty( "lights", m_lights );
        properties->setProperty( "lightmapUVs", m_lightmapUVs );

        properties->setProperty( "Save", "SaveButton", "button", false );
        properties->setProperty( "Import", "ImportButton", "button", false );

        return properties;
    }

    void MeshResource::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "scale", m_scale );
        properties->getPropertyValue( "constraints", m_constraints );
        properties->getPropertyValue( "animation", m_animation );
        properties->getPropertyValue( "visibility", m_visibility );
        properties->getPropertyValue( "cameras", m_cameras );
        properties->getPropertyValue( "lights", m_lights );
        properties->getPropertyValue( "lightmapUVs", m_lightmapUVs );

        if(properties->hasProperty( "Save" ))
        {
            auto &resetButton = properties->getPropertyObject( "Save" );
            if(resetButton.getAttribute( "click" ) == "true")
            {
                save();
            }
        }

        if(properties->hasProperty( "Import" ))
        {
            auto &resetButton = properties->getPropertyObject( "Import" );
            if(resetButton.getAttribute( "click" ) == "true")
            {
                save();
                import();
            }
        }
    }

    SmartPtr<IStateContext> MeshResource::getStateObject() const
    {
        return m_stateObject;
    }

    void MeshResource::setStateObject( SmartPtr<IStateContext> stateObject )
    {
        m_stateObject = stateObject;
    }

    void MeshResource::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

    f32 MeshResource::getScale() const
    {
        return m_scale;
    }

    void MeshResource::setScale( f32 val )
    {
        m_scale = val;
    }

    IMeshResource::MaterialNaming MeshResource::getMaterialNaming() const
    {
        return m_materialNaming;
    }

    void MeshResource::setMaterialNaming( MaterialNaming val )
    {
        m_materialNaming = val;
    }

    bool MeshResource::getConstraints() const
    {
        return m_constraints;
    }

    void MeshResource::setConstraints( bool val )
    {
        m_constraints = val;
    }

    bool MeshResource::getAnimation() const
    {
        return m_animation;
    }

    void MeshResource::setAnimation( bool val )
    {
        m_animation = val;
    }

    bool MeshResource::getVisibility() const
    {
        return m_visibility;
    }

    void MeshResource::setVisibility( bool val )
    {
        m_visibility = val;
    }

    bool MeshResource::getCameras() const
    {
        return m_cameras;
    }

    void MeshResource::setCameras( bool val )
    {
        m_cameras = val;
    }

    bool MeshResource::getLights() const
    {
        return m_lights;
    }

    void MeshResource::setLights( bool val )
    {
        m_lights = val;
    }

    bool MeshResource::getLightmapUVs() const
    {
        return m_lightmapUVs;
    }

    void MeshResource::setLightmapUVs( bool val )
    {
        m_lightmapUVs = val;
    }

    SmartPtr<IMesh> MeshResource::getMesh() const
    {
        return m_mesh;
    }

    void MeshResource::setMesh( SmartPtr<IMesh> val )
    {
        m_mesh = val;
    }

    String MeshResource::getMeshPath() const
    {
        return m_meshPath;
    }

    void MeshResource::setMeshPath( const String &val )
    {
        m_meshPath = StringUtil::cleanupPath( val );
    }
} // end namespace fb
