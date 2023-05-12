#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/FBRenderUI.h>
#include <FBRenderUI/CUIManager.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    namespace ui
    {
        SmartPtr<IUIManager> FBRenderUI::createUIManager()
        {
            return fb::make_ptr<CUIManager>();
        }
    } // end namespace ui
}     // end namespace fb
