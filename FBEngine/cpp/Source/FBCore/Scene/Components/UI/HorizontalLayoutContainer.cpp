#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/HorizontalLayoutContainer.h>

namespace fb
{
    namespace scene
    {

        HorizontalLayoutContainer::HorizontalLayoutContainer( int width ) : width( width )
        {
        }

        void HorizontalLayoutContainer::add_widget( const Widget &widget )
        {
            widgets.push_back( widget );
        }

        void HorizontalLayoutContainer::layout()
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
