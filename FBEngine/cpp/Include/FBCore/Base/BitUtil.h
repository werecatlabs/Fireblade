#ifndef __BitUtil_H__
#define __BitUtil_H__

#include <FBCore/FBCoreTypes.h>

namespace fb
{

    /**
     * Class for manipulating bit patterns.
     */
    class BitUtil
    {
    public:
        /**
         * Determines whether the number is power-of-two or not.
         * @note 0 and 1 are tread as power of two.
         * @tparam T The type of the number.
         * @param n The number to test.
         * @return True if n is a power of two, false otherwise.
         */
        template <typename T>
        static bool isPO2( T n );

        //
        // basic operations
        //

        /**
         * Computes bitwise AND of two values.
         * @tparam T The type of the values.
         * @param val0 The first value.
         * @param val1 The second value.
         * @return The result of the AND operation.
         */
        template <class T>
        static T bit_and( T val0, T val1 );

        /**
         * Computes bitwise OR of two values.
         * @tparam T The type of the values.
         * @param val0 The first value.
         * @param val1 The second value.
         * @return The result of the OR operation.
         */
        template <class T>
        static T bit_or( T val0, T val1 );

        /**
         * Computes bitwise OR of three values.
         * @tparam T The type of the values.
         * @param val0 The first value.
         * @param val1 The second value.
         * @param val2 The third value.
         * @return The result of the OR operation.
         */
        template <class T>
        static T bit_or( T val0, T val1, T val2 );

        /**
         * Computes bitwise OR of four values.
         * @tparam T The type of the values.
         * @param val0 The first value.
         * @param val1 The second value.
         * @param val2 The third value.
         * @param val3 The fourth value.
         * @return The result of the OR operation.
         */
        template <class T>
        static T bit_or( T val0, T val1, T val2, T val3 );

        /**
         * Updates flags stored in the type and returns a value holding the updated flags.
         * @tparam T The type of the flags.
         * @param flags The flags to update.
         * @param flag The flag to update.
         * @param value The new value of the flag.
         * @return The value holding the updated flags.
         */
        template <class T>
        static T setFlagValue( T flags, T flag, bool value );

        /**
         * Gets the value of a flag.
         * @tparam T The type of the flags.
         * @param flags The flags to read.
         * @param flag The flag to read.
         * @return The value of the flag.
         */
        template <class T>
        static bool getFlagValue( T flags, T flag );

        /**
         * Converts fixed point value to float.
         * @param value The fixed point value.
         * @param bits The number of bits used to represent the fractional part.
         * @return The floating point value.
         */
        static f32 fixedToFloat( u32 value, u32 bits );

        /** Convert floating point color channel value between 0.0 and 1.0 (otherwise clamped)
         * to an integer of a certain number of bits. Works for any value of bits between 0 and 31.
         *
         * \param value The value to convert, between 0.0 and 1.0.
         * \param bits The number of bits to use in the output integer.
         * \return The converted integer.
         */
        static u32 floatToFixed( f32 value, u32 bits );

        /** Convert a float32 to a float16 (NV_half_float).
         * Courtesy of OpenEXR.
         *
         * \param i The float value to convert.
         * \return The converted float16 value.
         */
        static u16 floatToHalf( float i );

        /** Converts a float in u32 format to a half in u16 format.
         *
         * \param i The float value in u32 format.
         * \return The converted float value in u16 format.
         */
        static u16 floatToHalfI( u32 i );

        /**
         * Convert a float16 (NV_half_float) to a float32.
         * Courtesy of OpenEXR.
         *
         * \param y The float16 value to convert.
         * \return The converted float32 value.
         */
        static f32 halfToFloat( u16 y );

        /** Converts a half in u16 format to a float in u32 format.
         *
         * \param y The half value in u16 format.
         * \return The converted float value in u32 format.
         */
        static u32 halfToFloatI( u16 y );
    };

}  // end namespace fb

#include <FBCore/Base/BitUtil.inl>

#endif
