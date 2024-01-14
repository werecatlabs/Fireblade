#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/XmlUtil.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include "FBCore/Memory/PointerUtil.h"
#include <tinyxml.h>

namespace fb
{

    auto XmlUtil::loadFile( const String &filePath ) -> SharedPtr<TiXmlDocument>
    {
        auto doc = fb::make_shared<TiXmlDocument>();
        doc->LoadFile( filePath.c_str() );
        return doc;
    }

    auto XmlUtil::loadFile( const StringW &filePath ) -> SharedPtr<TiXmlDocument>
    {
        auto doc = fb::make_shared<TiXmlDocument>();

#if defined FB_PLATFORM_WIN32
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();
        auto stream = fileSystem->openW( filePath );
        if( stream->isOpen() )
        {
            auto dataStr = stream->getAsString();
            doc->Parse( dataStr.c_str() );
        }
#endif

        return doc;
    }

    auto XmlUtil::getFromFile( const String &filePath ) -> String
    {
        auto doc = loadFile( filePath );

        TiXmlPrinter printer;
        printer.SetIndent( "	" );

        doc->Accept( &printer );
        const char *str = printer.CStr();
        String xmltext = str ? str : "";

        return xmltext;
    }

    auto XmlUtil::getFromFile( const StringW &filePath ) -> String
    {
        auto doc = loadFile( filePath );

        TiXmlPrinter printer;
        printer.SetIndent( "	" );

        doc->Accept( &printer );
        const char *str = printer.CStr();
        String xmltext = str ? str : "";

        return xmltext;
    }

    auto XmlUtil::parseDocument( const String &xmlString ) -> SharedPtr<TiXmlDocument>
    {
        auto doc = fb::make_shared<TiXmlDocument>();
        doc->Parse( xmlString.c_str() );
        if( doc->Error() )
        {
            String msg = String( "Error: could not parse document: " ) + xmlString;
            FB_LOG( msg.c_str() );
        }

        return doc;
    }

    auto XmlUtil::getText( TiXmlElement *parent, const String &childName ) -> String
    {
        TiXmlElement *element = parent->FirstChildElement( childName.c_str() );
        return getText( element );
    }

    auto XmlUtil::getText( TiXmlElement *element ) -> String
    {
        return ( element && element->GetText() ) ? element->GetText() : "";
    }

    auto XmlUtil::getTextW( TiXmlElement *parent, const StringW &childName ) -> StringW
    {
        TiXmlElement *element = parent->FirstChildElement( StringUtil::toStringC( childName ).c_str() );
        return getTextW( element );
    }

    auto XmlUtil::getTextW( TiXmlElement *element ) -> StringW
    {
        return ( element && element->GetText() ) ? StringUtil::toStringW( element->GetText() ) : L"";
    }

    void XmlUtil::writeProperties( const Properties &propertyGroup, TiXmlNode *curNode )
    {
        const auto properties = propertyGroup.getPropertiesAsArray();

        // loop though the objects keys
        for( const auto &property : properties )
        {
            auto keyNode = new TiXmlElement( property.getName().c_str() );
            curNode->LinkEndChild( keyNode );

            // keyNode->SetAttribute("Type", StringUtil::getPointer(property.getType()));
            // keyNode->SetAttribute("label", StringUtil::getPointer(property.getLabel()));
            // keyNode->SetAttribute("readOnly",
            // StringUtil::getPointer(StringUtil::toString(property.isReadOnly())));

            ////key Value
            // String valueStr = property.getValue();
            // TiXmlText* keyValueNode = new TiXmlText(StringUtil::getPointer(property.getValue()));

            // keyNode->LinkEndChild(keyValueNode);
        }
    }

    void XmlUtil::writeChildProperties( const Properties &propertyGroup, TiXmlNode *pCurNode )
    {
        // write child properties
        u32 numChildren = propertyGroup.getNumChildren();
        for( u32 i = 0; i < numChildren; ++i )
        {
            const SmartPtr<Properties> &curPropertyGroup = propertyGroup.getChild( i );

            auto propGroupNameNode = new TiXmlElement( "PropertyGroup" );
            pCurNode->LinkEndChild( propGroupNameNode );

            propGroupNameNode->SetAttribute( "Name", curPropertyGroup->getName().c_str() );

            writeProperties( *curPropertyGroup, propGroupNameNode );
        }
    }

    void XmlUtil::loadProperties( Properties &propertyGroup, const TiXmlNode *propertyGroupNode )
    {
        if( !propertyGroupNode )
        {
            return;
        }

        String propertyType;
        String defaultPropertyType( "string" );
        String propertyLabel;
        String propertyIsReadOnly;

        const TiXmlElement *propertyGroupElem = propertyGroupNode->ToElement();
        if( propertyGroupElem )
        {
            const String propertyGroupName = getString( propertyGroupElem->Attribute( "Name" ) );
            propertyGroup.setName( propertyGroupName );
        }

        const TiXmlNode *propertyNode = propertyGroupNode->FirstChild();
        while( propertyNode )
        {
            Property property;
            property.setName( getString( propertyNode->Value() ) );

            const TiXmlNode *propertyValueNode = propertyNode->FirstChild();
            if( propertyValueNode )
            {
                property.setValue( getString( propertyValueNode->Value() ) );
            }

            const TiXmlElement *propertyElem = propertyNode->ToElement();
            if( propertyElem )
            {
                propertyType = getString( propertyElem->Attribute( "Type" ) );
                if( propertyType.length() == 0 )
                {
                    property.setType( defaultPropertyType );
                }
                else
                {
                    property.setType( propertyType );
                }

                propertyLabel = getString( propertyElem->Attribute( "label" ) );
                if( propertyLabel.length() != 0 )
                {
                    property.setLabel( propertyLabel );
                }

                propertyIsReadOnly = getString( propertyElem->Attribute( "readOnly" ) );
                if( propertyIsReadOnly.length() != 0 )
                {
                    property.setReadOnly( StringUtil::parseBool( propertyIsReadOnly, false ) );
                }
            }

            propertyGroup.addProperty( property );
            propertyNode = propertyNode->NextSibling();
        }
    }

    auto XmlUtil::getString( const char *pStr ) -> String
    {
        return pStr ? pStr : "";
    }
}  // namespace fb
