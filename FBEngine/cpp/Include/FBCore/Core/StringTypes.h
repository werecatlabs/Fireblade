#ifndef StringTypes_h__
#define StringTypes_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/Array.h>
#include <string>
#include <map>
#include <vector>
#include <boost/beast/core/static_string.hpp>

namespace fb
{
#if 0
    // Define a custom allocator class
    template <typename T>
    struct StringAllocator
    {
        using value_type = T;

        StringAllocator() noexcept
        {
        }

        template <typename U>
        StringAllocator( const StringAllocator<U> & ) noexcept
        {
        }

        T *allocate( std::size_t n )
        {
            if( n > std::size_t( -1 ) / sizeof( T ) )
                throw std::bad_alloc();

            return static_cast<T *>( ::operator new( n * sizeof( T ) ) );
        }

        void deallocate( T *p, std::size_t ) noexcept
        {
            ::operator delete( p );
        }
    };

    using String = std::basic_string<c8, std::char_traits<c8>, StringAllocator<c8>>;
    using StringC = std::basic_string<c8, std::char_traits<c8>, StringAllocator<c8>>;
    using StringW = std::basic_string<wchar_t, std::char_traits<wchar_t>, StringAllocator<wchar_t>>;
#else
    using String = std::string;
    using StringC = std::string;
    using StringW = std::wstring;
#endif

    template <std::size_t N, class CharT = char, class Traits = std::char_traits<CharT>>
    using FixedString = boost::beast::static_string<N, CharT, Traits>;

    using NameValuePairList = std::map<String, String>;

    using string_pair = std::pair<std::string, std::string>;
    using string_pairs = Array<string_pair>;

    typedef Array<std::string> strings;
}  // end namespace fb

#endif  // StringTypes_h__
