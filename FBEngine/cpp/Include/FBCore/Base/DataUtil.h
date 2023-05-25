#ifndef DataUtil_h__
#define DataUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Math/Transform3.h>

namespace boost
{
    namespace json
    {
        class object;
        class value;
    }  // namespace json
}  // namespace boost

namespace fb
{

    /** Utility class used to convert data. */
    class DataUtil
    {
    public:
        template <class T>
        static String toString( T *ptr, bool formatted = false );

        template <class T>
        static void parse( const String &jsonDataStr, T *ptr );

        template <class T>
        static void parseXML( const String &xmlData, T *ptr );

        /** Parses a JSON string into a Properties object. */
        static SmartPtr<Properties> parseJson( const String &jsonDataStr );

        /** Parses a JSON string into a Properties object. Expects the JSON string to for a properties object.
         */
        static SmartPtr<Properties> parsePropertiesFromJson( const String &jsonDataStr );

        static void parse( SmartPtr<Properties> properties, Vector2F &value );

        static void parse( SmartPtr<Properties> properties, Vector3F &value );

        static void parse( SmartPtr<Properties> properties, Vector4F &value );

        static void parse( SmartPtr<Properties> properties, QuaternionF &value );

        static void parse( SmartPtr<Properties> properties, ColourF &value );

    private:
        static void parseJson( const boost::json::object *val, Vector2I *ptr );
        static void parseJson( const boost::json::object *val, Vector2F *ptr );
        static void parseJson( const boost::json::object *val, Vector2D *ptr );
        static void parseJson( const boost::json::object *val, Vector3I *ptr );
        static void parseJson( const boost::json::object *val, Vector3F *ptr );
        static void parseJson( const boost::json::object *val, Vector3D *ptr );

        static void parseJson( const boost::json::object *val, Transform3F *ptr );
        static void parseJson( const boost::json::object *val, Transform3D *ptr );

        static void parseJson( const boost::json::object *val, Properties *ptr );
        static void parseJson( const boost::json::object *val, Property *ptr );

        static String objectToJsonStr( const rttr::instance &instance );
    };

}  // end namespace fb

#endif  // DataUtil_h__
