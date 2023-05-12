#ifndef FBRenderUI_h__
#define FBRenderUI_h__

#include <FBCore/Memory/SmartPtr.h>

#if FB_USE_AUTO_LINK
#    pragma comment( lib, "FBRenderUI.lib" )
#endif

namespace fb
{
    namespace ui
    {
        // forward declaration
        class IUIManager;

        class FBRenderUI
        {
        public:
            static SmartPtr<IUIManager> createUIManager();
        };
    } // namespace ui
}     // end namespace fb

#endif  // FBRenderUI_h__
