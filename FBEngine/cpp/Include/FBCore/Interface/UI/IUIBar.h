#ifndef _IGUIBar_H
#define _IGUIBar_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class IUIBar : public IUIElement
        {
        public:
            enum class BarOrientation
            {
                BO_HORIZONTAL,
                BO_VERTICAL,

                BO_COUNT
            };

            /** Virtual destructor. */
            ~IUIBar() override = default;

            /** The amount of points the bar has
            this represented as a whole number.
            */
            virtual void setPoints( f32 points ) = 0;

            /** Set the maximum amount of points to the bar can contain
            e.g. a player might have 250 health points.
            */
            virtual void setMaxPoints( f32 maxPoints ) = 0;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
