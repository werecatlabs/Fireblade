#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/FBRenderUI.h>
#include <FBRenderUI/CUIManager.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb::ui
{
    auto FBRenderUI::createUIManager() -> SmartPtr<IUIManager>
    {
        return fb::make_ptr<CUIManager>();
    }
}  // namespace fb::ui
