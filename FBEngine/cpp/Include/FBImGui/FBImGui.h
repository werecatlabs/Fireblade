#ifndef __FBImGui_h__
#define __FBImGui_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace ui
    {
        class FBImGui : public SharedObject<ISharedObject>
        {
        public:
            FBImGui() = default;
            ~FBImGui() override = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            static SmartPtr<IUIManager> createUI();
        };
    } // namespace ui
}     // end namespace fb

#endif  // __FBImGui_h__
