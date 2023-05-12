#ifndef __IUIRenderWindow_h__
#define __IUIRenderWindow_h__

#include <FBCore/Interface/UI/IUIWindow.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIRenderWindow
         * @brief Interface for a UI Render Window, extending IUIWindow and providing additional functionality for rendering
         */
        class IUIRenderWindow : public IUIWindow
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIRenderWindow() override = default;

            /**
             * @brief Gets the associated render window
             * @return A shared pointer to the render::IWindow object
             */
            virtual SmartPtr<render::IWindow> getWindow() const = 0;

            /**
             * @brief Sets the associated render window
             * @param window A shared pointer to the render::IWindow object
             */
            virtual void setWindow( SmartPtr<render::IWindow> window ) = 0;

            /**
             * @brief Gets the associated render texture
             * @return A shared pointer to the render::ITexture object
             */
            virtual SmartPtr<render::ITexture> getRenderTexture() const = 0;

            /**
             * @brief Sets the associated render texture
             * @param renderTexture A shared pointer to the render::ITexture object
             */
            virtual void setRenderTexture( SmartPtr<render::ITexture> renderTexture ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIWindow_h__
