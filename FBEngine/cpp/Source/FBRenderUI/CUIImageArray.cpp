#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIImageArray.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {
        CUIImageArray::CUIImageArray()
        {
        }

        CUIImageArray::~CUIImageArray()
        {
        }

        void CUIImageArray::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            const TiXmlElement *element = pNode->ToElement();
            {
                const String id = XmlUtil::getString( element->Attribute( "id" ) );
                if(id.length() > 0)
                {
                    setName( id );
                }
            }

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

            // m_container = overlayManager->addElement(String("Panel"), getName());

            SmartPtr<render::IOverlayElementContainer>
                cont; // = overlayManager->findElement(parent->getName());
            cont->addChild( m_container );
        }

        SmartPtr<IUIImage> CUIImageArray::getImage( u32 index )
        {
            return nullptr;
        }

        void CUIImageArray::onAddChild( IUIElement *child )
        {
            // m_images.push_back(child);
        }

        void CUIImageArray::onRemoveChild( IUIElement *child )
        {
            // m_images.erase_element(child);
        }
    } // namespace ui
}     // end namespace fb
