#ifndef __IUIVector3_h__
#define __IUIVector3_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIVector3
         * @brief Interface for a 3D vector UI element, responsible for managing 3D vector elements in the user interface
         */
        class IUIVector3 : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIVector3() override = default;

            /**
             * @brief Gets the value of the 3D vector element
             * @return The value of the 3D vector element as a Vector3 object
             */
            virtual Vector3<real_Num> getValue() const = 0;

            /**
             * @brief Sets the value of the 3D vector element
             * @param value The new value for the 3D vector element as a Vector3 object
             */
            virtual void setValue( const Vector3<real_Num> &value ) = 0;

            /**
             * @brief Gets the label text associated with the 3D vector element
             * @return The label text as a string
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label text associated with the 3D vector element
             * @param label The new label text for the 3D vector element
             */
            virtual void setLabel( const String &label ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // __IUIVector3_h__
