#ifndef TableLayout_h__
#define TableLayout_h__

#include <FBCore/Scene/Components/UI/LayoutContainer.h>

namespace fb
{
    namespace scene
    {
        class TableLayout : public LayoutContainer
        {
        public:
            TableLayout( int width );
            void add_widget( const Widget &widget );
            void layout();
            int width;
            std::vector<Widget> widgets;
        };

    }  // namespace scene
}  // namespace fb

#endif  // TableLayout_h__
