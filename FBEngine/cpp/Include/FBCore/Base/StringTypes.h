#ifndef StringTypes_h__
#define StringTypes_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Base/Array.h>
#include <string>
#include <map>
#include <vector>
#include <boost/beast/core/static_string.hpp>

namespace fb
{
    using String = std::string;
    using StringC = std::string;
    using StringW = std::wstring;

    template <std::size_t N, class CharT = char, class Traits = std::char_traits<CharT>>
    using FixedString = boost::beast::static_string<N, CharT, Traits>;

    using NameValuePairList = std::map<String, String>;

    using string_pair = std::pair<std::string, std::string>;
    using string_pairs = Array<string_pair>;

    typedef Array<std::string> strings;
}  // end namespace fb

#endif  // StringTypes_h__
