#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UIImage.h>
#include "FBGraphicsOgreNext/UI/UIManager.h"
#include "ColibriGui/ColibriCustomShape.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/ColibriLabel.h"
#include "ColibriGui/ColibriManager.h"
#include "Wrapper/CMaterialOgreNext.h"
#include "ColibriGui/Ogre/OgreHlmsColibri.h"
#include "ColibriGui/Ogre/OgreHlmsColibriDatablock.h"
#include "OgreHlmsManager.h"
#include "OgreHlms.h"
#include "OgreRoot.h"
#include "Wrapper/CTextureOgreNext.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        UIImage::UIImage()
        {
            createStateContext();
        }

        UIImage::~UIImage()
        {
            unload( nullptr );
        }

        void UIImage::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                auto ui = fb::static_pointer_cast<UIManager>( applicationManager->getRenderUI() );

                auto window = ui->getLayoutWindow();
                auto colibriManager = ui->getColibriManager();
                //m_renderable = colibriManager->createWindow( window );
                m_renderable = colibriManager->createWidget<Colibri::CustomShape>( window );

                m_renderable->setNumTriangles( 2 );
                m_renderable->setQuad( 0, Ogre::Vector2::UNIT_SCALE * -1.0f,
                                       Ogre::Vector2::UNIT_SCALE * 2.0f, Ogre::ColourValue::White );

                m_renderable->setTransform( Ogre::Vector2( 0, 0 ), Ogre::Vector2( 1920, 1080 ) );
                m_renderable->setSkin( "EmptyBg" );

                m_renderable->setKeyboardNavigable(false);

                createDatablock();
                m_renderable->setDatablock( m_datablock );

                setWidget( m_renderable );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIImage::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                m_renderable = nullptr;
                UIElement<ui::IUIImage>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIImage::createDatablock()
        {
            auto root = Ogre::Root::getSingletonPtr();

            Ogre::HlmsManager *hlmsManager = root->getHlmsManager();
            Ogre::Hlms *hlms = hlmsManager->getHlms( Ogre::HLMS_USER0 );
            COLIBRI_ASSERT_HIGH( dynamic_cast<Ogre::HlmsColibri *>( hlms ) );
            auto hlmsColibri = static_cast<Ogre::HlmsColibri *>( hlms );

            Ogre::HlmsMacroblock macroblock;
            Ogre::HlmsBlendblock blendblock;

            macroblock.mDepthCheck = false;
            macroblock.mDepthWrite = false;
            blendblock.setBlendType( Ogre::SBT_TRANSPARENT_ALPHA );

            m_datablockName = StringUtil::getUUID();
            auto datablock = hlms->createDatablock( m_datablockName, m_datablockName, macroblock,
                                                    blendblock, Ogre::HlmsParamVec() );

            m_datablock = (Ogre::HlmsColibriDatablock *)datablock;

            m_datablock->setUseColour( true );
            m_datablock->setColour( Ogre::ColourValue::White );
        }

        void UIImage::setTexture( SmartPtr<render::ITexture> texture )
        {
            m_texture = texture;

            if( auto stateObject = getStateObject() )
            {
                stateObject->setDirty( true );
            }
        }

        SmartPtr<render::ITexture> UIImage::getTexture() const
        {
            return m_texture;
        }

        void UIImage::setMaterialName( const String &materialName )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            ScopedLock lock( graphicsSystem );

            m_renderable->setDatablockOrMaterialName(
                materialName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            if( auto stateObject = getStateObject() )
            {
                stateObject->setDirty( true );
            }
        }

        String UIImage::getMaterialName() const
        {
            return "";
        }

        void UIImage::setMaterial( SmartPtr<render::IMaterial> material )
        {
            if( m_material )
            {
                auto materialStateObject = m_material->getStateObject();
                if( materialStateObject )
                {
                    materialStateObject->removeStateListener( m_materialStateListener );
                }
            }

            m_material = material;

            if( m_material )
            {
                auto materialStateObject = m_material->getStateObject();
                if( materialStateObject )
                {
                    materialStateObject->addStateListener( m_materialStateListener );
                }
            }

            if( auto stateObject = getStateObject() )
            {
                stateObject->setDirty( true );
            }
        }

        SmartPtr<render::IMaterial> UIImage::getMaterial() const
        {
            return nullptr;
        }

        void UIImage::handleStateChanged( SmartPtr<IState> &state )
        {
            updateMaterial();
        }

        void UIImage::setupMaterial( SmartPtr<render::IMaterial> material )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( m_material )
                    {
                        auto materialStateObject = m_material->getStateObject();
                        if( materialStateObject )
                        {
                            materialStateObject->removeStateListener( m_materialStateListener );
                        }
                    }

                    m_material = material;

                    if( m_material )
                    {
                        auto materialStateObject = m_material->getStateObject();
                        if( materialStateObject )
                        {
                            materialStateObject->addStateListener( m_materialStateListener );
                        }
                    }

                    if( m_material )
                    {
                        if( m_material->getLoadingState() == LoadingState::Loaded )
                        {
                            auto pMaterial =
                                fb::static_pointer_cast<render::CMaterialOgreNext>( m_material );
                            auto datablock = pMaterial->getHlmsDatablock();
                            if( datablock )
                            {
                                auto materialName = pMaterial->getDatablockName();

                                FB_ASSERT( !StringUtil::isNullOrEmpty( materialName ) );

                                updateMaterial();

                                //m_renderable->setSkin( materialName );
                            }
                        }
                        else
                        {
                            auto message = factoryManager->make_ptr<StateMessageMaterial>();
                            message->setMaterial( material );

                            if( auto stateObject = getStateObject() )
                            {
                                stateObject->addMessage( stateTask, message );
                            }
                        }
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageMaterial>();
                    message->setMaterial( material );

                    if( auto stateObject = getStateObject() )
                    {
                        stateObject->addMessage( stateTask, message );
                    }
                }
            }
            catch( Ogre::Exception &e )
            {
                auto errorMessage = String( e.getFullDescription().c_str() );
                FB_LOG_ERROR( errorMessage );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UIImage::updateMaterial()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            ScopedLock lock( graphicsSystem );

            if( m_datablock )
            {
                //m_renderable->setSkin( "EmptyBg" );

                auto ogreTexture = fb::static_pointer_cast<render::CTextureOgreNext>( getTexture() );
                if( ogreTexture )
                {
                    Ogre::HlmsSamplerblock hlmsSamplerblock;
                    hlmsSamplerblock.mU = Ogre::TAM_WRAP;
                    hlmsSamplerblock.mV = Ogre::TAM_WRAP;
                    hlmsSamplerblock.mU = Ogre::TAM_WRAP;

                    auto pOgreTexture = ogreTexture->getTexture();
                    m_datablock->setTexture( 0, pOgreTexture, &hlmsSamplerblock );

                    m_renderable->setDatablock( m_datablock );
                    //m_renderable->setSkin( m_datablockName );

                    //m_renderable->setSizeAfterClipping( Ogre::Vector2::UNIT_SCALE * 512 );

                    //float borderSize[Colibri::Borders::Borders::NumBorders] = { 0 };
                    //borderSize[2] = 512;
                    //borderSize[3] = 512;
                    //m_renderable->setBorderSize( borderSize );

                    //m_renderable->setClipBordersMatchSkin();
                }
            }
        }

        UIImage::MaterialStateListener::~MaterialStateListener()
        {
        }

        void UIImage::MaterialStateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
        {
        }

        void UIImage::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void UIImage::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<UIImage> UIImage::MaterialStateListener::getOwner() const
        {
            return m_owner;
        }

        void UIImage::MaterialStateListener::setOwner( SmartPtr<UIImage> owner )
        {
            m_owner = owner;
        }
    }  // namespace ui
}  // namespace fb
