#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIVector.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {
        CUIVector::CUIVector()
        {
        }

        CUIVector::~CUIVector()
        {
        }

        void CUIVector::initialise( SmartPtr<IUIElement> &parent )
        {
        }

        void CUIVector::initialise( SmartPtr<IUIElement> &parent, const Properties &properties )
        {
        }

        void CUIVector::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            const TiXmlElement *element = pNode->ToElement();
            {
                const String id = XmlUtil::getString( element->Attribute( "id" ) );
                if(id.length() > 0)
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

            // const String templateId = XmlUtil::getString(element->Attribute("template"));
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

            String materialName = XmlUtil::getString( element->Attribute( "fileName" ) );
            // const Ogre::SmartPtr<IMaterial>& material =
            // Ogre::MaterialManager::getSingletonPtr()->getByName(materialName.c_str());
            // if(!material.isNull())
            //{
            //	material->load(nullptr);

            //	Ogre::Technique* tech = material->getBestTechnique();
            //	if(tech)
            //	{
            //		Ogre::Pass* pass = tech->getPass(0);
            //		if(pass)
            //		{
            //			if(pass->getNumTextureUnitStates() > 0)
            //			{
            //				Ogre::TextureUnitState* state = pass->getTextureUnitState(0);
            //				if(state)
            //				{
            //					if(!state->isTextureLoadFailing())
            //					{
            //						std::pair<size_t, size_t> dims = state->getTextureDimensions();
            //						width = (f32)(dims.first) / 1920.0f;
            //						height = (f32)(dims.second) / 1080.0f;
            //					}
            //				}
            //			}
            //		}
            //	}

            //	material->unload();
            //}

            value = XmlUtil::getString( element->Attribute( "posX" ) );
            if(value.length() > 0)
                left = StringUtil::parseFloat( value );

            value = XmlUtil::getString( element->Attribute( "posY" ) );
            if(value.length() > 0)
                top = StringUtil::parseFloat( value );

            value = XmlUtil::getString( element->Attribute( "width" ) );
            if(value.length() > 0)
                width = StringUtil::parseFloat( value );

            value = XmlUtil::getString( element->Attribute( "height" ) );
            if(value.length() > 0)
                height = StringUtil::parseFloat( value );

            Vector2F scale = Vector2F::UNIT;
            value = XmlUtil::getString( element->Attribute( "scaleX" ) );
            if(value.length() > 0)
                scale.X() = StringUtil::parseFloat( value );

            value = XmlUtil::getString( element->Attribute( "scaleY" ) );
            if(value.length() > 0)
                scale.Y() = StringUtil::parseFloat( value );

            width *= scale.X();
            height *= scale.Y();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            if(!overlayManager)
                return;

            // m_container = overlayManager->addElement(String("VectorImage"), getName());
            if(m_container)
            {
                SmartPtr<render::IOverlayElementContainer>
                    cont; // = overlayManager->findElement(parent->getName());
                cont->addChild( m_container );

                //m_container->setLeft( left );
                //m_container->setTop( top );
                //m_container->setWidth( width );
                //m_container->setHeight( height );

                //m_position = Vector2F( left, top );
                //m_size = Vector2F( width, height );

                // m_container->setMaterialName(materialName);
                m_container->setFileName( materialName );

                const String pAlignmentStr = XmlUtil::getString( element->Attribute( "alignment" ) );
                if(pAlignmentStr.length() > 0)
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
                value = XmlUtil::getString( element->Attribute( "visible" ) );
                if(value == String( "false" ))
                    isVisible = false;

                if(parent)
                    isVisible = isVisible && parent->isVisible();

                setVisible( isVisible );

                auto zorder = cont->getZOrder();
                auto newZOrder = zorder + (u32)parent->getChildren()->size() + 1;
                m_container->setZOrder( newZOrder );
            }
        }

        String CUIVector::getFileName() const
        {
            return m_fileName;
        }

        void CUIVector::setFileName( const String &val )
        {
            m_fileName = val;
        }

        SmartPtr<render::IMaterial> CUIVector::getMaterial() const
        {
            return nullptr;
        }

        void CUIVector::setMaterial( SmartPtr<render::IMaterial> val )
        {
        }
    } // end namespace ui
}     // end namespace fb
