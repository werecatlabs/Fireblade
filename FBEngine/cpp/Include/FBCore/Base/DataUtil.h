#ifndef DataUtil_h__
#define DataUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Base/Data.h>

namespace fb
{
    
    /** Utility class used to convert data. */
    class DataUtil
    {
    public:
        template <class T>
        static String toString( T *ptr, bool formatted = false );

        template <class T>
        static String toString( RawPtr<T> ptr, bool formatted = false );

        template <class T>
        static String toString( SmartPtr<T> ptr, bool formatted = false );

        template <class T>
        static void parse( const String &jsonData, T *ptr );

        template <class T>
        static void parse( const String &jsonData, RawPtr<T> ptr );

        template <class T>
        static void parseXML( const String &xmlData, T *ptr );

        template <class T>
        static void parseXML( const String &xmlData, RawPtr<T> ptr );

        template <class T>
        static data::vec4 toData( const Vector3<T> &v )
        {
            FB_ASSERT( v.isValid() );
            return data::vec4( v.x, v.y, v.z, T( 0.0 ) );
        }
    };
}  // end namespace fb

#endif  // DataUtil_h__
