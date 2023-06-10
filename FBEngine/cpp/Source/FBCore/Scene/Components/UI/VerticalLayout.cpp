#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/VerticalLayout.h>

namespace fb
{
    namespace scene
    {

        VerticalLayout::VerticalLayout( int width ) : width( width )
        {
        }

        void VerticalLayout::add_widget( const Widget &widget )
        {
            widgets.push_back( widget );
        }

        void VerticalLayout::layout()
        {
            int x = 0, y = 0;
            for( const auto &widget : widgets )
            {
                y += widget.height;
            }
        }

    }  // namespace scene
}  // namespace fb
