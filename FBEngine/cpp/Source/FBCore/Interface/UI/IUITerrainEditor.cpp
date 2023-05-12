#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUITerrainEditor.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUITerrainEditor, IUIElement );

        const hash_type IUITerrainEditor::selectTerrainTextureHash =
            StringUtil::getHash( "selectTerrainTexture" );

    }  // end namespace ui
}  // end namespace fb
