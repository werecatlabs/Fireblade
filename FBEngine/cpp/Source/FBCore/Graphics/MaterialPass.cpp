#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/MaterialPass.h>
#include <FBCore/Graphics/MaterialTexture.h>
#include <FBCore/Graphics/MaterialTechnique.h>
#include <FBCore/Graphics/Material.h>
#include <FBCore/Graphics/Texture.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/State/States/MaterialPassState.h>
#include <FBCore/State/Messages/StateMessageVector4.h>
#include <FBGraphics/GraphicsUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, MaterialPass, IMaterialPass );

    MaterialPass::MaterialPass() = default;

    MaterialPass::~MaterialPass()
    {
        const auto &loadingState = getLoadingState();
        if( loadingState != LoadingState::Unloaded )
        {
            unload( nullptr );
        }
    }

    void MaterialPass::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            createTextureSlots();

            FB_ASSERT( getTexturesPtr() && !getTexturesPtr()->empty() );

            setupMaterial();

            //FB_ASSERT( m_pass );

            if( auto p = getTexturesPtr() )
            {
                auto &textures = *p;
                for( auto t : textures )
                {
                    t->load( nullptr );
                }
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MaterialPass::createTextureSlots()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto parent = getParent();
        FB_ASSERT( parent );

        auto technique = fb::static_pointer_cast<MaterialTechnique>( parent );
        if( technique )
        {
            auto owner = technique->getMaterial();
            auto material = fb::static_pointer_cast<Material>( owner );
            if( material )
            {
                switch( auto materialType = material->getMaterialType() )
                {
                case IMaterial::MaterialType::Standard:
                {
                    auto numSlots =
                        static_cast<size_t>( IMaterial::PbsTextureTypes::NUM_PBSM_TEXTURE_TYPES );
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
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
                    if( numSlots != getNumTexturesNodes() )
                    {
                        auto textures = Array<SmartPtr<IMaterialTexture>>();
                        textures.resize( numSlots );

                        for( size_t i = 0; i < textures.size(); ++i )
                        {
                            auto &texture = textures[i];
                            if( !texture )
                            {
                                texture = factoryManager->make_ptr<MaterialTexture>();
                                texture->setParent( this );

                                auto textureType = static_cast<IMaterial::SkyboxCubeTextureTypes>( i );
                                auto textureTypeStr =
                                    GraphicsUtil::getSkyboxCubeTextureType( textureType );

                                texture->setTextureType( static_cast<u32>( i ) );
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
                                texture = factoryManager->make_ptr<MaterialTexture>();
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

    void MaterialPass::reload( SmartPtr<ISharedObject> data )
    {
        auto parent = getParent();

        unload( data );

        setParent( parent );
        load( data );

        setupMaterial();
    }

    void MaterialPass::setupMaterial()
    {
        try
        {
            auto parent = getParent();
            auto technique = fb::static_pointer_cast<MaterialTechnique>( parent );
            if( technique )
            {
                auto owner = technique->getMaterial();
                auto material = fb::static_pointer_cast<Material>( owner );
            }

            auto d = getDiffuse();
            auto e = getEmissive();

            //if( auto pass = getPass() )
            //{
            //    auto diffuse = Ogre::ColourValue( d.r, d.g, d.b, d.a );
            //    pass->setDiffuse( diffuse );
            //}
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MaterialPass::setTextureUnits( Array<SmartPtr<IMaterialTexture>> textures )
    {
        auto p = getTexturesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IMaterialTexture>>>();
            setTexturesPtr( p );
        }

        if( p )
        {
            *p = ConcurrentArray<SmartPtr<IMaterialTexture>>( textures.begin(), textures.end() );
        }
    }

    auto MaterialPass::getTexturesPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>>
    {
        return m_textures;
    }

    void MaterialPass::setTexturesPtr( SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>> p )
    {
        m_textures = p;
    }

    void MaterialPass::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto stateManager = applicationManager->getStateManager() )
                {
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
                }

                if( auto p = getTexturesPtr() )
                {
                    auto &textures = *p;
                    for( auto &t : textures )
                    {
                        t->unload( nullptr );
                    }

                    textures.clear();
                }

                MaterialNode<IMaterialPass>::unload( data );
                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MaterialPass::setSceneBlending( u32 blendType )
    {
    }

    auto MaterialPass::isDepthCheckEnabled() const -> bool
    {
        return false;
    }

    void MaterialPass::setDepthCheckEnabled( bool enabled )
    {
    }

    auto MaterialPass::isDepthWriteEnabled() const -> bool
    {
        return false;
    }

    void MaterialPass::setDepthWriteEnabled( bool enabled )
    {
    }

    auto MaterialPass::getCullingMode() const -> u32
    {
        return 0;
    }

    void MaterialPass::setCullingMode( u32 mode )
    {
    }

    void MaterialPass::setLightingEnabled( bool enabled, s32 passIdx )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                state->setLightingEnabled( enabled );
            }
        }
    }

    auto MaterialPass::getLightingEnabled( s32 passIdx ) const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->isLightingEnabled();
            }
        }

        return false;
    }

    auto MaterialPass::createTextureUnit() -> SmartPtr<IMaterialTexture>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        auto materialTexture = factoryManager->make_ptr<MaterialTexture>();
        addTextureUnit( materialTexture );

        return materialTexture;
    }

    void MaterialPass::addTextureUnit( SmartPtr<IMaterialTexture> textureUnit )
    {
        auto p = getTexturesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IMaterialTexture>>>();
            setTexturesPtr( p );
        }

        if( p )
        {
            auto &textures = *p;
            textures.push_back( textureUnit );
        }
    }

    void MaterialPass::removeTextureUnit( SmartPtr<IMaterialTexture> textureUnit )
    {
        if( auto p = getTexturesPtr() )
        {
            auto &textures = *p;

            auto texturesArray = Array<SmartPtr<IMaterialTexture>>( textures.begin(), textures.end() );
            auto it = std::find( texturesArray.begin(), texturesArray.end(), textureUnit );
            if( it != texturesArray.end() )
            {
                texturesArray.erase( it );
            }

            auto newTextures = fb::make_shared<ConcurrentArray<SmartPtr<IMaterialTexture>>>();
            *newTextures = ConcurrentArray<SmartPtr<IMaterialTexture>>( texturesArray.begin(),
                                                                        texturesArray.end() );
            setTexturesPtr( newTextures );
        }
    }

    auto MaterialPass::getTextureUnits() const -> Array<SmartPtr<IMaterialTexture>>
    {
        if( auto p = getTexturesPtr() )
        {
            auto &textures = *p;
            return Array<SmartPtr<IMaterialTexture>>( textures.begin(), textures.end() );
        }

        return {};
    }

    auto MaterialPass::getNumTexturesNodes() const -> size_t
    {
        if( auto p = getTexturesPtr() )
        {
            auto &textures = *p;
            return textures.size();
        }

        return 0;
    }

    void MaterialPass::setTexture( SmartPtr<ITexture> tex, u32 layerIdx /*= 0 */ )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
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

                for( auto &texture : textures )
                {
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<MaterialTexture>();
                    }
                }

                auto &texture = textures[layerIdx];
                if( !texture )
                {
                    texture = factoryManager->make_ptr<MaterialTexture>();
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
                            texture = factoryManager->make_ptr<MaterialTexture>();
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

    void MaterialPass::setTexture( const String &fileName, u32 layerIdx /*= 0 */ )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
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

                for( auto &texture : textures )
                {
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<MaterialTexture>();
                    }
                }

                auto &texture = textures[layerIdx];
                if( !texture )
                {
                    texture = factoryManager->make_ptr<MaterialTexture>();
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
                            texture = factoryManager->make_ptr<MaterialTexture>();
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

    void MaterialPass::setCubicTexture( const String &fileName, bool uvw, u32 layerIdx /*= 0 */ )
    {
    }

    void MaterialPass::setFragmentParam( const String &name, f32 value )
    {
    }

    void MaterialPass::setFragmentParam( const String &name, const Vector2<real_Num> &value )
    {
    }

    void MaterialPass::setFragmentParam( const String &name, const Vector3<real_Num> &value )
    {
    }

    void MaterialPass::setFragmentParam( const String &name, const Vector4F &value )
    {
    }

    void MaterialPass::setFragmentParam( const String &name, const ColourF &value )
    {
    }

    auto MaterialPass::getRenderTechnique() const -> hash_type
    {
        return 0;
    }

    void MaterialPass::setRenderTechnique( hash_type renderTechnique )
    {
    }

    auto MaterialPass::getAmbient() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->getAmbient();
            }
        }

        return ColourF::White;
    }

    void MaterialPass::setAmbient( const ColourF &ambient )
    {
        try
        {
            if( auto stateContext = getStateContext() )
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

    auto MaterialPass::getDiffuse() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->getDiffuse();
            }
        }

        return ColourF::White;
    }

    void MaterialPass::setDiffuse( const ColourF &diffuse )
    {
        try
        {
            if( auto stateContext = getStateContext() )
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

    auto MaterialPass::getSpecular() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->getSpecular();
            }
        }

        return ColourF::White;
    }

    void MaterialPass::setSpecular( const ColourF &specular )
    {
        try
        {
            if( auto stateContext = getStateContext() )
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

    auto MaterialPass::getEmissive() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->getEmissive();
            }
        }

        return ColourF::White;
    }

    void MaterialPass::setEmissive( const ColourF &emissive )
    {
        try
        {
            if( auto stateContext = getStateContext() )
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

    auto MaterialPass::getTint() const -> ColourF
    {
        return m_tint;
    }

    void MaterialPass::setTint( const ColourF &tint )
    {
        m_tint = tint;
    }

    auto MaterialPass::getMetalness() const -> f32
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->getMetalness();
            }
        }

        return 0.0f;
    }

    void MaterialPass::setMetalness( f32 metalness )
    {
        try
        {
            if( auto stateContext = getStateContext() )
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

    auto MaterialPass::getRoughness() const -> f32
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<MaterialPassState>() )
            {
                return state->getRoughness();
            }
        }

        return 0.0f;
    }

    void MaterialPass::setRoughness( f32 roughness )
    {
        try
        {
            if( auto stateContext = getStateContext() )
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

    auto MaterialPass::toData() const -> SmartPtr<ISharedObject>
    {
        auto data = fb::make_ptr<Properties>();
        data->setName( "passes" );

        auto d = getDiffuse();
        auto e = getEmissive();

        data->setProperty( "diffuse", d );
        data->setProperty( "emissive", e );

        data->setProperty( "metalness", getMetalness() );
        data->setProperty( "roughness", getRoughness() );

        auto textureUnits = getTextureUnits();
        for( auto textureUnit : textureUnits )
        {
            auto textureData = fb::static_pointer_cast<Properties>( textureUnit->toData() );
            textureData->setName( "textures" );
            data->addChild( textureData );
        }

        return data;
    }

    void MaterialPass::fromData( SmartPtr<ISharedObject> data )
    {
        auto properties = fb::static_pointer_cast<Properties>( data );

        auto diffuse = ColourF::White;
        properties->getPropertyValue( "diffuse", diffuse );

        if( getDiffuse() != diffuse )
        {
            setDiffuse( diffuse );
        }

        auto emissive = ColourF::White;
        properties->getPropertyValue( "emissive", emissive );
        setEmissive( emissive );

        auto metalness = 0.0f;
        properties->getPropertyValue( "metalness", metalness );
        setMetalness( metalness );

        auto roughness = 0.0f;
        properties->getPropertyValue( "roughness", roughness );
        setRoughness( roughness );

        createTextureSlots();

        auto textureIndex = 0;
        auto textures = properties->getChildrenByName( "textures" );
        for( auto &texture : textures )
        {
            auto textureUnits = getTextureUnits();
            if( textureIndex < textureUnits.size() )
            {
                auto pTexture = textureUnits[textureIndex];

                pTexture->fromData( texture );
            }

            textureIndex++;
        }
    }

    auto MaterialPass::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = MaterialNode<IMaterialPass>::getProperties();

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
        auto lightingEnabled = getLightingEnabled();

        properties->setProperty( "ambient", ambient );
        properties->setProperty( "diffuse", diffuse );
        properties->setProperty( "specular", specular );
        properties->setProperty( "emissive", emissive );
        properties->setProperty( "tint", tint );
        properties->setProperty( "metalness", metalness );
        properties->setProperty( "roughness", roughness );
        properties->setProperty( "lightingEnabled", lightingEnabled );

        return properties;
    }

    void MaterialPass::setProperties( SmartPtr<Properties> properties )
    {
        auto ambient = getAmbient();
        auto diffuse = getDiffuse();
        auto specular = getSpecular();
        auto emissive = getEmissive();
        auto tint = getTint();

        auto metalness = getMetalness();
        auto roughness = getRoughness();

        auto lightingEnabled = getLightingEnabled();

        // properties->getPropertyValue("Material Name", m_materialName);
        // properties->getPropertyValue("Main Texture", m_mainTexturePath);
        properties->getPropertyValue( "ambient", ambient );
        properties->getPropertyValue( "diffuse", diffuse );
        properties->getPropertyValue( "specular", specular );
        properties->getPropertyValue( "emissive", emissive );
        properties->getPropertyValue( "tint", tint );

        properties->getPropertyValue( "metalness", metalness );
        properties->getPropertyValue( "roughness", roughness );
        properties->setProperty( "lightingEnabled", lightingEnabled );

        setAmbient( ambient );
        setDiffuse( diffuse );
        setSpecular( specular );
        setEmissive( emissive );
        setTint( tint );

        setMetalness( metalness );
        setRoughness( roughness );

        setLightingEnabled( lightingEnabled );

        reload( nullptr );
    }

    auto MaterialPass::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto textures = getTextureUnits();

        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( textures.size() );

        for( auto texture : textures )
        {
            objects.emplace_back( texture );
        }

        return objects;
    }

    void MaterialPass::MaterialPassStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        if( auto owner = fb::static_pointer_cast<MaterialPass>( getOwner() ) )
        {
            const auto &loadingState = owner->getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                state->setDirty( false );
            }
        }
    }

    void MaterialPass::MaterialPassStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        if( auto owner = fb::static_pointer_cast<MaterialPass>( getOwner() ) )
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

    void MaterialPass::MaterialPassStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }
}  // namespace fb::render
