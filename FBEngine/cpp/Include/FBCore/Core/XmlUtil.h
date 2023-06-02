#ifndef XmlUtil_h__
#define XmlUtil_h__

#include <FBCore/Core/Properties.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    class XmlUtil
    {
    public:
        /**
         * Loads an XML document from a file.
         *
         * @param filePath The path to the XML file to load.
         * @return A shared pointer to the loaded `TiXmlDocument` object.
         */
        static SharedPtr<TiXmlDocument> loadFile( const String &filePath );

        /**
         * Loads an XML document from a file with wide string path.
         *
         * @param filePath The wide string path to the XML file to load.
         * @return A shared pointer to the loaded `TiXmlDocument` object.
         */
        static SharedPtr<TiXmlDocument> loadFile( const StringW &filePath );

        /**
         * Retrieves the content of an XML file as a string.
         *
         * @param filePath The path to the XML file.
         * @return The content of the XML file as a string.
         */
        static String getFromFile( const String &filePath );

        /**
         * Retrieves the content of an XML file as a string with wide string path.
         *
         * @param filePath The wide string path to the XML file.
         * @return The content of the XML file as a string.
         */
        static String getFromFile( const StringW &filePath );

        /**
         * Parses an XML document from a string.
         *
         * @param xmlString The XML string to parse.
         * @return A shared pointer to the parsed `TiXmlDocument` object.
         */
        static SharedPtr<TiXmlDocument> parseDocument( const String &xmlString );

        /**
         * Retrieves the text content of an XML element with the specified child element name.
         *
         * @param parent The parent XML element.
         * @param childName The name of the child element.
         * @return The text content of the child element.
         */
        static String getText( TiXmlElement *parent, const String &childName );

        /**
         * Retrieves the text content of an XML element.
         *
         * @param element The XML element.
         * @return The text content of the element.
         */
        static String getText( TiXmlElement *element );

        /**
         * Retrieves the wide string text content of an XML element with the specified child element name.
         *
         * @param parent The parent XML element.
         * @param childName The name of the child element.
         * @return The wide string text content of the child element.
         */
        static StringW getTextW( TiXmlElement *parent, const StringW &childName );

        /**
         * Retrieves the wide string text content of an XML element.
         *
         * @param element The XML element.
         * @return The wide string text content of the element.
         */
        static StringW getTextW( TiXmlElement *element );

        /**
         * Writes the properties of a property group to an XML structure.
         *
         * @param properties The properties to write.
         * @param pCurNode The XML node to write the properties to.
         */
        static void writeProperties( const Properties &properties, TiXmlNode *pCurNode );

        /**
         * Writes the properties of a child property group to an XML structure.
         *
         * @param properties The properties to write.
         * @param pCurNode The XML node to write the child properties to.
         */
        static void writeChildProperties( const Properties &properties, TiXmlNode *pCurNode );

        /**
         * Loads properties from an XML node representing a property group.
         *
         * @param properties The loaded properties will be stored in this object.
         * @param propertyGroupNode The XML
         */
        static void loadProperties( Properties &properties, const TiXmlNode *propertyGroupNode );

        /** Gets a string from char pointer. */
        static String getString( const char *pStr );
    };
}  // end namespace fb

#endif  // XmlUtil_h__
