#ifndef IUIVector4_h__
#define IUIVector4_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Math/Vector4.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIVector4
         * @brief Interface for a 4D vector UI element, responsible for managing 4D vector elements in the user interface
         */
        class IUIVector4 : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIVector4() override = default;

            /**
             * @brief Gets the value of the 4D vector element
             * @return The value of the 4D vector element as a Vector4 object
             */
            virtual Vector4<real_Num> getValue() const = 0;

            /**
             * @brief Sets the value of the 4D vector element
             * @param value The new value for the 4D vector element as a Vector4 object
             */
            virtual void setValue( const Vector4<real_Num> &value ) = 0;

            /**
             * @brief Gets the label text associated with the 4D vector element
             * @return The label text as a string
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label text associated with the 4D vector element
             * @param label The new label text for the 4D vector element
             */
            virtual void setLabel( const String &label ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIVector4_h__
