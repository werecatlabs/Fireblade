#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CMaterialPass.h>
#include <FBCore/Graphics/CMaterialTexture.h>
#include <FBCore/Graphics/CMaterialTechnique.h>
#include <FBCore/Graphics/CMaterial.h>
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialPass, IMaterialPass );

        CMaterialPass::CMaterialPass()
        {
        }

        CMaterialPass::~CMaterialPass()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void CMaterialPass::load( SmartPtr<ISharedObject> data )
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

        void CMaterialPass::createTextureSlots()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto parent = getParent();
            FB_ASSERT( parent );

            auto technique = fb::static_pointer_cast<CMaterialTechnique>( parent );
            if( technique )
            {
                auto owner = technique->getMaterial();
                auto material = fb::static_pointer_cast<CMaterial>( owner );
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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                    auto textureTypeStr = GraphicsUtil::getPbsTextureType( textureType );

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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                    auto textureTypeStr = GraphicsUtil::getPbsTextureType( textureType );

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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::PbsTextureTypes>( i );
                                    auto textureTypeStr = GraphicsUtil::getPbsTextureType( textureType );

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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::TerrainTextureTypes>( i );
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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::TerrainTextureTypes>( i );
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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::TerrainTextureTypes>( i );
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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

                                    auto textureType = static_cast<IMaterial::SkyboxTextureTypes>( i );
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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
                                    texture->setParent( this );

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
                                    texture = factoryManager->make_ptr<CMaterialTexture>();
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

        void CMaterialPass::reload( SmartPtr<ISharedObject> data )
        {
            auto parent = getParent();

            unload( data );

            setParent( parent );
            load( data );

            setupMaterial();
        }

        void CMaterialPass::setupMaterial()
        {
            try
            {
                auto parent = getParent();
                auto technique = fb::static_pointer_cast<CMaterialTechnique>( parent );
                if( technique )
                {
                    auto owner = technique->getMaterial();
                    auto material = fb::static_pointer_cast<CMaterial>( owner );
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

        void CMaterialPass::setTextureUnits( Array<SmartPtr<IMaterialTexture>> textures )
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

        SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>> CMaterialPass::getTexturesPtr() const
        {
            return m_textures;
        }

        void CMaterialPass::setTexturesPtr( SharedPtr<ConcurrentArray<SmartPtr<IMaterialTexture>>> p )
        {
            m_textures = p;
        }

        void CMaterialPass::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    if( auto stateManager = applicationManager->getStateManager() )
                    {
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

                    if( auto p = getTexturesPtr() )
                    {
                        auto &textures = *p;
                        for( auto &t : textures )
                        {
                            t->unload( nullptr );
                        }

                        textures.clear();
                    }

                    CMaterialNode<IMaterialPass>::unload( data );
                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialPass::setSceneBlending( u32 blendType )
        {
        }

        bool CMaterialPass::isDepthCheckEnabled() const
        {
            return false;
        }

        void CMaterialPass::setDepthCheckEnabled( bool enabled )
        {
        }

        bool CMaterialPass::isDepthWriteEnabled() const
        {
            return false;
        }

        void CMaterialPass::setDepthWriteEnabled( bool enabled )
        {
        }

        u32 CMaterialPass::getCullingMode() const
        {
            return 0;
        }

        void CMaterialPass::setCullingMode( u32 mode )
        {
        }

        void CMaterialPass::setLightingEnabled( bool enabled, s32 passIdx )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    state->setLightingEnabled( enabled );
                }
            }
        }

        bool CMaterialPass::getLightingEnabled( s32 passIdx ) const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<MaterialPassState>() )
                {
                    return state->isLightingEnabled();
                }
            }

            return false;
        }

        SmartPtr<IMaterialTexture> CMaterialPass::createTextureUnit()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            auto materialTexture = factoryManager->make_ptr<CMaterialTexture>();
            addTextureUnit( materialTexture );

            return materialTexture;
        }

        void CMaterialPass::addTextureUnit( SmartPtr<IMaterialTexture> textureUnit )
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

        void CMaterialPass::removeTextureUnit( SmartPtr<IMaterialTexture> textureUnit )
        {
            if( auto p = getTexturesPtr() )
            {
                auto &textures = *p;

                auto texturesArray =
                    Array<SmartPtr<IMaterialTexture>>( textures.begin(), textures.end() );
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

        Array<SmartPtr<IMaterialTexture>> CMaterialPass::getTextureUnits() const
        {
            if( auto p = getTexturesPtr() )
            {
                auto &textures = *p;
                return Array<SmartPtr<IMaterialTexture>>( textures.begin(), textures.end() );
            }

            return Array<SmartPtr<IMaterialTexture>>();
        }

        size_t CMaterialPass::getNumTexturesNodes() const
        {
            if( auto p = getTexturesPtr() )
            {
                auto &textures = *p;
                return textures.size();
            }

            return 0;
        }

        void CMaterialPass::setTexture( SmartPtr<ITexture> tex, u32 layerIdx /*= 0 */ )
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
                            texture = factoryManager->make_ptr<CMaterialTexture>();
                        }
                    }

                    auto &texture = textures[layerIdx];
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTexture>();
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
                                texture = factoryManager->make_ptr<CMaterialTexture>();
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

        void CMaterialPass::setTexture( const String &fileName, u32 layerIdx /*= 0 */ )
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
                            texture = factoryManager->make_ptr<CMaterialTexture>();
                        }
                    }

                    auto &texture = textures[layerIdx];
                    if( !texture )
                    {
                        texture = factoryManager->make_ptr<CMaterialTexture>();
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
                                texture = factoryManager->make_ptr<CMaterialTexture>();
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

        void CMaterialPass::setCubicTexture( const String &fileName, bool uvw, u32 layerIdx /*= 0 */ )
        {
        }

        void CMaterialPass::setFragmentParam( const String &name, f32 value )
        {
        }

        void CMaterialPass::setFragmentParam( const String &name, const Vector2F &value )
        {
        }

        void CMaterialPass::setFragmentParam( const String &name, const Vector3F &value )
        {
        }

        void CMaterialPass::setFragmentParam( const String &name, const Vector4F &value )
        {
        }

        void CMaterialPass::setFragmentParam( const String &name, const ColourF &value )
        {
        }

        hash_type CMaterialPass::getRenderTechnique() const
        {
            return 0;
        }

        void CMaterialPass::setRenderTechnique( hash_type renderTechnique )
        {
        }

        ColourF CMaterialPass::getAmbient() const
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

        void CMaterialPass::setAmbient( const ColourF &ambient )
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

        ColourF CMaterialPass::getDiffuse() const
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

        void CMaterialPass::setDiffuse( const ColourF &diffuse )
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

        ColourF CMaterialPass::getSpecular() const
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

        void CMaterialPass::setSpecular( const ColourF &specular )
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

        ColourF CMaterialPass::getEmissive() const
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

        void CMaterialPass::setEmissive( const ColourF &emissive )
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

        ColourF CMaterialPass::getTint() const
        {
            return m_tint;
        }

        void CMaterialPass::setTint( const ColourF &tint )
        {
            m_tint = tint;
        }

        f32 CMaterialPass::getMetalness() const
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

        void CMaterialPass::setMetalness( f32 metalness )
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

        f32 CMaterialPass::getRoughness() const
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

        void CMaterialPass::setRoughness( f32 roughness )
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

        SmartPtr<ISharedObject> CMaterialPass::toData() const
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

        void CMaterialPass::fromData( SmartPtr<ISharedObject> data )
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

        SmartPtr<Properties> CMaterialPass::getProperties() const
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

        void CMaterialPass::setProperties( SmartPtr<Properties> properties )
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

        Array<SmartPtr<ISharedObject>> CMaterialPass::getChildObjects() const
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

        void CMaterialPass::MaterialPassStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = fb::static_pointer_cast<CMaterialPass>( getOwner() ) )
            {
                const auto &loadingState = owner->getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    state->setDirty( false );
                }
            }
        }

        void CMaterialPass::MaterialPassStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = fb::static_pointer_cast<CMaterialPass>( getOwner() ) )
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

        void CMaterialPass::MaterialPassStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }
    }  // end namespace render
}  // end namespace fb
