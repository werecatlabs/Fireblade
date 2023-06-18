#ifndef LayoutContainer_h__
#define LayoutContainer_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {

        class Widget
        {
        public:
            Widget( int w, int h ) : width( w ), height( h )
            {
            }

            int x;
            int y;
            int width;
            int height;
        };

        class LayoutContainer : public UIComponent
        {
        public:
        };

    }  // namespace scene
}  // namespace fb

#endif  // LayoutContainer_h__
