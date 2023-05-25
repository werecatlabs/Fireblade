#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CMaterialOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTechniqueOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTextureOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialPassOgre.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgre.h>
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialOgre, CMaterial );

        u32 CMaterialOgre::m_nameExt = 0;

        CMaterialOgre::CMaterialOgre()
        {
            setResourceType( ResourceType::Material );

            static const auto MaterialStr = String( "Material" );
            auto name = MaterialStr + StringUtil::toString( m_nameExt++ );

            if( auto handle = getHandle() )
            {
                handle->setName( name );
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();
            setStateObject( stateObject );

            auto stateListener = factoryManager->make_ptr<MaterialStateListener>();
            stateListener->setOwner( this );
            setStateListener( stateListener );
            stateObject->addStateListener( stateListener );

            auto state = factoryManager->make_ptr<MaterialState>();
            stateObject->setState( state );

            auto renderTask = graphicsSystem->getStateTask();
            state->setTaskId( renderTask );
        }

        CMaterialOgre::~CMaterialOgre()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void CMaterialOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
                FB_ASSERT( resourceGroupManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                const auto stateTask = graphicsSystem->getStateTask();

                setLoadingState( LoadingState::Loading );

                FB_ASSERT( getMaterialType() < IMaterial::MaterialType::Count );

                if( !m_material )
                {
                    createMaterialByType();
                }

                FB_ASSERT( m_material );

                FB_ASSERT( m_material->getNumTechniques() == 1 );

                if( m_material )
                {
                    m_material->load();
                }

                FB_ASSERT( m_material->getNumTechniques() == 1 );

                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( auto technique : techniques )
                    {
                        technique->load( nullptr );
                    }
                }

                setLoadingState( LoadingState::Loaded );

                auto message = factoryManager->make_ptr<StateMessageLoad>();
                message->setType( StateMessageLoad::LOADED_HASH );
                message->setObject( this );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( stateTask, message );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialOgre::reload( SmartPtr<ISharedObject> data )
        {
            try
            {
                FB_ASSERT( isThreadSafe() );

                if( m_material )
                {
                    m_material->changeGroupOwnership(
                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                    m_material->reload();

                    for( u32 techIdx = 0; techIdx < m_material->getNumTechniques(); ++techIdx )
                    {
                        Ogre::Technique *tech = m_material->getTechnique( techIdx );
                        for( u32 passIdx = 0; passIdx < tech->getNumPasses(); ++passIdx )
                        {
                            Ogre::Pass *pass = tech->getPass( passIdx );

                            for( u32 texIdx = 0; texIdx < pass->getNumTextureUnitStates(); ++texIdx )
                            {
                                Ogre::TextureUnitState *texState = pass->getTextureUnitState( texIdx );
                                texState->retryTextureLoad();
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &state = getLoadingState();
                if( state != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto techniques = getTechniques();
                    for( auto technique : techniques )
                    {
                        technique->unload( data );
                    }

                    setTechniquesPtr( nullptr );

                    if( auto stateManager = applicationManager->getStateManager() )
                    {
                        if( auto stateObject = getStateObject() )
                        {
                            if( auto stateListener = getStateListener() )
                            {
                                stateObject->removeStateListener( stateListener );
                            }

                            stateManager->removeStateObject( stateObject );

                            stateObject->unload( nullptr );
                            setStateObject( nullptr );
                        }

                        if( auto stateListener = getStateListener() )
                        {
                            stateListener->unload( nullptr );
                            setStateListener( nullptr );
                        }
                    }

                    m_material = nullptr;

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialOgre::setCubicTexture( const String &fileName, bool uvw, u32 layerIdx )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            if( isThreadSafe() )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        Ogre::TextureUnitState *textureUnitState = pass->getTextureUnitState( layerIdx );
                        if( textureUnitState )
                        {
                            textureUnitState->setCubicTextureName( fileName.c_str(), uvw );
                        }
                    }
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageSetTexture>();
                message->setType( SET_TEXTURE_HASH );
                message->setTextureName( fileName );
                message->setTextureIndex( layerIdx );
                addMessage( message );
            }
        }

        void CMaterialOgre::setCubicTexture( Array<SmartPtr<render::ITexture>> textures,
                                             u32 layerIdx /*= 0 */ )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            if( isThreadSafe() )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        Ogre::TextureUnitState *textureUnitState = pass->getTextureUnitState( layerIdx );
                        if( textureUnitState )
                        {
                            std::vector<String> textureNames;
                            textureNames.resize( 6 );

                            Array<u32> map;
                            map.reserve( 6 );
                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Right );
                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Left );
                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Up );
                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Down );
                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Front );
                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Back );

                            for( size_t i = 0; i < 6; ++i )
                            {
                                auto index = map[i];
                                auto pTexture = textures[index];
                                if( pTexture )
                                {
                                    textureNames[i] = pTexture->getFilePath();
                                }
                                else
                                {
                                    textureNames[i] = "checker.png";
                                }
                            }

                            auto materialName = getHandle()->getName();
                            auto grp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                            auto textureManager = graphicsSystem->getTextureManager();
                            auto textureResult =
                                textureManager->createOrRetrieve( materialName + "_Cubemap" );
                            auto texture = fb::static_pointer_cast<CTextureOgre>( textureResult.first );
                            texture->setTextureType( ITexture::TextureType::TEX_TYPE_CUBE_MAP );
                            texture->load( nullptr );

#if 0
                            textureNames[0] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_0_Front+Z.png";
                            textureNames[1] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_1_Back-Z.png";
                            textureNames[2] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_2_Left+X.png";
                            textureNames[3] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_3_Right-X.png";
                            textureNames[4] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_4_Up+Y.png";
                            textureNames[5] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_5_Down-Y.png";

                            for( size_t i = 0; i < 6; ++i )
                            {
                                textureNames[i] = "checker.png";
                            }
#endif

                            auto tex = texture->getTexture();
                            if( tex )
                            {
                                tex->setLayerNames( textureNames );
                                tex->reload();

                                textureUnitState->setTexture( tex );
                            }

                            //textureUnitState->setLayerArrayNames( Ogre::TEX_TYPE_CUBE_MAP,
                            //                                      textureNames );
                        }
                    }
                }

                if( auto stateObject = getStateObject() )
                {
                    stateObject->setDirty( true );
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageObjectsArray>();
                message->setType( StateMessage::SET_CUBEMAP );

                auto objects = Array<SmartPtr<ISharedObject>>( textures.begin(), textures.end() );
                message->setObjects( objects );

                addMessage( message );
            }
        }

        void CMaterialOgre::setFragmentParam( const String &name, f32 value )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        if( pass->hasFragmentProgram() )
                        {
                            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
                                pass->getFragmentProgramParameters();
                            const Ogre::GpuNamedConstants &fragmentNamedConstants =
                                fragmentParameters->getConstantDefinitions();
                            fragmentParameters->setNamedConstant( name.c_str(), value );
                        }
                    }
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageFragmentParam>();
                message->setType( FRAGMENT_FLOAT_HASH );
                message->setName( name );
                message->setFloat( value );

                if( auto stateContext = getStateObject() )
                {
                    stateContext->addMessage( renderTask, message );
                }
            }
        }

        void CMaterialOgre::setFragmentParam( const String &name, const Vector2F &value )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        if( pass->hasFragmentProgram() )
                        {
                            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
                                pass->getFragmentProgramParameters();
                            const Ogre::GpuNamedConstants &fragmentNamedConstants =
                                fragmentParameters->getConstantDefinitions();
                            // fragmentParameters->setNamedConstant(name.c_str(),
                            // Ogre::Vector2(value.X(), value.Y()));
                        }
                    }
                }
            }
            else
            {
                SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
                message->setType( FRAGMENT_VECTOR2F_HASH );
                message->setName( name );
                message->setVector2f( value );

                if( auto stateConext = getStateObject() )
                {
                    stateConext->addMessage( renderTask, message );
                }
            }
        }

        void CMaterialOgre::setFragmentParam( const String &name, const Vector3F &value )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        if( pass->hasFragmentProgram() )
                        {
                            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
                                pass->getFragmentProgramParameters();
                            const Ogre::GpuNamedConstants &fragmentNamedConstants =
                                fragmentParameters->getConstantDefinitions();
                            fragmentParameters->setNamedConstant(
                                name.c_str(), Ogre::Vector3( value.X(), value.Y(), value.Z() ) );
                        }
                    }
                }
            }
            else
            {
                SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
                message->setType( FRAGMENT_VECTOR3F_HASH );
                message->setName( name );
                message->setVector3f( value );

                if( auto stateConext = getStateObject() )
                {
                    stateConext->addMessage( renderTask, message );
                }
            }
        }

        void CMaterialOgre::setFragmentParam( const String &name, const Vector4F &value )
        {
            auto applicationManager = core::IApplicationManager::instance();

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        if( pass->hasFragmentProgram() )
                        {
                            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
                                pass->getFragmentProgramParameters();
                            const Ogre::GpuNamedConstants &fragmentNamedConstants =
                                fragmentParameters->getConstantDefinitions();
                            fragmentParameters->setNamedConstant(
                                name.c_str(),
                                Ogre::Vector4( value.X(), value.Y(), value.Z(), value.W() ) );
                        }
                    }
                }
            }
            else
            {
                SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
                message->setType( FRAGMENT_VECTOR4F_HASH );
                message->setName( name );
                message->setVector4f( value );

                if( auto stateConext = getStateObject() )
                {
                    stateConext->addMessage( renderTask, message );
                }
            }
        }

        void CMaterialOgre::setFragmentParam( const String &name, const ColourF &value )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            auto renderTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                Ogre::Technique *tech = m_material->getBestTechnique();
                if( tech )
                {
                    Ogre::Pass *pass = tech->getPass( 0 );
                    if( pass )
                    {
                        if( pass->hasFragmentProgram() )
                        {
                            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
                                pass->getFragmentProgramParameters();
                            const Ogre::GpuNamedConstants &fragmentNamedConstants =
                                fragmentParameters->getConstantDefinitions();
                            fragmentParameters->setNamedConstant(
                                name.c_str(), Ogre::ColourValue( value.r, value.g, value.b, value.a ) );
                        }
                    }
                }
            }
            else
            {
                SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
                message->setType( FRAGMENT_COLOUR_HASH );
                message->setName( name );
                message->setColourf( value );

                if( auto stateConext = getStateObject() )
                {
                    stateConext->addMessage( renderTask, message );
                }
            }
        }

        void CMaterialOgre::setScale( const Vector3F &scale, u32 textureIndex /*= 0*/,
                                      u32 passIndex /*= 0*/, u32 techniqueIndex /*= 0 */ )
        {
            if( auto material = getMaterial() )
            {
                auto mat = Ogre::Matrix4::IDENTITY;
                mat.setScale( Ogre::Vector3( scale.X(), scale.Y(), scale.Z() ) );
                material->getTechnique( techniqueIndex )
                    ->getPass( passIndex )
                    ->getTextureUnitState( textureIndex )
                    ->setTextureTransform( mat );
            }
        }

        SmartPtr<IMaterialNode> CMaterialOgre::getRoot() const
        {
            return m_root;
        }

        void CMaterialOgre::setRoot( SmartPtr<IMaterialNode> root )
        {
            m_root = root;
        }

        void CMaterialOgre::createMaterialByType()
        {
            try
            {
                auto uuid = String();

                if( auto handle = getHandle() )
                {
                    uuid = handle->getUUID();
                }

                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                auto materialManager = Ogre::MaterialManager::getSingletonPtr();

                auto resourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                if( m_material == nullptr )
                {
                    //if ( getNumTechniques() != 0 )
                    //{
                    //    removeAllTechniques();
                    //}

                    switch( auto materialType = getMaterialType() )
                    {
                    case MaterialType::Standard:
                    {
                        //auto result = materialManager->load( "glTF2/PBR", resourceGroup );
                        //auto pbrBase = Ogre::dynamic_pointer_cast<Ogre::Material>( result );
                        //if( pbrBase )
                        //{
                        //    m_material = pbrBase->clone( uuid );
                        //}
                        //else
                        {
                            auto result = materialManager->createOrRetrieve( uuid, resourceGroup );
                            m_material = Ogre::static_pointer_cast<Ogre::Material>( result.first );
                        }
                    }
                    break;
                    default:
                    {
                        auto result = materialManager->createOrRetrieve( uuid, resourceGroup );
                        m_material = Ogre::static_pointer_cast<Ogre::Material>( result.first );
                    }
                    };
                }

                if( m_material )
                {
                    m_material->load();
                }

                //if( auto p = getTechniquesPtr() )
                //{
                //    auto &techniques = *p;
                //    for( auto t : techniques )
                //    {
                //        t->unload( nullptr );
                //    }

                //    techniques.clear();
                //}

                //m_material->removeAllTechniques();

                //FB_ASSERT( isValid() );

                auto ogreTechniques = m_material->getTechniques();

                auto count = 0;

                if( ogreTechniques.size() > getNumTechniques() )
                {
                    while( ogreTechniques.size() != getNumTechniques() )
                    {
                        auto technique = fb::make_ptr<CMaterialTechniqueOgre>();
                        addTechnique( technique );
                    }
                }

                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( size_t i = 0; i < techniques.size(); ++i )
                    {
                        auto technique =
                            fb::static_pointer_cast<CMaterialTechniqueOgre>( techniques[i] );

                        if( count < ogreTechniques.size() )
                        {
                            technique->setTechnique( ogreTechniques[count] );
                        }

                        technique->setMaterial( this );

                        count++;
                    }
                }

                //if( auto p = getTechniquesPtr() )
                //{
                //    auto &techniques = *p;
                //    techniques.push_back( technique );
                //}

                FB_ASSERT( isValid() );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        //
        // SmartPtr<render::IMaterialTechnique> CMaterial::createTechnique()
        //{
        //	auto applicationManager = core::IApplicationManager::instance();
        //	FB_ASSERT(applicationManager);

        //	auto factoryManager = applicationManager->getFactoryManager();
        //	auto technique = factoryManager->make_ptr<CMaterialTechnique>();
        //	technique->setOwner(this);
        //	m_techniques.push_back(technique);
        //	return technique;
        //}

        SmartPtr<ISharedObject> CMaterialOgre::toData() const
        {
            //try
            //{
            //    auto applicationManager = core::IApplicationManager::instance();
            //    FB_ASSERT( applicationManager );

            //    auto factoryManager = applicationManager->getFactoryManager();
            //    FB_ASSERT( factoryManager );

            //    auto data = factoryManager->make_ptr<Data<data::material_graph>>();
            //    auto materialData = data->getDataAsType<data::material_graph>();
            //    materialData->materialType = static_cast<s32>( getMaterialType() );

            //    auto techniques = getTechniques();
            //    for( auto technique : techniques )
            //    {
            //        auto pTechniqueData = technique->toData();
            //        auto techniqueData = pTechniqueData->getDataAsType<data::material_scheme>();

            //        materialData->schemes.push_back( *techniqueData );
            //    }

            //    return data;
            //}
            //catch( std::exception &e )
            //{
            //    FB_LOG_EXCEPTION( e );
            //}

            return nullptr;
        }

        void CMaterialOgre::fromData( SmartPtr<ISharedObject> data )
        {
            //try
            //{
            //    auto applicationManager = core::IApplicationManager::instance();
            //    FB_ASSERT( applicationManager );

            //    auto factoryManager = applicationManager->getFactoryManager();
            //    FB_ASSERT( factoryManager );

            //    auto materialData = data->getDataAsType<data::material_graph>();

            //    auto materialType = static_cast<IMaterial::MaterialType>( materialData->materialType );
            //    setMaterialType( materialType );

            //    auto &schemes = materialData->schemes;

            //    auto numTechniques = getNumTechniques();
            //    if( numTechniques < schemes.size() )
            //    {
            //        while( numTechniques != schemes.size() )
            //        {
            //            createTechnique();
            //            numTechniques = getNumTechniques();
            //        }
            //    }

            //    //FB_ASSERT(getNumTechniques() == 1);

            //    auto count = 0;
            //    for( auto &scheme : schemes )
            //    {
            //        if( auto p = getTechniquesPtr() )
            //        {
            //            auto &techniques = *p;

            //            if( count < techniques.size() )
            //            {
            //                auto technique = techniques[count];
            //                if( technique )
            //                {
            //                    auto pSchemeData =
            //                        factoryManager->make_ptr<Data<data::material_scheme>>();
            //                    pSchemeData->setData( &scheme );
            //                    technique->fromData( pSchemeData );
            //                }
            //            }
            //        }

            //        count++;
            //    }
            //}
            //catch( std::exception &e )
            //{
            //    FB_LOG_EXCEPTION( e );
            //}
        }

        SmartPtr<Properties> CMaterialOgre::getProperties() const
        {
            try
            {
                auto properties = CMaterial::getProperties();

                static const auto materialTypeStr = String( "Material Type" );

                auto materialTypeValueStr = GraphicsUtil::getMaterialType( m_materialType );
                properties->setProperty( materialTypeStr, materialTypeValueStr );

                auto &materialPathProperty = properties->getPropertyObject( materialTypeStr );
                materialPathProperty.setType( "enum" );

                auto enumValues = GraphicsUtil::getMaterialTypesString();
                materialPathProperty.setAttribute( "enum", enumValues );

                return properties;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CMaterialOgre::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                CMaterial::setProperties( properties );

                auto materialTypeStr = String( "" );
                properties->getPropertyValue( "Material Type", materialTypeStr );
                materialTypeStr = StringUtil::trim( materialTypeStr );

                auto enumValues = GraphicsUtil::getMaterialTypes();

                auto it = std::find( enumValues.begin(), enumValues.end(), materialTypeStr );
                if( it != enumValues.end() )
                {
                    auto pos = std::distance( enumValues.begin(), it );

                    auto materialType = static_cast<IMaterial::MaterialType>( pos );
                    if( m_materialType != materialType )
                    {
                        m_materialType = materialType;
                        createMaterialByType();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Array<SmartPtr<ISharedObject>> CMaterialOgre::getChildObjects() const
        {
            auto objects = CMaterial::getChildObjects();

            return objects;
        }

        Ogre::MaterialPtr CMaterialOgre::getMaterial() const
        {
            return m_material;
        }

        void CMaterialOgre::setMaterial( Ogre::MaterialPtr material )
        {
            m_material = material;
        }

        bool CMaterialOgre::isValid() const
        {
            if( isLoaded() )
            {
                if( auto material = getMaterial() )
                {
                    auto techniques = getTechniques();
                    return material->getNumTechniques() == techniques.size();
                }
            }

            return true;
        }

        void CMaterialOgre::MaterialStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( !m_owner->isLoaded() )
            {
                m_owner->load( nullptr );
            }

            auto messageType = message->getType();

            if( message->isExactly<StateMessagePair<bool, int>>())
            {
                auto pairMessage = fb::static_pointer_cast<StateMessagePair<bool, int>>( message );

                auto first = pairMessage->getFirst();
                auto second = pairMessage->getSecond();

                if (messageType == IMaterial::LIGHTING_ENABLED_HASH)
                {
                    m_owner->setLightingEnabled( first, second );
                }
            }
            else if( message->isExactly<StateMessageUIntValue>() )
            {
                auto intMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );

                if( messageType == StringUtil::getHash( "materialType" ) )
                {
                    m_owner->setMaterialType( (IMaterial::MaterialType)intMessage->getValue() );
                }
            }
            else if( message->isExactly<StateMessageLoad>() )
            {
                auto loadMessage = fb::static_pointer_cast<StateMessageLoad>( message );
                if( messageType == StateMessageLoad::LOAD_HASH )
                {
                    m_owner->load( nullptr );
                }
                else
                {
                    m_owner->reload( nullptr );
                }
            }
            else if( message->isExactly<StateMessageFragmentParam>() )
            {
                auto fragmentMessage = fb::static_pointer_cast<StateMessageFragmentParam>( message );
                
                if( messageType == FRAGMENT_FLOAT_HASH )
                {
                    m_owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getFloat() );
                }
                else if( messageType == FRAGMENT_VECTOR2F_HASH )
                {
                    m_owner->setFragmentParam( fragmentMessage->getName(),
                                               fragmentMessage->getVector2f() );
                }
                else if( messageType == FRAGMENT_VECTOR3F_HASH )
                {
                    m_owner->setFragmentParam( fragmentMessage->getName(),
                                               fragmentMessage->getVector3f() );
                }
                else if( messageType == FRAGMENT_VECTOR4F_HASH )
                {
                    m_owner->setFragmentParam( fragmentMessage->getName(),
                                               fragmentMessage->getVector4f() );
                }
                else if( messageType == FRAGMENT_COLOUR_HASH )
                {
                    m_owner->setFragmentParam( fragmentMessage->getName(),
                                               fragmentMessage->getColourf() );
                }
            }
            else if( message->isExactly<StateMessageSetTexture>() )
            {
                auto textureMessage = fb::static_pointer_cast<StateMessageSetTexture>( message );
                if( auto texture = textureMessage->getTexture() )
                {
                    const auto textureIndex = textureMessage->getTextureIndex();
                    m_owner->setTexture( texture, textureIndex );
                }
                else
                {
                    const auto textureName = textureMessage->getTextureName();
                    const auto textureIndex = textureMessage->getTextureIndex();
                    m_owner->setTexture( textureName, textureIndex );
                }
            }
            else if( message->isExactly<StateMessageObjectsArray>() )
            {
                auto arrayMessage = fb::static_pointer_cast<StateMessageObjectsArray>( message );
                auto value = arrayMessage->getObjects();

                if( messageType == StateMessage::SET_CUBEMAP )
                {
                    m_owner->setCubicTexture( Array<SmartPtr<ITexture>>( value.begin(), value.end() ) );
                }
            }
        }

        void CMaterialOgre::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                auto materialState = fb::static_pointer_cast<MaterialState>( state );
                if( materialState->getMaterialType() != m_owner->m_materialType )
                {
                    owner->m_materialType = materialState->getMaterialType();
                    owner->removeAllTechniques();
                    owner->createMaterialByType();
                    owner->load( nullptr );
                }
            }

            const auto &loadingState = m_owner->getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                state->setDirty( false );
            }
        }

        void CMaterialOgre::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CMaterialOgre *CMaterialOgre::MaterialStateListener::getOwner() const
        {
            return m_owner;
        }

        void CMaterialOgre::MaterialStateListener::setOwner( CMaterialOgre *owner )
        {
            m_owner = owner;
        }

        CMaterialOgre::MaterialStateListener::~MaterialStateListener()
        {
        }

        CMaterialOgre::MaterialStateListener::MaterialStateListener()
        {
        }

        CMaterialOgre::MaterialStateListener::MaterialStateListener( CMaterialOgre *material ) :
            m_owner( material )
        {
        }

        void CMaterialOgre::MaterialEvents::preRenderTargetUpdate( const Ogre::RenderTargetEvent &evt )
        {
        }

        void CMaterialOgre::MaterialEvents::postRenderTargetUpdate( const Ogre::RenderTargetEvent &evt )
        {
        }

        void CMaterialOgre::MaterialEvents::preViewportUpdate(
            const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        void CMaterialOgre::MaterialEvents::postViewportUpdate(
            const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        void CMaterialOgre::MaterialEvents::viewportAdded( const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        void CMaterialOgre::MaterialEvents::viewportRemoved( const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        CMaterialOgre::MaterialEvents::~MaterialEvents()
        {
        }

        CMaterialOgre::MaterialEvents::MaterialEvents( CMaterialOgre *material ) : m_material( material )
        {
        }
    }  // end namespace render
}  // end namespace fb
