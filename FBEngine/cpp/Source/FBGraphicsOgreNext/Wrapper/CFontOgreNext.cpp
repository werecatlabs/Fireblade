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
            unload( data );
            load( data );
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

    auto CFontOgreNext::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = ResourceGraphics<IFont>::getProperties();
        properties->setProperty( "font_type", font_type );
        properties->setProperty( "font_source", font_source );
        properties->setProperty( "font_size", font_size );
        properties->setProperty( "font_resolution", font_resolution );
        return properties;
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

}  // namespace fb::render
