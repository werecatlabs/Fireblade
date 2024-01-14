#ifndef UIComponentUtil_h__
#define UIComponentUtil_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>

namespace fb
{
    namespace scene
    {

        class UIComponentUtil
        {
        public:
            static String getHorizontalAlignmentString(
                LayoutTransform::HorizontalAlignment horizontalAlignment );
            static LayoutTransform::HorizontalAlignment getHorizontalAlignment( const String &str );
            static String getHorizontalAlignmentTypesString();

            static String getVerticalAlignmentString(
                LayoutTransform::VerticalAlignment verticalAlignment );
            static LayoutTransform::VerticalAlignment getVerticalAlignment( const String &str );
            static String getVerticalAlignmentTypesString();
        };

    }  // namespace scene
}  // end namespace fb

#endif  // UIComponentUtil_h__
