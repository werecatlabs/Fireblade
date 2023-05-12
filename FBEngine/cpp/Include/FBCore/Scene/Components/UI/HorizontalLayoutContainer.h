#ifndef HorizontalLayoutContainer_h__
#define HorizontalLayoutContainer_h__

#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb
{
    namespace scene
    {

        class HorizontalLayoutContainer : public LayoutContainer
        {
        public:
            HorizontalLayoutContainer( int width );

            void add_widget( const Widget &widget );

            void layout();

            int width;
            std::vector<Widget> widgets;
        };

    }  // namespace scene
}  // namespace fb

#endif  // HorizontalLayoutContainer_h__
