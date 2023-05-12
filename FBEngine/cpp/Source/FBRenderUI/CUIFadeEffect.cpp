#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIFadeEffect.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIFadeEffect::CUIFadeEffect()
        {
            m_type = "FadeEffect";

            auto applicationManager = core::IApplicationManager::instance();
            // FactoryPtr factory = engine->getFactory();
            // m_fsm = factory->create("FSMStandard");
            // m_fsm->addFSMListener(this);
        }

        CUIFadeEffect::~CUIFadeEffect()
        {
            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );

            // m_fsm->removeFSMListener(this);
        }

        void CUIFadeEffect::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            const TiXmlElement *element = pNode->ToElement();
            {
                const String id( element->Attribute( "id" ) );
                if( id.length() > 0 )
                {
                    setName( id );
                }
            }

            const String templateId( element->Attribute( "template" ) );
            if( templateId.length() > 0 )
            {
                // SmartPtr<CUIItemTemplate> templateItem =
                // CGUIManager::getSingletonPtr()->findElement(templateId); if(!templateItem.isNull())
                //{
                // }
                // else
                //{
                //	String message = String("Could not find template: ") + String(templateId);
                //	MessageBoxUtil::show(message.c_str());
                // }
            }

            f32 left = StringUtil::parseFloat( element->Attribute( "posX" ) );
            f32 top = StringUtil::parseFloat( element->Attribute( "posY" ) );
            f32 width = StringUtil::parseFloat( element->Attribute( "width" ) );
            f32 height = StringUtil::parseFloat( element->Attribute( "height" ) );

            String material( element->Attribute( "material" ) );

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

            // m_container = overlayManager->addElement(String("Panel"), getName());

            SmartPtr<render::IOverlayElementContainer>
                cont;  // = overlayManager->findElement(parent->getName());
            cont->addChild( m_container );

            //m_container->setLeft( left );
            //m_container->setTop( top );
            //m_container->setWidth( width );
            //m_container->setHeight( height );

            //m_position = Vector2F( left, top );
            //m_size = Vector2F( width, height );

            //m_container->setMaterialName( material.c_str() );

            bool isVisible = StringUtil::parseBool( element->Attribute( "visible" ) );
            if( parent )
                isVisible = isVisible && parent->isVisible();

            setVisible( isVisible );

            auto zorder = cont->getZOrder();
            auto newZOrder = zorder + (u32)parent->getChildren()->size() + 1;
            m_container->setZOrder( newZOrder );

            // getFSM()->setInitialState(FS_IDLE);

            // if(element->Attribute("state") != NULL)
            //	getFSM()->setStateName(element->Attribute("state"));
        }

        void CUIFadeEffect::update()
        {
            // getFSM()->update();
            CUIElement::update();
        }

        void CUIFadeEffect::onToggleVisibility()
        {
            m_container->setVisible( isVisible() );

            CUIElement::onToggleVisibility();
        }

        void CUIFadeEffect::setMaterialName( const String &materialName )
        {
            //m_container->setMaterialName( materialName.c_str() );
        }

        void CUIFadeEffect::setPosition( const Vector2F &position )
        {
            //m_container->setLeft( position.X() );
            //m_container->setTop( position.Y() );
            CUIElement::setPosition( position );
        }

        void CUIFadeEffect::setSize( const Vector2F &size )
        {
            //m_container->setWidth( size.X() );
            //m_container->setHeight( size.Y() );
            CUIElement::setSize( size );
        }

        void CUIFadeEffect::OnEnterState( u8 state )
        {
            switch( state )
            {
            case FS_IDLE:
                OnEnterIdleState();
                break;
            case FS_FADEIN:
                OnEnterFadeInState();
                break;
            case FS_FADEOUT:
                OnEnterFadeOutState();
                break;
            default:
            {
            }
            }
        }

        void CUIFadeEffect::OnUpdateState( u8 state )
        {
            switch( state )
            {
            case FS_IDLE:
                OnUpdateIdleState();
                break;
            case FS_FADEIN:
                OnUpdateFadeInState();
                break;
            case FS_FADEOUT:
                OnUpdateFadeOutState();
                break;
            default:
            {
            }
            }
        }

        void CUIFadeEffect::OnLeaveState( u8 state )
        {
            switch( state )
            {
            case FS_IDLE:
                OnLeaveIdleState();
                break;
            case FS_FADEIN:
                OnLeaveFadeInState();
                break;
            case FS_FADEOUT:
                OnLeaveFadeOutState();
                break;
            default:
            {
            }
            }
        }

        void CUIFadeEffect::OnEnterIdleState()
        {
        }

        void CUIFadeEffect::OnEnterFadeInState()
        {
            auto timer = core::IApplicationManager::instance()->getTimer();
            m_displayHitUntilTime = timer->getTimeMilliseconds() + 1000;
            m_container->setVisible( true );
            m_alpha = 1.0f;
            m_container->setColour( ColourF( 1.0f, 0.f, 0.f, m_alpha ) );
        }

        void CUIFadeEffect::OnEnterFadeOutState()
        {
            auto timer = core::IApplicationManager::instance()->getTimer();
            m_displayHitUntilTime = timer->getTimeMilliseconds() + 1000;
            m_container->setVisible( true );
            m_alpha = 0.01f;
            m_container->setColour( ColourF( 1.0f, 0.f, 0.f, m_alpha ) );
        }

        void CUIFadeEffect::OnUpdateIdleState()
        {
        }

        void CUIFadeEffect::OnUpdateFadeInState()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            m_alpha -= 0.5f * (f32)dt;
            if( m_alpha > 1.0f )
            {
                m_alpha = 1.0f;
            }

            // String materialName = m_container->getMaterialName();
            // const Ogre::SmartPtr<IMaterial> mat =
            // Ogre::MaterialManager::getSingletonPtr()->getByName(materialName.c_str()); Ogre::Technique
            // *tech = mat->getTechnique(0);   // Get the technique Ogre::Pass *pass = tech->getPass(0);
            // // Get the pass Ogre::TextureUnitState *tex = pass->getTextureUnitState(0);   // Get the
            // texture_unit state tex->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL,
            // Ogre::LBS_TEXTURE, m_alpha);   // Change the alpha operation */

            // m_container->setColour(ColourF(1.0f,0.0f,0.0f,m_alpha));

            // if(m_alpha < 0.01f)
            //{
            //	getFSM()->setState(FS_IDLE);
            // }
        }

        void CUIFadeEffect::OnUpdateFadeOutState()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            m_alpha += 0.5f * (f32)dt;
            if( m_alpha > 1.0f )
            {
                m_alpha = 1.0f;
            }

            // String materialName = m_container->getMaterialName();
            // const Ogre::SmartPtr<IMaterial> mat =
            // Ogre::MaterialManager::getSingletonPtr()->getByName(materialName.c_str()); Ogre::Technique
            // *tech = mat->getTechnique(0);   // Get the technique Ogre::Pass *pass = tech->getPass(0);
            // // Get the pass Ogre::TextureUnitState *tex = pass->getTextureUnitState(0);   // Get the
            // texture_unit state tex->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL,
            // Ogre::LBS_TEXTURE, m_alpha);   // Change the alpha operation */

            // m_container->setColour(ColourF(1.0f,0.0f,0.0f,m_alpha));

            // if(m_alpha >= 1.0f)
            //{
            //	getFSM()->setState(FS_IDLE);
            // }
        }

        void CUIFadeEffect::OnLeaveIdleState()
        {
            handleEvent( String( "EndLeaveIdle" ) );
        }

        void CUIFadeEffect::OnLeaveFadeInState()
        {
            handleEvent( String( "EndFadeIn" ) );
        }

        void CUIFadeEffect::OnLeaveFadeOutState()
        {
            handleEvent( String( "EndFadeOut" ) );
        }

        u8 CUIFadeEffect::GetStateFromName( const String &stateName ) const
        {
            if( stateName == ( "Idle" ) )
            {
                return FS_IDLE;
            }
            if( stateName == ( "FadeIn" ) )
            {
                return FS_FADEIN;
            }
            if( stateName == ( "FadeOut" ) )
            {
                return FS_FADEOUT;
            }

            return 0;
        }

        String CUIFadeEffect::GetStateNameFromId( u8 stateId ) const
        {
            switch( stateId )
            {
            case FS_IDLE:
                return String( "Idle" );
            case FS_FADEIN:
                return String( "FadeIn" );
            case FS_FADEOUT:
                return String( "FadeOut" );
            default:
            {
                return StringUtil::EmptyString;
            }
            }

            return StringUtil::EmptyString;
        }

        SmartPtr<IFSM> &CUIFadeEffect::getFSM()
        {
            return m_fsm;
        }

        void CUIFadeEffect::handleEvent( const String &eventType )
        {
            if( eventType == ( String( "FadeIn" ) ) )
            {
                // getFSM()->setState(FS_FADEIN);
            }
            else if( eventType == ( String( "FadeOut" ) ) )
            {
                // getFSM()->setState(FS_FADEOUT);
            }
            else
            {
                CUIElement::onEvent( eventType );
            }
        }
    }  // namespace ui
}  // end namespace fb
