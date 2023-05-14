#ifndef DataUtil_h__
#define DataUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Properties.h>
//#include <FBCore/Extern/rttr/registration.h>
//#include <boost/json.hpp>

namespace boost
{
    namespace json
    {
        class object;
    }  // namespace json
}  // namespace boost

namespace rttr
{
	class instance;
}  // namespace rttr

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

        static String objectToJsonStr(const rttr::instance &instance);
    };



}  // end namespace fb

#endif  // DataUtil_h__
