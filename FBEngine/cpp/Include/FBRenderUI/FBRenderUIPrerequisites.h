#ifndef FBRenderUIPrerequisites_h__
#define FBRenderUIPrerequisites_h__

#include <FBCore/FBCorePrerequisites.h>

//
// forward declarations
//

namespace Ogre
{
    class Overlay;
    class OverlayElement;
    class OverlayContainer;
}; // namespace Ogre


class TiXmlNode;

#ifndef _FB_STATIC_LIB_
#    ifdef FB_EXPORTS
#        define FBGui_API __declspec( dllexport )
#    else
#        define FBGui_API __declspec( dllimport )
#    endif  // FB_EXPORT
#else
#    define FBGui_API
#endif  // _FB_STATIC_LIB_

namespace fb
{
    namespace ui
    {
        class GameEventReceiver;

        // forward declarations
        class IGUIMenuItemListener;
        class IUIMenu;
        class CUIImage;
        class CUIToggleButton;
        class IGUIMenuItemListener;
        class GameInput;

        class DataManager;
        class RenderManager;
    } // end namespace ui
}     // end namespace fb

#endif  // FBRenderUIPrerequisites_h__
