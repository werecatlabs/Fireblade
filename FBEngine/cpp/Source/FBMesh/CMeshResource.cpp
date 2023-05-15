#include <FBMesh/FBMeshPCH.h>
#include <FBMesh/CMeshResource.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, CMeshResource, IMeshResource );

    CMeshResource::CMeshResource()
    {
    }

    CMeshResource::~CMeshResource()
    {
    }

    bool CMeshResource::hasSkeleton() const
    {
        return false;
    }

    String CMeshResource::getSkeletonName() const
    {
        return "";
    }

    u32 CMeshResource::getNumLodLevels() const
    {
        return 0;
    }

    bool CMeshResource::isEdgeListBuilt() const
    {
        return false;
    }

    bool CMeshResource::hasVertexAnimation() const
    {
        return false;
    }

    void CMeshResource::save()
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

    void CMeshResource::import()
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

    void CMeshResource::load( SmartPtr<ISharedObject> data )
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

    void CMeshResource::reload( SmartPtr<ISharedObject> data )
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

    void CMeshResource::unload( SmartPtr<ISharedObject> data )
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

    SmartPtr<Properties> CMeshResource::getProperties() const
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

    void CMeshResource::setProperties( SmartPtr<Properties> properties )
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

    SmartPtr<IStateContext> CMeshResource::getStateObject() const
    {
        return m_stateObject;
    }

    void CMeshResource::setStateObject( SmartPtr<IStateContext> stateObject )
    {
        m_stateObject = stateObject;
    }

    void CMeshResource::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

    f32 CMeshResource::getScale() const
    {
        return m_scale;
    }

    void CMeshResource::setScale( f32 val )
    {
        m_scale = val;
    }

    IMeshResource::MaterialNaming CMeshResource::getMaterialNaming() const
    {
        return m_materialNaming;
    }

    void CMeshResource::setMaterialNaming( MaterialNaming val )
    {
        m_materialNaming = val;
    }

    bool CMeshResource::getConstraints() const
    {
        return m_constraints;
    }

    void CMeshResource::setConstraints( bool val )
    {
        m_constraints = val;
    }

    bool CMeshResource::getAnimation() const
    {
        return m_animation;
    }

    void CMeshResource::setAnimation( bool val )
    {
        m_animation = val;
    }

    bool CMeshResource::getVisibility() const
    {
        return m_visibility;
    }

    void CMeshResource::setVisibility( bool val )
    {
        m_visibility = val;
    }

    bool CMeshResource::getCameras() const
    {
        return m_cameras;
    }

    void CMeshResource::setCameras( bool val )
    {
        m_cameras = val;
    }

    bool CMeshResource::getLights() const
    {
        return m_lights;
    }

    void CMeshResource::setLights( bool val )
    {
        m_lights = val;
    }

    bool CMeshResource::getLightmapUVs() const
    {
        return m_lightmapUVs;
    }

    void CMeshResource::setLightmapUVs( bool val )
    {
        m_lightmapUVs = val;
    }

    SmartPtr<IMesh> CMeshResource::getMesh() const
    {
        return m_mesh;
    }

    void CMeshResource::setMesh( SmartPtr<IMesh> val )
    {
        m_mesh = val;
    }

    String CMeshResource::getMeshPath() const
    {
        return m_meshPath;
    }

    void CMeshResource::setMeshPath( const String &val )
    {
        m_meshPath = StringUtil::cleanupPath( val );
    }
} // end namespace fb
