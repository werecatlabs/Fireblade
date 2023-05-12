#ifndef XMLUtil_h__
#define XMLUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/Properties.h>

namespace fb
{
    class XMLUtil
    {
    public:
        static SharedPtr<TiXmlDocument> loadFile( const String &filePath );
        static SharedPtr<TiXmlDocument> loadFile( const StringW &filePath );

        static String getFromFile( const String &filePath );
        static String getFromFile( const StringW &filePath );

        static SharedPtr<TiXmlDocument> parseDocument( const String &xmlString );

        static String getText( TiXmlElement *parent, const String &childName );
        static String getText( TiXmlElement *element );

        static StringW getTextW( TiXmlElement *parent, const StringW &childName );
        static StringW getTextW( TiXmlElement *element );

        /** Write the properties of a property group to an xml structure. */
        static void writeProperties( const Properties &properties, TiXmlNode *pCurNode );

        /** */
        static void writeChildProperties( const Properties &properties, TiXmlNode *pCurNode );

        /** */
        static void loadProperties( Properties &properties, const TiXmlNode *propertyGroupNode );

        static String getString( const char *pStr );
    };
}  // namespace fb

#endif  // XMLUtil_h__