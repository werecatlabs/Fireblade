#ifndef ISystemManager_h__
#define ISystemManager_h__

#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    class ISystemManager : public scene::IComponent
    {
    public:
        /** Virtual destructor. */
        ~ISystemManager() override = default;

        virtual void setFullScreen( bool isFullScreen ) = 0;
        virtual bool isFullScreen() const = 0;
        
        virtual u32 getTargetRate() const = 0;
        virtual void setTargetRate( u32 fps ) = 0;

        virtual bool isWindowActive() const = 0;
        virtual void setWindowActive( bool val ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ISystemManager_h__
