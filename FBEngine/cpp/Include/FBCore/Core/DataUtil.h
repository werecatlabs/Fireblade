#ifndef DataUtil_h__
#define DataUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Properties.h>
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
        enum class Format
		{
			JSON,
			XML,
            YAML,

            COUNT
		};

        template <class T>
        static String toString( T *ptr, bool formatted = false, Format fmt = Format::JSON );

        template <class T>
        static void parse( const String &jsonDataStr, T *ptr, Format fmt = Format::JSON );

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
        static String formatJson( const String &jsonString );

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

        static String objectToJsonStr( const rttr::instance &instance, bool formatted );
    };

}  // end namespace fb

#endif  // DataUtil_h__
