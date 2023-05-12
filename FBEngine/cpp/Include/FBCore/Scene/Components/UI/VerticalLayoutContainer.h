#ifndef VerticalLayoutContainer_h__
#define VerticalLayoutContainer_h__

#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb
{
    namespace scene
    {

        class VerticalLayoutContainer : public LayoutContainer
        {
        public:
            VerticalLayoutContainer( int width );

            void add_widget( const Widget &widget );

            void layout();

            int width;
            std::vector<Widget> widgets;
        };

    }  // namespace scene
}  // namespace fb

#endif  // VerticalLayoutContainer_h__
