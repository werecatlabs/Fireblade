#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {
        CUIItemTemplate::CUIItemTemplate()
        {
            setType( String( "GUIItemTemplate" ) );
        }

        CUIItemTemplate::~CUIItemTemplate()
        {
            removeAllChildren();
        }

        void CUIItemTemplate::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            const TiXmlElement *element = pNode->ToElement();
            FB_ASSERT_TRUE( !element );

            const String id = XMLUtil::getString( element->Attribute( "id" ) );
            if(id.length() > 0)
            {
                setName( id );
            }

            auto attribute = const_cast<TiXmlAttribute *>(element->FirstAttribute());
            while(attribute)
            {
                const String name( attribute->Name() );
                const String value( attribute->Value() );

                m_propertyGroup.addProperty( name, value );

                attribute = attribute->Next();
            }
        }

        const Properties &CUIItemTemplate::getPropertyGroup() const
        {
            return m_propertyGroup;
        }

        Properties &CUIItemTemplate::getPropertyGroup()
        {
            return m_propertyGroup;
        }
    } // namespace ui
}     // end namespace fb
