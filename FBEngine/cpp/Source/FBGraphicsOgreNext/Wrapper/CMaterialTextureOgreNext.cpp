#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTechniqueOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialPassOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBCore/FBCore.h>

#include <OgreHlmsPbsDatablock.h>
#include <OgreHlmsUnlitDatablock.h>
#include <OgreHlms.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsManager.h>
#include <OgreTextureGpuManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialTextureOgreNext, IMaterialTexture );

        CMaterialTextureOgreNext::CMaterialTextureOgreNext()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();

            auto stateContext = stateManager->addStateObject();
            setStateObject( stateContext );

            auto stateListener = factoryManager->make_ptr<MaterialTextureOgreStateListener>();
            stateListener->setOwner( this );
            setStateListener( stateListener );
            stateContext->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<MaterialPassState>();
            stateContext->setState( state );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );

            auto listener = fb::make_ptr<TextureListener>();
            listener->setOwner( this );
            m_textureListener = listener;
        }

        CMaterialTextureOgreNext::~CMaterialTextureOgreNext()
        {
            unload( nullptr );
        }

        void CMaterialTextureOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTextureOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    m_animator = nullptr;
                    m_texture = nullptr;
                    CMaterialNode<IMaterialTexture>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTextureOgreNext::loadImage( Ogre::TextureGpu *texture, const String &filePath )
        {
            using namespace Ogre;

            auto imagePtr = new Image2();
            imagePtr->load( filePath, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            texture->scheduleTransitionTo( GpuResidency::Resident, imagePtr, true );
            // texture->scheduleReupload(imagePtr, true);
        }

        String CMaterialTextureOgreNext::getTextureName() const
        {
            return m_texturePath;
        }

        void CMaterialTextureOgreNext::setTextureName( const String &texturePath )
        {
            if( m_texturePath != texturePath )
            {
                m_texturePath = texturePath;

                if( !StringUtil::isNullOrEmpty( texturePath ) )
                {
                    auto parent = getParent();
                    FB_ASSERT( fb::dynamic_pointer_cast<CMaterialPassOgreNext>( parent ) );

                    auto pass = fb::static_pointer_cast<CMaterialPassOgreNext>( parent );
                    if( pass )
                    {
                        auto passParent = pass->getParent();
                        FB_ASSERT( fb::dynamic_pointer_cast<CMaterialTechniqueOgreNext>( passParent ) );

                        auto technique =
                            fb::static_pointer_cast<CMaterialTechniqueOgreNext>( passParent );
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
                                        auto pDatablock =
                                            static_cast<Ogre::HlmsPbsDatablock *>( datablock );
                                        pDatablock->setTexture( Ogre::PbsTextureTypes::PBSM_DIFFUSE,
                                                                texturePath );
                                    }
                                    break;
                                    case IMaterial::MaterialType::UI:
                                    {
                                        auto pDatablock =
                                            static_cast<Ogre::HlmsUnlitDatablock *>( datablock );

                                        auto texUnit = 0;
                                        auto creator = pDatablock->getCreator();

                                        auto renderSystem = creator->getRenderSystem();
                                        auto textureManager = renderSystem->getTextureGpuManager();

                                        auto texture = textureManager->createOrRetrieveTexture(
                                            texturePath, Ogre::GpuPageOutStrategy::Discard,
                                            Ogre::TextureFlags::AutomaticBatching |
                                                Ogre::TextureFlags::PrefersLoadingFromFileAsSRGB,
                                            Ogre::TextureTypes::Type2D,
                                            Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
                                        loadImage( texture, texturePath );

                                        Ogre::HlmsSamplerblock *refParams = nullptr;
                                        pDatablock->setTexture( texUnit, texture, refParams );
                                    }
                                    break;
                                    default:
                                    {
                                    }
                                    break;
                                    };
                                }
                            }
                        }
                    }
                }
            }
        }

        SmartPtr<ITexture> CMaterialTextureOgreNext::getTexture() const
        {
            return m_texture;
        }

        void CMaterialTextureOgreNext::setTexture( SmartPtr<ITexture> texture )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ISharedObject::ScopedLock lock( graphicsSystem );

            if( m_texture != texture )
            {
                if( m_texture )
                {
                    m_texture->removeObjectListener( m_textureListener );
                }

                m_texture = texture;

                if( m_texture )
                {
                    m_texture->addObjectListener( m_textureListener );
                }

                if( m_texture )
                {
                    if( !m_texture->isLoaded() )
                    {
                        m_texture->load( nullptr );
                    }
                }

                Ogre::HlmsSamplerblock hlmsSamplerblock;
                hlmsSamplerblock.mU = Ogre::TAM_WRAP;
                hlmsSamplerblock.mV = Ogre::TAM_WRAP;
                hlmsSamplerblock.mU = Ogre::TAM_WRAP;

                auto parent = getParent();
                FB_ASSERT( fb::dynamic_pointer_cast<CMaterialPassOgreNext>( parent ) );

                auto pass = fb::static_pointer_cast<CMaterialPassOgreNext>( parent );
                if( pass )
                {
                    auto passParent = pass->getParent();
                    FB_ASSERT( fb::dynamic_pointer_cast<CMaterialTechniqueOgreNext>( passParent ) );

                    auto technique = fb::static_pointer_cast<CMaterialTechniqueOgreNext>( passParent );
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
                                    if( texture )
                                    {
                                        auto pDatablock =
                                            static_cast<Ogre::HlmsPbsDatablock *>( datablock );

                                        auto pTexture =
                                            fb::static_pointer_cast<CTextureOgreNext>( texture );
                                        auto t = pTexture->getTexture();

                                        auto type = getTextureType();
                                        auto eType = (Ogre::PbsTextureTypes)type;

                                        pDatablock->setTexture( eType, t, &hlmsSamplerblock );
                                    }
                                }
                                break;
                                case IMaterial::MaterialType::UI:
                                {
                                    if( texture )
                                    {
                                        auto pDatablock =
                                            static_cast<Ogre::HlmsUnlitDatablock *>( datablock );

                                        auto texUnit = 0;

                                        auto pTexture =
                                            fb::static_pointer_cast<CTextureOgreNext>( texture );
                                        auto t = pTexture->getTexture();
                                        pDatablock->setTexture( texUnit, t, &hlmsSamplerblock );
                                    }
                                }
                                break;
                                default:
                                {
                                }
                                break;
                                };
                            }
                        }
                    }
                }
            }
        }

        void CMaterialTextureOgreNext::setScale( const Vector3F &scale )
        {
            m_scale = scale;
        }

        SmartPtr<IAnimator> CMaterialTextureOgreNext::getAnimator() const
        {
            return m_animator;
        }

        void CMaterialTextureOgreNext::setAnimator( SmartPtr<IAnimator> animator )
        {
            m_animator = animator;
        }

        void CMaterialTextureOgreNext::_getObject( void **ppObject )
        {
            *ppObject = nullptr;
        }

        Array<SmartPtr<ISharedObject>> CMaterialTextureOgreNext::getChildObjects() const
        {
            try
            {
                auto objects = CMaterialNode<IMaterialTexture>::getChildObjects();
                objects.push_back( m_animator );
                objects.push_back( m_texture );
                return objects;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return Array<SmartPtr<ISharedObject>>();
        }

        u32 CMaterialTextureOgreNext::getTextureType() const
        {
            return m_textureType;
        }

        void CMaterialTextureOgreNext::setTextureType( u32 textureType )
        {
            m_textureType = textureType;
        }

        Ogre::TextureUnitState *CMaterialTextureOgreNext::getTextureUnitState() const
        {
            return m_textureUnitState;
        }

        void CMaterialTextureOgreNext::setTextureUnitState( Ogre::TextureUnitState *textureUnitState )
        {
            m_textureUnitState = textureUnitState;
        }

        void CMaterialTextureOgreNext::MaterialTextureOgreStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CMaterialTextureOgreNext::MaterialTextureOgreStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CMaterialTextureOgreNext::MaterialTextureOgreStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }

        Parameter CMaterialTextureOgreNext::TextureListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto task = Thread::getCurrentTask();
            if( task == Thread::Task::Render )
            {
                if( eventValue == IEvent::loadingStateChanged )
                {
                    auto newState = (LoadingState)arguments[1].getS32();
                    if( newState == LoadingState::Loaded )
                    {
                        if( auto owner = getOwner() )
                        {
                            if( auto stateObject = owner->getStateObject() )
                            {
                                stateObject->setDirty( true );
                            }
                        }
                    }
                }
            }

            return Parameter();
        }

        CMaterialTextureOgreNext *CMaterialTextureOgreNext::TextureListener::getOwner() const
        {
            return m_owner;
        }

        void CMaterialTextureOgreNext::TextureListener::setOwner( CMaterialTextureOgreNext *owner )
        {
            m_owner = owner;
        }

    }  // end namespace render
}  // end namespace fb
