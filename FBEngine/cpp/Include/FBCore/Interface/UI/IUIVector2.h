#ifndef IUIVector2_h__
#define IUIVector2_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIVector2
         * @brief Interface for a 2D vector UI element, responsible for managing 2D vector elements in the user interface
         */
        class IUIVector2 : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIVector2() override = default;

            /**
             * @brief Gets the value of the 2D vector element
             * @return The value of the 2D vector element as a Vector2 object
             */
            virtual Vector2<real_Num> getValue() const = 0;

            /**
             * @brief Sets the value of the 2D vector element
             * @param value The new value for the 2D vector element as a Vector2 object
             */
            virtual void setValue( const Vector2<real_Num> &value ) = 0;

            /**
             * @brief Gets the label text associated with the 2D vector element
             * @return The label text as a string
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label text associated with the 2D vector element
             * @param label The new label text for the 2D vector element
             */
            virtual void setLabel( const String &label ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIVector2_h__
