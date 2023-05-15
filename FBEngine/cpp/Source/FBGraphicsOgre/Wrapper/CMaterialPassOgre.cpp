#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CMaterialPassOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTextureOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTechniqueOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialOgre.h>
#include <FBGraphicsOgre/Ogre/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialPassOgre, CMaterialPass );

        const hash_type CMaterialPassOgre::DIFFUSE_HASH = StringUtil::getHash( "diffuse" );
        const hash_type CMaterialPassOgre::EMISSION_HASH = StringUtil::getHash( "emmision" );

        CMaterialPassOgre::CMaterialPassOgre()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            auto stateContext = stateManager->addStateObject();
            setStateObject( stateContext );

            auto stateListener = factoryManager->make_ptr<MaterialPassOgreStateListener>();
            stateListener->setOwner( this );
            setStateListener( stateListener );
            stateContext->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<MaterialPassState>();
            stateContext->setState( state );
        }

        CMaterialPassOgre::~CMaterialPassOgre()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            if( auto stateContext = getStateObject() )
            {
                if( auto stateListener = getStateListener() )
                {
                    stateContext->removeStateListener( stateListener );
                }

                stateManager->removeStateObject( stateContext );

                stateContext->unload( nullptr );
                setStateObject( nullptr );
            }

            if( auto stateListener = getStateListener() )
            {
                stateListener->unload( nullptr );
                setStateListener( nullptr );
            }
        }

        void CMaterialPassOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto pMaterial = getMaterial();
                FB_ASSERT( pMaterial );

                auto material = fb::static_pointer_cast<CMaterialOgre>( pMaterial );

                auto pParent = getParent();
                FB_ASSERT( pParent );

                auto parent = fb::static_pointer_cast<CMaterialTechniqueOgre>( pParent );

                auto ogreMaterial = material->getMaterial();
                auto technique = parent->getTechnique();

                if( !m_pass )
                {
                    auto pass = technique->createPass();
                    setPass( pass );
                }

                createTextureSlots();

                //FB_ASSERT( getTexturesPtr() && !getTexturesPtr()->empty() );

                setupMaterial();

                FB_ASSERT( m_pass );

                auto ogreTextureUnits = m_pass->getTextureUnitStates();

                if( auto p = getTexturesPtr() )
                {
                    auto &textures = *p;

                    auto count = 0;
                    for( auto &texture : textures )
                    {
                        auto pTexture = fb::static_pointer_cast<CMaterialTextureOgre>( texture );

                        if( count < ogreTextureUnits.size() )
                        {
                            pTexture->setTextureUnitState( ogreTextureUnits[count] );
                        }

                        count++;
                    }
                }

                if( auto p = getTexturesPtr() )
                {
                    auto &textures = *p;
                    for( auto t : textures )
                    {
                        t->load( nullptr );
                    }

                    for( auto t : textures )
                    {
                        if( auto stateContext = t->getStateObject() )
                        {
                            stateContext->setDirty( true );
                        }
                    }
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialPassOgre::createTextureSlots()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            if( auto parent = getParent() )
            {
                auto technique = fb::static_pointer_cast<CMaterialTechniqueOgre>( parent );
                if( technique )
                {
                    auto owner = technique->getMaterial();
                    auto material = fb::static_pointer_cast<CMaterialOgre>( owner );
                    if( material )
                    {
                        switch( auto materialType = material->getMaterialType() )
                        {
                        case IMaterial::MaterialType::Standard:
                        {
                            auto numSlots = static_cast<size_t>(
                                IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        auto pTexture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        pTexture->setParent( this );
                                        pTexture->setEnabled( i == 0 );  // todo temp while no PBR
                                        texture = pTexture;

                                        auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getPbsTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::StandardSpecular:
                        {
                            auto numSlots = static_cast<size_t>(
                                IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );
                                        texture->setEnabled( i == 0 );  // todo temp while no PBR

                                        auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getPbsTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::StandardTriPlanar:
                        {
                            auto numSlots = static_cast<size_t>(
                                IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );

                                        auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getPbsTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::TerrainStandard:
                        {
                            auto numSlots = static_cast<size_t>( IMaterial::TerrainTextureTypes::Count );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );

                                        auto textureType =
                                            static_cast<IMaterial::TerrainTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getTerrainTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::TerrainSpecular:
                        {
                            auto numSlots = static_cast<size_t>( IMaterial::TerrainTextureTypes::Count );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );

                                        auto textureType =
                                            static_cast<IMaterial::TerrainTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getTerrainTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::TerrainDiffuse:
                        {
                            auto numSlots = static_cast<size_t>( IMaterial::TerrainTextureTypes::Count );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );

                                        auto textureType =
                                            static_cast<IMaterial::TerrainTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getTerrainTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::Skybox:
                        {
                            auto numSlots = static_cast<size_t>( IMaterial::SkyboxTextureTypes::Count );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );

                                        auto textureType =
                                            static_cast<IMaterial::SkyboxTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getSkyboxTextureType( textureType );

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::SkyboxCubemap:
                        {
                            auto numSlots =
                                static_cast<size_t>( IMaterial::SkyboxCubeTextureTypes::Count );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        auto pTexture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        pTexture->setParent( this );

                                        texture = pTexture;

                                        auto textureType =
                                            static_cast<IMaterial::SkyboxCubeTextureTypes>( i );
                                        auto textureTypeStr =
                                            GraphicsUtil::getSkyboxCubeTextureType( textureType );

                                        texture->setTextureType( (u32)i );
                                        texture->setMaterial( material );

                                        if( auto textureHandle = texture->getHandle() )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::UI:
                        {
                            auto numSlots = static_cast<size_t>( 1 );
                            if( numSlots != getNumTexturesNodes() )
                            {
                                auto textures = Array<SmartPtr<IMaterialTexture>>();
                                textures.resize( numSlots );

                                for( size_t i = 0; i < textures.size(); ++i )
                                {
                                    auto &texture = textures[i];
                                    if( !texture )
                                    {
                                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                                        texture->setParent( this );

                                        // auto textureType = (IMaterial::PbsTextureTypes)i;
                                        auto textureTypeStr = String(
                                            "Base" );  // GraphicsUtil::getPbsTextureType(textureType);

                                        texture->setTextureType( (u32)i );

                                        texture->setMaterial( material );

                                        auto textureHandle = texture->getHandle();
                                        if( textureHandle )
                                        {
                                            textureHandle->setName( textureTypeStr );
                                        }
                                    }
                                }

                                setTextureUnits( textures );
                            }
                        }
                        break;
                        case IMaterial::MaterialType::Custom:
                        {
                        }
                        break;
                        default:
                        {
                        }
                        break;
                        }
                    }
                }
            }
        }

        void CMaterialPassOgre::reload( SmartPtr<ISharedObject> data )
        {
            auto parent = getParent();

            unload( data );

            setParent( parent );
            load( data );

            setupMaterial();
        }

        void CMaterialPassOgre::setupMaterial()
        {
            try
            {
                auto parent = getParent();
                auto technique = fb::static_pointer_cast<CMaterialTechniqueOgre>( parent );
                if( technique )
                {
                    auto owner = technique->getMaterial();
                    auto material = fb::static_pointer_cast<CMaterialOgre>( owner );

                    setMaterial( material );
                }

                auto d = getDiffuse();
                auto e = getEmissive();

                if( auto pass = getPass() )
                {
                    auto diffuse = Ogre::ColourValue( d.r, d.g, d.b, d.a );
                    pass->setDiffuse( diffuse );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialPassOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    CMaterialPass::unload( data );
                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialPassOgre::initialise( Ogre::Pass *pass )
        {
            m_pass = pass;
        }

        void CMaterialPassOgre::setSceneBlending( u32 blendType )
        {
        }

        bool CMaterialPassOgre::isDepthCheckEnabled() const
        {
            return false;
        }

        void CMaterialPassOgre::setDepthCheckEnabled( bool enabled )
        {
        }

        bool CMaterialPassOgre::isDepthWriteEnabled() const
        {
            return false;
        }

        void CMaterialPassOgre::setDepthWriteEnabled( bool enabled )
        {
        }

        u32 CMaterialPassOgre::getCullingMode() const
        {
            return 0;
        }

        void CMaterialPassOgre::setCullingMode( u32 mode )
        {
        }

        SmartPtr<IMaterialTexture> CMaterialPassOgre::createTextureUnit()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            auto materialTexture = factoryManager->make_ptr<CMaterialTextureOgre>();

            auto material = getMaterial();
            FB_ASSERT( material );

            materialTexture->setMaterial( material );

            addTextureUnit( materialTexture );

            return materialTexture;
        }

        void CMaterialPassOgre::setTexture( SmartPtr<ITexture> tex, u32 layerIdx /*= 0 */ )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto factoryManager = applicationManager->getFactoryManager();

                if( layerIdx >= getNumTexturesNodes() )
                {
                    auto textures = Array<SmartPtr<IMaterialTexture>>();
                    textures.resize( layerIdx + 1 );

                    FB_ASSERT( getNumTexturesNodes() < textures.size() );

                    if( auto p = getTexturesPtr() )
                    {
                        auto &texs = *p;
                        for( size_t i = 0; i < texs.size(); ++i )
                        {
                            auto &texture = texs[i];
                            if( texture )
                            {
                                textures[i] = texture;
                            }
                        }
                    }

                    for( size_t i = 0; i < textures.size(); ++i )
                    {
                        auto &texture = textures[i];
                        if( !texture )
                        {
                            texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                        }
                    }

                    auto &texture = textures[layerIdx];
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                    }

                    if( texture )
                    {
                        texture->setParent( this );
                        texture->setTexture( tex );

                        graphicsSystem->loadObject( texture );
                    }

                    setTextureUnits( textures );
                }
                else
                {
                    FB_ASSERT( layerIdx < getNumTexturesNodes() );

                    if( layerIdx < getNumTexturesNodes() )
                    {
                        if( auto p = getTexturesPtr() )
                        {
                            auto &textures = *p;

                            auto &texture = textures[layerIdx];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                            }

                            if( texture )
                            {
                                texture->setParent( this );
                                texture->setTexture( tex );

                                graphicsSystem->loadObject( texture );
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

        void CMaterialPassOgre::setTexture( const String &fileName, u32 layerIdx /*= 0 */ )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();

                if( layerIdx >= getNumTexturesNodes() )
                {
                    auto textures = Array<SmartPtr<IMaterialTexture>>();
                    textures.resize( layerIdx + 1 );

                    FB_ASSERT( getNumTexturesNodes() < textures.size() );

                    if( auto p = getTexturesPtr() )
                    {
                        auto &texs = *p;
                        for( size_t i = 0; i < texs.size(); ++i )
                        {
                            auto &texture = texs[i];
                            if( texture )
                            {
                                textures[i] = texture;
                            }
                        }
                    }

                    for( size_t i = 0; i < textures.size(); ++i )
                    {
                        auto &texture = textures[i];
                        if( !texture )
                        {
                            texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                        }
                    }

                    auto &texture = textures[layerIdx];
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                    }

                    if( texture )
                    {
                        texture->setParent( this );
                        texture->setTextureName( fileName );
                    }

                    setTextureUnits( textures );
                }
                else
                {
                    FB_ASSERT( layerIdx < getNumTexturesNodes() );

                    if( layerIdx < getNumTexturesNodes() )
                    {
                        if( auto p = getTexturesPtr() )
                        {
                            auto &texs = *p;

                            auto &texture = texs[layerIdx];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgre>();
                            }

                            if( texture )
                            {
                                texture->setParent( this );
                                texture->setTextureName( fileName );
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

        void CMaterialPassOgre::setCubicTexture( const String &fileName, bool uvw,
                                                 u32 layerIdx /*= 0 */ )
        {
        }

        void CMaterialPassOgre::setFragmentParam( const String &name, f32 value )
        {
        }

        void CMaterialPassOgre::setFragmentParam( const String &name, const Vector2F &value )
        {
        }

        void CMaterialPassOgre::setFragmentParam( const String &name, const Vector3F &value )
        {
        }

        void CMaterialPassOgre::setFragmentParam( const String &name, const Vector4F &value )
        {
        }

        void CMaterialPassOgre::setFragmentParam( const String &name, const ColourF &value )
        {
        }

        hash_type CMaterialPassOgre::getRenderTechnique() const
        {
            return 0;
        }

        void CMaterialPassOgre::setRenderTechnique( hash_type renderTechnique )
        {
        }

        ColourF CMaterialPassOgre::getAmbient() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->getAmbient();
                }
            }

            return ColourF::White;
        }

        void CMaterialPassOgre::setAmbient( const ColourF &ambient )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<MaterialPassState>() )
                    {
                        state->setAmbient( ambient );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        ColourF CMaterialPassOgre::getDiffuse() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->getDiffuse();
                }
            }

            return ColourF::White;
        }

        void CMaterialPassOgre::setDiffuse( const ColourF &diffuse )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<MaterialPassState>() )
                    {
                        state->setDiffuse( diffuse );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        ColourF CMaterialPassOgre::getSpecular() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->getSpecular();
                }
            }

            return ColourF::White;
        }

        void CMaterialPassOgre::setSpecular( const ColourF &specular )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<MaterialPassState>() )
                    {
                        state->setSpecular( specular );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        ColourF CMaterialPassOgre::getEmissive() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->getEmissive();
                }
            }

            return ColourF::White;
        }

        void CMaterialPassOgre::setEmissive( const ColourF &emissive )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<MaterialPassState>() )
                    {
                        state->setEmissive( emissive );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        ColourF CMaterialPassOgre::getTint() const
        {
            return m_tint;
        }

        void CMaterialPassOgre::setTint( const ColourF &tint )
        {
            m_tint = tint;
        }

        f32 CMaterialPassOgre::getMetalness() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->getMetalness();
                }
            }

            return 0.0f;
        }

        void CMaterialPassOgre::setMetalness( f32 metalness )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<MaterialPassState>() )
                    {
                        state->setMetalness( metalness );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        f32 CMaterialPassOgre::getRoughness() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->getRoughness();
                }
            }

            return 0.0f;
        }

        void CMaterialPassOgre::setRoughness( f32 roughness )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<MaterialPassState>() )
                    {
                        state->setRoughness( roughness );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<Properties> CMaterialPassOgre::getProperties() const
        {
            auto properties = CMaterialNode<IMaterialPass>::getProperties();

            // auto handle = getHandle();
            // properties->setProperty("name", handle->getName());

            static const auto mainTextureStr = String( "Main Texture" );

            // properties->setProperty("Material Name", m_materialName);
            // properties->setProperty(mainTextureStr, m_mainTexturePath);

            auto ambient = getAmbient();
            auto diffuse = getDiffuse();
            auto specular = getSpecular();
            auto emissive = getEmissive();
            auto tint = getTint();

            auto metalness = getMetalness();
            auto roughness = getRoughness();

            properties->setProperty( "ambient", ambient );
            properties->setProperty( "diffuse", diffuse );
            properties->setProperty( "specular", specular );
            properties->setProperty( "emissive", emissive );
            properties->setProperty( "tint", tint );
            properties->setProperty( "metalness", metalness );
            properties->setProperty( "roughness", roughness );

            return properties;
        }

        void CMaterialPassOgre::setProperties( SmartPtr<Properties> properties )
        {
            auto ambient = getAmbient();
            auto diffuse = getDiffuse();
            auto specular = getSpecular();
            auto emissive = getEmissive();
            auto tint = getTint();

            auto metalness = getMetalness();
            auto roughness = getRoughness();

            // properties->getPropertyValue("Material Name", m_materialName);
            // properties->getPropertyValue("Main Texture", m_mainTexturePath);
            properties->getPropertyValue( "ambient", ambient );
            properties->getPropertyValue( "diffuse", diffuse );
            properties->getPropertyValue( "specular", specular );
            properties->getPropertyValue( "emissive", emissive );
            properties->getPropertyValue( "tint", tint );

            properties->getPropertyValue( "metalness", metalness );
            properties->getPropertyValue( "roughness", roughness );

            setAmbient( ambient );
            setDiffuse( diffuse );
            setSpecular( specular );
            setEmissive( emissive );
            setTint( tint );

            setMetalness( metalness );
            setRoughness( roughness );
        }

        Array<SmartPtr<ISharedObject>> CMaterialPassOgre::getChildObjects() const
        {
            auto textures = getTextureUnits();

            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( textures.size() );

            for( auto texture : textures )
            {
                objects.push_back( texture );
            }

            return objects;
        }

        void CMaterialPassOgre::MaterialPassOgreStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
            if( auto owner = fb::static_pointer_cast<CMaterialPassOgre>( getOwner() ) )
            {
                const auto &loadingState = owner->getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    auto passState = fb::static_pointer_cast<MaterialPassState>( state );
                    auto diffuse = OgreUtil::convertToOgre( passState->getDiffuse() );
                    auto specular = OgreUtil::convertToOgre( passState->getSpecular() );
                    auto emissive = OgreUtil::convertToOgre( passState->getEmissive() );
                    auto tint = OgreUtil::convertToOgre( passState->getTint() );
                    auto ambient = OgreUtil::convertToOgre( passState->getAmbient() );
                    auto lightingEnabled = passState->isLightingEnabled();

                    if( auto pass = owner->getPass() )
                    {
                        pass->setDiffuse( diffuse );
                        pass->setSpecular( specular );
                        pass->setEmissive( emissive );
                        //pass->setTint( tint );
                        //pass->setDiffuse( diffuse );

                        pass->setLightingEnabled(lightingEnabled);
                    }

                    state->setDirty( false );
                }
            }
        }

        void CMaterialPassOgre::MaterialPassOgreStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( message->isExactly<StateMessageVector4>() )
                {
                    auto vectorMessage = fb::static_pointer_cast<StateMessageVector4>( message );
                    auto value = vectorMessage->getValue();
                    auto type = vectorMessage->getType();

                    if( type == DIFFUSE_HASH )
                    {
                        owner->setDiffuse( ColourF( value.X(), value.Y(), value.Z(), value.W() ) );
                    }
                    else if( type == EMISSION_HASH )
                    {
                        owner->setEmissive( ColourF( value.X(), value.Y(), value.Z(), value.W() ) );
                    }
                }
            }
        }

        void CMaterialPassOgre::MaterialPassOgreStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }

        Ogre::Pass *CMaterialPassOgre::getPass() const
        {
            return m_pass;
        }

        void CMaterialPassOgre::setPass( Ogre::Pass *pass )
        {
            m_pass = pass;
        }

    }  // end namespace render
}  // end namespace fb
