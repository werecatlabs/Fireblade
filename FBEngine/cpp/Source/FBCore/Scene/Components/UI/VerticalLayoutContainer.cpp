#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/VerticalLayoutContainer.h>

namespace fb
{
    namespace scene
    {

        VerticalLayoutContainer::VerticalLayoutContainer( int width ) : width( width )
        {
        }

        void VerticalLayoutContainer::add_widget( const Widget &widget )
        {
            widgets.push_back( widget );
        }

        void VerticalLayoutContainer::layout()
        {
            int x = 0, y = 0;
            for( const auto &widget : widgets )
            {
                y += widget.height;
            }
        }

    }  // namespace scene
}  // namespace fb
