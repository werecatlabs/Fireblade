#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIAnimatedMaterial.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb::ui
{

    CUIAnimatedMaterial::CUIAnimatedMaterial() : m_currentFrame( 0 )
    {
        m_type = "AnimatedMaterial";
    }

    CUIAnimatedMaterial::~CUIAnimatedMaterial()
    {
        removeAllChildren();

        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
        overlayManager->removeElement( m_container );
    }

    void CUIAnimatedMaterial::initialise( SmartPtr<IUIElement> &parent )
    {
        // String value;
        // f32 left = 0.0f;
        // f32 top = 0.0f;
        // f32 width = 0.0f;
        // f32 height = 0.0f;
        // String alignment;

        // SmartPtr<IOverlayManager> overlayManager =
        // IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

        // m_container = overlayManager->addElement(String("Panel"), getName());

        // SmartPtr<IOverlayElementContainer> cont = overlayManager->findElement(parent->getName());
        // cont->addChild(m_container);

        // m_container->setLeft(left);
        // m_container->setTop(top);
        // m_container->setWidth(width);
        // m_container->setHeight(height);

        // m_container->setVisible(false);

        // m_position = Vector2F(left, top);
        // m_size = Vector2F(width, height);

        // int zorder = cont->getZOrder();
        // int newZOrder = zorder+parent->getChildren().size()+1;
        // m_container->setZOrder(newZOrder);
    }

    void CUIAnimatedMaterial::initialise( SmartPtr<IUIElement> &parent, const Properties &properties )
    {
        // String value;
        // f32 left = 0.0f;
        // f32 top = 0.0f;
        // f32 width = 0.0f;
        // f32 height = 0.0f;
        // String alignment;

        // SmartPtr<IOverlayManager> overlayManager =
        // IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

        // m_container = overlayManager->addElement(String("Panel"), getName());

        // SmartPtr<IOverlayElementContainer> cont = overlayManager->findElement(parent->getName());
        // cont->addChild(m_container);

        // m_container->setLeft(left);
        // m_container->setTop(top);
        // m_container->setWidth(width);
        // m_container->setHeight(height);

        // m_container->setVisible(false);

        // m_position = Vector2F(left, top);
        // m_size = Vector2F(width, height);

        // int zorder = cont->getZOrder();
        // int newZOrder = zorder+parent->getChildren().size()+1;
        // m_container->setZOrder(newZOrder);
    }

    void CUIAnimatedMaterial::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
    {
        const TiXmlElement *element = pNode->ToElement();
        {
            const String id( element->Attribute( "id" ) );
            if( id.length() > 0 )
            {
                setName( id );
            }
        }

        String value;
        f32 left = 0.0f;
        f32 top = 0.0f;
        f32 width = 0.0f;
        f32 height = 0.0f;
        String alignment;

        // const String templateId(element->Attribute("template"));
        // if(templateId.length() > 0)
        //{
        //	SmartPtr<CUIItemTemplate> templateItem =
        //CGUIManager::getSingletonPtr()->findElement(templateId); 	if(!templateItem.isNull())
        //	{
        //	}
        //	else
        //	{
        //		String message = String("Could not find template: ") + String(templateId);
        //		MessageBoxUtil::show(message.c_str());
        //	}
        // }

        auto materialName = String( element->Attribute( "material" ) );
        /*const Ogre::SmartPtr<IMaterial>& material =
        Ogre::MaterialManager::getSingletonPtr()->getByName(materialName.c_str());
        if(!material.isNull())
        {
            material->load(nullptr);

            Ogre::Technique* tech = material->getBestTechnique();
            if(tech)
            {
                Ogre::Pass* pass = tech->getPass(0);
                if(pass)
                {
                    Ogre::TextureUnitState* state = pass->getTextureUnitState(0);
                    if(state)
                    {
                        if(!state->isTextureLoadFailing())
                        {
                            std::pair<size_t, size_t> dims = state->getTextureDimensions();
                            width = (f32)(dims.first) / 1920.0f;
                            height = (f32)(dims.second) / 1080.0f;
                        }
                    }
                }
            }
        }*/

        value = String( element->Attribute( "posX" ) );
        if( value.length() > 0 )
        {
            left = StringUtil::parseFloat( value );
        }

        value = String( element->Attribute( "posY" ) );
        if( value.length() > 0 )
        {
            top = StringUtil::parseFloat( value );
        }

        value = String( element->Attribute( "width" ) );
        if( value.length() > 0 )
        {
            width = StringUtil::parseFloat( value );
        }

        value = String( element->Attribute( "height" ) );
        if( value.length() > 0 )
        {
            height = StringUtil::parseFloat( value );
        }

        Vector2F scale = Vector2F::UNIT;
        value = String( element->Attribute( "scaleX" ) );
        if( value.length() > 0 )
        {
            scale.X() = StringUtil::parseFloat( value );
        }

        value = String( element->Attribute( "scaleY" ) );
        if( value.length() > 0 )
        {
            scale.Y() = StringUtil::parseFloat( value );
        }

        width *= scale.X();
        height *= scale.Y();

        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

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

        //m_container->setMaterialName( materialName );

        const String pAlignmentStr( element->Attribute( "alignment" ) );
        if( pAlignmentStr.length() > 0 )
        {
            alignment = pAlignmentStr;
        }

        // if(alignment==(String("center")))
        //{
        //	m_container->setHorizontalAlignment(Ogre::GHA_CENTER);
        //	m_container->setVerticalAlignment(Ogre::GVA_CENTER);
        // }
        // else if(alignment==(String("left")))
        //{
        //	m_container->setHorizontalAlignment(Ogre::GHA_LEFT);
        // }
        // else if(alignment==(String("right")))
        //{
        //	m_container->setHorizontalAlignment(Ogre::GHA_RIGHT);
        // }

        bool isVisible = true;
        value = String( element->Attribute( "visible" ) );
        if( value == String( "false" ) )
        {
            isVisible = false;
        }

        if( parent )
        {
            isVisible = isVisible && parent->isVisible();
        }

        setVisible( isVisible );

        auto zorder = cont->getZOrder();
        auto newZOrder = zorder + static_cast<u32>( parent->getChildren()->size() ) + 1;
        m_container->setZOrder( newZOrder );

        m_numFrames = getNumFrames();

        f32 animationTime = StringUtil::parseFloat( element->Attribute( "animationTime" ), 1.0f );
        // m_animationFunction = new AnimationFunction(animationTime, 0.0);
        // Ogre::ControllerFunctionRealPtr animationFunctionCtrl(m_animationFunction);
        // setAnimationFunction(animationFunctionCtrl);
    }

    void CUIAnimatedMaterial::update()
    {
        if( isEnabled() && isVisible() )
        {
            u32 currentFrame = getCurrentFrame();
            if( m_currentFrame != currentFrame )
            {
                m_currentFrame = currentFrame;

                OnEnterFrame();
            }
        }
    }

    void CUIAnimatedMaterial::onToggleVisibility()
    {
        m_container->setVisible( isVisible() );

        CUIElement::onToggleVisibility();
    }

    void CUIAnimatedMaterial::setMaterialName( const String &materialName )
    {
        //m_container->setMaterialName( materialName );
    }

    auto CUIAnimatedMaterial::getMaterialName() const -> String
    {
        return StringUtil::EmptyString;
    }

    void CUIAnimatedMaterial::play()
    {
        f32 time = 0.0f;
        // m_animationFunction->setTime(time);
        // m_animationFunction->setIsPlaying(true);
    }

    void CUIAnimatedMaterial::pause()
    {
        // m_animationFunction->setIsPlaying(false);
    }

    void CUIAnimatedMaterial::stop()
    {
        // m_animationFunction->setIsPlaying(false);
    }

    void CUIAnimatedMaterial::setPosition( const Vector2F &position )
    {
        //m_container->setLeft( position.X() );
        //m_container->setTop( position.Y() );
        CUIElement::setPosition( position );
    }

    void CUIAnimatedMaterial::setSize( const Vector2F &size )
    {
        //m_container->setWidth( size.X() );
        //m_container->setHeight( size.Y() );
        CUIElement::setSize( size );
    }

    // void CAnimatedMaterial::setAnimationFunction(Ogre::ControllerFunctionRealPtr func)
    //{

    // Ogre::SmartPtr<IMaterial> mat =
    // Ogre::MaterialManager::getSingleton().getByName(m_container->getMaterialName().c_str());
    // Ogre::Technique* tech = mat->getTechnique(0);
    // if(tech)
    //{
    //	Ogre::Pass* pass = tech->getPass(0);
    //	if(pass)
    //	{
    //		Ogre::TextureUnitState* textureUnit = pass->getTextureUnitState(0);
    //		if(textureUnit)
    //		{
    //			Ogre::Controller<f32>* controller = textureUnit->_getAnimController();
    //			if (controller)
    //			{
    //				controller->setFunction(func);
    //			}
    //		}
    //	}
    // }
    //}

    void CUIAnimatedMaterial::setFrameTime( f32 time )
    {
        // Ogre::SmartPtr<IMaterial> mat =
        // Ogre::MaterialManager::getSingleton().getByName(m_container->getMaterialName().c_str());
        // Ogre::Technique* tech = mat->getTechnique(0);
        // if(tech)
        //{
        //	Ogre::Pass* pass = tech->getPass(0);
        //	if(pass)
        //	{
        //		Ogre::TextureUnitState* textureUnit = pass->getTextureUnitState(0);
        //		if(textureUnit)
        //		{
        //			Ogre::Controller<f32>* controller = textureUnit->_getAnimController();
        //			if (controller)
        //			{
        //				Ogre::ControllerFunctionRealPtr func = controller->getFunction();
        //				AnimationFunction* animFunc = static_cast<AnimationFunction*>(func.get());
        //				animFunc->setTime(time);
        //			}
        //		}
        //	}
        // }
    }

    auto CUIAnimatedMaterial::getFrameTime() const -> f32
    {
        // Ogre::SmartPtr<IMaterial> mat =
        // Ogre::MaterialManager::getSingleton().getByName(m_container->getMaterialName().c_str());
        // Ogre::Technique* tech = mat->getTechnique(0);
        // if(tech)
        //{
        //	Ogre::Pass* pass = tech->getPass(0);
        //	if(pass)
        //	{
        //		Ogre::TextureUnitState* textureUnit = pass->getTextureUnitState(0);
        //		if(textureUnit)
        //		{
        //			Ogre::Controller<f32>* controller = textureUnit->_getAnimController();
        //			if (controller)
        //			{
        //				Ogre::ControllerFunctionRealPtr func = controller->getFunction();
        //				AnimationFunction* animFunc = static_cast<AnimationFunction*>(func.get());
        //				return (f32)animFunc->getTime();
        //			}
        //		}
        //	}
        // }

        return 0.f;
    }

    auto CUIAnimatedMaterial::getCurrentFrame() const -> u32
    {
        // Ogre::SmartPtr<IMaterial> mat =
        // Ogre::MaterialManager::getSingleton().getByName(m_container->getMaterialName().c_str());
        // Ogre::Technique* tech = mat->getTechnique(0);
        // if(tech)
        //{
        //	Ogre::Pass* pass = tech->getPass(0);
        //	if(pass)
        //	{
        //		Ogre::TextureUnitState* textureUnit = pass->getTextureUnitState(0);
        //		if(textureUnit)
        //		{
        //			return textureUnit->getCurrentFrame();
        //		}
        //	}
        // }

        return 0;
    }

    auto CUIAnimatedMaterial::getNumFrames() const -> u32
    {
        // Ogre::SmartPtr<IMaterial> mat =
        // Ogre::MaterialManager::getSingleton().getByName(m_container->getMaterialName().c_str());
        // Ogre::Technique* tech = mat->getTechnique(0);
        // if(tech)
        //{
        //	Ogre::Pass* pass = tech->getPass(0);
        //	if(pass)
        //	{
        //		Ogre::TextureUnitState* textureUnit = pass->getTextureUnitState(0);
        //		if(textureUnit)
        //		{
        //			return textureUnit->getNumFrames();
        //		}
        //	}
        // }

        return 0;
    }

    void CUIAnimatedMaterial::OnEnterFrame() const
    {
        //if(!m_listeners.empty())
        //{
        //    Parameters params;
        //    // params.set_used(4);
        //    params[0].setPtr( (void *)this );
        //    params[1].setU32( m_currentFrame );

        //    // for (u32 i = 0; i < m_listeners.size(); ++i)
        //    //{
        //    //	m_listeners[i]->OnEvent(StringUtil::getHash("OnEnterFrame"), params);
        //    // }
        //}
    }

    // AnimationFunction

    //   CAnimatedMaterial::AnimationFunction::AnimationFunction(f32 sequenceTime, f32 timeOffset)
    //	: ControllerFunction<f32>(false), m_isPlaying(false), m_loop(false)
    //   {
    //       mSeqTime = sequenceTime;
    //       mTime = timeOffset;
    //   }
    //
    //   f32 CAnimatedMaterial::AnimationFunction::calculate(f32 source)
    //   {
    //	if(m_isPlaying)
    //	{
    //		// Assume source is time since last update
    //		mTime += source;

    //		if(m_loop)
    //		{
    //			// Wrap
    //			while (mTime >= mSeqTime)
    //				mTime -= mSeqTime;

    //			while (mTime < 0)
    //				mTime += mSeqTime;
    //		}
    //		else
    //		{
    //			if (mTime >= mSeqTime)
    //				mTime = mSeqTime;

    //			if (mTime < 0)
    //				mTime = 0.0f;
    //		}
    //	}

    //       return mTime / mSeqTime;
    //   }
    //
    // void CAnimatedMaterial::AnimationFunction::setTime(f32 timeVal)
    //{
    //	mTime = timeVal;
    // }
    //
    // void CAnimatedMaterial::AnimationFunction::setSequenceTime(f32 seqVal)
    //{
    //	mSeqTime = seqVal;
    // }
}  // namespace fb::ui
