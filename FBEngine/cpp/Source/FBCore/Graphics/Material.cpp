#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Material.h>
#include <FBCore/Graphics/MaterialTechnique.h>
#include <FBCore/Graphics/MaterialTexture.h>
#include <FBCore/Graphics/MaterialPass.h>
#include <FBCore/Graphics/Texture.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/State/Messages/StateMessageSetTexture.h>
#include <FBCore/State/Messages/StateMessagePair.h>
#include <FBCore/State/Messages/StateMessageLoad.h>
#include <FBCore/State/Messages/StateMessageObjectsArray.h>
#include <FBCore/State/Messages/StateMessageFragmentParam.h>
#include <FBCore/State/Messages/StateMessageIntValue.h>
#include <FBCore/State/Messages/StateMessageUIntValue.h>
#include <FBCore/State/States/MaterialState.h>
#include <FBGraphics/GraphicsUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, Material, IMaterial );

    u32 Material::m_nameExt = 0;

    Material::Material()
    {
        setResourceType( ResourceType::Material );

        static const auto MaterialStr = String( "Material" );
        auto name = MaterialStr + StringUtil::toString( m_nameExt++ );

        if( auto handle = getHandle() )
        {
            handle->setName( name );
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();
        setStateContext( stateContext );

        /*
        auto stateListener = factoryManager->make_ptr<MaterialStateListener>();
        stateListener->setOwner( this );
        m_stateListener = stateListener;
        stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<MaterialState>();
        stateContext->setState(state);
        stateContext->setOwner(this);

        auto renderTask = graphicsSystem->getStateTask();
        state->setTaskId(renderTask);
        */

        auto textures = fb::make_shared<Array<SmartPtr<ITexture>>>();
        textures->resize( 32 );
        setTextures( textures );
    }

    Material::~Material()
    {
        const auto &loadingState = getLoadingState();
        if( loadingState != LoadingState::Unloaded )
        {
            unload( nullptr );
        }
    }

    void Material::saveToFile( const String &filePath )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto data = toData();
        auto properties = fb::static_pointer_cast<Properties>( data );
        auto materialStr = DataUtil::toString( properties.get(), true );

        fileSystem->writeAllText( filePath, materialStr );
    }

    void Material::loadFromFile( const String &filePath )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto stream = fileSystem->open( filePath, true, false, false, false, false );
        if( !stream )
        {
            stream = fileSystem->open( filePath, true, false, false, true, true );
        }

        if( stream )
        {
            auto materialStr = stream->getAsString();

            auto materialData = fb::make_ptr<Properties>();
            DataUtil::parse( materialStr, materialData.get() );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( filePath, fileInfo ) )
            {
                auto fileId = fileInfo.fileId;
                setFileSystemId( fileId );
            }

            setFilePath( filePath );

            fromData( materialData );
        }
    }

    void Material::save()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = toData();
            auto materialData = fb::static_pointer_cast<Properties>( data );
            FB_ASSERT( materialData );

            auto dataStr = DataUtil::toString( materialData.get(), true );
            FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            auto fileId = getFileSystemId();
            if( fileId != 0 )
            {
                FileInfo fileInfo;
                if( fileSystem->findFileInfo( fileId, fileInfo ) )
                {
                    auto filePath = String( fileInfo.filePath.c_str() );
                    fileSystem->writeAllText( filePath, dataStr );
                }
                else
                {
                    FB_LOG_ERROR( "Could not save material: " + String( fileInfo.filePath.c_str() ) );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Material::load( SmartPtr<ISharedObject> data )
    {
        auto filePath = getFilePath();
        if( !StringUtil::isNullOrEmpty( filePath ) )
        {
            loadFromFile( filePath );
        }
    }

    void Material::reload( SmartPtr<ISharedObject> data )
    {
    }

    void Material::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto techniques = getTechniques();
                for( auto technique : techniques )
                {
                    technique->unload( data );
                }

                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    techniques.clear();
                }

                if( auto stateContext = getStateContext() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateContext );

                    stateContext->unload( nullptr );
                    setStateContext( nullptr );
                }

                if( auto stateListener = getStateListener() )
                {
                    stateListener->unload( nullptr );
                    setStateListener( nullptr );
                }

                ResourceGraphics<IMaterial>::unload( nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Material::setTexture( SmartPtr<ITexture> texture, u32 layerIdx )
    {
        try
        {
            if( auto p = getTextures() )
            {
                auto &textures = *p;
                textures[layerIdx] = texture;
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto renderTask = graphicsSystem->getRenderTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded && task == renderTask )
            {
                auto techniques = getTechniques();
                for( auto technique : techniques )
                {
                    FB_ASSERT( technique->isValid() );

                    auto passes = technique->getPasses();
                    for( auto pass : passes )
                    {
                        FB_ASSERT( pass->isValid() );

                        auto textureUnits = pass->getTextureUnits();
                        if( !textureUnits.empty() )
                        {
                            if( layerIdx < textureUnits.size() )
                            {
                                auto textureUnit = textureUnits[layerIdx];
                                FB_ASSERT( textureUnit->isValid() );

                                if( !textureUnit->isLoaded() )
                                {
                                    textureUnit->load( nullptr );
                                }

                                textureUnit->setTexture( texture );
                            }
                        }
                    }
                }

                if( auto stateContext = getStateContext() )
                {
                    stateContext->setDirty( true );
                }
            }
            else
            {
                auto message = fb::make_ptr<StateMessageSetTexture>();
                message->setType( SET_TEXTURE_HASH );
                message->setTexture( texture );
                message->setTextureIndex( layerIdx );

                if( auto stateContext = getStateContext() )
                {
                    stateContext->addMessage( renderTask, message );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Material::setTexture( const String &fileName, u32 layerIdx )
    {
        try
        {
            if( !StringUtil::isNullOrEmpty( fileName ) )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();
                auto texture = resourceDatabase->loadResourceByType<ITexture>( fileName );
                if( texture )
                {
                    setTexture( texture, layerIdx );

                    if( auto stateContext = getStateContext() )
                    {
                        stateContext->setDirty( true );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Material::getTextures() const -> SharedPtr<Array<SmartPtr<ITexture>>>
    {
        return m_textures;
    }

    void Material::setTextures( SharedPtr<Array<SmartPtr<ITexture>>> textures )
    {
        m_textures = textures;
    }

    auto Material::getTextureName( u32 layerIdx /*= 0*/ ) const -> String
    {
        if( auto texture = getTexture( layerIdx ) )
        {
            auto handle = texture->getHandle();
            return handle->getName();
        }

        return "";
    }

    auto Material::getTexture( u32 layerIdx ) const -> SmartPtr<ITexture>
    {
        if( auto p = getTextures() )
        {
            auto &textures = *p;
            if( layerIdx < textures.size() )
            {
                return textures[layerIdx];
            }
        }

        //auto techniques = getTechniques();
        //for( auto technique : techniques )
        //{
        //    FB_ASSERT( technique->isValid() );

        //    auto passes = technique->getPasses();
        //    for( auto pass : passes )
        //    {
        //        FB_ASSERT( pass->isValid() );

        //        auto textureUnits = pass->getTextureUnits();
        //        if( layerIdx < textureUnits.size() )
        //        {
        //            auto textureUnit = textureUnits[layerIdx];
        //            return textureUnit->getTexture();
        //        }
        //    }
        //}

        return nullptr;
    }

    void Material::setLightingEnabled( bool enabled, s32 passIdx /*= -1 */ )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        if( isThreadSafe() )
        {
            if( passIdx == -1 )
            {
                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( auto technique : techniques )
                    {
                        FB_ASSERT( technique->isValid() );
                        auto passes = technique->getPasses();
                        for( auto pass : passes )
                        {
                            FB_ASSERT( pass->isValid() );
                            pass->setLightingEnabled( enabled );
                        }
                    }
                }
            }
            else
            {
                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( auto technique : techniques )
                    {
                        FB_ASSERT( technique->isValid() );

                        auto passes = technique->getPasses();
                        if( passIdx < passes.size() )
                        {
                            auto pass = passes[passIdx];
                            FB_ASSERT( pass->isValid() );
                            pass->setLightingEnabled( enabled );
                        }
                    }
                }
            }
        }
        else
        {
            auto message = factoryManager->make_ptr<StateMessagePair<bool, int>>();
            message->setType( LIGHTING_ENABLED_HASH );
            message->setFirst( enabled );
            message->setSecond( passIdx );

            addMessage( message );
        }
    }

    auto Material::getLightingEnabled( s32 passIdx /*= -1 */ ) const -> bool
    {
        return false;
    }

    void Material::setCubicTexture( const String &fileName, bool uvw, u32 layerIdx )
    {
    }

    void Material::setCubicTexture( Array<SmartPtr<ITexture>> textures, u32 layerIdx /*= 0 */ )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();
        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            //                Ogre::Technique *tech = m_material->getBestTechnique();
            //                if( tech )
            //                {
            //                    Ogre::Pass *pass = tech->getPass( 0 );
            //                    if( pass )
            //                    {
            //                        Ogre::TextureUnitState *textureUnitState = pass->getTextureUnitState( layerIdx );
            //                        if( textureUnitState )
            //                        {
            //                            std::vector<String> textureNames;
            //                            textureNames.resize( 6 );
            //
            //                            Array<u32> map;
            //                            map.reserve( 6 );
            //                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Right );
            //                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Left );
            //                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Up );
            //                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Down );
            //                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Front );
            //                            map.push_back( (u32)IMaterial::SkyboxTextureTypes::Back );
            //
            //                            for( size_t i = 0; i < 6; ++i )
            //                            {
            //                                auto index = map[i];
            //                                auto pTexture = textures[index];
            //                                if( pTexture )
            //                                {
            //                                    textureNames[i] = pTexture->getFilePath();
            //                                }
            //                                else
            //                                {
            //                                    textureNames[i] = "checker.png";
            //                                }
            //                            }
            //
            //                            auto materialName = getHandle()->getName();
            //                            auto grp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
            //
            //                            auto textureManager = graphicsSystem->getTextureManager();
            //                            auto textureResult =
            //                                textureManager->createOrRetrieve( materialName + "_Cubemap" );
            //                            auto texture = fb::static_pointer_cast<CTextureOgre>( textureResult.first );
            //                            texture->setTextureType( ITexture::TextureType::TEX_TYPE_CUBE_MAP );
            //                            texture->load( nullptr );
            //
            //#if 0
            //                            textureNames[0] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_0_Front+Z.png";
            //                            textureNames[1] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_1_Back-Z.png";
            //                            textureNames[2] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_2_Left+X.png";
            //                            textureNames[3] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_3_Right-X.png";
            //                            textureNames[4] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_4_Up+Y.png";
            //                            textureNames[5] = "Sky_Cartoon_Airbrush_Day_NoSun_Cam_5_Down-Y.png";
            //
            //                            for( size_t i = 0; i < 6; ++i )
            //                            {
            //                                textureNames[i] = "checker.png";
            //                            }
            //#endif
            //
            //                            auto tex = texture->getTexture();
            //                            if( tex )
            //                            {
            //                                tex->setLayerNames( textureNames );
            //                                textureUnitState->setTexture( tex );
            //                            }
            //
            //                            //textureUnitState->setLayerArrayNames( Ogre::TEX_TYPE_CUBE_MAP,
            //                            //                                      textureNames );
            //                        }
            //                    }
            //                }
        }
        else
        {
            auto message = factoryManager->make_ptr<StateMessageObjectsArray>();
            message->setType( StateMessage::SET_CUBEMAP );
            message->setObjects( Array<SmartPtr<ISharedObject>>( textures.begin(), textures.end() ) );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( renderTask, message );
            }
        }
    }

    void Material::setFragmentParam( const String &name, f32 value )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();
        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            //Ogre::Technique *tech = m_material->getBestTechnique();
            //if( tech )
            //{
            //    Ogre::Pass *pass = tech->getPass( 0 );
            //    if( pass )
            //    {
            //        if( pass->hasFragmentProgram() )
            //        {
            //            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
            //                pass->getFragmentProgramParameters();
            //            const Ogre::GpuNamedConstants &fragmentNamedConstants =
            //                fragmentParameters->getConstantDefinitions();
            //            fragmentParameters->setNamedConstant( name.c_str(), value );
            //        }
            //    }
            //}
        }
        else
        {
            auto message = factoryManager->make_ptr<StateMessageFragmentParam>();
            message->setType( FRAGMENT_FLOAT_HASH );
            message->setName( name );
            message->setFloat( value );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
    }

    void Material::setFragmentParam( const String &name, const Vector2F &value )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();
        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            //Ogre::Technique *tech = m_material->getBestTechnique();
            //if( tech )
            //{
            //    Ogre::Pass *pass = tech->getPass( 0 );
            //    if( pass )
            //    {
            //        if( pass->hasFragmentProgram() )
            //        {
            //            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
            //                pass->getFragmentProgramParameters();
            //            const Ogre::GpuNamedConstants &fragmentNamedConstants =
            //                fragmentParameters->getConstantDefinitions();
            //            // fragmentParameters->setNamedConstant(name.c_str(),
            //            // Ogre::Vector2(value.X(), value.Y()));
            //        }
            //    }
            //}
        }
        else
        {
            SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
            message->setType( FRAGMENT_VECTOR2F_HASH );
            message->setName( name );
            message->setVector2f( value );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
    }

    void Material::setFragmentParam( const String &name, const Vector3F &value )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();
        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            //Ogre::Technique *tech = m_material->getBestTechnique();
            //if( tech )
            //{
            //    Ogre::Pass *pass = tech->getPass( 0 );
            //    if( pass )
            //    {
            //        if( pass->hasFragmentProgram() )
            //        {
            //            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
            //                pass->getFragmentProgramParameters();
            //            const Ogre::GpuNamedConstants &fragmentNamedConstants =
            //                fragmentParameters->getConstantDefinitions();
            //            fragmentParameters->setNamedConstant(
            //                name.c_str(), Ogre::Vector3( value.X(), value.Y(), value.Z() ) );
            //        }
            //    }
            //}
        }
        else
        {
            SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
            message->setType( FRAGMENT_VECTOR3F_HASH );
            message->setName( name );
            message->setVector3f( value );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
    }

    void Material::setFragmentParam( const String &name, const Vector4F &value )
    {
        auto applicationManager = core::ApplicationManager::instance();

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();
        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            //Ogre::Technique *tech = m_material->getBestTechnique();
            //if( tech )
            //{
            //    Ogre::Pass *pass = tech->getPass( 0 );
            //    if( pass )
            //    {
            //        if( pass->hasFragmentProgram() )
            //        {
            //            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
            //                pass->getFragmentProgramParameters();
            //            const Ogre::GpuNamedConstants &fragmentNamedConstants =
            //                fragmentParameters->getConstantDefinitions();
            //            fragmentParameters->setNamedConstant(
            //                name.c_str(),
            //                Ogre::Vector4( value.X(), value.Y(), value.Z(), value.W() ) );
            //        }
            //    }
            //}
        }
        else
        {
            SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
            message->setType( FRAGMENT_VECTOR4F_HASH );
            message->setName( name );
            message->setVector4f( value );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
    }

    void Material::setFragmentParam( const String &name, const ColourF &value )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto renderTask = graphicsSystem->getRenderTask();
        auto task = Thread::getCurrentTask();

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded && task == renderTask )
        {
            //Ogre::Technique *tech = m_material->getBestTechnique();
            //if( tech )
            //{
            //    Ogre::Pass *pass = tech->getPass( 0 );
            //    if( pass )
            //    {
            //        if( pass->hasFragmentProgram() )
            //        {
            //            Ogre::GpuProgramParametersSharedPtr fragmentParameters =
            //                pass->getFragmentProgramParameters();
            //            const Ogre::GpuNamedConstants &fragmentNamedConstants =
            //                fragmentParameters->getConstantDefinitions();
            //            fragmentParameters->setNamedConstant(
            //                name.c_str(), Ogre::ColourValue( value.r, value.g, value.b, value.a ) );
            //        }
            //    }
            //}
        }
        else
        {
            SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
            message->setType( FRAGMENT_COLOUR_HASH );
            message->setName( name );
            message->setColourf( value );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
    }

    auto Material::getName() const -> String
    {
        return m_name;
    }

    void Material::setName( const String &name )
    {
        m_name = name;
    }

    auto Material::getId() const -> hash32
    {
        return m_id;
    }

    auto Material::createTechnique() -> SmartPtr<IMaterialTechnique>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto technique = factoryManager->make_object<IMaterialTechnique>();
            technique->setMaterial( this );

            auto p = getTechniquesPtr();
            if( !p )
            {
                p = fb::make_shared<Array<SmartPtr<IMaterialTechnique>>>();
                setTechniquesPtr( p );
            }

            if( p )
            {
                auto &techniques = *p;
                //FB_ASSERT( techniques.size() == 0 );
                techniques.push_back( technique );
            }

            return technique;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void Material::removeTechnique( SmartPtr<IMaterialTechnique> technique )
    {
        auto techniques = getTechniques();
        auto it = std::find( techniques.begin(), techniques.end(), technique );
        if( it != techniques.end() )
        {
            techniques.erase( it );
        }

        auto p = fb::make_shared<Array<SmartPtr<IMaterialTechnique>>>();
        *p = techniques;
        setTechniquesPtr( p );
    }

    void Material::setTechniques( Array<SmartPtr<IMaterialTechnique>> techniques )
    {
        auto p = fb::make_shared<Array<SmartPtr<IMaterialTechnique>>>();
        *p = techniques;
        setTechniquesPtr( p );
    }

    void Material::removeAllTechniques()
    {
        setTechniquesPtr( nullptr );
    }

    auto Material::getTechniques() const -> Array<SmartPtr<IMaterialTechnique>>
    {
        if( auto p = getTechniquesPtr() )
        {
            auto &techniques = *p;
            return Array<SmartPtr<IMaterialTechnique>>( techniques.begin(), techniques.end() );
        }

        return {};
    }

    auto Material::getTechniqueByScheme( hash32 scheme ) const -> SmartPtr<IMaterialTechnique>
    {
        return m_technique;
    }

    auto Material::getNumTechniques() const -> u32
    {
        if( const auto p = getTechniquesPtr() )
        {
            const auto &techniques = *p;
            return static_cast<u32>( techniques.size() );
        }

        return 0;
    }

    void Material::setScale( const Vector3F &scale, u32 textureIndex /*= 0*/, u32 passIndex /*= 0*/,
                             u32 techniqueIndex /*= 0 */ )
    {
        //if( m_material )
        //{
        //    Ogre::Matrix4 mat = Ogre::Matrix4::IDENTITY;
        //    mat.setScale( Ogre::Vector3( scale.X(), scale.Y(), scale.Z() ) );
        //    m_material->getTechnique( techniqueIndex )
        //        ->getPass( passIndex )
        //        ->getTextureUnitState( textureIndex )
        //        ->setTextureTransform( mat );
        //}
    }

    auto Material::getRoot() const -> SmartPtr<IMaterialNode>
    {
        return m_root;
    }

    void Material::setRoot( SmartPtr<IMaterialNode> root )
    {
        m_root = root;
    }

    auto Material::getRendererType() const -> hash32
    {
        return m_rendererType;
    }

    void Material::setRendererType( hash32 rendererType )
    {
        m_rendererType = rendererType;
    }

    void Material::createMaterialByType()
    {
        try
        {
            auto techniques = getTechniques();
            for( auto t : techniques )
            {
                t->setMaterial( this );

                auto passes = t->getPasses();
                for( auto p : passes )
                {
                    if( !p->isLoaded() )
                    {
                        p->setMaterial( this );
                        p->load( nullptr );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Material::addTechnique( SmartPtr<IMaterialTechnique> technique )
    {
        auto p = getTechniquesPtr();
        if( !p )
        {
            p = fb::make_shared<Array<SmartPtr<IMaterialTechnique>>>();
            setTechniquesPtr( p );
        }

        if( p )
        {
            auto &techniques = *p;
            techniques.push_back( technique );
        }
    }

    auto Material::getTechniquesPtr() const -> SharedPtr<Array<SmartPtr<IMaterialTechnique>>>
    {
        return m_techniques;
    }

    void Material::setTechniquesPtr( SharedPtr<Array<SmartPtr<IMaterialTechnique>>> p )
    {
        m_techniques = p;
    }

    auto Material::getMaterialType() const -> IMaterial::MaterialType
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialState>() )
            {
                return state->getMaterialType();
            }
        }

        return MaterialType::Standard;
    }

    void Material::setMaterialType( MaterialType materialType )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialState>() )
            {
                state->setMaterialType( materialType );
            }
        }
    }

    void Material::setDirty( bool dirty )
    {
        // if (m_isDirty != dirty)
        //{
        //	m_isDirty = dirty;

        //	auto applicationManager = core::ApplicationManager::instance();
        //	FB_ASSERT(applicationManager);

        //	auto factoryManager = applicationManager->getFactoryManager();
        //	auto message = factoryManager->make_ptr<StateMessageDirty>();
        //	message->setDirty(dirty);

        //	auto graphicsSystem = applicationManager->getGraphicsSystem();
        //	auto stateTask = graphicsSystem->getStateTask();
        //	m_stateContext->addMessage(stateTask, message);
        //}
    }

    auto Material::isDirty() const -> bool
    {
        return false;
    }

    //
    // SmartPtr<render::IMaterialTechnique> CMaterial::createTechnique()
    //{
    //	auto applicationManager = core::ApplicationManager::instance();
    //	FB_ASSERT(applicationManager);

    //	auto factoryManager = applicationManager->getFactoryManager();
    //	auto technique = factoryManager->make_ptr<CMaterialTechnique>();
    //	technique->setOwner(this);
    //	m_techniques.push_back(technique);
    //	return technique;
    //}

    auto Material::toData() const -> SmartPtr<ISharedObject>
    {
        try
        {
            auto properties = fb::make_ptr<Properties>();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            const auto iMaterialType = static_cast<s32>( getMaterialType() );
            properties->setProperty( "materialType", iMaterialType );

            auto texturesProperties = fb::make_ptr<Properties>();
            texturesProperties->setName( "textures" );

            if( auto p = getTextures() )
            {
                auto &textures = *p;
                for( auto texture : textures )
                {
                    if( texture )
                    {
                        auto textureData = fb::static_pointer_cast<Properties>( texture->toData() );
                        if( textureData )
                        {
                            textureData->setName( "texture" );
                            texturesProperties->addChild( textureData );
                        }
                    }
                    else
                    {
                        auto textureData = fb::make_ptr<Properties>();
                        if( textureData )
                        {
                            textureData->setName( "texture" );
                            texturesProperties->addChild( textureData );
                        }
                    }
                }
            }

            properties->addChild( texturesProperties );

            auto techniques = getTechniques();
            for( auto technique : techniques )
            {
                auto pTechniqueData = fb::static_pointer_cast<Properties>( technique->toData() );
                pTechniqueData->setName( "schemes" );
                properties->addChild( pTechniqueData );
            }

            return properties;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void Material::fromData( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( data && data->isDerived<Properties>() )
            {
                auto properties = fb::static_pointer_cast<Properties>( data );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();

                auto iMaterialType = 0;
                properties->getPropertyValue( "materialType", iMaterialType );

                auto materialType = static_cast<MaterialType>( iMaterialType );
                FB_ASSERT( materialType < IMaterial::MaterialType::Count );
                setMaterialType( materialType );

                createMaterialByType();

                auto textures = Array<SmartPtr<ITexture>>();
                textures.resize( m_textures->capacity() );

                auto textureCount = 0;
                if( auto texturesChild = properties->getChild( "textures" ) )
                {
                    auto textureChildren = texturesChild->getChildrenByName( "texture" );
                    for( auto textureChild : textureChildren )
                    {
                        auto uuid = textureChild->getProperty( "uuid" );
                        if( !StringUtil::isNullOrEmpty( uuid ) )
                        {
                            auto tex = resourceDatabase->loadResourceById( uuid );
                            textures[textureCount++] = tex;
                        }
                    }
                }

                auto count = 0;

                auto techniques = getTechniques();

                auto schemes = properties->getChildrenByName( "schemes" );
                for( auto &scheme : schemes )
                {
                    auto technique = SmartPtr<IMaterialTechnique>();

                    if( count < techniques.size() )
                    {
                        technique = techniques[count];
                    }
                    else
                    {
                        technique = createTechnique();
                    }

                    technique->fromData( scheme );

                    count++;
                }

                auto layerIdx = 0;
                for( auto texture : textures )
                {
                    if( texture )
                    {
                        setTexture( texture, layerIdx );
                    }

                    layerIdx++;
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Material::getProperties() const -> SmartPtr<Properties>
    {
        try
        {
            auto properties = ResourceGraphics<IMaterial>::getProperties();

            static const auto materialTypeStr = String( "Material Type" );

            auto materialType = getMaterialType();
            auto materialTypeValueStr = GraphicsUtil::getMaterialType( materialType );
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

    void Material::setProperties( SmartPtr<Properties> properties )
    {
        try
        {
            ResourceGraphics<IMaterial>::setProperties( properties );

            auto materialTypeStr = String( "" );
            properties->getPropertyValue( "Material Type", materialTypeStr );
            materialTypeStr = StringUtil::trim( materialTypeStr );

            auto enumValues = GraphicsUtil::getMaterialTypes();

            auto it = std::find( enumValues.begin(), enumValues.end(), materialTypeStr );
            if( it != enumValues.end() )
            {
                auto pos = std::distance( enumValues.begin(), it );

                auto materialType = static_cast<MaterialType>( pos );

                auto currentMaterialType = getMaterialType();
                if( currentMaterialType != materialType )
                {
                    setMaterialType( materialType );
                    createMaterialByType();
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Material::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto techniques = getTechniques();

        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( techniques.size() );

        for( auto technique : techniques )
        {
            objects.emplace_back( technique );
        }

        return objects;
    }

    void Material::MaterialStateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
        auto messageType = message->getType();

        if( message->isExactly<StateMessageIntValue>() )
        {
            auto intMessage = fb::static_pointer_cast<StateMessageIntValue>( message );
            auto value = intMessage->getValue();

            if( messageType == LIGHTING_ENABLED_HASH )
            {
                m_owner->setLightingEnabled( value );
            }
        }
        else if( message->isExactly<StateMessageUIntValue>() )
        {
            auto intMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );

            if( intMessage->getType() == StringUtil::getHash( "materialType" ) )
            {
                m_owner->setMaterialType(
                    static_cast<IMaterial::MaterialType>( intMessage->getValue() ) );
            }
        }
        else if( message->isExactly<StateMessageLoad>() )
        {
            auto loadMessage = fb::static_pointer_cast<StateMessageLoad>( message );
            if( loadMessage->getType() == StateMessageLoad::LOAD_HASH )
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
            auto type = fragmentMessage->getType();

            if( type == FRAGMENT_FLOAT_HASH )
            {
                m_owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getFloat() );
            }
            else if( type == FRAGMENT_VECTOR2F_HASH )
            {
                m_owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getVector2f() );
            }
            else if( type == FRAGMENT_VECTOR3F_HASH )
            {
                m_owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getVector3f() );
            }
            else if( type == FRAGMENT_VECTOR4F_HASH )
            {
                m_owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getVector4f() );
            }
            else if( type == FRAGMENT_COLOUR_HASH )
            {
                m_owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getColourf() );
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
            auto type = arrayMessage->getType();
            auto value = arrayMessage->getObjects();

            if( type == StateMessage::SET_CUBEMAP )
            {
                m_owner->setCubicTexture( Array<SmartPtr<ITexture>>( value.begin(), value.end() ) );
            }
        }
    }

    void Material::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void Material::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto Material::MaterialStateListener::getOwner() const -> Material *
    {
        return m_owner;
    }

    void Material::MaterialStateListener::setOwner( Material *owner )
    {
        m_owner = owner;
    }

    Material::MaterialStateListener::~MaterialStateListener() = default;

    Material::MaterialStateListener::MaterialStateListener() = default;

    Material::MaterialStateListener::MaterialStateListener( Material *material ) : m_owner( material )
    {
    }
}  // namespace fb::render
