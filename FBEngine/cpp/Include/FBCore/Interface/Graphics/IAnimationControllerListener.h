#ifndef IAnimationControllerListener_h__
#define IAnimationControllerListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace render
    {
        
        class IAnimationControllerListener : public ISharedObject
        {
        public:
            ~IAnimationControllerListener() override = default;

            virtual void OnAnimationEnd( const String &name ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IAnimationControllerListener_h__
