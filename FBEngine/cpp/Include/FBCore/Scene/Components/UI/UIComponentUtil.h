#ifndef UIComponentUtil_h__
#define UIComponentUtil_h__

#include <FBCore/Scene/Components/BaseComponent.h>
#include <FBCore/Scene/Components/UI/CanvasTransform.h>

namespace fb
{
    namespace scene
    {

        class UIComponentUtil
        {
        public:
            static String getHorizontalAlignmentString(
                CanvasTransform::HorizontalAlignment horizontalAlignment );
            static CanvasTransform::HorizontalAlignment getHorizontalAlignment( const String &str );
            static String getHorizontalAlignmentTypesString();

            static String getVerticalAlignmentString(
                CanvasTransform::VerticalAlignment verticalAlignment );
            static CanvasTransform::VerticalAlignment getVerticalAlignment( const String &str );
            static String getVerticalAlignmentTypesString();
        };
    }  // namespace scene
}  // end namespace fb

#endif  // UIComponentUtil_h__
