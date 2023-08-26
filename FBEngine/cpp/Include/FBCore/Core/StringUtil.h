#ifndef _FBStringUtil_H_
#define _FBStringUtil_H_

#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Core/ColourI.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    /**
     * @brief A template-based string utility class for converting and manipulating data types.
     *
     * @tparam T The data type to be used as the base string type in the utility class.
     */
    template <class T>
    class FBCore_API StringUtility
    {
    public:
        static const T EmptyString;

        /** @brief Returns the default delimiter.
         *  @return T The default delimiter.
         */
        static T default_dilim();

        /** @brief Returns an empty string of type T.
         *  @return T The empty string.
         */
        static T blank();

        /** @brief Compares two strings using lower case.
         *  @param a The first string to compare.
         *  @param b The second string to compare.
         *  @return bool True if the strings are equal, false otherwise.
         */
        static bool isEqual( const T &a, const T &b );

        /** @brief Checks if a string is null or is an empty string.
         *  @param str The string to check.
         *  @return bool True if the string is null or empty, false otherwise.
         */
        static bool isNullOrEmpty( const T &str );

        /** Checks if a string contains a string value. */
        static bool contains( const T &str, const T &value );

        /** Converts a boolean to a string. */
        static T toString( bool value );

        /** Converts a string to a boolean.
        @remarks
        Returns true if the strings value equals "true", "yes" or "1", false otherwise.
        This test is not case sensitive.
        */
        static bool parseBool( const T &value, bool defaultValue = true );

        /** Converts a s32 to a string. */
        static String toString( s32 value );

        /** Converts a s64 to a string. */
        static String toString( s64 value );

        /** Converts a string to an int. */
        static s32 parseInt( const String &value, s32 defaultValue = 0 );

        /** Converts a u32 to a string. */
        static String toString( u32 value );

#if defined FB_PLATFORM_WIN32
        /** Converts a u64 to a string. */
        static String toString( u64 value );

        /** Converts a u64 to a string. */
        static String toString( unsigned long int value );
#elif defined FB_PLATFORM_APPLE
        /** Converts a u64 to a string. */
        static String toString( u64 value );

        /** Converts a u64 to a string. */
        static String toString( unsigned long int value );
#elif defined FB_PLATFORM_LINUX
        /** Converts a u64 to a string. */
        static String toString( u64 value );
#endif

        /** Converts a string to an unsigned int. */
        static u32 parseUInt( const String &value, u32 defaultValue = 0 );

        /** Converts a f32 to a string. */
        static String toString( f32 value );

        /** Converts a string to a float. */
        static f32 parseFloat( const T &value, f32 defaultValue = 0.0f );

        /** Converts a f64 to a string. */
        static String toString( f64 value );

        /** Converts a string to a double. */
        static f64 parseDouble( const T &value, f64 defaultValue = 0.0 );

        /** Converts a 2d vector to a string. */
        template <class B>
        static T toString( const Vector2<B> &value );

        /** Converts a string to a 2d vector. */
        template <class B>
        static Vector2<B> parseVector2( const T &value,
                                        const Vector2<B> &defaultValue = Vector2<B>::zero() );

        /** Converts a 3d vector to a string. */
        template <class B>
        static T toString( const Vector3<B> &value );

        /** Converts a string to a 3d vector. */
        template <class B>
        static Vector3<B> parseVector3( const T &value,
                                        const Vector3<B> &defaultValue = Vector3<B>::zero() );

        /** Converts a string to a 3d vector. */
        template <class B>
        static Vector3<B> parseVector3( const T &val, const T &split,
                                        const Vector3<B> &defaultValue = Vector3<B>::zero() );

        /** Converts a 4d vector to a string. */
        template <class B>
        static T toString( const Vector4<B> &value );

        /** Converts a string to a 4d vector. */
        template <class B>
        static Vector4<B> parseVector4( const T &value,
                                        const Vector4<B> &defaultValue = Vector4<B>::zero() );

        /** Converts a string to a 4d vector. */
        template <class B>
        static Vector4<B> parseVector4( const T &val, const T &split,
                                        const Vector4<B> &defaultValue = Vector4<B>::zero() );

        /** Converts a 4d vector to a string. */
        template <class B>
        static T toString( const Quaternion<B> &value );

        /** Converts a string to a 4d vector. */
        template <class B>
        static Quaternion<B> parseQuaternion(
            const T &value, const Quaternion<B> &defaultValue = Quaternion<B>::identity() );

        /** Converts a boolean to a string. */
        static String toString( const ColourI &colour );

        /** Converts a boolean to a string. */
        static String toString( const ColourF &colour );

        /** Expects a string in an R,G,B,A format eg. 255,255,255,255. */
        static ColourI parseColour( const String &value );

        /** Expects a string in an R,G,B,A format. */
        static ColourF parseColourf( const String &value );

        /** Converts a formatted string to an Array of strings. */
        static void parseArray( const T &formatedString, Array<T> &stringArray );

        /** Converts a formatted string to an Array of floats. */
        static void parseArray( const T &formatedString, Array<f32> &floatArray );

        /** Converts an Array of strings to a formatted string. */
        static String toString( const Array<String> &stringArray );

        /** Converts an Array of floats to a formatted string. */
        static String toString( const Array<f32> &floatArray );

        /** Converts an Array of strings to a formatted string. */
        static String toString( const Array<String> &stringArray1, const Array<String> &stringArray2 );

        /** Converts a formatted string to an Array of strings. */
        static void parseMultiChoice( const String &formatedString, Array<String> &stringArray1,
                                      Array<String> &stringArray2 );

        /** Creates a hash value from the string. */
        static hash_type getHash( const String &str );

        /** Creates a hash value from the string. */
        static hash_type getHashMakeLower( const String &str );

        /** Creates a hash value from the string. */
        static hash32 getHash32( const String &str );

        /** Creates a hash value from the string. */
        static hash32 getHashMakeLower32( const String &str );

        /** Creates a hash value from the string. */
        static hash64 getHash64( const String &str );

        /** Creates a hash value from the string. */
        static hash64 getHashMakeLower64( const String &str );

        /** @brief Converts a narrow string to a wide string.
         *  @param str The narrow string to convert.
         *  @return StringW The converted wide string.
         */
        static StringW getWString( const String &str );

        /** @brief Converts a wide string to a narrow string.
         *  @param str The wide string to convert.
         *  @return String The converted narrow string.
         */
        static String toString( const StringW &str );

        /** @brief Trims whitespace from the start of a string.
         *  @param s The string to trim.
         *  @return String The trimmed string.
         */
        static String ltrim( const String &s );

        /** @brief Trims whitespace from the end of a string.
         *  @param s The string to trim.
         *  @return String The trimmed string.
         */
        static String rtrim( const String &s );

        /** @brief Trims whitespace from both ends of a string.
         *  @param s The string to trim.
         *  @return String The trimmed string.
         */
        static String trim( const String &s );

        /** Lower-cases all the characters in the string. */
        static String make_lower( const String &str );

        /** Upper-cases all the characters in the string. */
        static String make_upper( const String &str );

        /** @brief Replaces all occurrences of a character in a string with another character.
         *  @param str The string to perform the replacement on.
         *  @param toReplace The character to replace.
         *  @param replaceWith The character to replace with.
         *  @return String The modified string with characters replaced.
         */
        static String replace( const String &str, char toReplace, char replaceWith );

        /** @brief Gets the current date and time as a string.
         *  @return T The current date and time as a string.
         */
        static T getCurrentDateTime();

        /** @brief Gets the current date and time as a string, with an optional display format.
         *  @param bDisplayStr True to display the date and time in a human-readable format, false for a
         * machine-readable format.
         *  @return T The current date and time as a string.
         */
        static T getCurrentDateTime( bool bDisplayStr );

        /** @brief Gets the current date and time as a string, with optional display and SQLite formats.
         *  @param bDisplayStr True to display the date and time in a human-readable format, false for a
         * machine-readable format.
         *  @param sqlLiteFormat True to return the date and time in SQLite format, false for a standard
         * format.
         *  @return T The current date and time as a string.
         */
        static T getCurrentDateTime( bool bDisplayStr, bool sqlLiteFormat );

        /** @brief Gets the current time as a string.
         *  @return T The current time as a string.
         */
        static T getCurrentTime();

        /** @brief Gets the current time as a string, with an optional display format.
         *  @param bDisplayStr True to display the time in a human-readable format, false for a
         * machine-readable format.
         *  @return T The current time as a string.
         */
        static T getCurrentTime( bool bDisplayStr );

        /** @brief Converts a UTF-16 wide string to a UTF-8 narrow string.
         *  @param str The UTF-16 wide string to convert.
         *  @return String The converted UTF-8 narrow string.
         */
        static String toUTF16to8( const StringW &str );

        /** @brief Converts a UTF-8 narrow string to a UTF-16 wide string.
         *  @param str The UTF-8 narrow string to convert.
         *  @return StringW The converted UTF-16 wide string.
         */
        static StringW toUTF8to16( const String &str );

        /** @brief Splits a string into an array of substrings based on delimiters.
         *  @param str The string to split.
         *  @param delims The delimiters to split the string on (default is the result of
         * default_dilim()).
         *  @param maxSplits The maximum number of splits to perform (default is 0, which means no
         * limit).
         *  @param preserveDelims True to include the delimiters in the resulting array, false to discard
         * them (default is false).
         *  @return Array<T> An array of substrings resulting from the split operation.
         */
        static Array<T> split( const T &str, const T &delims = default_dilim(),
                               unsigned int maxSplits = 0, bool preserveDelims = false );

        /** @brief Replaces all occurrences of a substring in a string with another substring.
         *  @param source The original string.
         *  @param replaceWhat The substring to replace.
         *  @param replaceWithWhat The substring to replace with.
         *  @return T The modified string with all occurrences of replaceWhat replaced with
         * replaceWithWhat.
         */
        static const T replaceAll( const T &source, const T &replaceWhat, const T &replaceWithWhat );

        /** @brief Encodes a byte array into a Base64 string.
         *  @param bytes_to_encode The byte array to encode.
         *  @param in_len The length of the input byte array.
         *  @return String The encoded Base64 string.
         */
        static String encodeBase64( const unsigned char *bytes_to_encode, size_t in_len );

        /** @brief Decodes a Base64 string into a byte array.
         *  @param encoded_string The Base64 string to decode.
         *  @return String The decoded byte array.
         */
        static String decodeBase64( const String &encoded_string );

        /** @brief Generates a universally unique identifier (UUID) as a string.
         *  @return T The generated UUID as a string.
         */
        static T getUUID();

        /** @brief Converts a UTF-8 encoded string to a wide string.
         *  @param str The UTF-8 encoded string to be converted.
         *  @return StringW The resulting wide string.
         */
        static StringW toStringW( String str );

        /** @brief Converts a wide string to a UTF-8 encoded string.
         *  @param str The wide string to be converted.
         *  @return String The resulting UTF-8 encoded string.
         */
        static String toStringC( StringW str );

        /** Pareses a StringVector from a string.
        @remarks
            Strings must not contain spaces since space is used as a delimiter in
            the output.
        */
        static Array<T> parseStringVector( const T &val );

        /** Checks the String is a valid number value. */
        static bool isNumber( const T &val );

        /** @brief Splits a fully qualified file name into its base name and path.
         *  @param qualifiedName The fully qualified file name.
         *  @param[out] outBasename The base name of the file (output).
         *  @param[out] outPath The path to the file (output).
         */
        static void splitFilename( const String &qualifiedName, String &outBasename, String &outPath );

        /** @brief Splits a file name with extension into its base name and extension.
         *  @param fullName The full file name including the extension.
         *  @param[out] outBasename The base name of the file without extension (output).
         *  @param[out] outExtention The file extension (output).
         */
        static void splitBaseFilename( const String &fullName, String &outBasename,
                                       String &outExtention );

        /** @brief Splits a fully qualified file name into its base name, extension, and path.
         *  @param qualifiedName The fully qualified file name.
         *  @param[out] outBasename The base name of the file without extension (output).
         *  @param[out] outExtention The file extension (output).
         *  @param[out] outPath The path to the file (output).
         */
        static void splitFullFilename( const String &qualifiedName, String &outBasename,
                                       String &outExtention, String &outPath );

        /** @brief Checks if a string matches a given pattern with optional case sensitivity.
         *  @param str The string to check.
         *  @param pattern The pattern to match against.
         *  @param caseSensitive True for case-sensitive matching, false for case-insensitive matching
         * (default is true).
         *  @return bool True if the string matches the pattern, false otherwise.
         */
        static bool match( const String &str, const String &pattern, bool caseSensitive = true );

        /** @brief Cleans up a file path, resolving redundant separators and relative references.
         *  @param path The file path to clean up.
         *  @return T The cleaned-up file path.
         */
        static T cleanupPath( const T &path );

        /** @brief Copies the contents of a string into a specified buffer.
         *  @param src The source string to copy.
         *  @param[out] dst The destination buffer to copy the string into.
         *  @param bufferSize The size of the destination buffer.
         */
        static void toBuffer( const T &src, void *dst, size_t bufferSize );

        static std::string longestCommonSubsequence( const std::string &str1, const std::string &str2 );

        static size_t numCommonSubsequence( const std::string &str1, const std::string &str2 );

        static int countMatchingCharacters( const std::string &str, char target );

        static int countMatchingCharacters( const std::string &a, const std::string &b );

        // Tokenize the input sentence
        static std::vector<std::string> tokenize( const std::string &input );

        // Extract named entities using a simple keyword approach
        static std::vector<std::string> extractNamedEntities( const std::vector<std::string> &tokens,
                                                              const std::vector<std::string> &entities );
    };

    using StringUtil = StringUtility<String>;
    using StringUtilW = StringUtility<StringW>;

}  // end namespace fb

#endif
