#ifndef IGraphicsSystemConfiguration_h__
#define IGraphicsSystemConfiguration_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        /** Interface for graphics settings. */
        class IGraphicsSettings : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IGraphicsSettings() override = default;

            /** Gets a boolean to know if to create a window. */
            virtual bool getCreateWindow() const = 0;

            /** Sets a boolean to know if to create a window. */
            virtual void setCreateWindow( bool createWindow ) = 0;

            /** Gets a boolean to know if to show the configuration dialog. */
            virtual bool getShowDialog() const = 0;

            /** Sets a boolean to know if to show the configuration dialog. */
            virtual void setShowDialog( bool showDialog ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace render
}  // end namespace fb

#endif  // IGraphicsSystemConfiguration_h__
