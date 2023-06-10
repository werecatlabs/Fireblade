#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/HorizontalLayout.h>

namespace fb
{
    namespace scene
    {

        HorizontalLayout::HorizontalLayout( int width ) : width( width )
        {
        }

        void HorizontalLayout::add_widget( const Widget &widget )
        {
            widgets.push_back( widget );
        }

        void HorizontalLayout::layout()
        {
            int x = 0, y = 0;
            for( const auto &widget : widgets )
            {
                //widget.x = x;
                //widget.y = y;
                y += widget.height;
            }
        }

    }  // namespace scene
}  // namespace fb
