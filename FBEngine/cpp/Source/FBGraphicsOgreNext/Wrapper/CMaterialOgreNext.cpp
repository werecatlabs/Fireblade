#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTechniqueOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialPassOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>
#include <Ogre.h>
#include <OgreImage2.h>
#include <OgreTextureGpuManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreRoot.h>
#include <OgreResourceGroupManager.h>
#include <OgreGpuProgramParams.h>
#include <OgreHlmsPbsDatablock.h>
#include <OgreHlms.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreHlmsManager.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CMaterialOgreNext, Material );
    FB_CLASS_REGISTER_DERIVED( fb, CMaterialOgreNext::MaterialStateListener, IStateListener );

    u32 CMaterialOgreNext::m_nameExt = 0;

    CMaterialOgreNext::CMaterialOgreNext()
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

        auto stateContext = getStateContext();
        //setStateContext( stateContext );

        //auto stateListener = factoryManager->make_ptr<MaterialStateListener>();
        //stateListener->setOwner( this );
        //setStateListener( stateListener );
        //stateContext->addStateListener( stateListener );

        auto stateListener = factoryManager->make_ptr<MaterialStateListener>();
        stateListener->setOwner( this );
        m_stateListener = stateListener;
        stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<MaterialState>();
        stateContext->addState( state );
        stateContext->setOwner( this );

        auto renderTask = graphicsSystem->getStateTask();
        state->setTaskId( renderTask );
    }

    CMaterialOgreNext::~CMaterialOgreNext()
    {
        unload( nullptr );
    }

    void CMaterialOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( isLoaded() == false );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
            FB_ASSERT( resourceGroupManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            ScopedLock lock( graphicsSystem );

            // FB_ASSERT(getLoadingState() == LoadingState::Unloaded);
            setLoadingState( LoadingState::Loading );

            Material::load( data );

            createMaterialByType();

            if( auto p = getTechniquesPtr() )
            {
                auto &techniques = *p;
                for( auto technique : techniques )
                {
                    technique->load( nullptr );
                }
            }

            if( auto stateContext = getStateContext() )
            {
                stateContext->setDirty( true );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialOgreNext::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto renderTask = graphicsSystem->getRenderTask();
            auto stateTask = graphicsSystem->getStateTask();
            auto task = Thread::getCurrentTask();

            const auto &loadingState = getLoadingState();
            if( task == renderTask )
            {
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

                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( auto technique : techniques )
                    {
                        technique->reload( data );
                    }
                }

                auto message = factoryManager->make_ptr<StateMessageLoad>();
                message->setType( StateMessageLoad::LOADED_HASH );
                message->setObject( this );

                if( auto stateContext = getStateContext() )
                {
                    stateContext->addMessage( stateTask, message );
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageLoad>();
                message->setType( StateMessageLoad::RELOAD_HASH );

                if( auto stateContext = getStateContext() )
                {
                    stateContext->addMessage( stateTask, message );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialOgreNext::unload( SmartPtr<ISharedObject> data )
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

                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( auto technique : techniques )
                    {
                        technique->unload( data );
                    }

                    techniques.clear();
                }

                if( auto stateContext = getStateContext() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    if( stateManager )
                    {
                        stateManager->removeStateObject( m_stateContext );
                    }

                    stateContext->unload( nullptr );
                    setStateContext( nullptr );
                }

                if( auto stateListener = getStateListener() )
                {
                    stateListener->unload( nullptr );
                    setStateListener( nullptr );
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialOgreNext::setCubicTexture( const String &fileName, bool uvw, u32 layerIdx )
    {
        if( Thread::getCurrentTask() != Thread::Task::Render )
        {
            SmartPtr<StateMessageSetTexture> message( new StateMessageSetTexture );
            message->setType( SET_TEXTURE_HASH );
            message->setTextureName( fileName );
            message->setTextureIndex( layerIdx );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
        else
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
    }

    void CMaterialOgreNext::setCubicTexture( Array<SmartPtr<ITexture>> textures, u32 layerIdx )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto textureManager = graphicsSystem->getTextureManager();

        ScopedLock lock( graphicsSystem );

        std::vector<String> textureNames;
        textureNames.resize( 6 );

        Array<u32> map;
        map.reserve( 6 );

        map.push_back( static_cast<u32>( IMaterial::SkyboxTextureTypes::Right ) );
        map.push_back( static_cast<u32>( IMaterial::SkyboxTextureTypes::Left ) );
        map.push_back( static_cast<u32>( IMaterial::SkyboxTextureTypes::Up ) );
        map.push_back( static_cast<u32>( IMaterial::SkyboxTextureTypes::Down ) );
        map.push_back( static_cast<u32>( IMaterial::SkyboxTextureTypes::Front ) );
        map.push_back( static_cast<u32>( IMaterial::SkyboxTextureTypes::Back ) );

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

        m_cubeTexture = textureManager->createSkyBoxCubeMap( textureNames );
    }

    void CMaterialOgreNext::setFragmentParam( const String &name, f32 value )
    {
        if( Thread::getCurrentTask() != Thread::Task::Render )
        {
            SmartPtr<StateMessageFragmentParam> message( new StateMessageFragmentParam );
            message->setType( FRAGMENT_FLOAT_HASH );
            message->setName( name );
            message->setFloat( value );

            if( auto stateContext = getStateContext() )
            {
                stateContext->addMessage( Thread::Task::Render, message );
            }
        }
        else
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
    }

    void CMaterialOgreNext::setFragmentParam( const String &name, const Vector2F &value )
    {
        if( Thread::getCurrentTask() != Thread::Task::Render )
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
        else
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
    }

    void CMaterialOgreNext::setFragmentParam( const String &name, const Vector3F &value )
    {
        if( Thread::getCurrentTask() != Thread::Task::Render )
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
        else
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
    }

    void CMaterialOgreNext::setFragmentParam( const String &name, const Vector4F &value )
    {
        if( Thread::getCurrentTask() != Thread::Task::Render )
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
        else
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
                            name.c_str(), Ogre::Vector4( value.X(), value.Y(), value.Z(), value.W() ) );
                    }
                }
            }
        }
    }

    void CMaterialOgreNext::setFragmentParam( const String &name, const ColourF &value )
    {
        if( Thread::getCurrentTask() != Thread::Task::Render )
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
        else
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
    }

    void CMaterialOgreNext::setScale( const Vector3F &scale, u32 textureIndex /*= 0*/,
                                      u32 passIndex /*= 0*/, u32 techniqueIndex /*= 0 */ )
    {
        if( m_material )
        {
            Ogre::Matrix4 mat = Ogre::Matrix4::IDENTITY;
            mat.setScale( Ogre::Vector3( scale.X(), scale.Y(), scale.Z() ) );
            auto t = m_material->getTechnique( techniqueIndex );
            auto pass = t->getPass( passIndex );
            auto tex = pass->getTextureUnitState( textureIndex );
            tex->setTextureTransform( mat );
        }
    }

    void CMaterialOgreNext::createMaterialByType()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ScopedLock lock( graphicsSystem );

            auto root = Ogre::Root::getSingletonPtr();
            auto hlmsManager = root->getHlmsManager();

            switch( auto materialType = getMaterialType() )
            {
            case MaterialType::Standard:
            {
                auto hlmsPbs = static_cast<Ogre::HlmsPbs *>( hlmsManager->getHlms( Ogre::HLMS_PBS ) );
                if( hlmsPbs )
                {
                    auto datablockName = StringUtil::getUUID();

                    Ogre::HlmsMacroblock macroblock;
                    Ogre::HlmsBlendblock blendblock;

                    auto pDataBlock = hlmsPbs->createDatablock( datablockName, datablockName, macroblock,
                                                                blendblock, Ogre::HlmsParamVec() );
                    auto datablock = static_cast<Ogre::HlmsPbsDatablock *>( pDataBlock );

                    datablock->setReceiveShadows( true );

                    setDatablockName( datablockName );
                    setHlmsDatablock( datablock );
                }
            }
            break;
            case MaterialType::StandardSpecular:
            {
                auto hlmsPbs = static_cast<Ogre::HlmsPbs *>( hlmsManager->getHlms( Ogre::HLMS_PBS ) );
                if( hlmsPbs )
                {
                    auto datablockName = StringUtil::getUUID();
                    auto datablock = static_cast<Ogre::HlmsPbsDatablock *>(
                        hlmsPbs->createDatablock( datablockName, datablockName, Ogre::HlmsMacroblock(),
                                                  Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );

                    setDatablockName( datablockName );
                    setHlmsDatablock( datablock );
                }
            }
            break;
            case MaterialType::StandardTriPlanar:
            {
            }
            break;
            case MaterialType::TerrainStandard:
            {
            }
            break;
            case MaterialType::TerrainSpecular:
            {
            }
            break;
            case MaterialType::TerrainDiffuse:
            {
            }
            break;
            case MaterialType::Skybox:
            {
                /*for( auto t : m_techniques )
                {
                    auto technique = fb::static_pointer_cast<CMaterialTechniqueOgreNext>( t );

                    auto pTechnique = technique->getTechnique();
                    auto materialPasses = pTechnique->getPasses();
                    auto passes = t->getPasses();

                    auto passCount = 0;
                    for( auto p : passes )
                    {
                        auto pass = fb::static_pointer_cast<CMaterialPassOgreNext>( p );

                        auto pPass = materialPasses[passCount];
                        pass->setPass( pPass );
                        passCount++;

                        if( passCount == materialPasses.size() )
                        {
                            break;
                        }
                    }

                    for( auto p : passes )
                    {
                        auto numTextureUnits = p->getNumTexturesNodes();

                        for( size_t i = numTextureUnits > 0 ? numTextureUnits - 1 : 0;
                             i < static_cast<size_t>( 6 ); ++i )
                        {
                            p->createTextureUnit();
                        }
                    }
                }*/
            }
            break;
            case MaterialType::SkyboxCubemap:
            {
                if( auto p = getTechniquesPtr() )
                {
                    auto &techniques = *p;
                    for( auto t : techniques )
                    {
                        auto technique = fb::static_pointer_cast<CMaterialTechniqueOgreNext>( t );

                        if( auto pTechnique = technique->getTechnique() )
                        {
                            auto materialPasses = Array<Ogre::Pass *>();
                            auto passIt = pTechnique->getPassIterator();
                            while( passIt.peekNext() )
                            {
                                auto p = *passIt.current();
                                materialPasses.push_back( p );
                                passIt.moveNext();
                            }

                            auto passes = t->getPasses();

                            auto passCount = 0;
                            for( auto p : passes )
                            {
                                auto pass = fb::static_pointer_cast<CMaterialPassOgreNext>( p );

                                auto pPass = materialPasses[passCount];
                                pass->setPass( pPass );
                                passCount++;

                                //pPass->setLightingEnabled( false );
                                //pPass->setDepthWriteEnabled( false );

                                if( passCount == materialPasses.size() )
                                {
                                    break;
                                }
                            }

                            for( auto p : passes )
                            {
                                auto numTextureUnits = p->getNumTexturesNodes();

                                if( numTextureUnits != 1 )
                                {
                                    auto pPass = fb::static_pointer_cast<CMaterialPassOgreNext>( p );
                                    pPass->reload( nullptr );

                                    auto tu = pPass->getPass()->createTextureUnitState();

                                    auto textureUnit = p->createTextureUnit();
                                    auto pTextureUnit =
                                        fb::static_pointer_cast<CMaterialTextureOgreNext>( textureUnit );
                                    pTextureUnit->setTextureUnitState( tu );
                                    textureUnit->load( nullptr );
                                }
                            }
                        }
                    }
                }
            }
            break;
            case MaterialType::UI:
            {
                auto root = Ogre::Root::getSingletonPtr();
                auto hlmsManager = root->getHlmsManager();

                auto hlmsPbs = hlmsManager->getHlms( Ogre::HLMS_UNLIT );
                if( hlmsPbs )
                {
                    Ogre::HlmsMacroblock macroblock;
                    macroblock.mDepthCheck = false;

                    Ogre::HlmsBlendblock blendblock;
                    Ogre::HlmsParamVec paramVec;

                    auto datablockName = StringUtil::getUUID();
                    auto datablock = hlmsPbs->createDatablock( datablockName, datablockName, macroblock,
                                                               blendblock, paramVec );

                    setDatablockName( datablockName );
                    setHlmsDatablock( datablock );
                }
            }
            break;
            case MaterialType::Custom:
            {
            }
            break;
            default:
            {
            }
            break;
            }

            auto techniques = getTechniques();
            if( techniques.empty() )
            {
                auto technique = createTechnique();
                if( technique )
                {
                    technique->load( nullptr );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CMaterialOgreNext::createTechnique() -> SmartPtr<IMaterialTechnique>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto technique = factoryManager->make_ptr<CMaterialTechniqueOgreNext>();
        technique->setMaterial( this );
        addTechnique( technique );
        return technique;
    }

    auto CMaterialOgreNext::getHlmsDatablock() const -> Ogre::HlmsDatablock *
    {
        return m_hlmsDatablock;
    }

    void CMaterialOgreNext::setHlmsDatablock( Ogre::HlmsDatablock *hlmsDatablock )
    {
        m_hlmsDatablock = hlmsDatablock;
    }

    auto CMaterialOgreNext::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
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

    auto CMaterialOgreNext::getDatablockName() const -> String
    {
        return m_datablockName;
    }

    void CMaterialOgreNext::setDatablockName( const String &datablockName )
    {
        m_datablockName = datablockName;
    }

    auto CMaterialOgreNext::getCubeTexture() const -> SmartPtr<ITexture>
    {
        return m_cubeTexture;
    }

    void CMaterialOgreNext::setCubeTexture( SmartPtr<ITexture> cubeTexture )
    {
        m_cubeTexture = cubeTexture;
    }

    void CMaterialOgreNext::MaterialStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        if( auto owner = getOwner() )
        {
            if( message->isExactly<StateMessageUIntValue>() )
            {
                auto valueMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );

                auto messageType = owner->getMaterialType();
                owner->setMaterialType( messageType );
            }
            else if( message->isExactly<StateMessageLoad>() )
            {
                auto loadMessage = fb::static_pointer_cast<StateMessageLoad>( message );
                auto messageType = loadMessage->getType();

                if( messageType == StateMessageLoad::LOAD_HASH )
                {
                    owner->load( nullptr );
                }
                else if( messageType == StateMessageLoad::RELOAD_HASH )
                {
                    owner->reload( nullptr );
                }
            }
            else if( message->isExactly<StateMessageFragmentParam>() )
            {
                auto fragmentMessage = fb::static_pointer_cast<StateMessageFragmentParam>( message );
                auto type = fragmentMessage->getType();

                if( type == FRAGMENT_FLOAT_HASH )
                {
                    owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getFloat() );
                }
                else if( type == FRAGMENT_VECTOR2F_HASH )
                {
                    owner->setFragmentParam( fragmentMessage->getName(),
                                             fragmentMessage->getVector2f() );
                }
                else if( type == FRAGMENT_VECTOR3F_HASH )
                {
                    owner->setFragmentParam( fragmentMessage->getName(),
                                             fragmentMessage->getVector3f() );
                }
                else if( type == FRAGMENT_VECTOR4F_HASH )
                {
                    owner->setFragmentParam( fragmentMessage->getName(),
                                             fragmentMessage->getVector4f() );
                }
                else if( type == FRAGMENT_COLOUR_HASH )
                {
                    owner->setFragmentParam( fragmentMessage->getName(), fragmentMessage->getColourf() );
                }
            }
            else if( message->isExactly<StateMessageSetTexture>() )
            {
                auto textureMessage = fb::static_pointer_cast<StateMessageSetTexture>( message );
                if( auto texture = textureMessage->getTexture() )
                {
                    const auto textureIndex = textureMessage->getTextureIndex();
                    owner->setTexture( texture, textureIndex );
                }
                else
                {
                    const auto textureName = textureMessage->getTextureName();
                    const auto textureIndex = textureMessage->getTextureIndex();
                    owner->setTexture( textureName, textureIndex );
                }
            }
            else if( message->isExactly<StateMessageObjectsArray>() )
            {
                auto arrayMessage = fb::static_pointer_cast<StateMessageObjectsArray>( message );
                auto type = arrayMessage->getType();
                auto value = arrayMessage->getObjects();

                if( type == StateMessage::SET_CUBEMAP )
                {
                    owner->setCubicTexture( Array<SmartPtr<ITexture>>( value.begin(), value.end() ) );
                }
            }
        }
    }

    void CMaterialOgreNext::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        if( auto owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
                FB_ASSERT( resourceGroupManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                ScopedLock lock( graphicsSystem );

                auto techniques = owner->getTechniques();
                for( auto technique : techniques )
                {
                    auto passes = technique->getPasses();
                    for( auto pass : passes )
                    {
                        auto ogreNextPass = fb::static_pointer_cast<CMaterialPassOgreNext>( pass );
                        ogreNextPass->setupMaterial();
                    }
                }

                state->setDirty( false );
            }
        }
    }

    void CMaterialOgreNext::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto CMaterialOgreNext::MaterialStateListener::getOwner() const -> SmartPtr<CMaterialOgreNext>
    {
        return m_owner;
    }

    void CMaterialOgreNext::MaterialStateListener::setOwner( SmartPtr<CMaterialOgreNext> owner )
    {
        m_owner = owner;
    }

    CMaterialOgreNext::MaterialStateListener::~MaterialStateListener() = default;

    CMaterialOgreNext::MaterialStateListener::MaterialStateListener() = default;

    CMaterialOgreNext::MaterialStateListener::MaterialStateListener( CMaterialOgreNext *material ) :
        m_owner( material )
    {
    }
}  // namespace fb::render
