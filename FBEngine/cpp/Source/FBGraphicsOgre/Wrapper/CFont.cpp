#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CFont.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTechniqueOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialPassOgre.h>
#include <FBGraphicsOgre/Wrapper/CTextureOgre.h>
#include <FBCore/FBCore.h>
#include <FBGraphics/FBGraphics.h>
#include <Ogre.h>
#include <OgreFontManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CFont, CResourceGraphics<IFont> );

        const hash_type CFont::SET_TEXTURE_HASH = StringUtil::getHash( "setTexture" );
        const hash_type CFont::FRAGMENT_FLOAT_HASH = StringUtil::getHash( "fragmentFloat" );
        const hash_type CFont::FRAGMENT_VECTOR2F_HASH = StringUtil::getHash( "fragmentVector2f" );
        const hash_type CFont::FRAGMENT_Vector3f_HASH = StringUtil::getHash( "fragmentVector3f" );
        const hash_type CFont::FRAGMENT_VECTOR4F_HASH = StringUtil::getHash( "fragmentVector4f" );
        const hash_type CFont::FRAGMENT_COLOUR_HASH = StringUtil::getHash( "fragmentColour" );

        u32 CFont::m_nameExt = 0;

        CFont::CFont()
        {
            setResourceType( ResourceType::Material );

            static const auto MaterialStr = String( "Material" );
            auto name = MaterialStr + StringUtil::toString( m_nameExt++ );

            auto handle = getHandle();
            handle->setName( name );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            m_stateObject = stateManager->addStateObject();

            auto stateListener = factoryManager->make_ptr<MaterialStateListener>();
            stateListener->setOwner( this );
            m_stateListener = stateListener;
            m_stateObject->addStateListener( m_stateListener );

            /*
            auto sceneNodeState = factoryManager->make_ptr<MaterialState>();
            sceneNodeState->setSceneNode(this);
            m_stateObject->setState(sceneNodeState);
            m_stateObject->setOwner(this);
            m_state = sceneNodeState;

            auto renderTask = graphicsSystem->getStateTask();
            m_state->setTaskId(renderTask);
             */
        }

        CFont::~CFont()
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void CFont::saveToFile( const String &filePath )
        {
            //auto applicationManager = core::IApplicationManager::instance();
            //FB_ASSERT( applicationManager );

            //auto fileSystem = applicationManager->getFileSystem();
            //FB_ASSERT( fileSystem );

            //auto pData = toData();

            //auto mat = pData->getDataAsType<data::material_graph>();
            //auto materialStr = DataUtil::toString( mat, true );

            //fileSystem->writeAllText( filePath, materialStr );
        }

        void CFont::loadFromFile( const String &filePath )
        {
            //auto applicationManager = core::IApplicationManager::instance();
            //FB_ASSERT( applicationManager );

            //auto fileSystem = applicationManager->getFileSystem();
            //FB_ASSERT( fileSystem );

            //auto stream = fileSystem->open( filePath );
            //if( stream )
            //{
            //    auto materialStr = stream->getAsString();

            //    data::material_graph materialData;
            //    DataUtil::parse( materialStr, &materialData );

            //    FileInfo fileInfo;
            //    if( fileSystem->findFileInfo( filePath, fileInfo ) )
            //    {
            //        auto fileId = fileInfo.fileId;
            //        setFileSystemId( fileId );
            //    }

            //    auto data = fb::make_ptr<Data<data::material_graph>>();
            //    data->setData( &materialData );
            //    fromData( data );
            //    load( nullptr );
            //}
        }

        void CFont::save()
        {
            //try
            //{
            //    auto applicationManager = core::IApplicationManager::instance();
            //    FB_ASSERT( applicationManager );

            //    auto fileSystem = applicationManager->getFileSystem();
            //    FB_ASSERT( fileSystem );

            //    auto data = toData();
            //    FB_ASSERT( data );

            //    auto materialData = data->getDataAsType<data::material_graph>();

            //    auto dataStr = DataUtil::toString( materialData, true );
            //    FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            //    auto fileId = getFileSystemId();
            //    FB_ASSERT( fileId != 0 );

            //    FileInfo fileInfo;
            //    if( fileSystem->findFileInfo( fileId, fileInfo ) )
            //    {
            //        auto filePath = String( fileInfo.filePath.c_str() );
            //        fileSystem->writeAllText( filePath, dataStr );
            //    }
            //    else
            //    {
            //        FB_LOG_ERROR( "Could not save material: " + String( fileInfo.filePath.c_str() ) );
            //    }
            //}
            //catch( std::exception &e )
            //{
            //    FB_LOG_EXCEPTION( e );
            //}
        }

        void CFont::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto ogreFontManager = Ogre::FontManager::getSingletonPtr();

                auto uuid = "default";  //StringUtil::getUUID();
                auto grp = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
                auto fontResource = ogreFontManager->createOrRetrieve( uuid, grp );
                
                auto font = Ogre::static_pointer_cast<Ogre::Font>( fontResource.first );
                font->setSource( font_source );
                font->setType( Ogre::FT_TRUETYPE );
                font->setTrueTypeResolution( font_resolution );
                font->setTrueTypeSize( (u32)font_size );

                font->load();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CFont::reload( SmartPtr<ISharedObject> data )
        {
            try
            {
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CFont::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &state = getLoadingState();
                if( state != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    //auto ogreFontManager = Ogre::FontManager::getSingletonPtr();
                    //if( ogreFontManager )
                    //{
                    //    if( m_font )
                    //    {
                    //        ogreFontManager->remove( m_font );
                    //        m_font = nullptr;
                    //    }
                    //}

                    CResourceGraphics<IFont>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IStateContext> CFont::getStateObject() const
        {
            return m_stateObject;
        }

        void CFont::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        String CFont::getName() const
        {
            return m_name;
        }

        void CFont::setName( const String &name )
        {
            m_name = name;
        }

        hash32 CFont::getId() const
        {
            return m_id;
        }

        hash32 CFont::getRendererType() const
        {
            return m_rendererType;
        }

        void CFont::setRendererType( hash32 rendererType )
        {
            m_rendererType = rendererType;
        }

        void CFont::createMaterialByType()
        {
            try
            {
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CFont::setDirty( bool dirty )
        {
            // if (m_isDirty != dirty)
            //{
            //	m_isDirty = dirty;

            //	auto applicationManager = core::IApplicationManager::instance();
            //	FB_ASSERT(applicationManager);

            //	auto factoryManager = applicationManager->getFactoryManager();
            //	auto message = factoryManager->make_ptr<StateMessageDirty>();
            //	message->setDirty(dirty);

            //	auto graphicsSystem = applicationManager->getGraphicsSystem();
            //	auto stateTask = graphicsSystem->getStateTask();
            //	m_stateObject->addMessage(stateTask, message);
            //}
        }

        bool CFont::isDirty() const
        {
            return false;
        }

        SmartPtr<Properties> CFont::getProperties() const
        {
            return nullptr;
        }

        void CFont::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "font_type", font_type );
            properties->getPropertyValue( "font_source", font_source );
            properties->getPropertyValue( "font_size", font_size );
            properties->getPropertyValue( "font_resolution", font_resolution );
        }

        Array<SmartPtr<ISharedObject>> CFont::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();

            return objects;
        }

        void CFont::MaterialStateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
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

        void CFont::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CFont::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CFont *CFont::MaterialStateListener::getOwner() const
        {
            return m_owner;
        }

        void CFont::MaterialStateListener::setOwner( CFont *owner )
        {
            m_owner = owner;
        }

        CFont::MaterialStateListener::~MaterialStateListener()
        {
        }

        CFont::MaterialStateListener::MaterialStateListener()
        {
        }

        CFont::MaterialStateListener::MaterialStateListener( CFont *material ) : m_owner( material )
        {
        }

        void CFont::MaterialEvents::preRenderTargetUpdate( const Ogre::RenderTargetEvent &evt )
        {
        }

        void CFont::MaterialEvents::postRenderTargetUpdate( const Ogre::RenderTargetEvent &evt )
        {
        }

        void CFont::MaterialEvents::preViewportUpdate( const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        void CFont::MaterialEvents::postViewportUpdate( const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        void CFont::MaterialEvents::viewportAdded( const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        void CFont::MaterialEvents::viewportRemoved( const Ogre::RenderTargetViewportEvent &evt )
        {
        }

        CFont::MaterialEvents::~MaterialEvents()
        {
        }

        CFont::MaterialEvents::MaterialEvents( CFont *material ) : m_material( material )
        {
        }
    }  // end namespace render
}  // end namespace fb
