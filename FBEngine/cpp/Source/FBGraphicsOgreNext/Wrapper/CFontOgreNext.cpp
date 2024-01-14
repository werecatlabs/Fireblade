#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CFontOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>
#include <Ogre.h>
#include <OgreFontManager.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CFontOgreNext, ResourceGraphics<IFont> );

    const hash_type CFontOgreNext::SET_TEXTURE_HASH = StringUtil::getHash( "setTexture" );
    const hash_type CFontOgreNext::FRAGMENT_FLOAT_HASH = StringUtil::getHash( "fragmentFloat" );
    const hash_type CFontOgreNext::FRAGMENT_VECTOR2F_HASH = StringUtil::getHash( "fragmentVector2f" );
    const hash_type CFontOgreNext::FRAGMENT_Vector3f_HASH = StringUtil::getHash( "fragmentVector3f" );
    const hash_type CFontOgreNext::FRAGMENT_VECTOR4F_HASH = StringUtil::getHash( "fragmentVector4f" );
    const hash_type CFontOgreNext::FRAGMENT_COLOUR_HASH = StringUtil::getHash( "fragmentColour" );

    u32 CFontOgreNext::m_nameExt = 0;

    CFontOgreNext::CFontOgreNext()
    {
        setResourceType( ResourceType::Material );

        static const auto MaterialStr = String( "Material" );
        auto name = MaterialStr + StringUtil::toString( m_nameExt++ );

        auto handle = getHandle();
        handle->setName( name );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        m_stateContext = stateManager->addStateObject();

        auto stateListener = factoryManager->make_ptr<MaterialStateListener>();
        stateListener->setOwner( this );
        m_stateListener = stateListener;
        m_stateContext->addStateListener( m_stateListener );

        /*
        auto sceneNodeState = factoryManager->make_ptr<MaterialState>();
        sceneNodeState->setSceneNode(this);
        m_stateContext->setState(sceneNodeState);
        m_stateContext->setOwner(this);
        m_state = sceneNodeState;

        auto renderTask = graphicsSystem->getStateTask();
        m_state->setTaskId(renderTask);
         */
    }

    CFontOgreNext::~CFontOgreNext()
    {
        const auto &loadingState = getLoadingState();
        if( loadingState != LoadingState::Unloaded )
        {
            unload( nullptr );
        }
    }

    void CFontOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ScopedLock lock( graphicsSystem );

            setLoadingState( LoadingState::Loading );

            auto ogreFontManager = Ogre::FontManager::getSingletonPtr();

            auto uuid = "default";  //StringUtil::getUUID();
            auto grp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
            auto fontResource = ogreFontManager->createOrRetrieve( uuid, grp );

            auto font = Ogre::static_pointer_cast<Ogre::Font>( fontResource.first );
            font->setSource( font_source );
            font->setType( Ogre::FT_TRUETYPE );
            font->setTrueTypeResolution( font_resolution );
            font->setTrueTypeSize( font_size );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CFontOgreNext::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CFontOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto ogreFontManager = Ogre::FontManager::getSingletonPtr();

                if( m_font )
                {
                    ogreFontManager->remove( m_font->getHandle() );
                    m_font.setNull();
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CFontOgreNext::getStateContext() const -> SmartPtr<IStateContext>
    {
        return m_stateContext;
    }

    void CFontOgreNext::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    auto CFontOgreNext::getName() const -> String
    {
        return m_name;
    }

    void CFontOgreNext::setName( const String &name )
    {
        m_name = name;
    }

    auto CFontOgreNext::getId() const -> hash32
    {
        return m_id;
    }

    auto CFontOgreNext::getRendererType() const -> hash32
    {
        return m_rendererType;
    }

    void CFontOgreNext::setRendererType( hash32 rendererType )
    {
        m_rendererType = rendererType;
    }

    void CFontOgreNext::createMaterialByType()
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CFontOgreNext::setDirty( bool dirty )
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

    auto CFontOgreNext::isDirty() const -> bool
    {
        return false;
    }

    auto CFontOgreNext::toJson() const -> String
    {
        //auto data = toData();
        //if( data )
        //{
        //    auto materialData = data->getDataAsType<data::material_graph>();
        //    return DataUtil::toString( materialData, true );
        //}

        return nullptr;
    }

    auto CFontOgreNext::toData() const -> SmartPtr<ISharedObject>
    {
        //try
        //{
        //    auto applicationManager = core::ApplicationManager::instance();
        //    FB_ASSERT( applicationManager );

        //    auto factoryManager = applicationManager->getFactoryManager();
        //    FB_ASSERT( factoryManager );

        //    auto data = factoryManager->make_ptr<Data<data::material_graph>>();
        //    auto materialData = data->getDataAsType<data::material_graph>();
        //    //materialData->materialType = static_cast<s32>( getMaterialType() );

        //    return data;
        //}
        //catch( std::exception &e )
        //{
        //    FB_LOG_EXCEPTION( e );
        //}

        return nullptr;
    }

    void CFontOgreNext::fromData( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CFontOgreNext::getProperties() const -> SmartPtr<Properties>
    {
        return nullptr;
    }

    void CFontOgreNext::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "font_type", font_type );
        properties->getPropertyValue( "font_source", font_source );
        properties->getPropertyValue( "font_size", font_size );
        properties->getPropertyValue( "font_resolution", font_resolution );
    }

    auto CFontOgreNext::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Array<SmartPtr<ISharedObject>>();

        return objects;
    }

    void CFontOgreNext::MaterialStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        if( message->isExactly<StateMessageLoad>() )
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
        }
        else if( message->isExactly<StateMessageSetTexture>() )
        {
        }
    }

    void CFontOgreNext::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void CFontOgreNext::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto CFontOgreNext::MaterialStateListener::getOwner() const -> CFontOgreNext *
    {
        return m_owner;
    }

    void CFontOgreNext::MaterialStateListener::setOwner( CFontOgreNext *owner )
    {
        m_owner = owner;
    }

    CFontOgreNext::MaterialStateListener::~MaterialStateListener() = default;

    CFontOgreNext::MaterialStateListener::MaterialStateListener() = default;

    CFontOgreNext::MaterialStateListener::MaterialStateListener( CFontOgreNext *material ) :
        m_owner( material )
    {
    }
}  // namespace fb::render
