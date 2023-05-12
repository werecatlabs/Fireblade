#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/TableLayout.h>

namespace fb
{
    namespace scene
    {

        TableLayout::TableLayout( int width ) : width( width )
        {
        }

        void TableLayout::add_widget( const Widget &widget )
        {
            widgets.push_back( widget );
        }

        void TableLayout::layout()
        {
            int x = 0, y = 0;
            for( const auto &widget : widgets )
            {
                y += widget.height;
            }
        }

    }  // namespace scene
}  // namespace fb
