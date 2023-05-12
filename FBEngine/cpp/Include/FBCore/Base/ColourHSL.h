#ifndef ColourHSL_h__
#define ColourHSL_h__

#include <FBCore/Base/ColourI.h>
#include <FBCore/Math/Math.h>

namespace fb
{
    /**
     * @brief Class representing a color in the HSL color space.
     * @tparam T Type of the HSL components. Usually a floating-point type.
     */
    template <class T>
    class ColourHSL
    {
    public:
        /**
         * @brief Default constructor. Initializes all components to zero.
         */
        ColourHSL() = default;

        /**
         * @brief Constructs a color from hue, saturation and luminance.
         * @param h Hue component of the color, in the range [0, 360).
         * @param s Saturation component of the color, in the range [0, 1].
         * @param l Luminance component of the color, in the range [0, 1].
         */
        ColourHSL( T h, T s, T l );

        /**
         * @brief Default destructor.
         */
        ~ColourHSL() = default;

        /**
         * @brief Sets the color from an RGB color.
         * @param color The RGB color to set the HSL color from.
         */
        void setfromRGB( const ColourI &color );

        /**
         * @brief Sets an RGB color from this HSL color.
         * @param color The RGB color to set from this HSL color.
         */
        void settoRGB( ColourI &color ) const;

        /**
         * @brief Gets the hue component of the color.
         * @return The hue component of the color, in the range [0, 360).
         */
        T getHue() const;

        /**
         * @brief Sets the hue component of the color.
         * @param hue The hue component of the color, in the range [0, 360).
         */
        void setHue( T hue );

        /**
         * @brief Gets the saturation component of the color.
         * @return The saturation component of the color, in the range [0, 1].
         */
        T getSaturation() const;

        /**
         * @brief Sets the saturation component of the color.
         * @param saturation The saturation component of the color, in the range [0, 1].
         */
        void setSaturation( T saturation );

        /**
         * @brief Gets the luminance component of the color.
         * @return The luminance component of the color, in the range [0, 1].
         */
        T getLuminance() const;

        /**
         * @brief Sets the luminance component of the color.
         * @param luminance The luminance component of the color, in the range [0, 1].
         */
        void setLuminance( T luminance );

        /** Returns true as there are no known conditions that would make the ColourHSL object invalid. */
        bool isValid() const;

    private:
        /**
         * @brief Converts a value from the HSL color space to the RGB color space.
         * @param rm1 The first component of the RGB color.
         * @param rm2 The second component of the RGB color.
         * @param rh The hue component of the color.
         * @return The value converted to the RGB color space.
         */
        u32 toRGB1( T rm1, T rm2, T rh ) const;

        T m_hue = T( 0.0 );         //!< Hue component of the color.
        T m_saturation = T( 0.0 );  //!< Saturation component of the color.
        T m_luminance = T( 0.0 );   //!< Luminance component of the color.
    };

}  // end namespace fb

#include <FBCore/Base/ColourHSL.inl>

#endif  // ColourHSL_h__
