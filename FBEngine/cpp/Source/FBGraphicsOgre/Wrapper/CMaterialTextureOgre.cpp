#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTextureOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTechniqueOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialPassOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialOgre.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgre.h>
#include <FBCore/FBCore.h>
#include <OgreTextureManager.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialTextureOgre, CMaterialTexture );

        CMaterialTextureOgre::CMaterialTextureOgre()
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

        CMaterialTextureOgre::~CMaterialTextureOgre()
        {
            unload( nullptr );
        }

        void CMaterialTextureOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                if( isEnabled() )
                {
                    if( !m_textureUnitState )
                    {
                        createTextureUnitState();
                    }
                }

                if( auto texture = getTexture() )
                {
                    if( !texture->isLoaded() )
                    {
                        texture->load( nullptr );
                    }
                }

                if( auto texture = getTexture() )
                {
                    auto pTexture = fb::static_pointer_cast<CTextureOgre>( texture );

                    if( auto t = pTexture->getTexture() )
                    {
                        if( auto textureUnitState = getTextureUnitState() )
                        {
                            textureUnitState->setTexture( t );
                        }
                    }
                }

                if( auto stateContext = getStateObject() )
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

        void CMaterialTextureOgre::createTextureUnitState()
        {
            FB_ASSERT( getParent() );

            if( auto parent = getParent() )
            {
                FB_ASSERT( parent->getMaterial() == getMaterial() );

                auto parentPass = fb::static_pointer_cast<CMaterialPassOgre>( parent );
                if( parentPass )
                {
                    FB_ASSERT( parentPass->getPass() );

                    if( auto pass = parentPass->getPass() )
                    {
                        m_textureUnitState = pass->createTextureUnitState();
                    }
                }
            }
        }

        void CMaterialTextureOgre::reload( SmartPtr<ISharedObject> data )
        {
        }

        void CMaterialTextureOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    if( m_texture )
                    {
                        m_texture->removeObjectListener( m_textureListener.get() );
                    }

                    m_textureUnitState = nullptr;

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

        void CMaterialTextureOgre::initialise( Ogre::TextureUnitState *textureUnitState )
        {
            m_textureUnitState = textureUnitState;
        }

        String CMaterialTextureOgre::getTextureName() const
        {
            return m_texturePath;
        }

        void CMaterialTextureOgre::setTextureName( const String &name )
        {
            m_textureUnitState->setTextureName( name );

            auto textureManager = Ogre::TextureManager::getSingletonPtr();
            auto texture =
                textureManager->load( name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
            if( texture )
            {
                m_textureUnitState->setTexture( texture );
            }
        }

        SmartPtr<ITexture> CMaterialTextureOgre::getTexture() const
        {
            return m_texture;
        }

        void CMaterialTextureOgre::setTexture( SmartPtr<ITexture> texture )
        {
            //FB_ASSERT( getMaterial() );
            //FB_ASSERT( getMaterial() && getMaterial()->isValid() );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

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

                if( !texture->isLoaded() )
                {
                    graphicsSystem->loadObject( texture );
                }

                FB_ASSERT( isValid() );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->setDirty( true );
                }
            }
        }

        void CMaterialTextureOgre::_getObject( void **ppObject )
        {
            *ppObject = nullptr;
        }

        Array<SmartPtr<ISharedObject>> CMaterialTextureOgre::getChildObjects() const
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

        u32 CMaterialTextureOgre::getTextureType() const
        {
            return m_textureType;
        }

        void CMaterialTextureOgre::setTextureType( u32 textureType )
        {
            m_textureType = textureType;
        }

        Ogre::TextureUnitState *CMaterialTextureOgre::getTextureUnitState() const
        {
            return m_textureUnitState;
        }

        void CMaterialTextureOgre::setTextureUnitState( Ogre::TextureUnitState *textureUnitState )
        {
            m_textureUnitState = textureUnitState;
        }

        Parameter CMaterialTextureOgre::TextureListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::loadingStateChanged )
            {
                auto newState = (LoadingState)arguments[0].getS32();
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

            return Parameter();
        }

        CMaterialTextureOgre *CMaterialTextureOgre::TextureListener::getOwner() const
        {
            return m_owner;
        }

        void CMaterialTextureOgre::TextureListener::setOwner( CMaterialTextureOgre *owner )
        {
            m_owner = owner;
        }

        void CMaterialTextureOgre::MaterialTextureOgreStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CMaterialTextureOgre::MaterialTextureOgreStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
            if( auto owner = fb::static_pointer_cast<CMaterialTextureOgre>( getOwner() ) )
            {
                if( !owner->isLoaded() )
                {
                    return;
                }

                FB_ASSERT( owner->getMaterial() );

                if( auto material = owner->getMaterial() )
                {
                    //FB_ASSERT( material->isValid() );

                    if( material->isLoaded() )
                    {
                        if( auto texture = owner->getTexture() )
                        {
                            auto pTexture = fb::static_pointer_cast<CTextureOgre>( texture );

                            if( auto t = pTexture->getTexture() )
                            {
                                if( auto textureUnitState = owner->getTextureUnitState() )
                                {
                                    textureUnitState->setTexture( t );

                                    state->setDirty( false );
                                }
                            }
                        }

                        if( auto stateObject = material->getStateObject() )
                        {
                            stateObject->setDirty( true );
                        }

                        state->setDirty( false );
                    }
                }
            }
        }

        void CMaterialTextureOgre::MaterialTextureOgreStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }

    }  // end namespace render
}  // end namespace fb
