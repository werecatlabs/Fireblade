#ifndef IUIPropertyGrid_h__
#define IUIPropertyGrid_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIPropertyGrid
         * @brief Interface for a UI Property Grid, extending IUIElement and providing functionality to manage and display properties
         */
        class IUIPropertyGrid : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIPropertyGrid() override = default;

            /**
             * @brief Gets the data as a Properties object
             * @return A shared pointer to the Properties object
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /**
             * @brief Sets the data as a Properties object
             * @param properties A shared pointer to the Properties object to be associated with the grid
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIPropertyGrid_h__
