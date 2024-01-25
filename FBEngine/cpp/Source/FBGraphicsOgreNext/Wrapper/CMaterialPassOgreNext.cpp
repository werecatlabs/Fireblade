#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialPassOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTechniqueOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include "FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h"
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>
#include <OgreHlmsPbsDatablock.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreHlms.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsManager.h>
#include <OgreTextureGpuManager.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CMaterialPassOgreNext, MaterialPass );

    CMaterialPassOgreNext::CMaterialPassOgreNext()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto stateContext = stateManager->addStateObject();

        auto stateListener = factoryManager->make_ptr<MaterialPassStateListener>();
        stateListener->setOwner( this );
        setStateListener( stateListener );
        stateContext->addStateListener( stateListener );

        auto state = factoryManager->make_ptr<MaterialPassState>();
        stateContext->addState( state );
        setStateContext( stateContext );
    }

    CMaterialPassOgreNext::~CMaterialPassOgreNext()
    {
        unload( nullptr );
    }

    void CMaterialPassOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            createTextureSlots();
            setupMaterial();

            auto textureUnits = getTextureUnits();
            for( auto tu : textureUnits )
            {
                tu->load( data );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialPassOgreNext::createTextureSlots()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto parent = getParent();
        FB_ASSERT( parent );

        auto textureUnits = getTextureUnits();

        auto technique = fb::static_pointer_cast<CMaterialTechniqueOgreNext>( parent );
        if( technique )
        {
            auto owner = technique->getMaterial();
            auto material = fb::static_pointer_cast<CMaterialOgreNext>( owner );
            if( material )
            {
                auto materialType = material->getMaterialType();
                switch( materialType )
                {
                case IMaterial::MaterialType::Standard:
                {
                    auto numSlots =
                        static_cast<size_t>( IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );

                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getPbsTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    auto numSlots =
                        static_cast<size_t>( IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getPbsTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    auto numSlots =
                        static_cast<size_t>( IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getPbsTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::TerrainTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getTerrainTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::TerrainTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getTerrainTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::TerrainTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getTerrainTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::SkyboxTextureTypes>( i );
                                auto textureTypeStr = GraphicsUtil::getSkyboxTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                    auto numSlots = static_cast<size_t>( IMaterial::SkyboxCubeTextureTypes::Count );
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::SkyboxCubeTextureTypes>( i );
                                auto textureTypeStr =
                                    GraphicsUtil::getSkyboxCubeTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );

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
                case IMaterial::MaterialType::UI:
                {
                    auto numSlots = static_cast<size_t>( 1 );
                    if( numSlots != textureUnits.size() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                                texture->setParent( this );

                                // auto textureType = (IMaterial::PbsTextureTypes)i;
                                auto textureTypeStr =
                                    String( "Base" );  // GraphicsUtil::getPbsTextureType(textureType);

                                texture->setTextureType( static_cast<u32>( i ) );

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

    void CMaterialPassOgreNext::reload( SmartPtr<ISharedObject> data )
    {
    }

    void CMaterialPassOgreNext::setupMaterial()
    {
        try
        {
            auto parent = getParent();
            auto technique = fb::static_pointer_cast<CMaterialTechniqueOgreNext>( parent );
            if( technique )
            {
                auto owner = technique->getMaterial();
                auto material = fb::static_pointer_cast<CMaterialOgreNext>( owner );
                if( material )
                {
                    auto datablock = material->getHlmsDatablock();
                    if( datablock )
                    {
                        auto materialType = material->getMaterialType();
                        switch( materialType )
                        {
                        case IMaterial::MaterialType::Standard:
                        {
                            auto pDatablock = static_cast<Ogre::HlmsPbsDatablock *>( datablock );

                            auto d = getDiffuse();
                            auto e = getEmissive();

                            auto metalness = getMetalness();
                            auto roughness = getRoughness();

                            pDatablock->setDiffuse( Ogre::Vector3( d.r, d.g, d.b ) );
                            pDatablock->setEmissive( Ogre::Vector3( e.r, e.g, e.b ) );

                            pDatablock->setWorkflow(
                                Ogre::HlmsPbsDatablock::Workflows::MetallicWorkflow );
                            pDatablock->setMetalness( metalness );
                            pDatablock->setRoughness( roughness );

                            /*
                            auto count = 0;
                            auto textures = *material->getTextures();
                            for( auto texture : textures )
                            {
                                auto ogreNextTexture =
                                    fb::dynamic_pointer_cast<CTextureOgreNext>( texture );

                                if( ogreNextTexture )
                                {
                                    try
                                    {
                                        auto pTexture = ogreNextTexture->getTexture();
                                        if( pTexture )
                                        {
                                            pDatablock->setTexture( (Ogre::PbsTextureTypes)count,
                                                                    pTexture );
                                        }
                                    }
                                    catch( Ogre::Exception &e )
                                    {
                                        FB_LOG_EXCEPTION( e );
                                    }
                                    catch( std::exception &e )
                                    {
                                        FB_LOG_EXCEPTION( e );
                                    }
                                }

                                count++;
                            }
                            */
                        }
                        break;
                        case IMaterial::MaterialType::UI:
                        {
                            auto pDatablock = static_cast<Ogre::HlmsUnlitDatablock *>( datablock );

                            // auto d = getDiffuse();
                            // auto e = getEmmissive();

                            // auto metalness = getMetalness();
                            // auto roughness = getRoughness();

                            // pbsDatablock->setDiffuse(Ogre::Vector3(d.r, d.g, d.b));
                            // pbsDatablock->setEmissive(Ogre::Vector3(e.r, e.g, e.b)); // todo hack
                            // for testing

                            // pbsDatablock->setWorkflow(Ogre::HlmsPbsDatablock::Workflows::MetallicWorkflow);
                            // pbsDatablock->setMetalness(metalness);
                            // pbsDatablock->setRoughness(roughness);

                            auto textures = getTextureUnits();
                            for( auto texture : textures )
                            {
                                auto textureName = texture->getTextureName();
                                if( !StringUtil::isNullOrEmpty( textureName ) )
                                {
                                    try
                                    {
                                        using namespace Ogre;

                                        auto texUnit = 0;
                                        auto mCreator = pDatablock->getCreator();
                                        HlmsSamplerblock *refParams = nullptr;

                                        TextureGpuManager *textureManager =
                                            mCreator->getRenderSystem()->getTextureGpuManager();
                                        TextureGpu *texture;
                                        if( !textureName.empty() )
                                        {
                                            texture = textureManager->createOrRetrieveTexture(
                                                textureName, GpuPageOutStrategy::Discard,
                                                TextureFlags::AutomaticBatching |
                                                    TextureFlags::PrefersLoadingFromFileAsSRGB,
                                                TextureTypes::Type2D,
                                                ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
                                        }

                                        pDatablock->setTexture( texUnit, texture, refParams );
                                    }
                                    catch( Ogre::Exception &e )
                                    {
                                        FB_LOG_EXCEPTION( e );
                                    }
                                    catch( std::exception &e )
                                    {
                                        FB_LOG_EXCEPTION( e );
                                    }
                                }
                            }
                        }
                        break;
                        default:
                        {
                        }
                        break;
                        }
                    }
                    else
                    {
                        auto handle = material->getHandle();
                        auto name = handle->getName();
                        FB_LOG_ERROR( "Datablock null: " + name );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialPassOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto textureUnits = getTextureUnits();
                for( auto &textureUnit : textureUnits )
                {
                    textureUnit->unload( data );
                }

                setTexturesPtr( nullptr );

                MaterialPass::unload( data );
                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialPassOgreNext::initialise( Ogre::Pass *pass )
    {
        m_pass = pass;
    }

    auto CMaterialPassOgreNext::createTextureUnit() -> SmartPtr<IMaterialTexture>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        return factoryManager->make_ptr<CMaterialTextureOgreNext>();
    }

    void CMaterialPassOgreNext::setTexture( SmartPtr<ITexture> tex, u32 layerIdx /*= 0 */ )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto factoryManager = applicationManager->getFactoryManager();

            auto textureUnits = getTextureUnits();

            if( layerIdx >= textureUnits.size() )
            {
                auto textures = Array<SmartPtr<IMaterialTexture>>();
                textures.resize( layerIdx + 1 );

                FB_ASSERT( textureUnits.size() < textures.size() );

                for( size_t i = 0; i < textureUnits.size(); ++i )
                {
                    auto &texture = textureUnits[i];
                    if( texture )
                    {
                        textures[i] = texture;
                    }
                }

                for( auto &texture : textures )
                {
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                    }
                }

                auto &texture = textures[layerIdx];
                if( !texture )
                {
                    texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
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
                FB_ASSERT( layerIdx < textureUnits.size() );

                if( layerIdx < textureUnits.size() )
                {
                    auto &texture = textureUnits[layerIdx];
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
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
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialPassOgreNext::setTexture( const String &fileName, u32 layerIdx /*= 0 */ )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();

            auto textureUnits = getTextureUnits();

            if( layerIdx >= textureUnits.size() )
            {
                auto textures = Array<SmartPtr<IMaterialTexture>>();
                textures.resize( layerIdx + 1 );

                FB_ASSERT( textureUnits.size() < textures.size() );

                for( size_t i = 0; i < textureUnits.size(); ++i )
                {
                    auto &texture = textureUnits[i];
                    if( texture )
                    {
                        textures[i] = texture;
                    }
                }

                for( auto &texture : textures )
                {
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                    }
                }

                auto &texture = textures[layerIdx];
                if( !texture )
                {
                    texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
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
                FB_ASSERT( layerIdx < textureUnits.size() );

                if( layerIdx < textureUnits.size() )
                {
                    auto &texture = textureUnits[layerIdx];
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTextureOgreNext>();
                    }

                    if( texture )
                    {
                        texture->setParent( this );
                        texture->setTextureName( fileName );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    CMaterialPassOgreNext::MaterialPassStateListener::MaterialPassStateListener(
        CMaterialPassOgreNext *pass ) :
        m_owner( pass )
    {
    }

    CMaterialPassOgreNext::MaterialPassStateListener::MaterialPassStateListener() = default;

    CMaterialPassOgreNext::MaterialPassStateListener::~MaterialPassStateListener() = default;

    void CMaterialPassOgreNext::MaterialPassStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto materialPassState = fb::static_pointer_cast<MaterialPassState>( state );

        if( auto owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                auto pMaterial = owner->getMaterial();
                auto material = fb::static_pointer_cast<CMaterialOgreNext>( pMaterial );

                auto datablock = owner->getDataBlock();
                auto materialType = material->getMaterialType();
                switch( materialType )
                {
                case IMaterial::MaterialType::Standard:
                {
                    if( auto pDatablock = static_cast<Ogre::HlmsPbsDatablock *>( datablock ) )
                    {
                        auto d = materialPassState->getDiffuse();
                        auto specular = materialPassState->getSpecular();
                        auto e = materialPassState->getEmissive();

                        auto metalness = materialPassState->getMetalness();
                        auto roughness = materialPassState->getRoughness();

                        pDatablock->setSpecular( Ogre::Vector3( specular.r, specular.g, specular.b ) );
                        pDatablock->setDiffuse( Ogre::Vector3( d.r, d.g, d.b ) );
                        pDatablock->setEmissive( Ogre::Vector3( e.r, e.g, e.b ) );

                        pDatablock->setWorkflow( Ogre::HlmsPbsDatablock::Workflows::MetallicWorkflow );
                        pDatablock->setMetalness( metalness );
                        pDatablock->setRoughness( roughness );
                    }
                }
                break;
                default:
                {
                }
                break;
                };

                state->setDirty( false );
            }
        }
    }

    void CMaterialPassOgreNext::MaterialPassStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        if( message->isExactly<StateMessageVector4>() )
        {
            auto vectorMessage = fb::static_pointer_cast<StateMessageVector4>( message );
            auto value = vectorMessage->getValue();
            auto type = vectorMessage->getType();

            if( type == DIFFUSE_HASH )
            {
                m_owner->setDiffuse( ColourF( value.X(), value.Y(), value.Z(), value.W() ) );
            }
            else if( type == EMISSION_HASH )
            {
                m_owner->setEmissive( ColourF( value.X(), value.Y(), value.Z(), value.W() ) );
            }
        }
    }

    void CMaterialPassOgreNext::MaterialPassStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto CMaterialPassOgreNext::MaterialPassStateListener::getOwner() const -> CMaterialPassOgreNext *
    {
        return m_owner;
    }

    void CMaterialPassOgreNext::MaterialPassStateListener::setOwner( CMaterialPassOgreNext *owner )
    {
        m_owner = owner;
    }

    auto CMaterialPassOgreNext::getPass() const -> Ogre::Pass *
    {
        return m_pass;
    }

    void CMaterialPassOgreNext::setPass( Ogre::Pass *pass )
    {
        m_pass = pass;
    }

    auto CMaterialPassOgreNext::getDataBlock() const -> Ogre::HlmsDatablock *
    {
        auto pMaterial = getMaterial();
        auto material = fb::static_pointer_cast<CMaterialOgreNext>( pMaterial );
        return material->getHlmsDatablock();
    }

}  // namespace fb::render
