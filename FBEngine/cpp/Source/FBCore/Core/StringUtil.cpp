#include <FBCore/Core/ColourUtil.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/FBCorePCH.h>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <codecvt>
#include <ctime>
#include <functional>
#include <locale>
#include <sstream>
#include <string>
#include <utf8.h>
#include <vector>

#include "FBCore/Core/Path.h"

#ifdef FB_USE_BOOST
#    include <boost/uuid/uuid.hpp>             // uuid class
#    include <boost/uuid/uuid_generators.hpp>  // generators
#    include <boost/uuid/uuid_io.hpp>          // streaming operators etc.

#    include <boost/filesystem/operations.hpp>

#    include <boost/archive/iterators/binary_from_base64.hpp>
#    include <boost/archive/iterators/base64_from_binary.hpp>
#    include <boost/archive/iterators/insert_linebreaks.hpp>
#    include <boost/archive/iterators/transform_width.hpp>
#    include <boost/archive/iterators/ostream_iterator.hpp>
#    include <boost/date_time/gregorian/gregorian.hpp>
#    include <boost/date_time/gregorian/formatters.hpp>
#    include <boost/date_time/posix_time/ptime.hpp>
#    include <boost/date_time/posix_time/posix_time_types.hpp>
#    include <boost/date_time/posix_time/time_formatters.hpp>
#endif

enum
{
    MAX_BUFFER_SIZE = 256
};

namespace fb
{
    static const String base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    static const String base64_padding[] = { "", "==", "=" };

    template <>
    const String StringUtility<String>::EmptyString = "";

    template <>
    const StringW StringUtility<StringW>::EmptyString = L"";

    template <>
    auto StringUtility<String>::default_dilim() -> String
    {
        return "\t\n ";
    }

    template <>
    auto StringUtility<StringW>::default_dilim() -> StringW
    {
        return L"\t\n ";
    }

    template <>
    auto StringUtility<String>::blank() -> String
    {
        return "";
    }

    template <>
    auto StringUtility<StringW>::blank() -> StringW
    {
        return L"";
    }

    template <class T>
    auto StringUtility<T>::isEqual( const T &a, const T &b ) -> bool
    {
        return std::equal( a.begin(), a.end(), b.begin(), b.end(),
                           []( typename T::value_type a, typename T::value_type b ) {
                               return tolower( a ) == tolower( b );
                           } );
    }

    template <>
    auto StringUtility<std::string>::isNullOrEmpty( const std::string &str ) -> bool
    {
        return str.empty() || str == "";
    }

    template <>
    auto StringUtility<StringW>::isNullOrEmpty( const StringW &str ) -> bool
    {
        return str.empty() || str == L"";
    }

    template <class T>
    auto StringUtility<T>::contains( const T &str, const T &value ) -> bool
    {
        return str.find( value ) != T::npos;
    }

    template <>
    auto StringUtility<String>::toString( bool value ) -> String
    {
        if( value )
        {
            return String( "true" );
        }

        return String( "false" );
    }

    template <>
    auto StringUtility<StringW>::toString( bool value ) -> StringW
    {
        if( value )
        {
            return StringW( L"true" );
        }

        return StringW( L"false" );
    }

    template <>
    auto StringUtility<String>::parseBool( const String &value, bool defaultValue ) -> bool
    {
        if( isNullOrEmpty( value ) )
        {
            return defaultValue;
        }

        static const auto trueStr = String( "true" );
        static const auto oneStr = String( "1" );
        static const auto yesStr = String( "yes" );

        return value == trueStr || value == oneStr || value == yesStr;
    }

    template <>
    auto StringUtility<StringW>::parseBool( const StringW &value, bool defaultValue ) -> bool
    {
        if( isNullOrEmpty( value ) )
        {
            return defaultValue;
        }

        static const auto trueStr = StringW( L"true" );
        static const auto oneStr = StringW( L"1" );
        static const auto yesStr = StringW( L"yes" );

        return value == trueStr || value == oneStr || value == yesStr;
    }

    template <class T>
    auto StringUtility<T>::toString( s32 value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::toString( s64 value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::parseInt( const String &value, s32 defaultValue ) -> s32
    {
        if( value.length() > 0 )
        {
            s32 intVal = defaultValue;
            int retVal = sscanf( value.c_str(), "%i", &intVal );
            if( retVal != -1 )
            {
                return intVal;
            }
        }

        return defaultValue;
    }

    template <class T>
    auto StringUtility<T>::toString( u32 value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

#if defined FB_PLATFORM_WIN32
    template <class T>
    auto StringUtility<T>::toString( u64 value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::toString( unsigned long int value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }
#elif defined FB_PLATFORM_APPLE
    template <class T>
    String StringUtility<T>::toString( u64 value )
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    template <class T>
    String StringUtility<T>::toString( unsigned long int value )
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }
#elif defined FB_PLATFORM_LINUX
    template <class T>
    String StringUtility<T>::toString( u64 value )
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }
#endif

    template <class T>
    auto StringUtility<T>::parseUInt( const String &value, u32 defaultValue ) -> u32
    {
        if( value.length() > 0 )
        {
            s32 intVal = 0;
            int retVal = sscanf( value.c_str(), "%i", &intVal );
            if( retVal )
            {
                return static_cast<u32>( intVal );
            }
        }

        return defaultValue;
    }

    template <class T>
    auto StringUtility<T>::toString( f32 value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::parseFloat( const T &value, f32 defaultValue ) -> f32
    {
        if( !isNullOrEmpty( value ) )
        {
            std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                    std::allocator<typename T::value_type>>
                str( value );
            auto ret = defaultValue;
            if( !( str >> ret ) )
            {
                return defaultValue;
            }

            if( std::isfinite( ret ) )
            {
                return ret;
            }
        }

        return defaultValue;
    }

    template <class T>
    auto StringUtility<T>::toString( f64 value ) -> String
    {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::parseDouble( const T &value, f64 defaultValue ) -> f64
    {
        if( !isNullOrEmpty( value ) )
        {
            std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                    std::allocator<typename T::value_type>>
                str( value );
            auto ret = defaultValue;
            if( !( str >> ret ) )
            {
                return defaultValue;
            }

            if( std::isfinite( ret ) )
            {
                return ret;
            }
        }

        return defaultValue;
    }

    template <class T>
    auto StringUtility<T>::toString( const ColourI &colour ) -> String
    {
        std::stringstream stream;
        stream << colour.getRed() << ", " << colour.getGreen() << ", " << colour.getBlue() << ", "
               << colour.getAlpha() << std::endl;

        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::toString( const ColourF &colour ) -> String
    {
        std::stringstream stream;
        stream << colour.r << ", " << colour.g << ", " << colour.b << ", " << colour.a << std::endl;
        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::parseColour( const String &value ) -> ColourI
    {
        s32 r, g, b, a;
        sscanf( value.c_str(), "%i, %i, %i, %i", &r, &g, &b, &a );

        return { (u32)a, (u32)r, (u32)g, (u32)b };
    }

    template <class T>
    auto StringUtility<T>::parseColourf( const String &value ) -> ColourF
    {
        f32 r, g, b, a;
        sscanf( value.c_str(), "%f, %f, %f, %f", &r, &g, &b, &a );

        return { r, g, b, a };
    }

    template <>
    void StringUtility<String>::parseArray( const String &formatedString, Array<String> &stringArray )
    {
        auto connectedToStr = formatedString;

        auto curCharIdx = 0;
        auto charIdx = connectedToStr.find( ';' );

        while( charIdx != -1 )
        {
            String substr = connectedToStr.substr( curCharIdx, charIdx );
            connectedToStr = connectedToStr.substr( charIdx + 1, connectedToStr.length() );
            stringArray.push_back( substr );
            charIdx = connectedToStr.find( ';' );
        }
    }

    template <>
    void StringUtility<StringW>::parseArray( const StringW &formatedString, Array<StringW> &stringArray )
    {
        auto connectedToStr = formatedString;

        auto curCharIdx = 0;
        auto charIdx = connectedToStr.find( ';' );

        while( charIdx != -1 )
        {
            auto substr = connectedToStr.substr( curCharIdx, charIdx );
            connectedToStr = connectedToStr.substr( charIdx + 1, connectedToStr.length() );
            stringArray.push_back( substr );
            charIdx = connectedToStr.find( ';' );
        }
    }

    template <class T>
    void StringUtility<T>::parseArray( const T &formatedString, Array<f32> &floatArray )
    {
        auto connectedToStr = formatedString;

        auto curCharIdx = 0;
        auto charIdx = connectedToStr.find( ';' );

        while( charIdx != -1 )
        {
            auto substr = connectedToStr.substr( curCharIdx, charIdx );
            connectedToStr = connectedToStr.substr( charIdx + 1, connectedToStr.length() );
            floatArray.push_back( StringUtility<T>::parseFloat( substr ) );
            charIdx = connectedToStr.find( ';' );
        }
    }

    template <class T>
    auto StringUtility<T>::toString( const Array<String> &stringArray ) -> String
    {
        std::stringstream stream;
        for( const auto &e : stringArray )
        {
            stream << e;
            stream << String( ";" );
        }

        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::toString( const Array<f32> &floatArray ) -> String
    {
        std::stringstream stream;

        for( const auto &f : floatArray )
        {
            stream << StringUtility<T>::toString( f );
            stream << String( ";" );
        }

        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::toString( const Array<String> &stringArray1,
                                     const Array<String> &stringArray2 ) -> String
    {
        std::stringstream stream;

        for( const auto &e : stringArray1 )
        {
            stream << e;
            stream << String( ";" );
        }

        stream << String( "-" );

        for( const auto &e : stringArray2 )
        {
            stream << e;
            stream << String( ";" );
        }

        return stream.str();
    }

    template <class T>
    auto StringUtility<T>::toString( const StringW &str ) -> String
    {
        auto cBuffer = new char[str.length() + 1];
        wcstombs( cBuffer, str.c_str(), str.length() );
        cBuffer[str.length()] = 0;

        std::string cstr = cBuffer;
        delete[] cBuffer;
        return cstr;
    }

    template <class T>
    void StringUtility<T>::parseMultiChoice( const String &formatedString, Array<String> &stringArray1,
                                             Array<String> &stringArray2 )
    {
        auto tempStr = formatedString;

        auto curCharIdx = 0;
        auto charIdx = tempStr.find( ';' );

        while( charIdx != -1 )
        {
            auto substr = tempStr.substr( curCharIdx, charIdx );
            tempStr = tempStr.substr( charIdx + 1, tempStr.length() );
            stringArray1.push_back( substr );
            charIdx = tempStr.find( ';' );

            if( tempStr[0] == '-' )
            {
                curCharIdx = curCharIdx + 1;
                charIdx = charIdx - 1;
                break;
            }
        }

        while( charIdx != -1 )
        {
            auto substr = tempStr.substr( curCharIdx, charIdx );
            tempStr = tempStr.substr( charIdx + 1, tempStr.length() );
            stringArray2.push_back( substr );
            charIdx = tempStr.find( ';' );

            if( tempStr[0] == ';' )
            {
                break;
            }
        }
    }

    template <class T>
    auto StringUtility<T>::getHash( const String &str ) -> hash_type
    {
        // DJB Hash Function
        hash_type hash = 5381;

        for( char i : str )
        {
            hash = ( ( hash << 5 ) + hash ) + i; /* hash * 33 + c */
        }

        return hash;
    }

    template <class T>
    auto StringUtility<T>::getHashMakeLower( const String &str ) -> hash_type
    {
        auto lowerStr = make_lower( str );
        return getHash( lowerStr );
    }

    template <class T>
    auto StringUtility<T>::getHash32( const String &str ) -> hash32
    {
        // DJB Hash Function
        hash32 hash = 5381;

        for( char i : str )
        {
            hash = ( ( hash << 5 ) + hash ) + i; /* hash * 33 + c */
        }

        return hash;
    }

    template <class T>
    auto StringUtility<T>::getHashMakeLower32( const String &str ) -> hash32
    {
        auto lowerStr = make_lower( str );
        return getHash32( lowerStr );
    }

    template <class T>
    auto StringUtility<T>::getHash64( const String &str ) -> hash64
    {
        // DJB Hash Function
        hash64 hash = 5381;

        for( char i : str )
        {
            hash = ( ( hash << 5 ) + hash ) + i; /* hash * 33 + c */
        }

        return hash;
    }

    template <class T>
    auto StringUtility<T>::getHashMakeLower64( const String &str ) -> hash64
    {
        auto lowerStr = make_lower( str );
        return getHash64( lowerStr );
    }

    template <class T>
    auto StringUtility<T>::getWString( const String &str ) -> StringW
    {
        if( str.length() > 0 )
        {
            StringW wstr;
            auto origsize = str.length() + 1;
            wstr.resize( origsize );

            auto ret = mbstowcs( &wstr[0], str.c_str(), origsize );
            if( ret == -1 )
            {
                return StringW( L"" );
            }

            return wstr;
        }

        return StringW( L"" );
    }

    template <class T>
    auto StringUtility<T>::ltrim( const String &str ) -> std::string
    {
        std::string s = str;
        s.erase( s.begin(), std::find_if( s.begin(), s.end(),
                                          []( unsigned char ch ) { return !std::isspace( ch ); } ) );

        return s;
    }

    template <class T>
    auto StringUtility<T>::rtrim( const String &str ) -> std::string
    {
        std::string s = str;
        s.erase(
            std::find_if( s.rbegin(), s.rend(), []( unsigned char ch ) { return !std::isspace( ch ); } )
                .base(),
            s.end() );

        return s;
    }

    template <class T>
    auto StringUtility<T>::trim( const String &s ) -> std::string
    {
        return ltrim( rtrim( s ) );
    }

    template <class T>
    auto StringUtility<T>::make_lower( const String &str ) -> String
    {
        String temp = str;
        std::transform( temp.begin(), temp.end(), temp.begin(), tolower );
        return temp;
    }

    template <class T>
    auto StringUtility<T>::make_upper( const String &str ) -> String
    {
        String temp = str;
        std::transform( temp.begin(), temp.end(), temp.begin(), toupper );
        return temp;
    }

    template <class T>
    auto StringUtility<T>::replace( const String &str, const char toReplace, const char replaceWith )
        -> String
    {
        String temp = str;
        for( char &i : temp )
        {
            if( i == toReplace )
            {
                i = replaceWith;
            }
        }

        return temp;
    }

    template <class T>
    auto StringUtility<T>::toUTF8to16( const std::string &str ) -> StringW
    {
        StringW utf16line;
        utf8::utf8to16( str.begin(), str.end(), std::back_inserter( utf16line ) );
        return utf16line;
    }

    template <class T>
    auto StringUtility<T>::toUTF16to8( const StringW &str ) -> std::string
    {
        std::string utf8line;
        utf8::utf16to8( str.begin(), str.end(), std::back_inserter( utf8line ) );
        return utf8line;
    }

    template <class T>
    auto StringUtility<T>::replaceAll( const T &source, const T &replaceWhat, const T &replaceWithWhat )
        -> const T
    {
        T result = source;
        typename T::size_type pos = 0;
        while( true )
        {
            pos = result.find( replaceWhat, pos );
            if( pos == T::npos )
            {
                break;
            }
            result.replace( pos, replaceWhat.size(), replaceWithWhat );
            pos += replaceWithWhat.size();
        }
        return result;
    }

    template <>
    auto StringUtility<std::string>::getCurrentTime( bool bDisplayStr ) -> std::string
    {
#if defined FB_PLATFORM_WIN32
        char tmpbuf[128];
        time_t ltime;
        tm today;

        _tzset();
        _time64( &ltime );
        localtime_s( &today, &ltime );

        if( !bDisplayStr )
        {
            strftime( tmpbuf, 128, "%H_%M_%S", &today );
        }
        else
        {
            strftime( tmpbuf, 128, "%H:%M:%S", &today );
        }

        return std::string( tmpbuf );
#else
        return "";
#endif
    }

    template <>
    auto StringUtility<StringW>::getCurrentTime( bool bDisplayStr ) -> StringW
    {
#if defined FB_PLATFORM_WIN32
        char tmpbuf[128];
        time_t ltime;
        tm today;

        _tzset();
        _time64( &ltime );
        localtime_s( &today, &ltime );

        if( !bDisplayStr )
        {
            strftime( tmpbuf, 128, "%H_%M_%S", &today );
        }
        else
        {
            strftime( tmpbuf, 128, "%H:%M:%S", &today );
        }

        return StringUtility<std::string>::toUTF8to16( tmpbuf );
#else
        return L"";
#endif
    }

    template <>
    auto StringUtility<std::string>::getCurrentDateTime( bool bDisplayStr, bool sqlLiteFormat )
        -> std::string
    {
#if defined FB_PLATFORM_WIN32
        char tmpbuf[128];
        time_t ltime;
        tm today;

        _tzset();
        _time64( &ltime );
        localtime_s( &today, &ltime );

        if( sqlLiteFormat )
        {
            strftime( tmpbuf, 128, "%Y-%m-%d %H:%M:%S", &today );
        }
        else
        {
            if( !bDisplayStr )
            {
                strftime( tmpbuf, 128, "%Y_%m_%d_%H_%M_%S", &today );
            }
            else
            {
                strftime( tmpbuf, 128, "%Y/%m/%d %H:%M:%S", &today );
            }
        }

        return std::string( tmpbuf );
#else
        return "";
#endif
    }

    template <>
    auto StringUtility<StringW>::getCurrentDateTime( bool bDisplayStr, bool sqlLiteFormat ) -> StringW
    {
#if defined FB_PLATFORM_WIN32
        char tmpbuf[128];
        time_t ltime;
        tm today;

        _tzset();
        _time64( &ltime );
        localtime_s( &today, &ltime );

        if( sqlLiteFormat )
        {
            strftime( tmpbuf, 128, "%Y-%m-%d %H:%M:%S", &today );
        }
        else
        {
            if( !bDisplayStr )
            {
                strftime( tmpbuf, 128, "%Y_%m_%d_%H_%M_%S", &today );
            }
            else
            {
                strftime( tmpbuf, 128, "%Y/%m/%d %H:%M:%S", &today );
            }
        }

        return StringUtility<std::string>::toUTF8to16( tmpbuf );
#else
        return L"";
#endif
    }

    template <>
    auto StringUtility<std::string>::getCurrentTime() -> std::string
    {
        using boost::gregorian::day_clock;
        using boost::posix_time::ptime;
        using boost::posix_time::second_clock;
        using boost::posix_time::to_simple_string;

        ptime todayUtc( day_clock::universal_day(), second_clock::universal_time().time_of_day() );
        return to_simple_string( todayUtc );
    }

    template <>
    auto StringUtility<StringW>::getCurrentTime() -> StringW
    {
        using boost::gregorian::day_clock;
        using boost::posix_time::ptime;
        using boost::posix_time::second_clock;
        using boost::posix_time::to_simple_string;

        ptime todayUtc( day_clock::universal_day(), second_clock::universal_time().time_of_day() );
        return StringUtil::toUTF8to16( to_simple_string( todayUtc ) );
    }

    template <>
    auto StringUtility<std::string>::getCurrentDateTime() -> std::string
    {
        namespace bg = boost::gregorian;

        static const auto fmt = "%Y%m%d";
        std::ostringstream ss;
        // assumes std::cout's locale has been set appropriately for the entire app
        ss.imbue( std::locale( std::cout.getloc(), new bg::date_facet( fmt ) ) );
        ss << bg::day_clock::universal_day();
        return ss.str();
    }

    template <>
    auto StringUtility<StringW>::getCurrentDateTime() -> StringW
    {
        namespace bg = boost::gregorian;

        static const auto fmt = "%Y%m%d";
        std::ostringstream ss;
        // assumes std::cout's locale has been set appropriately for the entire app
        ss.imbue( std::locale( std::cout.getloc(), new bg::date_facet( fmt ) ) );
        ss << bg::day_clock::universal_day();
        return StringUtil::toUTF8to16( ss.str() );
    }

    template <>
    auto StringUtility<std::string>::getCurrentDateTime( bool bDisplayStr ) -> std::string
    {
#if defined FB_PLATFORM_WIN32
        char tmpbuf[128];
        time_t ltime;
        tm today;

        _tzset();
        _time64( &ltime );
        localtime_s( &today, &ltime );

        if( !bDisplayStr )
        {
            strftime( tmpbuf, 128, "%H_%M_%S", &today );
        }
        else
        {
            strftime( tmpbuf, 128, "%H:%M:%S", &today );
        }

        return std::string( tmpbuf );
#else
        return "";
#endif
    }

    template <>
    auto StringUtility<StringW>::getCurrentDateTime( bool bDisplayStr ) -> StringW
    {
#if defined FB_PLATFORM_WIN32
        char tmpbuf[128];
        time_t ltime;
        tm today;

        _tzset();
        _time64( &ltime );
        localtime_s( &today, &ltime );

        if( !bDisplayStr )
        {
            strftime( tmpbuf, 128, "%H_%M_%S", &today );
        }
        else
        {
            strftime( tmpbuf, 128, "%H:%M:%S", &today );
        }

        return StringUtility<std::string>::toStringW( tmpbuf );
#else
        return L"";
#endif
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::toString( const Vector2<B> &val ) -> T
    {
        std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                std::allocator<typename T::value_type>>
            stream;
        stream << val.X() << " " << val.Y();
        return stream.str();
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::toString( const Vector3<B> &val ) -> T
    {
        std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                std::allocator<typename T::value_type>>
            stream;
        stream << val.X() << " " << val.Y() << " " << val.Z();
        return stream.str();
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::toString( const Vector4<B> &val ) -> T
    {
        std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                std::allocator<typename T::value_type>>
            stream;
        stream << val.X() << " " << val.Y() << " " << val.Z() << " " << val.W();
        return stream.str();
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::toString( const Quaternion<B> &val ) -> T
    {
        std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                std::allocator<typename T::value_type>>
            stream;
        stream << val.X() << " " << val.Y() << " " << val.Z() << " " << val.W();
        return stream.str();
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::parseVector2( const T &val, const Vector2<B> &defaultValue ) -> Vector2<B>
    {
        // Split on space
        auto vec = StringUtility::split( val );
        if( vec.size() != 2 )
        {
            return defaultValue;
        }

        return Vector2<B>( static_cast<B>( parseDouble( vec[0] ) ),
                           static_cast<B>( parseDouble( vec[1] ) ) );
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::parseVector3( const T &val, const Vector3<B> &defaultValue ) -> Vector3<B>
    {
        // Split on space
        auto vec = StringUtility::split( val );
        if( vec.size() != 3 )
        {
            return defaultValue;
        }

        return Vector3<B>( static_cast<B>( parseDouble( vec[0] ) ),
                           static_cast<B>( parseDouble( vec[1] ) ),
                           static_cast<B>( parseDouble( vec[2] ) ) );
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::parseVector3( const T &val, const T &split, const Vector3<B> &defaultValue )
        -> Vector3<B>
    {
        // Split on space
        auto vec = StringUtility::split( val, split );
        if( vec.size() != 3 )
        {
            return defaultValue;
        }

        return Vector3<B>( static_cast<B>( parseDouble( vec[0] ) ),
                           static_cast<B>( parseDouble( vec[1] ) ),
                           static_cast<B>( parseDouble( vec[2] ) ) );
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::parseVector4( const T &val, const Vector4<B> &defaultValue ) -> Vector4<B>
    {
        // Split on space
        auto vec = StringUtility::split( val );
        if( vec.size() != 4 )
        {
            return defaultValue;
        }

        return Vector4<B>(
            static_cast<B>( parseDouble( vec[0] ) ), static_cast<B>( parseDouble( vec[1] ) ),
            static_cast<B>( parseDouble( vec[2] ) ), static_cast<B>( parseDouble( vec[3] ) ) );
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::parseVector4( const T &val, const T &split, const Vector4<B> &defaultValue )
        -> Vector4<B>
    {
        // Split on space
        auto vec = StringUtility::split( val, split );
        if( vec.size() != 4 )
        {
            return defaultValue;
        }

        return Vector4<B>(
            static_cast<B>( parseDouble( vec[0] ) ), static_cast<B>( parseDouble( vec[1] ) ),
            static_cast<B>( parseDouble( vec[2] ) ), static_cast<B>( parseDouble( vec[3] ) ) );
    }

    template <class T>
    template <class B>
    auto StringUtility<T>::parseQuaternion( const T &val, const Quaternion<B> &defaultValue )
        -> Quaternion<B>
    {
        // Split on space
        auto vec = StringUtility::split( val );
        if( vec.size() != 4 )
        {
            return defaultValue;
        }

        return Quaternion<B>(
            static_cast<B>( parseDouble( vec[3] ) ), static_cast<B>( parseDouble( vec[0] ) ),
            static_cast<B>( parseDouble( vec[1] ) ), static_cast<B>( parseDouble( vec[2] ) ) );
    }

    template <class T>
    auto StringUtility<T>::split( const T &str, const T &delims, unsigned int maxSplits,
                                  bool preserveDelims ) -> Array<T>
    {
        Array<T> ret;
        // Pre-allocate some space for performance
        ret.reserve( maxSplits ? maxSplits + 1 : 10 );  // 10 is guessed capacity for most case

        unsigned int numSplits = 0;

        // Use STL methods
        size_t start, pos;
        start = 0;
        do
        {
            pos = str.find_first_of( delims, start );
            if( pos == start )
            {
                // Do nothing
                start = pos + 1;
            }
            else if( pos == T::npos || ( maxSplits && numSplits == maxSplits ) )
            {
                // Copy the rest of the string
                ret.push_back( str.substr( start ) );
                break;
            }
            else
            {
                // Copy up to delimiter
                ret.push_back( str.substr( start, pos - start ) );

                if( preserveDelims )
                {
                    // Sometimes there could be more than one delimiter in a row.
                    // Loop until we don't find any more delims
                    size_t delimStart = pos, delimPos;
                    delimPos = str.find_first_not_of( delims, delimStart );
                    if( delimPos == T::npos )
                    {
                        // Copy the rest of the string
                        ret.push_back( str.substr( delimStart ) );
                    }
                    else
                    {
                        ret.push_back( str.substr( delimStart, delimPos - delimStart ) );
                    }
                }

                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of( delims, start );
            ++numSplits;
        } while( pos != T::npos );

        return ret;
    }

    template <class T>
    auto StringUtility<T>::encodeBase64( const unsigned char *bytes_to_encode, size_t in_len )
        -> std::string
    {
#if !FB_USE_BOOST
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while( in_len-- )
        {
            char_array_3[i++] = *( bytes_to_encode++ );
            if( i == 3 )
            {
                char_array_4[0] = ( char_array_3[0] & 0xfc ) >> 2;
                char_array_4[1] =
                    ( ( char_array_3[0] & 0x03 ) << 4 ) + ( ( char_array_3[1] & 0xf0 ) >> 4 );
                char_array_4[2] =
                    ( ( char_array_3[1] & 0x0f ) << 2 ) + ( ( char_array_3[2] & 0xc0 ) >> 6 );
                char_array_4[3] = char_array_3[2] & 0x3f;

                for( i = 0; ( i < 4 ); i++ )
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if( i )
        {
            for( j = i; j < 3; j++ )
                char_array_3[j] = '\0';

            char_array_4[0] = ( char_array_3[0] & 0xfc ) >> 2;
            char_array_4[1] = ( ( char_array_3[0] & 0x03 ) << 4 ) + ( ( char_array_3[1] & 0xf0 ) >> 4 );
            char_array_4[2] = ( ( char_array_3[1] & 0x0f ) << 2 ) + ( ( char_array_3[2] & 0xc0 ) >> 6 );
            char_array_4[3] = char_array_3[2] & 0x3f;

            for( j = 0; ( j < i + 1 ); j++ )
                ret += base64_chars[char_array_4[j]];

            while( ( i++ < 3 ) )
                ret += '=';
        }

        return ret;
#else
        using namespace boost::archive::iterators;

        std::stringstream os;
        using base64_text = base64_from_binary<transform_width<const char *, 6, 8>>;
        std::copy( base64_text( bytes_to_encode ), base64_text( bytes_to_encode + in_len ),
                   ostream_iterator<char>( os ) );
        os << base64_padding[in_len % 3];
        return os.str();
#endif
    }

    template <class T>
    auto StringUtility<T>::decodeBase64( const std::string &s ) -> std::string
    {
#if !FB_USE_BOOST
        int in_len = s.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while( in_len-- && ( s[in_] != '=' ) && is_base64( s[in_] ) )
        {
            char_array_4[i++] = s[in_];
            in_++;
            if( i == 4 )
            {
                for( i = 0; i < 4; i++ )
                    char_array_4[i] = base64_chars.find( char_array_4[i] );

                char_array_3[0] = ( char_array_4[0] << 2 ) + ( ( char_array_4[1] & 0x30 ) >> 4 );
                char_array_3[1] =
                    ( ( char_array_4[1] & 0xf ) << 4 ) + ( ( char_array_4[2] & 0x3c ) >> 2 );
                char_array_3[2] = ( ( char_array_4[2] & 0x3 ) << 6 ) + char_array_4[3];

                for( i = 0; ( i < 3 ); i++ )
                    ret += char_array_3[i];
                i = 0;
            }
        }

        if( i )
        {
            for( j = i; j < 4; j++ )
                char_array_4[j] = 0;

            for( j = 0; j < 4; j++ )
                char_array_4[j] = base64_chars.find( char_array_4[j] );

            char_array_3[0] = ( char_array_4[0] << 2 ) + ( ( char_array_4[1] & 0x30 ) >> 4 );
            char_array_3[1] = ( ( char_array_4[1] & 0xf ) << 4 ) + ( ( char_array_4[2] & 0x3c ) >> 2 );
            char_array_3[2] = ( ( char_array_4[2] & 0x3 ) << 6 ) + char_array_4[3];

            for( j = 0; ( j < i - 1 ); j++ )
                ret += char_array_3[j];
        }

        return ret;
#else
        using namespace boost::archive::iterators;

        std::stringstream os;

        using base64_dec = transform_width<binary_from_base64<const char *>, 8, 6>;

        auto size = s.size();

        // Remove the padding characters, cf. https://svn.boost.org/trac/boost/ticket/5629
        if( size && s[size - 1] == '=' )
        {
            --size;
            if( size && s[size - 1] == '=' )
            {
                --size;
            }
        }
        if( size == 0 )
        {
            return {};
        }

        std::copy( base64_dec( s.data() ), base64_dec( s.data() + size ),
                   std::ostream_iterator<char>( os ) );

        return os.str();
#endif
    }

    template <>
    auto StringUtility<std::string>::getUUID() -> std::string
    {
#if FB_USE_BOOST
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return to_string( uuid );
#else
        return "";
#endif
    }

    template <>
    auto StringUtility<StringW>::getUUID() -> StringW
    {
#if FB_USE_BOOST
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return to_wstring( uuid );
#else
        return L"";
#endif
    }

    template <class T>
    auto StringUtility<T>::toStringW( std::string str ) -> StringW
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes( str.c_str() );
    }

    template <class T>
    auto StringUtility<T>::toStringC( StringW str ) -> std::string
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes( str.c_str() );
    }

    template <class T>
    auto StringUtility<T>::parseStringVector( const T &val ) -> Array<T>
    {
        return StringUtility::split( val );
    }

    template <class T>
    auto StringUtility<T>::isNumber( const T &val ) -> bool
    {
        std::basic_stringstream<typename T::value_type, std::char_traits<typename T::value_type>,
                                std::allocator<typename T::value_type>>
            str( val );
        float tst;
        str >> tst;
        return !str.fail() && str.eof();
    }

    template <class T>
    void StringUtility<T>::splitFilename( const String &qualifiedName, String &outBasename,
                                          String &outPath )
    {
        String path = qualifiedName;
        // Replace \ with / first
        std::replace( path.begin(), path.end(), '\\', '/' );
        // split based on final /
        size_t i = path.find_last_of( '/' );

        if( i == String::npos )
        {
            outPath.clear();
            outBasename = qualifiedName;
        }
        else
        {
            outBasename = path.substr( i + 1, path.size() - i - 1 );
            outPath = path.substr( 0, i + 1 );
        }
    }

    template <class T>
    void StringUtility<T>::splitBaseFilename( const String &fullName, String &outBasename,
                                              String &outExtention )
    {
        size_t i = fullName.find_last_of( "." );
        if( i == String::npos )
        {
            outExtention.clear();
            outBasename = fullName;
        }
        else
        {
            outExtention = fullName.substr( i + 1 );
            outBasename = fullName.substr( 0, i );
        }
    }

    template <class T>
    void StringUtility<T>::splitFullFilename( const String &qualifiedName, String &outBasename,
                                              String &outExtention, String &outPath )
    {
        String fullName;
        splitFilename( qualifiedName, fullName, outPath );
        splitBaseFilename( fullName, outBasename, outExtention );
    }

    template <class T>
    auto StringUtility<T>::match( const String &str, const String &pattern, bool caseSensitive ) -> bool
    {
        String tmpStr = str;
        String tmpPattern = pattern;
        if( !caseSensitive )
        {
            tmpStr = StringUtil::make_lower( tmpStr );
            tmpPattern = StringUtil::make_lower( tmpPattern );
        }

        String::const_iterator strIt = tmpStr.begin();
        String::const_iterator patIt = tmpPattern.begin();
        String::const_iterator lastWildCardIt = tmpPattern.end();
        while( strIt != tmpStr.end() && patIt != tmpPattern.end() )
        {
            if( *patIt == '*' )
            {
                lastWildCardIt = patIt;
                // Skip over looking for next character
                ++patIt;
                if( patIt == tmpPattern.end() )
                {
                    // Skip right to the end since * matches the entire rest of the string
                    strIt = tmpStr.end();
                }
                else
                {
                    // scan until we find next pattern character
                    while( strIt != tmpStr.end() && *strIt != *patIt )
                    {
                        ++strIt;
                    }
                }
            }
            else
            {
                if( *patIt != *strIt )
                {
                    if( lastWildCardIt != tmpPattern.end() )
                    {
                        // The last wildcard can match this incorrect sequence
                        // rewind pattern to wildcard and keep searching
                        patIt = lastWildCardIt;
                        lastWildCardIt = tmpPattern.end();
                    }
                    else
                    {
                        // no wildwards left
                        return false;
                    }
                }
                else
                {
                    ++patIt;
                    ++strIt;
                }
            }
        }
        // If we reached the end of both the pattern and the string, we succeeded
        if( ( patIt == tmpPattern.end() || ( *patIt == '*' && patIt + 1 == tmpPattern.end() ) ) &&
            strIt == tmpStr.end() )
        {
            return true;
        }
        return false;
    }

    template <>
    auto StringUtility<String>::cleanupPath( const String &path ) -> String
    {
        if( path == "./" )
        {
            return ".";
        }

        std::vector<std::string> components;
        components.reserve( 10 );

        std::string component;

#ifdef FB_USE_BOOST
        boost::filesystem::path fspath( path );
        bool isAbsolute = fspath.is_absolute();
#else
        bool isAbsolute = false;
#endif

        for( const auto &c : path )
        {
            if( c == '/' || c == '\\' )
            {
                if( component == ".." )
                {
                    if( !components.empty() && components.back() != ".." )
                    {
                        components.pop_back();
                    }
                    else if( !isAbsolute )
                    {
                        components.emplace_back( ".." );
                    }
                }
                else if( component != "." && !component.empty() )
                {
                    components.push_back( component );
                }
                component.clear();
            }
            else
            {
                component.push_back( c );
            }
        }

        if( component == ".." )
        {
            if( !components.empty() && components.back() != ".." )
            {
                components.pop_back();
            }
            else if( !isAbsolute )
            {
                components.emplace_back( ".." );
            }
        }
        else if( component != "." && !component.empty() )
        {
            components.push_back( component );
        }

        std::string result;

        for( const auto &component : components )
        {
            if( !result.empty() && component != "../" && component != "..\\" )
            {
                result += '/';
            }
            else if( isAbsolute && result.empty() )
            {
#ifdef FB_PLATFORM_APPLE
                result += '/';
#endif
            }

            result += component;
        }

        return result.empty() ? "/" : result;
    }

    template <>
    auto StringUtility<StringW>::cleanupPath( const StringW &path ) -> StringW
    {
        if( path == L"./" )
        {
            return L".";
        }

        std::vector<std::wstring> components;
        std::wstring component;

#ifdef FB_USE_BOOST
        boost::filesystem::path fspath( path );
        bool isAbsolute = fspath.is_absolute();
#else
        bool isAbsolute = false;
#endif

        for( const auto &c : path )
        {
            if( c == L'/' || c == L'\\' )
            {
                if( component == L".." )
                {
                    if( !components.empty() && components.back() != L".." )
                    {
                        components.pop_back();
                    }
                    else if( !isAbsolute )
                    {
                        components.emplace_back( L".." );
                    }
                }
                else if( component != L"." && !component.empty() )
                {
                    components.push_back( component );
                }
                component.clear();
            }
            else
            {
                component.push_back( c );
            }
        }

        if( component == L".." )
        {
            if( !components.empty() && components.back() != L".." )
            {
                components.pop_back();
            }
            else if( !isAbsolute )
            {
                components.emplace_back( L".." );
            }
        }
        else if( component != L"." && !component.empty() )
        {
            components.push_back( component );
        }

        std::wstring result;

        for( const auto &component : components )
        {
            if( !result.empty() && component != L"../" && component != L"..\\" )
            {
                result += L'/';
            }
            else if( isAbsolute && result.empty() )
            {
#ifdef FB_PLATFORM_APPLE
                result += L'/';
#endif
            }

            result += component;
        }

        return result.empty() ? L"/" : result;
    }

    template <>
    void StringUtility<String>::toBuffer( const String &src, void *dst, size_t bufferSize )
    {
#if defined FB_PLATFORM_WIN32
        sprintf_s( static_cast<String::value_type *>( dst ), bufferSize, "%s", src.c_str() );
#else
        sprintf( (String::value_type *)dst, "%s", src.c_str() );
#endif
    }

    template <>
    void StringUtility<StringW>::toBuffer( const StringW &src, void *dst, size_t bufferSize )
    {
#if defined FB_PLATFORM_WIN32
        swprintf_s( static_cast<StringW::value_type *>( dst ), bufferSize, L"%s", src.c_str() );
#else
        swprintf( (StringW::value_type *)dst, bufferSize, L"%s", src.c_str() );
#endif
    }

    template <class T>
    auto fb::StringUtility<T>::extractNamedEntities( const std::vector<std::string> &tokens,
                                                     const std::vector<std::string> &entities )
        -> std::vector<std::string>
    {
        std::vector<std::string> namedEntities;
        for( const auto &token : tokens )
        {
            // Check if the token is a named entity (e.g., "home")
            for( const std::string &entity : entities )
            {
                if( token == entity )
                {
                    namedEntities.push_back( token );
                }
            }
        }

        return namedEntities;
    }

    template <class T>
    auto fb::StringUtility<T>::tokenize( const std::string &input ) -> std::vector<std::string>
    {
        std::vector<std::string> tokens;
        std::istringstream iss( input );
        std::string token;
        while( iss >> token )
        {
            tokens.push_back( token );
        }
        return tokens;
    }

    template <class T>
    auto fb::StringUtility<T>::countMatchingCharacters( const std::string &a, const std::string &b )
        -> int
    {
        int count = 0;
        for( char cB : b )
        {
            count += countMatchingCharacters( a, cB );
        }

        return count;
    }

    template <class T>
    auto fb::StringUtility<T>::countMatchingCharacters( const std::string &str, char target ) -> int
    {
        int count = 0;
        for( char c : str )
        {
            if( c == target )
            {
                count++;
            }
        }
        return count;
    }

    template <class T>
    auto fb::StringUtility<T>::numCommonSubsequence( const std::string &str1, const std::string &str2 )
        -> size_t
    {
        auto len1 = str1.length();
        auto len2 = str2.length();

        // Create a 2D DP table to store lengths of longest common subsequences
        std::vector<std::vector<int>> dp( len1 + 1, std::vector<int>( len2 + 1, 0 ) );

        // Fill the DP table
        for( int i = 1; i <= len1; ++i )
        {
            for( int j = 1; j <= len2; ++j )
            {
                if( str1[i - 1] == str2[j - 1] )
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max( dp[i - 1][j], dp[i][j - 1] );
                }
            }
        }

        // Reconstruct the longest common subsequence
        auto i = len1, j = len2;
        std::string result;
        while( i > 0 && j > 0 )
        {
            if( str1[i - 1] == str2[j - 1] )
            {
                result = str1[i - 1] + result;
                i--;
                j--;
            }
            else if( dp[i - 1][j] > dp[i][j - 1] )
            {
                i--;
            }
            else
            {
                j--;
            }
        }

        return result.size();
    }

    template <class T>
    auto fb::StringUtility<T>::longestCommonSubsequence( const std::string &str1,
                                                         const std::string &str2 ) -> std::string
    {
        auto len1 = str1.length();
        auto len2 = str2.length();

        // Create a 2D DP table to store lengths of longest common subsequences
        std::vector<std::vector<int>> dp( len1 + 1, std::vector<int>( len2 + 1, 0 ) );

        // Fill the DP table
        for( int i = 1; i <= len1; ++i )
        {
            for( int j = 1; j <= len2; ++j )
            {
                if( str1[i - 1] == str2[j - 1] )
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max( dp[i - 1][j], dp[i][j - 1] );
                }
            }
        }

        // Reconstruct the longest common subsequence
        auto i = len1, j = len2;
        std::string result;
        while( i > 0 && j > 0 )
        {
            if( str1[i - 1] == str2[j - 1] )
            {
                result = str1[i - 1] + result;
                i--;
                j--;
            }
            else if( dp[i - 1][j] > dp[i][j - 1] )
            {
                i--;
            }
            else
            {
                j--;
            }
        }

        return result;
    }

    template class StringUtility<std::string>;
    template std::string StringUtility<std::string>::toString<s32>( const Vector2I & );
    template std::string StringUtility<std::string>::toString<f32>( const Vector2F & );
    template std::string StringUtility<std::string>::toString<f64>( const Vector2D & );
    template std::string StringUtility<std::string>::toString<s32>( const Vector3I & );
    template std::string StringUtility<std::string>::toString<f32>( const Vector3F & );
    template std::string StringUtility<std::string>::toString<f64>( const Vector3D & );
    template std::string StringUtility<std::string>::toString<f32>( const QuaternionF & );
    template std::string StringUtility<std::string>::toString<f64>( const QuaternionD & );

    template Vector2I StringUtility<std::string>::parseVector2<s32>( const std::string &,
                                                                     const Vector2I & );
    template Vector2F StringUtility<std::string>::parseVector2<f32>( const std::string &,
                                                                     const Vector2F & );
    template Vector2D StringUtility<std::string>::parseVector2<f64>( const std::string &,
                                                                     const Vector2D & );

    template Vector3I StringUtility<std::string>::parseVector3<s32>( const std::string &,
                                                                     const Vector3I & );
    template Vector3F StringUtility<std::string>::parseVector3<f32>( const std::string &,
                                                                     const Vector3F & );
    template Vector3D StringUtility<std::string>::parseVector3<f64>( const std::string &,
                                                                     const Vector3D & );

    template Vector3I StringUtility<std::string>::parseVector3<s32>( const std::string &,
                                                                     const std::string &,
                                                                     const Vector3I & );
    template Vector3F StringUtility<std::string>::parseVector3<f32>( const std::string &,
                                                                     const std::string &,
                                                                     const Vector3F & );
    template Vector3D StringUtility<std::string>::parseVector3<f64>( const std::string &,
                                                                     const std::string &,
                                                                     const Vector3D & );

    template Vector4I StringUtility<std::string>::parseVector4<s32>( const std::string &,
                                                                     const Vector4I & );
    template Vector4F StringUtility<std::string>::parseVector4<f32>( const std::string &,
                                                                     const Vector4F & );
    template Vector4D StringUtility<std::string>::parseVector4<f64>( const std::string &,
                                                                     const Vector4D & );
    template Vector4I StringUtility<std::string>::parseVector4<s32>( const std::string &,
                                                                     const std::string &,
                                                                     const Vector4I & );
    template Vector4F StringUtility<std::string>::parseVector4<f32>( const std::string &,
                                                                     const std::string &,
                                                                     const Vector4F & );
    template Vector4D StringUtility<std::string>::parseVector4<f64>( const std::string &,
                                                                     const std::string &,
                                                                     const Vector4D & );

    template QuaternionF StringUtility<std::string>::parseQuaternion<f32>( const std::string &,
                                                                           const QuaternionF & );
    template QuaternionD StringUtility<std::string>::parseQuaternion<f64>( const std::string &,
                                                                           const QuaternionD & );

    template class StringUtility<StringW>;
}  // end namespace fb
